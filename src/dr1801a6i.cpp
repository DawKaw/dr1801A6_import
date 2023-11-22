#include "dr1801a6i.h"

#include <string.h>
#include<algorithm>


/*
Nagłówek
0000000 41 55 43 54 55 53 41 36 00 03 68 00 9E E6 02 00  AUCTUSA6�h�žć�

LN - Length of Contacts
                     LN:11
                     _____
00004330 00 00 00 00 0B 00 01 00  ������

c - Counter
L - Text length
B - Breaker 1-at end, 2, 3
N - Number uint24
t - Type: 80-Group call, 40-Private call, 20-All call


         c:2   L:5 B    N:260 t
         _____ __ __ ________ _
00004338 02 00 05 03 04 01 00 80  ��€
00004340 54 47 32 36 30 00 00 00  TG260���    <--| Texts 16 bytes
00004348 00 00 00 00 00 00 00 00  �������� <-/


         c:3   L:6     N:2606 t
         _____ __    ________ _
00004350 03 00 06 03 2E 0A 00 80  �.�€
00004358 54 47 32 36 30 36 00 00  TG2606��
00004360 00 00 00 00 00 00 00 00  ��������

         c:4   L:6  N:2606142 t
         _____ __    ________ _
00004368 04 00 06 03 3E C4 27 40  �>Ä'@
00004370 53 51 36 4E 4C 51 00 00  SQ6NLQ��
00004378 00 00 00 00 00 00 00 00  ��������

         c:5   L:6  N:2606049 t
         _____ __    ________ _
00004380 05 00 06 03 E1 C3 27 40  �áĂ'@
00004388 53 51 36 4B 4F 45 00 00  SQ6KOE��
00004390 00 00 00 00 00 00 00 00  ��������

         c:6   L:9        N:5 t
         _____ __    ________ _
00004398 06 00 09 03 05 00 00 80  �	��€
000043A0 43 6F 6E 74 61 63 74 20  Contact
000043A8 35 00 00 00 00 00 00 00  5�������

         c:7   L:9        N:6 t
         _____ __    ________ _
000043B0 07 00 09 03 06 00 00 80  �	��€
000043B8 43 6F 6E 74 61 63 74 20  Contact
000043C0 36 00 00 00 00 00 00 00  6�������

         c:8   L:9        N:7 t
         _____ __    ________ _
000043C8 08 00 09 03 07 00 00 80  �	��€
000043D0 43 6F 6E 74 61 63 74 20  Contact
000043D8 37 00 00 00 00 00 00 00  7�������

         c:9   L:9        N:8 t
         _____ __    ________ _
000043E0 09 00 09 03 08 00 00 80  	�	��€
000043E8 43 6F 6E 74 61 63 74 20  Contact
000043F0 38 00 00 00 00 00 00 00  8�������

         c:10  L:9        N:9 t
         _____ __    ________ _
000043F8 0A 00 09 03 09 00 00 80  �		��€
00004400 43 6F 6E 74 61 63 74 20  Contact
00004408 39 00 00 00 00 00 00 00  9�������

         c:11  L:10      N:10 t
         _____ __    ________ _
00004410 0B 00 0A 03 0A 00 00 80  ���€
00004418 43 6F 6E 74 61 63 74 20  Contact
00004420 31 30 00 00 00 00 00 00  10������

         c:0  L:10 N:16777215 t
         _____ __ xx ________ _
00004428 00 00 0A 01 FF FF FF 20  ��˙˙˙
00004430 43 6F 6E 74 61 63 74 20  Contact
00004438 31 31 00 00 00 00 00 00  11������


0000A338 09 00 00 00 01 07 00 00 Scan List begin
0000A340 00 00 00 00 00 00 00 00
*/


// *** Class Constructor ***
DR1801a6i::DR1801a6i()
{

}


// *** Class Destructor ***
DR1801a6i::~DR1801a6i()
{
    if (arrContact != NULL)
        delete[] arrContact;
}


#ifdef QT_CORE_LIB
void DR1801a6i::setCallbackMessage(void(*cb)(QString, void *) , void *ctx)
#else
void DR1801a6i::setCallbackMessage(void(*cb)(const char*, void *) , void *ctx)
#endif
{
    this->ctx = ctx;
    this->_cb = cb;
}


#ifdef QT_CORE_LIB
void DR1801a6i::sendMsg(QString s){
    if (this->_cb==nullptr) return;
    this->_cb(s, this->ctx);
}
#else
void DR1801a6i::sendMsg(std::string s){
    if (this->_cb==nullptr) return;
    const char *txt = s.c_str();
    this->_cb(txt, this->ctx);
}
#endif


// *** Return count of Contacts ***
int DR1801a6i::getContactLen()
{
    return arrContactLen;
}


// *** Get one record of Contacts table ***
bool DR1801a6i::getContact(int nr, Contact *c){
    if (nr>=arrContactLen){
        c = NULL;
        return false;
    }
    memcpy(c, &arrContact[nr], sizeof(Contact));
    return true;
}


bool DR1801a6i::check_accpsConsist(const char *fileN)
{


    bool ret = true;
    std::ifstream in_file;

    // *** Open file .accps ***
    in_file.open(fileN, std::ios::in | std::ios::binary);
    if (!in_file.is_open()){
#ifdef QT_CORE_LIB
        sendMsg(tr("Can't open file!"));
#else
        sendMsg("Can't open file!");
#endif
        return false;
    }

    // *** check file size ***
    in_file.seekg(0, std::ios::end);
    std::streampos fsize = in_file.tellg();
    const int fiesizeconst = 122256;
    if (fsize < fiesizeconst){
#ifdef QT_CORE_LIB
      sendMsg(QString("File size is less than min: %1[%2]").arg(fiesizeconst, fsize));
#else
      sendMsg("File size is less than min: " + std::to_string(fiesizeconst) + "[" + std::to_string(fsize)+ "]");
#endif
      in_file.close();
      return false;
    }

    // *** Read 8 bytes and check text "AUCTUSA6" ***
    const char *file_header = "AUCTUSA6";
    char bufHead[9]; memset(bufHead, 0, sizeof(bufHead));
    in_file.seekg(0, std::ios::beg);
    in_file.read( (char*)bufHead, 8);
    if(strcmp(bufHead, file_header) != 0){
        std::cout << "Header ERR. is not AUCTUSA6" << std::endl;
        ret = false;
    }// else {
    //  //cout << "Header OK [" << bufRead  << "]" << endl;
    //  std::string str(bufHead, 8);
    //sendMsg("Header OK [" +str+ "]");
    //}

    in_file.close();
    return ret;
}


// *** Read .accps file ***
bool DR1801a6i::read_accps(const char* fileN)
{
    if (!check_accpsConsist(fileN))
       return false;

    bool ret = true;
    std::ifstream in_file;

    // *** Open file .accps ***
    in_file.open(fileN, std::ios::in | std::ios::binary);
    if (!in_file.is_open()){
        sendMsg("Can't open file");
        return false;
    }

    // *** Read Contacts count ***
    in_file.seekg(0x4334, std::ios::beg);
    char bufL[2];
    in_file.read( (char*)bufL, 2);
    arrContactLen = 0;
    memcpy(&arrContactLen, &bufL[0], 2);
    if (arrContactLen <= 1024){
#ifdef QT_CORE_LIB
        sendMsg(QString(tr("Contacts: %1")).arg(this->arrContactLen));
#else
        sendMsg("Contacts:" + std::to_string(this->arrContactLen));
#endif
    } else {
#ifdef QT_CORE_LIB
        sendMsg(QString(tr("To many contacts: %1 !")).arg(this->arrContactLen));
#else
        sendMsg("To many contacts:" + std::to_string(this->arrContactLen)+"!");
#endif
        arrContactLen = 1024;
    }


    // *** Create array for contacts with properly length
    if (arrContact != NULL)
        delete[] arrContact;
    arrContact = new Contact[arrContactLen];


    // *** Read 24 bytes of contacts in loop ***
    in_file.seekg(0x4338, std::ios::beg);
    char bufC[24];
    for (int i=0; i<this->arrContactLen; i++){
      in_file.read( (char*)bufC, 24);

      Contact *c = &arrContact[i];
      memset(c, 0, sizeof(Contact));
      memcpy(&c->cnt, &bufC[0], 2);
      memcpy(&c->txtLen, &bufC[2], 1);
      memcpy(&c->breaker, &bufC[3], 1);
      memcpy(&c->number, &bufC[4], 3);
      memcpy(&c->typ, &bufC[7], 1); c->typ = c->typ >> 4;
      memcpy(c->name, &bufC[8], 16);
      //cout << c->cnt << ": br[" << c->breaker << "] typ[" << c->typ << "] number[" << c->number << "] name[" << c->txtLen << "][" << c->name << "]" << endl;
    }

    in_file.close();
    return ret;
}


// *** Find delimiter character ***
int DR1801a6i::findDelimiterMax(std::string delim, std::string s){
    int found;
    int cnt=0;
    found = s.find(delim);
    if (found>=0) cnt++;

    while ( found<(int)s.length() ){
       found=s.find(delim, found+1);
       if (found<0)
           break;
       else
           cnt++;
    }
    return cnt;
}


// *** Read data from CSV ***
bool DR1801a6i::read_csv(const char *fileN, bool withN)
{    
    // *** Open file .csv ***
    std::ifstream in_file;
    in_file.open(fileN, std::ios::in);
    if (!in_file.is_open()){
        //std::cout << "Can't open file!" << std::endl;
#ifdef QT_CORE_LIB
        sendMsg(tr("Can't open file!"));
#else
        sendMsg("Can't open file!");
#endif
        return false;
    }


    // *** check file size ***
    in_file.seekg(0, std::ios::end);
    std::streampos fsize = in_file.tellg();
    const int fiesizeconst = 16;
    if (fsize < fiesizeconst){
#ifdef QT_CORE_LIB
      sendMsg(QString(tr("File size is less than min: %1[%2]")).arg(fiesizeconst,fsize));
#else
      sendMsg("File size is less than min:" + std::to_string(fiesizeconst)+"["+std::to_string(fsize)+"]");
#endif
      in_file.close();
      return false;
    }

    in_file.seekg(0, std::ios::beg);
    char line[100];

    // *** Read Header ***
    in_file.getline(line, 99);
    std::string lineS = line;


    // *** Find delimiter in header ***
    std::string delimiterSArr[4] = {";", ",", "\t", "|"};
    int delimiterCnt[4] = {0, 0, 0, 0};
    int delimiterCntMax = 0;
    int delimiterI = 0;

    for (int i = 0; i < 4; i++) {
      delimiterCnt[i] = findDelimiterMax(delimiterSArr[i], lineS);
      if (delimiterCnt[i] > delimiterCntMax){
          delimiterCntMax = delimiterCnt[i];
          delimiterI = i;
      }
    }
    delimiterS = delimiterSArr[delimiterI];
#ifdef QT_CORE_LIB
    sendMsg(QString(tr("Delimiter '%1' found in header %2 times.")).arg(QString::fromStdString(delimiterS)).arg(delimiterCntMax));
#else
    sendMsg("Delimiter '"+ delimiterS + "' found in header " + std::to_string(delimiterCntMax) + " times.");
#endif

    int linesCount = 0;
    // *** Determine lines count
    while (!in_file.eof()) {
         in_file.getline(line, 99);
         linesCount++;
    }
    linesCount--; //skip header line in counts
#ifdef QT_CORE_LIB
    sendMsg(QString(tr("csv lines count: %1")).arg(linesCount));
#else
    sendMsg("csv lines count: "+ std::to_string(linesCount));
#endif

    // *** Create array for contacts with properly length
    if (arrContact != NULL)
        delete[] arrContact;
    arrContactLen = linesCount;
    arrContact = new Contact[arrContactLen];


    // *** Back to first line ***
    in_file.clear();
    in_file.seekg(0, std::ios::beg);
    in_file.getline(line, 99); //skip header line


    // *** Loop line by line ***

    int cnt = 0;
    while (!in_file.eof()) {

        in_file.getline(line, 99);
        std::string s = line;
        if (s.length()==0)
            break; //Finish loop when line is empty.

        memset(arrContact[cnt].name, 0, 16);

        //First column - ID
        int found = s.find(delimiterS);
        if (found > 0){
           std::string ids = s.substr(0, found);
           ids.erase(remove(ids.begin(), ids.end(), '"'), ids.end()); //remove " from string
           s.erase(0, found+1);
           //std::cout << cnt << " ID:" << ids;
           arrContact[cnt].cnt = cnt+2;
           arrContact[cnt].breaker = 3;
           arrContact[cnt].typ = 4;    //Private call
           try {arrContact[cnt].number = std::stoi(ids);}
           catch(...){
#ifdef QT_CORE_LIB
               sendMsg(QString(tr("Error at line: %1")).arg(cnt));
#else
               sendMsg("Error at line " + std::to_string(cnt));
#endif
               return false;
           }
           //std::cout << arrContact[cnt].cnt << " ID:" << arrContact[cnt].number;
        }

        //Second column - Call
        std::string call = "";
        found = s.find(delimiterS);
        if (found > 0){
           call = s.substr(0, found);
           s.erase(0, found+1);
           //std::cout << " Call:" << call;
        } else {
           call = s;
           s = "";
        }

        call.erase(remove(call.begin(), call.end(), '"'), call.end()); //remove " from string

        if (chkTGxxx(call))
           arrContact[cnt].typ = 8;   //Group call


        //Third column - Name
        std::string name = "";
        found = s.find(delimiterS);
        if (found > 0 || s.length() > 0)
           name = s.substr(0, found);

        if (withN && name.length()>0)
           call = call + " " + name;

        if (call.length()>15)
           call.erase(15);

        //std::cout << " Call[" << call.length() << "]:" << call << std::endl;
        memcpy(&arrContact[cnt].name, call.c_str(), call.length());
        //arrContact[cnt].name[16] = 0;
        arrContact[cnt].txtLen = call.length();

        //std::cout << " Call:" << arrContact[cnt].name << std::endl;
        cnt++;
    }

    in_file.close();
    return true;
}


// *** call is TG (Taliking Group) ***
bool DR1801a6i::chkTGxxx(std::string call){
    if (call.length() < 3)
        return false;

    if (call[0] == 't') call[0] = 't';
    if (call[1] == 'g') call[1] = 'g';

    if (call[0] != 'T' || call[1] != 'G') //Text begint "TG" ?
        return false;

    call.erase(0, 2);                  //Cut first 2 characters

    int found = call.find(" ");        //Try find whitespace
    if (found > 0 || call.length() > 0)// if found -
       call = call.substr(0, found);   //   cat all characters past

    int ci = 0;
    try {
        ci = std::stoi(call);          //Try convert text to int
    }
    catch(...){
        return false;
    }

    //std::cout << " call:" << call << " ci:" << ci << std::endl;
    if (ci > 0)
        return true;

    return true;
}


// *** Write data to CSV ***
bool DR1801a6i::write_csv(const char* fileN, bool withN, std::string delim){
    if (delim.length()==0)
        delim=",";

    if (arrContactLen <= 0){
        std::cout << "Buffer is empty!" << std::endl;
#ifdef QT_CORE_LIB
        sendMsg(QString(tr("Buffer is empty!")));
#else
        sendMsg("Buffer is empty!");
#endif
        return false;
    }
    std::ofstream out_file;

    // *** Open file .csv ***
    out_file.open(fileN, std::ios::out);
    if (!out_file.is_open()){
        //std::cout << "Can't open file for write!" << std::endl;
#ifdef QT_CORE_LIB
        sendMsg(QString(tr("Can't open file for write!")));
#else
        sendMsg("Can't open file for write!");
#endif
        return false;
    } else {
#ifdef QT_CORE_LIB
         sendMsg(QString(tr("File open for write ok.")));
#else
        sendMsg("File open for write ok.");
#endif
    }

    // *** Write proprly header to CSV ***
    std::string header = "ID,Call";
    if (withN)
        header = header + delim[0] + "Name";
    header = header + "\n";
    out_file.write(header.c_str(), header.length());

    // *** Write data ***
    for (int i=0; i<arrContactLen; i++) {

        std::string call_name = arrContact[i].name;
        if (withN){
           int f = call_name.find(" ");
           if (f>0)
               call_name[f] = delim[0];
        }
        std::string s = std::to_string(arrContact[i].number) + delim + call_name + "\n";
        out_file.write(s.c_str(), s.length());
    }

    out_file.close();
    return true;
}


// *** Write data to accps ***
bool DR1801a6i::write_accps(const char *fileN){

    if (arrContactLen <= 0){
#ifdef QT_CORE_LIB
       sendMsg(tr("Buffer is empty!"));
#else
       sendMsg("Buffer is empty!");
#endif
        return false;
    }

    if (!check_accpsConsist(fileN))
       return false;

    std::fstream out_file;

    // *** Open file .accps ***
    out_file.open(fileN, std::ios::in | std::ios::out | std::ios::binary);
    if (!out_file.is_open()){
#ifdef QT_CORE_LIB
        sendMsg(tr("Can't open file for write!"));
#else
        sendMsg("Can't open file for write!");
#endif
        return false;
    } else {
#ifdef QT_CORE_LIB
        sendMsg(tr("File open for write ok."));
#else
        sendMsg("File open for write ok.");
#endif
    }

    // *** Write Contacts count ***
    out_file.seekg(0x4334, std::ios::beg);
    char bufL[2];
    memcpy(&bufL[0], &arrContactLen, 2);
    out_file.write(bufL, 2);


    // *** Write 24 bytes of contacts in loop ***
    out_file.seekg(0x4338, std::ios::beg);
    char bufC[24];
    for (int i=0; i<arrContactLen; i++){
      Contact *c = &arrContact[i];

      if (i==arrContactLen-1){
         c->breaker = 0;
         c->cnt = 0xFFFF;
      }

      memcpy(&bufC[0], &c->cnt, 2);
      memcpy(&bufC[2], &c->txtLen, 1);
      memcpy(&bufC[3], &c->breaker, 1);
      memcpy(&bufC[4], &c->number, 3);
      unsigned int t = c->typ << 4;
      memcpy(&bufC[7], &t, 1);
      memcpy(&bufC[8], c->name, 16);
      out_file.write(bufC, 24);
    }



    memset(bufC, 0, 24);
    for (int i=arrContactLen; i<1024; i++)
      out_file.write(bufC, 24);
#ifdef QT_CORE_LIB
    sendMsg(tr("Finish writing file ok: ") + fileN);
#else
    sendMsg("Finish writing file ok: ");
#endif

    out_file.close();
    return true;
}
