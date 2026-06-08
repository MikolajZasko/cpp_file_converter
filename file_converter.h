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
    file_converter() { main_menu_init_(); };

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
    
    // setters
    
    /// @brief preps the next menu #title_ and #options_
    void prep_menu(std::string new_title, std::vector<std::string> new_options);


private:

    // fields / variables

    /// @brief stores the current menu title
    std::string title_{};

    /// @brief stores the current menu options
    std::vector<std::string> options_{};

    /// @brief stores the current input
    std::string input_{};

    /// @brief stores the current selected valid output extension (with ".")
    std::string extension_{};

    // helper functions
    //
    // console helpers

    /// @brief clears the console
    void clear_();

    // input handling

    /// @brief waits for user to press "enter"
    static void wait_enter_();

    /// @brief waits for string input, places it in #input_
    /// @param message tells the user what we expect
    void get_input_(const std::string& message);

    /// @brief can modify #input_ (adds a "." if not present)
    /// @returns true if #input_ can be a valid extension
    bool validate_extension_();

    /// @brief displays an error message and waits for "enter" key
    /// @param message an error message to be displayed
    void display_error_(const std::string& message);

    /// @brief reads files from input, accepted separators: " " / "," / " , "
    void read_files_();

    // menu handling

    /// @brief initializes the file_converter / main entry point
    void main_menu_init_();

    /// @brief handles the conversion extension selection menu
    void extension_menu_init_();

    /// @brief collects data about files to convert
    void file_menu_init_();
};
