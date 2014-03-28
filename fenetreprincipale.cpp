#include <QGridLayout>
#include <QFont>
#include <QPalette>
#include <QKeyEvent>

#include "fenetreprincipale.h"
#include "Const.h"

/* L'entier commande est utilisé en global.
 * Il est partagé entre la FenetrePrincipale qui le modifiera en fonction des
 * actions de l'utilisateur, et le threadCommunication qui lira sa valeur pour savoir
 * quelles instructions envoyer au robot. */
int commande;


/**
 * @brief FenetrePrincipale::FenetrePrincipale
 * @param ipRobot
 * @param portRobot
 * Instancie une fenetre principale avec les données de connexions choisies par l'utilisateur, transmises par le
 * dialogue de connexion.
 * Créé et lance le thread de communication.
 */
FenetrePrincipale::FenetrePrincipale(QString ipRobot, int portRobot) : QWidget()
{
    this->setWindowTitle("WifiBot Control");

    QFont police("Trebuchet MS", 12);
    this->setFont(police);

    /* Initialisation des elements de fonctionnement : */
    commande = RIEN;    // initialement aucune commande n'est demandé, donc RIEN
    ip = ipRobot;
    port = portRobot;

    /* Création et lancement du thread de communication : */
    tCommunication = new ThreadCommunication(ip, port);
    tCommunication->start();

    /* Initialisation des flags des touchess clavier. On considère qu'aucune n'est appuyée au début. */
    keyUp = false;
    keyDown = false;
    keyLeft = false;
    keyRight = false;

    /* Création des elements graphiques : */
    boutonAccelerer = new QPushButton(this);
    boutonReculer = new QPushButton(this);
    boutonGauche = new QPushButton(this);
    boutonDroite = new QPushButton(this);
    boutonAvantDroite = new QPushButton(this);
    boutonArriereDroite = new QPushButton(this);
    boutonAvantGauche = new QPushButton(this);
    boutonArriereGauche = new QPushButton(this);

    labelVitesseGauche = new QLabel(this);
    labelVitesseDroite = new QLabel(this);
    labelTensionBatterie = new QLabel(this);
    labelIRGauche = new QLabel(this);
    labelIRGauche2 = new QLabel(this);
    labelIRDroit = new QLabel(this);
    labelIRDroit2 = new QLabel(this);
    labelIR = new QLabel(this);
    labelVitesses = new QLabel(this);
    labelCourant = new QLabel(this);

    boutonQuitter = new QPushButton(this);

    /* Donnees des elements graphiques */
    boutonAccelerer->setText("Accelerer");
    boutonReculer->setText("Reculer");
    boutonGauche->setText("Gauche");
    boutonDroite->setText("Droite");
    boutonAvantDroite->setText("Av.Droite");
    boutonArriereDroite->setText("Ar.Droite");
    boutonAvantGauche->setText("Av.Gauche");
    boutonArriereGauche->setText("Ar.Gauche");
    boutonQuitter->setText("Quitter");

    boutonAccelerer->setFixedSize(100,40);
    boutonReculer->setFixedSize(100,40);
    boutonGauche->setFixedSize(100,40);
    boutonDroite->setFixedSize(100,40);
    boutonAvantDroite->setFixedSize(100,40);
    boutonArriereDroite->setFixedSize(100,40);
    boutonAvantGauche->setFixedSize(100,40);
    boutonArriereGauche->setFixedSize(100,40);
    boutonQuitter->setFixedSize(100,40);

    QFont policeGras("Trebuchet MS", 14, QFont::Bold);
    labelVitesses->setFont(policeGras);
    labelVitesses->setText("Vitesses");
    labelVitesseGauche->setText("Gauche :");
    labelVitesseDroite->setText("Droite :");
    labelTensionBatterie->setText("Batterie :");
    labelIR->setFont(policeGras);
    labelIR->setText("Infrarouges");
    labelIRGauche->setText("Gauche :");
    labelIRGauche2->setText("Gauche 2 :");
    labelIRDroit->setText("Droite :");
    labelIRDroit2->setText("Droite 2 :");
    labelCourant->setText("Courant :");

    /* Elements de page Web pour la camera : */
    pageWeb = new QWebView(this);
    pageWeb->setFocusPolicy(Qt::NoFocus);
    pageWeb->load(QUrl("http://qt-project.org"));

    /* On utilisera 4 layouts :
     * - Un principal qui contiendra tous les éléments
     * - Un de droite qui contiendra les layouts commandes et data
     * - un layout commandes qui regroupe les boutons
     * - un layout data qui regroupe les éléments d'affichage des données des capteurs */
    QGridLayout *layoutCommandes = new QGridLayout;
    QGridLayout *layoutPrincipal = new QGridLayout;
    QGridLayout *layoutDroite = new QGridLayout;
    QGridLayout *layoutData = new QGridLayout;

    /* Deux séparateurs pour l'esthétique */
    QFrame* separateurH1 = new QFrame(this);
    separateurH1->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    separateurH1->show();

    QFrame* separateurH2 = new QFrame(this);
    separateurH2->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    separateurH2->show();

    /* Configuration des layout et ajout des éléments */
    layoutData->setVerticalSpacing(10);
    layoutData->setHorizontalSpacing(30);

    layoutData->addWidget(labelVitesses, 0, 0, Qt::AlignRight);
    layoutData->addWidget(labelVitesseGauche, 0, 1);
    layoutData->addWidget(labelVitesseDroite, 1, 1);
    layoutData->addWidget(labelIR, 2, 0, Qt::AlignRight);
    layoutData->addWidget(labelIRGauche, 2, 1);
    layoutData->addWidget(labelIRGauche2, 2, 2);
    layoutData->addWidget(labelIRDroit, 3, 1);
    layoutData->addWidget(labelIRDroit2, 3, 2);
    layoutData->addWidget(labelTensionBatterie, 4, 0, 1, 2);
    layoutData->addWidget(labelCourant, 5, 0);

    layoutCommandes->addWidget(boutonAvantGauche, 1, 0);
    layoutCommandes->addWidget(boutonAccelerer, 1, 1);
    layoutCommandes->addWidget(boutonAvantDroite, 1, 2);
    layoutCommandes->addWidget(boutonGauche, 2, 0);
    layoutCommandes->addWidget(boutonDroite, 2, 2);
    layoutCommandes->addWidget(boutonArriereGauche, 3, 0);
    layoutCommandes->addWidget(boutonReculer, 3, 1);
    layoutCommandes->addWidget(boutonArriereDroite, 3, 2);

    layoutDroite->setVerticalSpacing(30);

    layoutDroite->addLayout(layoutCommandes,0,0, Qt::AlignRight|Qt::AlignTop);
    layoutDroite->addWidget(separateurH1, 1, 0);
    layoutDroite->addLayout(layoutData, 2, 0, Qt::AlignLeft);
    layoutDroite->addWidget(separateurH2, 3, 0);
    layoutDroite->addWidget(boutonQuitter, 4,0, Qt::AlignBottom|Qt::AlignRight);

    layoutPrincipal->addLayout(layoutDroite, 0,1);

    layoutPrincipal->addWidget(pageWeb, 0,0);

    /* Application du layout à cette fenêtre */
    this->setLayout(layoutPrincipal);






    /* Liaison entre les boutons de l'interface et les actions à effectuer :
     * Si un SIGNAL est émis, alors la fonction SLOT correspondante est exécutée. */
    QObject::connect(boutonQuitter, SIGNAL(clicked()), this, SLOT(boutonQuitterClicked()));
    QObject::connect(boutonAccelerer, SIGNAL(pressed()), this, SLOT(boutonAccelererPressed()));
    QObject::connect(boutonAccelerer, SIGNAL(released()), this, SLOT(boutonAccelererReleased()));
    QObject::connect(boutonReculer, SIGNAL(pressed()), this, SLOT(boutonReculerPressed()));
    QObject::connect(boutonReculer, SIGNAL(released()), this, SLOT(boutonReculerReleased()));
    QObject::connect(boutonGauche, SIGNAL(pressed()), this, SLOT(boutonGauchePressed()));
    QObject::connect(boutonGauche, SIGNAL(released()), this, SLOT(boutonGaucheReleased()));
    QObject::connect(boutonDroite, SIGNAL(pressed()), this, SLOT(boutonDroitePressed()));
    QObject::connect(boutonDroite, SIGNAL(released()), this, SLOT(boutonDroiteReleased()));
    QObject::connect(boutonAvantGauche, SIGNAL(pressed()), this, SLOT(boutonAvantGauchePressed()));
    QObject::connect(boutonAvantGauche, SIGNAL(released()), this, SLOT(boutonAvantGaucheReleased()));
    QObject::connect(boutonAvantDroite, SIGNAL(pressed()), this, SLOT(boutonAvantDroitePressed()));
    QObject::connect(boutonAvantDroite, SIGNAL(released()), this, SLOT(boutonAvantDroiteReleased()));
    QObject::connect(boutonArriereGauche, SIGNAL(pressed()), this, SLOT(boutonArriereGauchePressed()));
    QObject::connect(boutonArriereGauche, SIGNAL(released()), this, SLOT(boutonArriereGaucheReleased()));
    QObject::connect(boutonArriereDroite, SIGNAL(pressed()), this, SLOT(boutonArriereDroitePressed()));
    QObject::connect(boutonArriereDroite, SIGNAL(released()), this, SLOT(boutonArriereDroiteReleased()));

    /* Liaison de la modification des donnees du robot (dans le thread communication) avec l'update des labels */
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
    /* On applique les changements a la commande, en fonction des flags */
    // Si la touche du haut est enfoncée et pas celle du bas :
    if(keyUp && !keyDown)
    {
        if(keyLeft && !keyRight)
            commande = AVANT_GAUCHE;
        else if(keyRight && !keyLeft)
            commande = AVANT_DROIT;
        else
            commande = AVANCER;
    }
    // Si la touche du bas est enfoncée et pas celle du haut :
    else if(keyDown && !keyUp)
    {
        if(keyLeft && !keyRight)
            commande = ARRIERE_GAUCHE;
        else if(keyRight && !keyLeft)
            commande = ARRIERE_DROIT;
        else
            commande = RECULER;
    }
    // Si ni la touche de haut ni celle du bas ne sont enfoncée (le robot tourne sur lui-même) :
    else if(!keyUp && !keyDown)
    {
        if(keyLeft && !keyRight)
            commande = PIVOTER_GAUCHE;
        else if(keyRight && !keyLeft)
            commande = PIVOTER_DROITE;
        else
            commande = RIEN;
    }
    else
        commande = RIEN;
}


/***************************
 * SLOTS : actions effectuees lorsqu'un evemenement a lieu
 ***************************/

/**
 * @brief FenetrePrincipale::updateDataLabels
 * Slot appelé lorsque le thread de communication a recu de nouvelles données du robot.
 * Met à jour les labels de l'interface pour afficher les infos.
 */
void FenetrePrincipale::updateDataLabels()
{
    SensorData *capteurs = tCommunication->getCapteurs();
    labelVitesseGauche->setText("Gauche : " + QString::number(capteurs->getVitesseGauche(), 10));
    labelVitesseDroite->setText("Droite : " + QString::number(capteurs->getVitesseDroite(), 10));
    labelIRDroit->setText("Droite : " + QString::number(capteurs->getIRdroit(), 10));
    labelIRDroit2->setText("Droite 2 : " + QString::number(capteurs->getIRdroit2(), 10));
    labelIRGauche->setText("Gauche : " + QString::number(capteurs->getIRgauche(), 10));
    labelIRGauche2->setText("Gauche 2 : " + QString::number(capteurs->getIRgauche2(), 10));
    labelTensionBatterie->setText("Batterie : " + QString::number(capteurs->getTensionBatterie(), 10));
    labelCourant->setText("Courant : " + QString::number(capteurs->getCourant(), 10));
}

/**
 * @brief FenetrePrincipale::boutonAccelererPressed
 * Appui sur le bouton accelerer : modification de la commande.
 */
void FenetrePrincipale::boutonAccelererPressed()
{
    keyUp = true;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonAccelererReleased()
{
    keyUp = false;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonReculerPressed()
{
    keyDown = true;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonReculerReleased()
{
    keyDown = false;
    majCommandeFromFlags();
}

/**
 * @brief FenetrePrincipale::boutonDroitePressed
 */
void FenetrePrincipale::boutonDroitePressed()
{
    keyRight = true;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonDroiteReleased()
{
    keyRight = false;
    majCommandeFromFlags();
}

/**
 * @brief FenetrePrincipale::boutonGauchePressed
 */
void FenetrePrincipale::boutonGauchePressed()
{
    keyLeft = true;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonGaucheReleased()
{
    keyLeft = false;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonAvantGauchePressed()
{
    keyUp = true;
    keyLeft = true;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonAvantGaucheReleased()
{
    keyUp = false;
    keyLeft = false;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonAvantDroitePressed()
{
    keyUp = true;
    keyRight = true;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonAvantDroiteReleased()
{
    keyUp = false;
    keyRight = false;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonArriereGauchePressed()
{
    keyDown = true;
    keyLeft = true;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonArriereGaucheReleased()
{
    keyDown = false;
    keyLeft = false;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonArriereDroitePressed()
{
    keyDown = true;
    keyRight = true;
    majCommandeFromFlags();
}

void FenetrePrincipale::boutonArriereDroiteReleased()
{
    keyDown = false;
    keyRight = false;
    majCommandeFromFlags();
}




/**
 * Quitte le programme lorsque le bouton quitter est clique
 * L'appel a terminate() permet au thread de communication de fermer son socket et de se terminer proprement.
 * On attend qu'il soit terminé pendant maximum 5 secondes puis on ferme cette fenetre.
 * L'application se termine alors.
 */
void FenetrePrincipale::boutonQuitterClicked()
{
    // Fermer les threads et attendre leur fin (join())
    tCommunication->terminate();
    tCommunication->wait(5000);
    // Ferme la fenetre :
    close();
}





