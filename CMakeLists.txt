cmake_minimum_required(VERSION 3.21)

project(FinisherLowHP VERSION 1.0.0 LANGUAGES CXX)

set(PLUGIN_CONFIG_FOLDER "FinisherLowHP" CACHE STRING "SKSE/Plugins config subdirectory")

if(DEFINED ENV{SKYRIM_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_FOLDER}/Data")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_FOLDER}/Data")
endif()

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME} ${CMAKE_BUILD_TYPE}")
endif()

find_package(CommonLibSSE CONFIG REQUIRED)
find_package(directxtk CONFIG REQUIRED)
include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${headers} ${sources})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		${SIMPLEINI_INCLUDE_DIRS}
)

if(DEFINED OUTPUT_FOLDER)
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")
    set(SRC_CONFIG_FOLDER "${CMAKE_SOURCE_DIR}/config")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        VERBATIM
    )
    if(EXISTS "${SRC_CONFIG_FOLDER}")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different "${CMAKE_SOURCE_DIR}/config" "${DLL_FOLDER}/${PLUGIN_CONFIG_FOLDER}"
            VERBATIM
        )
    endif()
endif()
