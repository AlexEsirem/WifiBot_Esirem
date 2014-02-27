#include <QMessageBox>
#include <QApplication>

#include "threadcommunication.h"
#include "Const.h"


/**
 * @brief ThreadCommunication::ThreadCommunication
 * @param ipRobot
 * @param portRobot
 * Ce thread gère les communications avec le robot. Envoie les instructions et reçoit les données des capteurs.
 * L'utilisation d'un thread dédié à ceci permet de ne pas ralentir l'interface graphique qui doit rester
 * réactive aux actions de l'utilisateur.
 */
ThreadCommunication::ThreadCommunication(QString ipRobot, int portRobot) : QThread()
{
    /* Le thread n'est pas terminé par défaut et le socket n'est pas connecté : */
    termine = false;
    connecte = 0;

    ip = ipRobot;
    port = portRobot;

    /* On détermine si on travaille avec le robot ou le simulateur. La taille du buffer d'envoi en dépend */
    if(ip == IP_REEL)
        bufferEnvoi = new char[9];
    else
        bufferEnvoi = new char[2];

    /* Dans tous les cas le buffer de réception a une taille de 21 */
    bufferReception = new char[21];

    /* Sens initial : aucun (sur place) */
    sensPrecedent = SUR_PLACE;
    /* Vitesse initiale nulle */
    vitessePrecedente = 0;

    /* Création de l'objet SensorData pour stocker les infos des capteurs */
    capteurs = new SensorData();

    /* Creation du socket TCP : */
    socket = new QTcpSocket(this);

    /* Connexion du socket à l'ip/port demandé.
     * On attend ensuite la connexion pendant 5 sec max.
     * Si la connexion est établie (1), c'est bon
     * Sinon (!=1), alors on affiche un message d'erreur et on quitte l'application. */
    socket->connectToHost(ip, port);
    if(socket->waitForConnected(5000))
        connecte = 1;
    else
    {
        connecte = -1;
        QMessageBox *message = new QMessageBox();
        message->setText("La connexion a échoué. Vérifiez les informations.");
        message->exec();
        QApplication::quit();
    }

    /* Liaison du signal "socket déconnecté" avec le slot "connexion perdue". */
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(connexionPerdue()));

}

/**
 * @brief ThreadCommunication::run
 * Fonction exécutée lors du lancement du thread par appel à start().
 * Tant que le booleen "termine" est faux, on effectue la boucle.
 * Si le socket est bien connecté (on peut lire et ecrire dessus), on envoie
 * la commande et puis on lit les données des capteurs.
 */
void ThreadCommunication::run(){
    while(!termine)
    {
        if(socket->isWritable() && socket->isReadable())
        {
            if(ip==IP_REEL)
            {
                /* Les messgages sont de 9 chars */
            }
            else
            {
                /* Messages de 2 chars */
                // Char[0] : vitesse gauche : de 0 a 60
                // CTRL DIR S5 S4 S3 S2 S1 S0
                // ctrl : 1 closed loop ; 0 open loop ??
                // dir = 1 pour avant, 0 pour arriere

                int cGauche = 0;
                int cDroite = 0;
                int vitesse = 0;


                /// MODIFIER VITESSEPRECEDENTE pour avoir la vraie vitesse recue du robot !

                switch(commande)
                {
                case RIEN:
                    // Ne rien faire => vitesses nulles donc on ne modifie pas cGauche et cDroite.
                    break;

                case FREIN:
                    if(sensPrecedent == EN_AVANT)
                    {
                        // alors on va en arriere pour atteindre une vitesse nulle
                        if(vitessePrecedente > 0)
                            vitesse = vitessePrecedente - 1;
                        else // sauf si la vitesse est deja nulle
                            vitesse = 0;
                    }
                    else
                    {
                        // alors on va en avant pour atteindre une vitesse nulle
                        cGauche += 64;
                        cDroite += 64;
                        if(vitessePrecedente > 0)
                            vitesse = vitessePrecedente - 1;
                        else // sauf si la vitesse est deja nulle
                            vitesse = 0;
                    }
                    break;

                case AVANCER:
                    // Dans tous les cas on indique le sens "en avant" en ajoutant 64
                    cGauche += 64;
                    cDroite += 64;
                    if(sensChange())
                    {
                        // si le sens a change on met la vitesse à 5 (lent)
                        vitesse = 5;
                    }
                    else
                    {
                        // si le sens n'a pas changé on augmente la vitesse de 1 :
                        if(vitessePrecedente < 60)
                            vitesse = vitessePrecedente + 1;
                        else // sauf si la vitesse est deja a 60
                            vitesse = vitessePrecedente;
                    }
                    // on ajoute ces vitesses aux 2 chars :
                    cGauche += vitesse;
                    cDroite += vitesse;
                    // on met a jour les infos precedentes :
                    sensPrecedent = EN_AVANT;
                    vitessePrecedente = vitesse;
                    break;

                case RECULER:
                    if(sensChange())
                    {
                        // Si le sens a change on met la vitesse à 5 (lent)
                        vitesse = 5;
                    }
                    else
                    {
                        if(vitessePrecedente < 60)
                            vitesse = vitessePrecedente + 1;
                        else
                            vitesse = vitessePrecedente;
                    }
                    // on ajoute ces vitesses aux 2 chars :
                    cGauche += vitesse;
                    cDroite += vitesse;
                    // on met a jour les infos precedentes :
                    sensPrecedent = EN_ARRIERE;
                    vitessePrecedente = vitesse;
                    break;

                case AVANT_GAUCHE:
                    // Dans tous les cas on indique le sens "en avant" en ajoutant 64
                    cGauche += 64;
                    cDroite += 64;
                    if(sensChange())
                    {
                        // si le sens a change on met la vitesse à 5 (lent)
                        vitesse = 5;
                    }
                    else
                    {
                        // si le sens n'a pas changé on augmente la vitesse de 1 :
                        if(vitessePrecedente < 60)
                            vitesse = vitessePrecedente + 1;
                        else // sauf si la vitesse est deja a 60
                            vitesse = vitessePrecedente;
                    }
                    // on ajoute ces vitesses aux 2 chars.
                    // la vitesse gauche est 4 fois plus petite (pour tourner a gauche)
                    cGauche += vitesse/4;
                    cDroite += vitesse;
                    // on met a jour les infos precedentes :
                    sensPrecedent = EN_AVANT;
                    vitessePrecedente = vitesse;
                    break;

                case AVANT_DROIT:
                    // Dans tous les cas on indique le sens "en avant" en ajoutant 64
                    cGauche += 64;
                    cDroite += 64;
                    if(sensChange())
                    {
                        // si le sens a change on met la vitesse à 5 (lent)
                        vitesse = 5;
                    }
                    else
                    {
                        // si le sens n'a pas changé on augmente la vitesse de 1 :
                        if(vitessePrecedente < 60)
                            vitesse = vitessePrecedente + 1;
                        else // sauf si la vitesse est deja a 60
                            vitesse = vitessePrecedente;
                    }
                    // on ajoute ces vitesses aux 2 chars.
                    // la vitesse droite est 4 fois plus petite (pour tourner a droite)
                    cGauche += vitesse;
                    cDroite += vitesse/4;
                    // on met a jour les infos precedentes :
                    sensPrecedent = EN_AVANT;
                    vitessePrecedente = vitesse;
                    break;

                case ARRIERE_GAUCHE:
                    if(sensChange())
                    {
                        // si le sens a change on met la vitesse à 5 (lent)
                        vitesse = 5;
                    }
                    else
                    {
                        // si le sens n'a pas changé on augmente la vitesse de 1 :
                        if(vitessePrecedente < 60)
                            vitesse = vitessePrecedente + 1;
                        else // sauf si la vitesse est deja a 60
                            vitesse = vitessePrecedente;
                    }
                    // on ajoute ces vitesses aux 2 chars.
                    // la vitesse gauche est 4 fois plus petite (pour tourner a gauche)
                    cGauche += vitesse/4;
                    cDroite += vitesse;
                    // on met a jour les infos precedentes :
                    sensPrecedent = EN_ARRIERE;
                    vitessePrecedente = vitesse;
                    break;

                case ARRIERE_DROIT:
                    if(sensChange())
                    {
                        // si le sens a change on met la vitesse à 5 (lent)
                        vitesse = 5;
                    }
                    else
                    {
                        // si le sens n'a pas changé on augmente la vitesse de 1 :
                        if(vitessePrecedente < 60)
                            vitesse = vitessePrecedente + 1;
                        else // sauf si la vitesse est deja a 60
                            vitesse = vitessePrecedente;
                    }
                    // on ajoute ces vitesses aux 2 chars.
                    // la vitesse gauche est 4 fois plus petite (pour tourner a gauche)
                    cGauche += vitesse;
                    cDroite += vitesse/4;
                    // on met a jour les infos precedentes :
                    sensPrecedent = EN_ARRIERE;
                    vitessePrecedente = vitesse;
                    break;

                case PIVOTER_GAUCHE:
                    // le robot tourne sur lui-meme :
                    if(sensChange())
                    {
                        // si le sens a change on met la vitesse à 5 (lent)
                        vitesse = 5;
                    }
                    else
                    {
                        if(vitessePrecedente < 60)
                            vitesse = vitessePrecedente + 1;
                        else
                            vitesse = vitessePrecedente;
                    }
                    // Le coté gauche ira en arriere et le droit en avant, avec la même vitesse :
                    cDroite += 64;
                    cDroite += vitesse;
                    cGauche += vitesse;
                    sensPrecedent = SUR_PLACE;
                    vitessePrecedente = vitesse;
                    break;

                case PIVOTER_DROITE:
                    // le robot tourne sur lui-meme :
                    if(sensChange())
                    {
                        // si le sens a change on met la vitesse à 5 (lent)
                        vitesse = 5;
                    }
                    else
                    {
                        if(vitessePrecedente < 60)
                            vitesse = vitessePrecedente + 1;
                        else
                            vitesse = vitessePrecedente;
                    }
                    // Le coté droite ira en arriere et le gauche en avant, avec la même vitesse :
                    cGauche+= 64;
                    cGauche += vitesse;
                    cDroite += vitesse;
                    sensPrecedent = SUR_PLACE;
                    vitessePrecedente = vitesse;
                    break;

                default:
                    break;
                }

                bufferEnvoi[0] = (char)cGauche;
                bufferEnvoi[1] = (char)cDroite;

                socket->write(bufferEnvoi, 2);
            }
            readData();
            msleep(10); // sleep 10 ms.
        }
    }
    // si on arrive ici c'est que termine est a true.
    // on ferme le socket et ce thread se terminera.
    socket->disconnectFromHost();
    // si le socket ne se ferme pas tout de suite on attend sa deconnexion 5 secondes max.
    if (socket->state() != QAbstractSocket::UnconnectedState)
        socket->waitForDisconnected(5000);
}

/**
 * @brief ThreadCommunication::sensChange
 * Determine si la commande actuelle entraine un changement de sens
 * par exemple on allait en avant et on demande de reculer.
 * Cela permet de savoir si on doit remettre la vitesse à 0,
 * ou si on augmente juste la vitesse précédente.
 * @return true si sens changé et false sinon
 */
bool ThreadCommunication::sensChange()
{
    if(sensPrecedent == EN_AVANT)  // si on allait en avant et que...
    {
        if(commande == FREIN // on demande de freiner
            || commande == RECULER  // on demande de reculer
                || commande == ARRIERE_GAUCHE
                    ||commande == ARRIERE_DROIT
                        || commande == PIVOTER_DROITE
                            || commande == PIVOTER_GAUCHE)
        {
            // alors le sens a change
            return true;
        }
    }
    else if(sensPrecedent == EN_ARRIERE)  // si on allait en arriere et que...
    {
        if(commande == FREIN // on demande de freiner
                || commande == AVANCER // on demande d'avancer
                    || commande == AVANT_DROIT
                        || commande == AVANT_GAUCHE
                            || commande == PIVOTER_DROITE
                               || commande == PIVOTER_GAUCHE)
        {
            // alors le sens a change
            return true;
        }
    }
    else if(sensPrecedent == SUR_PLACE)
    {
        if(commande == FREIN
                || commande == AVANCER
                    || commande == AVANT_DROIT
                      || commande == AVANT_GAUCHE
                        || commande == RECULER
                            || commande == ARRIERE_GAUCHE
                                ||commande == ARRIERE_DROIT )
            return true;
    }
    // dans les autres cas, le sens n'a pas change :
    return false;
}

/**
 * @brief ThreadCommunication::readData
 * Lit les données des capteurs envoyées par le robot et les stocke dans l'objet de type SensorData.
 */
void ThreadCommunication::readData()
{
    if(socket->bytesAvailable() >= 21)
    {
        socket->read(bufferReception, 21);

        capteurs->setTensionBatterie((int)bufferReception[0]);
        capteurs->setVitesseAvantGauche((int)bufferReception[1]);
        capteurs->setVitesseArriereGauche((int)bufferReception[2]);
        capteurs->setVitesseAvantDroit((int)bufferReception[3]);
        capteurs->setVitesseArriereDroit((int)bufferReception[4]);
        capteurs->setIRgauche((int)bufferReception[5]);
        capteurs->setIRdroit((int)bufferReception[6]);

        emit sensorDataChanged();
    }
}

/**
 * @brief ThreadCommunication::terminate
 * Fonction appelée pour arrêter ce thread. On met simplement termine à true.
 * Cela aura pour effet la fin du run() qui fermera avant ça le socket.
 */
void ThreadCommunication::terminate()
{
    termine = true;
}

/**
 * @brief ThreadCommunication::connexionPerdue
 * Slot appelé en cas de perte de connexion. On affiche un message d'erreur et on quitte.
 */
void ThreadCommunication::connexionPerdue()
{
    if(!termine)
    {
        QMessageBox *message = new QMessageBox();
        message->setText("La connexion a été perdue.");
        message->exec();
        QApplication::quit();
    }
}

/********************************
 * Accesseurs et mutateurs
 ********************************/
SensorData *ThreadCommunication::getCapteurs() const
{
    return capteurs;
}

void ThreadCommunication::setCapteurs(SensorData *value)
{
    capteurs = value;
}

int ThreadCommunication::getConnecte()
{
    return connecte;
}

