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
        std::string specialization;
        std::string fieldOfStudy;
        std::string yearInput;
        std::string gradeInput;

        auto inputFirstName = Input(&firstName, "Imię");
        auto inputLastName = Input(&lastName, "Nazwisko");
        auto inputSpecialization = Input(&specialization, "Specjalność");
        auto inputFieldOfStudy = Input(&fieldOfStudy, "Kierunek");
        auto inputYear = Input(&yearInput, "Rok studiów");
        auto inputGrade = Input(&gradeInput, "Średnia ocen");
        auto saveButton = Button("Dodaj", [&]
                                 {
            Student student;
            student.firstName = firstName;
            student.lastName = lastName;
            student.specialization = specialization;
            student.fieldOfStudy = fieldOfStudy;
            student.currentYear = std::stoi(yearInput.empty() ? "0" : yearInput);
            student.gradesAvg = std::stod(gradeInput.empty() ? "0" : gradeInput);
            studentsDatabase.pushBack(student);
            screen.ExitLoopClosure()(); });
        auto backButton = Button("Powrót", screen.ExitLoopClosure());

        auto container = Container::Vertical({inputFirstName, inputLastName, inputSpecialization, inputFieldOfStudy, inputYear, inputGrade, saveButton, backButton});
        auto rend = Renderer(container, [&]
                             { return vbox({
                                          text("Dodawanie studenta") | bold | color(Color::Cyan),
                                          separator(),
                                          hbox(text("Imię:            ") | color(Color::White), inputFirstName->Render()),
                                          hbox(text("Nazwisko:        ") | color(Color::White), inputLastName->Render()),
                                          hbox(text("Specjalność:     ") | color(Color::White), inputSpecialization->Render()),
                                          hbox(text("Kierunek:        ") | color(Color::White), inputFieldOfStudy->Render()),
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
                                           if (index < studentsDatabase.size())
                                           {
                                               studentsDatabase.erase(index);
                                           }
                                       }
                                       catch (const std::exception &e)
                                       {
                                           throw std::invalid_argument("Nieprawidłowy indeks studenta.");
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
        if (studentsDatabase.size() == 0)
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
        std::string specialization;
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
                                         if (index < studentsDatabase.size())
                                         {
                                             const Student &student = studentsDatabase.at(index);
                                             firstName = student.firstName;
                                             lastName = student.lastName;
                                             specialization = student.specialization;
                                             fieldOfStudy = student.fieldOfStudy;
                                             yearInput = std::to_string(student.currentYear);
                                             gradeInput = std::to_string(student.gradesAvg);
                                             selectedStudent = static_cast<int>(index);
                                         }
                                     }
                                     catch (const std::exception &e)
                                     {
                                         throw std::invalid_argument("Nieprawidłowy indeks studenta.");
                                     } });

        auto inputFirstName = Input(&firstName, "Imię");
        auto inputLastName = Input(&lastName, "Nazwisko");
        auto inputSpecialization = Input(&specialization, "Kierunek");
        auto inputFieldOfStudy = Input(&fieldOfStudy, "Specjalność");
        auto inputYear = Input(&yearInput, "Rok studiów");
        auto inputGrade = Input(&gradeInput, "Średnia ocen");

        auto saveButton = Button("Zapisz zmiany", [&]
                                 {
                                     if (selectedStudent >= 0 && static_cast<unsigned>(selectedStudent) < studentsDatabase.size())
                                     {
                                         Student &student = studentsDatabase[selectedStudent];
                                         student.firstName = firstName;
                                         student.lastName = lastName;
                                         student.specialization = specialization;
                                         student.fieldOfStudy = fieldOfStudy;
                                         student.currentYear = std::stoi(yearInput.empty() ? "0" : yearInput);
                                         student.gradesAvg = std::stod(gradeInput.empty() ? "0" : gradeInput);
                                     }
                                     screen.ExitLoopClosure()(); });
        auto backButton = Button("Powrót", screen.ExitLoopClosure());

        auto container = Container::Vertical({inputIndex, loadButton, inputFirstName, inputLastName, inputSpecialization, inputFieldOfStudy, inputYear, inputGrade, saveButton, backButton});
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
                                          hbox(text("Specjalizacja:   ") | color(Color::White), inputSpecialization->Render()),
                                          hbox(text("Kierunek:     ") | color(Color::White), inputFieldOfStudy->Render()),
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
                                 const unsigned count = studentsDatabase.size();
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
        std::ifstream file("students.bin", std::ios::binary);

        if (file.is_open())
        {
            studentsDatabase.clear();

            while (file.peek() != EOF)
            {
                Student student;

                auto readString = [&](std::string& str)
                {
                    size_t len;
                    file.read(reinterpret_cast<char*>(&len), sizeof(len));

                    if (!file)
                        return;

                    str.resize(len);
                    file.read(&str[0], len);
                };

                readString(student.firstName);
                if (!file) break;

                readString(student.lastName);
                readString(student.specialization);
                readString(student.fieldOfStudy);

                file.read(reinterpret_cast<char*>(&student.currentYear),
                          sizeof(student.currentYear));

                file.read(reinterpret_cast<char*>(&student.gradesAvg),
                          sizeof(student.gradesAvg));

                if (file)
                    studentsDatabase.pushBack(student);
            }

            file.close();
        }

        screen.ExitLoopClosure()(); });

        auto backButton = Button("Powrót", screen.ExitLoopClosure());

        auto container = Container::Vertical({loadButton, backButton});

        auto rend = Renderer(container, [&]
                             { return vbox({text("Wczytaj z pliku") | bold,
                                            separator(),
                                            loadButton->Render(),
                                            backButton->Render()}) |
                                      border; });

        screen.Loop(rend);
        return;
    }

    if (optionName == "Zapisz do pliku")
    {
        auto saveButton = Button("Zapisz teraz", [&]
                                 {
        std::ofstream file(
            "students.bin",
            std::ios::binary | std::ios::trunc);

        if (file.is_open())
        {
            auto writeString =
                [&](const std::string& str)
            {
                size_t len = str.size();

                file.write(
                    reinterpret_cast<const char*>(&len),
                    sizeof(len));

                file.write(
                    str.c_str(),
                    len);
            };

            for (unsigned i = 0; i < studentsDatabase.size(); ++i)
            {
                const Student& student =
                    studentsDatabase.at(i);

                writeString(student.firstName);
                writeString(student.lastName);
                writeString(student.specialization);
                writeString(student.fieldOfStudy);

                file.write(
                    reinterpret_cast<const char*>(&student.currentYear),
                    sizeof(student.currentYear));

                file.write(
                    reinterpret_cast<const char*>(&student.gradesAvg),
                    sizeof(student.gradesAvg));
            }

            file.close();
        }

        screen.ExitLoopClosure()(); });

        auto backButton = Button("Powrót", screen.ExitLoopClosure());

        auto container =
            Container::Vertical(
                {saveButton, backButton});

        auto rend =
            Renderer(container, [&]
                     { return vbox({text("Zapisz do pliku BIN") | bold,
                                    separator(),
                                    saveButton->Render(),
                                    backButton->Render()}) |
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
                                 if (studentsDatabase.size() == 0)
                                 {
                                     lines.push_back(text("Brak studentów w bazie.") | color(Color::Yellow));
                                 }
                                 else
                                 {
                                     for (unsigned i = 0; i < studentsDatabase.size(); ++i)
                                     {
                                         const Student &student = studentsDatabase.at(i);
                                         lines.push_back(text("[" + std::to_string(i) + "] " + student.firstName + " " + student.lastName +
                                                              " | kierunek: " + student.specialization +
                                                              " | rok: " + std::to_string(student.currentYear) +
                                                              " | średnia: " + std::to_string(student.gradesAvg)) | color(Color::White));
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