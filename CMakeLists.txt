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
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
elseif()
    message("Debug?")
    set_target_properties(${OpenR_TARGET} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:WINDOWS"
    )
    
    
endif()

target_include_directories(${OpenR_TARGET} PUBLIC 
    "${OpenR_INC}"
)

#------------------------------------------------------------------------------------------------
# EXTERNAL LIBRARIES
#------------------------------------------------------------------------------------------------

# DearImGUI
target_include_directories(${OpenR_TARGET} PUBLIC
    ${OpenR_EXT}/dear_imgui
)

target_sources(${OpenR_TARGET} PUBLIC
    ${OpenR_EXT}/dear_imgui/imgui.cpp
    ${OpenR_EXT}/dear_imgui/imgui_demo.cpp
    ${OpenR_EXT}/dear_imgui/imgui_draw.cpp
    ${OpenR_EXT}/dear_imgui/imgui_tables.cpp
    ${OpenR_EXT}/dear_imgui/imgui_widgets.cpp
    ${OpenR_EXT}/dear_imgui/backends/imgui_impl_opengl3.cpp
    ${OpenR_EXT}/dear_imgui/backends/imgui_impl_glfw.cpp
)

# RayLib - Base app
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

# build raylib
execute_process(COMMAND mkdir ${OpenR_EXT}/raylib/CMakeBuild)
add_subdirectory(${OpenR_EXT}/raylib ${OpenR_EXT}/raylib/CMakeBuild)
target_include_directories(${OpenR_TARGET} PUBLIC
    ${OpenR_EXT}/raylib/src
)

target_link_libraries(${OpenR_TARGET} PUBLIC
    raylib
)
#------------------------------------------------------------------------------------------------
# GROUP SOURCING FOR IDEs
#------------------------------------------------------------------------------------------------

#source_group()