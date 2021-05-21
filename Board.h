#pragma once
#include<string>
enum BoardStatus {
	AVAILABLE,
	DISABLE
};
class Board {
	public:
		std::string Boardname;
		int BoardPost=0;
		int BoardID;
		int Status= AVAILABLE;
};