cmake_minimum_required(VERSION 3.1)

#------------------------------------------------------------------------------------------------
# HELPER MODULES
#------------------------------------------------------------------------------------------------
set(OpenR_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/_cmake)

#------------------------------------------------------------------------------------------------
# SETUP CURRENT PROJECT
#------------------------------------------------------------------------------------------------
project("MarcFly's OpenRole"    VERSION 2021.1
                                DESCRIPTION "Personal tool for managing DnD games"
                                LANGUAGES C CXX)
set(OpenR_TARGET "MarcFly_OpenRole")

# C / CXX Config

# Base Include Directiories
set(OpenR_INC ${CMAKE_CURRENT_SOURCE_DIR}/_src)
set(OpenR_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/_deps)
set(OpenR_EXT ${CMAKE_CURRENT_SOURCE_DIR}/_submodules)

# Engine Source Files
set(OpenR_SRC
    ${OpenR_INC}/main.cpp

)

add_executable(${OpenR_TARGET} "${OpenR_SRC}")

#------------------------------------------------------------------------------------------------
# BUILD
#------------------------------------------------------------------------------------------------

# Windows basics
message("Does htis if work?")
if(CMAKE_BUILD_TYPE EQUAL "Release")
    message("Release?")
    set_target_properties(${OpenR_TARGET} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS" "/MT"
    )
elseif()
    message("Debug?")
    set_target_properties(${OpenR_TARGET} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS" "/MT"
    )
    
    
endif()

target_include_directories(${OpenR_TARGET} PUBLIC 
    "${OpenR_INC}"
)

#------------------------------------------------------------------------------------------------
# EXTERNAL LIBRARIES
#------------------------------------------------------------------------------------------------

add_custom_target( libmatoya_build 
    COMMAND cd ./_submodules/libmatoya
    COMMAND nmake
)
target_include_directories(${OpenR_TARGET} PUBLIC
    ${OpenR_EXT}/libmatoya/src
)

target_link_libraries(${OpenR_TARGET} PUBLIC
    ${OpenR_EXT}/libmatoya/bin/windows/x86_x64/matoya.lib
    bcrypt.lib d3d11.lib d3d9.lib hid.lib uuid.lib dxguid.lib opengl32.lib ws2_32.lib user32.lib gdi32.lib xinput9_1_0.lib ole32.lib shell32.lib windowscodecs.lib shlwapi.lib imm32.lib winmm.lib winhttp.lib secur32.lib crypt32.lib
)

#------------------------------------------------------------------------------------------------
# GROUP SOURCING FOR IDEs
#------------------------------------------------------------------------------------------------

#source_group()