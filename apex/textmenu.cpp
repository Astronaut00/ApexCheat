#include "textmenu.h"

TextMenu* textMenu = new TextMenu();
unsigned int TextMenu::selectedOption = 0;

TextMenu::TextMenu()
{
    selectedOption = 0;
}

void TextMenu::HandleInput()
{
    if (enabled)
    {
        {
            static bool upWasDown = false;
            if (keydown(VK_UP))
                upWasDown = true;
            else if (upWasDown)
            {
                selectedOption--;
                upWasDown = false;
            }

            static bool downWasDown = false;
            if (keydown(VK_DOWN))
                downWasDown = true;
            else if (downWasDown)
            {
                selectedOption++;
                downWasDown = false;
            }

            selectedOption = std::clamp((unsigned int)(selectedOption), 0u, unsigned int(menuItems.size() - 1));
        }

        {

            static bool leftWasDown = false;
            static int leftCounter = 0;
            if (keydown(VK_LEFT))
            {
                leftCounter++;

                if (leftCounter > 25) 
                {
                    if (leftCounter % 3 == 0)
                        menuItems[selectedOption].UpdateValue(VK_LEFT);
                }
                leftWasDown = true;
            }
            else if (leftWasDown)
            {
                leftCounter = 0;
                menuItems[selectedOption].UpdateValue(VK_LEFT);
                leftWasDown = false;
            }
        }


        {
            static bool rightWasDown = false;
            static int rightCounter = 0;
            if (keydown(VK_RIGHT))
            {
                rightCounter++;
                if (rightCounter > 25)
                {
                    if (rightCounter % 3 == 0)
                        menuItems[selectedOption].UpdateValue(VK_RIGHT);
                }
                rightWasDown = true;
            }
            else if (rightWasDown)
            {
                rightCounter = 0;
                menuItems[selectedOption].UpdateValue(VK_RIGHT);
                rightWasDown = false;
            }
        }
    }

}

void TextMenu::RenderMenuItems()
{
    complete = true;

    auto start = ImVec2(10, 5);

    for (int i = 1; i < menuItems.size(); i++)
    {
        auto& item = menuItems[i];
        switch (item.type)
        {
        case TYPE_BOOL:
            RenderBool(item);
            break;
        case TYPE_FLOAT:
            RenderFloat(item);
            break;
        case TYPE_SWITCH:
            RenderSwitch(item);
            break;
        }
    }
}