#ifndef GUI_HPP
#define GUI_HPP
#include <GLFW/glfw3.h>
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/examples/imgui_impl_glfw.h"
#include "ThirdParty/imgui/examples/imgui_impl_opengl3.h"
#include <Sources/Engine/GUI/ConsoleGUI.h>


class GUI
{
public:
    GUI(GLFWwindow *inWindow);
    ~GUI();

    void InitImGUI(GLFWwindow* InWindow);
    void Draw();
    void CheckKeyPresses();
    void ShowMainMenuBar();
    ConsoleGUI * GetConsoleGUI();

    int MouseInputMode;

    bool show_console;

private:
    GLFWwindow * Window;
    ConsoleGUI * ConsoleGui;
};



#endif 
