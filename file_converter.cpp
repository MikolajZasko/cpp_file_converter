//
// Created by Mikołaj Zaśko on 30.05.2026.
//

#include "file_converter.h"
#include <iostream>
#include <limits>
#include <vector>
#include <opencv2/opencv.hpp>

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

// helper functions

/// @brief clears the console
void file_converter::clear_() {
    // "\033[" is the CSI (Control Sequence Introducer)
    // "2J" clears the entire screen
    // "H" moves the cursor to the home position (1,1)
    std::cout << "\x1b[2J\x1b[H";
}

/// @brief preps the next menu @ref title_ and @ref options_
void file_converter::prep_menu_(std::string new_title, std::vector<std::string> new_options) {

    // move the new_title to title_ and new_options to options_
    title_ = std::move(new_title);
    options_ = std::move(new_options);

}

/// @brief initializes the file_converter / main entry point
void file_converter::init_() {

    // prep the Main menu
    prep_menu_("Main menu", std::vector<std::string>{"Select output extension (ex: .png)"});

    // start the Main menu
    const int main_menu_result = menu();

    // 
    if (main_menu_result == 0) {
        exit(0);
    }
    if (main_menu_result == 1) {
        extension_init_();
    }
}

/// @brief handles the conversion type selection menu
void file_converter::extension_init_() {


    // prep the "Select output extension (ex: .png)"
    prep_menu_("Select output extension (ex: .png)", std::vector<std::string>{"See the extensions supported", "Provide the string of the extension"});

    //
    const int result = menu();

    if (result == 0) {
        // go back to Main menu
        init_();
        return;
    }
    if (result == 1) {
        std::cout << cv::getBuildInformation() << std::endl;

        // wait for any input
        wait_enter_();

        init_();
        return;
    }
    if (result == 2) {
        // get input
        get_input_("extension (no '.' needed, example input: png)");

        // validate if input makes sense

    }
}

/// @brief waits for user to press "enter"
void file_converter::wait_enter_() {

    // display message
    std::cout << "Press Enter to continue...";

    // clears any leftover characters in the input buffer (like a previous newline) - ai
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // waits for the user to hit Enter - ai
    std::cin.get();
}

/// @brief waits for string input, places it in #input_
void file_converter::get_input_(const std::string& message) {

    // display the message
    std::cout << "Enter " << message << " :" << std::endl;

    // wait for "enter" key
    std::cin.get();

    // write directly do #input_
    std::getline(std::cin, input_);
}
