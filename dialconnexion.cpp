

#include <QGridLayout>
#include <QMessageBox>

#include "Const.h"
#include "dialconnexion.h"
#include "fenetreprincipale.h"

DialConnexion::DialConnexion() : QWidget()
{
    this->setWindowTitle("Connexion au robot");

    ip = "";
    port = -1;

    // Creation ddes elements :
    labelChoixIP = new QLabel(this);
    labelChoixPort = new QLabel(this);

    champIP = new QLineEdit(this);
    champPort = new QLineEdit(this);

    boutonSimulateur = new QPushButton(this);
    boutonReel = new QPushButton(this);
    boutonOK = new QPushButton(this);
    boutonQuitter = new QPushButton(this);

    QGridLayout *layoutGrid = new QGridLayout;

    // Donnees des elements :
    labelChoixIP->setText("IP");
    labelChoixPort->setText("Port");
    boutonSimulateur->setText("Simulateur");
    boutonReel->setText("Robot");
    boutonOK->setText("OK");
    boutonQuitter->setText("Quitter");

    layoutGrid->addWidget(labelChoixIP, 0,0);
    layoutGrid->addWidget(champIP, 0,1,1,2);
    layoutGrid->addWidget(labelChoixPort, 1,0);
    layoutGrid->addWidget(champPort, 1, 1, 1, 2);
    layoutGrid->addWidget(boutonSimulateur, 2, 1);
    layoutGrid->addWidget(boutonReel, 2, 2);
    layoutGrid->addWidget(boutonQuitter, 3, 0);
    layoutGrid->addWidget(boutonOK, 3, 2);

    this->setLayout(layoutGrid);

    // Liaison entre les boutons et les actions :
    QObject::connect(boutonQuitter, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(boutonOK, SIGNAL(clicked()), this, SLOT(boutonOKClicked()));
    QObject::connect(boutonSimulateur, SIGNAL(clicked()), this, SLOT(boutonSimulateurClicked()));
    QObject::connect(boutonReel, SIGNAL(clicked()), this, SLOT(boutonReelClicked()));
}

void DialConnexion::boutonOKClicked()
{
    if(champIP->text().isEmpty() && champPort->text().toInt(0, 10) == 0)
    {
        QMessageBox *message = new QMessageBox();
        message->setText("Veuillez entrer les informations.");
        message->exec();
    }
    else if(champIP->text().isEmpty())
    {
        QMessageBox *message = new QMessageBox();
        message->setText("Veuillez entrer l'adresse ip.");
        message->exec();
    }
    else if(champPort->text().toInt(0, 10) == 0)
    {
        QMessageBox *message = new QMessageBox();
        message->setText("Veuillez entrer le port.");
        message->exec();
    }
    else
    {
        /*
         * Fermer ce dialogue et afficher la fenetre principale
         */
        FenetrePrincipale *fenetre = new FenetrePrincipale(champIP->text(), champPort->text().toInt(0, 10));
        fenetre->show();
        this->close();
    }
}

void DialConnexion::boutonReelClicked()
{
    champIP->setText(IP_REEL);
    champPort->setText(QString::number(PORT_REEL, 10));
}

void DialConnexion::boutonSimulateurClicked()
{
    champIP->setText(IP_SIMULATEUR);
    champPort->setText(QString::number(PORT_SIMULATEUR, 10));
}
