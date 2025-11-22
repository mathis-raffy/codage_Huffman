#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int TElement;
#define INDEFINI -100000

typedef struct tas {
	int tailleMax;
	int taille;
	TElement* tab;
}Tas;

Tas allocMemTas(int n) {
	Tas t;
	t.tab = (TElement*)malloc(n * sizeof(TElement));
	t.tailleMax = n;
	t.taille = 0;
	return t;
}

Tas libMemTas(Tas t) {
	free(t.tab);
	t.tailleMax = 0;
	t.taille = 0;
}

TElement minTas(Tas t) {
	if (t.taille == 0) {
		return INDEFINI;
	}
	return t.tab[0];
}

int taille(Tas t) {
	return t.taille;
}

int pere(int n) {
	return (n - 1) / 2;
}

int estVideTas(Tas t) {
	return taille(t) == 0;
}

TElement donneeAP(int nd, Tas t) {
	return t.tab[nd];
}

int filsGaucheAP(int nd) {
	return (2 * nd + 1);
}

int filsDroitAP(int nd) {
	return (2 * nd + 2);
}

int estExistNoeud(int nd, Tas t) {
	return nd < taille(t);
}

void inserTas(TElement e, Tas* t, int* freq) {
	int n = taille(*t);
	int indice = e;   // poids du nouvel élément
	if (n == 0) {
		t->tab[0] = indice;
		t->taille = 1;
		return;
	}
	int p = pere(n);
	while (n > 0 && freq[t->tab[p]] > freq[indice]) {  // pour un tas MIN
		t->tab[n] = t->tab[p];
		n = p;
		p = pere(n);
	}
	t->tab[n] = indice;
	t->taille = t->taille + 1;
}

//précondition non estVideTas(t)
void suppTas(Tas* t, int* freq) {
	int r = 0;
	int dn = taille(*t) - 1;
	t->tab[r] = t->tab[dn];
	t->taille = dn;
	if (!estVideTas(*t)) {
		TElement e = donneeAP(r, *t);
		int echange = 1;
		int fg = filsGaucheAP(r);
		while (echange && estExistNoeud(fg, *t)) {
			int fd = filsDroitAP(r);
			int fmin = fg;

			if (estExistNoeud(fd, *t) && freq[t->tab[fd]] < freq[t->tab[fg]]) {
				fmin = fd;
			}
			if (freq[e] <= freq[t->tab[fmin]]) {
				echange = 0;
			}
			else {
				t->tab[r] = donneeAP(fmin, *t);
				r = fmin;
				fg = filsGaucheAP(r);
			}
			t->tab[r] = e;
		}
	}
}
