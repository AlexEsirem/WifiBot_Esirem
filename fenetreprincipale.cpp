#include <QGridLayout>
#include <QFont>
#include <QPalette>
#include <QKeyEvent>
#include <QWebFrame>
#include <QMessageBox>

#include "fenetreprincipale.h"
#include "Const.h"
#include "dialconnexion.h"

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
    this->setWindowTitle("Contrôle du WifiBot");

    /* Initialisation des elements de fonctionnement : */
    commande = RIEN;    // initialement aucune commande n'est demandée
    ip = ipRobot;
    port = portRobot;

    /* Création et lancement du thread de communication : */
    tCommunication = new ThreadCommunication(ip, port);
    tCommunication->start();

    while(tCommunication->getConnecte() == 0)
    {
        QMutex mutex;
        mutex.lock();
        QWaitCondition waitCondition;
        waitCondition.wait(&mutex, 50);
    }

    if(tCommunication->getConnecte() == 1)
    {
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
        boutonCapture = new QPushButton(this);
        boutonOptions = new QPushButton(this);

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
        labelAutres = new QLabel(this);

        boutonQuitter = new QPushButton(this);

        panelCommandes = new QFrame(this);
        panelData = new QFrame(this);
        panelCamera = new QFrame(this);

        /* Donnees des elements graphiques */
        boutonAccelerer->setIcon(QIcon(":/images/icones/arrow_up.png"));
        boutonReculer->setIcon(QIcon(":/images/icones/arrow_down.png"));
        boutonGauche->setIcon(QIcon(":/images/icones/arrow_left.png"));
        boutonDroite->setIcon(QIcon(":/images/icones/arrow_right.png"));
        boutonAvantDroite->setIcon(QIcon(":/images/icones/arrow_right-up.png"));
        boutonArriereDroite->setIcon(QIcon(":/images/icones/arrow_right-down.png"));
        boutonAvantGauche->setIcon(QIcon(":/images/icones/arrow_left-up.png"));
        boutonArriereGauche->setIcon(QIcon(":/images/icones/arrow_left-down.png"));
        boutonQuitter->setIcon(QIcon(":/images/icones/close_delete.png"));
        boutonCapture->setIcon(QIcon(":/images/icones/save_as.png"));
        boutonOptions->setIcon(QIcon(":/images/icones/options.png"));

        boutonQuitter->setToolTip("Quitter le programme");
        boutonCapture->setToolTip("Prendre une capture d'écran");
        boutonOptions->setToolTip("Changer les informations de connexion");

        labelVitesses->setText("Vitesses");
        labelVitesseGauche->setText("Gauche :");
        labelVitesseDroite->setText("Droite :");
        labelTensionBatterie->setText("Batterie :");
        labelIR->setText("Infrarouges");
        labelIRGauche->setText("Gauche :");
        labelIRGauche2->setText("Gauche 2 :");
        labelIRDroit->setText("Droite :");
        labelIRDroit2->setText("Droite 2 :");
        labelCourant->setText("Courant :");
        labelAutres->setText("Autres");

        /* Elements de page Web pour la camera : */
        pageWeb = new QWebView(this);
        pageWeb->setFocusPolicy(Qt::NoFocus);
        pageWeb->load(QUrl("http://google.com"));

        QFrame *frameWeb = new QFrame(this);
        QGridLayout *layoutWeb = new QGridLayout;
        layoutWeb->addWidget(pageWeb);
        layoutWeb->setContentsMargins(0,0,0,0);
        frameWeb->setLayout(layoutWeb);
        frameWeb->setObjectName("panel");

        /* Désactivation de la barre de défilement  de la page web : */
        pageWeb->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
        pageWeb->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

        /* On utilisera 4 layouts :
         * - Un principal qui contiendra tous les éléments
         * - Un de droite qui contiendra les layouts commandes et data
         * - un layout commandes qui regroupe les boutons
         * - un layout data qui regroupe les éléments d'affichage des données des capteurs */
        QGridLayout *layoutCommandes = new QGridLayout;
        QGridLayout *layoutPrincipal = new QGridLayout;
        QGridLayout *layoutDroite = new QGridLayout;
        QGridLayout *layoutData = new QGridLayout;
        QGridLayout *layoutCamera = new QGridLayout;

        /* Noms des objets pour le style QSS : */
        this->setObjectName("fenetre");
        panelCommandes->setObjectName("panel");
        panelData->setObjectName("panel");
        panelCamera->setObjectName("panel");
        labelVitesses->setObjectName("gras");
        labelIR->setObjectName("gras");
        labelAutres->setObjectName("gras");

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
        layoutData->addWidget(labelVitesseDroite, 0, 2);
        layoutData->addWidget(labelIR, 1, 0, Qt::AlignRight);
        layoutData->addWidget(labelIRGauche, 1, 1);
        layoutData->addWidget(labelIRGauche2, 1, 2);
        layoutData->addWidget(labelIRDroit, 2, 1);
        layoutData->addWidget(labelIRDroit2, 2, 2);
        layoutData->addWidget(labelAutres, 3, 0, Qt::AlignRight);
        layoutData->addWidget(labelTensionBatterie, 3, 1);
        layoutData->addWidget(labelCourant, 3, 2);

        layoutCommandes->addWidget(boutonAvantGauche, 1, 0);
        layoutCommandes->addWidget(boutonAccelerer, 1, 1);
        layoutCommandes->addWidget(boutonAvantDroite, 1, 2);
        layoutCommandes->addWidget(boutonGauche, 2, 0);
        layoutCommandes->addWidget(boutonDroite, 2, 2);
        layoutCommandes->addWidget(boutonArriereGauche, 3, 0);
        layoutCommandes->addWidget(boutonReculer, 3, 1);
        layoutCommandes->addWidget(boutonArriereDroite, 3, 2);

        layoutCamera->addWidget(boutonCapture, 0, 0, Qt::AlignTop|Qt::AlignLeft);

        layoutDroite->setVerticalSpacing(30);

        panelCamera->setLayout(layoutCamera);
        panelData->setLayout(layoutData);
        panelCommandes->setLayout(layoutCommandes);

        panelCamera->setMinimumSize(panelCommandes->sizeHint());

        layoutDroite->addWidget(panelCamera,0,0, Qt::AlignLeft|Qt::AlignTop);
        layoutDroite->addWidget(panelCommandes,0,1, Qt::AlignRight|Qt::AlignTop);
        layoutDroite->addWidget(separateurH1, 1, 0, 1, 2);
        layoutDroite->addWidget(panelData, 2, 0, 1, 2, Qt::AlignCenter);
        layoutDroite->addWidget(separateurH2, 3, 0, 1, 2);
        layoutDroite->addWidget(boutonOptions, 4, 0, Qt::AlignBottom|Qt::AlignLeft);
        layoutDroite->addWidget(boutonQuitter, 4, 1, Qt::AlignBottom|Qt::AlignRight);

        panelData->setMinimumWidth(2*panelCommandes->sizeHint().width()+20);
        frameWeb->setMinimumWidth((4/3)*frameWeb->sizeHint().height());

        layoutPrincipal->addLayout(layoutDroite, 0,1);
        layoutPrincipal->addWidget(frameWeb, 0,0);

        /* Application du layout à cette fenêtre */
        this->setLayout(layoutPrincipal);

        /* Liaison entre les boutons de l'interface et les actions à effectuer :
         * Si un SIGNAL est émis, alors la fonction SLOT correspondante est exécutée. */
        QObject::connect(boutonQuitter, SIGNAL(clicked()), this, SLOT(boutonQuitterClicked()));
        QObject::connect(boutonOptions, SIGNAL(clicked()), this, SLOT(boutonOptionsClicked()));

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

        QObject::connect(boutonCapture, SIGNAL(clicked()), this, SLOT(boutonCaptureClicked()));

        /* Liaison de la modification des donnees du robot (dans le thread communication) avec l'update des labels */
        QObject::connect(tCommunication, SIGNAL(sensorDataChanged()), this, SLOT(updateDataLabels()));

        /* La création s'est passée sans problème */
        valid = true;
    }
    else
    {
        /* il y a eu un problème de connexion */
        valid = false;
    }
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

void FenetrePrincipale::boutonCaptureClicked()
{
    //pageWeb->page()->setViewportSize(pageWeb->page()->currentFrame()->contentsSize());
    QImage img(pageWeb->page()->viewportSize(), QImage::Format_ARGB32);
    QPainter paintView(&img);
    pageWeb->page()->currentFrame()->render(&paintView);
    paintView.end();

    QString imgFileName = QDir::homePath() + "/Desktop/Wifibot_capture_" + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh'h'mm'm'ss's'") + ".jpg";
    img.save(imgFileName);

    QMessageBox *message = new QMessageBox();
    message->setText("Image enregistrée sous : " + imgFileName);
    message->setWindowModality(Qt::NonModal);
    message->show();
}

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
 * @brief FenetrePrincipale::boutonOptionsClicked
 * Ferme la fenetre et le thread de communication
 * Ouvre un nouveau dialogue de connexion
 */
void FenetrePrincipale::boutonOptionsClicked()
{
    // Ferme cette fenetre et ouvre le dialogue de connexion :
    DialConnexion *fenetreConnexion = new DialConnexion;
    fenetreConnexion->show();
    // Fermer les threads et attendre leur fin (join())
    tCommunication->terminate();
    tCommunication->wait(5000);
    // Ferme la fenetre :
    close();
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

bool FenetrePrincipale::isValid()
{
    return valid;
}




