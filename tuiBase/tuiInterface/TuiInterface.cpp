#include "TuiInterface.hpp"

#include <iostream>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

TuiInterface::TuiInterface()
    : screen(ScreenInteractive::Fullscreen()) {}

TuiInterface::~TuiInterface() {}

void TuiInterface::render()
{
    std::vector<std::string> menuEntries = {
        "Dodaj studenta",
        "Usuń studenta",
        "Edytuj studenta",
        "Wyświetl wszystkich",
        "Policz studentów",
        "Wczytaj z pliku",
        "Zapisz do pliku",
        "Pokaż zakres studentów",
        "Pokaż studenta pod indeksem",
        "Szukaj po nazwisku",
        "Szukaj po numerze indeksu",
        "Średnia ocen studentów",
        "Średnia ocen dla nazwiska",
        "Wyjście"};

    bool isWorking = true;
    int selectedMenuEntry = 0;

    MenuOption menuOpt;
    menuOpt.on_enter = screen.ExitLoopClosure();

    auto menu = Menu(&menuEntries, &selectedMenuEntry, menuOpt);
    auto elementsNavigation = Container::Vertical({menu});

    auto rend = Renderer(elementsNavigation, [&]
                         { return vbox({text("SYSTEM ZARZĄDZANIA STUDENTAMI") | center | bold | color(Color::Cyan),
                                        separator(),
                                        menu->Render()}) |
                                  border; });

    while (isWorking)
    {
        screen.Loop(rend);
        screen.Clear();

        if (menuEntries[selectedMenuEntry] == "Wyjście")
        {
            isWorking = false;
        }
        else
        {
            showSubScreen(menuEntries[selectedMenuEntry]);
        }
    }
}

void TuiInterface::showSubScreen(const std::string &optionName)
{
    auto backButton = Button("Powrót do menu", screen.ExitLoopClosure());
    auto elementsNavigation = Container::Vertical({backButton});

    auto rend = Renderer(elementsNavigation, [&]
                         { return vbox({text("Wybrano: " + optionName) | bold | color(Color::Yellow),
                                        separator(),
                                        text("To jest ekran podmenu dla: " + optionName),
                                        separator(),
                                        backButton->Render() | center}) |
                                  border; });

    screen.Loop(rend);
}

Element TuiInterface::renderSubScreen(const std::string &optionName)
{
    return vbox({text("Wybrano: " + optionName) | bold | color(Color::Yellow),
                 separator(),
                 text("To jest ekran podmenu dla: " + optionName)}) |
           border;
}