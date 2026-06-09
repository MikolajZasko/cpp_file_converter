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
#include <algorithm>

// constructors

/// @brief default constructor
file_converter::file_converter() {

    // init the default out directory
    std::filesystem::path source_file_path(__FILE__);

    // get just the directory (without man.exe)
    source_file_path.remove_filename();

    source_file_path = source_file_path/ "out";

    out_directory_ = source_file_path.string();

    menu_ = true;

    // init the main menu
    main_menu_init_();
}

/// @brief constructor with ext name and files
file_converter::file_converter(std::vector<std::string>& v) {

    // init the default out directory
    std::filesystem::path source_file_path(__FILE__);

    // get just the directory (without man.exe)
    source_file_path.remove_filename();

    source_file_path = source_file_path/ "out";

    out_directory_ = source_file_path.string();

    // get the ext
    extension_ = v[1];

    // remove the ext from the vector and the script path
    //
    // Overwrite the target with the last element
    v[0] = v.back();

    // Remove the duplicate last element
    v.pop_back();

    // Overwrite the target with the last element
    v[1] = v.back();

    // Remove the duplicate last element
    v.pop_back();

    // insert vec values to set
    files_.insert(v.begin(), v.end());

    // debugging stuff
    // std::cout << extension_ << std::endl;
    // std::cout << out_directory_ << std::endl;
    //
    // for (auto& e : v) {
    //     std::cout << e << std::endl;
    // }

    // turn off the menus and popups
    menu_ = false;

    // start the conversion
    conversion_init();

    exit(0);
}


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
        std::cout << title_ << std::endl << std::endl << std::endl;

        // print each option with a corresponding int
        std::size_t index = 0;
        const std::size_t vec_size = options_.size();

        // first option is always "Go back"
        std::cout << "0) Go back" << std::endl << std::endl;

        while (index < vec_size) {

            // print index
            std::cout << index + 1 << ") ";

            // print option
            std::cout << options_[index] << std::endl << std::endl;

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

// conversion process

/// @brief starts the conversion process
void file_converter::conversion_init() {

    // verify the directory if user entered in_directory_
    if (! in_directory_.empty()) {
        if (! validate_directory(in_directory_)) {
            display_error_("the provided directory: '" + in_directory_ + "' is invalid/does not exist !!!");

            main_menu_init_();

            return;
        }
    }

    // check if user inserted an extension
    if (extension_.empty()) {
        display_error_("no extension provided");

        main_menu_init_();

        return;
    }

    // check if user provided any files
    if (files_.empty()) {
        display_error_("no files provided");

        main_menu_init_();

        return;
    }

    // prep filesystem::path variables
    const std::filesystem::path in_directory_path(in_directory_);
    const std::filesystem::path out_directory_path(out_directory_);
    std::filesystem::path new_full_path {};

    // iterate through files
    for (auto old_file_string : files_) {

        // convert string -> filesystem::path
        std::filesystem::path old_file_path(old_file_string);

        // get just the filename
        std::filesystem::path filename = old_file_path.filename();

        // replace filename extension
        filename.replace_extension(extension_);

        // check if directory was provided
        if (! in_directory_.empty()) {

            // here we assume that relative paths were provided
            //
            // create the old path
            old_file_path = in_directory_path / old_file_path;

            // create the new path
            new_full_path = out_directory_path / filename;

        }
        else {

            // here we assume that full paths were provided
            //
            // create the new path
            new_full_path = out_directory_path / filename;

        }

        // read the old file from disk
        cv::Mat image = cv::imread(old_file_path.string(), cv::IMREAD_COLOR);

        // check if image was loaded
        if (image.empty()) {
            display_error_("could not read the image: " + old_file_path.string());
            continue;
        }

        // try to write the converted file (openCV handles the conversion based on extension)
        const bool success = cv::imwrite(new_full_path.string(), image);

        if (!success) {
            display_error_("OpenCV failed to write the image to:" + new_full_path.string());
            continue;
        }
        else {
            display_success_("image: '" + new_full_path.string() + "' converted successfully");
        }
    }

    if (menu_) {
        wait_enter_();

        main_menu_init_();
    }
    else {
        exit(0);
    }
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

    // check if menus disabled
    if (menu_) {
        // display message
        std::cout << "Press Enter to continue...";

        // clears any leftover characters in the input buffer (like a previous newline) - ai
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // waits for the user to hit Enter - ai
        std::cin.get();
    }
}

/// @brief waits for single string input, places it in #input_
/// @param message tells the user what we expect
void file_converter::read_input_(const std::string& message) {

    // display the message
    std::cout << "Enter " << message << " :" << std::endl;

    // safe clear ???
    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }

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
    std::cout << "❌ Error: " << message << std::endl;

    // check if popups disabled
    if (menu_) {
        // wait for "enter" key
        wait_enter_();
    }
}

/// @brief displays a success message
/// @param message a success message to be displayed
void file_converter::display_success_(const std::string &message) {

        // display the message
        std::cout << "✅ Success: " << message << std::endl;
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
        files_.insert(trim_(temp_word));
    }
}

/// @brief reads the input directory from std input
void file_converter::read_directory() {

    // read input directly to #input_
    read_input_("directory, provide absolute path");

    // check if directory exists
    if (validate_directory(input_)) {
        in_directory_ = input_;
    }
    else {
        display_error_("Directory: '" + input_ + "' does not exist !!!");
    }
}

/// returns a string containing all files separated by \n
std::string file_converter::flatten_files_() const{

    // if no files selected, display "none"
    if (files_.empty()) {
        return "none";
    }

    // else, join all files
    std::string s = "\n";
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
    const std::string current_in_directory = (in_directory_.empty()) ? "none" : in_directory_;
    const std::string current_out_directory = (out_directory_.empty()) ? "none" : out_directory_;

    // some ascii artwork
    std::string file_ascii =
    " ______           ______  \n"
    "| |__| |         | |__| | \n"
    "|  ()  |   ==>   |  ()  | \n"
    "|______|         |______|   ";

    // prep the "Main menu"
    prep_menu("Main menu - file converter \n" + file_ascii, std::vector<std::string>{
        "Select output extension (ex: .png), current: " + current_extension,
        "Manage files \n current input directory: "
        + current_in_directory
        + "\n current output directory: "  + current_out_directory
        +  "\n current files: " + flatten_files_(),
        "Convert the files"
    });

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
    if (main_menu_result == 3) {
        conversion_init();
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
bool file_converter::validate_directory(const std::string& dir) const {

    // // convert to std::filesystem::path
    // const std::filesystem::path path(input_);

    // Checks if the path exists AND is a directory
    return std::filesystem::is_directory(dir);
}

/// @brief collects data about files to convert
void file_converter::file_menu_init_() {
    // prep the "Select output extension (ex: .png)"
    prep_menu("Provide files", std::vector<std::string>{
        "Add files",
        "Provide a directory (and relative file path's)",
        "Clear files_ vector",
        "Clear in_directory_ string"
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

    if (result == 4) {
        in_directory_.clear();

        file_menu_init_();

        return;
    }

}