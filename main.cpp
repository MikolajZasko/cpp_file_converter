//
// Created by Mikołaj Zaśko on 30.05.2026.
//

#include "file_converter.h"
#include <string>
#include <vector>
#include <iostream>
#include <opencv2/core/utils/logger.hpp>
#include "messages.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[]) {

    // Set log level to WARNING or ERROR to suppress [INFO] logs
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    #ifdef _WIN32
        // Set console output code page to UTF-8 - so I can display emojis :D
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // iterate through args - create a vector
    std::vector<std::string> args(argv, argv + argc);

    // if args present save the extension
    std::string extension{};

    for (auto& arg : args) {
        if (arg == "-h" || arg == "--help") {
            // display help message and exit
            std::cout << Messages::HELP_STRING << std::endl;
            return 0;
        }

        // run with default settings (default in / out dir, only ext needed)
        if (arg == "-m" || arg == "--menu") {
            // implement this !!! ??? - update readme (gfx dir / in / out dirs - defaults)
        }
    }

    // check if any args provided
    if (argc > 1) {

        // first is the extension
        // second is the directory
        // all the rest are files
        file_converter f(args);

    }
    else {
        // start 1 instance of the file converter
        file_converter f;
    }

    return 0;
}
