#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <src/engine/model.hpp>
#include <src/common/resource_loader.hpp>
#include <src/engine/world.hpp>
#include <map>
#include <vector>
#include <string>

#include "lib/imgui/imgui.h"
#include "lib/imgui/examples/imgui_impl_glfw.h"
#include "lib/imgui/examples/imgui_impl_opengl3.h"

class Engine
{
public:
    Engine();
    void mainloop();
    void init();
    void calcPlayerView();
    void checkKeyPresses();
    void pollTime();
    void resetCamera();
    glm::mat4 calculateMVP(float ratio, float nearz, float farz);

    void loopImGui(bool show_demo_window, bool show_another_window, ImVec4& clear_color, GLFWwindow* window);
    void initImGui();
    void destroyImGui();

    GLFWwindow * main_window;
    ResourceLoader resource_loader = ResourceLoader();
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
        float player_speed;
        float mouse_speed;
        float player_fov;
    } options;
    struct _HWSpecs
    {
        float scr_h;
        float scr_w;
    } hw_specs;
    struct _Player
    {
        glm::vec3 player_pos;
        float horizontal_angle;
        float vertical_angle;
        glm::vec3 direction, up, right;
    } player;
private:
    World world;
    int MouseInputMode;
};
#endif
