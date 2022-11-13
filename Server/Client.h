#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include "../../RSA/RSAFunctions.h"



class Client : public QObject{

    Q_OBJECT


public:

    Client(QTcpSocket* socket, clef e, clef n) : m_socket(socket), m_e(e), m_n(n)
    {
        QObject::connect(m_socket, &QTcpSocket::readyRead, this, &Client::socketReadyRead);
        QObject::connect(m_socket, &QTcpSocket::disconnected, this, &Client::socketDisconnected);

    }


    QTcpSocket* getSocket() {return m_socket;}
    clef getE() const {return m_e;}
    clef getN() const {return m_n;}


    void setE(clef const& e){m_e = e;}
    void setN(clef const& n){m_n = n;}


public: signals :

    void readyRead();
    void disconnected();




private slots:

    void socketReadyRead();
    void socketDisconnected();

private:


    QTcpSocket* m_socket;
    clef m_e;
    clef m_n;

};



#endif // CLIENT_H
