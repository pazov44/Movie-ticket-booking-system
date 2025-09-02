#include "pch.h"
#include "menu.h"
#include "options.h"

namespace Menu
{
    Menu::Menu(int max_len, std::string header, std::string head_color, std::string body_color,
        char up_key, char down_key, char sel_key) {
        if (max_len <= 0) {
            DbgMsg("Error: Max String Length is set to 0.");
            return;
        }

        max_string_len = max_len;

        if (head_color.size() != 2) {
            DbgMsg("Error: Head Color must contain only two characters in ANSI Color Code form.");
            return;
        }

        if (body_color.size() != 2) {
            DbgMsg("Error: Body Color must contain only two characters in ANSI Color Code form.");
            return;
        }

        colors.first = "\033[0;" + head_color + "m";
        colors.second = "\033[0;" + body_color + "m";
        neutral = "\033[0m";
        heading = header;
        keybinds[0] = up_key;
        keybinds[1] = down_key;
        keybinds[2] = sel_key;
        pos = names.begin();
        names = {};
        colors = {};
        is_run = false;
    }

    int Menu::get_max_len() {
        return max_string_len;
    }

    void Menu::set_max_len(int max_len) {
        if (max_len <= 0) {
            DbgMsg("Error: Max String Length is set to 0.");
            return;
        }

        max_string_len = max_len;
    }

    std::string Menu::get_heading() {
        return heading;
    }

    void Menu::set_heading(std::string header) {
        heading = header;
    }

    std::pair<std::string, std::string> Menu::get_colors() {
        return colors;
    }

    void Menu::set_colors(color c_head, color c_body) {

        colors.first = "\033[0;" + std::to_string((int)c_head) + "m";
        colors.second = "\033[0;" + std::to_string((int)c_body) + "m";
    }

    std::string Menu::get_keybinds() {
        return keybinds;
    }

    void Menu::set_up_key(char key) {
        if(isalnum(key))
        keybinds[0] = key;
    }

    void Menu::set_down_key(char key) {
        if (isalnum(key))
        keybinds[1] = key;
    }

    void Menu::set_sel_key(char key) {
        if (isalnum(key))
        keybinds[2] = key;
    }

    void Menu::add_field(std::string name) {
        names.push_back(name);
        pos = names.begin();
    }

    void Menu::remove_field(std::string name) {
        for (auto itr = names.begin(); itr != names.end(); itr++) {
            if (*itr == name) {
                names.erase(itr);
                pos = names.begin();
                return;
            }
        }

        DbgMsg("Error: Entry doesn\'t exist in the first place.");
    }

    void Menu::edit_field(std::string old_name, std::string new_name) {
        for (int itr = 0; itr != (int)names.size(); itr++) {
            if (names[itr] == old_name) {
                names[itr] = { new_name };
                pos = names.begin();
                return;
            }
        }

        DbgMsg("Error: Entry doesn\'t exist in the first place.");
    }

    bool Menu::has_field(std::string name) {
        for (auto itr = names.begin(); itr != names.end(); itr++) {
            if (*itr == name) return true;
        }

        return false;
    }

    void Menu::gen_element(std::string str, std::string color) {
        int space_size = (max_string_len - ((int)str.size())) / 2;

        for (int i = 1; i <= max_string_len; i++) {
            if (i <= space_size || i > space_size + (int)str.size()) std::cout << " ";
            else std::cout << color << str[i - space_size - 1] << neutral;
        }
    }

    void Menu::printMenu() {
        if (max_string_len <= 0) {
            DbgMsg("Error: Invalid Max String Length provided.");
            return;
        }

        std::cout << std::string(max_string_len + 2, '=') << std::endl << "|";
        gen_element(heading, colors.first);
        std::cout << "|" << std::endl << std::string(max_string_len + 2, '=') << std::endl;

        for (auto itr = names.begin(); itr != names.end(); itr++) {
            std::cout << ((itr != pos) ? "|" : "<");
            gen_element(*itr, ((itr == pos) ? colors.first : colors.second));
            std::cout << ((itr != pos) ? "|" : ">") << std::endl;
        }

        std::cout << std::string(max_string_len + 2, '=') << std::endl;
    }

    int Menu::run_menu() {
        char choice;
        bool run = true;
        pos = names.begin();

        if (!is_run)
        {
            add_field("Exit"); //add extra exit option
            is_run = true; // true if run_menu has been run atleast once
            set_colors(color::RED, color::CYAN);
        }
        while (run) {

            Utils::Clear();
            std::cout << "Press " << keybinds[0] << " and " << keybinds[1] << " to move up and down the menu." << std::endl;
            std::cout << "Press " << keybinds[2] << " to select an item." << std::endl;
            printMenu();
            choice = tolower(getch());

            if (choice == tolower(keybinds[0])) {
                if (pos != names.begin()) pos--;
            }

            else if (choice == tolower(keybinds[1])) {
                if (pos != prev(names.end())) pos++;
            }

            else if (choice == tolower(keybinds[2])) {
                Utils::Clear();
                if (pos == prev(names.end())) {
                    //run = false;
                    Utils::Exit();
                }
               return pos - names.begin();
            }
        }

        names.pop_back();
    }
    //////////////////////////////////////////////////////////////////////////////////

    Menu CreateMenu(const std::vector<std::string>& options,const std::string& heading)
    {
        Menu ret; // returned menu obj
        for (const auto& str : options)
        {
            ret.add_field(str);
        }
        if(!heading.empty())
        ret.set_heading(heading);

        return ret;
    }
    int AuthMenu()
    {
        static Menu authMenu = CreateMenu({ "LogIn","SignUp" }, "MovieSystem");

        // auth menu choice
        int ret = -1;
        for (;;)
        {

            int choice = authMenu.run_menu();
            switch (choice)
            {
            case 0: ret = Options::LogSign(LOG); break;
            case 1: ret = Options::LogSign(SIGN); break;
            case 2: Utils::Exit(); break;
            }
            if (ret == ENTER_ADMIN || ret == ENTER_CUSTOMER) break;
        }
        
        return ret;
    }
    void AdminMenu()
    {
        static Menu adminMenu = CreateMenu({ "Insert Movie" ,"Delete Movie","Insert Show","Delete Show","Update Show","Return to register" }, "Admin Panel");

        for(;;)
        {
            int choice = adminMenu.run_menu();
            switch (choice)
            {
            case 0: Options::InsertMovie(); break;
            case 1: Options::DeleteMovie(); break;
            case 2: Options::InsertShow(); break;
            case 3: Options::DeleteShow(); break;
            case 4: Options::UpdateShow();  break;
            case 5: return; break;
            }
        }
    }
    void CustomerMenu()
    {
        static Menu customerMenu = CreateMenu({ "Book a movie","Cancel a booking","Return to register" }, "Customer Menu");

        for (;;)
        {
            int choice = customerMenu.run_menu();
            switch (choice)
            {
            case 0: Options::BookMovie(); break;
            case 1: Options::CancelBooking(); break;
            case 2: return; break;
            }
        }
    }
}