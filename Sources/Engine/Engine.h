#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <vector>
#include <string>
#include <functional>

#include <Sources/Engine/ConsoleHandler.h>
#include <Sources/Engine/GUI/GUI.h>
#include <Sources/Engine/GUI/ConsoleGUI.h>
#include <Sources/Engine/Player.h>
#include <Sources/Engine/Model.h>
#include <Sources/Common/ResourceLoader.h>
#include <Sources/Engine/World.h>
#include <Sources/Common/Shader.h>
#include <Sources/Engine/Constants.h>
#include <Sources/Engine/ConsoleHandler.h>
#include <Sources/Engine/Object.h>


class Engine
{

public:
    static Engine * getEngine() {return Engine::self;}
    Engine();
    void mainloop();
    void initgl();
    void pollTime();
    void checkKeyPresses();
    void loadShader(std::string name);
    void useShader(std::string name);
    void debugPrint(unsigned char level, std::string message);
    void quit();
    glm::mat4 calculateMVP(float ratio, float nearz, float farz);

    GLFWwindow* main_window;
    ResourceLoader resource_loader = ResourceLoader();
    GUI * EngineGUI;
    Player * PlayerObject;
    double last_time;
    double delta_time;
    struct GLVariables
    {
        std::map<std::string, Shader *> shaders;
        Shader * current_shader;
    } * gl_variables;
    struct Options
    {
        float mouse_speed;
    } options;
    struct HWSpecs
    {
        float scr_h;
        float scr_w;
    } hw_specs;
    
private:
    static Engine * self; //pointer to self
    World * world;
    ConsoleHandler * ConsoleHandlerObject;
};
#endif
