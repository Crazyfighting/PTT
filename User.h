#pragma once
#include<vector>
#include <string>
#include"Mail.h"
#include<fstream>
enum nowuser {
	USER,
	ADMINSTRATOR,
};
class User {
	public:
		int Permission_level;
		int mails;
		int currentmail;
		std::vector<int> postsID;
		std::string nickname;
		std::string accountname;
		std::string password;
		std::vector<Mail> maillist;
		User() {
			Permission_level = USER;
		}
		void mailload() {
			for (int i = 1;i <= mails;i++) {
				ifstream mailinput(accountname + "mail" + to_string(i) + ".txt");
				//cout << accountname + "mail" + to_string(i) + ".txt";
				Mail input;
				comment inpcom;
				mailinput >> input.state >>input.mailID>> input.title >> input.sender>>inpcom.commentline;
				for (int j = 0;j < inpcom.commentline;j++) {
					string a;
					mailinput >> a;
					inpcom.content.push_back(a);
				}
				input.com.push_back(inpcom);
				if (input.state == UNDELETED)maillist.push_back(input);
				mailinput.close();
			}
		}
};