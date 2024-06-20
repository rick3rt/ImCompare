#pragma once
#include <string>
#include <vector>

class Settings
{
  private:
    std::vector<std::string> m_fontNames;

  public:
    Settings() {}
    ~Settings() {}

    void Start();
    void Update(); // main loop
};
