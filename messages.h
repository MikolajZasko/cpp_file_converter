//
// Created by Mikołaj Zaśko on 11.06.2026.
//

#pragma once
#ifndef FILE_CONVERTER_MESSAGES_H
#define FILE_CONVERTER_MESSAGES_H
#include <string_view>

namespace Messages {
    inline constexpr std::string_view HELP_STRING =
        R"(
File Converter Tool - convert images using OpenCV.

Usage:
  file_converter [options] <extension> <input_directory> <files>

Options:
  -h, --help        Show this help message and exit.
  -m, --menu        Launch the program in the menu mode (provide info through std::cin - input)

Arguments:
  <extension>         Target output image format (e.g., .png, .jpg, .bmp).

  <input_directory>   If provided, 1) with no args reads recursively and converts every file
                                   2) with files followed, converts only files provided

  <files>             Full path to files separated by " "

Examples:
  Windows (.exe file) / Linux (no .exe) :
      # converts 2 images to png (both have to be in /input/path)
      .\file_converter .png /input/dir/path Image1 Image2

      # reads the dir recursively and tries to convert all files to .png
      .\file_converter .png /input/dir/path

      # reads the dir recursively and tries to convert all files to .png
      # using the default in/out dirs located in <project-root>/in <project-root>/out
      .\file_converter .png

      # displays help (this message)
      .\file_converter -h

      # runs the app in menu mode
      .\file_converter -m

Project structure:
    file-converter
    ├── 🙈 .gitignore                 # Git ignore rules
    ├── 📐 CMakeLists.txt             # CMake build configuration
    ├── 🧱 file_converter.cpp         # Core implementation logic
    ├── 📑 file_converter.h           # Header declarations
    ├── 🚀 main.cpp                   # Application entry point
    ├── 📖 README.md                  # This file
    ├── 📁 cmake-build-debug/         # CMake debug build directory
    │   └── 🖥️ file_converter.exe     # Compiled Windows executable
    │   └── 🐧 file_converter         # Compiled Linux executable
    ├── 📁 out/                       # Default output directory
    ├── 📁 in/                        # Default input directory
    └── 📁 gfx/                       # Holds graphics (for README.md for example)
)";
}

#endif //FILE_CONVERTER_MESSAGES_H
