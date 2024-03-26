project(example CXX)



add_executable(example    
    main.cpp
    # example.cpp
    # ${VENDOR_PATH}/imgui/backends/imgui_impl_glfw.cpp # imgui backend
    # ${VENDOR_PATH}/imgui/backends/imgui_impl_opengl3.cpp # imgui backend
)
target_compile_definitions(example PUBLIC  _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING) #  https://github.com/gabime/spdlog/issues/2912


# include dirs
target_include_directories(example
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${VENDOR_PATH}/imgui
    PRIVATE ${VENDOR_PATH}/imgui/backends
    PRIVATE ${VENDOR_PATH}/glew/include
    PRIVATE ${GLFW_INCLUDE_DIR}
)

target_link_libraries(example
    PRIVATE
    # glfw 
    # libglew_static
    # imgui
)