#ifndef THREADCOMMUNICATION_H
#define THREADCOMMUNICATION_H

#include <QThread>
#include <QString>
#include <QTcpSocket>

#include "sensordata.h"

extern int commande;

class ThreadCommunication : public QThread
{
    Q_OBJECT

public:
    ThreadCommunication(QString ipRobot, int portRobot);
    void run();
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
    QString ip;
    int port;
    QTcpSocket *socket;
    int connecte; // -1 : erreur ; 0 : non ; 1 : oui
    bool termine;
    char *bufferEnvoi;
    char *bufferReception;
    // deux variables qui retiennent le sens et la vitesse demandées au robot lors de la communication précédente.
    int sensPrecedent;
    int vitessePrecedente;

    SensorData *capteurs;
};

#endif // THREADCOMMUNICATION_H
