#ifndef DR1801A6I_H
#define DR1801A6I_H
#include <QTranslator>
#include <iostream>
#include <fstream>

#ifdef QT_CORE_LIB
#include <QString>
#endif




#ifdef QT_CORE_LIB
class DR1801a6i : public QObject {
    Q_OBJECT
#else
class DR1801a6i {
#endif

        public:
    DR1801a6i();
    ~DR1801a6i();


    struct Contact{
        unsigned int cnt;     //Counter 2..1024 (0 last)
        unsigned int txtLen;  //Text length (max 15 + null)
        unsigned int breaker; //3-normal, 1-break
        unsigned int number;  //Call number
        unsigned int typ;     //8-Group call, 4-Private call, 2-All call
        char name[16];        //CallSign or CallSign + " " + Name (max 15 characters)
    };

    int getContactLen();
    std::string delimiterS;
    static void removeCharsFromString(std::string& str, const char* charsToRemove);
    bool check_accpsConsist(const char *fileN);
    bool getContact(int nr, Contact *c);
    bool read_accps(const char *fileN);
    bool read_csv(const char *fileN, bool withN);
    bool write_csv(const char *fileN, bool withN, std::string delim=",");
    bool write_accps(const char *fileN);


#ifdef QT_CORE_LIB
    void setCallbackMessage(void(*cb)(QString, void *), void *ctx);
#else
    void setCallbackMessage(void(*cb)(const char *, void *), void *ctx);
#endif



private:
    void *ctx;
#ifdef QT_CORE_LIB
    void (*_cb)(QString txt, void *ctx) = nullptr;
#else
    void (*_cb)(const char *txt, void *ctx) = nullptr;
#endif

    Contact *arrContact = NULL;
    int arrContactLen;
    int findDelimiterMax(std::string delim, std::string s=",");
    bool chkTGxxx(std::string call);
#ifdef QT_CORE_LIB
    void sendMsg(QString s);
#else
    void sendMsg(std::string s);
#endif

};

#endif // DR1801A6I_H
