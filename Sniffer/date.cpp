#include "date.h"
#include "ui_date.h"

Date::Date(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Date)
{
    ui->setupUi(this);
    filter=new Filter();
    file =0;
}

Date::~Date()
{
    delete filter;
    delete ui;
}

void Date::init(PcapUse *inpcap, int row){
    table_i=0;

    pcap=inpcap;
    this->start(row);
    //pcap->setFilter(filter->getString());
    ui->label_2->setText(pcap->indexDev->name);//网卡名
    connect(filter,SIGNAL(filterChanged(char*)),this,SLOT(filterChangeTable(char*)));

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setRowCount(TABLE_COUNT);
    /*设置表格是否充满，即行末不留空*/
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //ui->tableWidget->resizeColumnsToContents();
    //ui->tableWidget->resizeRowsToContents();
    QStringList header;
    header<<"Time"<<"S IP"<<"S MAC"<<"D IP"<<"D MAC"<<"Frame"<<"Transport"<<"length";   //表头
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(header);

}


//过滤器
void Date::on_pushButton_clicked()
{
    filter->show();
}

//刷新table
void Date::filterChangeTable(char *str){
    cout<<str<<"     ok \n"<<endl;
    ui->tableWidget->clear();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setRowCount(TABLE_COUNT);
    /*设置表格是否充满，即行末不留空*/
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //ui->tableWidget->resizeColumnsToContents();
    //ui->tableWidget->resizeRowsToContents();
    QStringList header;
    header<<"Time"<<"S IP"<<"S MAC"<<"D IP"<<"D MAC"<<"Frame"<<"Transport"<<"length";   //表头
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(header);

    pcap->stopThread();//结束多线程
    disconnect((pcap->thread),SIGNAL(packet_receive(Infor*)),this,SLOT(packetReceive(Infor*)));

    table_i =0;
    for(int i=0;i<inforList.count();i++){
        free(inforList.at(i)->date);
        delete inforList.at(i);
    }
    inforList.clear();
    closeDataFile();
    countToZero();
    openDataFile();
    pcap->setFilterAndStart(str);
    connect((pcap->thread),SIGNAL(packet_receive(Infor*)),this,SLOT(packetReceive(Infor*)));
}

void Date::start(int index){
    //运行多线程
    pcap->chooseDev(index);
    cout<<"Date::start :"<<filter->getString()<<endl;
    //closeDataFile();
    openDataFile();
    countToZero();
    pcap->setFilterAndStart(filter->getString());

    //openDataFile();
    connect((pcap->thread),SIGNAL(packet_receive(Infor*)),this,SLOT(packetReceive(Infor*)));
}

void Date::packetReceive(Infor *infor){

    //cout<<"receive packet"<<endl;
    ui->tableWidget->setItem(table_i,0,new QTableWidgetItem(infor->time));
    ui->tableWidget->setItem(table_i,1,new QTableWidgetItem(infor->sip));
    ui->tableWidget->setItem(table_i,2,new QTableWidgetItem(infor->smac));
    ui->tableWidget->setItem(table_i,3,new QTableWidgetItem(infor->dip));
    ui->tableWidget->setItem(table_i,4,new QTableWidgetItem(infor->dmac));
    ui->tableWidget->setItem(table_i,5,new QTableWidgetItem(infor->fram));
    ui->tableWidget->setItem(table_i,6,new QTableWidgetItem(infor->transform));
    ui->tableWidget->setItem(table_i,7,new QTableWidgetItem(QString::number(infor->length)));
    inforList.append(infor);
    table_i ++;
    addDataFile(infor);
    if(!strcmp((infor->fram),"ip")){
        ipcount ++;
        if(!strcmp((infor->transform),"tcp")){
            tcpcount ++;
        }else if(!strcmp((infor->transform),"udp")){
            udpcount ++;
        }else if(!strcmp((infor->transform),"icmp")){
            icmpcount ++;
        }
    }else if(!strcmp((infor->fram),"rarp")){
        rarpcount ++;
    }else if(!strcmp((infor->fram),"arp")){
        arpcount ++;
    }else{
        othercount ++;
    }

    flushCount();
}

void Date::closeEvent(QCloseEvent *event){
    closeDataFile();
    delete pcap;
    event->accept();
    exit(0);
//       QMessageBox::StandardButton button;
//       button=QMessageBox::question(this,tr("Quit"),QString(tr("Quit exe?")),QMessageBox::Yes|QMessageBox::No);
//       if(button==QMessageBox::No)
//       {
//           event->ignore(); // 忽略退出信号，程序继续进行
//       }
//       else if(button==QMessageBox::Yes)
//       {
//           delete pcap;
//           event->accept(); // 接受退出信号，程序退出
//       }
}

void Date::dnsPackedge(QString &str, uchar *data){
    str += "-------------dns----------\n";

    dns_header *dns = (dns_header*)data;
    str += ("Transaction ID: "+QString::number(ntohs(dns->tran))+"\n");
    str += ("Flags         : "+QString::number(ntohs(dns->flags))+"\n");
    str += ("Questions num : "+QString::number(ntohs(dns->qn))+"\n");
    str += ("Answer     RRS: "+QString::number(ntohs(dns->rr))+"\n");
    str += ("Authority  RRS: "+QString::number(ntohs(dns->aurr))+"\n");
    str += ("Additional RRS: "+QString::number(ntohs(dns->adrr))+"\n");
    str += ("\n");

}

//选择数据包
void Date::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    if(row>= inforList.count()){
        return;
    }
    //cout<<"row ="<<row<<endl;
    Infor * infor = inforList.at(row);
    QString str ="Frame Type: "+QString(infor->fram)+"\nSource MAC: "+QString(infor->smac)+"\nDestination MAC: "+QString(infor->dmac)+"\n\n";

    uchar * data = infor->date;
    showData(infor);
    //以太网
    enthernet * ent = (enthernet *)data;
    data += sizeof(enthernet);
    datalength = infor->length;
    switch(ent->type){
    case TYPE_ARP:
        arpPackedge(str,data);
        break;
    case TYPE_IP:
        ipPackedge(str,data);
        break;
    case TYPE_RARP:
        str += "RARP packedge!";
        break;
    default:
        str += "unknown data packedge!";
    }

    //    ui->textEdit->setText(QString::number(row)+QString(infor->sip));

    ui->textEdit->setText(str);
}

void Date::countToZero(){
    ipcount=0;
    tcpcount=0;
    arpcount=0;
    rarpcount=0;
    icmpcount=0;
    udpcount=0;
    othercount=0;
    flushCount();
}

void Date::flushCount(){
    ui->label_ip->setText(QString::number(ipcount));
    ui->label_tcp->setText(QString::number(tcpcount));
    ui->label_arp->setText(QString::number(arpcount));
    ui->label_rarp->setText(QString::number(rarpcount));
    ui->label_udp->setText(QString::number(udpcount));
    ui->label_icmp->setText(QString::number(icmpcount));
    ui->label_o->setText(QString::number(othercount));
}

void Date::arpPackedge(QString &str, uchar *data){
    str += "-----arp--------\n";
    ether_arp * arp = (ether_arp *)data;
    str +=("Hardware Type: "+QString::number(ntohs(arp->ea_hdr.ar_hrd))+"\n" );
    str +=("Protocol Type: "+QString::number(ntohs(arp->ea_hdr.ar_pro))+"\n" );
    str +=("MAC    length: "+QString::number(arp->ea_hdr.ar_hln)+"\n" );
    str +=("Proto  length: "+QString::number(arp->ea_hdr.ar_pln)+"\n" );
    str +=("Operate   num: "+QString::number(arp->ea_hdr.ar_op)+"\n" );
    char tempStr[20];
    pcap->thread->byteToMac(tempStr,arp->arp_sha);
    str +=("Source    MAC: "+QString(tempStr)+"\n" );
    pcap->thread->byteToIP(tempStr,arp->arp_spa);
    str +=("Source     IP: "+(QString(tempStr))+"\n" );
    pcap->thread->byteToMac(tempStr,arp->arp_tha);
    str +=("Destinate MAC: "+QString(tempStr)+"\n" );
    pcap->thread->byteToIP(tempStr,arp->arp_tpa);
    str +=("Destinate  IP: "+QString(tempStr)+"\n" );
    str += "\n\n";

    datalength -= sizeof(ether_arp);
}

void Date::ipPackedge(QString &str, uchar *data){
    ip_header * ip = (ip_header *)data;
    //data += sizeof(ip_header);

    str += "-----ip--------\n";
    str +=("Version      : "+QString::number((ip->ver_ihl>>4)&0xf)+"\n" );
    str +=("Header length: "+QString::number(((ip->ver_ihl))&0xf)+"\n" );
    str +=("Service  type: "+QString::number(ip->tos)+"\n" );
    str +=("Total  length: "+QString::number(ntohs(ip->tlen))+"\n" );
    str +=("Identificate : "+QString::number(ntohs(ip->identification))+"\n" );
    str +=("Life    cycle: "+QString::number(ip->ttl)+"\n" );
    str +=("Protocol     : "+QString::number(ip->proto)+"\n" );
    char tempStr[20];
    pcap->thread->byteToIP(tempStr,ip->saddr);
    str +=("Source     IP: "+QString(tempStr)+"\n" );
    pcap->thread->byteToIP(tempStr,ip->daddr);
    str +=("DestinationIP: "+QString(tempStr)+"\n" );
    str += "\n";

    data += (((ip->ver_ihl)&0xf)*4);
    datalength -= (((ip->ver_ihl)&0xf)*4);

    switch (ip->proto) {
    case TYPE_TCP:
        tcpPackedge(str,data);
        break;
    case TYPE_UDP:
        udpPackedge(str,data);
        break;
    case TYPE_ICMP:
        icmpPackedge(str,data);
        break;
    default:
        break;
    }
}

void Date::tcpPackedge(QString &str, uchar *data){

    str += "-----tcp--------\n";
    tcp_header * tcp = (tcp_header*)data;
    /*cout<<"start : "<<dec<<sizeof(tcp_header)<<endl;
    for(int i=0;i<sizeof(tcp_header);i++){
        cout<<hex<<(int)data[i]<<" ";
    }
    cout<<endl;*/
    str += ("Source      port: "+QString::number(ntohs(tcp->src_port))+"\n");
    str += ("Destination port: "+QString::number(ntohs(tcp->dst_port))+"\n");
    str += ("Serial    number: "+QString::number(ntohl(tcp->seq_no))+"\n");
    str += ("Confirm   number: "+QString::number(ntohl(tcp->ack_no))+"\n");
    str += ("Tcpheader length: "+QString::number(tcp->thl)+"\n");
    str += ("6-digit     flag: "+QString::number(tcp->flag)+"\n");
    str += ("Window      size: "+QString::number(ntohs(tcp->wnd_size))+"\n");
    str += ("Inspection      : "+QString::number(ntohs(tcp->chk_sum))+"\n");
    str += ("EmergencyPointer: "+QString::number(ntohs(tcp->urgt_p))+"\n");
    str += "\n";

    data += (tcp->thl)*4;
    datalength -= (tcp->thl)*4;

    //其他的东西了
    //http
    if(ntohs(tcp->src_port)==TYPE_HTTP||ntohs(tcp->dst_port)==TYPE_HTTP){
        httpPackedge(str,data);
    }else if(ntohs(tcp->src_port)==TYPE_DNS||ntohs(tcp->dst_port)==TYPE_DNS){
        dnsPackedge(str,data);
    }

}

void Date::httpPackedge(QString &str, uchar *data){
    str += "----------http-----------\n";
    str += QString::fromUtf8(reinterpret_cast<char *>(data),datalength);

    str += "\n";

}

void Date::udpPackedge(QString &str, uchar *data){

    str += "-----udp--------\n";
    udp_header *udp = (udp_header*)data;

    str += ("Source      port: "+QString::number(ntohs(udp->sport))+"\n");
    str += ("Destination port: "+QString::number(ntohs(udp->dport))+"\n");
    str += ("Data      length: "+QString::number(ntohs(udp->len))+"\n");
    str += ("Confirm   number: "+QString::number(ntohs(udp->crc))+"\n");
    str += "\n";

    data += sizeof(udp_header);
    datalength -= sizeof(udp_header);
    //其他东西
    if(ntohs(udp->sport)==TYPE_DNS||ntohs(udp->dport)==TYPE_DNS){
            dnsPackedge(str,data);
    }
}

void Date::icmpPackedge(QString &str, uchar *data){
     str += "-----icmp--------\n";

    icmp_header* icmp = (icmp_header *)data;

    str += ("Type   : "+QString::number(icmp->icmp_type)+"\n");
    str += ("Code   : "+QString::number(icmp->code)+"\n");
    str += ("Confirm: "+QString::number(ntohs(icmp->chk_sum))+"\n");

    str += "\n";

    data += sizeof(icmp_header);
    datalength -= sizeof(icmp_header);

    unsigned short * usx;
    usx = (unsigned short *)data;
    data += 4;
    //其他东西
    switch(icmp->icmp_type){
    case 0:
        str += "----------Echo Reply:--------\n";
        str += ("Indentifier : "+ QString::number(ntohs(usx[0]))+"\n");
        str += ("Sequence Num: "+ QString::number(ntohs(usx[1]))+"\n");
        str + "\n";
        str += QString::fromUtf8(reinterpret_cast<char *>(data));
        str += "\n------------end-------------\n";
        break;
    case 3:
        str += "Unreachable\n";
        break;
    case 5:
        str += "Redirect \n";
        break;
    case 8:
        str += "-----------Echo reques-------\n";
        str += ("Indentifier : "+ QString::number(ntohs(usx[0]))+"\n");
        str += ("Sequence Num: "+ QString::number(ntohs(usx[1]))+"\n");
        str += "\n";
        str += QString::fromUtf8(reinterpret_cast<char *>(data));
        str += "\n------------end-------------\n";
        break;
    default:
        str += "too lazy to write!!!!\n";
    }
}

void Date::showData(Infor *infor){
    uint size=32;
    uchar * data = infor->date;
    uint num=0;
    QStringList strlist;
    while(num<infor->length){
        if(infor->length - num < size){
            strlist.append(QString("%1    %2    %3")
                           .arg(num,4,10,QChar('0'))
                           .arg(QString(QByteArray::fromRawData((char *)data,infor->length-num).toHex()).toUpper(),size*2,QChar(' '))
                           .arg(QString(QString::fromUtf8(reinterpret_cast<char *>(data),infor->length-num))));
            break;
        }
        strlist.append(QString("%1    %2    %3")
                       .arg(num,4,10,QChar('0'))
                       .arg(QString(QByteArray::fromRawData((char *)data,size).toHex()).toUpper())
                       .arg(QString::fromUtf8(reinterpret_cast<char *>(data),size)));
        data += size;
        num += size;
    }
    ui->listWidget->clear();
    ui->listWidget->addItems(strlist);
}

void Date::openDataFile(){
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh.mm.ss");
    QString fileName = DATA_FILE_NAME;
    current_date.replace(QChar(' '),"_");
    fileName += current_date;
    if(!(file=fopen(fileName.toStdString().c_str(),"w+"))){
        qDebug()<<"fail to create "<<fileName;
        return;
    }
    qDebug()<<"Sucess to create "<<fileName;
}

void Date::closeDataFile(){
    if(!file){
        return ;
    }
    fclose(file);
}

void Date::addDataFile(Infor *infor){
    if(!file){
        return ;
    }
    FileInfor fileInfor;
    fileInfor.infor=*infor;
    memcpy(fileInfor.data,infor->date,infor->length);
    fwrite(&fileInfor,sizeof(FileInfor),1,file);
}

