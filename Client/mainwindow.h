#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets>
#include <QTimer>
#include <QtNetwork>
#include "ui_fenclient.h"
#include "../../RSA/RSAFunctions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FenClient; }
QT_END_NAMESPACE

class MainWindow : public QWidget, private Ui::FenClient
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_connectButton_clicked();
    void on_sendButton_clicked();
    void on_message_returnPressed();
    void dataReceived();
    void connected();
    void disconnected();
    void errorSocket(QAbstractSocket::SocketError erreur);


private:

    //================METHODES PRIVEES================

    void sendServ(QString const& mess);
    void sendMess(const QString &mess);
    void newKeySet();
    bool checkKeys();


    QTcpSocket* m_socket;
    quint16 m_sizeMessage;

    bool m_isConnected;
    clef m_privee;
    clef m_e;
    clef m_n;

    clef m_eServ;
    clef m_nServ;

    QTimer* m_timer;
};
#endif // MAINWINDOW_H
