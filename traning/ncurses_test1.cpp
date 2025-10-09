#include <iostream>
#include <ncurses.h>
// #include <unistd.h> // для usleep (більш точна затримка)
#include <unistd.h>



int main() {
    setlocale(LC_ALL, "");


    initscr();
    noecho();
    cbreak();
    curs_set(0);
    start_color();
    keypad(stdscr, true);

    use_default_colors();

    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_WHITE, -1);

    int choise = 1;
    int y,x;
    int getch_result;
    
    while (true)
    {
        

        clear();
        getmaxyx(stdscr, y, x);

        //V1
        // for(int one = 0;one <= y; one++)
        // {
        //     for(int two = 0; two <= x;two++)
        //     {
        //         clear();
        //         mvprintw(one, two, "Привет терминал");
        //         refresh();
        //         usleep(100000);
        //     }
        // }
        
        mvprintw(0,0,"Нажмите 'q' для того что бы закончить программу");
        
        attron(COLOR_PAIR(2));
        mvprintw((y/2)-2, x/2, "Привет терминал");
        
        // attron(COLOR_PAIR(1));
        // mvprintw((y/2)-1,(x/2)-12,"Вы попали в мою програму рад вас видеть");
        // attroff(COLOR_PAIR(1));

        


        switch (choise)
        {
            case 1:
            {
                attron(COLOR_PAIR(1));
                mvprintw((y/2)-1,(x/2)-12,"1) Создать файл");
                attroff(COLOR_PAIR(1));
                mvprintw((y/2)-0,(x/2)-12,"2) Удалить файл");
                mvprintw((y/2)+1,(x/2)-12,"3) Редактировать файл");

                break;
            }
            case 2:
            {
                mvprintw((y/2)-1,(x/2)-12,"1) Создать файл");
                attron(COLOR_PAIR(1));
                mvprintw((y/2)-0,(x/2)-12,"2) Удалить файл");
                attroff(COLOR_PAIR(1));
                mvprintw((y/2)+1,(x/2)-12,"3) Редактировать файл");
                break;
            }
            case 3:
            {
                mvprintw((y/2)-1,(x/2)-12,"1) Создать файл");
                mvprintw((y/2)-0,(x/2)-12,"2) Удалить файл");
                attron(COLOR_PAIR(1));
                mvprintw((y/2)+1,(x/2)-12,"3) Редактировать файл");
                attroff(COLOR_PAIR(1));
                break;
            }
        
        }

        refresh();
        // mvprintw((y/2)-1,(x/2)-12,"1) Создать файл");
        // mvprintw((y/2)-0,(x/2)-12,"2) Удалить файл");
        // mvprintw((y/2)+1,(x/2)-12,"3) Редактировать файл");

        attroff(COLOR_PAIR(2));


        getch_result = getch();
        
        bool is_enter_pressed = (getch_result == KEY_ENTER || getch_result == 10 || getch_result == 13);

        if (getch_result == 'q') {
            break;
        } else if (getch_result == KEY_DOWN) {
            if (choise < 3) {
                choise++;
            }
        } else if (getch_result == KEY_UP) {
            if (choise > 1) {
                choise--;
            }
        } 
        else if (is_enter_pressed) 
        {
            clear();
            if (choise == 1) {
                mvprintw(y / 2, x / 2, "Вы выбрали создание файла");
            } else if (choise == 2) {
                mvprintw(y / 2, x / 2, "Вы выбрали удалить файл");
            } else if (choise == 3) {
                mvprintw(y / 2, x / 2, "Вы выбрали редактировать файл");
            }
            refresh();
            getch(); 
        }

        


    
    }

    endwin();
    return 0;
}