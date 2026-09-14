#--------------------------------------------------------------
# Structural optimization of a building using a genetic algorithm.
#
# Built on Project Chrono (multibody dynamics) + Irrlicht (rendering)
# + Dear ImGui (in-scene UI, DirectX 9 backend).
#
# Quick start:
#   cmake -S . -B build -G "Visual Studio 17 2022" -A x64 ^
#         -DChrono_DIR="<path>/chrono-build/cmake"
#   cmake --build build --config Release
#--------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)

project(ChronoProject_FinalGA LANGUAGES CXX)

#--------------------------------------------------------------
# Platform
#
# The ImGui integration talks to the Irrlicht device's underlying
# IDirect3DDevice9 (see src/System/ConfigureSystem.cpp), so this
# project only builds and runs on Windows.
#--------------------------------------------------------------

if(NOT WIN32)
    message(FATAL_ERROR
        "This project uses the Dear ImGui DirectX 9 backend and is Windows-only.")
endif()

#--------------------------------------------------------------
# Locate Project Chrono
#
# Chrono_DIR must point at the folder containing ChronoConfig.cmake,
# which is either <chrono-build>/cmake or <chrono-install>/lib/cmake.
#--------------------------------------------------------------

find_package(Chrono COMPONENTS Irrlicht CONFIG)

if(NOT Chrono_FOUND)
    message(FATAL_ERROR
        "Could not find Project Chrono (7.0.3) or its Irrlicht module.\n"
        "Point Chrono_DIR at the folder holding ChronoConfig.cmake, e.g.\n"
        "  cmake -S . -B build -DChrono_DIR=\"C:/path/to/chrono-build/cmake\"")
endif()

# ChronoConfig.cmake bakes in absolute paths recorded when Chrono was
# configured. Moving the Chrono source or build tree afterwards leaves those
# paths dangling, and find_package still reports success -- the failure only
# shows up later as a wall of "cannot open include file". Catch it here.
list(GET CHRONO_INCLUDE_DIRS 0 CHRONO_FIRST_INCLUDE_DIR)
if(NOT EXISTS "${CHRONO_FIRST_INCLUDE_DIR}")
    message(FATAL_ERROR
        "Chrono was found at ${Chrono_DIR}, but the include path it records "
        "does not exist:\n"
        "  ${CHRONO_FIRST_INCLUDE_DIR}\n"
        "This means the Chrono source/build tree was moved after it was "
        "configured. Either re-run CMake on the Chrono tree in its current "
        "location, or update the absolute paths inside ChronoConfig.cmake.")
endif()

#--------------------------------------------------------------
# Target
#--------------------------------------------------------------

file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/inc/*.h")
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})

# Mirror the on-disk folder layout in the Visual Studio solution explorer.
source_group(TREE "${PROJECT_SOURCE_DIR}" FILES ${PROJECT_HEADERS} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/inc"
    ${CHRONO_INCLUDE_DIRS})

# Requires C++20.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# PROJECT_DATA_DIR lets the app resolve algorithm_settings.txt, initial_individual.txt
# and its generated output against the project directory, so it behaves the same
# whichever working directory it is launched from. Trailing slash is required.
target_compile_definitions(${PROJECT_NAME} PRIVATE
    CHRONO_DATA_DIR="${CHRONO_DATA_DIR}"
    PROJECT_DATA_DIR="${PROJECT_SOURCE_DIR}/")

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _CRT_SECURE_NO_DEPRECATE   # Chrono/Irrlicht use "unsafe" CRT calls
        _SCL_SECURE_NO_DEPRECATE
        NOMINMAX)                  # keep min/max macros out of std::min/max
endif()

# Chrono's own flags (/arch:AVX2, OpenMP, /wd4275, ...) must match those the
# Chrono libraries were compiled with.
separate_arguments(CHRONO_CXX_FLAGS_LIST NATIVE_COMMAND "${CHRONO_CXX_FLAGS}")
target_compile_options(${PROJECT_NAME} PRIVATE ${CHRONO_CXX_FLAGS_LIST})

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "${CHRONO_LINKER_FLAGS}")

# d3d9.lib backs the ImGui DirectX 9 backend.
target_link_libraries(${PROJECT_NAME} PRIVATE ${CHRONO_LIBRARIES} d3d9.lib)

#--------------------------------------------------------------
# Runtime DLLs
#
# Copied next to the executable after every build, for whichever
# configuration is being built. This works for both single-config
# generators (Ninja) and multi-config ones (Visual Studio), unlike
# Chrono's add_DLL_copy_command, which only handles Release.
#--------------------------------------------------------------

get_filename_component(CHRONO_ROOT "${Chrono_DIR}/.." ABSOLUTE)

if(EXISTS "${CHRONO_ROOT}/bin/Release" OR EXISTS "${CHRONO_ROOT}/bin/Debug")
    set(CHRONO_RUNTIME_DIR "${CHRONO_ROOT}/bin/$<CONFIG>")   # Chrono build tree
else()
    set(CHRONO_RUNTIME_DIR "${CHRONO_ROOT}/bin")             # Chrono install tree
endif()

set(RUNTIME_DLLS Irrlicht.dll ${CHRONO_DLL_NAMES})
list(REMOVE_DUPLICATES RUNTIME_DLLS)

foreach(DLL IN LISTS RUNTIME_DLLS)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CHRONO_RUNTIME_DIR}/${DLL}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Copying ${DLL} next to the executable")
endforeach()

#--------------------------------------------------------------
# Visual Studio conveniences
#
# Data files are addressed through PROJECT_DATA_DIR, so the working directory no
# longer affects where they are read from or written to. It is still pointed at
# the project directory so that anything the app writes relative to the working
# directory (Dear ImGui's imgui.ini) lands somewhere predictable.
#--------------------------------------------------------------

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")

set_property(DIRECTORY "${PROJECT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
