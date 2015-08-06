#ifndef SPIDER_H
#define SPIDER_H

#include <regex>
#include "gumbo.h"
#include "common.h"

using std::regex;

class Spider
{
    private:
	GumboOutput *output;
	GumboNode *root;
	string page;
	regex re;
	mutex wmtx;
	stack<string> request;
	void one_search(GumboNode*);

    public:
	Spider(string);
	void loadPage(string);
	void search_for_link(string);
	bool empty();
	int size();
	string getUrl();
	GumboNode* getRootNode();
	~Spider();
};
#endif
