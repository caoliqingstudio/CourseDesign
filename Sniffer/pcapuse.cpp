#include "pcapuse.h"

PcapUse::PcapUse()
{
    thread = new ThreadPcap();
    if (pcap_findalldevs_ex((char *)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        cout<<"Error in pcap_findalldevs_ex: "<< errbuf<<endl;
        return;
    }
    int i=0;
    /* Print the list */
    for(pcap_if_t * d=alldevs; d; d=d->next)
    {
        //cout<<++i<< d->name;
        QVector<QString> item;
        item.append(QString::number(++i));
        item.append(d->name);
        if (d->description){
            item.append(d->description);
            //cout<<( d->description)<<endl;
        }else{
            item.append(" (No description available)");
        }
        devs.append(item);
    }

    if (i==0)
    {
        cout<<("No interfaces found! Exiting.\n");
    }
    indexDev=alldevs;
}

QVector<QVector<QString>>* PcapUse::getDevs(){
    return &devs;
}

void PcapUse::chooseDev(int i){
    pcap_if_t *d;
    d=alldevs;
    while(i-- > 0){
        d=d->next;
    }
    indexDev=new pcap_if_t();
    *indexDev = *d;
    thread->init(indexDev,alldevs);
}


PcapUse::~PcapUse(){
    if(!thread->isFinished()){
        thread->terminate();
        thread->quit();
    }
}

void PcapUse::stopThread(){
    if(!thread->isFinished()){
        thread->terminate();
        thread->quit();
    }
}

void PcapUse::setFilterAndStart(char *str){
    pcap_if_t *x,*y;
    x=thread->alldevs;
    y=thread->dev_if_t;
    if(!thread->isFinished()){
        thread->terminate();
        thread->quit();
    }
    delete thread;

    //cout<<"pcapUse ::setFiletas :: "<<y->name<<endl;
    thread = new ThreadPcap();
    thread->str=str;
    thread->init(y,x);
    thread->start();
}

