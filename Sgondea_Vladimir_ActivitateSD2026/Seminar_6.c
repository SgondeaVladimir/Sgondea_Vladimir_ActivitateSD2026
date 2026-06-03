
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}


struct Nod {
	Masina masina;
	struct Nod* next;
};
typedef struct Nod Nod;



void pushStack(Nod** stiva, Masina masina) {
	Nod* first = (Nod*)malloc(sizeof(Nod));
	first->masina = masina;
	first->next = *stiva;
	*stiva = first;

}

Masina popStack(Nod** stiva) {
	if ((*stiva) == NULL) {
		Masina rezultat;
		rezultat.id = -1;
		return rezultat;
	}
	Masina rezultat = (*stiva)->masina;
	Nod* temp = (*stiva)->next;
	free(*stiva);
	*stiva = temp;
	return rezultat;

}

int isEmptyStack(Nod* stiva) {
	return stiva == NULL;
}

void* citireStackMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		return NULL;
	}
	Nod* stiva = NULL;
	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		pushStack(&stiva, masina);
	}
	fclose(file);
	return stiva;

}

typedef struct NodDublu NodDublu;
struct NodDublu {
	Masina info;
	NodDublu* next;
	NodDublu* prev;
};

typedef struct LDI LDI;
struct LDI {
	NodDublu* first;
	NodDublu* last;
};

void dezalocareStivaDeMasini(Nod** stiva) {
	while (*stiva) {
		Nod* temp = (*stiva)->next;
		free((*stiva)->masina.model);
		free((*stiva)->masina.numeSofer);
		free(*stiva);
		*stiva = temp;
	}
}

int size(Nod** stiva) {
	int size = 0;
	Nod* stivaNoua = NULL;
	while (*stiva) {
		pushStack(&stivaNoua, popStack(stiva));
		size++;
	}
	while (stivaNoua) {
		pushStack(stiva, popStack(&stivaNoua));
	}
	return size;
}

void enqueue(LDI* coada, Masina masina) {
	NodDublu* newNod = (NodDublu*)malloc(sizeof(NodDublu));
	newNod->info = masina;
	newNod->next = NULL;
	newNod->prev = coada->last;
	if (coada->last) {
		coada->last->next = newNod;
	}
	else {
		coada->first = newNod;
	}
	coada->last = newNod;

}

Masina dequeue(LDI* coada) {
	Masina rezultat;
	rezultat.id = -1;
	if (coada->first) {
		rezultat = coada->first->info;
		NodDublu* temp = coada->first;
		coada->first = temp->next;
		free(temp);
	}

	return rezultat;
}

LDI citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	LDI coada;
	coada.first = coada.last = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		return coada;
	}
	while (!feof(file)) {
		enqueue(&coada, citireMasinaDinFisier(file));
	}
	fclose(file);
	return coada;
}

void dezalocareCoadaDeMasini(LDI* coada) {
	while (coada->first) {

		NodDublu* temp = coada->first->next;
		free(coada->first->info.numeSofer);
		free(coada->first->info.model);
		free(coada->first);
		coada->first = temp;
	}

	coada->last = NULL;
}

Masina getMasinaByID(Nod** stiva, int id) {
	Masina rezultat;
	if ((*stiva) == NULL) {
		rezultat.id = -1;
		return rezultat;
	}
	Nod* stivaNoua = NULL;
	while ((*stiva)) {
		Masina masinaNoua = popStack(stiva);
		if (masinaNoua.id == id) {
			rezultat = masinaNoua;
			break;
		}
		else {
			pushStack(&stivaNoua, masinaNoua);
		}

	}
	while (stivaNoua) {
		pushStack(stiva, popStack(&stivaNoua));
	}

	return rezultat;


}

float calculeazaPretTotal(Nod** stiva) {
	float pretTotal = 0;
	int count = 0;
	Nod* stivaNoua = NULL;
	while ((*stiva)) {
		Masina masinaNoua = popStack(stiva);
		pretTotal += masinaNoua.pret;
		count++;
		pushStack(&stivaNoua, masinaNoua);
	}
	while (stivaNoua) {
		pushStack(stiva, popStack(&stivaNoua));
	}
	return (count != 0) ? pretTotal / (float)count : 0;
}
int isEmptyQueue(LDI* coada) {
	return coada->first == NULL;
}
int main() {

	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	printf("Se afla %d masini in stiva.\n\n", size(&stiva));

	printf("PopStack call:\n");
	afisareMasina(popStack(&stiva));

	printf("PopStack call:\n");
	afisareMasina(popStack(&stiva));

	printf("Afisare masina cu ID 2:\n");
	afisareMasina(getMasinaByID(&stiva, 2));

	printf("Test dezalocare masini din stiva:\n");
	dezalocareStivaDeMasini(&stiva);
	printf("Functia de isEmptyStack returneaza %d (1-true,0-false)\n\n", isEmptyStack(stiva));

	stiva = citireStackMasiniDinFisier("masini.txt");
	printf("Valoarea medie a masinilor este de %1.2f um\n\n", calculeazaPretTotal(&stiva));
	dezalocareStivaDeMasini(&stiva);

	printf("Afisare prima masina din coada:\n");
	LDI coada = citireCoadaDeMasiniDinFisier("masini.txt");
	afisareMasina(dequeue(&coada));
	printf("\n");

	coada = citireCoadaDeMasiniDinFisier("masini.txt");
	printf("Test dezalocare masini din coada:\n");
	dezalocareCoadaDeMasini(&coada);
	printf("Functia de isEmptyQueue returneaza %d (1-true,0-false)\n\n", isEmptyQueue(&coada));

	return 0;
}
