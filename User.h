#pragma once
#include<vector>
#include <string>
enum nowuser {
	USER,
	ADMINSTRATOR,
};
class User {
	public:
		int Permission_level;
		std::vector<int> postsID;
		std::string nickname;
		std::string accountname;
		std::string password;
		User() {
			Permission_level = USER;
		}

};