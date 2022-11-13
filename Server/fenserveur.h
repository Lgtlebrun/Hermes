#ifndef FENSERVEUR_H
#define FENSERVEUR_H

#include <QtWidgets>
#include <QtNetwork>
#include <QTimer>
#include "../../RSA/RSAFunctions.h"
#include "Client.h"

class FenServeur: public QWidget
{
    Q_OBJECT

public:
    FenServeur();
    void sendAll(const QString& message);
    void sendAllCrypt(const QString& message);
    void sendOne(QTcpSocket* socket, const QString& message);
    void sendKeys();

private slots:

    void newConnection();
    void dataReceived();
    void dataReceivedFromPending();
    void deconnectionClient();
    void deconnectionPending();

private:

    void newKeySet();
    bool checkKeys();

    QLabel* m_etatServeur;
    QPushButton* m_quit;
    QPushButton* m_changeKeys;

    QTcpServer* m_serv;
    QList<QTcpSocket*> m_pending;
    QList<Client*> m_clients;
    QList<clef> m_usedPublicKeys;
    quint16 m_sizeMessage;

    clef m_privee;
    clef m_e;
    clef m_n;

    QTimer* m_timer;

};

#endif // FENSERVEUR_H
