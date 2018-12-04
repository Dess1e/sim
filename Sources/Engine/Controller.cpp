#include "Controller.h"
#include <GLFW/glfw3.h>
#include "Engine.h"

void awd(float dt)
{
    auto e = Engine::getEngine();
    e->PlayerObject->player_pos += e->PlayerObject->direction * dt * e->PlayerObject->player_speed;
}

Controller::Controller()
{
    this->key_map[GLFW_KEY_W] = std::function<void(float)>(awd);
}

void Controller::processKeyPress(int keycode, int status)
{
    while(!cb_mutex.try_lock());

    auto& v = this->pressed_keys;
    auto f_result = std::find(v.begin(), v.end(), keycode);
    if (f_result == v.end())
    {
        if (status == GLFW_PRESS)
            v.push_back(keycode);
    }
    else
    {
        if (status == GLFW_RELEASE)
            v.erase(f_result);

    }
    cb_mutex.unlock();
}

void Controller::tickController(float deltatime)
{
    while(!cb_mutex.try_lock());
    for (int key : this->pressed_keys)
    {
        if (this->key_map.find(key) == this->key_map.end())
        {
            auto f = this->key_map[key];
            f(deltatime);
        }
    }
    cb_mutex.unlock();
}
