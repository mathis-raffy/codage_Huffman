#ifndef TAS_H
#define TAS_H

#include <stdlib.h>

// Définition du type d'éléments du tas
typedef int TElement;

// Structure du tas
typedef struct tas {
    int tailleMax;   // capacité maximale
    int taille;      // nombre d'éléments actuels
    TElement* tab;   // tableau contenant les éléments
} Tas;

// Prototypes des fonctions

/**
 * @brief Alloue un tas de taille n.
 * @param n Taille maximale du tas.
 * @return Tas initialisé avec tab alloué et taille = 0.
 */
Tas allocMemTas(int n);

Tas libMemTas(Tas t);

/**
 * @brief Renvoie l'élément minimal du tas (racine).
 * @param t Tas
 * @return Valeur minimale
 */
TElement minTas(Tas t);

/**
 * @brief Renvoie la taille actuelle du tas.
 * @param t Tas
 * @return Nombre d'éléments dans le tas
 */
int taille(Tas t);

/**
 * @brief Renvoie l'indice du père d'un élément donné.
 * @param n Indice de l'élément
 * @param t Tas
 * @return Indice du père
 */
int pere(int n);

int estVideTas(Tas t);

TElement donneeAP(int nd, Tas t);

int filsGaucheAP(int nd);

int filsDroitAP(int nd);

int estExistNoeud(int nd, Tas t);

/**
 * @brief Insère un élément dans le tas.
 * @param e Élément à insérer
 * @param t Pointeur vers le tas
 */
void inserTas(TElement e, Tas* t, int* freq);

void suppTas(Tas* t, int* freq);
#endif // TAS_H