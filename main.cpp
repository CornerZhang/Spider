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
string head="zhidao.baidu.com";

void download(Downloader *dl, Spider *sp)
{
    while(true){
	while(sp->empty()){
	    cout << "downloader sleep" << endl;
	    rcond.wait_for(rlock,chrono::seconds(10));
	    if(sp->empty()){
	      cout << sp->size() << endl;
	      cout << "downloader exit" << endl;
//	      return;
	    }
	}
	string url = head + sp->getUrl();
	dl->setUrl(url);
	dl->downloadPage();
	wcond.notify_all();
    }
}

void spider(Spider *sp, Downloader *dl)
{
    while(true){
	while(dl->empty()){
	    wcond.wait_for(wlock,chrono::seconds(10));
	    if(dl->empty()){
	      cout << "spider exit" << endl;
//	      return;
	    }
	}
    	sp->loadPage(dl->getContext());
    	sp->search_for_link("");
    	rcond.notify_all();
    }
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
