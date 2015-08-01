#include "downloader.h"
#include "spider.h"
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <chrono>

using namespace std;

mutex rmtx,wmtx;
condition_variable rcond,wcond;
unique_lock<mutex> rlock,wlock;
bool finished = false;
string head="zhidao.baidu.com";

void download(Downloader *dl, Spider *sp)
{
    
    while(!sp->empty() || !dl->empty()){

	while(sp->empty()){
	    rcond.wait_for(rlock,chrono::seconds(5));
	    if(finished)return;
	}
	string url = head + sp->getUrl();

	dl->setUrl(url);
	cout << "downlowd" << endl;
	dl->downloadPage();
	cout << "finish" << endl;
	wcond.notify_all();

    }
    cout << "downloader exit" << endl;
    finished = true;
    
}

void spider(Spider *sp, Downloader *dl)
{
    while(!sp->empty() || !dl->empty()){

	while(dl->empty()){
	    wcond.wait_for(wlock,chrono::seconds(5));
	    if(finished)return;
	}


    	sp->loadPage(dl->getContext());
	cout << "search" << endl;
    	sp->search_for_link("");
	cout << "out" << endl;
    	rcond.notify_all();
    }
    cout << "spider exit" << endl;
    finished = true;
    
}

int 
main(int argv,char *argc[])
{
    rlock = unique_lock<mutex>(rmtx);
    wlock = unique_lock<mutex>(wmtx);
    Spider *sp = new Spider("/browse/80");
    Downloader *dl = new Downloader();
    thread t1(download,dl,sp);
    thread t2(spider,sp,dl);
    t1.join();
    t2.join();
    delete dl;
    delete sp;
}




