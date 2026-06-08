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

    /// @brief stores the file paths to be converted
    /// (can store both absolute paths and relative paths, dependent on #directory_)
    std::vector<std::string> files_{};

    /// @brief stores the absolute path to the directory that the input files are in
    std::string directory_{};


    // helper functions
    //
    // console helpers

    /// @brief clears the console
    void clear_();

    // input handling

    /// @brief waits for user to press "enter"
    static void wait_enter_();

    /// @brief waits for single string input, places it in #input_
    /// @param message tells the user what we expect
    void read_input_(const std::string& message);

    /// @brief can modify #input_ (adds a "." if not present)
    /// @returns true if #input_ can be a valid extension
    bool validate_extension_();

    /// @brief checks if directory exists
    bool validate_directory() const;

    /// @brief displays an error message and waits for "enter" key
    /// @param message an error message to be displayed
    void display_error_(const std::string& message);

    /// @brief reads files from input, and places found files to #files  \n
    /// accepted separators: ',' / ' , ' etc.
    void read_files_();

    /// @brief
    void read_directory();

    /// returns a string containing all files separated by \n
    std::string flatten_files_() const;

    /// @brief removes redundant whitespaces / C++20 ranges split - ai
    static std::string trim_(const std::string&);

    // menu handling

    /// @brief initializes the file_converter / main entry point
    void main_menu_init_();

    /// @brief handles the conversion extension selection menu
    void extension_menu_init_();

    /// @brief collects data about files to convert
    void file_menu_init_();
};
