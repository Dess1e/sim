#include <Sources/Engine/Player.h>

Player::Player(GLFWwindow* InWindow, float scrW, float scrH)
{
    this->player_speed = 3;
    this->player_pos = glm::vec3(3, 0, 3);
    this->horizontal_angle = 0;
    this->vertical_angle = 0;
    this->player_fov = 90.0;

    this->Window = InWindow;
    this->ScreenWidth = scrW;
    this->ScreenHeight = scrH;
}

void Player::CalcPlayerView(int MouseInputMode, double indelta_time, float mouse_speed)
{
    // Check mouse mode
    if(MouseInputMode == GLFW_CURSOR_DISABLED)
    {
        double xpos, ypos;
        glfwGetCursorPos(Window, &xpos, &ypos);

        float scr_w = this->ScreenWidth;
        float scr_h = this->ScreenHeight;
        glfwSetCursorPos(Window, scr_w / 2, scr_h / 2);

        double delta_time = indelta_time;
        horizontal_angle += mouse_speed * delta_time * (this->ScreenWidth / 2 - xpos);
        auto vert_angle_increment = mouse_speed * delta_time * (this->ScreenHeight / 2 - ypos);

        if (vertical_angle + vert_angle_increment > 3.14)
        {
            vertical_angle = 3.14;
        }
        else if (vertical_angle + vert_angle_increment < -3.14)
        {
            vertical_angle = -3.14;
        }
        else
        {
            vertical_angle += vert_angle_increment;
        }

        glm::vec3 indirection( cos(vertical_angle) * sin(horizontal_angle),
                             sin(vertical_angle),
                             cos(vertical_angle) * cos(horizontal_angle));
        glm::vec3 inright(sin(horizontal_angle - 3.14 / 2.0),
                        0,
                        cos(horizontal_angle - 3.14 / 2.0));
        glm::vec3 inup = glm::cross(right, direction);

        this->direction = indirection;
        this->right = inright;
        this->up = inup;
    }
}

void Player::ResetPlayerCamera()
{
    horizontal_angle = 3.3;
    vertical_angle = 0;
}

