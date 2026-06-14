#ifndef TUIINTERFACE_H
#define TUIINTERFACE_H

#include <string>
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "SmartArray.hpp"

struct Student
{
    std::string firstName;
    std::string lastName;
    std::string department;
    std::string fieldOfStudy;
    int currentYear;
    double gradesAvg;
};

class TuiInterface
{
public:
    TuiInterface();
    ~TuiInterface();

    void render();

private:
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::TerminalOutput();

    int selectedMenuEntry = 0;
    int currentScreen = 0;
    void showSubScreen(const std::string& optionName);
    ftxui::Element renderSubScreen(const std::string& optionName);

    SmartArray<Student> m_database;
};

#endif