#include <iostream>
#include <string>
#include <vector>
#include <ncurses.h>

#include "declarations.h"

using namespace std;


int show_menu(const string& title, const vector<string>& items) {
    int highlight = 0;
    int key_pressed;

    int max_len = 0;
    for (const string& item : items) 
    {
        if (item.length() > max_len) 
        {
            max_len = item.length();
        }
    }

    while (true) {
        int y, x;
        getmaxyx(stdscr, y, x);
        clear();
        
        mvprintw(2, (x - title.length()) / 2, "%s", title.c_str());
        
        int start_x = (x - max_len) / 2;

        for (int i = 0; i < items.size(); ++i) {
            if (i == highlight) attron(A_REVERSE);
            
            mvprintw(4 + i, start_x, "%s", items[i].c_str());
            
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







string get_string_from_user(int y, int x, const string& prompt) {
    string result = "";
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








void show_song_list_screen(const vector<Sounds>& song_catalog,ScreenState &screen_state ) {
    clear();
    int y, x;
    getmaxyx(stdscr, y, x);

    mvprintw(1, 2,"%s",lang[MENU_SHOW_ALL].c_str());

    if (song_catalog.empty()) {
        mvprintw(3, 2,"%s", lang[MSG_NO_SONGS_YET].c_str());
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

    mvprintw(y - 1, 2, "%s",lang[PROMPT_PRESS_ANY_KEY].c_str());
    refresh();
    getch();

    screen_state = MAIN_MENU;

}



vector<string> get_lyrics_from_user(int start_y, int start_x) {
    
    attron(A_BOLD);
    mvprintw(start_y, start_x,"%s", lang[PROMPT_ENTER_LYRICS_CTRL_D].c_str());
    attroff(A_BOLD);

    start_y++; 

    vector<string> all_lines;
    string current_line = "";
    int ch;
    
    curs_set(1);
    echo();

    int y = start_y;
    int x = start_x;
    move(y, x);
    refresh();

    while (true) {
        ch = getch();

        if (ch == 4) { //Нажат ли ctrl + D
            break; 
        }

        switch (ch) {
            case KEY_ENTER:
            case 10: // Enter
                all_lines.push_back(current_line); 
                current_line.clear();              
                y++;                               
                x = start_x;
                move(y, x);
                break;

            case KEY_BACKSPACE:
            case 127: // Backspace
                if (!current_line.empty()) {
                    current_line.pop_back();

                    int current_y, current_x;
                    getyx(stdscr, current_y, current_x);
                    mvaddch(current_y, current_x, ' ');
                    move(current_y, current_x);
                }
                break;
            
            default: 
                
                int term_y, term_x;
                getmaxyx(stdscr, term_y, term_x);
                if (x + current_line.length() < term_x - 1) {
                    current_line += ch;
                }
                break;
        }
        refresh();
    }
    
    if (!current_line.empty()) {
        all_lines.push_back(current_line);
    }
    
    noecho();
    curs_set(0);

    return all_lines;
}


void printLyricWithncurses(const Sounds& sound,const vector<string>& lyric)
{
    clear();

    int line_len = 0;
    for(string line: lyric)
    {    
        if(line_len <line.size())
        {
            line_len = line.size();
        }
    }

    int y,x;
    getmaxyx(stdscr,y,x);
    
    int start_x = (x/2)-(line_len/2);

    int y_current = 1;

    mvprintw(y_current++, start_x, (lang[SONG_TITLE_PREFIX] + "%s").c_str(), sound.title.c_str());
    mvprintw(y_current++, start_x, (lang[SONG_AUTHOR_PREFIX] + "%s").c_str(), sound.author.c_str());
    mvprintw(y_current++, start_x, (lang[SONG_YEAR_PREFIX] + "%d").c_str(), sound.year);
    y_current++;

    for(string line : lyric)
    {
        mvprintw(y_current++,start_x,"%s",line.c_str());
        if(y_current == y-2)
        {
            break;
        }
    }

    mvprintw(y, 0, "%s",lang[PROMPT_PRESS_ANY_KEY].c_str());
    refresh();
    getch();
}