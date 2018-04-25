#ifndef THREADPCAP_H
#define THREADPCAP_H

#include <QThread>
#include "state.h"

class ThreadPcap:public QThread
{
    Q_OBJECT
public:
    char *str;
    ThreadPcap();
    pcap_if_t *dev_if_t,*alldevs;
    void init(pcap_if_t *d, pcap_if_t *inalldevs);
    void sendSignal(Infor *infor);
    static void byteToMac(char *str, uchar *byte);
    static void byteToIP(char *str,uchar *byte);

signals:
   void packet_receive(Infor *infor);

private:
   struct bpf_program fcode;
    pcap_t * cap_ins_des;
    char errbuf[PCAP_ERRBUF_SIZE];
    virtual void run();
    static void packet_handler(u_char *user, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data);
   static void arpReceive(Infor *infor,uchar *data);
    static void ipReceive(Infor *infor,uchar *data);
    static void tcpReceive(Infor *infor,uchar *data);
    static void udpReceive(Infor *infor,uchar *data);
    static void icmpReceive(Infor *infor,uchar *data);
};

#endif // THREADPCAP_H
