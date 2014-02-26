#ifndef SENSORDATA_H
#define SENSORDATA_H

class SensorData
{
public:
    SensorData();
    int getTensionBatterie() const;
    void setTensionBatterie(int value);

    int getVitesseAvantGauche() const;
    void setVitesseAvantGauche(int value);

    int getVitesseArriereGauche() const;
    void setVitesseArriereGauche(int value);

    int getVitesseArriereDroit() const;
    void setVitesseArriereDroit(int value);

    int getVitesseAvantDroit() const;
    void setVitesseAvantDroit(int value);

    int getIRgauche() const;
    void setIRgauche(int value);

    int getIRdroit() const;
    void setIRdroit(int value);

    long getOdometrieGauche() const;
    void setOdometrieGauche(long value);

    long getOdometrieDroite() const;
    void setOdometrieDroite(long value);

private:
    int tensionBatterie;
    int vitesseAvantGauche;
    int vitesseArriereGauche;
    int vitesseAvantDroit;
    int vitesseArriereDroit;
    int IRgauche;
    int IRdroit;
    long odometrieGauche;
    long odometrieDroite;
};

#endif // SENSORDATA_H
