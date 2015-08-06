#include "spider.h"

Spider::Spider(string url)
{
    output = gumbo_parse("");
    request.push(url);
}

Spider::~Spider()
{
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

void
Spider::loadPage(string p)
{
    page = p;
    output = gumbo_parse_with_options(&kGumboDefaultOptions,page.c_str(),page.size());
    root = output->root;
}

void
Spider::search_for_link(string strict)
{
    re.assign(strict);
    one_search(root);
    page.clear();
}

void
Spider::one_search(GumboNode *node)
{
    if(node->type != GUMBO_NODE_ELEMENT)return;

    GumboAttribute *rel;
    if(node->v.element.tag == GUMBO_TAG_A && (rel=gumbo_get_attribute(&node->v.element.attributes,"class"))){
	if(string(rel->value) == "pager-next"){
	    rel = gumbo_get_attribute(&node->v.element.attributes,"href");
	    if(wmtx.try_lock()){
		cout << string(rel->value) << endl;
		request.push(string(rel->value));
		cout << request.size() << endl;
		wmtx.unlock();
	    }
	    return;
	}
    }

    GumboVector *children = &node->v.element.children;

    for(unsigned int i = 0; i< children->length; ++i){
	one_search(static_cast<GumboNode*>(children->data[i]));
    }
}

GumboNode*
Spider::getRootNode()
{
    return root;
}

string
Spider::getUrl()
{
    string tmp;
    if(wmtx.try_lock()){
	tmp = request.top();
	request.pop();
	wmtx.unlock();
    }
    return tmp;
}

bool
Spider::empty()
{
    bool res;
    if(wmtx.try_lock()){
	res = request.empty();
	wmtx.unlock();
    }
    return res;
}

int
Spider::size()
{
    return request.size();
}











