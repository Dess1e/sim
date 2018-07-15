#ifndef GUI_HPP
#define GUI_HPP
#include <GLFW/glfw3.h>
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/examples/imgui_impl_glfw.h"
#include "ThirdParty/imgui/examples/imgui_impl_opengl3.h"

#define CONSOLE_INPUT_LINE_BUFFER_SIZE 64
#define CONSOLE_DEFAULT_LABEL "Console"

class ConsoleGUI
{
public:
    ConsoleGUI();
    ImGuiTextBuffer Buf;
    bool ScrollToBottom;
    bool is_enabled;
    void Clear();
    void AddLog(const char* fmt, ...);
    void Draw(bool* p_opened = NULL);
private:
    char * console_label;
    char * input_line_buffer;
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

    bool show_console;

private:
    GLFWwindow * Window;
    ConsoleGUI * Console;
};



#endif 
