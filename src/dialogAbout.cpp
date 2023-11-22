#include "dialogAbout.h"
#include "ui_dialogAbout.h"

//Dialog About class
DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    //Prepare version label
    #ifdef _WIN32
        QString ver = QString("%1.%2.%3.%4").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD).arg(VERSION_BUILD2);
    #else
        QString ver = QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD);
    #endif

    ui->retranslateUi(this); //Translate window
    ui->labelVer->setText(tr("Version:") + " " +ver);
}


DialogAbout::~DialogAbout()
{
    delete ui;
}
