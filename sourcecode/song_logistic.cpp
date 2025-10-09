#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> 
#include <vector>
#include <filesystem> 
#include <ncurses.h>

#include "declarations.h"

using namespace std;

void saveSongToFile(const Sounds& song, const vector<string>& lyrics,const int choice)
{

    ofstream fileW(song.source_filename);
    if (!fileW.is_open()) {
        if(choice == 1)
        {
            cout << ERROR << lang[ERROR_FILE_OPEN] << RESET << endl;
        }
        return;
    }


    fileW<<"Название песни:"<<song.title<<";"<<endl;
    fileW<<"Имя автора:"<<song.author<<";"<<endl;
    fileW<<"Год выпуска:"<<song.year<<";"<<endl;

    for(int i = 0;i < 3;i++)
    {
        fileW<<endl;
    }
    fileW<<"-----------------------------"<<endl;

    for(string line : lyrics)
    {
        fileW<<line<<endl;
    }

    fileW<<"-----------------------------"<<endl;

}



void parseSongFile(string &db_dir_path,vector<Sounds> &song_catalog)
{
    if(!filesystem::is_empty(db_dir_path))
    {
        for (const auto& entry : filesystem::directory_iterator(db_dir_path))
        {
            string thisPath;
            Sounds new_song;
            thisPath = entry.path();

            ifstream fileR(thisPath);
            if(!fileR.is_open())
            {
                cout << lang[ERROR_FILE_OPEN] << endl;
            }

            int line_index = 0;
            string line;

            string title,autor;
            int year = 0;

            while (getline(fileR,line))
            {
                if(line_index == 0)
                {
                    if(line.find("Название песни:") != string::npos)
                    {

                        int start_pos = line.find(':') + 1; 
                        int end_pos = line.rfind(';');

                        if (start_pos != string::npos && end_pos != string::npos) 
                        {
                            int length = end_pos - start_pos; 
                            title = line.substr(start_pos, length);
                        }

                        // cout<<title<<endl;
                        new_song.title = title;
                    }
                    line_index++;

                }else if(line_index == 1)
                {
                    if(line.find("Имя автора:") != string::npos)
                    {

                        int start_pos = line.find(':') + 1; 
                        int end_pos = line.find(';');

                        if (start_pos != string::npos && end_pos != string::npos) 
                        {
                            int length = end_pos - start_pos; 
                            autor = line.substr(start_pos, length);
                        }

                        // cout<<autor<<endl;
                        new_song.author = autor;
                    }
                    line_index++;

                }else if(line_index == 2)
                {
                    if(line.find("Год выпуска:") != string::npos)
                    {

                        int start_pos = line.find(':') + 1; 
                        int end_pos = line.find(';');

                        if (start_pos != string::npos && end_pos != string::npos) 
                        {
                            int length = end_pos - start_pos; 
                            year = stoi(line.substr(start_pos, length));
                        }

                        // cout<<year<<endl;
                        new_song.year = year;
                    }
                    line_index++;


                }else if(line_index >= 3)
                {
                    break;
                }


            }

            if(!title.empty() && !autor.empty() && year != 0)
            {
                // cout<<"Ошибок чтения файла НЕТ"<<endl;
                // cout<<endl;

                new_song.title = title;
                new_song.author = autor;
                new_song.year = year;
                new_song.source_filename = thisPath;

                song_catalog.push_back(new_song);
            }
            // else
            // {
            //     cout<<"Ошибка чтения данные не сохраняются"<<endl;
            // }
            

            


            fileR.close();
        }
    }else
    {
        cout << lang[MSG_NO_SONGS_YET] << endl;
    }
}

void loadSongs(string &db_dir_path,vector<Sounds> &song_catalog)
{
    if(!filesystem::exists(db_dir_path))
    {
        cout << endl << HEADER << lang[MSG_DB_NOT_FOUND] << RESET;
        int vubor;
        cin>>vubor;
        if(vubor == 1)
        {
            cout << PROMPT << lang[PROMPT_ENTER_FULL_PATH] << RESET;
            cin>>db_dir_path;
        }else if(vubor == 2)
        {
            filesystem::create_directory(db_dir_path);
            cout << VALUE << lang[MSG_FOLDER_CREATED] << db_dir_path << RESET << endl;
        }
    }

    parseSongFile(db_dir_path,song_catalog);
    
}

string format_string(const string& format, const string& value) {
    string result = format;
    long pos = result.find("%s");
    if (pos != string::npos) {
        result.replace(pos, 2, value); 
    }
    return result;
}


string zamenaSpasNa_(string &start_name)
{
    string end_name;
    for(auto b: start_name)
    {
        if(b != ' ')
        {
            end_name.push_back(b);
        }else
        {
            end_name.push_back('_');
        }
    }

    return end_name;
}

void createSound(string &db_dir_path,vector<Sounds> &song_catalog,const int choice, ScreenState* screen_state_ptr)
{

    Sounds new_song;
    vector<string> lyrics;

    switch (choice)
    {
        case 1:
        {   
            system("clear");
            cout << PROMPT << lang[PROMPT_ENTER_TITLE] << RESET;
            getline(cin,new_song.title);

            cout << PROMPT << lang[PROMPT_ENTER_AUTHOR] << RESET;
            getline(cin,new_song.author);

            cout << PROMPT << lang[PROMPT_ENTER_YEAR] << RESET;
            cin>>new_song.year;
            cin.ignore();

            break;
        }
        
        case 2:
        {
            clear();
            new_song.title = get_string_from_user(0, 0, lang[PROMPT_ENTER_TITLE]);
            
            clear();
            new_song.author = get_string_from_user(0, 0, lang[PROMPT_ENTER_AUTHOR]);

            clear();
            new_song.year = stoi(get_string_from_user(0, 0, lang[PROMPT_ENTER_YEAR]));

            break;
        }

    }
    

    string titleNoneSpase = zamenaSpasNa_(new_song.title);
    string authorNoneSpase = zamenaSpasNa_(new_song.author);

    new_song.source_filename = db_dir_path+"/"+titleNoneSpase+"_"+authorNoneSpase+".txt";


    int vubor;
    switch (choice)
    {
        case 1:
        { 
            cout << endl << HEADER << lang[TITLE_ADD_TEXT_MENU] << RESET << endl;
            cout << LABEL << "1) " << lang[MENU_ADD_TEXT_MANUALLY] << endl;
            cout << LABEL << "2) " << lang[MENU_ADD_TEXT_FROM_FILE] << endl;
            cout << PROMPT << lang[PROMPT_YOUR_CHOICE] << RESET;
            cin >> vubor;
            cin.ignore(); 
            break;
        }

        case 2:
        {
            // vector<string> items = {"1)добавление в ручную","2)Добавление из файла(указать полный путь к файлу с текстом)"};
            vector<string> items = {lang[MENU_ADD_TEXT_MANUALLY], lang[MENU_ADD_TEXT_FROM_FILE]};
            int menu_choice = show_menu(lang[TITLE_ADD_TEXT_MENU], items);
            vubor = menu_choice + 1;
            break;
        }

    }


    if(vubor == 1)
    {
        switch (choice)
        {
            case 1:
            { 
                cout << lang[PROMPT_ADD_TEXT_MANUAL_STOP] << endl;
                while (true)
                {
                    string line;
                    getline(cin,line);
                    if(line == "*")
                    {
                        break;
                    }else{
                        lyrics.push_back(line);
                    }
                }
                break;
            }
            case 2:
            {
                clear();
                lyrics = get_lyrics_from_user(0, 0);


                break;
            }


        }
        
    }else if(vubor == 2)
    {
        
        string fullPathToSoundText;


        switch (choice)
        {
            case 1:
            { 
                cout << lang[PROMPT_ADD_TEXT_FILE_PATH];
                getline(cin, fullPathToSoundText);
                break;
            }

            case 2:
            {
                clear();
                fullPathToSoundText = get_string_from_user(0, 0, lang[PROMPT_ADD_TEXT_FILE_PATH]);
                break;
            }

        }



        ifstream fileR(fullPathToSoundText);
        if(!fileR.is_open())
        {
            if(choice != 2)
            {
                cout << lang[ERROR_FILE_OPEN] << endl;
            }
            return;
        }

        string line;

        while (getline(fileR,line))
        {
            lyrics.push_back(line);
        }
        
    }




    song_catalog.push_back(new_song);

    saveSongToFile(new_song,lyrics,choice);

    if(screen_state_ptr != nullptr)
    {
        *screen_state_ptr = MAIN_MENU;
    }

}


void displayAllSongs(vector<Sounds> &song_catalog)
{
    cout<<endl;
    cout << endl << HEADER << "================ " << lang[HEADER_ALL_SONGS] << " ================" << RESET << endl << endl;
    int i = 1;
    for(Sounds song : song_catalog)
    {
        cout << HEADER << lang[SONG_NUMBER_PREFIX] << HIGHLIGHT << i << RESET << endl;
            cout << LABEL << lang[SONG_TITLE_PREFIX] << VALUE << song.title << endl;
            cout << LABEL << lang[SONG_AUTHOR_PREFIX] << VALUE << song.author << endl;
            cout << LABEL << lang[SONG_YEAR_PREFIX] << VALUE << song.year << RESET << endl;
            cout << "----------------------------------------" << endl << endl;
        i++;
    }



    cout << HEADER << "================ " << lang[FOOTER_ALL_SONGS] << " ===================" << RESET << endl << endl;
}

void deleteSong(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state )
{
    int delated_index;
    switch (choice)
    {
        case 1:
        {
            cout << PROMPT << lang[PROMPT_CHOOSE_SONG_TO_DELETE] << RESET;
            cin>>delated_index;
            break;
        }

        case 2:
        {
            clear();
            delated_index = stoi(get_string_from_user(0,0,lang[PROMPT_CHOOSE_SONG_TO_DELETE]));
            break;
        }

    }
    

    string delate_path_file = song_catalog[delated_index-1].source_filename;

    song_catalog.erase(song_catalog.begin()+delated_index-1);
    filesystem::remove(delate_path_file);

    if(screen_state != nullptr)
    {

        *screen_state = MAIN_MENU;
    }

}


void updateAndRenameSong(Sounds &song_to_edit,const vector<string> &soung_text,const int choice)
{

    string old_full_path = song_to_edit.source_filename;

    string new_song_name = zamenaSpasNa_(song_to_edit.title);
    string new_author_name = zamenaSpasNa_(song_to_edit.author);
    string new_file_name = new_song_name + "_" + new_author_name + ".txt";

    filesystem::path old_full_type_path = old_full_path;
    
    string new_full_path = old_full_type_path.parent_path().string() + "/" + new_file_name;


    if(old_full_path != new_full_path)
    {
        filesystem::rename(old_full_path,new_full_path);
    }

    song_to_edit.source_filename = new_full_path;

    saveSongToFile(song_to_edit, soung_text,choice);
}

vector<string> readTextFromFile(string &path_to_file)
{
    ifstream fileR(path_to_file);
    if(!fileR.is_open())
    {
        cout << lang[ERROR_FILE_OPEN] << endl;
        return vector<string>();
    }

    vector<string> soung_text;

    string line;
    bool is_started = false;
    while (getline(fileR,line))
    {
        if(line == "-----------------------------")
        {
            is_started = !is_started;
            continue;
        }
        if(is_started == true)
        {
            soung_text.push_back(line);
        }
    }



    fileR.close();

    return soung_text;
}

void writeTextToFile(string &path_to_file,const vector<string> &soung_text)
{

    ofstream fileW(path_to_file);
    for(string line: soung_text)
    {
        fileW<<line<<endl;
    }

    fileW.close();
}

void editSong(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state)
{

    int redacted_index;
    switch (choice)
    {
        case 1:
        {
            cout << lang[PROMPT_CHOOSE_SONG_TO_EDIT];
            cin>>redacted_index;
            cin.ignore();
            break;
        }

        case 2:
        {
            clear();
            redacted_index = stoi(get_string_from_user(0, 0,lang[PROMPT_CHOOSE_SONG_TO_EDIT]));
            break;
        }
    

    }
    
    
    



    if (redacted_index < 1 || redacted_index > song_catalog.size()) {
        if(choice == 1)
        {
            cout << lang[ERROR_INVALID_SONG_NUMBER] << endl;
        }
        return;
    }



    vector<string> soung_text = readTextFromFile(song_catalog[redacted_index-1].source_filename);
    Sounds &song_to_edit = song_catalog[redacted_index-1];

    int vubor;

    switch (choice)
    {
        case 1:
        {
            cout << endl << lang[PROMPT_EDIT_CHOICE] << ":" << endl;
            
            cout << "1) " << lang[MENU_EDIT_TITLE] << endl;
            cout << "2) " << lang[MENU_EDIT_AUTHOR] << endl;
            cout << "3) " << lang[MENU_EDIT_YEAR] << endl;
            cout << "4) " << lang[MENU_EDIT_LYRICS] << endl;
            
            cout << lang[PROMPT_YOUR_CHOICE]<<": ";
            cin>>vubor;
            cin.ignore();
            break;
        }

        case 2:
        {
            // vector<string> items_menu = {"1)Название песни","2)Имя автора","3)Год выпуска","4)Текст песни(Прийдется писать весь заново)"};
            vector<string> items_menu = {
                lang[MENU_EDIT_TITLE],
                lang[MENU_EDIT_AUTHOR],
                lang[MENU_EDIT_YEAR],
                lang[MENU_EDIT_LYRICS]
            };
            vubor = show_menu(lang[PROMPT_EDIT_CHOICE], items_menu)+1;
            break;
        }
    
    }

    

    
    switch (vubor)
    {
        case 1:
        {   

            switch (choice)
            {
                case 1:
                {
                    cout<<lang[MENU_EDIT_TITLE]<<": ";
                    getline(cin,song_to_edit.title);
                    break;
                }

                case 2:
                {
                    clear();
                    song_to_edit.title = get_string_from_user(0, 0,lang[MENU_EDIT_TITLE]);
                    break;
                }
            
            }
            break;
        }

        case 2:
        {

            switch (choice)
            {
                case 1:
                {
                    cout<<lang[MENU_EDIT_AUTHOR]<<": ";
                    getline(cin,song_to_edit.author);
                    break;
                }

                case 2:
                {
                    clear();
                    song_to_edit.author = get_string_from_user(0,0,lang[MENU_EDIT_AUTHOR]);
                    break;
                }
            
            }
            break;
        }

        case 3:
        {

            switch (choice)
            {
                case 1:
                {
                    cout<<lang[MENU_EDIT_YEAR]<<": ";
                    cin>>song_to_edit.year;
                    break;
                }

                case 2:
                {
                    clear();
                    song_to_edit.year = stoi(get_string_from_user(0,0,lang[MENU_EDIT_YEAR]));
                    break;
                }
            
            }
            break;
        }

        case 4:
        {
            soung_text.clear();


            switch (choice)
            {
                case 1:
                {
                    cout<<lang[PROMPT_ADD_TEXT_MANUAL_STOP]<<endl;
                    while (true)
                    {
                        string line;
                        getline(cin,line);
                        if(line == "*")
                        {
                            break;
                        }else{
                            soung_text.push_back(line);
                        }
                    }
                    break;
                }

                case 2:
                {
                    clear();
                    soung_text = get_lyrics_from_user(0,0);
                    break;
                }
            
            }
            break;
        }

    }

    updateAndRenameSong(song_to_edit,soung_text,choice);

    if(screen_state != nullptr)
    {
        *screen_state = MAIN_MENU;
    }
}



void findSongsByAuthor(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state)
{

    string author_name;

    switch (choice)
    {
        case 1:
        {   
            system("clear");
            cout << PROMPT << lang[PROMPT_FIND_BY_AUTHOR] << RESET;
            getline(cin,author_name);
            break;
        }

        case 2:
        {
            clear();
            author_name = get_string_from_user(0,0,lang[PROMPT_FIND_BY_AUTHOR]);
            break;
        }
    
    
    }

    int current_y = 1;
    const int x_coordinate = 4;
    switch (choice)
            {
                case 1:
                {
                    cout << endl << HEADER << lang[MSG_AUTHOR_SONGS_ARE] << RESET << endl;
                    cout << "--------------------------------" << endl;
                    break;
                }

                case 2:
                {
                    clear();
                    mvprintw(current_y,x_coordinate,"%s",lang[MSG_AUTHOR_SONGS_ARE].c_str());
                    current_y++;
                    break;
                }
            
            }
    

    int count = 0;
    for(const Sounds& name: song_catalog)
    {
        if(name.author == author_name)
        {
            switch (choice)
            {
                case 1:
                {
                    cout << VALUE << count+1 << ") " << name.title << " (" << name.year << lang[MSG_YEAR_SUFFIX] << ")" << RESET << endl;
                    break;
                }

                case 2:
                {
                    string formatted_output = to_string(count + 1) + ") " + name.title + " " + to_string(name.year) + lang[MSG_YEAR_SUFFIX];
                    mvprintw(current_y,x_coordinate, "%s", formatted_output.c_str());
                    current_y++;
                    refresh();
                    break;
                }
            
            }

            count++;
        }
    }

    if(count == 0)
    {
        switch (choice)
            {
                case 1:
                {
                    cout << HIGHLIGHT << lang[MSG_AUTHOR_SONGS_NOT_FOUND] << RESET << endl;
                    break;
                }

                case 2:
                {
                    clear();
                    mvprintw(current_y,x_coordinate,"%s",lang[MSG_AUTHOR_SONGS_NOT_FOUND].c_str());
                    break;
                }
            
            }

    }

    if (choice == 2) {
        int y = getmaxy(stdscr);
        mvprintw(y - 1, 2,"%s", lang[PROMPT_PRESS_ANY_KEY].c_str());
        refresh();
        getch(); 
    }


    if(screen_state != nullptr)
    {
        *screen_state = MAIN_MENU;
    }
}



void findSongsByWord(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state)
{
    string word;

    switch (choice)
    {
        case 1:
        {
            cout << lang[PROMPT_FIND_BY_WORD];
            cin>>word;
            cin.ignore();
            break;
        }

        case 2:
        {
            clear();
            word = get_string_from_user(0,0,lang[PROMPT_FIND_BY_WORD]);
            break;
        }
            
    }
    

    int current_y = 2; 
    const int x_coordinate = 4;
    int found_count = 0; 

    if (choice == 2) {
        clear();
        mvprintw(0, 2, lang[MSG_SEARCH_RESULTS_FOR_WORD].c_str(), word.c_str());
    } else {
        cout << HEADER << "--- " << format_string(lang[MSG_SEARCH_RESULTS_FOR_WORD], word) << " ---" << RESET << endl << endl;
    }

    for(Sounds song: song_catalog)
    {
        string path_to_file = song.source_filename;
        vector<string> soung_text = readTextFromFile(path_to_file);

        bool word_found_in_this_song = false;
        for(string linee : soung_text)
        {
            if(linee.find(word) != string::npos)
            {
                word_found_in_this_song = true;
                break;
            }
        }

        if(word_found_in_this_song == true)
        {
            found_count++; 
            if (choice == 1) {
                cout << VALUE << found_count << ") " << song.title << LABEL << lang[MSG_AUTHOR_SUFFIX] << VALUE << song.author << RESET << endl;
            } else {
                string formatted_output = to_string(found_count) + ") " + song.title + lang[MSG_AUTHOR_SUFFIX] + song.author;
                mvprintw(current_y, x_coordinate, "%s", formatted_output.c_str());
                current_y++; 
            }

            
        }

    }

    if (found_count == 0) {
        if (choice == 1) {
            cout << endl << HIGHLIGHT << lang[MSG_WORD_NOT_FOUND] << RESET << endl;
        } else {
            mvprintw(current_y, x_coordinate, "%s",lang[MSG_WORD_NOT_FOUND].c_str());
        }
    }

    if (choice == 2) {
        int y, x;
        getmaxyx(stdscr, y, x);
        mvprintw(y - 1, 2, "%s",lang[PROMPT_PRESS_ANY_KEY].c_str());
        refresh();
        getch();
    }



    if (screen_state != nullptr)
    {
        *screen_state = MAIN_MENU;
    }
}


void displayFullSong(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state)
{
    int vubor = 0 ;
    int number = 0;

    switch (choice)
    {
        case 1:
        {
            system("clear");
            cout << HEADER << "--- " << lang[TITLE_DISPLAY_MODE_MENU] << " ---" << RESET << endl;
            cout << LABEL << "1) " << lang[MENU_DISPLAY_TERMINAL] << endl;
            cout << LABEL << "2) " << lang[MENU_DISPLAY_EDITOR] << endl << endl;
            cout << PROMPT << lang[PROMPT_YOUR_CHOICE] << RESET;
            
            cin >> vubor;

            cout << PROMPT << lang[PROMPT_CHOOSE_SONG_TO_DISPLAY] << RESET;
            cin >> number;

            cin.ignore();
            break;
        }
        case 2:
        {
            clear();
            vector<string> items = {lang[MENU_DISPLAY_TERMINAL], lang[MENU_DISPLAY_EDITOR]};
            
            int menu_choice = show_menu(lang[TITLE_DISPLAY_MODE_MENU], items);
            vubor = menu_choice + 1;

            clear();
            number = stoi(get_string_from_user(0, 0, lang[PROMPT_CHOOSE_SONG_TO_DISPLAY]));
            
            break;
        }

    }

    if (number < 1 || number > song_catalog.size()) {
        if (choice == 1) {
            cout << endl << ERROR << lang[ERROR_INVALID_SONG_NUMBER] << RESET << endl;
        }
        if(screen_state != nullptr)
        {
            *screen_state = MAIN_MENU;
        }
        return;
    }
    

    switch (vubor)
    {
        case 1:
            {   
                string file_path = song_catalog[number-1].source_filename;
                vector<string> soung_text = readTextFromFile(file_path);
                switch (choice)
                {
                    case 1:
                    {
                        system("clear");
                        cout << HEADER << "========================================" << endl;
                        cout << LABEL << lang[SONG_TITLE_PREFIX] << VALUE << song_catalog[number-1].title << endl;
                        cout << LABEL << lang[SONG_AUTHOR_PREFIX] << VALUE << song_catalog[number-1].author << endl;
                        cout << LABEL << lang[SONG_YEAR_PREFIX] << VALUE << song_catalog[number-1].year << endl;
                        cout << HEADER << "========================================" << RESET << endl << endl;

                        cout << HEADER << lang[HEADER_LYRICS] << RESET << endl;
                        
                        
                        


                        for(string linee : soung_text)
                        {
                            cout << VALUE << linee << RESET << endl;
                        }

                        cout << HEADER << lang[FOOTER_LYRICS] << RESET << endl;

                        break;
                    }

                    case 2:
                    {
                        printLyricWithncurses(song_catalog[number-1],soung_text);
                        break;
                    }
                
                }

            

            break;
        }
        case 2:
        {
            string command  = "xdg-open \"" + song_catalog[number-1].source_filename + "\"";
            system(command.c_str());
            break;
        }
    }

    if(screen_state != nullptr)
    {
        *screen_state = MAIN_MENU;
    }

}


void saveTextSongToFile(vector<Sounds> &song_catalog,const int choice,ScreenState* screen_state)
{
    int song_index;
    switch (choice)
    {
        case 1:
        {
            cout << lang[PROMPT_CHOOSE_SONG_TO_SAVE];
            cin>>song_index;
            cin.ignore();
            break;
        }

        case 2:
        {
            clear();
            song_index = stoi(get_string_from_user(0,0,lang[PROMPT_CHOOSE_SONG_TO_SAVE]));

            break;
        }
    

    }

    

    string path_to_file = song_catalog[song_index-1].source_filename;
    vector<string> soung_text = readTextFromFile(path_to_file);


    switch (choice)
    {
        case 1:
        {
            cout << lang[PROMPT_ENTER_FULL_PATH];
            getline(cin,path_to_file);
            break;
        }

        case 2:
        {
            clear();
            path_to_file = get_string_from_user(0,0,lang[PROMPT_ENTER_FULL_PATH]);

            break;
        }
    

    }


    writeTextToFile(path_to_file,soung_text);

    if(screen_state != nullptr)
    {
        *screen_state = MAIN_MENU;
    }

}













bool loadLanguagePack(const string& lang_code) {
    string file_path = "./lang/" + lang_code + ".txt";
    ifstream fileR(file_path);

    if (!fileR.is_open()) {
        cout << lang[ERROR_FILE_OPEN] << endl;
        return false;
    }

    vector<string> new_lang_pack;
    string line;
    while (getline(fileR, line)) {
        new_lang_pack.push_back(line);
    }
    fileR.close();

    if (new_lang_pack.size() != STRING_COUNT) {
        return false;
    }

    lang = new_lang_pack; 
    return true;
}

void changeLanguage(const int choice, ScreenState* screen_state_ptr) {
    string lang_dir_path = "./lang";
    vector<string> available_languages;
    vector<string> lang_names;

    if (filesystem::exists(lang_dir_path) && filesystem::is_directory(lang_dir_path)) {
        for (const auto& entry : filesystem::directory_iterator(lang_dir_path)) {
            if (entry.path().extension() == ".txt") {
                string lang_code = entry.path().stem().string();
                available_languages.push_back(lang_code);

                if (lang_code == "en") lang_names.push_back("English");
                else if (lang_code == "ru") lang_names.push_back("Русский");
                else if (lang_code == "ua") lang_names.push_back("Українська");
                else lang_names.push_back(lang_code); 
            }
        }
    }

    if (available_languages.empty()) {
        if (screen_state_ptr) *screen_state_ptr = MAIN_MENU;
        return;
    }

    int lang_choice = -1;

    if (choice == 1) { 
        cout << endl << HEADER << "Виберіть мову:" << RESET << endl;
        for (int i = 0; i < lang_names.size(); ++i) {
            cout << LABEL << i + 1 << ") " << lang_names[i] << RESET << endl;
        }
        cout << PROMPT << lang[PROMPT_YOUR_CHOICE] << RESET;
        cin >> lang_choice;
        cin.ignore();
        lang_choice--; 
    } else { // Ncurses режим
        lang_choice = show_menu("Виберіть мову", lang_names);
        if (lang_choice == -1) { // нажал Esc
            if (screen_state_ptr) *screen_state_ptr = MAIN_MENU;
            return;
        }
    }
    

    if (lang_choice >= 0 && lang_choice < available_languages.size()) {
        string lang_code_to_load = available_languages[lang_choice];
        
        loadLanguagePack(lang_code_to_load);
    } else {
        if (choice == 1 && lang_choice != -2) { //   -2   Esc в ncurses
            //  cout << endl << ERROR << "Невірний вибір." << RESET << endl;
        }
    }


    if (screen_state_ptr)
    {
         *screen_state_ptr = MAIN_MENU;
    }
}
