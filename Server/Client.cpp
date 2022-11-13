#include "Client.h"


void Client::socketReadyRead(){

    emit readyRead();
}

void Client::socketDisconnected(){


    emit disconnected();
}
