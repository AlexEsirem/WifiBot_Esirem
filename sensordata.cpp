#include "sensordata.h"

/**
 * @brief SensorData::SensorData
 * Instancie un objet de type SensorData avec des valeurs nulles.
 */
SensorData::SensorData()
{
    tensionBatterie = 0;
    vitesseGauche = 0;
    vitesseDroite = 0;
    IRgauche = 0;
    IRdroit = 0;
    odometrieDroite = 0;
    odometrieGauche = 0;
    courant = 0;
}
int SensorData::getTensionBatterie() const
{
    return tensionBatterie;
}

void SensorData::setTensionBatterie(int value)
{
    tensionBatterie = value;
}
int SensorData::getVitesseGauche() const
{
    return vitesseGauche;
}

void SensorData::setVitesseGauche(int value)
{
    vitesseGauche = value;
}
int SensorData::getVitesseDroite() const
{
    return vitesseDroite;
}

void SensorData::setVitesseDroite(int value)
{
    vitesseDroite = value;
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
int SensorData::getCourant() const
{
    return courant;
}

void SensorData::setCourant(int value)
{
    courant = value;
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
int SensorData::getIRgauche2() const
{
    return IRgauche2;
}

void SensorData::setIRgauche2(int value)
{
    IRgauche2 = value;
}
int SensorData::getIRdroit2() const
{
    return IRdroit2;
}

void SensorData::setIRdroit2(int value)
{
    IRdroit2 = value;
}












/***************************
 * Accesseurs et mutateurs :
 ***************************/














