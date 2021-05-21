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
        int numberofboard;
        int currentboard;

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
                while (getline(userinput , input.accountname) ){
                    getline(userinput, input.password);
                    getline(userinput, input.nickname);
                    userinput >> input.Permission_level;
                    users.push_back(input);
                }
                userinput.close();
            }
            ifstream boardnum("numberofboard.txt");
            boardnum >> numberofboard;
            boardnum.close();
            for (int i = 1;i <= numberofboard;i++) {
                string id;
                id += i + '0';
                Board input;
                ifstream inputboard("Board"+id+".txt");
                inputboard >> input.Status >> input.BoardPost >> input.Boardname>>input.BoardID;
                if(input.Status==AVAILABLE)
                boards.push_back(input);
                inputboard.close();
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
                else if(BoardState==BOARD){
                    viewBoard();
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
                    f << input.accountname <<endl;
                    cout << "請輸入密碼:";
                    cin >> input.password;
                    f << input.password <<endl;
                    cout << "請輸入暱稱:";
                    cin >> input.nickname;
                    f << input.nickname <<endl;
                    input.Permission_level = USER;
                    f << input.Permission_level <<endl;
                    f.close();
                    users.push_back(input);
                    ofstream plusnumber("numberofuser.txt");
                    plusnumber << numberofuser;
                    plusnumber.close();
                    currentuser = users.size() - 1;
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
            cout << "吧合母待>>看板選擇" << endl;
            for (int i = 0;i < boards.size();i++) {
                cout << "(" << i << ")" << boards[i].Boardname << endl<<endl;
            }
            if (currentuser>=0) {
                if (users[currentuser].Permission_level == ADMINSTRATOR) {
                    cout << "輸入create創建看板,輸入delete刪除看板,";
                    viewer.printchooseBoard();
                    string input;
                    while (cin >> input) {
                        if (input == "r") {
                            BoardState = MENU;
                            return;
                        }
                        else if (input == "create") {
                            createBoard();
                            return;
                        }
                        else if (input == "delete") {
                            if (boards.size() >= 1) {
                                cout << "輸入看板編號,刪除該看板:";
                                string boardnum;
                                while (cin >> boardnum) {
                                    bool correctact = true;
                                    for (int i = 0;i < boardnum.size();i++) {
                                        if (boardnum[i] - '0' < 0 || boardnum[i] - '0' > 9) {
                                            correctact = false;
                                            break;
                                        }
                                    }
                                    if (correctact) {
                                        if (stoi(boardnum) > boards.size() - 1 || stoi(boardnum) < 0) {
                                            correctact = false;
                                        }
                                    }

                                    if (correctact) {
                                        ofstream del("Board" + to_string(boards[boardnum[0] - '0'].BoardID) + ".txt");
                                        boards[boardnum[0] - '0'].Status = DISABLE;
                                        del << boards[boardnum[0] - '0'].Status << endl << boards[boardnum[0] - '0'].BoardPost << endl << boards[boardnum[0] - '0'].Boardname << endl << boards[boardnum[0] - '0'].BoardID;
                                        del.close();
                                        cout << "刪除成功" << endl;
                                        reset();
                                        load();
                                        system("pause");
                                        return;
                                    }
                                    else {
                                        cout << "輸入非法!" << endl;
                                        cout << "輸入看板編號,刪除該看板:";
                                    }
                                }
                            }
                            else {
                                cout << "站上沒有任何看板" << endl;
                                system("pause");
                                return;
                            }
                        }
                        else {
                            for (int i = 0;i < input.size();i++) {
                                if (input[i] - '0' < 0 || input[i] - '0' > 9) {
                                    cout << "輸入非法!" << endl;
                                    system("pause");
                                    return;
                                }
                            }
                            if (stoi(input) < 0 || stoi(input) > 9) {
                                cout << "輸入非法!" << endl;
                                system("pause");
                                return;
                            }
                            else {
                                currentboard = stoi(input);
                                BoardState = BOARD;
                                return;
                            }
                        }
                    }
                }
                else {
                    viewer.printchooseBoard();
                    string input;
                    while (cin >> input) {
                        if (input == "r") {
                            BoardState = MENU;
                            return;
                        }
                        else {
                            for (int i = 0;i < input.size();i++) {
                                if (input[i] - '0' < 0 || input[i] - '0' > 9) {
                                    cout << "輸入非法!" << endl;
                                    system("pause");
                                    return;
                                }
                            }
                            if (stoi(input) < 0 || stoi(input) > 9) {
                                cout << "輸入非法!" << endl;
                                system("pause");
                                return;
                            }
                            else {
                                currentboard = stoi(input);
                                BoardState = BOARD;
                                return;
                            }
                        }
                    }
                }
            }
            else {
                viewer.printchooseBoard();
                string input;
                while (cin >> input) {
                    if (input == "r") {
                        BoardState = MENU;
                        return;
                    }
                    else {
                        for (int i = 0;i < input.size();i++) {
                            if (input[i] - '0' < 0 || input[i] - '0' > 9) {
                                cout << "輸入非法!" << endl;
                                system("pause");
                                return;
                            }
                        }
                        if (stoi(input) < 0 || stoi(input) > 9) {
                            cout << "輸入非法!" << endl;
                            system("pause");
                            return;
                        }
                        else {
                            currentboard = stoi(input);
                            BoardState = BOARD;
                            return;
                        }
                    }
                }
            }
        }

        void createBoard() {
            Board input;
            cout << "請輸入看板名稱:";
            while (cin >> input.Boardname) {
                bool notsame = true;
                for (int i = 0;i < boards.size();i++) {
                    if (input.Boardname == boards[i].Boardname) {
                        notsame = false;
                        break;
                    }
                }
                if (notsame) {
                    break;
                }
                else {
                    cout << "看板名稱已與現有看板重複!" << endl;
                    cout<< "請輸入看板名稱:";
                }
            }
            numberofboard++;
            input.BoardID = numberofboard;
            ofstream f("Board" + to_string(input.BoardID)+".txt");
            f << input.Status << endl << input.BoardPost << endl << input.Boardname<<endl<<input.BoardID;
            f.close();
            ofstream addnumber("numberofboard.txt");
            addnumber << numberofboard;
            addnumber.close();
            boards.push_back(input);
        }

        void reset() {
            numberofboard = 0;
            numberofuser = 0;
            users.clear();
            boards.clear();
        }

        void viewBoard() {
            cout<< "吧合母待>>"<<boards[currentboard].Boardname << endl<<endl;
            viewer.printviewBoard();
            string act;
            while (cin >> act) {
                if (act == "r") {
                    BoardState = SELECT_BOARD;
                    return;
                }
            }
        }
};
