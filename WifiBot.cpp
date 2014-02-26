
#include <QApplication>
#include <iostream>

#include "dialconnexion.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DialConnexion fenetreConnexion;

    fenetreConnexion.show();

    return app.exec();
}
