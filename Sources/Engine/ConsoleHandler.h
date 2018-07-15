#ifndef CONSOLE_HANDLER_H
#define CONSOLE_HANDLER_H
#include <string>


class Engine;

class ConsoleHandler
{
public:
    ConsoleHandler(Engine * engine);
    std::string ParseCommand(char * input, unsigned int input_len);
private:
    Engine * engine;
};



#endif
