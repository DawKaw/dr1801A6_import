#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include "dr1801a6i.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTranslator *qtTranslator;

private slots:
    void on_pushButtonOpenCsv_clicked();
    void on_pushButtonWriteCsv_clicked();
    void on_pushButtonReadCsv_clicked();
    void on_pushButtonOpenAccps_clicked();
    void on_pushButtonReadAccps_clicked();
    void on_pushButtonWriteAccps_clicked();
    void on_actionAbout_triggered();
    void on_pushButtonClearLogs_clicked();

private:
    Ui::MainWindow *ui;
    DR1801a6i dr;
    QString lastPath;
    void printContacts(DR1801a6i::Contact *c);
#ifdef QT_CORE_LIB
    static void callbackHelper(QString txt, void* ctx);
    void callback(QString txt);
#else
    static void callbackHelper(const char *txt, void* ctx);
    void callback(const char *txt);
#endif
};
#endif // MAINWINDOW_H
