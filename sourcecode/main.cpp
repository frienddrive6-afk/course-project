#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> 
#include <vector>
#include <filesystem> 
#include <ncurses.h>

#include "declarations.h"

using namespace std;


vector<string> lang;

/**
 * @brief Завантажує стандартний (український) мовний пакет при запуску програми.
 * 
 * Ця функція ініціалізує вектор 'lang' початковими значеннями.
 */
void loadDefaultLanguage() {
    lang = {
        // PROMPT_INTERFACE_TYPE
        "Оберіть тип інтерфейсу\n1) Стандартний (старий)\n2) Через ncurses (Новий)\nВаш вибір: ",
        // TITLE_MAIN_MENU
        "Головне меню програми \"Каталог текстів пісень\"",
        // PROMPT_MAIN_MENU
        "Доброї доби, користувачу.\nВас вітає програма \"Каталог текстів пісень\".\nОберіть дію:",
        // PROMPT_YOUR_CHOICE
        "Ваш вибір: ",
        // MENU_ADD_SONG
        "1) Додати нову пісню",
        // MENU_SHOW_ALL
        "2) Вивести всі існуючі пісні",
        // MENU_DELETE_SONG
        "3) Видалити пісню",
        // MENU_EDIT_SONG
        "4) Редагувати інформацію про пісню",
        // MENU_FIND_AUTHOR
        "5) Знайти всі пісні автора",
        // MENU_FIND_WORD
        "6) Знайти пісні за словом у тексті",
        // MENU_DISPLAY_FULL
        "7) Вивести повний текст пісні",
        // MENU_SAVE_TO_FILE
        "8) Зберегти текст пісні у файл",
        // MENU_CHANGE_LANGUAGE
        "9) Змінити мову",
        // MENU_EXIT
        "10) Вихід",
        // MSG_DB_NOT_FOUND
        "За шляхом за замовчуванням папки з піснями немає. У вас два варіанти:\n1) Вказати шлях до папки\n2) Створити нову папку\nВаш вибір: ",
        // PROMPT_ENTER_FULL_PATH
        "Вкажіть новий ПОВНИЙ шлях до папки з музикою: ",
        // MSG_FOLDER_CREATED
        "Було створено папку для зберігання пісень за таким шляхом -> ",
        // MSG_NO_SONGS_YET
        "На жаль, поки що немає жодної пісні",
        // PROMPT_ENTER_TITLE
        "Введіть назву пісні: ",
        // PROMPT_ENTER_AUTHOR
        "Введіть ім'я автора: ",
        // PROMPT_ENTER_YEAR
        "Введіть рік випуску: ",
        // TITLE_ADD_TEXT_MENU
        "Оберіть спосіб додавання тексту:",
        // MENU_ADD_TEXT_MANUALLY
        "Додавання вручну",
        // MENU_ADD_TEXT_FROM_FILE
        "Додавання з файлу",
        // PROMPT_ADD_TEXT_MANUAL_STOP
        "Введіть * для того щоб завершити",
        // PROMPT_ADD_TEXT_FILE_PATH
        "Введіть повний шлях до файлу з текстом: ",
        // ERROR_FILE_OPEN
        "ПОМИЛКА при відкритті файлу!",
        // PROMPT_CHOOSE_SONG_TO_DELETE
        "Введіть номер пісні, яку хочете видалити (починаючи з 1):",
        // PROMPT_CHOOSE_SONG_TO_EDIT
        "Введіть номер пісні для зміни (починаючи з 1):",
        // PROMPT_CHOOSE_SONG_TO_DISPLAY
        "Виберіть номер пісні, яку хочете вивести (починаючи з 1):",
        // PROMPT_CHOOSE_SONG_TO_SAVE
        "Виберіть пісню, яку хочете зберегти: ",
        // ERROR_INVALID_SONG_NUMBER
        "Помилка: невірний номер пісні",
        // PROMPT_EDIT_CHOICE
        "Введіть параметр, який хочете змінити",
        // MENU_EDIT_TITLE
        "Назва пісні",
        // MENU_EDIT_AUTHOR
        "Ім'я автора",
        // MENU_EDIT_YEAR
        "Рік випуску",
        // MENU_EDIT_LYRICS
        "Текст пісні (Доведеться писати весь заново)",
        // PROMPT_FIND_BY_AUTHOR
        "Введіть ім'я автора, за яким будемо шукати: ",
        // MSG_AUTHOR_SONGS_ARE
        "Цьому автору належать такі пісні як:",
        // MSG_AUTHOR_SONGS_NOT_FOUND
        "Пісень, пов'язаних з цим автором, не знайдено :(",
        // PROMPT_FIND_BY_WORD
        "Введіть слово, яке ви хочете знайти в пісні: ",
        // MSG_SEARCH_RESULTS_FOR_WORD
        "Результати пошуку за словом '%s':",
        // MSG_WORD_FOUND_IN
        "У пісні під назвою '%s' автора '%s'",
        // MSG_AUTHOR_SUFFIX
        " автора ",
        // MSG_WORD_NOT_FOUND
        "Пісень, що містять це слово, не знайдено.",
        // TITLE_DISPLAY_MODE_MENU
        "Оберіть спосіб відображення пісні: ",
        // MENU_DISPLAY_TERMINAL
        "Вивести в термінал",
        // MENU_DISPLAY_EDITOR
        "Відкрити в текстовому редакторі",
        // HEADER_LYRICS
        "----------Текст пісні----------",
        // FOOTER_LYRICS
        "----------Кінець пісні----------",
        // HEADER_ALL_SONGS
        "----------Початок виводу-----------",
        // SONG_NUMBER_PREFIX
        "Пісня номер -> ",
        // SONG_TITLE_PREFIX
        "Назва пісні: ",
        // SONG_AUTHOR_PREFIX
        "Автор: ",
        // SONG_YEAR_PREFIX
        "Рік випуску: ",
        // MSG_YEAR_SUFFIX
        " року випуску",
        // FOOTER_ALL_SONGS
        "---------Кінець----------",
        // PROMPT_PRESS_ANY_KEY
        "Натисніть будь-яку клавішу для повернення...",
        // PROMPT_ENTER_LYRICS_CTRL_D
        "Введіть текст. Натисніть Ctrl+D для завершення."
    };
}



/**
 * @brief Головний цикл взаємодії з користувачем.
 * 
 * Функція визначає, який інтерфейс (звичайний чи ncurses) обрав користувач,
 * та запускає відповідний цикл обробки команд меню.
 * @param song_catalog Посилання на вектор, що містить каталог пісень.
 * @param db_dir_path Посилання на рядок зі шляхом до папки з піснями.
 */
void workWithUser(vector<Sounds> &song_catalog,string &db_dir_path)
{

    cout<<lang[PROMPT_INTERFACE_TYPE];
    int choice;
    cin>>choice;
    cin.ignore();

    switch (choice)
    {
        case 1:
        {

            while (true)
            {
                system("clear");

                cout << HEADER << "===========================================================" << endl;
                cout << "        " << lang[TITLE_MAIN_MENU] << endl;
                cout << "===========================================================" << RESET << endl << endl;

                // cout<<"Доброго времени суток пользователь.\nВас приведствует программа \"Каталог текстов и песен\".\nВот вы можете сделать:\n1)Добавить новую песню.\n2)Вывести все существующие песни на экран.\n3)Удалить песню.\n4)Редактировать информацию о песне и текст песни.\n5)Найти все песни автора.\n6)Найти песни в тексте которых есть указаное вами слово.\n7)Вывести полностью текст песни.\n8)Сохранить текст песни в txt файл в указаный вами путь.\nДля того чтобы закончить введите любую другую цыфру.\nВаш выбор: ";
                cout << PROMPT << lang[PROMPT_MAIN_MENU] << RESET << endl;
                cout << LABEL << lang[MENU_ADD_SONG] << endl;
                cout << LABEL << lang[MENU_SHOW_ALL] << endl;
                cout << LABEL << lang[MENU_DELETE_SONG] << endl;
                cout << LABEL << lang[MENU_EDIT_SONG] << endl;
                cout << LABEL << lang[MENU_FIND_AUTHOR] << endl;
                cout << LABEL << lang[MENU_FIND_WORD] << endl;
                cout << LABEL << lang[MENU_DISPLAY_FULL] << endl;
                cout << LABEL << lang[MENU_SAVE_TO_FILE] << endl;
                cout << LABEL << lang[MENU_CHANGE_LANGUAGE] << endl;
                cout << HIGHLIGHT << lang[MENU_EXIT] << RESET << endl << endl; 
                cout << PROMPT << lang[PROMPT_YOUR_CHOICE] << RESET;


                int choise;
                cin>>choise;

                cin.ignore();

                switch (choise)
                {
                    case 1:
                    {
                        createSound(db_dir_path,song_catalog,choice);
                        break;
                    }

                    case 2:
                    {
                        displayAllSongs(song_catalog);
                        break;
                    }

                    case 3:
                    {
                        deleteSong(song_catalog,choice);
                        break;
                    }

                    case 4:
                    {
                        editSong(song_catalog,choice);
                        break;
                    }

                    case 5:
                    {
                        findSongsByAuthor(song_catalog,choice);
                        break;
                    }

                    case 6:
                    {
                        findSongsByWord(song_catalog,choice);
                        break;
                    }

                    case 7:
                    {
                        displayFullSong(song_catalog,choice);
                        break;
                    }

                    case 8:
                    {
                        saveTextSongToFile(song_catalog,choice);
                        break;
                    }

                    case 9:
                    {
                        changeLanguage(choice);
                        break;
                    }

                
                    default:
                        return;
                        break;
                }


                cout << endl << PROMPT << lang[PROMPT_PRESS_ANY_KEY] << RESET;
                cin.get();
            }

            break;
        }

        case 2:
        {
            setlocale(LC_ALL, "");
            initscr();
            noecho();
            cbreak();
            keypad(stdscr, true);
            curs_set(0);

            // vector<string> main_menu_items = { "1)Добавить новую песню","2)Вывести все существующие песни на экран","3)Удалить песню","4)Редактировать информацию о песне и текст песни","5)Найти все песни автора","6)Найти песни в тексте которых есть указаное вами слово","7)Вывести полностью текст песни","8)Сохранить текст песни в txt файл в указаный вами путь","Закончить"};



            ScreenState current_screen = MAIN_MENU;

            while (current_screen != EXIT_PROGRAM) {

                vector<string> main_menu_items = {
                    lang[MENU_ADD_SONG],
                    lang[MENU_SHOW_ALL],
                    lang[MENU_DELETE_SONG],
                    lang[MENU_EDIT_SONG],
                    lang[MENU_FIND_AUTHOR],
                    lang[MENU_FIND_WORD],
                    lang[MENU_DISPLAY_FULL],
                    lang[MENU_SAVE_TO_FILE],
                    lang[MENU_CHANGE_LANGUAGE], 
                    lang[MENU_EXIT] 
                };


                switch (current_screen)
                {
                    case MAIN_MENU:
                    {
                        int show_menu_res = show_menu(lang[TITLE_MAIN_MENU], main_menu_items);
                        if(show_menu_res == 0)
                        {
                            current_screen = CREATESOUND;

                        }else if(show_menu_res == 1)
                        {

                            current_screen = PRINTSOUND;

                        }else if(show_menu_res == 2)
                        {
                            current_screen = DELATESONG;

                        }else if(show_menu_res == 3)
                        {
                            current_screen = EDITSONG;

                        }else if(show_menu_res == 4)
                        {
                            current_screen = FINDBYAUTHOR;

                        }else if(show_menu_res == 5)
                        {
                            current_screen = FINDBYWORD;
                        }else if(show_menu_res == 6)
                        {
                            current_screen = TEXTONDISPLAY;

                        }else if(show_menu_res == 7)
                        {
                            current_screen = SAVETOFILE;

                        }else if (show_menu_res == 8) 
                        {
                            current_screen = CHANGE_LANGUAGE; 
                        }else if (show_menu_res == 9 || show_menu_res == -1) 
                        {
                            current_screen = EXIT_PROGRAM;
                        }


                        break;
                    }

                    case CREATESOUND:
                    {

                        createSound(db_dir_path,song_catalog,choice,&current_screen);
                        break;
                    }

                    case PRINTSOUND:
                    {
                        show_song_list_screen(song_catalog,current_screen);

                        break;
                    }

                    case DELATESONG:
                    {

                        deleteSong(song_catalog,choice,&current_screen);
                        break;
                    }

                    case EDITSONG:
                    {

                        editSong(song_catalog,choice,&current_screen);
                        break;
                    }

                    case FINDBYAUTHOR:
                    {
                        findSongsByAuthor(song_catalog,choice,&current_screen);
                        break;
                    }

                    case FINDBYWORD:
                    {
                        findSongsByWord(song_catalog,choice,&current_screen);
                        break;
                    }

                    case TEXTONDISPLAY:
                    {
                        displayFullSong(song_catalog,choice,&current_screen);
                        break;
                    }

                    case SAVETOFILE:
                    {
                        saveTextSongToFile(song_catalog,choice,&current_screen);
                        break;
                    }

                    case CHANGE_LANGUAGE: 
                    {
                        changeLanguage(choice, &current_screen); 
                        break;
                    }


                }





            }






            endwin();
            break;
        }
        
        default:
        {
            cout<<""<<endl;
            break;
        }

    }
    
}



/**
 * @brief Головна функція програми (точка входу).
 * 
 * Ініціалізує мову, завантажує каталог пісень та запускає
 * головний цикл взаємодії з користувачем.
 * @return 0 у разі успішного завершення.
 */
int main()
{
    setlocale(LC_ALL, "");
    loadDefaultLanguage();

    string db_dir_path = "./sound";// Шлях до папки з піснями за замовчуванням

    vector<Sounds> song_catalog;// Основний контейнер для зберігання всіх завантажених пісень


    loadSongs(db_dir_path,song_catalog);


    workWithUser(song_catalog,db_dir_path);


    return 0;
}