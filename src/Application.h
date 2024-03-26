#pragma once
#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

// #define APP_SCALE 2.0f  // 2.0f
// #define FONT_SCALE 1.0f // 3.0f

// Barebones Application Framework
class Application
{
  public:
    // Constructor.
    Application(std::string title, int w, int h, int argc, char const *argv[]);
    ~Application();
    virtual void Start() {}
    virtual void Update() {}
    void Run();

    inline static Application &Get() { return *s_Instance; }

    ImVec2 GetWindowSize() const;

    bool isRunning() const { return m_Running; }
    void Stop();

    virtual void OnDrop(std::vector<std::string> files) {} // dropping files from OS

  private:
    // ImVec2 m_openGLSize;
    GLFWwindow *m_Window;
    bool m_Running = false;

  protected:
    std::map<std::string, ImFont *> m_fonts;

  private:
    static Application *s_Instance;
};
