#ifndef DIALCONNEXION_H
#define DIALCONNEXION_H

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

/**
 * @brief The DialConnexion class
 * Définit le dialogue de connexion.
 * C'est la première fenêtre affichée.
 * Elle permet à l'utilisateur d'entrer l'ip et le port auxquels se connecter.
 * Deux choix prédéfinis sont proposés (robot et simulateur).
 */
class DialConnexion : public QWidget
{
    Q_OBJECT

public:
    DialConnexion();

public slots:
    void boutonOKClicked();
    void boutonReelClicked();
    void boutonSimulateurClicked();

private:
    /* données de connexion */
    QString ip;
    int port;

    /* Elements de l'interface graphique */
    QLabel *labelChoixIP;
    QLabel *labelChoixPort;
    QPushButton *boutonSimulateur;
    QPushButton *boutonReel;
    QPushButton *boutonOK;
    QPushButton *boutonQuitter;
    QLineEdit *champIP;
    QLineEdit *champPort;
};


#endif // DIALCONNEXION_H

