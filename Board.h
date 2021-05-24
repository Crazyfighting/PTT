#pragma once
#include<string>
#include<vector>
#include<string>
#include<fstream>
#include"Post.h"
using namespace std;
enum BoardStatus {
	AVAILABLE,
	DISABLE
};
class Board {
	public:
		std::string Boardname;
		std::vector<Post> posts;
		int BoardPost=0;
		int BoardID;
		int Status= AVAILABLE;
		int currentpost;
		void loadpost() {
			for (int i = 1;i <= BoardPost;i++) {
				ifstream f(Boardname + to_string(i)+".txt");
				Post input;
				f >> input.Status >>input.creator >>input.PostID >> input.title >> input.contentline >> input.comments>>input.like>>input.dislike;
				for (int i = 0;i < input.contentline;i++) {
					string inp;
					f >> inp;
					input.content.push_back(inp);
				}
				for (int i = 0;i < input.like + input.dislike;i++) {
					string luser;
					f >> luser;
					input.likeordislikeuser.push_back(luser);
				}
				for (int i = 1;i <= input.comments;i++) {
					ifstream inputcomment(Boardname + to_string(input.PostID) + "comment"+ to_string(i)+".txt");
					Comment inputcom;
					inputcomment >> inputcom.status >> inputcom.accountname >> inputcom.commentID >> inputcom.content;
					inputcomment.close();					
					if (inputcom.status == SURVIVE) {
						input.com.push_back(inputcom);
					}
				}
				f.close();
				if(input.Status==SURVIVE)
				posts.push_back(input);
			}
		}
};