#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

#include "threadcommunication.h"


class FenetrePrincipale : public QWidget
{
    Q_OBJECT

public:
    FenetrePrincipale(QString ipRobot, int portRobot);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void majCommandeFromFlags();

public slots:
    void updateDataLabels();
    void boutonReleased();
    void boutonAccelererPressed();
    void boutonDroitePressed();
    void boutonGauchePressed();
    void boutonFreinPressed();
    void boutonQuitterClicked();

private:
    // Donnees reseau :
    QString ip;
    int port;

    // flags pour savoir quelles touches clavier sont pressees :
    bool keyUp;
    bool keyDown;
    bool keyLeft;
    bool keyRight;

    // Elements de l'interface :
    QPushButton *boutonAccelerer;
    QPushButton *boutonFrein;
    QPushButton *boutonGauche;
    QPushButton *boutonDroite;
    QPushButton *boutonQuitter;
    QLabel *labelDirection;
    QSlider *choixDirection;
    QLabel *labelVitesseAvantGauche;
    QLabel *labelVitesseAvantDroit;
    QLabel *labelVitesseArriereGauche;
    QLabel *labelVitesseArriereDroit;
    QLabel *labelTensionBatterie;
    QLabel *labelIRGauche;
    QLabel *labelIRDroit;
    QLabel *labelIR;
    QLabel *labelVitesses;

    // thread de communication avec le robot :
    ThreadCommunication *tCommunication;
};

#endif // FENETREPRINCIPALE_H

