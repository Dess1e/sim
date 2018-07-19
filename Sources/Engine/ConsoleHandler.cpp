#include "ConsoleHandler.h"
#include <Sources/Engine/Engine.h>

ConsoleHandler::ConsoleHandler(Engine * engine)
{
    this->engine = engine;
}

std::string ConsoleHandler::ParseCommand(char *input, unsigned int input_len)
{
    std::string input_str(input);
    if (input_str == "clear")
    {
        this->engine->EngineGUI->GetConsoleGUI()->Clear();
        return "";
    }
    else if (input_str == "help")
        return "Here should be help";
    else if (input_str == "quit" || input_str == "exit")
        this->engine->quit();
    else
        return "Unable to parse command.";
    return "Error"; //if somehow control gets here
}
