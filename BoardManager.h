#pragma once
#include"User.h"
#include"Board.h"
#include"Viewer.h"
#include<fstream>
using namespace std;
enum BoardState
{
    MENU,
    SELECT_BOARD,
    BOARD,
    POST,
    ESC
};

class BoardManager {
	public:
        std::vector<User> users;
        std::vector<Board> boards;
        Viewer viewer;
        int numberofuser;
        int BoardState;
        int currentuser;
        BoardManager() {
            BoardState = MENU;
        }
        void setcurrentuser(int id) {
            currentuser = id;
        }
        void load() {
            ifstream f("numberofuser.txt");
            int number;
            f >> number;
            numberofuser = number;
            f.close();
            for (int i = 1;i <= numberofuser;i++) {
                string id;
                id += i + '0';
                ifstream userinput(id+".txt");
                User input;
                while (userinput >> input.accountname) {
                    userinput >> input.password;
                    userinput >> input.nickname;
                    userinput >> input.Permission_level;
                    users.push_back(input);
                }
                userinput.close();
            }
            
        }
        void loop() {
            load();
            while (1) {
                if (BoardState == MENU) {
                    menu();
                }
                else if (BoardState == SELECT_BOARD) {
                    selectBoard();
                }
                else if (BoardState == ESC) {
                    return;
                }
                system("CLS");
            }
        }
        void menu() {
            viewer.printMenu();
            std::string act;
            while (std::cin >> act) {
                if (act == "guest") {
                    BoardState = SELECT_BOARD;
                    setcurrentuser(-1);
                    break;
                }
                else if (act == "new") {
                    numberofuser++;
                    string id;
                    id = to_string(numberofuser);
                    ofstream f(id+".txt");
                    User input;                                   
                    while (1) {
                        cout << "請輸入帳號:";
                        cin >> input.accountname;
                        bool notsame = true;
                        for (int i = 0;i < users.size();i++) {
                            if (input.accountname == users[i].accountname) {
                                cout << "此帳號已與現有帳號重複!" << endl;
                                notsame = false;
                                break;
                            }
                        } 
                        if (notsame)break;
                    }
                    f << input.accountname << " ";
                    cout << "請輸入密碼:";
                    cin >> input.password;
                    f << input.password << " ";
                    cout << "請輸入暱稱:";
                    cin >> input.nickname;
                    f << input.nickname << " ";
                    input.Permission_level = USER;
                    f << input.Permission_level << " ";
                    f.close();
                    users.push_back(input);
                    ofstream plusnumber("numberofuser.txt");
                    plusnumber << numberofuser;
                    plusnumber.close();
                    BoardState = SELECT_BOARD;
                    break;
                }
                else if (act == "r") {
                    BoardState = ESC;
                    return;
                }
                else {
                    for (int i = 0;i < users.size();i++) {
                        if (act == users[i].accountname) {
                            string inputpassword;
                            cout << "請輸入密碼:";
                            while (cin >> inputpassword) {
                                if (inputpassword == users[i].password) {
                                    currentuser = i;
                                    cout << "歡迎登入! " << users[i].nickname<<endl;
                                    BoardState = SELECT_BOARD;
                                    system("pause");
                                    return;
                                }
                                else {
                                    cout << "密碼錯誤!" << endl;
                                    cout << "請輸入密碼:";
                                }
                            }
                        }
                    }
                    cout << "查無此帳號!"<<endl;
                    system("pause");
                    break;
                }
            }
        }
        void selectBoard() {
            viewer.printchooseBoard();
            char input;
            while (cin >> input) {
                if (input == 'r') {
                    BoardState = MENU;
                    return;
                }
                else {

                }
            }
        }
};
