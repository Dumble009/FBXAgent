cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(ProjectName FbxAgent)
set(LibraryName FbxAgent)
set(ExecutableName FbxAgentTest)

project(${ProjectName} VERSION 0.1.0)

include("C:/MyLib/CMakeModules/deploy.cmake")

enable_testing()
add_subdirectory(tests)

file(GLOB_RECURSE LIB_SOURCES LIST_DIRECTORIES false RELATIVE "${CMAKE_SOURCE_DIR}" CONFIGURE_DEPENDS "libsources/*.cpp")
file(GLOB_RECURSE EXE_SOURCES LIST_DIRECTORIES false RELATIVE "${CMAKE_SOURCE_DIR}" CONFIGURE_DEPENDS "sources/*.cpp")

add_library(${LibraryName} ${LIB_SOURCES})
add_executable(${ExecutableName} ${EXE_SOURCES})

target_include_directories(${LibraryName} PUBLIC "${CMAKE_SOURCE_DIR}/libsources" "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.2/include" "C:/MyLib/DebugTool/include" "C:/MyLib/MathLib/include")
target_link_directories(${LibraryName} PUBLIC "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.2/lib/vs2019/x64/debug" "C:/MyLib/DebugTool/lib" "C:/MyLib/MathLib/lib")
target_link_libraries(${LibraryName} libfbxsdk DebugTool MathLib)

target_include_directories(${ExecutableName} PUBLIC "${CMAKE_SOURCE_DIR}/sources" "${CMAKE_SOURCE_DIR}/libsources" "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.2/include" "C:/MyLib/DebugTool/include")
target_link_directories(${ExecutableName} PUBLIC "C:/Program Files/Autodesk/FBX/FBX SDK/2020.3.2/lib/vs2019/x64/debug" "${CMAKE_SOURCE_DIR}/build" "C:/MyLib/DebugTool/lib")
target_link_libraries(${ExecutableName} libfbxsdk FbxAgent DebugTool)

deploy_lib("FbxAgent" ${LibraryName} ${ProjectName})

add_subdirectory(libsources)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set_target_properties(${LibraryName} PROPERTIES COMPILE_PDB_NAME_DEBUG ${LibraryName} COMPILE_PDB_NAME_RELEASE ${LibraryName})

include(CPack)