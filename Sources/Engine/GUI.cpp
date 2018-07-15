#include "Sources/Engine/GUI.h"

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

    ImGui::SetNextWindowBgAlpha(0.7f);
    ImGui::SetNextWindowSize(ImVec2(800, 400));

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

ConsoleGUI::ConsoleGUI()
{
    this->is_enabled = false;
    this->input_line_buffer = new char[CONSOLE_INPUT_LINE_BUFFER_SIZE];
    this->input_line_buffer[0] = '\0'; //so there will be '' string at start
    this->console_label = (char *)CONSOLE_DEFAULT_LABEL;
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

void ConsoleGUI::Draw(bool *p_opened)
{
    ImGui::Begin(CONSOLE_DEFAULT_LABEL, p_opened);
    ImGui::TextUnformatted(Buf.begin());
    if (ScrollToBottom)
        ImGui::SetScrollHere(1.0f);
    ImGui::SetKeyboardFocusHere();
    if (ImGui::InputText("", this->input_line_buffer, CONSOLE_INPUT_LINE_BUFFER_SIZE,
                         ImGuiInputTextFlags_EnterReturnsTrue))
    {
        this->AddLog("> %s\n", this->input_line_buffer);
        memset(this->input_line_buffer, 0, CONSOLE_INPUT_LINE_BUFFER_SIZE);
    }

    ScrollToBottom = false;
    ImGui::End();
}
