#include "Sources/Engine/GUI.h"

GUI::GUI(GLFWwindow * inWindow)
{
    this->InitImGUI(inWindow);
    this->Console = new ConsoleGUI();
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

    ImGui::SetNextWindowBgAlpha(0.7f);
    ImGui::SetNextWindowSize(ImVec2(800, 400));

    this->Console->Draw("Console");

    ImGui::Render();
    
    int display_w, display_h;
    glfwMakeContextCurrent(Window);
    glfwGetFramebufferSize(Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(Window);
    glfwSwapBuffers(Window);
}

void GUI::CheckKeyPresses()
{
    if (glfwGetKey(Window, GLFW_KEY_M) == GLFW_PRESS)
    {
        this->MouseInputMode  = GLFW_CURSOR_NORMAL;
        glfwSetInputMode(this->Window, GLFW_CURSOR, this->MouseInputMode);
    }
    if (glfwGetKey(Window, GLFW_KEY_N) == GLFW_PRESS)
    {
        this->MouseInputMode  = GLFW_CURSOR_DISABLED;
        glfwSetInputMode(this->Window, GLFW_CURSOR, this->MouseInputMode);
    }
}

ConsoleGUI::ConsoleGUI()
{
    this->is_enabled = false;
}

void ConsoleGUI::Clear()
{
    this->Buf.clear();
}

void ConsoleGUI::AddLog(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    this->Buf.appendfv(fmt, args);
    va_end(args);
    this->ScrollToBottom = true;
}

void ConsoleGUI::Draw(const char *title, bool *p_opened)
{
    ImGui::Begin(title, p_opened);
    ImGui::TextUnformatted(Buf.begin());
    if (ScrollToBottom)
        ImGui::SetScrollHere(1.0f);
    ScrollToBottom = false;
    ImGui::End();
}
