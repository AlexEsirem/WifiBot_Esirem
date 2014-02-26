#ifndef DIALCONNEXION_H
#define DIALCONNEXION_H

#endif // DIALCONNEXION_H

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

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
    QString ip;
    int port;

    QLabel *labelChoixIP;
    QLabel *labelChoixPort;
    QPushButton *boutonSimulateur;
    QPushButton *boutonReel;
    QPushButton *boutonOK;
    QPushButton *boutonQuitter;
    QLineEdit *champIP;
    QLineEdit *champPort;
};
