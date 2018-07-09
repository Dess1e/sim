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

    int MouseInputMode;
private:
    GLFWwindow* Window;
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};
#endif 