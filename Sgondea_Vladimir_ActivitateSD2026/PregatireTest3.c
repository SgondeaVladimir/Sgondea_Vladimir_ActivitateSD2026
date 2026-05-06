//#define _CRT_SECURE_NO_WARNINGS
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//
//
////1. Definirea Structurii(1p)
////Defini?i structura Bilet care s? con?in? :
////
////id_bilet(int)
////
////film(char*)
////
////sala(int)
////
////pret(float)
////
////cod_loc(int)
////
////2. Coad? ?i Stiv?(2p)
////Scrie?i func?iile necesare pentru a gestiona biletele astfel :
////
////Crea?i o Coad? în care s? insera?i cel pu?in 5 bilete citite dintr - un fi?ier / vector.
////
////Extrage?i toate elementele din Coad? ?i introduce?i - le într - o Stiv?, dar numai dac? biletul are un pre? mai mare de 20 RON.Biletele care nu îndeplinesc condi?ia vor fi dezalocate imediat.
////
////3. Lista Simpl?(1.5p)
////Extrage?i toate elementele din Stiv? ?i salva?i - le într - o List? Simpl?, ordonat? cresc?tor dup? id_bilet.La final, afi?a?i lista.
////
////4. Tabela de Dispersie(Linear Probing) (2p)
////Implementa?i o func?ie care copiaz? toate elementele din Lista Simpl? într - o Tabel? de Dispersie cu o dimensiune de cel pu?in 10 elemente.
////
////Cheia de c?utare este numele filmului(film).
////
////Mecanismul de tratare a coliziunilor este Linear Probing(pas + 1).
////
////Cele dou? structuri NU partajeaz? memorie(Deep Copy).
////
////5. Calcul ?i Procesare(1.5p)
////Scrie?i o func?ie care parcurge Tabela de Dispersie ?i determin? suma total? încasat? pentru un anumit film(primit ca parametru).Rezultatul se afi?eaz? la consol?.
////
////6. Dezalocare(1p + 1p)
////Scrie?i secven?ele de cod care dezaloc? Tabela de Dispersie ?i Lista Simpl?.
//
//
//struct Bilet {
//	int id_bilet;
//	char* film;
//	int sala;
//	float pret;
//	int cod_loc;
//};
//typedef struct Bilet Bilet;
//
//struct Nod {
//	Bilet info;
//	struct Nod* next;
//
//};
//typedef struct Nod Nod;
//
//
//void pushStack(Nod** varf,Bilet b) {
//	
//	Nod* nou = malloc(sizeof(Nod));
//	nou->info = b;
//	nou->next = NULL;
//	if (*varf == NULL)*varf = nou;
//	else {
//		nou->next = *varf;
//		*varf = nou;
//	}
//}
//
//
//Bilet popStack(Nod** varf) {
//	if (*varf == NULL) {
//		Bilet b;
//		b.id_bilet = -1;
//		return b;
//	}
//	Bilet b = (*varf)->info;
//	Nod* temp = *varf;
//	*varf = (*varf)->next;
//	free(temp);
//
//	return b;
//}
//typedef struct NodDublu NodDublu;
//struct NodDublu {
//	Bilet info;
//	NodDublu* next;
//	NodDublu* prev;
//	
//}; 
//typedef struct ListaDubla ListaDubla;
//struct ListaDubla {
//	NodDublu* prim;
//	NodDublu* ultim;
//};
//
//void enqueue(ListaDubla* lista, Bilet b) {
//	/*if (lista == NULL) {
//		lista->prim->info=
//	}*/
//	NodDublu* nou = malloc(sizeof(NodDublu));
//	nou->info = b;
//	nou->prev = NULL;
//	nou->next = lista->prim;
//	if (lista->prim == NULL) {
//		lista->prim = nou;
//		lista->ultim = nou;
//	}
//	else {
//		lista->prim->prev = nou;
//		lista->prim = nou;
//	}
//
//}
//
//Bilet dequeue(ListaDubla* lista) {
//	
//	if (lista->prim == NULL) {
//		Bilet b;
//		b.id_bilet = -1;
//		return b;
//	}
//Bilet b=lista->ultim->info;
//NodDublu* deSters = lista->ultim;
//
//if (lista->prim == lista->ultim) {
//	lista->prim = NULL;
//	lista->ultim = NULL;
//}
//else {
//	lista->ultim = lista->ultim->prev;
//	lista->ultim->next = NULL;
//}
//
//free(deSters);
//return b;
//
//}
//
