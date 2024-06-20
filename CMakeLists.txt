# CMake 3.19 is the earliest tested version that works on both Windows and Linux
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(ImCompare VERSION 1.0.0 LANGUAGES CXX)

# ########################################################################################
# User variables
set(VENDOR_PATH "${PROJECT_SOURCE_DIR}/vendor")

# message("VENDOR_PATH = ${VENDOR_PATH}")

# add global flag _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
add_compile_definitions(_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
cmake_policy(SET CMP0048 OLD)

# ########################################################################################
# # Global flags
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
endif()

# ########################################################################################
# Build flags
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG=1")

# ########################################################################################
add_subdirectory(vendor) # include third party libraries
add_subdirectory(test) #
add_subdirectory(example) # test example imgui

# ########################################################################################
# add the executable
# ########################################################################################

# define sources for AppFramework
set(SRCS_TO_BUILD
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Explorer.cpp
    ${PROJECT_SOURCE_DIR}/src/Core/Core.h
    ${PROJECT_SOURCE_DIR}/src/Core/Log.cpp
    ${PROJECT_SOURCE_DIR}/src/Core/Menu.cpp
    ${PROJECT_SOURCE_DIR}/src/Core/Settings.cpp
    ${PROJECT_SOURCE_DIR}/src/Core/Application.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/Viewer.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer/ImageLoader.cpp
    ${PROJECT_SOURCE_DIR}/src/Filesystem/FileDialog.cpp
    ${PROJECT_SOURCE_DIR}/src/Filesystem/FileManager.cpp
    ${PROJECT_SOURCE_DIR}/src/Filesystem/FolderManager.cpp

    # ${PROJECT_SOURCE_DIR}/src/CompareManager.cpp
    ${VENDOR_PATH}/imgui/backends/imgui_impl_glfw.cpp # imgui backend
    ${VENDOR_PATH}/imgui/backends/imgui_impl_opengl3.cpp # imgui backend
    ${VENDOR_PATH}/imgui/misc/cpp/imgui_stdlib.cpp # imgui stl bindings
)

add_executable(${PROJECT_NAME} ${SRCS_TO_BUILD})
target_compile_definitions(${PROJECT_NAME} PUBLIC _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING) # https://github.com/gabime/spdlog/issues/2912

# include dirs
target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PRIVATE ${VENDOR_PATH}/imgui
    PRIVATE ${VENDOR_PATH}/imgui/backends
    PRIVATE ${VENDOR_PATH}/glew/include
    PRIVATE ${VENDOR_PATH}/stb
    PRIVATE ${GLFW_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    glfw
    libglew_static
    imgui
    implot
    spdlog::spdlog
)

# add symbolic link to resource folder to build directory for debugging in vscode
# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/resource $<TARGET_FILE_DIR:${PROJECT_NAME}>/resource
# COMMENT "symbolic link resource folder from ${CMAKE_CURRENT_SOURCE_DIR}/resource to $<TARGET_FILE_DIR:${PROJECT_NAME}>/resource"
# )
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Debug>,copy_if_different,true> $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.pdb ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:RelWithDebInfo>,copy_if_different,true> $<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.pdb ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/resource ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/resource
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/resource $<TARGET_FILE_DIR:${PROJECT_NAME}>/resource
)