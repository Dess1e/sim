#include "Sources/Engine/GUI.h"

GUI::GUI()
{
}

GUI::~GUI()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::Init(GLFWwindow* InWindow)
{
    Window = InWindow;
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

void GUI::Loop()
{
    static ImVec4 ClearColor = ImVec4(0.0f, 0.138f, 0.588f, 1.00f);
    bool ShowMainWindow = true;

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowBgAlpha(0.7f);
    // 1. Show a simple window.        
    if (ImGui::Begin("Main Window", &ShowMainWindow))
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::SetWindowPos(ImVec2(10.0, 30.0));
        ImGui::SetWindowSize(ImVec2(350.0, 150.0));
        ImGui::Text("Use 'M' to enable mouse cursor"); 
        ImGui::Text("Use 'N' to disable mouse cursor");

        ImGui::SliderFloat("float", &f, 0.0f, 0.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
        ImGui::ColorEdit3("clear color", (float*)&ClearColor); // Edit 3 floats representing a color

        //ImGui::SameLine();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 2. Show bar menu
    ShowMainMenuBar();

    // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
    if (show_demo_window)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    // Rendering
    ImGui::Render();
    
    int display_w, display_h;
    glfwMakeContextCurrent(Window);
    glfwGetFramebufferSize(Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
    glfwMakeContextCurrent(Window);
    glfwSwapBuffers(Window);
}

void GUI::ShowMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Functions"))
        {
            if (ImGui::MenuItem("Show Demo window", "CTRL+X")) {show_demo_window = true;}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
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
