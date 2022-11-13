#include "fenserveur.h"

FenServeur::FenServeur()
{
    m_etatServeur = new QLabel;
    m_quit = new QPushButton(tr("Quit"));

    QObject::connect(m_quit, &QPushButton::clicked, qApp, &QApplication::quit);

    m_changeKeys = new QPushButton(tr("Change Keys"));
    QObject::connect(m_changeKeys, &QPushButton::clicked, this, &FenServeur::newKeySet);

    QVBoxLayout* mainLay = new QVBoxLayout;
    mainLay->addWidget(m_etatServeur);
    mainLay->addWidget(m_changeKeys);
    mainLay->addWidget(m_quit);
    setLayout(mainLay);


    setWindowTitle(tr("Hermes"));


    m_serv = new QTcpServer(this);

    if(!m_serv->listen(QHostAddress::Any, 50885)){  // Si le serveur n'est pas correctement démarré

        m_etatServeur->setText(tr("The server was not correctly started. Error : <br />") +
                               m_serv->errorString());
    }

    else {

        m_etatServeur->setText("The server was started on port <strong>" + QString::number(m_serv->serverPort())
                + tr("</strong>. <br />Any client can now connect."));

        QObject::connect(m_serv, &QTcpServer::newConnection, this, &FenServeur::newConnection);
        }



    m_sizeMessage = 0;

    newKeySet();
    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, this, &FenServeur::newKeySet);
    m_timer->start(60000);
}


void FenServeur::newConnection(){

    sendAllCrypt(tr("<em>A new client has connected</em>"));

    QTcpSocket* newPending = m_serv->nextPendingConnection();
    m_pending << newPending;

    QObject::connect(newPending, &QTcpSocket::readyRead, this, &FenServeur::dataReceivedFromPending);
    QObject::connect(newPending, &QTcpSocket::disconnected, this, &FenServeur::deconnectionPending);

}



void FenServeur::deconnectionClient(){

    Client* client = qobject_cast<Client*>(sender());
    if(client == 0){return;}

    // Gestion des clefs utilisées
    m_usedPublicKeys.removeOne(client->getE());

    m_clients.removeOne(client);

    sendAllCrypt(tr("<em>A client has disconnected</em>"));

    client->deleteLater(); // Permet de delete le sender plus tard, sans faire bugger Qt

}

void FenServeur::deconnectionPending(){

    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(socket == 0) {return;}

    m_pending.removeOne(socket);

    socket->deleteLater();
}


void FenServeur::dataReceived(){

    Client* client(qobject_cast<Client*>(sender()));

    if(client->getSocket() == 0){
        return;
    }

    QDataStream in(client->getSocket());



    if(m_sizeMessage == 0){

        if (client->getSocket()->bytesAvailable() < (int)sizeof(quint16)){return;}

        in >> m_sizeMessage;

    }

    if(client->getSocket()->bytesAvailable() < m_sizeMessage){return;}

    QString message;
    in >> message;


    if(message.startsWith("/changeKeys")){


        QStringList words(message.split(' '));
        clef e(std::stoi(words[1].toStdString()));
        clef n(std::stoi(words[2].toStdString()));


        // Gestion des clefs déjà utilisées
        for(auto const& elt:m_usedPublicKeys){
            if(e == elt) {
                sendOne(client->getSocket(), "/badKeys");
                m_sizeMessage = 0;
                return;
            }
        }

        m_usedPublicKeys.removeOne(client->getE());

        client->setE(e);
        client->setN(n);

        m_usedPublicKeys << e;

        m_sizeMessage = 0;
        return;
    }

    std::string stdMess(message.toStdString());
    stdMess = RSAdecrypt(stdMess, m_privee, m_e, m_n);
    for (auto& client:m_clients){

        std::string cryptedMess(RSAencrypt(stdMess, client->getE(), client->getN()));
        sendOne(client->getSocket(), QString::fromStdString(cryptedMess));
    }

    m_sizeMessage = 0;

}



void FenServeur::dataReceivedFromPending(){

    std::cerr<<"data received";

    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    if(socket == 0) {return;}

    QDataStream in(socket);

    if(m_sizeMessage == 0) {

        if(socket->bytesAvailable() < (int)sizeof(quint16)){
            return;
        }
        in >> m_sizeMessage;
    }

    if(socket->bytesAvailable() < m_sizeMessage) {return;}

    QString message;

    in >> message;

    //On récupère les clefs envoyées
    QStringList words(message.split(' '));
    clef e((std::stoi(words[1].toStdString()))), n((std::stoi(words[2].toStdString())));

    for(auto& elt:m_usedPublicKeys){

        if(e == elt){sendOne(socket, "n"); m_sizeMessage=0; return;}
    }

    m_usedPublicKeys.push_back(e);

    Client* newClient = new Client(socket, e, n);

    QObject::disconnect(socket, nullptr, this, nullptr);

    QObject::connect(newClient, &Client::readyRead, this, &FenServeur::dataReceived);
    QObject::connect(newClient, &Client::disconnected, this, &FenServeur::deconnectionClient);


    m_clients << newClient;
    m_pending.removeOne(socket);

    m_sizeMessage = 0;
    sendKeys();
}



void FenServeur::sendAll(const QString &message){

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16)0;
    out << message;
    out.device()->seek(0);
    out << (quint16)(paquet.size() - sizeof(quint16)); // On écrase le 0, on inscrit la taille du message

    for (int i(0); i < m_clients.size(); i++){

        m_clients[i]->getSocket()->write(paquet);
    }


}

void FenServeur::sendAllCrypt(const QString &message){

    for (auto& elt:m_clients){

        std::string cryptMess(RSAencrypt(message.toStdString(), elt->getE(), elt->getN()));
        sendOne(elt->getSocket(), QString::fromStdString(cryptMess));
    }

}





void FenServeur::sendKeys(){

    QString mess("/changeKeys " + QString::fromStdString(std::to_string(m_e))
                 + ' ' + QString::fromStdString(std::to_string(m_n)));

    sendAll(mess);
}



void FenServeur::sendOne(QTcpSocket *socket, const QString &message){

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16)0;
    out << message;
    out.device()->seek(0);
    out << (quint16)(paquet.size() - sizeof(quint16));

    socket->write(paquet);

}

void FenServeur::newKeySet(){

    jeu clefs(giveKeys("../../RSA/data/liste_premiers.txt"));


    //Gestion des clefs utilisées
    for(auto const& elt: m_usedPublicKeys){
        if(elt == clefs.e){
            newKeySet();
            return;
        }
    }

    m_usedPublicKeys.removeOne(m_e);

    m_privee = clefs.privee;
    m_e = clefs.e;
    m_n = clefs.n;

    m_usedPublicKeys << clefs.e;

    if (!checkKeys()) {
        newKeySet();
        return;
    }

    sendKeys();

}


bool FenServeur::checkKeys(){

    std::string mess("test");
    mess = RSAencrypt(mess, m_e, m_n);
    mess = RSAdecrypt(mess, m_privee, m_e, m_n);

    return (mess == "test");
}
