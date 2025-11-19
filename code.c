#include <stdio.h>
#include <malloc.h>
#include <string.h>

int calculFreq(int nbCarAlph, char* msg, int* freq) {
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

void creatArbreHuffmann(int nbCarAlph, int nbCarAlphFreq, int* freq, int* pere, int* filsG, int* filsD, int Racine) {
}




int main(void) {
	printf("test");
	char* msg = "abadbacabccbccbcde";
	int* freq = (int*)malloc(8* sizeof(int));
	printf("nombre de caracteres frequents : %d \n", calculFreq(8, msg, freq));
	printf("tableau de frequences : ");
	for (int i = 0; i < 8; i++) {
		printf("%d ", freq[i]);
	}
	return 1;
}