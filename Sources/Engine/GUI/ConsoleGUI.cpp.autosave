#include "ConsoleGUI.h"
#include <Sources/Engine/ConsoleHandler.h>

ConsoleGUI::ConsoleGUI()
{
    this->is_enabled = false;
    this->input_line_buffer = new char[CONSOLE_INPUT_LINE_BUFFER_SIZE];
    this->input_line_buffer[0] = '\0'; //so there will be '' string at start
    this->console_label = (char *)CONSOLE_DEFAULT_LABEL;
}

void ConsoleGUI::Clear()
{
    this->Buf.clear();
}

void ConsoleGUI::AddLog(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    this->Buf.appendfv(fmt, args);
    va_end(args);
}

void ConsoleGUI::Draw(bool *p_opened)
{
    ImGui::Begin(CONSOLE_DEFAULT_LABEL, p_opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::BeginChild("console_input", ImVec2(-1, ImGui::GetWindowHeight() - 60.0f), true);
    ImGui::TextUnformatted(Buf.begin());
    ImGui::SetScrollHere(1.0f);
    ImGui::EndChild();
    ImGui::SetKeyboardFocusHere();
    ImGui::PushItemWidth(-1);
    if (ImGui::InputText("", this->input_line_buffer, CONSOLE_INPUT_LINE_BUFFER_SIZE,
                         ImGuiInputTextFlags_EnterReturnsTrue))
    {
        auto str = this->ConsoleHandlerObject->ParseCommand(this->input_line_buffer, CONSOLE_INPUT_LINE_BUFFER_SIZE);
        this->AddLog("> %s\n%s\n", this->input_line_buffer, str.c_str());
        memset(this->input_line_buffer, 0, CONSOLE_INPUT_LINE_BUFFER_SIZE);
    }
    ImGui::PopItemWidth();

    ImGui::End();
}
