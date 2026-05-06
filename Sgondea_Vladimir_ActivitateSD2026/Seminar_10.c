#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare
struct NodArbore {
	Masina info;
	struct NodArbore* st;
	struct NodArbore* dr;
	int inaltime;
};
typedef struct NodArbore NodArbore;

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

int calculeazaInaltimeArbore(NodArbore* radacina) {
//	//calculeaza inaltimea arborelui care este data de 
//	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (radacina == NULL) {
		return 0;
	}

	int inaltimeStanga = calculeazaInaltimeArbore(radacina->st);
	int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dr);

	if (inaltimeStanga > inaltimeDreapta) {
		return 1 + inaltimeStanga;
	}
	else {
		return 1 + inaltimeDreapta;
	}

}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.
int inaltime(NodArbore* nod) {
	if (nod == NULL) return 0;
	return nod->inaltime;
}
int maxim(int a, int b) {
	return (a > b) ? a : b;
}
int getFactorEchilibru(NodArbore* nod) {
	if (nod == NULL) return 0;
	return inaltime(nod->st) - inaltime(nod->dr);
}


NodArbore* rotireDreapta(NodArbore* y) {
	NodArbore* x = y->st;
	NodArbore* T2 = x->dr;
	x->dr = y;
	y->st = T2;
	y->inaltime = maxim(inaltime(y->st), inaltime(y->dr)) + 1;
	x->inaltime = maxim(inaltime(x->st), inaltime(x->dr)) + 1;
	return x;
}

NodArbore* rotireStanga(NodArbore* x) {
	NodArbore* y = x->dr;
	NodArbore* T2 = y->st;
	y->st = x;
	x->dr = T2;
	x->inaltime = maxim(inaltime(x->st), inaltime(x->dr)) + 1;
	y->inaltime = maxim(inaltime(y->st), inaltime(y->dr)) + 1;
	return y;
}

void adaugaMasinaInArboreEchilibrat(NodArbore** nod, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
	//dupa o anumita cheie pe care o decideti - poate fi ID
	if (*nod == NULL) {
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->info = masinaNoua;
		nou->st = nou->dr = NULL;
		nou->inaltime = 1;
		*nod = nou;
		return;
	}

	if (masinaNoua.id < (*nod)->info.id) {
		adaugaMasinaInArboreEchilibrat(&((*nod)->st), masinaNoua);
	}
	else if (masinaNoua.id > (*nod)->info.id) {
		adaugaMasinaInArboreEchilibrat(&((*nod)->dr), masinaNoua);
	}
	
	(*nod)->inaltime = 1 + maxim(inaltime((*nod)->st), inaltime((*nod)->dr));

	int balance = getFactorEchilibru(*nod);


	if (balance >= 2 && masinaNoua.id < (*nod)->st->info.id) {
		*nod = rotireDreapta(*nod);
		return;
	}

	if (balance <= -2 && masinaNoua.id > (*nod)->dr->info.id) {
		*nod = rotireStanga(*nod);
		return;
	}

	if (balance >= 2 && masinaNoua.id > (*nod)->st->info.id) {
		(*nod)->st = rotireStanga((*nod)->st);
		*nod = rotireDreapta(*nod);
		return;
	}

	if (balance <= -2 && masinaNoua.id < (*nod)->dr->info.id) {
		(*nod)->dr = rotireDreapta((*nod)->dr);
		*nod = rotireStanga(*nod);
		return;
	}

}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	if (!f)return NULL;
	NodArbore* radacina = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArboreEchilibrat(&radacina, m);

	}
	fclose(f);
	return radacina;

}
void inordine(NodArbore* r) {
	if (r) {
		inordine(r->st);
		afisareMasina(r->info);
		inordine(r->dr);
	}
}

void afisareMasiniDinArbore(NodArbore* r) {
	printf("Afisare Inordine\n");
	inordine(r);
}

void dezalocareArboreDeMasini(NodArbore** r) {
	if (*r) {
		dezalocareArboreDeMasini(&((*r)->st));
		dezalocareArboreDeMasini(&((*r)->dr));
		free((*r)->info.model);
		free((*r)->info.numeSofer);
		free(*r);
		*r = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(NodArbore* radacina, int id) {
	if (radacina == NULL) {
		Masina mGola;
		mGola.id = -1;
		return mGola;
	}

	if (radacina->info.id == id) {
		Masina m;
		m.id = radacina->info.id;
		m.model = (char*)malloc(strlen(radacina->info.model) + 1);
		strcpy(m.model, radacina->info.model);

		m.nrUsi = radacina->info.nrUsi;

		m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy(m.numeSofer, radacina->info.numeSofer);

		m.pret = radacina->info.pret;
		m.serie = radacina->info.serie;
		return m;

	}
	if (id < radacina->info.id) {
		return getMasinaByID(radacina->st, id);

	}
	if (id > radacina->info.id) {
		return getMasinaByID(radacina->dr, id);

	}
}

int determinaNumarNoduri(NodArbore* radacina) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	if (radacina == NULL)return 0;
	return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
}

float calculeazaPretTotal(NodArbore* radacina) {
	//calculeaza pretul tuturor masinilor din arbore.

	if (radacina == NULL)return 0;
	return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* radacina, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	if (radacina == NULL)return 0;

	float pretCurent;
	if (strcmp(numeSofer, radacina->info.numeSofer) == 0) {
		pretCurent = radacina->info.pret;
	}
	return pretCurent + calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);


	return 0;
}
int main() {
	NodArbore* arbore = (NodArbore*)citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(arbore);
	printf("\nTotal noduri: %d", determinaNumarNoduri(arbore));
	printf("\nPret total: %.2f", calculeazaPretTotal(arbore));
	dezalocareArboreDeMasini(&arbore);
	return 0;
}