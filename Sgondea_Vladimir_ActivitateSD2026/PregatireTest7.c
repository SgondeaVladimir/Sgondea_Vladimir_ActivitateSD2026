#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Implementați o aplicație în limbajul C pentru gestionarea unei rețele sociale a piloților de Formula 1 folosind un Graf Neorientat stocat sub formă de listă de liste(Listă Principală pentru piloți, Liste Secundare pentru conexiuni).
//
//Definiți structura Pilot ce conține : id(int), nume(char*), echipa_curenta(char*), și varsta(int).Definiți structurile NodP(Lista Principală) și NodS(Lista Secundară) necesare stocării grafului. (1 p.)
//
//Implementați funcțiile de inserare la final atât pentru Lista Principală, cât și pentru Lista Secundară. (1.5 p.)
//
//Implementați funcția de căutare a unui nod în Lista Principală după id(returnează pointer către nodul găsit). (0.5 p.)
//
//Implementați funcția de inserare muchie neorientată între doi piloți, bazați pe ID - urile lor. (1 p.)
//
//Implementați funcțiile de citire a datelor din două fișiere text : piloti.txt(pentru noduri) și coechipieri.txt(pentru muchii - format: ID1 ID2). (2 p.)
//
//Implementați o funcție care afișează toți piloții din graf, alături de numărul lor de conexiuni(gradul nodului). (1.5 p.)
//
//Implementați o funcție care determină și afișează dacă doi piloți(dați prin ID) sunt conectați direct(sunt coechipieri). (1 p.)
//
//Implementați funcția de eliberare a întregii memorii alocate în HEAP pentru graf(noduri principale, noduri secundare, și deep - copies / stringuri). (1.5 p.)



struct Pilot {
	int id;
	char* nume;
	char* echipa_curenta;
	int varsta;
};

typedef struct Pilot Pilot;

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Pilot info;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodP* info;  //pointer catre pilotul din lista principala
	NodS* next;
};

void inserareInListaP(NodP** graf, Pilot p) {
	NodP* nou = malloc(sizeof(NodP));
	nou->info = p;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*graf) {
		NodP* aux = *graf;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*graf = nou;
	}
}

void inserareInListaS(NodS** lista, NodP* vecin) {
	NodS* nou = malloc(sizeof(NodS));
	nou->info = vecin;
	nou->next = NULL;

	if (*lista) {
		NodS* aux = *lista;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*lista = nou;
	}
}

NodP* cautaNodDupaId(NodP* graf, int id) {
	NodP* aux = graf;
	while (aux) {
		if (aux->info.id == id)return aux;
		aux = aux->next;
	}
	return NULL;
}

//Implementați funcția de inserare muchie neorientată între doi piloți, bazați pe ID - urile lor. (1 p.)
void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
	NodP* start = cautaNodDupaId(listaPrincipala, idStart);
	NodP* stop = cautaNodDupaId(listaPrincipala, idStop);

	if (start && stop) {
		inserareInListaS(&start->vecini, stop);
		inserareInListaS(&stop->vecini, start);
	}
}


//Implementați funcțiile de citire a datelor din două fișiere text : piloti.txt(pentru noduri) și coechipieri.txt(pentru muchii - format: ID1 ID2). (2 p.)
NodP* citireNoduriDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) {
		printf("Eroare la deschiderea fisierului de noduri\n");
		return NULL;
	}

	NodP* graf = NULL;

	while (!feof(f)) {
		char buffer[100];
		char sep[3] = ",\n";
		fgets(buffer, 100, f);

		Pilot p;
		char* aux;

		p.id = atoi(strtok(buffer, sep));

		aux = strtok(NULL, sep);
		p.nume = malloc(strlen(aux) + 1);
		strcpy(p.nume, aux);

		aux = strtok(NULL, sep);
		p.echipa_curenta = malloc(strlen(aux) + 1);
		strcpy(p.echipa_curenta, aux);

		p.varsta = atoi(strtok(NULL, sep));

		inserareInListaP(&graf, p);
	}
	fclose(f);
	return graf;
	

}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la citirea fisierului de muchii\n");
			return;
		
	}
	while (!feof(f)) {
		int idStart;
		int idStop;
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(graf, idStart, idStop);
	}
	fclose(f);

}


//Implementați o funcție care afișează toți piloții din graf, alături de numărul lor de conexiuni(gradul nodului). (1.5 p.)

void afisareGraf(NodP* graf) {
	NodP* curent = graf;
	while (curent != NULL) {
		// Numaram vecinii
		int nrConexiuni = 0;
		NodS* vecin = curent->vecini;
		while (vecin != NULL) {
			nrConexiuni++;
			vecin = vecin->next;
		}

		printf("Pilotul %s (ID: %d) are %d fosti coechipieri.\n",
			curent->info.nume, curent->info.id, nrConexiuni);

		curent = curent->next;
	}
}

//Implementați o funcție care determină și afișează dacă doi piloți(dați prin ID) sunt conectați direct(sunt coechipieri). (1 p.)


void verificaPrietenie(NodP* graf, int id1, int id2) {
	NodP* pilot1 = cautaNodDupaId(graf, id1);
	NodP* pilot2 = cautaNodDupaId(graf, id2);

	if (pilot1 == NULL || pilot2 == NULL) {
		printf("Unul sau ambii piloti nu exista in graf.\n");
		return;
	}

	// Cautam pe id2 in lista de vecini a lui id1
	NodS* curent = pilot1->vecini;
	int gasit = 0;
	while (curent != NULL) {
		if (curent->info->info.id == id2) {
			gasit = 1;
			break;
		}
		curent = curent->next;
	}

	if (gasit) {
		printf("Da, %s si %s sunt/au fost coechipieri.\n", pilot1->info.nume, pilot2->info.nume);
	}
	else {
		printf("Nu, %s si %s nu au nicio conexiune directa.\n", pilot1->info.nume, pilot2->info.nume);
	}
}


void dezalocareGraf(NodP** graf) {
	if (graf == NULL || *graf == NULL) return;

	NodP* curentP = *graf;
	while (curentP != NULL) {
		NodS* curentS = curentP->vecini;
		while (curentS != NULL) {
			NodS* tempS = curentS;
			curentS = curentS->next;
			free(tempS);
		}

		if (curentP->info.nume) free(curentP->info.nume);
		if (curentP->info.echipa_curenta) free(curentP->info.echipa_curenta);

		NodP* tempP = curentP;
		curentP = curentP->next;
		free(tempP);
	}

	*graf = NULL;
}

int main() {


	printf("Citire si Creare Graf\n");
	NodP* retea = citireNoduriDinFisier("piloti3.txt");
	citireMuchiiDinFisier("muchii.txt", retea);

	printf("\nAfisare Grad Noduri\n");
	afisareGraf(retea);

	printf("\nVerificare Conexiuni\n");
	verificaPrietenie(retea, 1, 2); 
	verificaPrietenie(retea, 1, 8); 

	// Dezalocare
	dezalocareGraf(&retea);
	printf("\nMemoria grafului a fost eliberata cu succes!\n");


	return 0;
}

