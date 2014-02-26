#include "sensordata.h"

SensorData::SensorData()
{
    tensionBatterie = 0;
    vitesseAvantGauche = 0;
    vitesseArriereGauche = 0;
    vitesseAvantDroit = 0;
    vitesseArriereDroit = 0;
    IRgauche = 0;
    IRdroit = 0;
    odometrieDroite = 0;
    odometrieGauche = 0;
}





// Accesseurs et mutateurs :
int SensorData::getTensionBatterie() const
{
    return tensionBatterie;
}

void SensorData::setTensionBatterie(int value)
{
    tensionBatterie = value;
}
int SensorData::getVitesseAvantGauche() const
{
    return vitesseAvantGauche;
}

void SensorData::setVitesseAvantGauche(int value)
{
    vitesseAvantGauche = value;
}
int SensorData::getVitesseArriereGauche() const
{
    return vitesseArriereGauche;
}

void SensorData::setVitesseArriereGauche(int value)
{
    vitesseArriereGauche = value;
}
int SensorData::getVitesseArriereDroit() const
{
    return vitesseArriereDroit;
}

void SensorData::setVitesseArriereDroit(int value)
{
    vitesseArriereDroit = value;
}
int SensorData::getVitesseAvantDroit() const
{
    return vitesseAvantDroit;
}

void SensorData::setVitesseAvantDroit(int value)
{
    vitesseAvantDroit = value;
}
int SensorData::getIRgauche() const
{
    return IRgauche;
}

void SensorData::setIRgauche(int value)
{
    IRgauche = value;
}
int SensorData::getIRdroit() const
{
    return IRdroit;
}

void SensorData::setIRdroit(int value)
{
    IRdroit = value;
}



long SensorData::getOdometrieGauche() const
{
    return odometrieGauche;
}

void SensorData::setOdometrieGauche(long value)
{
    odometrieGauche = value;
}
long SensorData::getOdometrieDroite() const
{
    return odometrieDroite;
}

void SensorData::setOdometrieDroite(long value)
{
    odometrieDroite = value;
}













