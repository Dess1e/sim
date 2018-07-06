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
#include <src/engine/model.hpp>
#include <src/common/resource_loader.hpp>
#include <src/engine/world.hpp>
#include <map>
#include <vector>
#include <string>

class Engine
{
public:
    Engine();
    void mainloop();
    void init();
    void checkKeyPresses();
    void pollTime();
    glm::mat4 calculateMVP(float ratio, float nearz, float farz);

    GLFWwindow* main_window;
    ResourceLoader resource_loader = ResourceLoader();
    GUI EngineGUI = GUI();
    Player GamePlayer = Player();
    double last_time;
    double delta_time;
    struct _GLVariables
    {
        GLuint vertex_array_id;
        GLuint vertex_buffer;
        GLuint color_buffer;
        GLuint shaders_id;
    } gl_variables;
    struct _Options
    {
        float mouse_speed;
    } options;
    struct _HWSpecs
    {
        float scr_h;
        float scr_w;
    } hw_specs;
    
private:
    World world;
    int MouseInputMode;
};
#endif
