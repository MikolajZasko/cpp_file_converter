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

// namespaces
namespace ch = std::chrono;
namespace fs = std::filesystem;

// constructors

/// @brief default constructor - menu mode
file_converter::file_converter() {

    // init the default out directory
    fs::path source_file_path(__FILE__);

    // get just the directory (without man.exe)
    source_file_path.remove_filename();

    source_file_path = source_file_path/ "out";

    out_directory_ = source_file_path.string();

    menu_ = true;

    // init the main menu
    main_menu_init_();
}

/// @brief constructor with ext name - default mode
file_converter::file_converter(std::string& ext) {
    // init the default out directory
    fs::path project_root(__FILE__);

    // get just the directory (without main.exe)
    project_root.remove_filename();

    out_directory_ = (project_root/ "out").string();

    in_directory_ = (project_root/ "in").string();

    // get the right extension (even if for example just "png" was provided)
    input_ = ext;

    if (! validate_extension_()) {
        // input makes no sense
        display_error_("Invalid extension: " + input_);

        exit(0);
    }

    extension_ = input_;

    menu_ = false;

    // read the in_directory_ - will attempt to convert all files inside (including nested folders)
    read_directory_contents_(in_directory_);

    // start the conversion
    conversion_init();
}

/// @brief constructor with ext name, input directory and files
file_converter::file_converter(std::vector<std::string>& v) {

    // init the default out directory
    fs::path source_file_path(__FILE__);

    // get just the directory (without main.exe)
    source_file_path.remove_filename();

    source_file_path = source_file_path/ "out";

    out_directory_ = source_file_path.string();

    // get the ext
    extension_ = v[1];

    // get the input dir
    in_directory_ = v[2];

    // remove the the script path, ext name and the input directory from the vector
    if (v.size() >= 3) {
        v.erase(v.begin(), v.begin() + 3);
    }

    // debugging stuff
    // std::cout << extension_ << std::endl;
    // std::cout << out_directory_ << std::endl;
    //
    // for (auto& e : v) {
    //     std::cout << e << std::endl;
    // }

    // check if any files provided
    if (v.size() == 0) {
        // read the in_directory_ - will attempt to convert all files inside (including nested folders)
        read_directory_contents_(in_directory_);

    }
    else {
        // insert vec values to set
        files_.insert(v.begin(), v.end());
    }

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
        if (! validate_directory_(in_directory_)) {
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
    if (menu_) {
        if (files_.empty()) {
            display_error_("no files provided");

            main_menu_init_();

            return;
        }
    }


    // check if the output directory exists, if not try to create it
    if (! validate_directory_(out_directory_)) {
        create_directory_(out_directory_);
    }

    // prep filesystem::path variables
    const fs::path in_directory_path(in_directory_);
    const fs::path out_directory_path(out_directory_);
    fs::path new_full_path {};

    // iterate through files
    for (const auto& old_file_string : files_) {

        // convert string -> filesystem::path
        fs::path old_file_path(old_file_string);

        fs::path old_relative_filepath{};

        // check if was relative laready
        if (! old_file_path.is_relative()) {
            // get the old relative filepath
            old_relative_filepath = fs::relative(old_file_path,in_directory_path);
        } else {
            old_relative_filepath = old_file_path;
        }

        // replace filename extension
        old_relative_filepath.replace_extension(extension_);

        // check if it is a relative path
        // if not it is a fullpath, no changes needed
        if (old_file_path.is_relative()) {

            // create the old path - from relative path
            old_file_path = in_directory_path / old_file_path;
        }

        // create the new path
        new_full_path = out_directory_path / old_relative_filepath;

        // check if we have the filestructure required, if not create the directories needed
        if (! validate_directory_(new_full_path.parent_path().string())) {
            create_directory_(new_full_path.parent_path().string());
        }

        // read the old file from disk
        cv::Mat image = cv::imread(old_file_path.string(), cv::IMREAD_COLOR);

        // check if image was loaded
        if (image.empty()) {
            display_error_("could not read the image: " + old_file_path.string());
            continue;
        }

        // check if file already exists
        if (validate_file_(new_full_path.string())) {

            // add a timestamp to the filename
            add_timestamp_(new_full_path);
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

/// @brief displays an info message
/// @param message an info message to be displayed
void file_converter::display_info_(const std::string &message) {

    // display the message
    std::cout << "ℹ️ Info: " << message << std::endl;
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
void file_converter::read_directory_() {

    // read input directly to #input_
    read_input_("directory, provide absolute path");

    // check if directory exists
    if (validate_directory_(input_)) {
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

/// @brief attempts to read the contents of the directory,
/// if successful places all found files to #files_
/// @param dir full path of the directory to be read
void file_converter::read_directory_contents_(const std::string &dir) {

    try {
        // loop hrough items in dir
        for (const auto& item : fs::directory_iterator(dir)) {

            // item.path() gives you the full path object
            const auto& current_path = item.path();

            // Extract just the file/folder name (e.g., "image.png")
            const std::string filename = current_path.filename().string();

            // Check the type of the item
            if (fs::is_directory(current_path)) {

                // explore the nested dir
                read_directory_contents_(current_path.string());

            } else if (fs::is_regular_file(current_path)) {

                // insert the file to files_
                files_.insert(current_path.string());

            } else {
                display_info_("item: " + current_path.string() + " is not a directory and is not a file - skipping");
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        display_info_("Filesystem error: " + std::string(e.what()));
    }
}

/// @brief tries to create a directory at a given path
/// @param dir a fullpath to the created directory
void file_converter::create_directory_(const std::string &dir) {
    try {
        if (fs::create_directories(dir)) {
            display_info_("created output directory: " + dir);
        } else {
            display_info_("no directories needed to be created");
        }
    }
    catch (const std::exception& e) {
        display_error_("creating directory" + dir + "failed! " + e.what());
    }
}

/// @returns the current timestamp
std::string file_converter::get_timestamp_() {

    // process timestamp to local timezone
    const auto now = ch::system_clock::now();

    const auto local_time = ch::current_zone()->to_local(now);

    auto seconds = ch::floor<ch::seconds>(local_time);

    return std::format("{:%Y%m%d_%H%M%S}", seconds);
}

/// @brief adds timestamp to #file - Ai powered
void file_converter::add_timestamp_(fs::path &original_file_path) {
    // 1. Extract the components
    const fs::path dir = original_file_path.parent_path();
    const std::string stem = original_file_path.stem().string();       // File name without extension
    const std::string ext = original_file_path.extension().string();    // File extension (includes the dot)

    // 2. Generate the timestamp
    const std::string timestamp = get_timestamp_();

    // 3. Construct the new filename (e.g., "report_20260610_185000.txt")
    const std::string newFileName = stem + "_" + timestamp + ext;

    // 4. Combine the original directory with the new filename
    original_file_path = dir / newFileName;
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
bool file_converter::validate_directory_(const std::string& dir) {

    // Checks if the path exists AND is a directory
    return fs::exists(dir) && fs::is_directory(dir);
}

/// @brief check if file exists
bool file_converter::validate_file_(const std::string& file) {

    // Checks if the path exists AND is a file
    return fs::exists(file) && fs::is_regular_file(file);
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
        // read the directory from the input
        read_directory_();

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
