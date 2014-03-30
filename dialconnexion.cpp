#include <QGridLayout>
#include <QMessageBox>

#include "Const.h"
#include "dialconnexion.h"
#include "fenetreprincipale.h"

/**
 * @brief DialConnexion::DialConnexion
 * Instancie la fenêtre de connexion.
 *
 */
DialConnexion::DialConnexion() : QWidget()
{
    this->setWindowTitle("Connexion au robot");

    this->setObjectName("fenetre");

    ip = "";
    port = -1;

    // Creation ddes elements graphiques :
    labelChoixIP = new QLabel(this);
    labelChoixPort = new QLabel(this);

    champIP = new QLineEdit(this);
    champPort = new QLineEdit(this);

    boutonSimulateur = new QPushButton(this);
    boutonReel = new QPushButton(this);
    boutonOK = new QPushButton(this);
    boutonQuitter = new QPushButton(this);

    // Layout : "mise en page" des différents éléments sous forme de "grille"
    QGridLayout *layoutGrid = new QGridLayout;

    // Donnees des elements graphiques:
    labelChoixIP->setText("IP");
    labelChoixPort->setText("Port");
    boutonSimulateur->setText("Simulateur");
    boutonReel->setText("Robot");
    boutonOK->setIcon(QIcon(":/images/icones/check.png"));
    boutonQuitter->setIcon(QIcon(":/images/icones/close_delete.png"));

    // Ajout des éléments au layout :
    layoutGrid->addWidget(labelChoixIP, 0,0);
    layoutGrid->addWidget(champIP, 0,1,1,2);
    layoutGrid->addWidget(labelChoixPort, 1,0);
    layoutGrid->addWidget(champPort, 1, 1, 1, 2);
    layoutGrid->addWidget(boutonSimulateur, 2, 1);
    layoutGrid->addWidget(boutonReel, 2, 2);
    layoutGrid->addWidget(boutonQuitter, 3, 1);
    layoutGrid->addWidget(boutonOK, 3, 2);

    // application du layout à cette fenêtre et taille fixe de la fenetre :
    this->setLayout(layoutGrid);
    this->layout()->setSizeConstraint( QLayout::SetFixedSize );

    /* Liaison entre les boutons de l'interface et les actions à effectuer :
     * Si un SIGNAL est émis, alors la fonction SLOT correspondante est exécutée. */
    QObject::connect(boutonQuitter, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(boutonOK, SIGNAL(clicked()), this, SLOT(boutonOKClicked()));
    QObject::connect(boutonSimulateur, SIGNAL(clicked()), this, SLOT(boutonSimulateurClicked()));
    QObject::connect(boutonReel, SIGNAL(clicked()), this, SLOT(boutonReelClicked()));
}

/************************************
 * SLOTS :
 ************************************/

/**
 * @brief DialConnexion::boutonOKClicked
 * Slot appelé lorsque l'utilisateur clique sur le bouton "Ok".
 * Vérifie la validité des données (champs non vides).
 * Si non valides, on affiche un message d'erreur.
 * Si valides, on ferme cette fenetre et on lance le programme principal en créant et affichant une FenetrePrincipale.
 */
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
        if(fenetre->isValid())
        {
            fenetre->show();
            this->close();
        }
        else
        {
            QMessageBox *message = new QMessageBox();
            message->setText("La connexion a échoué. Vérifiez les informations.");
            message->exec();
        }
    }
}

/**
 * @brief DialConnexion::boutonReelClicked
 * Slot appelé lorsque l'utilisateur clique sur le bouton "Robot".
 * Remplit les champs IP et port avec les données correspondant au robot.
 */
void DialConnexion::boutonReelClicked()
{
    champIP->setText(IP_REEL);
    champPort->setText(QString::number(PORT_REEL, 10));
}

/**
 * @brief DialConnexion::boutonSimulateurClicked
 * Slot appelé lorsque l'utilisateur clique sur le bouton "Simulateur".
 * Remplit les champs IP et port avec les données correspondant au simulateur.
 */
void DialConnexion::boutonSimulateurClicked()
{
    champIP->setText(IP_SIMULATEUR);
    champPort->setText(QString::number(PORT_SIMULATEUR, 10));
}
