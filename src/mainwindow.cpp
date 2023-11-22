#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include "dialogAbout.h"

// *** Class constructior ***
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dr.setCallbackMessage(&MainWindow::callbackHelper, this);
    lastPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    //Translation
    qtTranslator = new QTranslator();
    QString s_lang = "dr1801A6_import_"+QLocale::system().name();
    QString m_langPath = QApplication::applicationDirPath();
    bool ret = qtTranslator->load(s_lang, m_langPath);
    QApplication::installTranslator(qtTranslator);
    if (ret) this->ui->retranslateUi(this);
}

// *** Class destructior ***
MainWindow::~MainWindow()
{
    delete ui;
}

// *** Event on click open *.accps file dialog ***
void MainWindow::on_pushButtonOpenAccps_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open .accps file"),
                                                    lastPath,
                                                    tr("accps files (*.accps)")
                                                    );
    if (fileName != ""){
        lastPath = QFileInfo(fileName).absolutePath();
        ui->lineEditOpenAccps->setText(fileName);  

        //Check file consistant
        std::string fileS = fileName.toStdString();
        const char *fileC = fileS.c_str();
        if (dr.check_accpsConsist(fileC))
            ui->textEditOut->append(tr("Check file .accps consistant PASS."));

    }
}

// *** Event on click open *.csv file dialog ***
void MainWindow::on_pushButtonOpenCsv_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open .csv file"),
                                                    lastPath,
                                                    tr("csv files (*.csv)")
                                                    );
    if (fileName != ""){
        lastPath = QFileInfo(fileName).absolutePath();
        ui->lineEditOpenCsv->setText(fileName);
    }
}

// *** Event on click read from *.accps file ***
void MainWindow::on_pushButtonReadAccps_clicked()
{
    std::string fileS = ui->lineEditOpenAccps->text().toStdString();
    lastPath = QFileInfo(ui->lineEditOpenAccps->text()).absolutePath();
    const char *fileC = fileS.c_str();
    DR1801a6i::Contact c;

    bool r = dr.read_accps(fileC);

    ui->pushButtonWriteAccps->setEnabled(r && dr.getContactLen()>0);
    if (r)
        printContacts(&c);

    ui->pushButtonWriteAccps->setEnabled(r);
    ui->pushButtonWriteCsv->setEnabled(r);
}

// *** Event on click read from *.csv file ***
void MainWindow::on_pushButtonReadCsv_clicked()
{
    std::string fileS = ui->lineEditOpenCsv->text().toStdString();
    lastPath = QFileInfo(ui->lineEditOpenCsv->text()).absolutePath();
    const char *fileC = fileS.c_str();
    DR1801a6i::Contact c;

    bool r = dr.read_csv(fileC, ui->checkBoxN->isChecked());

    ui->pushButtonWriteAccps->setEnabled(r && dr.getContactLen()>0);
    if (r)
        printContacts(&c);

    ui->pushButtonWriteAccps->setEnabled(r);
    ui->pushButtonWriteCsv->setEnabled(r);
}

// *** Event on click save *.accps file dialog ***
void MainWindow::on_pushButtonWriteCsv_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save to .csv file"),
                                                    lastPath,
                                                    tr("csv files (*.csv)")
                                                    );
    if (fileName != ""){
        std::string fileS = fileName.toStdString();
        const char *fileC = fileS.c_str();
        dr.write_csv(fileC, ui->checkBoxN->isChecked());
    }
}

// *** Event on click write to *.accps file ***
void MainWindow::on_pushButtonWriteAccps_clicked()
{
    std::string fileS = ui->lineEditOpenAccps->text().toStdString();
    lastPath = QFileInfo(ui->lineEditOpenAccps->text()).absolutePath();
    const char *fileC = fileS.c_str();
    dr.write_accps(fileC);
}

// *** Callback for print messages from DR1801a6i class static helper ***
#ifdef QT_CORE_LIB
void MainWindow::callbackHelper(QString txt, void *ctx)
#else
void MainWindow::callbackHelper(const char *txt, void *ctx)
#endif
{
    MainWindow* mwin = (MainWindow*)ctx;
    mwin->callback(txt);
}

// *** Callback for print messages from DR1801a6i class ***
#ifdef QT_CORE_LIB
void MainWindow::callback(QString txt)
#else
void MainWindow::callback(const char *txt)
#endif
{
    QString s = txt;
    ui->textEditOut->append(s);
}

// *** Function for print contacts to logs ***
void MainWindow::printContacts(DR1801a6i::Contact *c){
    for (int i=0; i<dr.getContactLen(); i++) {
        if (dr.getContact(i, c)==true){
#ifdef QT_DEBUG
            QString s = QString::number(c->cnt) + ": br[" + QString::number(c->breaker) + "]";
            s += " typ[" + QString::number(c->typ) + "] number[" + QString::number(c->number) + "]";
            s += " name[" +  QString::number(c->txtLen) + "][" + c->name + "]";
            ui->textEditOut->append(s);
#else
            QString s = QString::number(c->cnt);
            s += " number[" + QString::number(c->number) + "]";
            s += " name[" + (QString) c->name + "]";
            ui->textEditOut->append(s);
#endif
        }
    }
}

// *** Event on click Help/About ***
void MainWindow::on_actionAbout_triggered()
{
    DialogAbout dial;
    dial.exec();
}

// *** Function for clear logs ***
void MainWindow::on_pushButtonClearLogs_clicked()
{
    ui->textEditOut->clear();
}
