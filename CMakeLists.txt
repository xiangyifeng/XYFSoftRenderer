cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(XYFSoftRender LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "E:/Qt/6.2.4/mingw_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 COMPONENTS Widgets REQUIRED) # Qt COMPONENTS

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CMAKE_CXX_FLAGS "-fopenmp -O3")
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
aux_source_directory(. PROJECT_SOURCES)
add_subdirectory(Base/)
add_subdirectory(RendererCore/)
add_subdirectory(Shader/)
add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement 
    ${PROJECT_SOURCES} 
) 

include_directories(XYFSoftRender ${CMAKE_HOME_DIRECTORY}/3rdParty/assimp/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets) # Qt5 Shared Library


target_link_libraries(XYFSoftRender PUBLIC
    Base
    RendererCore
    Shader
)
target_include_directories(XYFSoftRender PUBLIC
    Base
    RendererCore
    Shader
)