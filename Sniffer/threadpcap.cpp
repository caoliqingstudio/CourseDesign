#include "threadpcap.h"

void*x=0;

ThreadPcap::ThreadPcap()
{
   x=this;
 //   threadStatic=this;
   // staticThread = this;
}

void ThreadPcap::run(){
    if (pcap_findalldevs_ex((char *)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        cout<<"Error in pcap_findalldevs_ex: "<< errbuf<<endl;
        return;
    }
    cout<<dev_if_t->name<<"  "<<str<<endl;
    cap_ins_des = pcap_open(dev_if_t->name /* char *source */, 65535, PCAP_OPENFLAG_PROMISCUOUS, 0, NULL, errbuf);
    if (cap_ins_des == NULL) {
        cout<<("%s\n", errbuf)<<endl;
        exit(-1);
    }

    /* free all the devices, because we don't use them any more */
    pcap_freealldevs(alldevs);

    //过滤
    u_int netmask; //定义子网掩码
   pcap_if_t * d=dev_if_t;
   if (d->addresses != NULL){
       /* 获取接口第一个地址的掩码 */
       netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
   }else{
       /* 如果这个接口没有地址，那么我们假设这个接口在C类网络中 */
       netmask=0xffffff;
   }
   if (pcap_compile(cap_ins_des, &(fcode),str, 1, netmask) < 0)
   {
       qDebug()<<("\nUnable to compile the packet filter. Check the syntax.\n");
       /* 释放设备列表 */
       pcap_freealldevs(alldevs);
       return;
   }

   if (pcap_setfilter(cap_ins_des, &(fcode)) < 0)
   {
       qDebug()<<("\nError setting the filter.\n");
       /* 释放设备列表 */
       pcap_freealldevs(alldevs);
       return;
   }
    cout<<"run start loop"<<endl;
    /* start the capture, deal with one packet each loop */
    pcap_loop(cap_ins_des, 0 , &(ThreadPcap::packet_handler), NULL);
    //emit packet_receive(0);
}

void ThreadPcap::init(pcap_if_t *d,pcap_if_t *inalldevs){
    dev_if_t=d;
    alldevs=inalldevs;
}

void ThreadPcap::packet_handler(u_char *user, const pcap_pkthdr *pkt_header, const u_char *pkt_data){
    time_t time = pkt_header->ts.tv_sec;
    struct tm *ltime = localtime(&time);

    Infor *infor=new Infor();
    memset(infor,0,sizeof(Infor));
    (VOID)user;
    //(VOID)pkt_data;

    strftime(&(infor->time[0]), 16, "%H:%M:%S", ltime);
    u_char * data=*((u_char **)(&pkt_data));

    //cout<<"infor= "<< timestr<<" "<<pkt_header->caplen<<" "<<" "<<pkt_header->ts.tv_usec<<" "<< pkt_header->caplen<<" "<<pkt_header->len<<endl;
    //以太网
    enthernet * ent = (enthernet *)data;
    data += sizeof(enthernet);

    byteToMac(infor->smac,ent->s_mac);
    byteToMac(infor->dmac,ent->d_mac);

    infor->length=pkt_header->len;
    infor->date = (unsigned char *)malloc(infor->length);
    memcpy(infor->date,pkt_data,infor->length);
    switch (ent->type) {
    case TYPE_ARP:
        strcpy(infor->fram,"arp");
        arpReceive(infor,data);
        //cout<<"type arp: %x "<<ent->type<<infor->fram<<endl;
        break;
    case TYPE_IP:
        strcpy(infor->fram,"ip");
        ipReceive(infor,data);
        //cout<<"type ip: %x "<<ent->type<<infor->fram<<endl;
        break;
    case TYPE_RARP:
        strcpy(infor->fram,"rarp");
        //cout<<"type rarp: %x "<<ent->type<<infor->fram<<endl;
        break;
    default:
        //cout<<"type other: %x"<<ent->type<<endl;
        strcpy(infor->fram,"other");
        break;
    }
    ((ThreadPcap*)x)->sendSignal(infor);
   //cout<<"receive: "<<infor->sip<<" "<<infor->smac<<" "<<infor->dip<<infor->dmac<<infor->fram<<infor->transform<<infor->length<<endl;
    //emit packet_receive(infor);
}


void ThreadPcap::byteToMac(char *str, uchar *byte){
    char hex[17]="0123456789ABCDEF";
    str[0]=hex[(byte[0]>>4)&0xf];
    str[1]=hex[(byte[0])&0xf];
    str[2]=':';
    str[3]=hex[(byte[1]>>4)&0xf];
    str[4]=hex[(byte[1])&0xf];
    str[5]=':';
    str[6]=hex[(byte[2]>>4)&0xf];
    str[7]=hex[(byte[2])&0xf];
    str[8]=':';
    str[9]=hex[(byte[3]>>4)&0xf];
    str[10]=hex[(byte[3])&0xf];
    str[11]=':';
    str[12]=hex[(byte[4]>>4)&0xf];
    str[13]=hex[(byte[4])&0xf];
    str[14]=':';
    str[15]=hex[(byte[5]>>4)&0xf];
    str[16]=hex[(byte[5])&0xf];
    str[17]='\0';
}

void ThreadPcap::byteToIP(char *str, uchar *byte){
    sprintf(str,"%d.%d.%d.%d",(int )byte[0],(int)byte[1],(int)byte[2],(int)byte[3]);
}



void ThreadPcap::arpReceive(Infor *infor, uchar *data){
    arp_header * arp=(arp_header*)data;
    data+=sizeof(arp_header);


}

void ThreadPcap::ipReceive(Infor *infor, uchar *data){

    ip_header * ip = (ip_header*)data;
    data += sizeof(ip_header);

//    byteToIP(infor->sip,ntohl(*(u_long*)(ip->saddr)));
//    byteToIP(infor->dip,ntohl(*(u_long*)(ip->daddr)));//不知道顺序变了对不对啊

    byteToIP(infor->sip,ip->saddr);
    byteToIP(infor->dip,ip->daddr);//不知道顺序变了对不对啊

    switch (ip->proto) {
    case TYPE_TCP:
        tcpReceive(infor,data);
        break;
    case TYPE_UDP:
        udpReceive(infor,data);
        break;
    case TYPE_ICMP:
        icmpReceive(infor,data);
        break;
    default:
        break;
    }
}

void ThreadPcap::tcpReceive(Infor *infor, uchar *data){
    strcpy(infor->transform,"tcp");
}

void ThreadPcap::udpReceive(Infor *infor, uchar *data){
    strcpy(infor->transform,"udp");

}

void ThreadPcap::icmpReceive(Infor *infor, uchar *data){
    strcpy(infor->transform,"icmp");
}


void ThreadPcap::sendSignal(Infor *infor){
    //cout<<"receive sendSignal"<<infor->sip<<infor->dip;
    emit packet_receive(infor);
}
