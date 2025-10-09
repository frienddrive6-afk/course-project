#include <iostream>
#include <string>
#include <fstream> // Для створення файлу
#include <cstdlib> // Для функції system()


using namespace std;

int main()
{

    string command = "xdg-open \"sonds/asdasdasd.txt\"";
    system(command.c_str());



    return 0;
}