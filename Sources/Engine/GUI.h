#ifndef GUI_HPP
#define GUI_HPP
#include <GLFW/glfw3.h>
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/examples/imgui_impl_glfw.h"
#include "ThirdParty/imgui/examples/imgui_impl_opengl3.h"

class GUI
{
public:
    GUI();
    ~GUI();

    void Init(GLFWwindow* InWindow);
    void Loop();
    void CheckKeyPresses();
    void ShowMainMenuBar();

    int MouseInputMode;

private:
    GLFWwindow* Window;
    bool show_demo_window = false;
};
#endif 