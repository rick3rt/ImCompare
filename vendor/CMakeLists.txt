include(FetchContent)


# =============================================================================
# FETCHCONTENT
# =============================================================================
# Dear ImGui -- imgui
# GIT_TAG 9aae45eb4a05a5a1f96be1ef37eb503a12ceb889 # 1.88
# GIT_TAG d7aca168eee54e1423e362384a3cfeec92b22563 # docking
# GIT_TAG 20e1caec858caa8123a6d52d410fa3f2578d3054 # docking 21-12-2023
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG 20e1caec858caa8123a6d52d410fa3f2578d3054 # docking
    SOURCE_DIR ${VENDOR_PATH}/imgui # to download to local dir
)
FetchContent_MakeAvailable(imgui)

# =================== IMGUI ===================
set(IMGUI_PATH ${VENDOR_PATH}/imgui)
add_library(imgui STATIC)
# set(MYIMGUI_CONFIG "${VENDOR_PATH}/my_imgui_config.h")
# target_compile_definitions(imgui PUBLIC IMGUI_USER_CONFIG="${MYIMGUI_CONFIG}")
target_sources(imgui
    PRIVATE
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp
    ${IMGUI_PATH}/imgui.cpp
)

target_include_directories(imgui
    PUBLIC ${IMGUI_PATH}

    # ${CMAKE_CURRENT_SOURCE_DIR}/config
)

# =============================================================================
# GLFW -- glfw/3.3.7
# GIT_TAG 7d5a16ce714f0b5f4efa3262de22e4d948851525 # 3.3.6
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 3.3.9 # 3.3.7
    SOURCE_DIR ${VENDOR_PATH}/glfw # to download to local dir
)
FetchContent_GetProperties(glfw)

if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    # add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
    add_subdirectory(glfw EXCLUDE_FROM_ALL)
endif()

# =============================================================================
# GLEW -- glew/2.2.0
FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
    GIT_TAG glew-cmake-2.2.0 # 2.2.0
    SOURCE_DIR ${VENDOR_PATH}/glew # to download to local dir
)
FetchContent_MakeAvailable(glew)


# =============================================================================
# STB
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb
    SOURCE_DIR ${VENDOR_PATH}/stb # to download to local dir
)
FetchContent_MakeAvailable(stb)

# =============================================================================
# SPDLOG
# =============================================================================
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
    SOURCE_DIR ${VENDOR_PATH}/spdlog # to download to local dir
)
option(SPDLOG_BUILD_SHARED OFF)
option(SPDLOG_BUILD_ALL OFF)
option(SPDLOG_BUILD_TESTING OFF)
FetchContent_MakeAvailable(spdlog)


# =============================================================================
# ImPlot -- implot/0.14
FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot
    GIT_TAG v0.16
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/implot # to download to local dir
)
FetchContent_MakeAvailable(implot)

# =================== IMPLOT ===================
set(IMPLOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/implot)
add_library(implot STATIC)
target_sources(implot
    PRIVATE
    ${IMPLOT_PATH}/implot.cpp
    ${IMPLOT_PATH}/implot_demo.cpp
    ${IMPLOT_PATH}/implot_items.cpp
)
target_include_directories(implot
    PUBLIC ${IMPLOT_PATH}
)
target_link_libraries(implot PUBLIC imgui)