#include <regex>
#include "gumbo.h"
#include "common.h"

using std::regex;

class Spider
{
    private:
	GumboOutput *output;
	GumboNode *root;
	regex re;
	mutex wmtx;
	stack<string> request;
	void one_search(GumboNode*);

    public:
	Spider(string);
	void loadPage(string);
	void search_for_link(string);
	bool empty();
	string getUrl();
	GumboNode* getRootNode();
	~Spider();
};
