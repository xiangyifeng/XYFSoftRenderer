cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_DEBUG_POSTFIX "d")
set(glm_DIR ../3rdParty/glm_x64-windows/share/glm)
project(Base LANGUAGES CXX)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(glm CONFIG REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(. DIR_LIB_SRCS)

add_library (Base ${DIR_LIB_SRCS})

target_include_directories(Base PUBLIC
    RendererCore
)
include_directories(Base ${CMAKE_HOME_DIRECTORY}/3rdParty/assimp/include)

target_link_libraries(Base PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Base PUBLIC glm::glm)
target_link_libraries(Base PUBLIC ${CMAKE_HOME_DIRECTORY}/3rdParty/assimp/build/lib/libassimpd.dll.a)

