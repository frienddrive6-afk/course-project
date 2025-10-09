#include <iostream>
#include <string>
#include <filesystem>


using namespace std;


int main()
{
    if(!filesystem::exists("./sonds"))
    {
        filesystem::create_directory("./sonds");
    }

    if(filesystem::is_directory("./sonds"))
    {
        cout<<"good"<<endl;
        // filesystem::remove("./sonds");
    }


    for (const auto& entry : filesystem::directory_iterator("./sonds")) {

        
        string filename = entry.path().filename().string();

        cout<<entry.path()<<endl;
        cout<<entry.path().filename()<<endl;
        cout<<entry.path().filename().string()<<endl;;


        // if (entry.is_regular_file()) {
        //     cout << "   Знайдено ФАЙЛ: " << filename << endl;
        // } else if (entry.is_directory()) {
        //     cout << "   Знайдено ПІДПАПКУ: " << filename << endl;
        // }
    }


    filesystem::rename("sonds/rwewr.txt", "sonds/1/newfilename.txt");

    return 0;
}