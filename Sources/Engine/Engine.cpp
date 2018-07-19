#include <Sources/Engine/Engine.h>
/*	#TODO
 *
 * Engine v2!
 * 1. Make private what should be private and create getters and setters
 * 2. Rm player class add camera object (calculate mvps there)
 * 3. Try to make variables visible to console to manipulate 'em in runtime
 * 4. Add callback, do not poll everything
 * 5. Init hw specs dynamically
 * 6. Exceptions...
 *
*/
Engine * Engine::self;

Engine::Engine()
{
    //init static ptr to self
    self = this;

    //init opengl and glfw at first
    this->initgl();

    //Gui init
    this->EngineGUI = new GUI(this->main_window);

    //init console handler
    this->ConsoleHandlerObject = new ConsoleHandler(this);

    //pass consolehandler pointer to consolegui
    this->EngineGUI->GetConsoleGUI()->setConsoleHandlerPointer(this->ConsoleHandlerObject);

    // Player init
    this->PlayerObject = new Player(this->main_window, this->hw_specs.scr_w, this->hw_specs.scr_h);

    this->world = new World();

    this->options.mouse_speed = 0.3;
    this->hw_specs.scr_h = 768;
    this->hw_specs.scr_w = 1366;

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

void Engine::checkKeyPresses()
{
    /*
     *
     * 	THIS UGLY SHITTY FUNCTION SHOULD BE UPDATED
     *  stop polling events, add callbacks where they needed
     *  add toggle functions for some objects such as gui
     *
    */

    auto delta_time = this->delta_time;
    auto Window = this->main_window;
    //ugly workaround should be replaced with callbacks
    static int old_state = GLFW_RELEASE;
    if(glfwGetKey(Window, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
        old_state = GLFW_RELEASE;
    if(glfwGetKey(Window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
    {
        if (old_state == GLFW_RELEASE)
        {
            this->EngineGUI->show_console ^= true;
            old_state = GLFW_PRESS;
        }
    }
    if (!this->EngineGUI->show_console)
    {
    //all the moving shit
        if (glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            this->PlayerObject->player_speed = 9;
        }
        else
        {
            this->PlayerObject->player_speed = 3;
        }
        if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
        {
            this->PlayerObject->player_pos += this->PlayerObject->direction * float(delta_time) * this->PlayerObject->player_speed;
        }
        if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
        {
            this->PlayerObject->player_pos -= this->PlayerObject->right * float(delta_time) * this->PlayerObject->player_speed;
        }
        if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
        {
            this->PlayerObject->player_pos -= this->PlayerObject->direction * float(delta_time) * this->PlayerObject->player_speed;
        }
        if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
        {
            this->PlayerObject->player_pos += this->PlayerObject->right * float(delta_time) * this->PlayerObject->player_speed;
        }
        if (glfwGetKey(Window, GLFW_KEY_C) == GLFW_PRESS)
        {
            this->PlayerObject->ResetPlayerCamera();
        }
    }


    //NIKITA NA ZAMETKY!!, VMESTO ETOGO GAVNO LISHNEGO SZNIZU MOJNO SDELAT' TAK	   >---------]
    //																						 |
    //																						 |
    /*																						 |
    if (glfwGetKey(Window, GLFW_KEY_M) == GLFW_PRESS)										 |
    {																						 |
        this->EngineGUI->MouseInputMode  = GLFW_CURSOR_NORMAL;								 |
        glfwSetInputMode(Window, GLFW_CURSOR, this->EngineGUI->MouseInputMode);				 |
    }																						 |
                                                                                             |
    if (glfwGetKey(Window, GLFW_KEY_N) == GLFW_PRESS)										 |
    {																						 |
        this->EngineGUI->MouseInputMode  = GLFW_CURSOR_DISABLED;							 |
        glfwSetInputMode(Window, GLFW_CURSOR, this->EngineGUI->MouseInputMode);				 |
    }																						 |
    *///																					 |
    int& MouseInputMode = (this->EngineGUI->MouseInputMode);//					   <---------]
    if (glfwGetKey(Window, GLFW_KEY_M) == GLFW_PRESS)
        if (MouseInputMode == GLFW_CURSOR_DISABLED)
            (MouseInputMode = GLFW_CURSOR_NORMAL, glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));
        else
            (MouseInputMode = GLFW_CURSOR_DISABLED, glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
}

void Engine::mainloop()
{
    auto x = Object(glm::vec3(2, 2, 2),
                    glm::vec3(30, 30, 30),
                    glm::vec3(2, 5, 2),
                    "Resources/nanosuit.obj");
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    do
    {
        this->pollTime();
        PlayerObject->CalcPlayerView(this->EngineGUI->MouseInputMode, this->delta_time, this->options.mouse_speed);
        this->checkKeyPresses();


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        x.render(); //if you render before clear you can't see it
        glfwPollEvents();

        EngineGUI->Draw();
    }
    while (glfwGetKey(this->main_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(this->main_window) == 0);
    this->quit();
}

void Engine::initgl()
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

    this->loadShader("simple");
    this->useShader("simple");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}



void Engine::pollTime()
{
    auto curr_time = glfwGetTime();
    this->delta_time = curr_time - this->last_time;
    this->last_time = curr_time;
}

