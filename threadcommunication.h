#ifndef THREADCOMMUNICATION_H
#define THREADCOMMUNICATION_H

#include <QThread>
#include <QString>
#include <QTcpSocket>

#include "sensordata.h"

/*
 * Cette instruction indique que l'entier commande est définit ailleurs dans le programme.
 * Ce fichier ira donc le chercher où il est définit (fenetreprincipale.cpp)
 * Permet d'utiliser la variable commande comme variable globale et donc de la
 * partager facilement entre la fenetreprincipale et le threadcommunication.
 */
extern int commande;


/**
 * @brief The ThreadCommunication class
 * Le thread de communication effectue toutes les actions concernant l'envoi et la reception de donnees au robot.
 * Il calcule egalement les vitesses et directions à appliquer au robot en fonction de la commande actuelle.
 */
class ThreadCommunication : public QThread
{
    Q_OBJECT

public:
    ThreadCommunication(QString ipRobot, int portRobot);
    void run();
    void calculVitesses(int *vGauche, int *vDroite, int vMax);
    void calculSens(bool *sGauche, bool *sDroite);
    short crc16(unsigned char* tab, unsigned char taille_max);
    int getConnecte();
    void terminate();
    SensorData *getCapteurs() const;
    void setCapteurs(SensorData *value);
    bool sensChange();

signals:
    void sensorDataChanged();

public slots:
    void readData();
    void connexionPerdue();

private:
    /* Donnees de connexion */
    QString ip;
    int port;
    QTcpSocket *socket;
    /* Entier représentant l'état du socket (-1 = erreur ; 0 = non connecté ; 1 = connecté) */
    int connecte;
    /* Booleen indiquant l'état du thread. Lors de son lancement termine=false. Si la fonction terminate() est appelée,
     * on passe termine à true et la boucle run terminera son exécution */
    bool termine;
    /* Buffers d'envoi et de réception, pour échanger des données via le socket */
    char *bufferEnvoi;
    char *bufferReception;
    /* deux variables qui retiennent le sens et la vitesse demandées au robot lors de la communication précédente */
    int sensPrecedent;
    int vitessePrecedente;
    /* Objet de type SensorData pour enregistrer les données reçues des capteurs */
    SensorData *capteurs;
};

#endif // THREADCOMMUNICATION_H
