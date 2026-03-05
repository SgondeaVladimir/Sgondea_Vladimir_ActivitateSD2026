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

void afisareMasina(Masina masina) {
	printf("Id: %d, Nr usi: %d, Pret: %5.2f, Model: %s, Nume sofer: %s, Serie: %c\n",
		masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	if (masini == NULL || nrMasini <= 0) {
		printf("Nu exista masini de afisat\n");
		return;
	}
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* temp = (Masina*)malloc(((*nrMasini) + 1) * sizeof(Masina));
	for (int i = 0; i < *nrMasini; i++) {
		temp[i] = (*masini)[i];
	}
	temp[*nrMasini] = masinaNoua;
	free(*masini);
	*masini = temp;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	char linie[256];

	if (fgets(linie, 255, file) == NULL) {
		Masina m = { -1, 0, 0, NULL, NULL, 'X' };
		return m;
	}

	char delimitator[3] = ",\n";
	Masina masina;

	masina.id = atoi(strtok(linie, delimitator));
	masina.nrUsi = atoi(strtok(NULL, delimitator));
	masina.pret = atof(strtok(NULL, delimitator));

	char* aux = strtok(NULL, delimitator);
	masina.model = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.model, aux);

	aux = strtok(NULL, delimitator);
	masina.numeSofer = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.numeSofer, aux);

	masina.serie = strtok(NULL, delimitator)[0];

	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		printf("Eroare la deschiderea fisierului\n");
		return NULL;
	}
	else {
		Masina* vectorMasini = NULL;
		*nrMasiniCitite = 0;

		while (1) {
			Masina masina = citireMasinaFisier(file);
			if (masina.id == -1) break; 

			adaugaMasinaInVector(&vectorMasini, nrMasiniCitite, masina);
		}

		fclose(file); 
		return vectorMasini;
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	if (*vector != NULL) {
		for (int i = 0; i < *nrMasini; i++) {
			free((*vector)[i].model);
			free((*vector)[i].numeSofer);
		}
		free(*vector);
		*vector = NULL; 
		*nrMasini = 0;
	}
}

int main() {
	Masina* masini = NULL;
	int nrMasini = 0;

	printf("Citire masini din fisier\n");
	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);

	if (masini != NULL && nrMasini > 0) {
		printf("\n Afisare vector masini (Total: %d)\n", nrMasini);
		afisareVectorMasini(masini, nrMasini);

		printf("\nDezalocare memorie\n");
		dezalocareVectorMasini(&masini, &nrMasini);

		afisareVectorMasini(masini, nrMasini);
	}
	else {
		printf("Nu s-au gasit masini in fisier sau fisierul nu a putut fi deschis.\n");
	}

	return 0;
}