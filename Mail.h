#pragma once
#include<iostream>
#include<vector>
using namespace std;
enum state {
	UNDELETED,
	DELETED
};
struct comment
{
	int commentline;
	vector<string> content;
};
class Mail {
	public:
		int state;
		int mailID;
		string title;
		string sender;
		vector<comment> com;
};