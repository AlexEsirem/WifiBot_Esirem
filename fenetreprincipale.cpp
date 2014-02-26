#include <QGridLayout>
#include <QFont>
#include <QPalette>
#include <QKeyEvent>

#include "fenetreprincipale.h"
#include "Const.h"

int commande;

FenetrePrincipale::FenetrePrincipale(QString ipRobot, int portRobot) : QWidget()
{
    this->setWindowTitle("WifiBot Control");

    ip = ipRobot;
    port = portRobot;

    /* Initialisation des elements de fonctionnement : */
    commande = RIEN;

    /* Lancement du thread de communication : */
    tCommunication = new ThreadCommunication(ip, port);


    tCommunication->start();

    /* Initialisation des flags des touchess clavier : */
    keyUp = false;
    keyDown = false;
    keyLeft = false;
    keyRight = false;

    /* Initialisation des elements graphiques : */

    // Creation des elements :
    boutonAccelerer = new QPushButton(this);
    boutonFrein = new QPushButton(this);
    boutonGauche = new QPushButton(this);
    boutonDroite = new QPushButton(this);

    labelDirection = new QLabel(this);
    choixDirection = new QSlider(Qt::Vertical, this);

    labelVitesseAvantGauche = new QLabel(this);
    labelVitesseArriereGauche = new QLabel(this);
    labelVitesseAvantDroit = new QLabel(this);
    labelVitesseArriereDroit = new QLabel(this);
    labelTensionBatterie = new QLabel(this);
    labelIRGauche = new QLabel(this);
    labelIRDroit = new QLabel(this);
    labelIR = new QLabel(this);
    labelVitesses = new QLabel(this);

    boutonQuitter = new QPushButton(this);

    // Donnees des elements :
    boutonAccelerer->setText("Accelerer");
    boutonFrein->setText("Frein");
    boutonGauche->setText("Gauche");
    boutonDroite->setText("Droite");
    boutonQuitter->setText("Quitter");

    boutonAccelerer->setFixedSize(100,40);
    boutonFrein->setFixedSize(100,40);
    boutonGauche->setFixedSize(100,40);
    boutonDroite->setFixedSize(100,40);
    boutonQuitter->setFixedSize(100,40);

    labelDirection->setText("Avant");
    choixDirection->setMinimum(0);
    choixDirection->setMaximum(1);
    choixDirection->setTickInterval(1);
    choixDirection->setValue(1);
    choixDirection->setFixedSize(30,40);

    QFont police("Trebuchet MS", 14, QFont::Bold);
    labelVitesses->setFont(police);
    labelVitesses->setText("Vitesses");
    labelVitesseAvantGauche->setText("Avant gauche :");
    labelVitesseArriereGauche->setText("Arriere gauche :");
    labelVitesseAvantDroit->setText("Avant droit :");
    labelVitesseArriereDroit->setText("Arriere droit :");
    labelTensionBatterie->setText("Batterie :");
    labelIR->setFont(police);
    labelIR->setText("Infrarouges");
    labelIRGauche->setText("Gauche :");
    labelIRDroit->setText("Droit :");

    QGridLayout *layoutCommandes = new QGridLayout;
    QGridLayout *layoutPrincipal = new QGridLayout;
    QGridLayout *layoutDroite = new QGridLayout;
    QGridLayout *layoutData = new QGridLayout;

    QFrame* separateurH1 = new QFrame(this);
    separateurH1->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    separateurH1->show();

    QFrame* separateurH2 = new QFrame(this);
    separateurH2->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    separateurH2->show();

    layoutData->setVerticalSpacing(10);
    layoutData->setHorizontalSpacing(30);

    layoutData->addWidget(labelVitesses, 0, 0, Qt::AlignRight);
    layoutData->addWidget(labelVitesseAvantGauche, 0, 1);
    layoutData->addWidget(labelVitesseAvantDroit, 0, 2);
    layoutData->addWidget(labelVitesseArriereGauche, 1, 1);
    layoutData->addWidget(labelVitesseArriereDroit, 1, 2);
    layoutData->addWidget(labelIR, 2, 0, Qt::AlignRight);
    layoutData->addWidget(labelIRGauche, 2, 1);
    layoutData->addWidget(labelIRDroit, 2, 2);
    layoutData->addWidget(labelTensionBatterie, 3, 0, 1, 2);

    layoutCommandes->addWidget(boutonAccelerer, 1, 1);
    layoutCommandes->addWidget(boutonGauche, 2, 0);
    layoutCommandes->addWidget(boutonFrein, 2, 1);
    layoutCommandes->addWidget(boutonDroite, 2, 2);
    layoutCommandes->addWidget(labelDirection, 0, 3, Qt::AlignCenter);
    layoutCommandes->addWidget(choixDirection, 1, 3, 2,1);

    layoutDroite->setVerticalSpacing(30);

    layoutDroite->addLayout(layoutCommandes,0,0, Qt::AlignRight|Qt::AlignTop);
    layoutDroite->addWidget(separateurH1, 1, 0);
    layoutDroite->addLayout(layoutData, 2, 0, Qt::AlignRight);
    layoutDroite->addWidget(separateurH2, 3, 0);
    layoutDroite->addWidget(boutonQuitter, 4,0, Qt::AlignBottom|Qt::AlignRight);

    layoutPrincipal->addLayout(layoutDroite, 0,1);

    this->setLayout(layoutPrincipal);

    // Liaison boutons avec les actions :
    QObject::connect(boutonQuitter, SIGNAL(clicked()), this, SLOT(boutonQuitterClicked()));
    QObject::connect(boutonAccelerer, SIGNAL(pressed()), this, SLOT(boutonAccelererPressed()));
    QObject::connect(boutonAccelerer, SIGNAL(released()), this, SLOT(boutonReleased()));
    QObject::connect(boutonFrein, SIGNAL(pressed()), this, SLOT(boutonFreinPressed()));
    QObject::connect(boutonFrein, SIGNAL(released()), this, SLOT(boutonReleased()));
    QObject::connect(boutonGauche, SIGNAL(pressed()), this, SLOT(boutonGauchePressed()));
    QObject::connect(boutonGauche, SIGNAL(released()), this, SLOT(boutonReleased()));
    QObject::connect(boutonDroite, SIGNAL(pressed()), this, SLOT(boutonDroitePressed()));
    QObject::connect(boutonDroite, SIGNAL(released()), this, SLOT(boutonReleased()));

    // Liaison de la modification des donnees du robot (thread communication) avec l'update des labels :
    QObject::connect(tCommunication, SIGNAL(sensorDataChanged()), this, SLOT(updateDataLabels()));


}

/************************
 * Fonctions liées aux evenements clavier :
 ***********************/

/**
 * @brief FenetrePrincipale::keyPressEvent
 * @param event
 * Fonction appelee lors d'un appui sur une touche du clavier.
 * On met a jour les flags (key... = true)
 * Puis on met a jour la commande en fonction des flags.
 */
void FenetrePrincipale::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        keyUp = true;
        break;

    case Qt::Key_Down:
        keyDown = true;
        break;

    case Qt::Key_Left:
        keyLeft = true;
        break;

    case Qt::Key_Right:
       keyRight = true;
        break;

    case Qt::Key_Escape:
        boutonQuitterClicked();
        break;

    default:
        break;
    }

    // maj de la commande :
    majCommandeFromFlags();
}

/**
 * @brief FenetrePrincipale::keyReleaseEvent
 * @param event
 * Fonction appelee lorsqu'une touche du clavier est relachee
 * On met a jour les flags clavier
 * Puis on met a jour la commande en fonction des flags.
 */
void FenetrePrincipale::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        keyUp = false;
        break;

    case Qt::Key_Down:
        keyDown = false;
        break;

    case Qt::Key_Left:
        keyLeft = false;
        break;

    case Qt::Key_Right:
       keyRight = false;
        break;

    default:
        break;
    }

    // maj de la commande :
    majCommandeFromFlags();
}

/**
 * @brief FenetrePrincipale::majCommandeFromFlags
 * Met a jour la commande en fonction des flags clavier.
 */
void FenetrePrincipale::majCommandeFromFlags()
{
    // On applique les changements a la commande, en fonction des flags :
    if(keyUp && !keyDown)   // si touche du haut appuyee mais pas celle du bas
    {
        if(keyLeft && !keyRight)    // avant gauche (droite non enfoncee)
            commande = AVANT_GAUCHE;
        else if(keyRight && !keyLeft)
            commande = AVANT_DROIT;
        else
            commande = AVANCER;
    }
    else if(keyDown && !keyUp)
    {
        if(keyLeft && !keyRight)
            commande = ARRIERE_GAUCHE;
        else if(keyRight && !keyLeft)
            commande = ARRIERE_DROIT;
        else
            commande = RECULER;
    }
    else
        commande = RIEN;
}


/***************************
 * SLOTS : actions effectuees lorsqu'un evemenement a lieu
 ***************************/

void FenetrePrincipale::updateDataLabels()
{
    labelVitesseAvantGauche->setText("Avant gauche : " + QString::number(tCommunication->getCapteurs()->getVitesseAvantGauche(), 10));
    labelVitesseAvantDroit->setText("Avant droit : " + QString::number(tCommunication->getCapteurs()->getVitesseAvantDroit(), 10));
    labelVitesseArriereGauche->setText("Arriere gauche : " + QString::number(tCommunication->getCapteurs()->getVitesseArriereGauche(), 10));
    labelVitesseArriereDroit->setText("Arriere droit : " + QString::number(tCommunication->getCapteurs()->getVitesseArriereDroit(), 10));
    labelIRDroit->setText("Droit : " + QString::number(tCommunication->getCapteurs()->getIRdroit(), 10));
    labelIRGauche->setText("Gauche : " + QString::number(tCommunication->getCapteurs()->getIRgauche(), 10));
    labelTensionBatterie->setText("Batterie : " + QString::number(tCommunication->getCapteurs()->getTensionBatterie(), 10));
}

/**
 * @brief FenetrePrincipale::boutonReleased
 * Appele lorsqu'un bouton de commande est relache.
 * Met la commande avec vitesses = 0 et sens = 1 (initial)
 */
void FenetrePrincipale::boutonReleased()
{
    commande = RIEN;
    // Envoyer cette donnee au thread d'envoi
}

/**
 * @brief FenetrePrincipale::boutonAccelererPressed
 * Appui sur le bouton accelerer : modification de la commande.
 */
void FenetrePrincipale::boutonAccelererPressed()
{
    if(choixDirection->value() == 1)
        commande = AVANCER;
    else
        commande = RECULER;
}

/**
 * @brief FenetrePrincipale::boutonDroitePressed
 */
void FenetrePrincipale::boutonDroitePressed()
{
    if(choixDirection->value() == 1)
        commande = AVANT_DROIT;
    else
        commande = ARRIERE_DROIT;
}

/**
 * @brief FenetrePrincipale::boutonGauchePressed
 */
void FenetrePrincipale::boutonGauchePressed()
{
    if(choixDirection->value() == 1)
        commande = AVANT_GAUCHE;
    else
        commande = ARRIERE_GAUCHE;
}

/**
 * @brief FenetrePrincipale::boutonFreinPressed
 */
void FenetrePrincipale::boutonFreinPressed()
{
    commande = FREIN;
}


/**
 * Quitte le programme lorsque le bouton quitter est clique
 * Ferme d'abord les threads et le socket TCP.
 * Attend la fermeture effective des thread et socket.
 */
void FenetrePrincipale::boutonQuitterClicked()
{
    // Fermer les threads et attendre leur fin (join())
    tCommunication->terminate();
    tCommunication->wait(2000);
    // Ferme la fenetre :
    close();
}





