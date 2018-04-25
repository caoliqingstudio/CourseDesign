#ifndef PCAPUSE_H
#define PCAPUSE_H

#include "state.h"
#include "threadpcap.h"

#define LINE_LEN 16

class PcapUse
{
public:
    QVector<QVector<QString>> devs;
    PcapUse();
    ~PcapUse();
    QVector<QVector<QString>>* PcapUse::getDevs();

    ThreadPcap *thread;
    void stopThread();
    void openDev(int i);//i start from 0;
    pcap_if_t * indexDev;
    void chooseDev(int i);
    void setFilterAndStart(char *str);
private:
    pcap_if_t *alldevs;
    char errbuf[PCAP_ERRBUF_SIZE];

    Infor arpPackdege(Infor *infor);

};

#endif // PCAPUSE_H
