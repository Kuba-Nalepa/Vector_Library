#ifndef TUIINTERFACE_H
#define TUIINTERFACE_H

#include "ftxui/component/screen_interactive.hpp"

class TuiInterface
{
public:
    TuiInterface();
    ~TuiInterface();

    void render();

private:
    ftxui::ScreenInteractive screen;
};

#endif