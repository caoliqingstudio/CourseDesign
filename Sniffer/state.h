#ifndef STATE_H
#define STATE_H

#define _CRT_SECURE_NO_WARNINGS
#define HAVE_REMOTE
#include "Include/pcap.h"
#include <iostream>
#include <QDebug>
#include <QVector>
using namespace std;

//网络序和主机序的区别
#define TYPE_IP 0x0008
#define TYPE_ARP 0x0608
#define TYPE_RARP 0x3580

#define TYPE_UDP 17
#define TYPE_TCP 6
#define TYPE_ICMP 1

#define TYPE_DNS 53
#define TYPE_HTTP 80

//以太网 14字节
typedef struct enthernet{
    u_char d_mac[6];
    u_char s_mac[6];
    qint16 type;
}enthernet;

/* 4字节的IP地址 */
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* IPv4 首部20*/
typedef struct ip_header{
    u_char  ver_ihl;        // 版本 (4 bits) + 首部长度 (4 bits)
    u_char  tos;            // 服务类型(Type of service)
    u_short tlen;           // 总长(Total length)
    u_short identification; // 标识(Identification)
    u_short flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    u_char  ttl;            // 存活时间(Time to live)
    u_char  proto;          // 协议(Protocol)
    u_short crc;            // 首部校验和(Header checksum)
    u_char  saddr[4];      // 源地址(Source address)
    u_char  daddr[4];      // 目的地址(Destination address)
    u_int   op_pad;         // 选项与填充(Option + Padding)
}ip_header;

/* UDP 首部 8字节*/
typedef struct udp_header{
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_short len;            // UDP数据包长度(Datagram length)
    u_short crc;            // 校验和(Checksum)
}udp_header;

//ICMP头部，总长度4字节
typedef struct _icmp_hdr
{
unsigned char icmp_type;    //类型
unsigned char code;     //代码
unsigned short chk_sum;    //16位检验和
}icmp_header;

//TCP头部，总长度20字节
typedef struct _tcp_hdr
{
unsigned short src_port;    //源端口号
unsigned short dst_port;    //目的端口号
unsigned int seq_no;     //序列号
unsigned int ack_no;     //确认号
#ifndef LITTLE_ENDIAN
unsigned char reserved_1:4; //保留6位中的4位首部长度
unsigned char thl:4;     //tcp头部长度
unsigned char flag:6;     //6位标志
unsigned char reseverd_2:2; //保留6位中的2位
#else
unsigned char thl:4;     //tcp头部长度
unsigned char reserved_1:4; //保留6位中的4位首部长度
unsigned char reseverd_2:2; //保留6位中的2位
unsigned char flag:6;     //6位标志
#endif
unsigned short wnd_size;    //16位窗口大小
unsigned short chk_sum;    //16位TCP检验和
unsigned short urgt_p;     //16为紧急指针
}tcp_header;

//arp
typedef struct arphdr
{
    u_short ar_hrd;
    u_short ar_pro;
    u_char ar_hln;
    u_char ar_pln;
    u_short ar_op;
}arp_header;

#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20

/*********************************************/
//=============================================
/*********************************************/
/*ARP与生成的报头*/
typedef struct ether_arp
{
    arp_header ea_hdr;
    u_char arp_sha[6];
    u_char arp_spa[4];
    u_char arp_tha[6];
    u_char arp_tpa[4];
}eth_arp;

typedef struct Infor{
    Infor(){}
    //    Infor(char * intime,char *insip,char * insmac,char *indip,char*indamc,char *infram,char * intransform,quint32 inlength,unsigned char *indate){
//        strcpy(time,intime);
//        strcpy(sip,insip);
//        strcpy(smac,insmac);
//        strcpy(dip,indip);
//        strcpy(dmac,indmac);
//        strcpy(fram,infram);
//        strcpy(transform,intransform);
//        length = inlength;
//        date =indate;
//    }
    char time[16];
    char sip[16];
    char smac[18];
    char dip[16];
    char dmac[18];
    char fram[5];
    char transform[5];
    quint32 length;
    unsigned char *date;
}Infor;

typedef struct FileInfor{
    Infor infor;
    unsigned char data[1900];
}FileInfor;

typedef struct dns_header{
    unsigned short tran;
    unsigned short flags;
    unsigned short qn;
    unsigned short rr;
    unsigned short aurr;
    unsigned short adrr;
}dns_header;


#define MAC_LEN				6		// MAC 地址, 128 bits = 6 bytes
#define IPV4_LEN			4		// IPV4 地址, 32 bits = 4 bytes
#define PADDING_LEN			18		// ARP 数据包的有效载荷长度

#define TABLE_COUNT 10000
// ARP 包
typedef struct arp_packet {
    enthernet eh;				// 以太网首部
    ether_arp ah;					// ARP 首部
    u_char padding[PADDING_LEN];
} arp_packet;

#define ETHPROTOCAL_IPV4		0x0800 // 以太网上层协议类型: IPv4
#define ETHPROTOCAL_ARP			0x0806 // 以太网上层协议类型: ARP

#define HARD_ETHERNET		0x0001
#define ARP_REQUEST				0x0001 // ARP 请求
#define ARP_RESPONCE			0x0002 // ARP 应答

#define ARP_OP_ALL 1
#define ARP_OP_IP 2

#endif // STATE_H
