#pragma once
#include<iostream>
class Viewer {
	public:
		void printMenu(){
			std::cout << "�w��Ө�a�X����,�п�J�b��,�άO��Jguest�ιC�ȼҦ��i�J,��Jnew�Ыطs�b��,��Jr�h�X:";
		}
		void printchooseBoard() {
			std::cout << "��J�s���i�J���w�ݪO,��Jr��^:";
		}
		void printviewBoard() {
			std::cout << "��J�s���i�J���w�K��,��Jr��^:";
		}
		void printPost() {
			std::cout << "��Jr��^:";
		}
		void printMailBoard() {
			std::cout << "��J�s���i�J���w�l��,��Jwrite���g�l��,��Jdelete�R���l��,��Jr��^:";
		}

		void printMail() {
			std::cout << "��Jreply�^�жl��,���D��(RE:�^�жl�󪺼��D),��Jr��^:";
		}
};