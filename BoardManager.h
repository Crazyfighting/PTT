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
                            cout << "請輸入密碼:";
                            while (cin >> inputpassword) {
                                cin.ignore();
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
                    while (getline(cin, input)) {
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
                            if (stoi(input) < 0 || stoi(input) > numberofboard -1) {
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
                    while (getline(cin, input)) {
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
                            if (stoi(input) < 0 || stoi(input) > numberofboard -1) {
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
                while (getline(cin, input)) {
                   // cin.ignore();
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
                        if (stoi(input) < 0 || stoi(input) > boards.size() - 1) {
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
                //cin.ignore();
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
            for (int i = 0;i < boards[currentboard].posts.size();i++) {
                cout << "(" << i << ")" << boards[currentboard].posts[i].title << endl << endl;
            }
            if (currentuser >= 0) {
                if (users[currentuser].Permission_level == USER) {
                    cout << "輸入create建立貼文,";
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
                        }else{
                            for (int i = 0;i < input.size();i++) {
                                if (input[i] - '0' < 0 || input[i] - '0' > 9) {
                                    cout << "輸入非法!" << endl;
                                    system("pause");
                                    return;
                                }
                            }
                            if (stoi(input) < 0 || stoi(input) > boards[currentboard].BoardPost - 1) {
                                cout << "輸入非法!" << endl;
                                system("pause");
                                return;
                            }
                            boards[currentboard].currentpost = stoi(input);
                            BoardState = POST;
                            return;
                        }
                    }
                }
                else if (users[currentuser].Permission_level == ADMINSTRATOR) {
                    cout << "輸入create建立貼文,輸入delete刪除貼文,";
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
                                    cout << "輸入非法!" << endl;
                                    system("pause");
                                    return;
                                }
                            }
                            if (stoi(input) < 0 || stoi(input) > boards[currentboard].BoardPost - 1) {
                                cout << "輸入非法!" << endl;
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
                                cout << "輸入非法!" << endl;
                                system("pause");
                                return;
                            }
                        }
                        
                        if (stoi(input) < 0 || stoi(input) > boards[currentboard].BoardPost - 1) {
                            cout << "輸入非法!" << endl;
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
            cout << "輸入標題,或是按r放棄編輯貼文:";
            string  title;
            while (cin >> title) {
                if (title == "r")return ;
                else {
                    break;
                }
            }
            cout << "請輸入內容,或是按r放棄編輯貼文,如果編輯完成請按下換行鍵再輸入fin:" <<endl;
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
            cout << "建立貼文成功！" << endl;
            system("pause");
        }

        void deletepost() {
            cout << "輸入欲刪除之貼文編號,或是按r返回:";
            string act;
            while (getline(cin, act)) {
                //cin.ignore();
                bool correctact=true;
                if (act == "r")return;
                for (int i = 0;i < act.size();i++) {
                    if (act[i] - '0' < 0 || act[i] - '0' > 9) {
                        cout << "輸入非法!" << endl;
                        correctact = false;
                        break;
                    }
                }
                if (correctact) {
                    if (stoi(act) < 0 || stoi(act) > boards[currentboard].posts.size() - 1) {
                        cout << "輸入非法!" << endl;
                        correctact = false;
                    }
                }              
                if (correctact) {
                    break;
                }
                else {
                    cout << "輸入欲刪除之貼文編號,或是按r返回:";
                }
            }
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
            cout << "刪除成功!" << endl;
            system("pause");
            return;
        }

        void viewpost() {
            cout << "吧合母待>>" << boards[currentboard].Boardname <<">>"<< boards[currentboard].posts[boards[currentboard].currentpost].title << endl << endl;
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
            cout <<"讚:"<< boards[currentboard].posts[boards[currentboard].currentpost].like <<"    噓:"<< boards[currentboard].posts[boards[currentboard].currentpost].dislike<<endl<<endl;
            for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].com.size();i++) {
                cout << i + 1 << "樓>>";
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
                if (users[currentuser].Permission_level == ADMINSTRATOR)cout << "輸入delete刪除留言,";
                cout << "輸入comment留言,輸入like或dislike按讚或噓,";
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
                            cout << "你不能對自己的貼文按讚或噓" << endl;
                            system("pause");
                            return;
                        }
                        for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.size();i++) {
                            if (boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser[i] == users[currentuser].accountname) {
                                cout << "你已對此貼文按過讚或噓!" << endl;
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
                        cout << "已對此貼文按讚!" << endl;
                        system("pause");
                        return;
                    }
                    else if (input == "dislike") {
                        int temp = boards[currentboard].currentpost;
                        if (boards[currentboard].posts[boards[currentboard].currentpost].creator == users[currentuser].accountname) {
                            cout << "你不能對自己的貼文按讚或噓" << endl;
                            system("pause");
                            return;
                        }
                        for (int i = 0;i < boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser.size();i++) {
                            if (boards[currentboard].posts[boards[currentboard].currentpost].likeordislikeuser[i] == users[currentuser].accountname) {
                                cout << "你已對此貼文按過讚或噓!" << endl;
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
                        cout << "已對此貼文按噓!" << endl;
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
                        else {
                            cout << "非法輸入!" << endl;
                            system("pause");
                            return;
                        }                      
                    }
                    else {
                        cout << "非法輸入!" << endl;
                        system("pause");
                        return;
                    }
                }
            }
            else {
                cout << "輸入r返回:";
                string input;
                while (getline(cin, input)) {
                    //cin.ignore();
                    if (input == "r") {
                        BoardState = BOARD;
                        return;
                    }
                    else {
                        cout << "非法輸入!"<<endl;
                        system("pause");
                        return;
                    }
                }
            }
            
        }

        int addcomment() {
            cout << "輸入留言(按下回車鍵結束),或是按r返回:";
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
            cout << "新增留言成功!" << endl;
            system("pause");
            return boards[currentboard].currentpost;
        }

        int deletecomment() {
            cout << "輸入樓層編號刪除留言,或是輸入r返回:";
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
                    cout << "非法輸入!" << endl;
                    cout << "輸入樓層編號刪除留言,或是輸入r返回:";
                }
            }
            boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].status = DELETE;
            ofstream f(boards[currentboard].Boardname + to_string(boards[currentboard].posts[boards[currentboard].currentpost].PostID) + "comment" + to_string(boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act)-1].commentID) + ".txt");
            f << boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].status << endl << boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].accountname << endl << boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].commentID << endl << boards[currentboard].posts[boards[currentboard].currentpost].com[stoi(act) - 1].content << endl;
            f.close();
            cout << "刪除成功!";
            system("pause");
            return boards[currentboard].currentpost;
            
        }
};
