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

struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;


struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* lista) {
	while (lista) {
		afisareMasina(lista->info);
		lista = lista->next;
	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* adaugat = (Nod*)malloc(sizeof(Nod));
	adaugat->info = masinaNoua;
	adaugat->info.numeSofer = (char*)malloc(sizeof(char) * strlen(masinaNoua.numeSofer) + 1);
	strcpy(adaugat->info.numeSofer, masinaNoua.numeSofer);
	adaugat->info.model = (char*)malloc(sizeof(char) * strlen(masinaNoua.model) + 1);
	strcpy(adaugat->info.model, masinaNoua.model);
	adaugat->next = NULL;
	Nod* temp = *lista;
	if (*lista) {
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = adaugat;
	}
	else
		*lista = adaugat;

}


HashTable initializareHashTable(int dimensiune) {
	HashTable hash;
	hash.dim = dimensiune;
	hash.vector = malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		hash.vector[i] = NULL;
	}
	return hash;
}

int calculeazaHash(int id, int dimensiune) {
	return id * 3 % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina galerie) {
	int hashCode = calculeazaHash(galerie.id, hash.dim);
	adaugaMasinaInLista(&hash.vector[hashCode], galerie); // am scris la tabla checkul
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(4);
	if (file) {
		while (!feof(file)) {
			Masina m = citireMasinaDinFisier(file);
			inserareMasinaInTabela(hash, m);
		}
	}
	fclose(file);
	return hash;
}

void afisareTabelaDeMasini(HashTable hash) {
	for (int i = 0; i < hash.dim; i++) {
		printf("Cluster nr. %d:\n", i + 1);
		afisareListaMasini(hash.vector[i]);
		printf("----------------\n\n");
	}
}

void dezalocareTabelaDeMasini(HashTable* hash) {
	for (int i = 0; i < hash->dim; i++) {
		Nod* temp = hash->vector[i];
		while (temp) {
			Nod* temp2 = temp;
			temp = temp->next;
			free(temp2->info.model);
			free(temp2->info.numeSofer);
			free(temp2);
		}
		hash->vector[i] = NULL;

	}
	free(hash->vector);
	hash->vector = NULL;
	hash->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable hash, int* nrClustere) { //am scos int* nrClustere si o folosesc in metoda
	*nrClustere = 0;
	for (int i = 0; i < hash.dim; i++) {
		if (hash.vector[i]) {
			(*nrClustere)++;
		}
	}
	if (*nrClustere == 0) {
		return NULL;
	}
	else {
		float* medie = (float*)malloc(sizeof(float) * (*nrClustere));
		int index = 0;
		for (int i = 0; i < hash.dim; i++) {
			if (hash.vector[i]) {
				float suma = 0;
				int nrMasini = 0;
				Nod* temp = hash.vector[i];
				while (temp) {
					suma += temp->info.pret;
					nrMasini++;
					temp = temp->next;
				}
				medie[index] = suma / nrMasini;
				index++;
			}
		}
		return medie;
	}

}

Masina getMasinaDupaId(HashTable hash, int id) {
	Masina m;
	int hashCode = calculeazaHash(id, hash.dim);
	Nod* cautare = hash.vector[hashCode];
	while (cautare) {
		if (cautare->info.id == id) {
			m = cautare->info;
			m.numeSofer = (char*)malloc(sizeof(char) * (strlen(cautare->info.numeSofer) + 1));
			strcpy(m.numeSofer, cautare->info.numeSofer);
			m.model = (char*)malloc(sizeof(char) * (strlen(cautare->info.model) + 1));
			strcpy(m.model, cautare->info.model);
			return m;
		}
		cautare = cautare->next;
	}
	return m;
}

int main() {

	HashTable tabelaHash = citireMasiniDinFisier("masini.txt");

	Masina masinaNoua = { 77, 5, 18500.50, (char*)"Toyota", (char*)"Marian", 'T' };
	inserareMasinaInTabela(tabelaHash, masinaNoua);

	printf("\n--- CONTINUT HASH ---\n");
	afisareTabelaDeMasini(tabelaHash);
	printf("------------------------------------------------\n\n");

	printf("--- MEDII DE PRET PE CLUSTERE ---\n");
	int numarClustereOcupate = 0;
	float* mediiPret = calculeazaPreturiMediiPerClustere(tabelaHash, &numarClustereOcupate);

	if (mediiPret != NULL) {
		for (int i = 0; i < numarClustereOcupate; i++) {
			printf(" -> Cluster %d: %1.2f\n", i + 1, mediiPret[i]);
		}
		free(mediiPret);
	}
	else {
		printf("Tabela este goala, nu se pot calcula medii.\n");
	}
	printf("------------------------------------------------\n\n");

	
	int idCautat = 3;
	printf("--- CAUTARE MASINA DUPA ID (%d) ---\n", idCautat);
	Masina gasita = getMasinaDupaId(tabelaHash, idCautat);

	if (gasita.id == idCautat) {
		afisareMasina(gasita);
		free(gasita.model);
		free(gasita.numeSofer);
	}
	else {
		printf("Masina cu ID-ul %d nu exista in baza de date.\n", idCautat);
	}
	printf("------------------------------------------------\n\n");

	dezalocareTabelaDeMasini(&tabelaHash);

	if (tabelaHash.dim == 0 && tabelaHash.vector == NULL) {
		printf("Tabela a fost dezalocata cu succes!\n");
	}
	else {
		printf("Eroare la dezalocare.\n");
	}

	return 0;
}