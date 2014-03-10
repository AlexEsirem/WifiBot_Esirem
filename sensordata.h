#ifndef SENSORDATA_H
#define SENSORDATA_H

/**
 * @brief The SensorData class
 * Cette classe offre une structure qui permet d'enregistrer les données reçues des capteurs du robot dans un seul objet.
 */
class SensorData
{
public:
    SensorData();


    int getTensionBatterie() const;
    void setTensionBatterie(int value);

    int getVitesseGauche() const;
    void setVitesseGauche(int value);

    int getVitesseDroite() const;
    void setVitesseDroite(int value);

    int getIRgauche() const;
    void setIRgauche(int value);

    int getIRdroit() const;
    void setIRdroit(int value);

    int getCourant() const;
    void setCourant(int value);

    long getOdometrieGauche() const;
    void setOdometrieGauche(long value);

    long getOdometrieDroite() const;
    void setOdometrieDroite(long value);

    int getIRgauche2() const;
    void setIRgauche2(int value);

    int getIRdroit2() const;
    void setIRdroit2(int value);

private:
    int tensionBatterie;
    int vitesseGauche;
    int vitesseDroite;
    int IRgauche;
    int IRgauche2;
    int IRdroit;
    int IRdroit2;
    int courant;
    long odometrieGauche;
    long odometrieDroite;
};

#endif // SENSORDATA_H
