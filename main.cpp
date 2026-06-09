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

int main(int argc, char* argv[]) {

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
            std::cout << R"(
File Converter CLI Tool - convert images using OpenCV.

Usage:
  file_converter [options] <extension> <files>

Options:
  -h, --help        Show this help message and exit.

Arguments:
  <extension>       Target output image format (e.g., .png, .jpg, .bmp).
  <files>           Full path to files separated by " "

Examples:
  file_converter .png C:\Users\Image1 , C:\Users\Image2 # converts 2 images to png
  file_converter -h                                     # displays help
  file_converter                                        # runs the program with menus
)" << std::endl;
            return 0;
        }
    }

    // check if any args provided
    if (argc > 1) {

        // first is the extension
        // all the rest are files
        file_converter f(args);

    }
    else {
        // start 1 instance of the file converter
        file_converter f;
    }

    return 0;
}
