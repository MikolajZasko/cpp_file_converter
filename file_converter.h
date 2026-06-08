//
// Created by Mikołaj Zaśko on 30.05.2026.
//

#pragma once

#include <vector>
#include <string>

class file_converter {
public:

    // constructors

    /// @brief default constructor
    file_converter() { init_(); };

    // getters

    /**
     * @brief creates menu, based on the #options_ and #title_
     * first option is always "Go back" - index 0
     *
     * @return returns 0 when "Go back" was selected
     * otherwise an index of an option -1 example:
     * * returns 1 => option on index 0 in vector options
     */
    int menu();



private:

    // fields / variables

    /// @brief stores the current menu title
    std::string title_{};

    /// @brief stores the current menu options
    std::vector<std::string> options_{};

    /// @brief stores the current input
    std::string input_{};

    // helper functions

    /// @brief clears the console
    void clear_();

    /// @brief waits for user to press "enter"
    static void wait_enter_();

    /// @brief waits for string input, places it in #input_
    /// @arg message tells the user what we expect
    void get_input_(const std::string&);

    /// @brief preps the next menu #title_ and #options_
    void prep_menu_(std::string new_title, std::vector<std::string> new_options);

    /// @brief initializes the file_converter / main entry point
    void init_();

    /// @brief handles the conversion extenstion selection menu
    void extension_init_();
};
