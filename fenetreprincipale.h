#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

#include "threadcommunication.h"

/**
 * @brief The FenetrePrincipale class
 * Définit la fenêtre principale de l'application.
 * Elle permet de modifier la commande du robot grâce à des boutons de l'interface.
 * Les appuis sur les touches directionnelles du clavier sont écoutés, on peut donc également
 * piloter le robot avec les flèches.
 * La fenêtre possède une instance de ThreadCommunication qui sera responsable des
 * échanges avec le robot ainsi que de la détermination des vitesses à envoyer.
 * En gros, la fenetre en elle-même ne sert qu'à recevoir les ordres de l'utilisateur. C'est le
 * thread de communication qui fera tout le reste.
 */
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
    /* Donnees reseau */
    QString ip;
    int port;

    /* flags clavier. Permettent de savoir quelles touches clavier sont pressees à chaque instant */
    bool keyUp;
    bool keyDown;
    bool keyLeft;
    bool keyRight;

    /* Elements de l'interface */
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

    /* thread de communication. Se chargera d'échanger avec le robot */
    ThreadCommunication *tCommunication;
};

#endif // FENETREPRINCIPALE_H

