#ifndef _SQUE_FILESYSTEM_
#define _SQUE_FILESYSTEM_

#include <vector>
#include "sque_utils.h"
#include "sque_timer.h"
#include <string>
#include <fstream>

static char exec_path[512];

#if defined(__linux__) || defined(ANDROID)
#	include <unistd.h>
#	include <sys/stat.h>
#	include <libgen.h>
#endif

#if defined(_WIN32)
//#define WIN32_LEAN_AND_MEAN // Just in case
//#define NOGDI
//#	include <Windows.h>
//#	include <libloaderapi.h>
#endif

struct SQUE_Dir
{
	uint32_t id = UINT32_MAX;
	char name[64];
	char location[512];

	uint32_t parent_id = -1;
	std::vector<uint32_t> children_ids;
};

SQUE_Dir SQUE_FS_GenDir(const char* location, const uint32_t parent_id = -1);

struct SQUE_Asset
{
	uint64_t size;
	char* raw_data;
};

struct SQUE_DataPack
{
	SQUE_Asset data;
	SQUE_Asset metadata;
};

typedef uint32_t(HandleNewAssetLocation)(const char* location);
typedef void(ReadWriteAssetFun)(const char* location, SQUE_DataPack* datapack);
typedef void(UnloadAssetFun)(SQUE_DataPack* datapack);

enum AssetStatus
{
	SQ_AS_NEUTRAL = BITSET1,
	SQ_AS_CHANGED = BITSET2,
	SQ_AS_DELETED = BITSET3,
	SQ_AS_LOADED = BITSET4,
	//...
};

struct SQUE_CtrlAsset
{
	// Static Data - Generated or Loaded
	uint32_t id = -1;
	char name[64] = "";
	char location[512] = "";
	uint32_t dir_id = UINT32_MAX;
	uint32_t type = -1; //?
	
	// Runtime Updates
	SQUE_Timer unused_timer;
	uint32_t current_users = 0;
	uint8_t status_flags = NULL; // refer to AssetStatus
	double last_update = 0;


	// Type Based functions  
	ReadWriteAssetFun* Save;
	ReadWriteAssetFun* Load;
	UnloadAssetFun* Unload;

	// Actual Data
	SQUE_DataPack datapack;
};

static void SQUE_FS_Init(const char* working_dir)
{
	memcpy(exec_path, working_dir, strlen(working_dir));
}

static const char* SQUE_FS_GetExecPath()
{
	return exec_path;
}

static bool SQUE_FS_CreateDirFullPath(const char* path)
{
#if defined(_WIN32)
	// System calls are dodgy, you would not like a console randomly popup...
	char temp[512];
	sprintf(temp, "mkdir -p %s", path);
	system(temp);	
	//ret = CreateDirectoryA(path, NULL);
#elif defined(ANDROID) || defined(__linux__)
	ret = (mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) > 0);
#endif
	return true;
}

// Path should be written without anything before, just if you want to do things out of directory
static bool SQUE_FS_CreateDirRelative(const char* path)
{
	bool ret = true;
	std::string exec_path = std::string(SQUE_FS_GetExecPath()) + FOLDER_ENDING + path;
	
	SQUE_FS_CreateDirFullPath(exec_path.c_str());

	return ret;
}

static SQUE_Asset* SQUE_FS_LoadFileRaw(const char* file)
{
	
	char* data = NULL;
	std::ifstream in;
	in.open(file, std::ifstream::binary);
	if(!in) return NULL;
	in.seekg(0, std::ios::end);
	int len = in.tellg();
	in.seekg(0, std::ios::beg);
	data = new char[len+1];
	in.read((char*)data, len);
	in.close();

	SQUE_Asset* ret = new SQUE_Asset();
	ret->size = len+1;
	ret->raw_data = data;
	memcpy(&ret->raw_data[len], "\0", 1);
	
	return ret;
}

static bool SQUE_FS_WriteFileRaw(const char* path, char* data, const uint64_t size)
{
	std::ofstream write_file;
	write_file.open(path, std::ofstream::binary);
	write_file.write(data, size);
	write_file.close();

	return true;
}

// RAW ASSET READING / LOADING

static SQUE_Dir* SQUE_FS_CreateBaseDirTree()
{
	SQUE_Dir* ret = new SQUE_Dir();
#if defined(ANDROID)
	/*
	memcpy(ret->name, "raw", 4);
	//ret->name = "raw";
	ret->native_dir_data = (char*)AAssetManager_openDir(my_app->activity->assetManager, ret->name);
	int dirs = 1;
	while (dirs)
	{
		const char* name = AAssetDir_getNextFileName((AAssetDir*)ret->native_dir_data);
		SQUE_PRINT(LT_INFO, " %s", name);
		if (name == NULL)
			break;
		else
		{
			AAssetDir* dir = AAssetManager_openDir(my_app->activity->assetManager, name);
			if (dir != NULL)
			{
				SQUE_Dir* branch = new SQUE_Dir();
				memcpy(branch->name, name, strlen(name));
				//branch->name = name;
				branch->native_dir_data = (char*)branch;
				branch->parent = ret;
				ret->children.push_back(branch);
			}
			else
				continue;
		}
	}
	*/
#else
	std::string main_path = SQUE_FS_GetExecPath();
	memcpy(ret->name, main_path.c_str(), main_path.length() + 1);

#endif
	return ret;
}

static SQUE_Asset* SQUE_FS_LoadAssetRaw(const char* file)
{
	SQUE_Asset* ret = NULL;
#if defined(ANDROID)
	ret = new SQUE_Asset();
	AAsset* asset = AAssetManager_open(my_app->activity->assetManager, file, AASSET_MODE_BUFFER);
	SQUE_PRINT(LT_INFO, "%d %s", asset, file);
	if(asset != NULL)
	{
		ret->size = AAsset_getLength(asset);
		char* tmp = new char[ret->size+1];
		memcpy(tmp, AAsset_getBuffer(asset), ret->size);
		tmp[ret->size] = '\0';
		ret->raw_data = tmp;
		AAsset_close(asset);
	}
	else
	{
		delete ret;
		ret = NULL;
	}
#else
	char* sprint_v = new char[256];
	int len = sprintf(sprint_v, "%s/Assets/%s\0", SQUE_FS_GetExecPath(), file);
	if (len > 256)
	{
		delete sprint_v;
		sprint_v = new char[len];
		sprintf(sprint_v, "%s/Assets/%s\0", SQUE_FS_GetExecPath(), file);
	}
	ret = SQUE_FS_LoadFileRaw(sprint_v);
	delete sprint_v;
#endif

	return ret;
}

static const char* SQUE_FS_GetFileName(const char* file)
{
	const char* ret1 = strrchr(file, '/');
	if (ret1 != NULL)
		return ret1;
	const char* ret2 = strrchr(file, '\\');
	if (ret2 != NULL)
		return ret2;
	return file;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FILEWATCHER //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <filesystem>

enum SQUE_FS_Status
{
	SQFS_UNKNOW = 0,
	SQFS_NEW,
	SQFS_CHANGED,
	SQFS_DELETED,
};

static uint32_t SQUE_FS_GetParentDir(const char* path)
{
	uint32_t ret = strlen(path);
	const char* fw_slash = strrchr(path, '/');
	if(fw_slash != NULL)
		return ret -= strlen(fw_slash);

	const char* d_slash = strrchr(path, '\\');
	return ret -= (strlen(d_slash));
}

#include "pcg/pcg_basic.h"

static SQUE_Dir SQUE_FS_GenDir(const char* location, const uint32_t parent_id)
{
	SQUE_Dir d;
	d.id = pcg32_random();
	const char* tmp = strrchr(location, '.');
	if (tmp != NULL)
	{
		uint32_t l = SQUE_FS_GetParentDir(location);
		l = (l < sizeof(d.location)) ? l : sizeof(d.location);
		memcpy(d.location, location, l);
	}
	else
		memcpy(d.location, location, sizeof(d.location));

	memcpy(d.name, SQUE_FS_GetFileName(d.location), sizeof(d.name));

	return d;
}



static uint32_t AddToParent(const uint32_t id, const char* location, std::vector<SQUE_Dir>& dirs)
{
	uint32_t len_dir = SQUE_FS_GetParentDir(location);
	char* tmp = new char[len_dir+1];
	memcpy(tmp, location, len_dir);
	tmp[len_dir] = '\0';
	for (uint16_t i = 0; i < dirs.size(); ++i)
	{
		if (strcmp(dirs[i].location, tmp) == 0)
		{
			dirs[i].children_ids.push_back(id);
			delete tmp;
			return dirs[i].id;
		}
	}
	delete tmp;
	return -1;
}

#include <filesystem>

static void SQUE_FS_GenDirectoryStructure(const char* location, std::vector<SQUE_Dir>* dirs)
{
	if(!std::filesystem::exists(location))
		return;
	dirs->push_back(SQUE_FS_GenDir(location));
	std::vector<SQUE_Dir>& dirs_v = *dirs;
	for (auto& file : std::filesystem::recursive_directory_iterator(location))
	{
		if (file.is_directory())
		{
			dirs->push_back(SQUE_FS_GenDir(file.path().string().c_str()));
			(*dirs)[dirs->size()-1].parent_id = AddToParent((*dirs)[dirs->size()-1].id, (*dirs)[dirs->size()-1].location, *dirs);
		}
	}
}

class SQUE_FW_NewAsset
{
public:
	SQUE_FW_NewAsset()
	{
		//str = new char[512];
		str[511] = '\0';
	};
	~SQUE_FW_NewAsset()
	{
		//delete[] str;
	};

	double last_update = 0;
	uint32_t str_len = 0;
	char str[512];
};

static std::vector<SQUE_FW_NewAsset*> SQUE_FS_CheckDirectoryChanges(const char* path, const std::vector<SQUE_CtrlAsset*>& assets_in_dir, HandleNewAssetLocation* handle_fun)
{
	for (uint32_t i = 0; i < assets_in_dir.size(); ++i)
	{
		assets_in_dir[i]->status_flags = 0;
		if (!std::filesystem::exists(assets_in_dir[i]->location))
			SET_FLAG(assets_in_dir[i]->status_flags, SQ_AS_DELETED);
	}
	std::vector<SQUE_FW_NewAsset*> new_items;
	for (auto& file : std::filesystem::recursive_directory_iterator(path))
	{
		// string hashing for ids... the MD5 old one might be great for that, it was decently fast
		uint32_t i;
		for (i = 0; i < assets_in_dir.size(); ++i)
		{
			if (CHK_FLAG(assets_in_dir[i]->status_flags,SQ_AS_DELETED)) continue;
			if (strcmp(assets_in_dir[i]->location, file.path().string().c_str()) == 0)
				break;
		}
		if (i < assets_in_dir.size())
		{
			double last_update = (double)std::chrono::duration_cast<std::chrono::seconds>(file.last_write_time().time_since_epoch()).count();
			if (last_update != assets_in_dir[i]->last_update)
			{
				assets_in_dir[i]->last_update;
				SET_FLAG(assets_in_dir[i]->status_flags, SQ_AS_CHANGED);
			}
		}
		else
		{
			new_items.push_back(new SQUE_FW_NewAsset());
			SQUE_FW_NewAsset* a = new_items[new_items.size()-1];
			memcpy(a->str, file.path().string().c_str(), 512);
			a->str_len = file.path().string().length();
		}
	}
	return new_items;
}



std::vector<SQUE_FW_NewAsset*> SQUE_FS_CheckDirectoryChanges(const char* path, const std::vector<SQUE_CtrlAsset*>& assets_in_dir, HandleNewAssetLocation* handle_fun);


// BruteForce Serialization
class SQUE_OutStream
{
	uint64_t _capacity = 0;
	uint64_t _size = 0;
	char* _data = NULL;	

	static char* allocate(uint32_t size)
	{
		return  new char[size];// (char*)malloc(size);
	}

	static void copyRange(char* begin, char* end, char* dest)
	{
		while (begin != end)
		{
			new((char*)dest) char(*begin); // new ((convert to void*)at pointer dest) Value<T>(copy from *begin)
			++dest;
			++begin;
		} // Why this in loop (guess it can be vectorized instead of single operation memcpy and then delete
	}

	static void deleteRange(char* begin, char* end)
	{
		// non templated type delete deallocate the whole block?
		// but why does it work before?
		delete begin; //?
		//while (begin != end)
		//{
		//	delete begin;
		//	++begin;
		//}
	}

	void reallocate(uint32_t new_capacity)
	{
		char* new_data = allocate(new_capacity);
		copyRange(_data, _data + _size, new_data);
		delete _data;
		//deleteRange(_data, _data + _size);
		//free(_data);
		_data = new_data;
		_capacity = new_capacity;
	}

public:
	uint64_t GetSize() const { return _size; }

	char* GetData() const { return _data; };

	template<class T>
	void WriteBytesAt(const T* from, uint64_t at, uint32_t num_items = 1)
	{
		uint64_t end_write = at + sizeof(T) * num_items;
		if (_capacity < end_write) reallocate(end_write * 2);
		memcpy(&_data[at], from, sizeof(T) * num_items);
		if (at + sizeof(T) * num_items > _size) _size = end_write;
	}

	template<class T>
	void WriteBytes(const T* from, uint32_t num_items = 1)
	{
		uint64_t end_write = _size + sizeof(T) * num_items;
		if (_capacity < end_write) reallocate(end_write * 2);
		memcpy(&_data[_size], from, sizeof(T) * num_items);
		_size = end_write;
	}
};

class SQUE_InStream
{
	SQUE_Asset* _data;

	char* _readpos;
	uint64_t _readpos_n;
public:
	SQUE_InStream() {};
	SQUE_InStream(SQUE_Asset* asset) : _data(asset), _readpos(asset->raw_data), _readpos_n(0) {};
	~SQUE_InStream() { delete _data; _readpos = NULL; }

	uint64_t GetSize() const { return _data->size; };
	uint64_t GetReadPos() const { return _readpos_n; }

	void AttachAsset(SQUE_Asset* asset) { _data = asset; _readpos = _data->raw_data; _readpos_n = 0; }

	template<class T>
	void ReadBytesAt(T* to, uint32_t num_items, uint64_t at)
	{
		SQ_ASSERT((num_items * sizeof(T) + at) < _data->size);
		memcpy(to, &_data->raw_data[at], num_items * sizeof(T));
	}

	template<class T>
	void ReadBytes(T* to, uint32_t num_items = 1)
	{
		memcpy(to, &_data->raw_data[_readpos_n], num_items * sizeof(T));
		_readpos_n += num_items * sizeof(T);
	}
};


#endif