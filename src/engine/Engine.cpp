#include <src/engine/Engine.h>

Engine::Engine()
{

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

    Landscape TestLandscape = Landscape();
    Mesh LandscapeMesh = TestLandscape.CreateBasicShape();

    do
    {
        this->pollTime();
        this->calcPlayerView();
        this->checkKeyPresses();
        mvp = calculateMVP(16/9, 0.1, 100.0);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        x.draw(this->gl_variables->current_shader);
        LandscapeMesh.draw(this->gl_variables->current_shader);

        glfwSwapBuffers(this->main_window);
        glfwPollEvents();
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

void Engine::calcPlayerView()
{
    glm::vec3 player_pos = this->player.player_pos;
    double xpos, ypos;
    glfwGetCursorPos(this->main_window, &xpos, &ypos);

    float scr_w = this->hw_specs.scr_w, scr_h = this->hw_specs.scr_h;
    glfwSetCursorPos(this->main_window, scr_w / 2, scr_h / 2);
    Engine::Options& options = this->options;
    Engine::Player& player = this->player;

    auto delta_time = this->delta_time;
    player.horizontal_angle += options.mouse_speed * delta_time * (scr_w / 2 - xpos);
    auto vert_angle_increment = options.mouse_speed * delta_time * (scr_h / 2 - ypos);
    if (player.vertical_angle + vert_angle_increment > 3.14)
        player.vertical_angle = 3.14;
    else if (player.vertical_angle + vert_angle_increment < -3.14)
        player.vertical_angle = -3.14;
    else
        player.vertical_angle += vert_angle_increment;
    glm::vec3 direction( cos(player.vertical_angle) * sin(player.horizontal_angle),
                         sin(player.vertical_angle),
                         cos(player.vertical_angle) * cos(player.horizontal_angle));
    glm::vec3 right(sin(player.horizontal_angle - 3.14 / 2.0),
                    0,
                    cos(player.horizontal_angle - 3.14 / 2.0));
    glm::vec3 up = glm::cross(right, direction);

    player.direction = direction;
    player.up = up;
    player.right = right;
}

glm::mat4 Engine::calculateMVP(float ratio, float nearz, float farz)
{
    glm::mat4 proj = glm::perspective(glm::radians(this->options.player_fov), ratio, nearz, farz);
    glm::mat4 view = glm::lookAt(this->player.player_pos,
                                 this->player.direction + this->player.player_pos,
                                 this->player.up);
    glm::mat4 model;
    //model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.5f);
    //model = glm::rotate(model, 45.0f, glm::vec3(0, 0, 1));
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
    if (glfwGetKey(this->main_window, GLFW_KEY_R) == GLFW_PRESS)
    {
        glfwSetInputMode(this->main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
