#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <src/engine/GUI.hpp>
#include <src/engine/Player.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <src/common/ResourceLoader.h>
#include <src/engine/World.h>
#include <map>
#include <vector>
#include <string>
#include <src/common/Shader.h>
#include "Model.h"

#define DBG_LEVEL_WARNING 0x0
#define DBG_LEVEL_ERROR 0x1
#define DBG_LEVEL_FATAL 0x2

class Engine
{
public:
    Engine();
    void mainloop();
    void init();
    void pollTime();
    void loadShader(std::string name);
    void useShader(std::string name);
    void debugPrint(unsigned char level, std::string message);
    void quit();
    glm::mat4 calculateMVP(float ratio, float nearz, float farz);

    GLFWwindow* main_window;
    ResourceLoader resource_loader = ResourceLoader();
    GUI EngineGUI = GUI();
    Player GamePlayer = Player();
    double last_time;
    double delta_time;
    struct GLVariables
    {
        std::map<std::string, Shader *> shaders;
        Shader * current_shader;
    } * gl_variables = new GLVariables;
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
    World world;
};
#endif
