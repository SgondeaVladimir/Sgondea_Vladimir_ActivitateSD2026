#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Pilot {
	int id;
	float greutate;
	char* echipa;
	int nrUsi; //verifica cate usid e acces are avionul. Util in cazul unei evacuari;
};
typedef struct Pilot Pilot;

struct NodArbore {
	Pilot info;
	struct NodArbore* st;
	struct NodArbore* dr;

};
typedef struct NodArbore NodArbore;


void inserare(NodArbore** radacina, Pilot pilotNou) {
	if (*radacina == NULL) {
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->info = pilotNou;
		nou->dr = NULL;
		nou->st = NULL;
		*radacina = nou;
	}
	else if (pilotNou.id < (*radacina)->info.id) {
		inserare(&((*radacina)->st), pilotNou);
	}
	else {
		inserare(&((*radacina)->dr), pilotNou);
	}
}

//Mereu cand citim din fisier lista/graficul/arborele este gol
NodArbore* citire(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f == NULL) {
		printf("Eroare la deschidere\n");
		return NULL;
	}
	NodArbore* radacina = NULL;
	while (!feof(f)) {
		char buffer[256];
		char sep[3] = ",\n";
		fgets(buffer, 256, f);
		char* aux;
		Pilot p;

		p.id = atoi(strtok(buffer, sep)); //primul elem e cu buffer
		p.greutate = atof(strtok(NULL, sep));

		aux = strtok(NULL, sep);       //modul de functionare pentru char*
		p.echipa = malloc(strlen(aux) + 1);
		strcpy(p.echipa, aux);

		p.nrUsi =atoi(strtok(NULL, sep));

		inserare(&radacina, p);
	}
	fclose(f);
	return radacina;
}

void afisare(Pilot p) {
	printf("Id: %d\n", p.id);
	printf("Greutate: %.2f\n", p.greutate);
	printf("Echipe: %s\n", p.echipa);
	printf("Nr usi: %d\n", p.nrUsi);
	printf("\n\n\n");

}

void InOrdine(NodArbore* radacina) {
	if (radacina) {
		InOrdine(radacina->st);
		afisare(radacina->info);
		InOrdine(radacina->dr);
	}
}

int totalPiloti(NodArbore* radacina, char* numeEchipa) {
	if (radacina == NULL)return 0;
	if (strcmp(radacina->info.echipa, numeEchipa) == 0) {
		return 1 + totalPiloti(radacina->st, numeEchipa) + totalPiloti(radacina->dr, numeEchipa);
	}
	else {
		return totalPiloti(radacina->st, numeEchipa) + totalPiloti(radacina->dr, numeEchipa);

	}
}

Pilot cautaPilot(NodArbore* radacina, int id) {
	if (radacina == NULL) {
		Pilot p;
		p.id = -1;
		return p;
	}
	if (radacina->info.id == id) {
		Pilot p = radacina->info;
		p.echipa = malloc(strlen(radacina->info.echipa) + 1);
		strcpy(p.echipa, radacina->info.echipa);
		return p;
	}
	if (id < radacina->info.id) {
		return cautaPilot(radacina->st, id);
	}
	if (id > radacina->info.id) {
		return cautaPilot(radacina->dr, id);
	}

}

int calculeazaInaltimeArbore(NodArbore* radacina) {
	if (radacina == NULL)return 0;

	int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
	int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);

	if (inaltimeSt > inaltimeDr)return 1 + inaltimeSt;
	else return 1 + inaltimeDr;

	return 0;
}


struct NodStiva {
	Pilot info;
	struct NodStiva* next;
};
typedef struct NodStiva NodStiva;

void deepCopy(NodStiva** stack, Pilot pilot)
{
	NodStiva* temp = (NodStiva*)malloc(sizeof(NodStiva));
	temp->info = pilot;

	temp->info.echipa = (char*)malloc(strlen(pilot.echipa) + 1);
	strcpy(temp->info.echipa, pilot.echipa);

	temp->next = *stack;
	*stack = temp;
}

void pushToStack(NodArbore* radacina, NodStiva** stack) {
	if (radacina == NULL) {
		return;
	}

	pushToStack(radacina->st, stack);
	deepCopy(stack, radacina->info);
	pushToStack(radacina->dr, stack);
}

void afisareStack(NodStiva* stiva) {
	while (stiva) {
		afisare(stiva->info);
		stiva = stiva->next;
	}
}

int calculTotalUsi(NodArbore* radacina) {
	if (radacina == NULL)return 0;
	return radacina->info.nrUsi + calculTotalUsi(radacina->st) + calculTotalUsi(radacina->dr);
}

void dezalocareArbore(NodArbore** radacina) {
	if (*radacina != NULL) {
		dezalocareArbore(&((*radacina)->st));
		dezalocareArbore(&((*radacina)->dr));
		free((*radacina)->info.echipa);
		free(*radacina);

		*radacina = NULL;

	}
}


//Inordine(Stânga - Rădăcină - Dreapta) : Îți dă valorile sortate crescător. (8, 10, 12, 15, 20).
//
//Preordine(Rădăcină - Stânga - Dreapta) : Folosită pentru a copia arborele.
//
//Postordine(Stânga - Dreapta - Rădăcină) : Folosită pentru a șterge(dezaloca) arborele.

int main() 
{
	NodArbore* radacina = citire("piloti.txt");
	InOrdine(radacina);

	int nrPiloti = totalPiloti(radacina, "Forza");
	printf("%d\n", nrPiloti);
	Pilot p = cautaPilot(radacina, 7);
	printf("%.2f", p.greutate);

	dezalocareArbore(&radacina);

}