//
// Created by Mikołaj Zaśko on 30.05.2026.
//

#pragma once

#include <vector>
#include <string>
#include <set>

class file_converter {
public:

    // constructors

    /// @brief default constructor
    file_converter();

    /// @brief constructor with ext name, input directory and files
    file_converter(std::vector<std::string>&);

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

    // conversion process

    /// @brief starts the conversion process
    void conversion_init();

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
    /// (can store both absolute paths and relative paths, dependent on #in_directory_)
    std::set<std::string> files_{};

    /// @brief stores the absolute path to the directory that the input files are in
    std::string in_directory_{};

    /// @brief stores the output directory
    std::string out_directory_{};

    /// @brief if false no menus are present
    bool menu_;

    // helper functions
    //
    // console helpers

    /// @brief clears the console
    void clear_();

    // input handling

    /// @brief waits for user to press "enter"
    void wait_enter_();

    /// @brief waits for single string input, places it in #input_
    /// @param message tells the user what we expect
    void read_input_(const std::string& message);

    /// @brief can modify #input_ (adds a "." if not present)
    /// @returns true if #input_ can be a valid extension
    bool validate_extension_();

    /// @brief checks if directory exists
    bool validate_directory_(const std::string&) const;

    /// @brief displays an error message and waits for "enter" key
    /// @param message an error message to be displayed
    void display_error_(const std::string& message);

    /// @brief displays a success message
    /// @param message a success message to be displayed
    void display_success_(const std::string& message);

    /// @brief displays an info message
    /// @param message an info message to be displayed
    void display_info_(const std::string& message);

    /// @brief reads files from input, and places found files to #files  \n
    /// accepted separators: ',' / ' , ' etc.
    void read_files_();

    /// @brief reads the input directory from std input
    void read_directory_();

    /// returns a string containing all files separated by \n
    [[nodiscard]] std::string flatten_files_() const;

    /// @brief removes redundant whitespaces / C++20 ranges split - ai
    static std::string trim_(const std::string&);

    /// @brief attempts to read the contents of the directory,
    /// if successful places all found files to #files_
    /// @param dir full path of the directory to be read
    void read_directory_contents_(const std::string& dir);

    /// @brief tries to create a directory at a given path
    /// @param dir a fullpath to the created directory
    void create_directory_(const std::string& dir);

    // menu handling

    /// @brief initializes the file_converter / main entry point
    void main_menu_init_();

    /// @brief handles the conversion extension selection menu
    void extension_menu_init_();

    /// @brief collects data about files to convert
    void file_menu_init_();
};