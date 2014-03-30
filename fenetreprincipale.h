#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

#include <QWebView>
#include <QtWebKit>

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
    bool isValid();

public slots:
    void updateDataLabels();
    void boutonAccelererPressed();
    void boutonAccelererReleased();
    void boutonReculerPressed();
    void boutonReculerReleased();
    void boutonDroitePressed();
    void boutonDroiteReleased();
    void boutonGauchePressed();
    void boutonGaucheReleased();
    void boutonAvantGauchePressed();
    void boutonAvantGaucheReleased();
    void boutonAvantDroitePressed();
    void boutonAvantDroiteReleased();
    void boutonArriereGauchePressed();
    void boutonArriereGaucheReleased();
    void boutonArriereDroitePressed();
    void boutonArriereDroiteReleased();

    void boutonQuitterClicked();
    void boutonOptionsClicked();

    void boutonCaptureClicked();

private:
    /* Donnees reseau */
    QString ip;
    int port;

    /* Flag pour indiquer si il y a une erreur ou non lors de la création de la fenetre */
    bool valid;

    /* flags clavier. Permettent de savoir quelles touches clavier sont pressees à chaque instant */
    bool keyUp;
    bool keyDown;
    bool keyLeft;
    bool keyRight;

    /* Elements de l'interface */
    QPushButton *boutonAccelerer;
    QPushButton *boutonReculer;
    QPushButton *boutonGauche;
    QPushButton *boutonDroite;
    QPushButton *boutonAvantDroite;
    QPushButton *boutonAvantGauche;
    QPushButton *boutonArriereDroite;
    QPushButton *boutonArriereGauche;
    QPushButton *boutonQuitter;
    QPushButton *boutonCapture;
    QPushButton *boutonOptions;

    QLabel *labelVitesseGauche;
    QLabel *labelVitesseDroite;
    QLabel *labelTensionBatterie;
    QLabel *labelIRGauche;
    QLabel *labelIRGauche2;
    QLabel *labelIRDroit;
    QLabel *labelIRDroit2;
    QLabel *labelIR;
    QLabel *labelAutres;
    QLabel *labelVitesses;
    QLabel *labelCourant;

    QFrame *panelCommandes;
    QFrame *panelCamera;
    QFrame *panelData;

    /* Elements Web pour la caméra */
    QWebView *pageWeb;

    /* thread de communication. Se chargera d'échanger avec le robot */
    ThreadCommunication *tCommunication;
};

#endif // FENETREPRINCIPALE_H

