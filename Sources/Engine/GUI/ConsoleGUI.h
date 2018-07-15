#ifndef CONSOLEGUI_H
#define CONSOLEGUI_H
#include <ThirdParty/imgui/imgui.h>

#define CONSOLE_INPUT_LINE_BUFFER_SIZE 128
#define CONSOLE_DEFAULT_LABEL "Console"


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
private:
    char * console_label;
    char * input_line_buffer;
};

#endif
