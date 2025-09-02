#pragma once

/**
* Menu.h (Effortless console menu making software)
* Copyright (C) 2023 by LeeTuah (Discord: leetuah).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma warning(disable : 4996)
# include <iostream>
# include <vector>
# include <functional>
# include <string>
# include <fstream>
# include <sstream>
# include <conio.h>
# include <utility>
#include "main.h"

namespace Menu
{
    /// @brief Colors for menu text output.
    enum color
    {
        DEFAULT = 0,
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37,
        BRIGHT_RED = 91,
        BRIGHT_GREEN = 92,
        BRIGHT_BLUE = 94
    };

    /// @brief Special menu entry codes.
    enum
    {
        ENTER_ADMIN = 80,
        ENTER_CUSTOMER = 81,
    };

    /// @brief Menu state identifiers.
    enum
    {
        empty,
        LOG,
        SIGN,
    };

    /// @brief Class representing a customizable terminal menu with color and keybind support.
    class Menu {
    private:
        /// @brief List of menu item names.
        std::vector<std::string> names;

        /// @brief Pair of strings representing head and body colors for menu.
        std::pair<std::string, std::string> colors;

        /// @brief ANSI escape sequence to reset text formatting.
        std::string neutral;

        /// @brief Keybinds stored as string: up key, down key, select key.
        std::string keybinds;

        /// @brief Heading/title of the menu.
        std::string heading;

        /// @brief Maximum number of characters menu items can extend to. Default is 32.
        int max_string_len;

        /// @brief Iterator pointing to currently selected menu item.
        std::vector<std::string>::iterator pos;

        /// @brief Prints the entire menu to the terminal.
        void printMenu();

        /// @brief Helper to print a string centered with a given color.
        /// @param str The string to print.
        /// @param color The ANSI color code as a string.
        void gen_element(std::string str, std::string color);

        /// @brief Flag indicating whether run_menu() has been called at least once.
        bool is_run;

    public:
        /// @brief Constructor initializing menu parameters.
        /// @param max_len Maximum string length for menu items.
        /// @param header Menu heading.
        /// @param head_color ANSI color code string for heading.
        /// @param body_color ANSI color code string for menu items.
        /// @param up_key Key to move up in the menu.
        /// @param down_key Key to move down in the menu.
        /// @param sel_key Key to select the current menu item.
        Menu(
            int max_len, std::string header, std::string head_color, std::string body_color,
            char up_key, char down_key, char sel_key
        );

        /// @brief Default constructor with default parameters.
        Menu() : Menu(32, "MENU", std::to_string(color::RED), std::to_string(color::CYAN), 'w', 's', 'e') {}

        /// @brief Returns the maximum length for menu items.
        /// @return Maximum string length.
        int get_max_len();

        /// @brief Sets the maximum length for menu items.
        /// @param max_len New maximum length.
        void set_max_len(int max_len);

        /// @brief Gets the menu heading.
        /// @return Current heading string.
        std::string get_heading();

        /// @brief Sets the menu heading.
        /// @param header New heading string.
        void set_heading(std::string header);

        /// @brief Gets the current head and body colors.
        /// @return Pair of color strings (head color, body color).
        std::pair<std::string, std::string> get_colors();

        /// @brief Sets new colors for the head and body text.
        /// @param c_head Head color enum.
        /// @param c_body Body color enum.
        void set_colors(color c_head, color c_body);

        /// @brief Gets the keybinds as a string.
        /// @return String containing up, down, and select keys.
        std::string get_keybinds();

        /// @brief Sets the key used for moving up in the menu.
        /// @param key Character key.
        void set_up_key(char key);

        /// @brief Sets the key used for moving down in the menu.
        /// @param key Character key.
        void set_down_key(char key);

        /// @brief Sets the key used for selecting a menu item.
        /// @param key Character key.
        void set_sel_key(char key);

        /// @brief Adds a new menu item.
        /// @param name Name of the menu item to add.
        void add_field(std::string name);

        /// @brief Removes a menu item by name.
        /// @param name Name of the menu item to remove.
        void remove_field(std::string name);

        /// @brief Edits an existing menu item name.
        /// @param old_name Current name of the menu item.
        /// @param new_name New name to replace with.
        void edit_field(std::string old_name, std::string new_name);

        /// @brief Checks if a menu item exists by name.
        /// @param name Name to check.
        /// @return True if item exists, false otherwise.
        bool has_field(std::string name);

        /// @brief Runs the menu event loop and handles user interaction.
        /// @return The zero-based index of the selected menu item.
        int run_menu();
    };

    /// @brief Factory function to create a Menu object from options and an optional heading.
    /// @param options List of menu item names.
    /// @param heading Optional heading string.
    /// @return A configured Menu object.
    Menu CreateMenu(const std::vector<std::string>& options, const std::string& heading = "");

    /// @brief Displays the authentication menu.
    /// @return Menu selection code.
    int AuthMenu();

    /// @brief Displays the customer menu.
    void CustomerMenu();

    /// @brief Displays the admin menu.
    void AdminMenu();
}