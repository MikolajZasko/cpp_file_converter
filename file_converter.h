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
     * @brief creates menu, based on the @ref options_ and @ref title_
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

    // helper functions

    /// @brief clears the console
    void clear_();

    /// @brief preps the next menu @ref title_ and @ref options_
    void prep_menu_(std::string new_title, std::vector<std::string> new_options);

    /// @brief initializes the file_converter / main entry point
    void init_();

    /// @brief handles the conversion type selection menu
    void conversion_init_();
};
