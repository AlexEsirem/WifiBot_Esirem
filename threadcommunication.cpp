#include <QMessageBox>
#include <QApplication>
#include <iostream>

#include "threadcommunication.h"
#include "Const.h"
#include "dialconnexion.h"

using namespace std;

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

    /* Creation du socket TCP : */
    socket = new QTcpSocket(this);

    /* Connexion du socket à l'ip/port demandé.
     * On attend ensuite la connexion pendant 5 sec max.
     * Si la connexion est établie (1), c'est bon
     * Sinon (!=1), alors on affiche un message d'erreur et on quitte l'application. */
    socket->connectToHost(ip, port);
    if(socket->waitForConnected(SOCK_TIMEOUT))
     {
        connecte = 1;
    }
    else
    {
        connecte = -1;
    }

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
            /* VitesseMax sera utilisée pour savoir quelle est la
             * vitesse maximale que l'on peut envoyer au robot.*/
            int vitesseMax;
            int vGauche = 0;
            int vDroite = 0;
            bool sGauche = false;
            bool sDroite = false;

            if(ip==IP_REEL)
            {
                /* Les messgages sont de 9 chars */
                // Voir le rapport pour les détails

                vitesseMax = 240;

                // calcul des vitesses vGauche et vDroite :
                calculVitesses(&vGauche, &vDroite, vitesseMax);
                // déterminaiton des sens sGauche et sDroite :
                calculSens(&sGauche, &sDroite);

                // Les deux premiers chars sont :
                bufferEnvoi[0] = 255;
                bufferEnvoi[1] = 0x07;

                // Cote gauche :
                bufferEnvoi[2] = vGauche; // vérifier à quoi sert celui-ci
                bufferEnvoi[3] = 0;
                // Cote droit :
                bufferEnvoi[4] = vDroite; // vérifier à quoi sert celui-ci
                bufferEnvoi[5] = 0;

                // Définition du flag de commande :
                int commandFlag = 0;
                if(sGauche)
                    commandFlag += 64;
                if(sDroite)
                    commandFlag += 16;
                bufferEnvoi[6] = (char)commandFlag;

                // Les deux derniers char sont les CRC :
                short crc = crc16((unsigned char*)bufferEnvoi+1, 6);
                bufferEnvoi[7] = (char)crc;
                bufferEnvoi[8] = (char)(crc >> 8);

                // Envoi du message :
                socket->write(bufferEnvoi, 9);
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

                vitesseMax = 60;

                // Calcul des vitesses vGauche et vDroite :
                calculVitesses(&vGauche, &vDroite, vitesseMax);
                // déterminaiton des sens sGauche et sDroite :
                calculSens(&sGauche, &sDroite);

                // il reste juste a ajouter 64 à cGauche ou cDroite en fonction de sGauche et sDroite :
                if(sGauche)
                    cGauche += 64;
                if(sDroite)
                    cDroite += 64;

                // On ajoute les vitesses aux chars :
                cGauche += vGauche;
                cDroite += vDroite;

                bufferEnvoi[0] = (char)cGauche;
                bufferEnvoi[1] = (char)cDroite;

                // Envoi du message :
                socket->write(bufferEnvoi, 2);
            }

            readData();

            msleep(T_DELAY); // en ms
        }

    }
    // si on arrive ici c'est que termine est a true.
    // on ferme le socket et ce thread se terminera.
    printf("Deconnexion");
    socket->disconnectFromHost();
    // si le socket ne se ferme pas tout de suite on attend sa deconnexion 5 secondes max.
    if (socket->state() != QAbstractSocket::UnconnectedState)
        socket->waitForDisconnected(SOCK_TIMEOUT);
    if(socket->state() != QAbstractSocket::UnconnectedState)
        printf("Erreur socket");
    else
        printf("Bien déconnecté");
}

/**
 * @brief ThreadCommunication::calculSens
 * @param sGauche : pointeur sur le booleen sGauche
 * @param sDroite : pointeur sur le booleen sGauche
 * Détermine si le coté gauche et le coté droit avancent ou non.
 * Si le coté avance, son booleen sX sera mis à true, sinon à false.
 */
void ThreadCommunication::calculSens(bool *sGauche, bool *sDroite)
{
    switch(commande)
    {
    case AVANCER:
        (*sGauche) = true;
        (*sDroite) = true;
        break;

    case RECULER:
        (*sGauche) = false;
        (*sDroite) = false;
        break;

    case ARRIERE_GAUCHE:
        (*sGauche) = false;
        (*sDroite) = false;
        break;

    case ARRIERE_DROIT:
        (*sGauche) = false;
        (*sDroite) = false;
        break;

    case AVANT_GAUCHE:
        (*sGauche) = true;
        (*sDroite) = true;
        break;

    case AVANT_DROIT:
        (*sGauche) = true;
        (*sDroite) = true;
        break;

    case PIVOTER_GAUCHE:
        (*sGauche) = false;
        (*sDroite) = true;
        break;

    case PIVOTER_DROITE:
        (*sGauche) = true;
        (*sDroite) = false;
        break;

    default:
        (*sGauche) = false;
        (*sDroite) = false;
        break;
    }
}


/**
 * @brief ThreadCommunication::calculVitesses
 * @param vGauche : pointeur sur l'entier vGauche
 * @param vDroite : pointeur sur l'entier vDroite
 * @param vMax : maximum que les vitesses peuvent prendre
 * Calcule les vitesses gauche et droite et les affecte aux deux variables dont les pointeurs
 * sont passées en paramètre. Aucune de ces vitesses ne doit dépasser vMax.
 */
void ThreadCommunication::calculVitesses(int *vGauche, int *vDroite, int vMax)
{
    if(commande == AVANCER
            || commande == RECULER
                || commande == PIVOTER_DROITE
                    || commande == PIVOTER_GAUCHE)
    {
            (*vGauche) = vMax;
            (*vDroite) = vMax;
    }
    else if(commande == AVANT_DROIT
            || commande == ARRIERE_DROIT)
    {
            (*vGauche) = vMax;
            (*vDroite) = vMax/4;
    }
    else if(commande == AVANT_GAUCHE
            || commande == ARRIERE_GAUCHE)
    {
            (*vGauche) = vMax/4;
            (*vDroite) = vMax;
    }
    else
    {
        (*vGauche) = 0;
        (*vDroite) = 0;
    }
}


short ThreadCommunication::crc16(unsigned char* tab, unsigned char taille_max)
{
    unsigned int crc = 0xFFFF;
    unsigned int polynome = 0xA001;
    unsigned int cptOctet = 0;
    unsigned int cptBit = 0;
    unsigned int parity = 0;

    crc = 0xFFFF;
    polynome = 0xA001;

    for(cptOctet = 0 ; cptOctet < taille_max ; cptOctet++)
    {
        crc ^= *(tab + cptOctet);
        for(cptBit = 0 ; cptBit <= 7 ; cptBit++)
        {
            parity = crc;
            crc >>= 1;
            if(parity%2 == true) crc ^= polynome;
        }
    }
    return(crc);
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
        if(commande == RECULER  // on demande de reculer
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
        if(commande == AVANCER // on demande d'avancer
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

        // Vitesse gauche *(-1) car elle est inversée
        // vitesse en tics/50ms
        // 140ticks/50ms = 0.5m/S
        int vGauche = (int)((bufferReception[1]<<8) + bufferReception[0])*(-1);
        if(vGauche > 32767)
            vGauche = vGauche - 65536;
        capteurs->setVitesseGauche(vGauche*0.02); // Conversion en km/h
        int vDroite = (int)((bufferReception[10]<<8) + bufferReception[9])*(-1);
        if(vDroite > 32767)
            vDroite = vDroite - 65536;
        capteurs->setVitesseDroite(vDroite*0.02); // km/h
        capteurs->setTensionBatterie((int)bufferReception[2]);
        capteurs->setIRgauche((int)bufferReception[3]);
        capteurs->setIRgauche2((int)bufferReception[4]);
        capteurs->setIRdroit((int)bufferReception[11]);
        capteurs->setIRdroit2((int)bufferReception[12]);
        capteurs->setCourant((int)bufferReception[17]);

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

