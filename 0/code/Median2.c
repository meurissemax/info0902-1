#include <stdlib.h>
#include <string.h>
#define uint unsigned int
#define N 101

/* Fonction qui génère des entiers signés issus d'un intervalle donné */
int f(int m, int M, uint* s) {
    *s^= (uint)(*s << 13);
    s[0] ^= (uint)(*s >> 17);
    *s ^= (uint)(s[0] << 5);

    return m + (*s%(M-m));
}

/* Fonction qui inverse deux valeurs, selon la valeur de la variable c */
void f2(int* a, int* b, int c) {
	if(c == 0)
		return;
	
	int t = *b;
	*b = *a;
	*a = t;
}

/* Fonction qui permet de trier un tableau par ordre croissant */
void f1(int* a) {
    int i, j;

    for(i = 0; i < N; i++)
    	for(j = (N - 1); j > i; --j)
    		f2(&a[j], &a[j-1], a[j] < a[j-1]);
}

int median() {
	// Déclaration des variables
	uint s = 1;
	int b = 0, i;
	int* a = NULL;
	a = malloc(N * sizeof(int));

	// On génère un tableau contenant des entiers signés aléatoires
	for(i = 0; i < N; i++)
		a[i] = f(-10, 11, &s);

	// On trie le tableau et on calcule la valeur médianne
	f1(a);
	b = a[N/2];

	// On libère le tableau alloué dynamiquement
	free(a);

	// On retourne la valeur médianne du tableau
	return b;
}