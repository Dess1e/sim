#include <src/engine/engine.hpp>

Engine::Engine()
{
    //engine class constructor
    this->world = World();
    this->options.player_speed = 3;
    this->player.player_pos = glm::vec3(3, 0, 3);
    this->player.horizontal_angle = 0;
    this->player.vertical_angle = 0;
    this->options.mouse_speed = 0.3;
    this->options.player_fov = 90.0;
    this->hw_specs.scr_h = 768;
    this->hw_specs.scr_w = 1366;
    this->init();
}

void Engine::mainloop()
{
    GLuint MatrixID = glGetUniformLocation(this->gl_variables.shaders_id,
                                           "model_projection_mat");
    glm::mat4 mvp;
    mvp = calculateMVP(16/9, 0.1, 100.0);

    GLuint texture = this->resource_loader.LoadTextureBMP("textures/texture.bmp");
    GLuint texture_id = glGetUniformLocation(this->gl_variables.shaders_id, "texture_sampler");

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    do
    {
        this->pollTime();
        this->calcPlayerView();
        this->checkKeyPresses();
        mvp = calculateMVP(16/9, 0.1, 100.0);

        glUniform1i(texture_id, 0);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwPollEvents();

        loopImGui(show_demo_window, show_another_window, clear_color, this->main_window);
    }
    while (glfwGetKey(this->main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(this->main_window) == 0);

    destroyImGui();
    
    glDeleteBuffers(1, &this->gl_variables.vertex_buffer);
    glDeleteBuffers(1, &this->gl_variables.color_buffer);
    glDeleteProgram(this->gl_variables.shaders_id);
    glDeleteVertexArrays(1, &this->gl_variables.vertex_array_id);
    glfwTerminate();

    return;
}

void Engine::init()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW, exiting...\n");
        return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //macos comp
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->main_window = glfwCreateWindow(1366, 768, "main window name", 0, 0);
    if (this->main_window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window, exiting...\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(this->main_window);
    glewExperimental = true; //Need for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW, exiting...\n");
        glfwTerminate();
        return;
    }

    // ImGui initialization
    initImGui();

    glfwSetInputMode(this->main_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(this->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(this->main_window, this->hw_specs.scr_w / 2, this->hw_specs.scr_h / 2);

    glGenVertexArrays(1, &this->gl_variables.vertex_array_id);
    glBindVertexArray(this->gl_variables.vertex_array_id);

    this->gl_variables.shaders_id = this->resource_loader.LoadShaders("shaders/vs.glsl",
                                              "shaders/fs.glsl");

    glUseProgram(this->gl_variables.shaders_id);

    static const GLfloat vertex_buffer[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    static const GLfloat uv_buffer[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
    };

    glGenBuffers(1, &this->gl_variables.vertex_buffer);
    glGenBuffers(1, &this->gl_variables.color_buffer);

    glBindBuffer(GL_ARRAY_BUFFER, this->gl_variables.vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer)/*size of vbuf*/,
                 vertex_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_variables.color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer)/*size of cbuf*/,
                 uv_buffer, GL_STATIC_DRAW);

    //zbuffering please
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_variables.vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_variables.color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);



}

void Engine::calcPlayerView()
{
    glm::vec3 player_pos = this->player.player_pos;
    double xpos, ypos;
    glfwGetCursorPos(this->main_window, &xpos, &ypos);

    float scr_w = this->hw_specs.scr_w, scr_h = this->hw_specs.scr_h;
    glfwSetCursorPos(this->main_window, scr_w / 2, scr_h / 2);
    Engine::_Options& options = this->options;
    Engine::_Player * player = &this->player;

    auto delta_time = this->delta_time;
    player->horizontal_angle += options.mouse_speed * delta_time * (scr_w / 2 - xpos);
    auto vert_angle_increment = options.mouse_speed * delta_time * (scr_h / 2 - ypos);
    if (player->vertical_angle + vert_angle_increment > 3.14)
        player->vertical_angle = 3.14;
    else if (player->vertical_angle + vert_angle_increment < -3.14)
        player->vertical_angle = -3.14;
    else
        player->vertical_angle += vert_angle_increment;
    glm::vec3 direction( cos(player->vertical_angle) * sin(player->horizontal_angle),
                         sin(player->vertical_angle),
                         cos(player->vertical_angle) * cos(player->horizontal_angle));
    glm::vec3 right(sin(player->horizontal_angle - 3.14 / 2.0),
                    0,
                    cos(player->horizontal_angle - 3.14 / 2.0));
    glm::vec3 up = glm::cross(right, direction);

    player->direction = direction;
    player->up = up;
    player->right = right;
}

glm::mat4 Engine::calculateMVP(float ratio, float nearz, float farz)
{
    glm::mat4 proj = glm::perspective(glm::radians(this->options.player_fov), ratio, nearz, farz);
    glm::mat4 view = glm::lookAt(this->player.player_pos,
                                 this->player.direction + this->player.player_pos,
                                 this->player.up);
    glm::mat4 model = glm::mat4(1);
    glm::mat4 mvp = proj * view * model;
    return mvp;
}

void Engine::checkKeyPresses()
{
    if (glfwGetKey(this->main_window, GLFW_KEY_C) == GLFW_PRESS)
    {
        this->resetCamera();
    }
    if (glfwGetKey(this->main_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        this->options.player_speed = 9;
    }
    else
    {
        this->options.player_speed = 3;
    }
    if (glfwGetKey(this->main_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        this->player.player_pos += this->player.direction * float(this->delta_time) * this->options.player_speed;
    }
    if (glfwGetKey(this->main_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        this->player.player_pos -= this->player.right * float(this->delta_time) * this->options.player_speed;
    }
    if (glfwGetKey(this->main_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        this->player.player_pos -= this->player.direction * float(this->delta_time) * this->options.player_speed;
    }
    if (glfwGetKey(this->main_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        this->player.player_pos += this->player.right * float(this->delta_time) * this->options.player_speed;
    }
}

void Engine::pollTime()
{
    auto curr_time = glfwGetTime();
    this->delta_time = curr_time - this->last_time;
    this->last_time = curr_time;
}

void Engine::resetCamera()
{
    this->player.horizontal_angle = 3.3;
    this->player.vertical_angle = 0;
}

void Engine::initImGui()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(this->main_window, true);
    ImGui_ImplOpenGL3_Init();

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

}

void Engine::destroyImGui()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Engine::loopImGui(bool show_demo_window, bool show_another_window, ImVec4& clear_color, GLFWwindow* window)
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
        ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
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
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
}
