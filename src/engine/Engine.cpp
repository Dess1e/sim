#include <src/engine/Engine.h>

Engine::Engine()
{
    this->MouseInputMode = GLFW_CURSOR_DISABLED;
    this->world = World();
    this->options.mouse_speed = 0.3;
    this->hw_specs.scr_h = 768;
    this->hw_specs.scr_w = 1366;
    this->init();
}

void Engine::debugPrint(unsigned char level, std::string message)
{
    switch (level)
    {
    case DBG_LEVEL_WARNING:
        printf("[Warning]: %s", message.c_str());
        break;
    case DBG_LEVEL_ERROR:
        printf("[ERROR]: %s", message.c_str());
        break;
    default:
    case DBG_LEVEL_FATAL:
        printf("[FATAL]: %s", message.c_str());
        this->quit();
    }
}

void Engine::quit()
{
    for (const auto& kv : this->gl_variables->shaders)
    {
        Shader * curr = kv.second;
        glDeleteProgram(curr->id);
    }
    glfwTerminate();
}

void Engine::loadShader(std::string name)
{
    try
    {
        this->gl_variables->shaders[name] = new Shader(name);
    }
    catch (ShaderCreateException& e)
    {
        auto error_message = e.what();
        this->debugPrint(DBG_LEVEL_FATAL, error_message);
    }
}
void Engine::useShader(std::string name)
{
    auto shader_map = this->gl_variables->shaders;
    if (!shader_map.count(name))
    {
        this->debugPrint(DBG_LEVEL_ERROR, "No compiled/linked shader with name %s found");
        return;
    }
    this->gl_variables->current_shader = shader_map[name];
    this->gl_variables->current_shader->use();
}
void Engine::mainloop()
{
    GLuint MatrixID = this->gl_variables->current_shader->getUniform("model_projection_mat");
    glm::mat4 mvp;
    mvp = calculateMVP(16/9, 0.1, 100.0);
    auto x = Model("resources/nanosuit.obj");
    do
    {
        this->pollTime();
        GamePlayer.CalcPlayerView(this->MouseInputMode, this->delta_time, this->options.mouse_speed);
        GamePlayer.CheckKeyPresses(this->delta_time);
        mvp = calculateMVP(16/9, 0.1, 100.0);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        x.draw(this->gl_variables->current_shader);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        EngineGUI.Loop();
    }
    while (glfwGetKey(this->main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(this->main_window) == 0);
    this->quit();
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
    glfwSetInputMode(this->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(this->main_window, this->hw_specs.scr_w / 2, this->hw_specs.scr_h / 2);

    glClearColor(0, 0, 0, 0);

    this->loadShader("simple");
    this->useShader("simple");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
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



void Engine::pollTime()
{
    auto curr_time = glfwGetTime();
    this->delta_time = curr_time - this->last_time;
    this->last_time = curr_time;
}

