#ifndef CONST_H
#define CONST_H

// constantes pour savoir si on utilise le simulateur ou le robot
#define SIMULATEUR 0
#define T_ENVOI_SIMULATEUR 2
#define IP_SIMULATEUR "127.0.0.1"
#define PORT_SIMULATEUR 15020

#define REEL 1
#define T_ENVOI_REEL 9
#define IP_REEL "192.168.1.106"
#define PORT_REEL 15020


// Constantes qui indiquent une commande :
#define RIEN 0
#define FREIN 1
#define AVANCER 2
#define RECULER 3
#define AVANT_GAUCHE 4
#define AVANT_DROIT 5
#define ARRIERE_GAUCHE 6
#define ARRIERE_DROIT 7

// constantes pour le sens (1 = en avant ; 0 = en arriere)
#define EN_AVANT 1
#define EN_ARRIERE 0

#endif // CONST_H