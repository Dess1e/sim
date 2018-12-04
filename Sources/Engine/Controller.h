#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <map>
#include <vector>
#include <functional>
#include <mutex>


class Controller
{
public:
    Controller();
    void processKeyPress(int keycode, int status);
    void tickController(float deltatime);
private:
    std::map<int, std::function<void(float)>> key_map;
    std::vector<int> pressed_keys;
    std::mutex cb_mutex;
};

#endif // CONTROLLER_H
