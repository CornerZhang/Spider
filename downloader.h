#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <curl/curl.h>
#include <assert.h>
#include "common.h"

class Downloader{
    private:
	string context;
	string url;
	stack<string> response;
	mutex wmtx;
	CURL *conn;
	CURLcode code;
     	static int writer(char *data, size_t size, size_t nmemb, string* writerData){
	    assert(writerData!=NULL);
	    writerData->append(data, size*nmemb);
	    return size * nmemb;
	}

    public:
	Downloader();
	~Downloader();
	void setUrl(string url);
	bool empty();
	void downloadPage();
	string getContext();
};
#endif
