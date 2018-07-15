#include "Sources/Engine/GUI/GUI.h"

GUI::GUI(GLFWwindow * inWindow)
{
    this->InitImGUI(inWindow);
    this->Console = new ConsoleGUI();

    this->show_console = false;
}

GUI::~GUI()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::InitImGUI(GLFWwindow* InWindow)
{
    this->Window = InWindow;
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init();
    // Setup style
    ImGui::StyleColorsLight();
}

void GUI::Draw()
{
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(1000, 600));
    ImGui::StyleColorsDark();
    if (this->show_console)
        this->Console->Draw();
    ImGui::Render();
    
    int display_w, display_h;
    glfwMakeContextCurrent(Window);
    glfwGetFramebufferSize(Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(Window);
    glfwSwapBuffers(Window);
}


