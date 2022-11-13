#include <QApplication>
#include "fenserveur.h"

int main(int argc, char* argv[]){

    srand(time(NULL));

    QApplication app(argc, argv);

    FenServeur fenetre;
    fenetre.show();


    return app.exec();

}
