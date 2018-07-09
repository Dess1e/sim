#include "src/engine/GUI.hpp"

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
    ImGui::StyleColorsDark();
}

void GUI::Loop()
{
    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
    {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Text("USE 'M' BUTTON TO ENABLE CURSOR");         // Display some text (you can use a format string too)
        ImGui::Text("USE 'N' BUTTON TO DISABLE CURSOR");        // Display some text (you can use a format string too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
        ImGui::ColorEdit3("clear color", (float*)&ClearColor); // Edit 3 floats representing a color
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
        ImGui::Checkbox("Another Window", &show_another_window);
        if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
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
