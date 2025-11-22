#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tas.h"

#define INDEFINI -100000


int estFeuille(int racine, int nbCarAlphFreq, int* FG, int* FD) {
	return (FG[racine] == INDEFINI && FD[racine] == INDEFINI);
}

int calculFreq(int nbCarAlph, char* msg, int* freq){
	for (int i = 0; i < nbCarAlph; i++) {
		freq[i] = 0;
	}
	char c = 'a';
	int codeA = c;
	int lgMSG = strlen(msg);
	for (int i = 0; i < lgMSG; i++) {
		char c = msg[i];
		int numCar = (int)c - codeA;
		freq[numCar]++;
	}
	int nbCarAlphFreq = 0;
	for (int i = 0; i < nbCarAlph; i++) {
		if (freq[i] > 0) {
			nbCarAlphFreq++;
		}
	}
	return nbCarAlphFreq;
}	

void creatArbreHuffmann(int nbCarAlph, int nbCarAlphFreq, int* freq, int* pere, int* filsG, int* filsD, int* racine) {
	Tas tas;
	//init
	tas = allocMemTas(nbCarAlphFreq);
	for (int c = 0; c < nbCarAlphFreq; c++) {
		if (freq[c] > 0) {
			inserTas(c, &tas, freq);
		}
		filsG[c] = INDEFINI;
		filsD[c] = INDEFINI;
	}
	//fin des insertions
	//le tas est complet

	//traitement
	for (int c = nbCarAlphFreq; c < (2*nbCarAlphFreq - 1); c++) {
		TElement c1 = minTas(tas);
		suppTas(&tas, freq);

		TElement c2 = minTas(tas);
		suppTas(&tas, freq);
		freq[c] = freq[c1] + freq[c2];
		pere[c1] = -c;
		pere[c2] = c;
		filsG[c] = c1;
		filsD[c] = c2;
		inserTas(c, &tas, freq);
	}

	//fin du traitement
	*racine = minTas(tas);
	suppTas(&tas, freq);
	pere[*racine] = INDEFINI;
	tas = libMemTas(tas);
}

char* codage(int nd, int* tab_pere) {
	if (tab_pere[nd] == INDEFINI){
		return "";
	}
	else {
		int abs_pere = abs(tab_pere[nd]);
		char* codage_pere = codage(abs(tab_pere[nd]), tab_pere);
		char* dernier_bit = (tab_pere[nd] < 0) ? "0" : "1";
		int len = strlen(codage_pere) + strlen(dernier_bit) + 1;
		char* res = malloc(len);
		if (res == NULL) {
			free(codage_pere);
			return NULL;
		}

		// copier et concaténer
		strcpy_s(res, len, codage_pere);
		strcat_s(res, len, dernier_bit);

		return res;
	}
}

void creatTableCodage(int nbCarAlphFreq, int* freq, int* pere, char** tabCode) {
	for (int c = 0; c < nbCarAlphFreq; c++) {
		if (freq[c] > 0) {
			tabCode[c] = codage(c, pere);
		}
	}
}

char* codageTexte(char* msgSrc, char** tabCode, int nbCarAlph) {
	char* msgCode = malloc(1);
	if (!msgCode) return NULL;
	msgCode[0] = '\0';

	size_t longMsg = strlen(msgSrc);

	for (size_t i = 0; i < longMsg; i++) {
		char c = msgSrc[i];
		int idx = c - 'a';          // index dans tabCode
		if (idx < 0 || idx >= nbCarAlph) continue;

		char* code = tabCode[idx];
		if (!code) code = "";

		size_t newSize = strlen(msgCode) + strlen(code) + 1;
		char* tmp = realloc(msgCode, newSize);
		if (!tmp) { free(msgCode); return NULL; }
		msgCode = tmp;

		strcat_s(msgCode, newSize, code);
	}

	return msgCode;
}

void decodage(int* ind, int lgMsgCode, char* Alphabet, char* MsgCode, char* MsgSrc, int* posMsgSrc, int racine, int* FG, int* FD, int nbCarAlphFreq){
	if (*ind > lgMsgCode) return; // condition d'arrêt
	if (estFeuille(racine, nbCarAlphFreq, FG, FD)) {
		// on a atteint une feuille : on ajoute le caractère au message décodé
		MsgSrc[*posMsgSrc] = Alphabet[racine];
		(*posMsgSrc)++;
		MsgSrc[*posMsgSrc] = '\0';
	}
	else {
		// descente dans l'arbre selon le bit
		if (MsgCode[*ind] == '0') {
			racine = FG[racine];
		}
		else {
			racine = FD[racine];
		}
		(*ind)++;
		decodage(ind, lgMsgCode, Alphabet, MsgCode, MsgSrc, posMsgSrc, racine, FG, FD, nbCarAlphFreq);
	}
}

void decodageMessage(char* Alphabet, char* MsgCode, int Racine, int* FG, int* FD, char* MsgSrc, int nbCarAlphFreq){
	int lgMsgCode = strlen(MsgCode);
	int ind = 0;          // position dans MsgCode
	int posMsgSrc = 0;    // position dans MsgSrc
	
	// MsgSrc = chaîne vide
	MsgSrc[0] = '\0';

	while (ind < lgMsgCode) {
		decodage(&ind, lgMsgCode, Alphabet, MsgCode, MsgSrc, &posMsgSrc,
			Racine, FG, FD, nbCarAlphFreq);
	}

	// Terminer la chaîne
	MsgSrc[posMsgSrc] = '\0';
}









int main(void) {
	int racine = 0;
	int nbCarAlph = 27;
	char* msg = "aaabbbccddee";
	//char* msg = "abadbabababbabacccddeeddbababaabafddddddggghhhiiijjjjkkkkllkkkkmmmmnnnnnooooopppppppppppppppppppppppppppppppppppppppppppppppppqqqqrrsstttuuvvwwwxxxyyyzzzz";
	int* freq = (int*)malloc((2*nbCarAlph-1)*sizeof(int));
	int nbCarAlphFreq = calculFreq(nbCarAlph, msg, freq);
	int tailleArbre = nbCarAlphFreq * 2 - 1;
	int* pere = (int*)malloc(tailleArbre * sizeof(int));
	int* filsG = (int*)malloc(tailleArbre * sizeof(int));
	int* filsD = (int*)malloc(tailleArbre * sizeof(int));
	for (int i = 0; i < ((2 * nbCarAlphFreq) - 1); i++) {
		pere[i] = INDEFINI;
		filsG[i] = INDEFINI;
		filsD[i] = INDEFINI;
	}

	char* alphabet = (char*)malloc(nbCarAlph * sizeof(char));
	for (int i = 0; i < nbCarAlph; i++) alphabet[i] = '\0';
	printf("nombre de caracteres frequents : %d \n", nbCarAlphFreq);
	printf("tableau de frequences : ");
	for (int i = 0; i < nbCarAlphFreq; i++) {
		printf("%d ", freq[i]);
		int code = i + (int)'a';
		alphabet[i] = (char)code;
	}




	creatArbreHuffmann(nbCarAlph, nbCarAlphFreq, freq, pere, filsG, filsD, &racine);

	printf("\ntableau pere : \n");
	for (int i = 0; i < 2 * nbCarAlphFreq - 1; i++) {
		printf("%d : %d \n", i, pere[i]);
	}

	printf("\ntableau filsGauche : \n");
	for (int i = 0; i < 2 * nbCarAlphFreq - 1; i++) {
		printf("%d : %d \n", i, filsG[i]);
	}

	printf("\ntableau filsDroit : \n");
	for (int i = 0; i < 2 * nbCarAlphFreq - 1; i++) {
		printf("%d : %d \n", i, filsD[i]);
	}

	char** tabCode = malloc(nbCarAlphFreq * sizeof(char*));
	for (int i = 0; i < nbCarAlphFreq; i++) tabCode[i] = NULL;
	creatTableCodage(nbCarAlphFreq, freq, pere, tabCode);
	for (int i = 0; i < nbCarAlphFreq; i++) {
		if (tabCode[i] != NULL)
			printf("Caractere %d : %s\n", i, tabCode[i]);
	}

	printf("\nMessage d'origine : %s\n", msg);
	char* msgCode = codageTexte(msg, tabCode, nbCarAlph);
	if (msgCode != NULL) {
		printf("\nMessage code : %s\n", msgCode);
	}

	char* msgSrc = malloc(strlen(msg) + 1);
	decodageMessage(alphabet, msgCode, racine, filsG, filsD,msgSrc, nbCarAlphFreq);
	printf("\nMessage decode : %s\n\n\n", msgSrc);
	return 1;
}