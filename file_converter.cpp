//
// Created by Mikołaj Zaśko on 30.05.2026.
//

#include "file_converter.h"
#include <iostream>
#include <limits>
#include <vector>
#include <opencv2/opencv.hpp>
#include <ranges>
#include <filesystem>

/**
 * @brief creates menu, based on the options and title
 * first option is always "Go back" - index 0
 *
 * @param title the title of the menu
 * @param options the options of the menu
 *
 * @return returns 0 when "Go back" was selected
 * otherwise an index of an option -1 example:
 * * returns 1 => option on index 0 in vector options
 */
int file_converter::menu() {

    // make a loop so that we print the menu until we get the right index
    while (true) {
        // clear the console
        clear_();

        // print title
        std::cout << title_ << std::endl << std::endl;

        // print each option with a corresponding int
        std::size_t index = 0;
        const std::size_t vec_size = options_.size();

        // first option is always "Go back"
        std::cout << "0) Go back" << std::endl;

        while (index < vec_size) {

            // print index
            std::cout << index + 1 << ") ";

            // print option
            std::cout << options_[index] << std::endl;

            // increment index
            index++;
        }

        // print an empty line
        std::cout << std::endl;

        // print a prompt message
        std::cout << "Enter a valid index: ";

        // wait for input
        int input;

        // check if input is an int
        if (!(std::cin >> input)) {

            // Clear the error flag
            std::cin.clear();

            // Discard the invalid input from the buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // if we do nothing here, we print the menu once again

        } else {

            // we have a number check if valid index
            if (input >= 0 && input <= vec_size) {
                return input;
            }

            // if not we print the menu once again
        }
    }
}

// setters

/// @brief preps the next menu @ref title_ and @ref options_
void file_converter::prep_menu(std::string new_title, std::vector<std::string> new_options) {

    // move the new_title to title_ and new_options to options_
    title_ = std::move(new_title);
    options_ = std::move(new_options);

}

// console helpers

/// @brief clears the console
void file_converter::clear_() {
    // "\033[" is the CSI (Control Sequence Introducer)
    // "2J" clears the entire screen
    // "H" moves the cursor to the home position (1,1)
    std::cout << "\x1b[2J\x1b[H";
}

// input handling

/// @brief waits for user to press "enter"
void file_converter::wait_enter_() {

    // display message
    std::cout << "Press Enter to continue...";

    // clears any leftover characters in the input buffer (like a previous newline) - ai
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // waits for the user to hit Enter - ai
    std::cin.get();
}

/// @brief waits for single string input, places it in #input_
/// @param message tells the user what we expect
void file_converter::read_input_(const std::string& message) {

    // display the message
    std::cout << "Enter " << message << " :" << std::endl;

    // wait for "enter" key
    std::cin.get();

    // write directly to #input_
    std::getline(std::cin, input_);
}

/// @brief can modify #input_ (adds a "." if not present)
/// @returns true if #input_ can be a valid extension
bool file_converter::validate_extension_() {
    // An extension shouldn't be empty
    if (input_.empty()) return false;

    // Allow for an optional leading dot (e.g., ".txt" vs "txt")
    if (input_[0] != '.') {
        // add the "." to the input_
        input_ = '.' + input_;
    }

    // If it was just a dot it's invalid
    if (input_.length() == 1) {
        return false;
    }

    // Check that all remaining characters are alphanumeric (letters or numbers)
    for (size_t i = 1; i < input_.length(); ++i) {
        if (!std::isalnum(static_cast<unsigned char>(input_[i]))) {
            return false;
        }
    }

    return true;
}

/// @brief displays an error message and waits for "enter" key
/// @param message an error message to be displayed
void file_converter::display_error_(const std::string &message) {

    // display the message
    std::cout << "Error: " << message << std::endl;

    // wait for "enter" key
    wait_enter_();
}

/// @brief reads files from input, and places found files to #files  \n
/// accepted separators: ',' / ' , ' etc.
void file_converter::read_files_() {

    // read input directly to #input_
    read_input_("files, accepted separators: ',' / ' , ' etc.");

    // Split input by "," and trim each token
    for (const auto word : std::views::split(input_, ',')) {
        // Convert subrange to a temporary string to pass to trim_
        std::string temp_word(word.begin(), word.end());

        // Trim it and push it to the vector
        files_.emplace_back(trim_(temp_word));
    }
}

// !!!??? sth is wrong here? + docstring???? !!! - gemini fix splitting strings in cpp
void file_converter::read_directory() {

    // read input directly to #input_
    read_input_("directory, provide absolute path");

    // check if directory exists
    if (validate_directory()) {
        directory_ = input_;
    }
    else {
        display_error_("Directory: '" + input_ + "' does not exist !!!");
    }
}

/// returns a string containing all files separated by \n
std::string file_converter::flatten_files_() const{
    std::string s;
    for (const auto& file : files_) {
        s += "-> " + file + "\n";
    }
    return s;
}

/// @brief removes redundant whitespaces / C++20 ranges split - ai
std::string file_converter::trim_(const std::string& r) {
    // 1. Drop leading spaces
    auto leading = r | std::views::drop_while([](unsigned char ch) { return std::isspace(ch); });

    // 2. Reverse, drop trailing spaces, and reverse back
    auto trailing = leading | std::views::reverse
                            | std::views::drop_while([](unsigned char ch) { return std::isspace(ch); })
                            | std::views::reverse;

    // 3. Construct and return a new std::string from the view
    return std::string(trailing.begin(), trailing.end());
}

// menu handling

/// @brief initializes the file_converter / main entry point
void file_converter::main_menu_init_() {

    // check if we have already an extension
    const std::string current_extension = (extension_.empty()) ? "none" : extension_;

    // prep the "Main menu"
    prep_menu("Main menu", std::vector<std::string>{
        "Select output extension (ex: .png), current: " + current_extension,
        "Manage files \n current directory: " + directory_ + "\n current files: \n" + flatten_files_() });

    // start the "Main menu"
    const int main_menu_result = menu();

    if (main_menu_result == 0) {
        exit(0);
    }
    if (main_menu_result == 1) {
        extension_menu_init_();
        return;
    }
    if (main_menu_result == 2) {
        file_menu_init_();
        return;
    }
}

/// @brief handles the conversion extension selection menu
void file_converter::extension_menu_init_() {

    // prep the "Select output extension (ex: .png)"
    prep_menu("Select output extension (ex: .png)", std::vector<std::string>{
        "See the extensions supported",
        "Provide the string of the output extension"
    });

    // start the "Select output extension (ex: .png)"
    const int result = menu();

    if (result == 0) {
        // go back to Main menu
        main_menu_init_();
        return;
    }
    if (result == 1) {
        std::cout << cv::getBuildInformation() << std::endl;

        // wait for any input
        wait_enter_();

        main_menu_init_();
        return;
    }
    if (result == 2) {
        // get input
        read_input_("extension (formats: png/.png accepted)");

        // validate if input makes sense
        if (validate_extension_()) {

            // input could be an extension, put it in extension_
            extension_ = input_;

            // go back to main menu, we have a potentially working extension
            main_menu_init_();
            return;
        }

        // input makes no sense
        display_error_("Invalid extension: " + input_);

        // retry the menu
        extension_menu_init_();

        return;
    }
}

/// @brief checks if directory exists
bool file_converter::validate_directory() const {

    // // convert to std::filesystem::path
    // const std::filesystem::path path(input_);

    // Checks if the path exists AND is a directory
    return std::filesystem::is_directory(input_);
}

/// @brief collects data about files to convert
void file_converter::file_menu_init_() {
    // prep the "Select output extension (ex: .png)"
    prep_menu("Provide files", std::vector<std::string>{
        "Add files with absolute path",
        "Provide a directory + relative file path's",
        "Clear files_ vector"
    });

    // start the "Select output extension (ex: .png)"
    const int result = menu();

    if (result == 0) {
        // go back to Main menu
        main_menu_init_();
        return;
    }
    if (result == 1) {
        // read files from input to files_
        read_files_();

        // go back to Main menu
        main_menu_init_();
        return;
    }
    if (result == 2) {
        // read files from input to files_
        read_files_();

        // read the directory
        read_directory();

        // go back to Main menu
        main_menu_init_();
        return;
    }

    if (result == 3) {

        // clear vector and redo the menu
        files_.clear();

        file_menu_init_();

        return;
    }

}
