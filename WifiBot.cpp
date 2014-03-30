
#include <QApplication>
#include <iostream>
#include <QFile>

#include "dialconnexion.h"
#include "Const.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* Application du style : */
    QFile qssFile(":/style/style.qss");
    if(qssFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        app.setStyleSheet(qssFile.readAll());
        qssFile.close();
    }

    DialConnexion fenetreConnexion;
    fenetreConnexion.show();

    return app.exec();
}
