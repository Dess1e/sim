#include <src/engine/engine.hpp>

Engine::Engine()
{
    //engine class constructor
    this->MouseInputMode = GLFW_CURSOR_DISABLED;
    this->world = World();
    this->options.mouse_speed = 0.3;
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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    do
    {
        this->pollTime();
        GamePlayer.CalcPlayerView(this->MouseInputMode, this->delta_time, this->options.mouse_speed);
        GamePlayer.CheckKeyPresses(this->delta_time);
        this->checkKeyPresses();
        mvp = calculateMVP(16/9, 0.1, 100.0);

        glUniform1i(texture_id, 0);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwPollEvents();

        EngineGUI.Loop();
    }
    while (glfwGetKey(this->main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(this->main_window) == 0);
    
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

    // Player init
    GamePlayer.Init(this->main_window, this->hw_specs.scr_w, this->hw_specs.scr_h);

    // ImGui initialization
    EngineGUI.Init(this->main_window);

    // Init cursor 
    this->MouseInputMode = GLFW_CURSOR_DISABLED;
    glfwSetInputMode(this->main_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(this->main_window, GLFW_CURSOR, this->MouseInputMode);
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

glm::mat4 Engine::calculateMVP(float ratio, float nearz, float farz)
{
    glm::mat4 proj = glm::perspective(glm::radians(this->GamePlayer.player_fov), ratio, nearz, farz);
    glm::mat4 view = glm::lookAt(this->GamePlayer.player_pos,
                                 this->GamePlayer.direction + this->GamePlayer.player_pos,
                                 this->GamePlayer.up);
    glm::mat4 model = glm::mat4(1);
    glm::mat4 mvp = proj * view * model;
    return mvp;
}

void Engine::checkKeyPresses()
{
    // Enable/Disable mouse cursor
    if (glfwGetKey(this->main_window, GLFW_KEY_M) == GLFW_PRESS)
    {
        this->MouseInputMode = GLFW_CURSOR_NORMAL;
        glfwSetInputMode(this->main_window, GLFW_CURSOR, this->MouseInputMode);
    }
    if (glfwGetKey(this->main_window, GLFW_KEY_N) == GLFW_PRESS)
    {
        this->MouseInputMode = GLFW_CURSOR_DISABLED;
        glfwSetInputMode(this->main_window, GLFW_CURSOR, this->MouseInputMode);
    }
}

void Engine::pollTime()
{
    auto curr_time = glfwGetTime();
    this->delta_time = curr_time - this->last_time;
    this->last_time = curr_time;
}
