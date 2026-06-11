cmake_minimum_required(VERSION 3.10.0)
project(tuiBase VERSION 0.1.0 LANGUAGES  CXX)

include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

add_executable(tuiBase 
    src/app/main.cpp
    tuiInterface/TuiInterface.cpp
)
target_include_directories(tuiBase PRIVATE 
    src 
    tuiInterface
)
target_link_libraries(tuiBase PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)

