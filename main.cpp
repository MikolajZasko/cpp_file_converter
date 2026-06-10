//
// Created by Mikołaj Zaśko on 30.05.2026.
//

#include "file_converter.h"
#include <string>
#include <vector>
#include <iostream>
#include <opencv2/core/utils/logger.hpp>

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
  Windows:
      file_converter.exe .png C:\input\dir\path Image1 Image2   # converts 2 images to png (both have to be in C:\input\dir\path )
      file_converter.exe .png C:\input\dir\path                 # reads the dir recursively and tries to convert all files to .png
      file_converter.exe -h                                     # displays help
      file_converter.exe                                        # runs the program with menus'

  Linux:
      file_converter .png /input/dir/path Image1 Image2     # converts 2 images to png (both have to be in /input/path)
      file_converter .png /input/dir/path                   # reads the dir recursively and tries to convert all files to .png
      file_converter -h                                     # displays help
      file_converter                                        # runs the program with menus'

Project structure:
    file-converter
    ├── 🙈 .gitignore                 # Git ignore rules
    ├── 📐 CMakeLists.txt             # CMake build configuration
    ├── 🧱 file_converter.cpp         # Core implementation logic
    ├── 📑 file_converter.h           # Header declarations
    ├── 🚀 main.cpp                   # Application entry point
    ├── 📖 README.md                  # This file
    ├── 📁 cmake-build-debug/         # CMake debug build directory (Windows)
    │   └── 🖥️ file_converter.exe     # Compiled Windows executable (Windows)
    └── 📁 out/                       # Default output directory
)" << std::endl;
            return 0;
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
