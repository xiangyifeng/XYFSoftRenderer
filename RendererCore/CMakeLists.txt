cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_DEBUG_POSTFIX "d")

set(glm_DIR ../3rdParty/glm_x64-windows/share/glm)

project(RendererCore LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fopenmp")
set(CMAKE_PREFIX_PATH "E:/Qt/6.2.4/mingw_64")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(OpenMP)

ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
set(RENDERERCORE_SRC
    DataDefine.hpp
    FrameBuffer.cpp
    FrameBuffer.h
    Shader.hpp
    RendererDevice.cpp
    RendererDevice.h
    MyTexture.h
    MyTexture.cpp
    RendererHelp.hpp
)

find_package(glm CONFIG REQUIRED)
add_library(RendererCore STATIC ${RENDERERCORE_SRC})
target_link_libraries(RendererCore PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(RendererCore PUBLIC glm::glm)
