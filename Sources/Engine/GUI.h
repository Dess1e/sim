#ifndef GUI_HPP
#define GUI_HPP
#include <GLFW/glfw3.h>
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/examples/imgui_impl_glfw.h"
#include "ThirdParty/imgui/examples/imgui_impl_opengl3.h"

class ConsoleGUI
{
public:
    ConsoleGUI();
    ImGuiTextBuffer Buf;
    bool ScrollToBottom;
    bool is_enabled;
    void Clear();
    void AddLog(const char* fmt, ...);
    void Draw(const char* title, bool* p_opened = NULL);
};

class GUI
{
public:
    GUI(GLFWwindow *inWindow);
    ~GUI();

    void InitImGUI(GLFWwindow* InWindow);
    void Draw();
    void CheckKeyPresses();
    void ShowMainMenuBar();

    int MouseInputMode;

private:
    GLFWwindow * Window;
    ConsoleGUI * Console;
};



#endif 
