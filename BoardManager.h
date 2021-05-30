#pragma once
#include"User.h"
#include"Board.h"
#include"Viewer.h"
#include<fstream>
using namespace std;
enum useforreplymail {
    REPLY,
    DELETEPOST,
    DELETECOMMENT
};

enum BoardState
{
    MENU,
    SELECT_BOARD,
    BOARD,
    POST,
    MAILLIST,
    MAIL,
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
                    userinput >> input.mails;
                    users.push_back(input);
                }
                userinput.close();
            }
            for (int i = 0;i < users.size();i++) {
                users[i].mailload();
            }
            loadboard();
        }

        void loadboard() {
            ifstream boardnum("numberofboard.txt");
            boardnum >> numberofboard;
            boardnum.close();
            for (int i = 1;i <= numberofboard;i++) {
                string id;
                id += i + '0';
                Board input;
                ifstream inputboard("Board" + id + ".txt");
                inputboard >> input.Status >> input.BoardPost >> input.Boardname >> input.BoardID;
                if (input.Status == AVAILABLE)
                    boards.push_back(input);
                inputboard.close();
            }
            for (int i = 0;i < boards.size();i++) {
                boards[i].loadpost();
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
                else if (BoardState == POST) {
                    viewpost();
                }
                else if (BoardState == MAILLIST) {
                    viewmaillist();
                }
                else if (BoardState == MAIL) {
                    viewmail();
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
            while (getline(cin, act)) {
                
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
                        cout << "�п�J�b��:";
                        cin >> input.accountname;
                        bool notsame = true;
                        for (int i = 0;i < users.size();i++) {
                            if (input.accountname == users[i].accountname) {
                                cout << "���b���w�P�{���b������!" << endl;
                                notsame = false;
                                break;
                            }
                        } 
                        if (notsame)break;
                    }
                    f << input.accountname <<endl;
                    cout << "�п�J�K�X:";
                    cin >> input.password;
                    f << input.password <<endl;
                    cout << "�п�J�ʺ�:";
                    cin >> input.nickname;
                    f << input.nickname <<endl;
                    input.Permission_level = USER;
                    f << input.Permission_level <<endl;
                    f << 0;
                    f.close();
                    users.push_back(input);
                    ofstream plusnumber("numberofuser.txt");
                    plusnumber << numberofuser;
                    plusnumber.close();
                    currentuser = users.size() - 1;
                    BoardState = SELECT_BOARD;
                    cin.ignore();
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
                            cout << "�п�J�K�X:";
                            while (cin >> inputpassword) {
                                cin.ignore();
                                if (inputpassword == users[i].password) {
                                    currentuser = i;
                                    cout << "�w��n�J! " << users[i].nickname<<endl;
                                    BoardState = SELECT_BOARD;
                                    system("pause");

                                    return;
                                }
                                else {
                                    cout << "�K�X���~!" << endl;
                                    cout << "�п�J�K�X:";
                                }
                            }
                        }
                    }
                    cout << "�d�L���b��!"<<endl;
                    system("pause");
                    break;
                }
            }
        }

        void selectBoard() {
            cout << "�a�X����>>�ݪO���" << endl;
            for (int i = 0;i < boards.size();i++) {
                cout << "(" << i << ")" << boards[i].Boardname << endl<<endl;
            }
            if (currentuser>=0) {
                if (users[currentuser].Permission_level == ADMINSTRATOR) {
                    cout << "��Jmail�i�J�H�c,��Jcreate�ЫجݪO,��Jdelete�R���ݪO,";
                    viewer.printchooseBoard();
                    string input;
                    while (getline(cin, input)) {
                        if (input == "r") {
                            BoardState = MENU;
                            return;
                        }
                        else if (input == "mail") {
                            BoardState = MAILLIST;
                            return;
                        }
                        else if (input == "create") {
                            createBoard();
                            return;
                        }
                        else if (input == "delete") {
                            if (boards.size() >= 1) {
                                cout << "��J�ݪO�s��,�R���ӬݪO:";
                                string boardnum;
                                while (getline(cin, boardnum)) {
                                    //cin.ignore();
                                    bool correctact = true;
                                    for (int i = 0;i < boardnum.size();i++) {
                                        if (boardnum[i] - '0' < 0 || boardnum[i] - '0' > 9) {
                                            correctact = false;
                                            break;
                                        }
                                    }
                                    if (correctact) {
                                        if (stoi(boardnum) > numberofboard - 1 || stoi(boardnum) < 0) {
                                            correctact = false;
                                        }
                                    }

                                    if (correctact) {
                                        ofstream del("Board" + to_string(boards[boardnum[0] - '0'].BoardID) + ".txt");
                                        boards[boardnum[0] - '0'].Status = DISABLE;
                                        del << boards[boardnum[0] - '0'].Status << endl << boards[boardnum[0] - '0'].BoardPost << endl << boards[boardnum[0] - '0'].Boardname << endl << boards[boardnum[0] - '0'].BoardID;
                                        del.close();
                                        cout << "�R�����\" << endl;
                                        reset();
                                        load();
                                        system("pause");
                                        return;
                                    }
                                    else {
                                        cout << "��J�D�k!" << endl;
                                        cout << "��J�ݪO�s��,�R���ӬݪO:";
                                    }
                                }
                            }
                            else {
                                cout << "���W�S������ݪO" << endl;
                                system("pause");
                                return;
                            }
                        }
                        else {
                            for (int i = 0;i < input.size();i++) {
                                if (input[i] - '0' < 0 || input[i] - '0' > 9) {
                                    cout << "��J�D�k!" << endl;
                                    system("pause");
                                    return;
                                }
                            }
                            if (stoi(input) < 0 || stoi(input) > numberofboard -1) {
                                cout << "��J�D�k!" << endl;
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
                    cout << "��Jmail�i�J�H�c,";
                    viewer.printchooseBoard();
                    string input;
                    while (getline(cin, input)) {
                        if (input == "r") {
                            BoardState = MENU;
                            return;
                        }
                        else if (input == "mail") {
                            BoardState = MAILLIST;
                            return;
                        }
                        else {
                            for (int i = 0;i < input.size();i++) {
                                if (input[i] - '0' < 0 || input[i] - '0' > 9) {
                                    cout << "��J�D�k!" << endl;
                                    system("pause");
                                    return;
                                }
                            }
                            if (stoi(input) < 0 || stoi(input) > numberofboard -1) {
                                cout << "��J�D�k!" << endl;
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
                while (getline(cin, input)) {
                   // cin.ignore();
                    if (input == "r") {
                        BoardState = MENU;
                        return;
                    }
                    else {
                        for (int i = 0;i < input.size();i++) {
                            if (input[i] - '0' < 0 || input[i] - '0' > 9) {
                                cout << "��J�D�k!" << endl;
                                system("pause");
                                return;
                            }
                        }
                        if (stoi(input) < 0 || stoi(input) > boards.size() - 1) {
                            cout << "��J�D�k!" << endl;
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
            cout << "�п�J�ݪO�W��:";
            while (cin >> input.Boardname) {               
                cin.ignore();
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
                    cout << "�ݪO�W�٤w�P�{���ݪO����!" << endl;
                    cout<< "�п�J�ݪO�W��:";
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
            cout<< "�a�X����>>"<<boards[currentboard].Boardname << endl<<endl;
            for (int i = 0;i < boards[currentboard].posts.size();i++) {
                cout << "(" << i << ")" << boards[currentboard].posts[i].title << endl << endl;
            }
            if (currentuser >= 0) {
                 
                    cout << "��Jcreate�إ߶K��,��Jdelete�R���K��,";
                    viewer.printviewBoard();
                    string input;

                    while (getline(cin, input)) {
                       // cin.ignore();
                        if (input == "create") {
                            createpost();
                            return;
                        }
                        else if (input == "r") {
                            BoardState = SELECT_BOARD;
                            return;
                        }
                        else if (input == "delete") {
                            deletepost();
                            return;
                        }
                        else {
                            for (int i = 0;i < input.size();i++) {
                                if (input[i] - '0' < 0 || input[i] - '0' > 9) {
                                    cout << "��J�D�k!" << endl;
                                    system("pause");
                                    return;
                                }
                            }
                            if (stoi(input) < 0 || stoi(input) > boards[currentboard].BoardPost - 1) {
                                cout << "��J�D�k!" << endl;
                                system("pause");
                                return;
                            }
                            boards[currentboard].currentpost = stoi(input);
                            BoardState = POST;
                            return;
                        }
                    }
                
            }
            else {
                viewer.printviewBoard();
                string input;
                while (getline(cin, input)) {
                   // cin.ignore();
                    if (input == "r") {
                        BoardState = SELECT_BOARD;
                        return;
                    }
                    else {
                        for (int i = 0;i < input.size();i++) {
                            if (input[i] - '0' < 0 || input[i] - '0' > 9) {
                                cout << "��J�D�k!" << endl;
                                system("pause");
                                return;
                            }
                        }
                        
                        if (stoi(input) < 0 || stoi(input) > boards[currentboard].BoardPost - 1) {
                            cout << "��J�D�k!" << endl;
                            system("pause");
                            return;
                        }
                        boards[currentboard].currentpost = stoi(input);
                        BoardState = POST;
                        return;
                    }
                }
            }
        }

        void createpost() {
            cout << "��J���D,�άO��r���s��K��:";
            string  title;
            while (cin >> title) {
                cin.ignore();
                if (title == "r")return ;
                else {
                    break;
                }
            }
            cout << "�п�J���e,�άO��r���s��K��,�p�G�s�觹���Ы��U������A��Jfin:" <<endl;
            vector<string> cont;
            string input;
            while (getline(cin,input)) {
               // cin.ignore();
                if (input == "r") {
                  
                    return;
                }
                else if (input == "fin") {
                    
                    break;
                }
                else {
                    cont.push_back(input);
                }
            }
            boards[currentboard].BoardPost++;
            ofstream boardupdate("Board" + to_string(boards[currentboard].BoardID) + ".txt");
            boardupdate << boards[currentboard].Status << endl << boards[currentboard].BoardPost << endl << boards[currentboard].Boardname << endl << boards[currentboard].BoardID;
            boardupdate.close();
            Post newpost;
            newpost.title = title;
            newpost.creator = users[currentuser].accountname;
            newpost.PostID = boards[currentboard].BoardPost;
            newpost.contentline = cont.size();
            newpost.content = cont;
            boards[currentboard].posts.push_back(newpost);
            ofstream postinput(boards[currentboard].Boardname + to_string(boards[currentboard].BoardPost) + ".txt");
            postinput << newpost.Status <<endl<<newpost.creator<< endl << newpost.PostID << endl << newpost.title << endl<<newpost.contentline << endl << newpost.comments<<endl<< newpost.like<<endl<< newpost.dislike<<endl;
            for (int i = 0;i < newpost.content.size();i++) {
                postinput << newpost.content[i] << endl;
            }
            postinput.close();
            cout << "�إ߶K�妨�\�I" << endl;
            system("pause");
        }

        void deletepost() {
            cout << "��J���R�����K��s��,�άO��r��^:";
            string act;
            while (getline(cin, act)) {
                //cin.ignore();
                bool correctact=true;
                if (act == "r")return;
                for (int i = 0;i < act.size();i++) {
                    if (act[i] - '0' < 0 || act[i] - '0' > 9) {
                        cout << "��J�D�k!" << endl;
                        correctact = false;
                        break;
                    }
                }
                if (correctact) {
                    if (stoi(act) < 0 || stoi(act) > boards[currentboard].posts.size() - 1) {
                        cout << "��J�D�k!" << endl;
                        correctact = false;
                    }
                }
                if (correctact) {
                    if (users[currentuser].Permission_level == USER) {
                        if (boards[currentboard].posts[stoi(act)].creator != users[currentuser].accountname) {
                            cout << "�o���O�A���K��!" << endl;
                            correctact = false;
                        }
                    }
                }
                if (correctact) {
                    break;
                }
                else {
                    cout << "��J���R�����K��s��,�άO��r��^:";
                }
            }
            int i;
            for (i = 0;i < users.size();i++) {
                if (users[i].accountname == boards[currentboard].posts[stoi(act)].creator) {
                    break;
                }
            }
            replymail(i, "�R����"+ boards[currentboard].Boardname +"���K��q��", DELETEPOST);
            //boards[currentboard].BoardPost--;
            boards[currentboard].posts[stoi(act)].Status = DELETE;
            ofstream updatepost(boards[currentboard].Boardname + to_string(boards[currentboard].posts[stoi(act)].PostID) + ".txt");
            updatepost << boards[currentboard].posts[stoi(act)].Status << endl<< boards[currentboard].posts[stoi(act)].creator<<endl << boards[currentboard].posts[stoi(act)].PostID << endl << boards[currentboard].posts[stoi(act)].title << endl << boards[currentboard].posts[stoi(act)].contentline << endl << boards[currentboard].posts[stoi(act)].comments << endl<< boards[currentboard].posts[stoi(act)].like<<endl<< boards[currentboard].posts[stoi(act)].dislike<<endl;
            for (int i = 0;i < boards[currentboard].posts[stoi(act)].contentline;i++) {
                updatepost << boards[currentboard].posts[stoi(act)].content[i]<<endl;
            }
            for (int i = 0;i < boards[currentboard].posts[stoi(act)].likeordislikeuser.size();i++) {
                updatepost << boards[currentboard].posts[stoi(act)].likeordislikeuser[i] << endl;
            }
            updatepost.close();
           /* ofstream updateboard("Board" + to_string(boards[currentboard].BoardID) + ".txt");
            updateboard << boards[currentboard].Status << endl << boards[currentboard].BoardPost << endl << boards[currentboard].Boardname << endl << boards[currentboard].BoardID;
            updateboard.close();*/
            reset();
            load();
            cout << "�R�����\!" << endl;
            system("pause");
            return;
        }

        void viewpost() {
            cout << "�a�X����>>" << boards[currentboard].Boardname <<">>"<< boards[currentboard].posts[boards[currentboard].currentpost].title << endl << endl;
            for (int i = 0;i < users.size();i++) {
                if (users[i].accountname == boards[currentboard].posts[boards[currentboard].currentpost].creator) {
                    cout << users[i].nickname;
                    break;
                }
            }
            cout << "(" <<boards[currentboard].posts[boards[currentboard].currentpost].creator<<"):" <<endl<<endl;           
            for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].content.size();i++) {
                cout << boards[currentboard].posts[boards[currentboard].currentpost].content[i]<<endl;
            }          
            cout << endl;
            cout <<"�g:"<< boards[currentboard].posts[boards[currentboard].currentpost].like <<"    �N:"<< boards[currentboard].posts[boards[currentboard].currentpost].dislike<<endl<<endl;
            for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].com.size();i++) {
                cout << i + 1 << "��>>";
                for (int j = 0;j < users.size();j++) {
                    if (boards[currentboard].posts[boards[currentboard].currentpost].com[i].accountname == users[j].accountname) {
                        cout << users[j].nickname;
                        break;
                    }
                }
                cout << "(" << boards[currentboard].posts[boards[currentboard].currentpost].com[i].accountname << "):"<< boards[currentboard].posts[boards[currentboard].currentpost].com[i].content<<endl;
                cout << endl;
            }
            if (currentuser >= 0) {
                
                if (users[currentuser].Permission_level == ADMINSTRATOR)cout << "��Jdelete�R���d��,";
                cout << "��Jcomment�d��,��Jlike��dislike���g�μN,";
                if (boards[currentboard].posts[boards[currentboard].currentpost].creator == users[currentuser].accountname) {
                    cout<<"��Jedit�s��K��,";
                }
                viewer.printPost();
                string input;
                while (getline(cin , input)) {
                    //cin.ignore();
                    
                    if (input == "r") {
                        BoardState = BOARD;
                        return;
                    }
                    else if(input=="like"){
                        int temp = boards[currentboard].currentpost;
                        if (boards[currentboard].posts[boards[currentboard].currentpost].creator == users[currentuser].accountname) {
                            cout << "�A�����ۤv���K����g�μN" << endl;
                            system("pause");
                            return;
                        }
                        for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.size();i++) {
                            if (boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser[i] == users[currentuser].accountname) {
                                cout << "�A�w�惡�K����L�g�μN!" << endl;
                                system("pause");
                                return;
                            }
                        }
                        boards[currentboard].posts[boards[currentboard].currentpost].like++;
                        boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.push_back(users[currentuser].accountname);
                        ofstream f(boards[currentboard].Boardname + to_string(boards[currentboard].posts[boards[currentboard].currentpost].PostID) + ".txt");
                        f << boards[currentboard].posts[boards[currentboard].currentpost].Status << endl << boards[currentboard].posts[boards[currentboard].currentpost].creator << endl << boards[currentboard].posts[boards[currentboard].currentpost].PostID << endl << boards[currentboard].posts[boards[currentboard].currentpost].title << endl << boards[currentboard].posts[boards[currentboard].currentpost].contentline << endl << boards[currentboard].posts[boards[currentboard].currentpost].comments << endl << boards[currentboard].posts[boards[currentboard].currentpost].like << endl << boards[currentboard].posts[boards[currentboard].currentpost].dislike<<endl;
                        for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].contentline;i++) {
                            f << boards[currentboard].posts[boards[currentboard].currentpost].content[i] << endl;
                        }
                        for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.size();i++) {
                            f << boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser[i] << endl;
                        }
                        f.close();
                        reset();
                        load();
                        boards[currentboard].currentpost=temp;
                        cout << "�w�惡�K����g!" << endl;
                        system("pause");
                        return;
                    }
                    else if (input == "dislike") {
                        int temp = boards[currentboard].currentpost;
                        if (boards[currentboard].posts[boards[currentboard].currentpost].creator == users[currentuser].accountname) {
                            cout << "�A�����ۤv���K����g�μN" << endl;
                            system("pause");
                            return;
                        }
                        for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.size();i++) {
                            if (boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser[i] == users[currentuser].accountname) {
                                cout << "�A�w�惡�K����L�g�μN!" << endl;
                                system("pause");
                                return;
                            }
                        }
                        boards[currentboard].posts[boards[currentboard].currentpost].dislike++;
                        boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.push_back(users[currentuser].accountname);
                        ofstream f(boards[currentboard].Boardname + to_string(boards[currentboard].posts[boards[currentboard].currentpost].PostID) + ".txt");
                        f << boards[currentboard].posts[boards[currentboard].currentpost].Status << endl << boards[currentboard].posts[boards[currentboard].currentpost].creator << endl << boards[currentboard].posts[boards[currentboard].currentpost].PostID << endl << boards[currentboard].posts[boards[currentboard].currentpost].title << endl << boards[currentboard].posts[boards[currentboard].currentpost].contentline << endl << boards[currentboard].posts[boards[currentboard].currentpost].comments << endl << boards[currentboard].posts[boards[currentboard].currentpost].like << endl << boards[currentboard].posts[boards[currentboard].currentpost].dislike<<endl;
                        for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].contentline;i++) {
                            f << boards[currentboard].posts[boards[currentboard].currentpost].content[i] << endl;
                        }
                        for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.size();i++) {
                            f << boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser[i] << endl;
                        }
                        f.close();
                        reset();
                        load();
                        boards[currentboard].currentpost=temp;
                        cout << "�w�惡�K����N!" << endl;
                        system("pause");
                        return;
                    }
                    else if (input == "comment") {
                        int temp=addcomment();
                        reset();
                        load();
                        boards[currentboard].currentpost = temp;
                        return;
                    }
                    else if (input == "delete") {
                        if (users[currentuser].Permission_level == ADMINSTRATOR) {
                            int temp=deletecomment();
                            reset();
                            load();
                            boards[currentboard].currentpost = temp;
                            return;
                        }
                      
                                          
                    }
                    else if (input == "edit") {
                        if (boards[currentboard].posts[boards[currentboard].currentpost].creator == users[currentuser].accountname) {
                            int now = editpost();
                            reset();
                            load();
                            boards[currentboard].currentpost = now;
                            return;
                        }
                        else {
                            cout << "�o���O�A���K��!" << endl;
                            system("pause");
                            return;
                        }

                    }
                    else {
                        cout << "�D�k��J!" << endl;
                        system("pause");
                        return;
                    }
                }
            }
            else {
                cout << "��Jr��^:";
                string input;
                while (getline(cin, input)) {
                    //cin.ignore();
                    if (input == "r") {
                        BoardState = BOARD;
                        return;
                    }
                    else {
                        cout << "�D�k��J!"<<endl;
                        system("pause");
                        return;
                    }
                }
            }
            
        }

        int addcomment() {
            cout << "��J�d��(���U�^���䵲��),�άO��r��^:";
            string comment;
            getline(cin, comment);
           // cin.ignore();
            if (comment == "r")return boards[currentboard].currentpost;
            boards[currentboard].posts[boards[currentboard].currentpost].comments++;
            Comment n;
            n.accountname = users[currentuser].accountname;
            n.commentID = boards[currentboard].posts[boards[currentboard].currentpost].comments;
            n.content = comment;
            ofstream f(boards[currentboard].Boardname + to_string(boards[currentboard].posts[boards[currentboard].currentpost].PostID) + "comment" + to_string(boards[currentboard].posts[boards[currentboard].currentpost].comments)+ ".txt");
            f <<n.status<<endl<< n.accountname << endl << n.commentID << endl << n.content << endl;
            boards[currentboard].posts[boards[currentboard].currentpost].com.push_back(n);
            f.close();
            ofstream updateboard(boards[currentboard].Boardname + to_string(boards[currentboard].posts[boards[currentboard].currentpost].PostID) + ".txt");
            updateboard << boards[currentboard].posts[boards[currentboard].currentpost].Status << endl << boards[currentboard].posts[boards[currentboard].currentpost].creator << endl << boards[currentboard].posts[boards[currentboard].currentpost].PostID << endl << boards[currentboard].posts[boards[currentboard].currentpost].title << endl << boards[currentboard].posts[boards[currentboard].currentpost].contentline << endl << boards[currentboard].posts[boards[currentboard].currentpost].comments << endl << boards[currentboard].posts[boards[currentboard].currentpost].like << endl << boards[currentboard].posts[boards[currentboard].currentpost].dislike << endl;
            for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].contentline;i++) {
                updateboard << boards[currentboard].posts[boards[currentboard].currentpost].content[i] << endl;
            }
            for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.size();i++) {
                updateboard << boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser[i] << endl;
            }
            updateboard.close();
            cout << "�s�W�d�����\!" << endl;
            system("pause");
            return boards[currentboard].currentpost;
        }

        int deletecomment() {
            cout << "��J�Ӽh�s���R���d��,�άO��Jr��^:";
            string act;
            while (getline(cin, act)) {
                //cin.ignore();
                bool correctact = true;
                if (act == "r")return boards[currentboard].currentpost;
                for (int i = 0;i < act.length();i++) {
                    if (act[i] - '0' < 0 || act[i] - '0' > 9) {
                        correctact = false;
                        break;
                    }
                }
                if (correctact) {
                    if (stoi(act) - 1 < 0 || stoi(act)-1 > boards[currentboard].posts[boards[currentboard].currentpost].com.size() - 1) {
                        correctact = false;
                    }
                }
                if (correctact) {
                    break;
                }
                else {
                    cout << "�D�k��J!" << endl;
                    cout << "��J�Ӽh�s���R���d��,�άO��Jr��^:";
                }
            }
            int i;
            for (i = 0;i < users.size();i++) {
                if (users[i].accountname == boards[currentboard].posts[boards[currentboard].currentpost].creator) {
                    break;
                }
            }
            replymail(i, "�R����"+ boards[currentboard].posts[boards[currentboard].currentpost].title+"���d���q��", DELETECOMMENT);
            boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].status = DELETE;
            ofstream f(boards[currentboard].Boardname + to_string(boards[currentboard].posts[boards[currentboard].currentpost].PostID) + "comment" + to_string(boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act)-1].commentID) + ".txt");
            f << boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].status << endl << boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].accountname << endl << boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].commentID << endl << boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].content << endl;
            f.close();
            cout << "�R�����\!";
            system("pause");
            return boards[currentboard].currentpost;
            
        }

        int editpost() {
            cout << "���D:" << boards[currentboard].posts[boards[currentboard].currentpost].title<<endl<<endl;
            cout << "���e:" << endl;
            for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].content.size();i++) {
                cout << boards[currentboard].posts[boards[currentboard].currentpost].content[i] << endl;
            }
            cout << endl;
            cout << "�п�J���D,�ο�Jr��^:";
            string title;
            cin >> title;
            cin.ignore();
            if (title == "r")return boards[currentboard].currentpost; 
            vector<string> content;
            string coninput;
             cout << "�п�J���e,�άO��r���s��K��,�p�G�s�觹���Ы��U������A��Jfin:" << endl;
            while (getline(cin, coninput)) {
                if (coninput == "r") {
                   return boards[currentboard].currentpost;
                }
                else if (coninput == "fin") {

                    break;
                }
                else {
                    content.push_back(coninput);
                }
            }
            Post newpost;
            newpost.title = title;
            newpost.creator = users[currentuser].accountname;
            newpost.PostID = boards[currentboard].BoardPost;
            newpost.contentline = content.size();
            newpost.content = content;
            newpost.comments = boards[currentboard].posts[boards[currentboard].currentpost].comments;
            ofstream postinput(boards[currentboard].Boardname + to_string(boards[currentboard].posts[boards[currentboard].currentpost].PostID) + ".txt");
            postinput << newpost.Status << endl << newpost.creator << endl << newpost.PostID << endl << newpost.title << endl << newpost.contentline << endl << newpost.comments << endl << newpost.like << endl << newpost.dislike << endl;
            for (int i = 0;i < newpost.content.size();i++) {
                postinput << newpost.content[i] << endl;
            }
            for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.size();i++) {
                postinput << boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser[i] << endl;
            }
            postinput.close();
            return boards[currentboard].currentpost;
        }

        void viewmaillist() {
            cout << "�a�X����>>"<<users[currentuser].accountname<<"���H�c" << endl<<endl;
            for (int i = 0;i < users[currentuser].maillist.size();i++) {
                cout << "(" << i << ")" << users[currentuser].maillist[i].title<<endl<<endl;
            }
            cout << endl;
            viewer.printMailBoard();
            string act;
            getline(cin, act);
            if (act == "r") {
                BoardState = SELECT_BOARD;
                return;
            }
            else if (act == "write") {
                int reciverID;
                string reciver;
                cout << "�п�J���H�̱b��:";
                while (cin >> reciver) {
                    //cin.ignore();
                    bool usersurvive = false;
                    for (int i = 0;i < users.size();i++) {
                        if (reciver == users[i].accountname) {
                            reciverID = i;
                            usersurvive = true;
                            break;
                        }
                    }
                    if (usersurvive) {
                        break;
                    }
                    else {
                        cout << "�d�L���b��!" << endl;
                        system("pause");
                        return;
                    }
                }
                writemail(reciverID);
                return;
            }
            else if (act == "delete") {
                deletemail();
                return;
            }
            else {
                bool correctact = true;
                for (int i = 0;i < act.length();i++) {
                    if (act[i] - '0' < 0 || act[i] - '0' > 9) {
                        correctact = false;
                        break;
                    }
                }
                if (correctact) {
                    if (stoi(act)  < 0 || stoi(act) > users[currentuser].maillist.size() - 1) {
                        correctact = false;
                    }
                }
                if (correctact) {
                    users[currentuser].currentmail = stoi(act);
                    BoardState = MAIL;
                    return;
                }
                else {
                    cout << "�D�k��J!" << endl;
                    system("pause");
                    return;
                }
            }            
        }

        void writemail(int reciver) {           
            string title;
            cout << "�п�J���D,�ο�Jr��^:"<<endl;
            cin >> title;
            cin.ignore();
            if (title == "r")return;
            vector<string> content;
            string coninput;
            //�K�K�����K��ӷh�ڴN�i
            cout << "�п�J���e,�άO��r��󼶼g,�p�G�s�觹���Ы��U������A��Jfin:" << endl;
            while (getline(cin, coninput)) {
                if (coninput == "r") {
                    return;
                }
                else if (coninput == "fin") {
                    break;
                }
                else {
                    content.push_back(coninput);
                }
            }
            users[reciver].mails += 1;
            ofstream updateuser(to_string(reciver+1) + ".txt");
            updateuser << users[reciver].accountname << endl << users[reciver].password << endl << users[reciver].nickname << endl << users[reciver].Permission_level << endl << users[reciver].mails;
            updateuser.close();
            ofstream f(users[reciver].accountname + "mail" + to_string(users[reciver].mails) + ".txt");
            f << UNDELETED <<endl <<users[reciver].mails << endl << title << endl << users[currentuser].accountname << endl << content.size() << endl;
            for (int i = 0;i < content.size();i++) {
                f << content[i] << endl;
            }
            Mail input;
            comment com;
            com.commentline = content.size();
            com.content = content;
            input.com.push_back(com);
            input.mailID = users[reciver].mails;
            input.sender = users[currentuser].accountname;
            input.state = UNDELETED;
            users[reciver].maillist.push_back(input);
            f.close();
            reset();
            load();
            return;
        }

        void replymail(int reciver,string title,int use) {
            vector<string> content;
            string coninput;
            if (use == REPLY) {
                cout << "�п�J���e,�άO��r��󼶼g,�p�G�s�觹���Ы��U������A��Jfin:" << endl;
            }
            else if (use == DELETECOMMENT) {
                cout << "�п�J�R���d����],�p�G��J�����Ы��U������A��Jfin:" << endl;
            }
            else if (use == DELETEPOST) {
                cout << "�п�J�R���K���],�p�G��J�����Ы��U������A��Jfin:" << endl;
            }

            while (getline(cin, coninput)) {
                if (coninput == "r") {
                    return;
                }
                else if (coninput == "fin") {
                    break;
                }
                else {
                    content.push_back(coninput);
                }
            }
            users[reciver].mails += 1;
            ofstream updateuser(to_string(reciver + 1) + ".txt");
            updateuser << users[reciver].accountname << endl << users[reciver].password << endl << users[reciver].nickname << endl << users[reciver].Permission_level << endl << users[reciver].mails;
            updateuser.close();
            ofstream f(users[reciver].accountname + "mail" + to_string(users[reciver].mails) + ".txt");
            f << UNDELETED << endl << users[reciver].mails << endl << title << endl << users[currentuser].accountname << endl << content.size() << endl;
            for (int i = 0;i < content.size();i++) {
                f << content[i] << endl;
            }
            f.close();
            return;
        }

        void deletemail() {
            cout << "��J���R�����l��s��,�άO��r��^:";
            string act;
            while (getline(cin, act)) {
                //cin.ignore();
                bool correctact = true;
                if (act == "r")return;
                for (int i = 0;i < act.size();i++) {
                    if (act[i] - '0' < 0 || act[i] - '0' > 9) {
                        cout << "��J�D�k!" << endl;
                        correctact = false;
                        break;
                    }
                }
                if (correctact) {
                    if (stoi(act) < 0 || stoi(act) >users[currentuser].maillist.size() - 1) {
                        cout << "��J�D�k!" << endl;
                        correctact = false;
                    }
                }
                if (correctact) {
                    break;
                }
                else {
                    cout << "��J���R�����l��s��,�άO��r��^:";
                }
            }
            ofstream del(users[currentuser].accountname + "mail" + to_string(users[currentuser].maillist[stoi(act)].mailID) + ".txt");
            del << DELETE << endl << users[currentuser].maillist[stoi(act)].mailID << endl << users[currentuser].maillist[stoi(act)].title << endl << users[currentuser].maillist[stoi(act)].sender << endl << users[currentuser].maillist[stoi(act)].com[0].commentline << endl;
            for (int i = 0;i < users[currentuser].maillist[stoi(act)].com[0].commentline;i++) {
                del << users[currentuser].maillist[stoi(act)].com[0].content[i]<<endl;
            }
            del.close();
            reset();
            load();
            cout << "�R�����\!" << endl;
            system("pause");
            return;
        }

        void viewmail() {
            cout << "�a�X����>>" << users[currentuser].accountname << "���H�c>>" << users[currentuser].maillist[users[currentuser].currentmail].title<<endl << endl;
            for (int i = 0;i < users.size();i++) {
                if (users[i].accountname == users[currentuser].maillist[users[currentuser].currentmail].sender) {
                    cout << users[i].nickname;
                    break;
                }
            }
            cout << "(" << users[currentuser].maillist[users[currentuser].currentmail].sender << "):" << endl << endl;
            for (int i = 0;i < users[currentuser].maillist[users[currentuser].currentmail].com[0].commentline;i++) {
                cout<<users[currentuser].maillist[users[currentuser].currentmail].com[0].content[i]<<endl;
            }
            cout << endl;       
            viewer.printMail();
            string act;
            getline(cin, act);
            if (act == "r") {
                BoardState = MAILLIST;
                return;
            }
            else if (act == "reply") {
                int i;
                for ( i = 0;i < users.size();i++) {
                    if (users[i].accountname == users[currentuser].maillist[users[currentuser].currentmail].sender,REPLY) {
                        break;
                    }
                }
                replymail(i, "RE:" + users[currentuser].maillist[users[currentuser].currentmail].title,REPLY);
                reset();
                load();
                return;
            }            
        }
};

