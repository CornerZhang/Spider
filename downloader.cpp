#include "downloader.h"

Downloader::Downloader()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    conn = curl_easy_init();
    assert(conn!=NULL);

    code = curl_easy_setopt(conn,CURLOPT_FOLLOWLOCATION,1L);
    code = curl_easy_setopt(conn,CURLOPT_WRITEFUNCTION,writer);
    code = curl_easy_setopt(conn,CURLOPT_WRITEDATA,&context);
    assert(code==CURLE_OK);
}

Downloader::~Downloader()
{
    curl_easy_cleanup(conn);
}

void
Downloader::setUrl(string u)
{
    url = u;
    code = curl_easy_setopt(conn,CURLOPT_URL,url.c_str());
    assert(code==CURLE_OK);
}

void
Downloader::downloadPage(){
    assert(url.c_str()!=NULL);
    code = curl_easy_perform(conn);
    assert(code==CURLE_OK);
    if(wmtx.try_lock()){
	response.push(context);
	wmtx.unlock();
    }
    url.clear(); 
    context.clear();
}

string
Downloader::getContext()
{
    string tmp;
    if(wmtx.try_lock()){
	tmp=response.top();
	response.pop();
	wmtx.unlock();
    }

    return tmp;	
}

bool
Downloader::empty()
{
    bool res;
    if(wmtx.try_lock()){
	res = response.empty();
	wmtx.unlock();
    }
    return res;
}

