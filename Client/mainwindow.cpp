#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), m_isConnected(false)
{
    setupUi(this);

    setWindowTitle("Hermes");
    setWindowIcon(QIcon(":/icone/images/logo.png"));

    m_socket = new QTcpSocket(this);
    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::dataReceived);
    QObject::connect(m_socket, &QTcpSocket::connected, this, &MainWindow::connected);
    QObject::connect(m_socket, &QTcpSocket::disconnected, this, &MainWindow::disconnected);
    QObject::connect(qApp, &QApplication::aboutToQuit, m_socket, &QTcpSocket::disconnectFromHost);
    QObject::connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSocket(QAbstractSocket::SocketError)));

    QObject::connect(boutonConnexion, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);
    QObject::connect(boutonEnvoyer, &QPushButton::clicked, this, &MainWindow::on_sendButton_clicked);


    m_sizeMessage = 0;

    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, this, &MainWindow::newKeySet);
}

MainWindow::~MainWindow()
{
}



void MainWindow::on_connectButton_clicked(){

    listeMessages->append(tr("<em>Trying to connect...</em>"));
    boutonConnexion->setEnabled(false);

    m_socket->abort();
    m_socket->connectToHost(serveurIP->text(), serveurPort->value());

}




void MainWindow::on_sendButton_clicked(){

    sendMess(message->text());



    message->clear();
    message->setFocus(); // On replace le curseur dans la zone de message

}


void MainWindow::on_message_returnPressed(){

    on_sendButton_clicked();
}


void MainWindow::dataReceived(){

    QDataStream in(m_socket);

    if(m_sizeMessage == 0) {

        if(m_socket->bytesAvailable() < (int)sizeof(quint16)){return;}

        in >> m_sizeMessage;


    }


    if(m_socket->bytesAvailable() < m_sizeMessage){return;}

    QString message;
    in >> message;

    //Gestion changement de clés serveur

    if(message.startsWith("/changeKeys")){

        m_isConnected = true;
        QStringList words(message.split(' '));

        clef tempE, tempN;

        try{
            tempE = std::stoul(words[1].toStdString());
            tempN = std::stoul(words[2].toStdString());
        }

        catch (std::exception const& e) {

            m_sizeMessage = 0;
            return;
        }

        m_eServ = tempE;
        m_nServ = tempN;

        m_sizeMessage = 0;
        return;
    }


    //Gestion du mauvais jeu de clefs
    if(message.startsWith("/badKeys")){
        newKeySet();
        m_sizeMessage = 0;
        return;
    }



    if(m_isConnected) {
        std::string stdMess(message.toStdString());

        listeMessages->append(QString::fromStdString(RSAdecrypt(stdMess, m_privee, m_e, m_n)));

    } // Si on est connecté, on affiche le message décrypté

    else{
       m_sizeMessage = 0;
       newKeySet();
     // Sinon, on recommence
    }

    m_sizeMessage = 0;

}


void MainWindow::connected(){

    QObject::connect(changeKeysButton, &QPushButton::clicked, this, &MainWindow::newKeySet);
    listeMessages->append(tr("<em>Connection succeed !</em>"));
    boutonConnexion->setEnabled(true);

    newKeySet();
    m_timer->start(60000);
}


void MainWindow::disconnected(){

    QObject::disconnect(changeKeysButton, nullptr, this, nullptr);
    listeMessages->append(tr("<em>Disconnected from server.</em>"));
    m_isConnected = false;
    m_timer->stop();

}


void MainWindow::errorSocket(QAbstractSocket::SocketError erreur){

    switch(erreur){

    case QAbstractSocket::HostNotFoundError:
        listeMessages->append(tr("<em>Error: server could not be found. Check the IP adress and port.</em>"));
        break;

    case QAbstractSocket::ConnectionRefusedError:
        listeMessages->append(tr("<em>ERROR : server declined connection. Check if server is correctly working. Check as well the IP adress and port.</em>"));
        break;


    case QAbstractSocket::RemoteHostClosedError:
                listeMessages->append(tr("<em>ERROR : server shut down the connection.</em>"));
                break;
            default:
                listeMessages->append(tr("<em>ERROR : ") + m_socket->errorString() + tr("</em>"));



    }

    boutonConnexion->setEnabled(true);
}




void MainWindow::sendServ(const QString &mess){


    QByteArray paquet;

    QDataStream out(&paquet, QIODevice::WriteOnly);


    out << (quint16)0;
    out << mess;
    out.device()->seek(0);
    out << (quint16)(paquet.size() - sizeof(quint16));

    m_socket->write(paquet);

}

void MainWindow::sendMess(const QString &mess){

    std::cerr << "message envoyé : " << mess.toStdString() << std::endl;

    QString toSend = tr("<strong>") + pseudo->text() + tr("</strong> : ") + mess;

    toSend = QString::fromStdString(RSAencrypt(toSend.toStdString(), m_eServ, m_nServ));
    std::cerr << std::endl << "message chiffré : " << toSend.toStdString() << std::endl;
    sendServ(toSend);
}


void MainWindow::newKeySet(){

    std::cerr << "nouveau jeu de clefs client";

    jeu clefs(giveKeys("../../RSA/data/liste_premiers.txt"));
    m_privee = clefs.privee;
    m_e = clefs.e;
    m_n = clefs.n;


    if (!checkKeys()) {
        newKeySet();
        return;
    }




    listeMessages->append(tr("<em>New keyset</em>"));


    QString mess("/changeKeys");
    mess += ' ';
    mess += QString::fromStdString((std::to_string(m_e)));
    mess += ' ';
    mess += QString::fromStdString((std::to_string(m_n)));


    sendServ(mess);
}


bool MainWindow::checkKeys(){

    std::string mess("test");
    mess = RSAencrypt(mess, m_e, m_n);
    mess = RSAdecrypt(mess, m_privee, m_e, m_n);

    return (mess == "test");

}
