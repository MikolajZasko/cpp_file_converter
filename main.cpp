//
// Created by Mikołaj Zaśko on 30.05.2026.
//

#include "file_converter.h"
#include <string>
#include <vector>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {

    #ifdef _WIN32
        // Set console output code page to UTF-8 - so I can display emojis :D
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // start 1 instance of the file converter
    file_converter f;

    // // testing
    // const std::string title = "Test title";
    // const std::vector<std::string> vec = {"this is opt 1", "this is opt 2"};
    //
    // const int result = f.menu(title, vec);
    //
    // std::cout << "The result of the menu is: " << result << std::endl;

    return 0;
}
