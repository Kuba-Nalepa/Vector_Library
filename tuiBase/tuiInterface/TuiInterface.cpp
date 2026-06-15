#include "TuiInterface.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
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
        "Wyjście"};

    bool isWorking = true;
    int selectedMenuEntry = 0;

    MenuOption menuOpt;
    menuOpt.on_enter = screen.ExitLoopClosure();

    auto menu = Menu(&menuEntries, &selectedMenuEntry, menuOpt);
    auto elementsNavigation = Container::Vertical({menu});

    auto rend = Renderer(elementsNavigation, [&]
                         { return vbox({text("PROJEKT C++") | center | bold | color(Color::Cyan),
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
    if (optionName == "Dodaj studenta")
    {
        std::string firstName;
        std::string lastName;
        std::string department;
        std::string fieldOfStudy;
        std::string yearInput;
        std::string gradeInput;

        auto inputFirstName = Input(&firstName, "Imię");
        auto inputLastName = Input(&lastName, "Nazwisko");
        auto inputDepartment = Input(&department, "Kierunek");
        auto inputFieldOfStudy = Input(&fieldOfStudy, "Specjalność");
        auto inputYear = Input(&yearInput, "Rok studiów");
        auto inputGrade = Input(&gradeInput, "Średnia ocen");
        auto saveButton = Button("Dodaj", [&]
                                 {
            Student student;
            student.firstName = firstName;
            student.lastName = lastName;
            student.department = department;
            student.fieldOfStudy = fieldOfStudy;
            student.currentYear = std::stoi(yearInput.empty() ? "0" : yearInput);
            student.gradesAvg = std::stod(gradeInput.empty() ? "0" : gradeInput);
            m_database.pushBack(student);
            screen.ExitLoopClosure()(); });
        auto backButton = Button("Powrót", screen.ExitLoopClosure());

        auto container = Container::Vertical({inputFirstName, inputLastName, inputDepartment, inputFieldOfStudy, inputYear, inputGrade, saveButton, backButton});
        auto rend = Renderer(container, [&]
                             { return vbox({
                                          text("Dodawanie studenta") | bold | color(Color::Cyan),
                                          separator(),
                                          hbox(text("Imię:            ") | color(Color::White), inputFirstName->Render()),
                                          hbox(text("Nazwisko:        ") | color(Color::White), inputLastName->Render()),
                                          hbox(text("Kierunek:        ") | color(Color::White), inputDepartment->Render()),
                                          hbox(text("Specjalność:     ") | color(Color::White), inputFieldOfStudy->Render()),
                                          hbox(text("Rok studiów:     ") | color(Color::White), inputYear->Render()),
                                          hbox(text("Średnia ocen:    ") | color(Color::White), inputGrade->Render()),
                                          separator(),
                                          hbox({saveButton->Render(), text("   "), backButton->Render()}) | center,
                                      }) |
                                      border; });

        screen.Loop(rend);
        return;
    }

    if (optionName == "Usuń studenta")
    {
        std::string indexInput;
        auto inputIndex = Input(&indexInput, "Numer indeksu do usunięcia");
        auto deleteButton = Button("Usuń", [&]
                                   {
                                       try
                                       {
                                           unsigned index = static_cast<unsigned>(std::stoi(indexInput));
                                           if (index < m_database.size())
                                           {
                                               m_database.erase(index);
                                           }
                                       }
                                       catch (...)
                                       {
                                       }
                                       screen.ExitLoopClosure()(); });
        auto backButton = Button("Powrót", screen.ExitLoopClosure());

        auto container = Container::Vertical({inputIndex, deleteButton, backButton});
        auto rend = Renderer(container, [&]
                             { return vbox({
                                          text("Usuwanie studenta") | bold | color(Color::RedLight),
                                          separator(),
                                          text("Podaj indeks studenta z listy wszystkich studentów."),
                                          hbox(text("Indeks: ") | color(Color::White), inputIndex->Render()),
                                          separator(),
                                          hbox({deleteButton->Render(), text("   "), backButton->Render()}) | center,
                                      }) |
                                      border; });

        screen.Loop(rend);
        return;
    }

    if (optionName == "Edytuj studenta")
    {
        if (m_database.size() == 0)
        {
            auto backButton = Button("Powrót", screen.ExitLoopClosure());
            auto container = Container::Vertical({backButton});
            auto rend = Renderer(container, [&]
                                 { return vbox({text("Brak studentów do edycji.") | color(Color::Yellow),
                                                separator(),
                                                backButton->Render() | center}) |
                                          border; });
            screen.Loop(rend);
            return;
        }

        std::string indexInput;
        std::string firstName;
        std::string lastName;
        std::string department;
        std::string fieldOfStudy;
        std::string yearInput;
        std::string gradeInput;
        int selectedStudent = -1;

        auto inputIndex = Input(&indexInput, "Indeks studenta do edycji");

        auto loadButton = Button("Wczytaj studenta", [&]
                                 {
                                     try
                                     {
                                         unsigned index = static_cast<unsigned>(std::stoi(indexInput));
                                         if (index < m_database.size())
                                         {
                                             const Student &s = m_database.at(index);
                                             firstName = s.firstName;
                                             lastName = s.lastName;
                                             department = s.department;
                                             fieldOfStudy = s.fieldOfStudy;
                                             yearInput = std::to_string(s.currentYear);
                                             gradeInput = std::to_string(s.gradesAvg);
                                             selectedStudent = static_cast<int>(index);
                                         }
                                     }
                                     catch (...)
                                     {
                                     } });

        auto inputFirstName = Input(&firstName, "Imię");
        auto inputLastName = Input(&lastName, "Nazwisko");
        auto inputDepartment = Input(&department, "Kierunek");
        auto inputFieldOfStudy = Input(&fieldOfStudy, "Specjalność");
        auto inputYear = Input(&yearInput, "Rok studiów");
        auto inputGrade = Input(&gradeInput, "Średnia ocen");

        auto saveButton = Button("Zapisz zmiany", [&]
                                 {
                                     if (selectedStudent >= 0 && static_cast<unsigned>(selectedStudent) < m_database.size())
                                     {
                                         Student &student = m_database[selectedStudent];
                                         student.firstName = firstName;
                                         student.lastName = lastName;
                                         student.department = department;
                                         student.fieldOfStudy = fieldOfStudy;
                                         student.currentYear = std::stoi(yearInput.empty() ? "0" : yearInput);
                                         student.gradesAvg = std::stod(gradeInput.empty() ? "0" : gradeInput);
                                     }
                                     screen.ExitLoopClosure()(); });
        auto backButton = Button("Powrót", screen.ExitLoopClosure());

        auto container = Container::Vertical({inputIndex, loadButton, inputFirstName, inputLastName, inputDepartment, inputFieldOfStudy, inputYear, inputGrade, saveButton, backButton});
        auto rend = Renderer(container, [&]
                             { return vbox({
                                          text("Edycja studenta") | bold | color(Color::Yellow),
                                          separator(),
                                          text("Wpisz indeks studenta, wczytaj go, a następnie edytuj dane."),
                                          hbox(text("Indeks: ") | color(Color::White), inputIndex->Render()),
                                          loadButton->Render(),
                                          separator(),
                                          hbox(text("Imię:            ") | color(Color::White), inputFirstName->Render()),
                                          hbox(text("Nazwisko:        ") | color(Color::White), inputLastName->Render()),
                                          hbox(text("Kierunek:        ") | color(Color::White), inputDepartment->Render()),
                                          hbox(text("Specjalność:     ") | color(Color::White), inputFieldOfStudy->Render()),
                                          hbox(text("Rok studiów:     ") | color(Color::White), inputYear->Render()),
                                          hbox(text("Średnia ocen:    ") | color(Color::White), inputGrade->Render()),
                                          separator(),
                                          hbox({saveButton->Render(), text("   "), backButton->Render()}) | center,
                                      }) |
                                      border; });

        screen.Loop(rend);
        return;
    }

    if (optionName == "Policz studentów")
    {
        auto backButton = Button("Powrót", screen.ExitLoopClosure());
        auto container = Container::Vertical({backButton});

        auto rend = Renderer(container, [&]
                             {
                                 const unsigned count = m_database.size();
                                 return vbox({
                                            text("Liczba studentów") | bold | color(Color::Cyan),
                                            separator(),
                                            text("Aktualnie zapisanych studentów: " + std::to_string(count)) | color(Color::White),
                                            separator(),
                                            backButton->Render() | center,
                                        }) |
                                        border; });

        screen.Loop(rend);
        return;
    }

    if (optionName == "Wczytaj z pliku")
    {
        auto loadButton = Button("Wczytaj teraz", [&]
                                 {
                                     std::ifstream file("students.txt", std::ios::in);
                                     if (file.is_open())
                                     {
                                         m_database.clear();
                                         std::string line;
                                         while (std::getline(file, line))
                                         {
                                             std::stringstream ss(line);
                                             std::string token;
                                             std::vector<std::string> values;
                                             while (std::getline(ss, token, ','))
                                             {
                                                 values.push_back(token);
                                             }

                                             if (values.size() == 6)
                                             {
                                                 Student student;
                                                 student.firstName = values[0];
                                                 student.lastName = values[1];
                                                 student.department = values[2];
                                                 student.fieldOfStudy = values[3];
                                                 student.currentYear = std::stoi(values[4]);
                                                 student.gradesAvg = std::stod(values[5]);
                                                 m_database.pushBack(student);
                                             }
                                         }
                                         file.close();
                                     }
                                     screen.ExitLoopClosure()(); });
        auto backButton = Button("Powrót", screen.ExitLoopClosure());

        auto container = Container::Vertical({loadButton, backButton});
        auto rend = Renderer(container, [&]
                             { return vbox({
                                          text("Wczytaj z pliku") | bold | color(Color::Cyan),
                                          separator(),
                                          text("Czy chcesz wczytać?"),
                                          separator(),
                                          loadButton->Render() | center,
                                          backButton->Render() | center,
                                      }) |
                                      border; });

        screen.Loop(rend);
        return;
    }

    if (optionName == "Zapisz do pliku")
    {
        auto saveButton = Button("Zapisz teraz", [&]
                                 {
                                     std::ofstream file("students.txt", std::ios::out | std::ios::trunc);
                                     if (file.is_open())
                                     {
                                         for (unsigned i = 0; i < m_database.size(); ++i)
                                         {
                                             const Student &s = m_database.at(i);
                                             file << s.firstName << ","
                                                  << s.lastName << ","
                                                  << s.department << ","
                                                  << s.fieldOfStudy << ","
                                                  << s.currentYear << ","
                                                  << s.gradesAvg << "\n";
                                         }
                                         file.close();
                                     }
                                     screen.ExitLoopClosure()(); });
        auto backButton = Button("Powrót", screen.ExitLoopClosure());

        auto container = Container::Vertical({saveButton, backButton});
        auto rend = Renderer(container, [&]
                             { return vbox({
                                          text("Zapisz do pliku") | bold | color(Color::Green),
                                          separator(),
                                          text("Czy chcesz zapisać?"),
                                          separator(),
                                          saveButton->Render() | center,
                                          backButton->Render() | center,
                                      }) |
                                      border; });

        screen.Loop(rend);
        return;
    }

    if (optionName == "Wyświetl wszystkich")
    {
        auto backButton = Button("Powrót", screen.ExitLoopClosure());
        auto container = Container::Vertical({backButton});

        auto rend = Renderer(container, [&]
                             {
                                 Elements lines;
                                 if (m_database.size() == 0)
                                 {
                                     lines.push_back(text("Brak studentów w bazie.") | color(Color::Yellow));
                                 }
                                 else
                                 {
                                     for (unsigned i = 0; i < m_database.size(); ++i)
                                     {
                                         const Student &s = m_database.at(i);
                                         lines.push_back(text("[" + std::to_string(i) + "] " + s.firstName + " " + s.lastName +
                                                              " | kierunek: " + s.department +
                                                              " | rok: " + std::to_string(s.currentYear) +
                                                              " | średnia: " + std::to_string(s.gradesAvg)) | color(Color::White));
                                     }
                                 }

                                 return vbox({
                                            text("Wszyscy studenci") | bold | color(Color::Cyan),
                                            separator(),
                                            vbox(lines) | flex,
                                            separator(),
                                            backButton->Render() | center,
                                        }) |
                                        border; });

        screen.Loop(rend);
        return;
    }

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