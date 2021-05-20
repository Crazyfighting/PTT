#pragma once
#include<iostream>
class Viewer {
	public:
		void printMenu(){
			std::cout << "歡迎來到吧合母待,請輸入帳號,或是輸入guest用遊客模式進入,輸入new創建新帳號,輸入r退出:";
		}
		void printchooseBoard() {
			std::cout << "輸入編號進入指定看板,輸入r退回:";
		}
};