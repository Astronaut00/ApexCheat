#pragma once
#include "cheat.h"
#include "render.hpp"
#include <d3d11.h>
#include <thread>
#include <mutex>

const ImColor selectedColor = ImColor(255, 255, 0, 255);
const ImColor unselectedColor = ImColor(255, 255, 255, 255);
const ImColor disabledColor = ImColor(255, 0, 0, 255);
const ImColor enabledColor = ImColor(0, 255, 0, 255);
const ImColor sliderText = ImColor(100, 3, 252);

enum itemType
{
    TYPE_FLOAT = 0,
    TYPE_BOOL,
    TYPE_SWITCH
};

struct MenuItem
{
    int type = -1;
    int index = 0;
    std::string label = "";
    void* value = 0;
    std::vector<std::string> optionLabels = {};

    void UpdateValue(int key = NULL)
    {
        if (value == NULL)
            return;
        switch (type)
        {
        case TYPE_BOOL:
            *(bool*)value = !*(bool*)value;
            break;
        case TYPE_FLOAT:
            if (key == VK_RIGHT)
                *(float*)value = *(float*)value + 1;
            else if (key == VK_LEFT)
                *(float*)value = *(float*)value - 1;
            break;
        case TYPE_SWITCH:
            if (key == VK_RIGHT)
                *(int*)value = *(int*)value + 1;
            else if (key == VK_LEFT)
                *(int*)value = *(int*)value - 1;
            break;
        }
    }
};


class TextMenu
{
public:
    TextMenu();
    bool complete = false;
    bool enabled = false;
    std::vector<MenuItem> menuItems{ MenuItem{} }; //skip the first index so that we can have a "menu" field
    static unsigned int selectedOption;
    void AddMenuItem(std::string label, bool* value)
    {
        if (complete) return;
        int index = menuItems.size();
        menuItems.push_back(MenuItem{ TYPE_BOOL, index, label, value });
    }

    void AddMenuItem(std::string label, float* value)
    {
        if (complete) return;
        int index = menuItems.size();
        menuItems.push_back(MenuItem{ TYPE_FLOAT, index, label, value });
    }

    void AddMenuItem(std::string label, int* value, std::vector<std::string> optionLabels)
    {
        if (complete) return;
        int index = menuItems.size();
        menuItems.push_back(MenuItem{ TYPE_SWITCH, index, label, value, optionLabels });
    }

    void RenderBool(MenuItem& item)
    {
        bool selected = (item.index == selectedOption);

        if (selected)
        {
            ImGui::TextColored(selectedColor, item.label.c_str());
            ImGui::SameLine(100);
            ImGui::TextColored(selectedColor, "%s", *(bool*)item.value ? "[ON]" : "[OFF]");
        }
        else
        {
            ImGui::TextColored(unselectedColor, item.label.c_str());
            ImGui::SameLine(100);
            ImGui::TextColored((*(bool*)item.value) ? enabledColor : disabledColor, "[%s]", *(bool*)item.value ? "ON" : "OFF");
        }
    }

    void RenderFloat(MenuItem& item)
    {
        bool selected = (item.index == selectedOption);
        ImGui::TextColored((!selected) ? unselectedColor : selectedColor, item.label.c_str());
        ImGui::SameLine(100);
        ImGui::TextColored((!selected) ? sliderText : selectedColor, "[%.0f]", *(float*)item.value);
    }

    void RenderSwitch(MenuItem& item)
    {
        bool selected = (item.index == selectedOption);
        ImGui::TextColored((!selected) ? unselectedColor : selectedColor, item.label.c_str());
        ImGui::SameLine(100);
        int currentPosition = *(int*)item.value % item.optionLabels.size();
        ImGui::TextColored((!selected) ? sliderText : selectedColor, "[%s]", item.optionLabels[currentPosition].c_str());

    }
    void HandleInput();
    void RenderMenuItems();
}; extern TextMenu* textMenu;