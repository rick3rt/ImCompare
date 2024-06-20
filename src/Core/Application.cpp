#include <iostream>

#include <imgui.h>
#include <implot.h>
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Core/Application.h"
#include "Core/Log.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

/// Macro to request high performance GPU in systems (usually laptops) with both
/// dedicated and discrete GPUs
#if defined(_WIN32)
extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement = 0;
extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0;
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    LOG_ERROR("GLFW Error {}: {}", error, description);
}

static void glfw_drop_callback(GLFWwindow *window, int count, const char **paths)
{
    std::vector<std::string> files(paths, paths + count);
    Application::Get().OnDrop(files);
}

// sprintf equivalent for std::string
std::string string_format(const std::string fmt_str, ...)
{
    int final_n,
        n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while (1)
    {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
        {
            n += abs(final_n - n + 1);
        } else
        {
            break;
        }
    }
    return std::string(formatted.get());
}

Application *Application::s_Instance = nullptr;

Application::Application(std::string title, int w, int h, int argc, char const *argv[])
{
    s_Instance = this;

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) abort();

    // GL 3.0 + GLSL 330
    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // 3.0+ only

    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE); // 3.0+ only

    // Create window with graphics context
    m_Window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
    if (!m_Window)
    {
        fprintf(stderr, "Failed to initialize GLFW window!\n");
        LOG_ERROR("Failed to initialize GLFW window!");
        glfwTerminate();
        abort();
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(m_Window, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(m_Window);
    glfwSetDropCallback(m_Window, glfw_drop_callback);

    // init glew
    if (GLEW_OK != glewInit()) { abort(); }
    while (GLFW_NO_ERROR != glfwGetError(NULL)) {
    } /* glewInit may cause some OpenGL errors -- flush the error state */

    // Print version on console to see that everything initialized correctly
    LOG_INFO("GLFW Version: {}", glfwGetVersionString());
    LOG_INFO("GLEW Version: {}", (const char *)glGetString(GL_VERSION));

    glfwSwapInterval(1); // Enable vsync
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // create FrameBuffer
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
    ImPlot::StyleColorsDark();

    // get the current DPI scaling from glfw
    float xscale, yscale;
    glfwGetWindowContentScale(m_Window, &xscale, &yscale);
    io.FontGlobalScale = xscale;
    LOG_INFO("DPI Scaling: {}x{}", xscale, yscale);

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(xscale);
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // load fonts
    m_fonts["menu"] = io.Fonts->AddFontFromFileTTF("resource/fonts/Roboto-Medium.ttf", 16.0f);
    // m_fonts["mono"] = io.Fonts->AddFontFromFileTTF("resource/fonts/ProggyClean.ttf", 14.0f);
    // m_fonts["mono"] = io.Fonts->AddFontFromFileTTF("resource/fonts/ProggyTiny.ttf", 14.0f);
    m_fonts["mono"] = io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 12.0f);

    // push fonts with different sizes
    for (int i = 5; i < 16; i++)
    {
        std::string key = string_format("menu_size%02i", i);
        LOG_INFO("Loading font: {}", key);
        m_fonts[key] = io.Fonts->AddFontFromFileTTF("resource/fonts/Roboto-Medium.ttf", (float)i);
    }
    // SetFont("menu_size05");
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Application::Run()
{
    m_Running = true;
    Start();

    // Main loop
    while (!glfwWindowShouldClose(m_Window))
    {
        // close glfw window
        if (!isRunning()) { glfwSetWindowShouldClose(m_Window, true); }

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        // clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(); // allow docking in main viewport

        Update(); // UI update loop of app

        ImGui::Render(); // Rendering ImGui before openGL render calls

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(m_Window);
    }
}

ImVec2 Application::GetWindowSize() const
{
    int w, h;
    glfwGetWindowSize(m_Window, &w, &h);
    return ImVec2(w, h);
}

void Application::Stop()
{
    LOG_INFO("Stopping Application");
    m_Running = false;
}

void Application::SetFont(const std::string &key)
{
    ImGuiIO &io = ImGui::GetIO();
    io.FontDefault = m_fonts[key];
}