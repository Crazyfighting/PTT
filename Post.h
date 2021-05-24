#pragma once
#include<vector>
#include<string>
enum status {
	SURVIVE,
	DELETE
};
struct Comment {
	int commentID;
	std::string accountname;
	std::string content;
	int status=SURVIVE;
};
class Post {
	public:
		int PostID;
		std::string creator;
		int Status = SURVIVE;
		int contentline=0;
		int comments = 0;
		int like=0;
		int dislike=0;
		std::vector<std::string> likeordislikeuser;
		std::string title;
		std::vector<std::string> content;
		std::vector<Comment> com;
};
