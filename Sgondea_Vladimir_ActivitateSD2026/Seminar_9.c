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

struct NodArbore {
	Masina info;
	struct NodArbore* st;   //vecinul din stanga
	struct NodArbore* dr;   //vecinul din dreapta

};

typedef struct NodArbore NodArbore;

//creare structura pentru un nod dintr-un arbore binar de cautare

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArbore(NodArbore** radacina, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID

	if (*radacina == NULL) {
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->info = masinaNoua;
		nou->dr = NULL;
		nou->st = NULL;
		*radacina = nou;
	}
	else if(masinaNoua.id<(*radacina)->info.id){
		adaugaMasinaInArbore(&((*radacina)->st), masinaNoua);

	}
	else if (masinaNoua.id > (*radacina)->info.id) {
		adaugaMasinaInArbore(&((*radacina)->dr), masinaNoua);
	}
	else {
		free(masinaNoua.model);
		free(masinaNoua.numeSofer);
	}
}



NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f=fopen(numeFisier,"r");
	if (f == NULL) {
		printf("Eroare la deschiderea fisierului.\n");
		return NULL;
	}
	NodArbore* radacina=NULL;
	while (!feof(f)) {
		Masina m;
		m = citireMasinaDinFisier(f);
		if (m.id != -1) {
			adaugaMasinaInArbore(&radacina, m);
		}
	}
	fclose(f);
	return radacina;
}

void afisareInOrdine(NodArbore* radacina) {
	if (radacina) {
		afisareInOrdine(radacina->st);
		afisareMasina(radacina->info);
		afisareInOrdine(radacina->dr);
	}
}

void afisareInPreordine(NodArbore* radacina) {
	if (radacina) {
		afisareMasina(radacina->info);
		afisareInPreordine(radacina->st);
		afisareInPreordine(radacina->dr);

	}
}

void afisareInPostordine(NodArbore* radacina) {
	if (radacina) {
		afisareInPostordine(radacina->st);
		afisareInPostordine(radacina->dr);
		afisareMasina(radacina->info);

	}
}

void afisareMasiniDinArbore(NodArbore* radacina) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	printf("Afisarea masinilor in ordine:\n");
	afisareInOrdine(radacina);

	printf("Afisarea masinilor in preordine\n");
	afisareInPreordine(radacina);

	printf("Afisarea masinilor in postordine:\n");
	afisareInPostordine(radacina);


}
void dezalocareArboreDeMasini(NodArbore** radacina) {
	if (*radacina != NULL) {
		dezalocareArboreDeMasini(&((*radacina)->st));
		dezalocareArboreDeMasini(&((*radacina)->dr));

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);

		*radacina = NULL; // Evitam dangling pointers
	}
}
Masina getMasinaByID(NodArbore* radacina,int id) {
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

		m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer)+1);
		strcpy(m.numeSofer, radacina->info.numeSofer);

		m.pret = radacina->info.pret;
		m.serie = radacina->info.serie;
		return m;

	}
	if(id<radacina->info.id){
		return getMasinaByID(radacina->st,id);

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

int calculeazaInaltimeArbore(NodArbore* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	if (radacina == NULL) return 0;

	int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
	int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);

	if (inaltimeSt > inaltimeDr) return 1 + inaltimeSt;
	else return 1 + inaltimeDr;

	return 0;
}

float calculeazaPretTotal(NodArbore* radacina) {
	//calculeaza pretul tuturor masinilor din arbore.

	if(radacina==NULL)return 0;
	return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* radacina, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	if (radacina == NULL)return 0;

	float pretCurent;
	if (strcmp(numeSofer, radacina->info.numeSofer) == 0) {
		pretCurent = radacina->info.pret;
	}
	return pretCurent + calculeazaPretulMasinilorUnuiSofer(radacina->st,numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dr,numeSofer);


	return 0;
}



int main() {
	NodArbore* arboreMasini = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(arboreMasini);

	
	printf("\n\n");

	int id = 3;
	Masina gasit = getMasinaByID(arboreMasini, id);
	if (gasit.id == -1) {
		printf("Masina nu a fost gasita\n");
	}
	else {
		afisareMasina(gasit);
	}

	printf("\n\n");
	int nrNoduri = determinaNumarNoduri(arboreMasini);
	printf("Nr de noduri este %d", nrNoduri);
	printf("\n\n\n");

	printf("- Inaltime arbore: %d\n", calculeazaInaltimeArbore(arboreMasini));
	printf("\n\n\n");
	const char* soferCautat = "Popescu";
	printf("- Pretul total al masinilor soferului '%s': %.2f\n", soferCautat, calculeazaPretulMasinilorUnuiSofer(arboreMasini, soferCautat));

	return 0;
}