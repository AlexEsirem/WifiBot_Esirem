#ifndef CONST_H
#define CONST_H

/* Fichier de constantes. Elles sont utilisées pour la clarté et une meilleure
 * compréhension du programme. On utilisera par exemple AVANT_GAUCHE au lieu de 4, pour indiquer
 * qu'on veut aller a gauche en avancant.
 * Contient aussi les IP et ports du simulateur et du robot réel.
 */

/* constantes pour savoir si on utilise le simulateur ou le robot */
#define IP_SIMULATEUR "127.0.0.1"
#define PORT_SIMULATEUR 15020

#define IP_REEL "192.168.1.106"
#define PORT_REEL 15020

/* Constantes qui indiquent une commande */
#define RIEN 0
#define FREIN 1
#define AVANCER 2
#define RECULER 3
#define AVANT_GAUCHE 4
#define AVANT_DROIT 5
#define ARRIERE_GAUCHE 6
#define ARRIERE_DROIT 7
#define PIVOTER_GAUCHE 8
#define PIVOTER_DROITE 9

/* constantes pour le sens (1 = en avant ; 0 = en arriere) */
#define EN_AVANT 1
#define EN_ARRIERE 0
#define SUR_PLACE 2

#endif // CONST_H
