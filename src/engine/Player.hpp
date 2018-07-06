#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player
{
public:
    Player();

    // Functiions
    void Init(GLFWwindow* InWindow, float scrW, float scrH);
    void CalcPlayerView(int MouseInputMode, double indelta_time, float mouse_speed);
    void ResetPlayerCamera();
    void CheckKeyPresses(double indelta_time);

    // Variables
    glm::vec3 player_pos;
    float horizontal_angle;
    float vertical_angle;
    glm::vec3 direction, up, right;
    float player_speed;
    float player_fov;
private:
    GLFWwindow* Window;
    float ScreenWidth;
    float ScreenHeight;
};
#endif