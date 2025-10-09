#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <ncurses.h>


// ====== ЦВЕТА ДЛЯ КОНСОЛИ ==========
#define RESET   "\033[0m"
#define HEADER  "\033[93m" // Ярко-желтый
#define LABEL   "\033[34m" // Синий
#define VALUE   "\033[32m" // Зеленый
#define HIGHLIGHT "\033[91m" // Ярко-красный
#define PROMPT  "\033[36m" // Голубой (Cyan)
#define ERROR   "\033[31m" // Красный



using namespace std;


/*
Структура для зберігання основної інформації про пісню.
*/
struct Sounds
{
    string title;
    string author;
    int year;
    string source_filename;

};


/**
 *  Перелік (enum) для керування станами екранів в ncurses-інтерфейсі.
 * 
 * Дозволяє відстежувати, який екран зараз активний, і перемикатися між ними.
 */
enum ScreenState {
    MAIN_MENU,
    CREATESOUND, 
    PRINTSOUND,
    DELATESONG,
    EDITSONG,
    FINDBYAUTHOR,
    FINDBYWORD,
    TEXTONDISPLAY,
    SAVETOFILE,
    CHANGE_LANGUAGE,      
    EXIT_PROGRAM
};


/**
 * Перелік (enum) для індексів рядків у мовному пакеті.
 * 
 * Надає імена константам для кожного рядка в векторі 'lang',
 * щоб зробити код більш читабельним і уникнути "магічних чисел".
 */
enum Language_pack_IDs {
    // Головний цикл програми
    PROMPT_INTERFACE_TYPE,
    TITLE_MAIN_MENU,
    PROMPT_MAIN_MENU,
    PROMPT_YOUR_CHOICE,

    // Пункти головного меню
    MENU_ADD_SONG,
    MENU_SHOW_ALL,
    MENU_DELETE_SONG,
    MENU_EDIT_SONG,
    MENU_FIND_AUTHOR,
    MENU_FIND_WORD,
    MENU_DISPLAY_FULL,
    MENU_SAVE_TO_FILE,
    MENU_CHANGE_LANGUAGE,
    MENU_EXIT,

    // Функція loadSongs
    MSG_DB_NOT_FOUND,
    PROMPT_ENTER_FULL_PATH,
    MSG_FOLDER_CREATED,
    MSG_NO_SONGS_YET,

    // Функція createSound
    PROMPT_ENTER_TITLE,
    PROMPT_ENTER_AUTHOR,
    PROMPT_ENTER_YEAR,
    TITLE_ADD_TEXT_MENU,
    MENU_ADD_TEXT_MANUALLY,
    MENU_ADD_TEXT_FROM_FILE,
    PROMPT_ADD_TEXT_MANUAL_STOP,
    PROMPT_ADD_TEXT_FILE_PATH,
    ERROR_FILE_OPEN,

    // Загальні запити до користувача
    PROMPT_CHOOSE_SONG_TO_DELETE,
    PROMPT_CHOOSE_SONG_TO_EDIT,
    PROMPT_CHOOSE_SONG_TO_DISPLAY,
    PROMPT_CHOOSE_SONG_TO_SAVE,
    ERROR_INVALID_SONG_NUMBER,

    // Функція editSong
    PROMPT_EDIT_CHOICE,
    MENU_EDIT_TITLE,
    MENU_EDIT_AUTHOR,
    MENU_EDIT_YEAR,
    MENU_EDIT_LYRICS,

    // Функція findSongsByAuthor
    PROMPT_FIND_BY_AUTHOR,
    MSG_AUTHOR_SONGS_ARE,
    MSG_AUTHOR_SONGS_NOT_FOUND,

    // Функція findSongsByWord
    PROMPT_FIND_BY_WORD,
    MSG_SEARCH_RESULTS_FOR_WORD,
    MSG_WORD_FOUND_IN,
    MSG_AUTHOR_SUFFIX,
    MSG_WORD_NOT_FOUND,

    // Функція displayFullSong
    TITLE_DISPLAY_MODE_MENU,
    MENU_DISPLAY_TERMINAL,
    MENU_DISPLAY_EDITOR,
    HEADER_LYRICS,
    FOOTER_LYRICS,

    // Заголовки та форматування виводу
    HEADER_ALL_SONGS,
    SONG_NUMBER_PREFIX,
    SONG_TITLE_PREFIX,
    SONG_AUTHOR_PREFIX,
    SONG_YEAR_PREFIX,
    MSG_YEAR_SUFFIX,
    FOOTER_ALL_SONGS,
    
    // Додаткові рядки для ncurses
    PROMPT_PRESS_ANY_KEY,
    PROMPT_ENTER_LYRICS_CTRL_D,
    
    STRING_COUNT 
};

// Глобальний вектор, що зберігає всі текстові рядки для поточного мовного пакету інтерфейсу.
extern vector<string> lang;

//===================NCURSES ФУНКЦИИ=========================

/**
 * @brief Відображає інтерактивне меню в режимі ncurses.
 * @param title Заголовок меню.
 * @param items Вектор з пунктами меню.
 * @return Індекс обраного пункту або -1, якщо натиснуто Esc.
 */
int show_menu(const string& title, const vector<string>& items);

/**
 * @brief Отримує рядок тексту від користувача в режимі ncurses.
 * @param y Початкова координата Y.
 * @param x Початкова координата X.
 * @param prompt Повідомлення-запрошення до вводу.
 * @return Введений користувачем рядок.
 */
string get_string_from_user(int y, int x, const string& prompt);

/**
 * @brief Відображає екран зі списком всіх пісень в режимі ncurses.
 * @param song_catalog Каталог пісень для відображення.
 * @param screen_state Посилання на поточний стан екрану для повернення в головне меню.
 */
void show_song_list_screen(const vector<Sounds>& song_catalog,ScreenState &screen_state );

/**
 * @brief Дозволяє користувачеві вводити багаторядковий текст в режимі ncurses.
 * @param start_y Початкова координата Y.
 * @param start_x Початкова координата X.
 * @return Вектор рядків, що містить введений текст.
 */
vector<string> get_lyrics_from_user(int start_y, int start_x);

/**
 * @brief Виводить повну інформацію та текст пісні на екран в режимі ncurses.
 * @param sound Об'єкт пісні для відображення.
 * @param lyric Вектор з рядками тексту пісні.
 */
void printLyricWithncurses(const Sounds& sound,const vector<string>& lyric);




//=========================ОСНОВНЫЕ ФУНКЦИИ===========================

/**
 * @brief Зберігає інформацію про пісню та її текст у файл.
 * @param song Об'єкт пісні з метаданими.
 * @param lyrics Вектор з рядками тексту пісні.
 * @param choice Тип інтерфейсу (1 - звичайний, 2 - ncurses).
 */
void saveSongToFile(const Sounds& song, const vector<string>& lyrics,const int choice);

/**
 * @brief Парсить файли пісень з вказаної папки і заповнює каталог.
 * @param db_dir_path Шлях до папки з файлами пісень.
 * @param song_catalog Посилання на вектор для заповнення даними про пісні.
 */
void parseSongFile(string &db_dir_path,vector<Sounds> &song_catalog);

/**
 * @brief Завантажує пісні з папки. Якщо папки немає, пропонує створити або вказати шлях.
 * @param db_dir_path Посилання на рядок зі шляхом до папки.
 * @param song_catalog Посилання на вектор для заповнення даними.
 */
void loadSongs(string &db_dir_path,vector<Sounds> &song_catalog);

/**
 * @brief Проста функція для форматування рядка (замінює перше входження %s).
 * @param format Рядок формату.
 * @param value Значення для вставки.
 * @return Відформатований рядок.
 */
string format_string(const string& format, const string& value);

/**
 * @brief Замінює пробіли в рядку на нижні підкреслення.
 * @param start_name Вхідний рядок.
 * @return Рядок без пробілів.
 */
string zamenaSpasNa_(string &start_name);

/**
 * @brief Створює новий запис про пісню, отримуючи дані від користувача.
 * @param db_dir_path Шлях до папки з піснями.
 * @param song_catalog Посилання на каталог пісень.
 * @param choice Тип інтерфейсу.
 * @param screen_state_ptr Вказівник на стан екрану (для ncurses).
 */
void createSound(string &db_dir_path,vector<Sounds> &song_catalog,const int choice, ScreenState* screen_state_ptr = nullptr);

/**
 * @brief Відображає список всіх пісень в каталозі.
 * @param song_catalog Посилання на каталог пісень.
 */
void displayAllSongs(vector<Sounds> &song_catalog);

/**
 * @brief Видаляє пісню з каталогу та її файл.
 * @param song_catalog Посилання на каталог пісень.
 * @param choice Тип інтерфейсу.
 * @param screen_state Вказівник на стан екрану (для ncurses).
 */
void deleteSong(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state = nullptr);

/**
 * @brief Оновлює дані пісні у файлі та перейменовує файл, якщо потрібно.
 * @param song_to_edit Посилання на об'єкт пісні, що редагується.
 * @param soung_text Новий текст пісні.
 * @param choice Тип інтерфейсу.
 */
void updateAndRenameSong(Sounds &song_to_edit,const vector<string> &soung_text,const int choice);

/**
 * @brief Зчитує текст пісні з файлу.
 * @param path_to_file Шлях до файлу.
 * @return Вектор з рядками тексту пісні.
 */
vector<string> readTextFromFile(string &path_to_file);

/**
 * @brief Записує текст пісні у вказаний файл.
 * @param path_to_file Шлях до файлу для запису.
 * @param soung_text Вектор з рядками тексту.
 */
void writeTextToFile(string &path_to_file,const vector<string> &soung_text);

/**
 * @brief Редагує інформацію про існуючу пісню.
 * @param song_catalog Посилання на каталог пісень.
 * @param choice Тип інтерфейсу.
 * @param screen_state Вказівник на стан екрану (для ncurses).
 */
void editSong(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state = nullptr);

/**
 * @brief Знаходить та виводить всі пісні вказаного автора.
 * @param song_catalog Посилання на каталог пісень.
 * @param choice Тип інтерфейсу.
 * @param screen_state Вказівник на стан екрану (для ncurses).
 */
void findSongsByAuthor(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state = nullptr);

/**
 * @brief Знаходить та виводить пісні, в тексті яких є вказане слово.
 * @param song_catalog Посилання на каталог пісень.
 * @param choice Тип інтерфейсу.
 * @param screen_state Вказівник на стан екрану (для ncurses).
 */
void findSongsByWord(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state = nullptr);

/**
 * @brief Виводить повний текст обраної пісні в термінал або відкриває у редакторі.
 * @param song_catalog Посилання на каталог пісень.
 * @param choice Тип інтерфейсу.
 * @param screen_state Вказівник на стан екрану (для ncurses).
 */
void displayFullSong(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state = nullptr);

/**
 * @brief Зберігає текст обраної пісні в новий файл за вказаним шляхом.
 * @param song_catalog Посилання на каталог пісень.
 * @param choice Тип інтерфейсу.
 * @param screen_state Вказівник на стан екрану (для ncurses).
 */
void saveTextSongToFile(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state = nullptr);

// ФУНКЦІЇ ДЛЯ ЗМІНИ МОВИ

/**
 * @brief Завантажує мовний пакет з файлу.
 * @param lang_code Код мови (наприклад, "en", "ua").
 * @return true, якщо завантаження успішне, інакше false.
 */
bool loadLanguagePack(const string& lang_code);

/**
 * @brief Дозволяє користувачеві змінити мову інтерфейсу.
 * @param choice Тип інтерфейсу.
 * @param screen_state_ptr Вказівник на стан екрану (для ncurses).
 */
void changeLanguage(const int choice, ScreenState* screen_state_ptr = nullptr);
