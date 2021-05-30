#pragma once
#include<iostream>
class Viewer {
	public:
		void printMenu(){
			std::cout << "歡迎來到吧合母待,請輸入帳號,或是輸入guest用遊客模式進入,輸入new創建新帳號,輸入r退出:";
		}
		void printchooseBoard() {
			std::cout << "輸入編號進入指定看板,輸入r返回:";
		}
		void printviewBoard() {
			std::cout << "輸入編號進入指定貼文,輸入r返回:";
		}
		void printPost() {
			std::cout << "輸入r返回:";
		}
		void printMailBoard() {
			std::cout << "輸入編號進入指定郵件,輸入write撰寫郵件,輸入delete刪除郵件,輸入r返回:";
		}

		void printMail() {
			std::cout << "輸入reply回覆郵件,標題為(RE:回覆郵件的標題),輸入r返回:";
		}
};