#include <QMessageBox>
#include <QApplication>

#include "threadcommunication.h"
#include "Const.h"

ThreadCommunication::ThreadCommunication(QString ipRobot, int portRobot) : QThread()
{
    termine = false;
    connecte = 0;

    ip = ipRobot;
    port = portRobot;

    if(ip == IP_REEL)
        bufferEnvoi = new char[9];
    else
        bufferEnvoi = new char[2];

    bufferReception = new char[21];

    sensPrecedent = EN_AVANT; // 1 pour avancer ; reculer = 0
    vitessePrecedente = 0;

    capteurs = new SensorData();

    socket = new QTcpSocket(this);

    //QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));

    socket->connectToHost(ip, port);
    if(socket->waitForConnected(1000))
        connecte = 1;
    else
    {
        connecte = -1;
        QMessageBox *message = new QMessageBox();
        message->setText("La connexion a échoué. Vérifiez les informations.");
        message->exec();
        QApplication::quit();
    }


    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(connexionPerdue()));

}

void ThreadCommunication::run(){
    while(!termine)
    {
        if(connecte == 1)
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
 * ou si on augmente la vitesse précédente.
 * @return true si sens changé et false sinon
 */
bool ThreadCommunication::sensChange()
{
    if(sensPrecedent == EN_AVANT)  // si on allait en avant et que...
    {
        if(commande == FREIN // on demande de freiner
            || commande == RECULER  // on demande de reculer
                || commande == ARRIERE_GAUCHE
                    ||commande == ARRIERE_DROIT)
        {
            // alors le sens a change
            return true;
        }
    }
    if(sensPrecedent == EN_ARRIERE)  // si on allait en arriere et que...
    {
        if(commande == FREIN // on demande de freiner
                || commande == AVANCER // on demande d'avancer
                    || commande == AVANT_DROIT
                        || commande == AVANT_GAUCHE)
        {
            // alors le sens a change
            return true;
        }
    }
    // dans les autres cas, le sens n'a pas change :
    return false;
}


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
SensorData *ThreadCommunication::getCapteurs() const
{
    return capteurs;
}

void ThreadCommunication::setCapteurs(SensorData *value)
{
    capteurs = value;
}

void ThreadCommunication::terminate()
{
    termine = true;
}

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

int ThreadCommunication::getConnecte()
{
    return connecte;
}
