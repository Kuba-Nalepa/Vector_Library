#include "TuiInterface.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include <ftxui/dom/elements.hpp>
#include <vector>
#include <string>

using namespace ftxui;

TuiInterface::TuiInterface() : screen(ScreenInteractive::Fullscreen())
{
}

TuiInterface::~TuiInterface()
{
}

void TuiInterface::render()
{
    struct Student
    {
        std::string firstName;
        std::string lastName;
        std::string department;
        std::string fieldOfStudy;
        int currentYear;
        double gradesAvg;
    };

    std::vector<Student> database = {
        {"Jan", "Kowalski", "WIP", "Cyberbezpieczeństwo", 2, 4.75},
        {"Anna", "Nowak", "WZ", "Logistyka", 1, 4.20},
        {"Michał", "Wiśniewski", "WI", "Informatyka", 3, 3.85},
        {"Maria", "Zielińska", "WIP", "Automatyka", 2, 4.90}};

    std::vector<std::string> menu_entries = {
        "Add student", "Delete student", "Edit student", "Print all students",
        "Count all students", "Load students from file", "Save students to file",
        "Show students from range", "Show student at index", "Search student by last name",
        "Search student by index number", "Show average students grade",
        "Show average grades for students (name)", "Exit"};

    int selected_menu = 0;
    int active_view_index = -1;
    bool show_exit_popup = false;

    auto btn_popup_yes = Button(" TAK ", screen.ExitLoopClosure(), ButtonOption::Animated());
    auto btn_popup_no = Button(" NIE ", [&]()
                               { show_exit_popup = false; }, ButtonOption::Animated());
    auto popup_buttons = Container::Horizontal({btn_popup_yes, btn_popup_no});

    Component main_interface_container = Container::Horizontal({});

    auto menu_options = MenuOption::Vertical();
    menu_options.entries_option.transform = [](EntryState state)
    {
        Element e = text(state.label);
        if (state.active && state.focused)
            return hbox({text("> "), e | bold});
        return hbox({state.active ? text("> ") : text("  "), e});
    };

    menu_options.on_enter = [&]()
    {
        if (menu_entries[selected_menu] == "Exit")
        {
            show_exit_popup = true;
        }
        else
        {
            active_view_index = selected_menu;
            if (main_interface_container->ChildCount() > 1)
            {
                main_interface_container->SetActiveChild(main_interface_container->ChildAt(1));
            }
        }
    };

    auto main_menu = Menu(&menu_entries, &selected_menu, menu_options);

    std::string new_student_name = "";
    Component input_student = Input(&new_student_name, "Enter name...");
    auto view_add_student = Container::Vertical({input_student,
                                                 Button("Save Student", [&]() {})});

    std::string delete_index = "";
    Component input_delete = Input(&delete_index, "Enter index to delete...");
    auto view_delete_student = Container::Vertical({input_delete,
                                                    Button("Delete", [&]() {})});

    auto view_placeholder = Renderer([]
                                     { return vbox({yflex(vbox())}); });

    auto right_panel_views = Container::Tab({view_add_student, view_delete_student, view_placeholder, view_placeholder,
                                             view_placeholder, view_placeholder, view_placeholder, view_placeholder,
                                             view_placeholder, view_placeholder, view_placeholder, view_placeholder,
                                             view_placeholder, view_placeholder},
                                            &active_view_index);

    auto right_panel_with_back_action = CatchEvent(right_panel_views, [&](Event event)
                                                   {
        if (event == Event::Escape || event == Event::ArrowLeft) {
            active_view_index = -1;
            new_student_name = "";
            delete_index = "";
            main_interface_container->SetActiveChild(main_interface_container->ChildAt(0));
            return true;
        }
        return false; });

    main_interface_container->Add(main_menu);
    main_interface_container->Add(right_panel_with_back_action);

    int active_layer = 0;
    auto master_container = Container::Tab({main_interface_container, popup_buttons}, &active_layer);

    auto renderer = Renderer(master_container, [&]
                             {
        if (show_exit_popup) active_layer = 1;
        else active_layer = 0;

        if (show_exit_popup) {
            return vbox({
                filler(),
                vbox({
                    filler(),
                    text("Czy na pewno chcesz wyjść?") | bold | center,
                    text(" "),
                    hbox({
                        btn_popup_yes->Render() | flex,
                        text(" "), 
                        btn_popup_no->Render() | flex
                    }) | size(WIDTH, LESS_THAN, 30) | center,
                    filler()
                }) 
                | bgcolor(Color::Black)
                | border 
                | size(WIDTH, EQUAL, 40) 
                | size(HEIGHT, EQUAL, 9) 
                | center,
                filler()
            }) | bgcolor(Color::Black);
        }

        Element right_panel_content;
        if (active_view_index == -1) {
            right_panel_content = vbox({ yflex(vbox()) }); 
        } 
        else if (active_view_index == 0) {
            right_panel_content = vbox({
                text("--- ADD NEW STUDENT ---") | bold | color(Color::Green),
                separator(),
                hbox(text(" Student Name: "), input_student->Render()) | border,
                yflex(vbox())
            });
        } 
        else if (active_view_index == 1) {
            right_panel_content = vbox({
                text("--- DELETE STUDENT ---") | bold | color(Color::Red),
                separator(),
                hbox(text(" Student Index: "), input_delete->Render()) | border,
                yflex(vbox())
            });
        } 
        else {
            right_panel_content = vbox({
                text("--- " + menu_entries[active_view_index] + " ---") | bold,
                separator(),
                yflex(vbox())
            });
        }

        Element top_columns = hbox({
            vbox({
                text(" SYSTEM MENU ") | bold | center,
                separator(),
                main_menu->Render()
            }) | border | size(WIDTH, EQUAL, 45),

            vbox({
                text(" OKNO INTERAKCYJNE ") | bold | center,
                separator(),
                right_panel_content | flex
            }) | border | flex
        }) | flex;

        Elements student_rows;
        student_rows.push_back(hbox({
            text(" Imię") | bold | size(WIDTH, EQUAL, 15) | color(Color::Cyan),
            text(" Nazwisko") | bold | size(WIDTH, EQUAL, 20) | color(Color::Cyan),
            text(" Wydział") | bold | size(WIDTH, EQUAL, 12) | color(Color::Cyan),
            text(" Kierunek") | bold | size(WIDTH, EQUAL, 25) | color(Color::Cyan),
            text(" Rok") | bold | size(WIDTH, EQUAL, 8) | color(Color::Cyan),
            text(" Średnia") | bold | color(Color::Cyan)
        }));
        student_rows.push_back(separator());

        if (database.empty()) {
            student_rows.push_back(text("Brak studentów w bazie danych.") | center | dim);
        } else {
            for (const auto& s : database) {
                student_rows.push_back(hbox({
                    text(" " + s.firstName) | size(WIDTH, EQUAL, 15),
                    text(" " + s.lastName) | size(WIDTH, EQUAL, 20),
                    text(" " + s.department) | size(WIDTH, EQUAL, 12),
                    text(" " + s.fieldOfStudy) | size(WIDTH, EQUAL, 25),
                    text(" " + std::to_string(s.currentYear)) | size(WIDTH, EQUAL, 8),
                    text(" " + std::to_string(s.gradesAvg).substr(0, 4)) | color(Color::Green)
                }));
            }
        }

        Element bottom_info_panel = vbox({
            hbox({
                text(" BAZA DANYCH STUDENTÓW ") | bold,
                text(" (Łącznie: " + std::to_string(database.size()) + ")") | color(Color::Yellow)
            }) | center,
            separator(),
            vbox(std::move(student_rows)) | vscroll_indicator | frame 
        }) | border | size(HEIGHT, EQUAL, 30); 

        return vbox({
            top_columns,
            bottom_info_panel
        }); });

    screen.Loop(renderer);
    screen.Clear();
}