#include <iostream>
#include <vector>
#include <string>
#include <ncurses.h>
#include <locale.h>

using namespace std;

struct Song {
    string title;
    string author;
    int year;
};

enum ScreenState {
    MAIN_MENU,
    SECOND_MENU,
    SHOW_ALL_SONGS_SCREEN, 
    ADD_SONG_SCREEN,       
    EXIT_PROGRAM
};


int show_menu(const string& title, const vector<string>& items) {
    int highlight = 0;
    int key_pressed;

    while (true) {
        int y, x;
        getmaxyx(stdscr, y, x);
        clear();
        mvprintw(2, (x - title.length()) / 2, "%s", title.c_str());
        for (int i = 0; i < items.size(); ++i) {
            if (i == highlight) attron(A_REVERSE);
            mvprintw(4 + i, (x - items[i].length()) / 2, "%s", items[i].c_str());
            if (i == highlight) attroff(A_REVERSE);
        }
        refresh();

        key_pressed = getch();
        switch (key_pressed) {
            case KEY_UP:
                if (highlight > 0) highlight--;
                break;
            case KEY_DOWN:
                if (highlight < items.size() - 1) highlight++;
                break;
            case 10: case KEY_ENTER:
                return highlight;
            case 27: // Esc
                return -1;
        }
    }
}

std::string get_string_from_user(int y, int x, const std::string& prompt) {
    std::string result = "";
    int ch;

    mvprintw(y, x, "%s", prompt.c_str());

    curs_set(1);
    echo();
    
    move(y, x + prompt.length());
    refresh(); 

    while ((ch = getch()) != '\n' && ch != KEY_ENTER) {
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (!result.empty()) {
                result.pop_back();
                
                int current_y, current_x;
                getyx(stdscr, current_y, current_x); 
                mvaddch(current_y, current_x, ' ');   
                move(current_y, current_x);           
            }
        } else {
            result += ch;
        }
    }
    
    noecho();
    curs_set(0);

    return result;
}



void show_song_list_screen(const vector<Song>& song_catalog) {
    clear();
    int y, x;
    getmaxyx(stdscr, y, x);

    mvprintw(1, 2, "--- Список всіх пісень ---");

    if (song_catalog.empty()) {
        mvprintw(3, 2, "Каталог порожній.");
    } else {
        for (int i = 0; i < song_catalog.size(); ++i) {
            if (3 + i >= y - 1) { 
                mvprintw(y - 2, 2, "...");
                break;
            }
            mvprintw(3 + i, 2, "%d. %s - %s (%d)", i + 1,
                     song_catalog[i].author.c_str(),
                     song_catalog[i].title.c_str(),
                     song_catalog[i].year);
        }
    }

    mvprintw(y - 1, 2, "Натисніть будь-яку клавішу, щоб повернутися...");
    refresh();
    getch();
}

void add_song_screen(vector<Song>& song_catalog) {
    clear();
    
    Song new_song;
    
    new_song.title = get_string_from_user(3, 2, "Введіть назву: ");
    new_song.author = get_string_from_user(4, 2, "Введіть автора: ");
    string year_str = get_string_from_user(5, 2, "Введіть рік: ");
    new_song.year = stoi(year_str); 

    song_catalog.push_back(new_song);

    mvprintw(8, 2, "Пісню '%s' успішно додано! (натисніть Enter)", new_song.title.c_str());
    refresh();
    getch();
}



int main() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    curs_set(0);

    vector<Song> song_catalog = { 
        {"Stairway to Heaven", "Led Zeppelin", 1971},
        {"Bohemian Rhapsody", "Queen", 1975},
        {"Imagine", "John Lennon", 1971}
    };
    vector<string> main_menu_items = { "1. Показати всі пісні", "2. Додати нову пісню", "3. Вийти" };
    vector<string> second_menu_items = { "1. (Приклад) Показати список", "2. (Приклад) Додати пісню", "3. Назад" };

    ScreenState current_screen = MAIN_MENU;

    while (current_screen != EXIT_PROGRAM) {
        
        switch (current_screen) {
            case MAIN_MENU:
            {
                int choice = show_menu("ГОЛОВНЕ МЕНЮ", main_menu_items);
                if (choice == 0) {
                    current_screen = SHOW_ALL_SONGS_SCREEN;
                } else if (choice == 1) {
                    current_screen = ADD_SONG_SCREEN;
                } else if (choice == 2 || choice == -1) { 
                    current_screen = EXIT_PROGRAM;
                }
                break;
            }
            case SECOND_MENU: 
            {
                int choice = show_menu("ДРУГЕ МЕНЮ", second_menu_items);
                if (choice == 0) {
                    current_screen = SHOW_ALL_SONGS_SCREEN;
                } else if (choice == 1) {
                    current_screen = ADD_SONG_SCREEN;
                } else if (choice == 2 || choice == -1) {
                    current_screen = MAIN_MENU; 
                }
                break;
            }
            case SHOW_ALL_SONGS_SCREEN:
            {
                show_song_list_screen(song_catalog);
                current_screen = MAIN_MENU; 
                break;
            }
            case ADD_SONG_SCREEN:
            {
                add_song_screen(song_catalog);
                current_screen = MAIN_MENU; 
                break;
            }
        }
    }

    endwin();
    return 0;
}