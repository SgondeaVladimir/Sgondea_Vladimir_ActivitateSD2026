#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define max(a,b)


//Implementați o aplicație în limbajul C pentru gestionarea piloților de Formula 1, asigurând un timp de căutare optim prin utilizarea unui Arbore AVL(Arbore Binar de Căutare Echilibrat).
//
//Definiți structura Pilot ce conține : id(int), nume(char*), echipa(char*), și salariu(float).Definiți structura nodului necesară pentru lucrul cu un Arbore AVL. (1 p.)
//
//Implementați funcțiile ajutătoare necesare echilibrării arborelui : calculul înălțimii, calculul factorului de echilibru și funcțiile pentru rotația la stânga și rotația la dreapta. (2 p.)
//
//Implementați funcția de inserare a unui pilot în Arborele AVL.Cheia de inserare este atributul id.Arborele trebuie să se re - echilibreze automat. (1.5 p.)
//
//Implementați funcția de citire a datelor dintr - un fișier text(piloti.txt) și creați Arborele AVL prin apeluri succesive ale funcției de inserare. (1 p.)
//
//Implementați o funcție de parcurgere care să afișeze toți piloții în consolă, sortați crescător după ID. (0.5 p.)
//
//Implementați o funcție care calculează și returnează salariul total al tuturor piloților din arbore. (1 p.)
//
//Implementați o funcție care caută un pilot după id și îl returnează printr - un deep - copy(alocare de memorie nouă pentru șirurile de caractere).Dacă nu este găsit, returnați un pilot cu id - ul - 1. (1.5 p.)
//
//Implementați o funcție care să returneze nivelul(înălțimea) la care se află arborele.Apelați funcția în main pentru a demonstra că structura este echilibrată. (0.5 p.)
//
//Implementați funcția de eliberare a întregii memorii alocate în HEAP(inclusiv deep - copy - ul și nodurile arborelui). (1 p.)


struct Pilot {
	int id;
	char* nume;
	char* echipa;
	float salariu;
};
typedef struct Pilot Pilot;

struct NodAVL {
	Pilot info;
	struct NodAVL* st;
	struct NodAVL* dr;
	int inaltime;
};
typedef struct NodAVL NodAVL;

int getFactorEchilibru(NodAVL* nod) {
	if (nod == NULL)return 0;
	return inaltimeNod(nod->st) - inaltimeNod(nod->dr);
}
int inaltimeNod(NodAVL* nod) {
	if (nod == NULL)return 0;
	else return nod->inaltime;
}

NodAVL* rotireDreapta(NodAVL* y) {
	NodAVL* x = y->st;
	NodAVL* T2 = x->dr;

	x->dr = y;
	y->st = T2;

	y->inaltime = max(inaltimeNod(y->st), inaltimeNod(y->dr)) + 1;
	x->inaltime = max(inaltimeNod(x->st), inaltimeNod(x->dr)) + 1;

	return x;
}

NodAVL* rotireStanga(NodAVL* x) {
	NodAVL* y = x->dr;
	NodAVL* T2 = y->st;

	y->st = x;
	x->dr = T2;

	x->inaltime = max(inaltimeNod(x->st), inaltimeNod(x->dr)) + 1;
	y->inaltime = max(inaltimeNod(y->st), inaltimeNod(y->dr)) + 1;

	return y;
}

void inserareAVL(NodAVL** radacina, Pilot pilotNou) {
	if ((*radacina) == NULL) {
		NodAVL* nou = malloc(sizeof(NodAVL));
		nou->info = pilotNou;
		nou->st = NULL;
		nou->dr = NULL;
		nou->inaltime = 1;
		*radacina = nou;
		return;
	}
	if (pilotNou.id < (*radacina)->info.id) {
		inserareAVL(&((*radacina)->st), pilotNou);
	}
	if (pilotNou.id > (*radacina)->info.id) {
		inserareAVL(&((*radacina)->dr), pilotNou);
	}

	(*radacina)->inaltime = max(inaltimeNod((*radacina)->st), inaltimeNod((*radacina)->dr))+1;

	int echilibru = getFactorEchilibru(*radacina);

	//Cele 4 cazuri de rotatie
	if (echilibru > 1 && pilotNou.id < (*radacina)->st->info.id) {
		*radacina = rotireDreapta(*radacina);
		return;
	}

	if (echilibru< -1 && pilotNou.id>(*radacina)->dr->info.id) {
		*radacina = rotireStanga(*radacina);
		return;
	}

	if (echilibru > 1 && pilotNou.id > (*radacina)->st->info.id) {
		(*radacina)->st = rotireStanga((*radacina)->st);
		*radacina = rotireDreapta(*radacina);
		return;
	}

	if (echilibru< -1 && pilotNou.id < (*radacina)->dr->info.id) {
		(*radacina)->dr = rotireDreapta((*radacina)->dr);
		*radacina = rotireStanga(*radacina);
		return;
	}
}

NodAVL* citireDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) {
		printf("Fisierul nu a putut fi deschis\n");
		return NULL;
	}

	NodAVL* radacina = NULL;
	while (!feof(f)) {
		char buffer[100];
		char sep[3] = ",\n";
		char* aux;
		Pilot p;
		fgets(buffer, 100, f);

		p.id = atoi(strtok(buffer, sep));

		aux = strtok(NULL, sep);
		p.nume = malloc(strlen(aux) + 1);
		strcpy(p.nume, aux);

		aux = strtok(NULL, sep);
		p.echipa = malloc(strlen(aux) + 1);
		strcpy(p.echipa, aux);

		p.salariu = atof(strtok(NULL, sep));

		inserareAVL(&radacina, p);
	}
	fclose(f);
	return radacina;
}

void afisarePilot(Pilot p) {
	printf("ID: %d\n", p.id);
	printf("Nume: %s\n", p.nume);
	printf("Echipa: %s\n", p.echipa);
	printf("Salariu: %.2f\n", p.salariu);
	printf("\n\n");
}

void parcurgereInOrdine(NodAVL* radacina) {
	if (radacina) {
		parcurgereInOrdine(radacina->st);
		afisarePilot(radacina->info);
		parcurgereInOrdine(radacina->dr);
	}
}

float calculeazaSalariuTotal(NodAVL* radacina) {
	if (radacina == NULL) { return 0; }

	return radacina->info.salariu + calculeazaSalariuTotal(radacina->st) + calculeazaSalariuTotal(radacina->dr);
}

//Implementați o funcție care caută un pilot după id și îl returnează printr - un deep - copy(alocare de memorie nouă pentru șirurile de caractere).Dacă nu este găsit, returnați un pilot cu id - ul - 1. (1.5 p.)
Pilot cautaDupaId(NodAVL* radacina,int id) {
	if (radacina == NULL) {
		Pilot p;
		p.id = -1;
		p.echipa = NULL;
		p.nume = NULL;
		return p;
	}
	if (radacina->info.id == id) {
		Pilot p;
		p = radacina->info;
		p.echipa = malloc(strlen(radacina->info.echipa) + 1);
		strcpy(p.echipa, radacina->info.echipa);
		p.nume = malloc(strlen(radacina->info.nume) + 1);
		strcpy(p.nume, radacina->info.nume);
		return p;
	}

	if (id < radacina->info.id) return cautaDupaId(radacina->st, id);
	if (id > radacina->info.id) return cautaDupaId(radacina->dr, id);

}

int calculeazaInaltimeArbore(NodAVL* radacina) {
	if (radacina == NULL)return 0;
	return radacina->inaltime;
}

void dezalocareAVL(NodAVL** radacina) {
	if (*radacina == NULL)return;
	dezalocareAVL(&(*radacina)->st);
	dezalocareAVL(&((*radacina)->dr));

	free((*radacina)->info.nume);
	free((*radacina)->info.echipa);
	free(*radacina);

	*radacina = NULL;

}

int main() {

	NodAVL* nod = citireDinFisier("piloti2.txt");

	parcurgereInOrdine(nod);

	Pilot p = cautaDupaId(nod, 4);
	printf("///////////////////////////////");
	afisarePilot(p);

	Pilot p1;
	p1.id = 9;
	p1.salariu = 9800.67;
	p1.nume = malloc(strlen("Mirel") + 1);
	strcpy(p1.nume, "Mirel");
	p1.echipa = malloc(strlen("Forza") + 1);
	strcpy(p1.echipa, "Forza");	inserareAVL(&nod, p1);

	parcurgereInOrdine(nod);

	dezalocareAVL(&nod);


}
