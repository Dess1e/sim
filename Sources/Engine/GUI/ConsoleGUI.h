#ifndef CONSOLEGUI_H
#define CONSOLEGUI_H
#include <ThirdParty/imgui/imgui.h>
#include <functional>
#include <Sources/Engine/Constants.h>

class ConsoleHandler;

class ConsoleGUI
{
public:
    ConsoleGUI();
    ImGuiTextBuffer Buf;
    bool ScrollToBottom;
    bool is_enabled;
    void Clear();
    void AddLog(const char* fmt, ...);
    void Draw(bool* p_opened = NULL);
    void setConsoleHandlerPointer(ConsoleHandler * ptr) {this->ConsoleHandlerObject = ptr;}
private:
    char * console_label;
    char * input_line_buffer;
    ConsoleHandler * ConsoleHandlerObject;
};

#endif
