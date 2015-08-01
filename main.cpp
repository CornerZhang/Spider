#include "downloader.h"
#include "spider.h"
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

using namespace std;

mutex rmtx,wmtx;
condition_variable rcond,wcond;
unique_lock<mutex> rlock,wlock;
string head="zhidao.baidu.com";

void download(Downloader *dl, Spider *sp)
{
    
    while(!sp->empty() || !dl->empty()){
	sleep(1);
	while(sp->empty())
	    rcond.wait(rlock);

	string url = head + sp->getUrl();

	dl->setUrl(url);
	dl->downloadPage();
	wcond.notify_all();
    }
}

void spider(Spider *sp, Downloader *dl)
{
    while(!sp->empty() || !dl->empty()){
	while(dl->empty())
    	    wcond.wait(rlock);

    	sp->loadPage(dl->getContext());
	cout << "in" << endl;
    	sp->search_for_link("");
	cout << "out" << endl;
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




