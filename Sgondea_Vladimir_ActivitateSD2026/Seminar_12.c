#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// STRUCTURI DE BAZĂ (Mașină)
// ============================================================================
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
	Masina m1;
	m1.id = -1; // Semnal de eșec / final de fișier

	char buffer[100];
	char sep[3] = ",\n";

	// PROTECȚIA: Verificăm dacă am putut citi linia
	if (fgets(buffer, 100, file) == NULL) return m1;

	char* aux;
	aux = strtok(buffer, sep);
	if (!aux) return m1;

	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux); // Folosim strcpy standard pentru portabilitate

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d | Nr. usi: %d | Pret: %.2f | Model: %s | Sofer: %s | Serie: %c\n",
		masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}


// ============================================================================
// 1. STRUCTURI NECESARE (Graf - Lista de Liste)
// ============================================================================
typedef struct NodS NodS;
typedef struct NodP NodP;

// Lista Secundară (Muchiile / Vecinii)
struct NodS {
	NodP* info; // Pointer către nodul din lista principală
	NodS* next;
};

// Lista Principală (Nodurile Grafului)
struct NodP {
	Masina m;
	NodS* vecini;
	NodP* next;
};


// ============================================================================
// 2. FUNCȚII DE INSERARE ÎN LISTE
// ============================================================================
void inserareListaPrincipala(NodP** graf, Masina m) {
	NodP* nou = malloc(sizeof(NodP));
	nou->m = m;
	nou->vecini = NULL;
	nou->next = NULL;

	if (*graf == NULL) {
		*graf = nou;
	}
	else {
		NodP* temp = *graf;
		while (temp->next) temp = temp->next;
		temp->next = nou;
	}
}

void inserareListaSecundara(NodS** lista, NodP* vecin) {
	NodS* nou = malloc(sizeof(NodS));
	nou->info = vecin;
	nou->next = NULL;

	if (*lista == NULL) {
		*lista = nou;
	}
	else {
		NodS* temp = *lista;
		while (temp->next) temp = temp->next;
		temp->next = nou;
	}
}


// ============================================================================
// 3. CĂUTARE ÎN LISTA PRINCIPALĂ
// ============================================================================
NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
	NodP* temp = listaPrincipala;
	while (temp != NULL) {
		if (temp->m.id == id) return temp;
		temp = temp->next;
	}
	return NULL;
}


// ============================================================================
// 4. INSERARE MUCHIE (Graf Neorientat)
// ============================================================================
void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
	NodP* start = cautaNodDupaID(listaPrincipala, idStart);
	NodP* stop = cautaNodDupaID(listaPrincipala, idStop);

	if (start != NULL && stop != NULL) {
		inserareListaSecundara(&start->vecini, stop);
		inserareListaSecundara(&stop->vecini, start);
	}
}


// ============================================================================
// 5. CITIRE DIN FIȘIERE ȘI DEZALOCARE
// ============================================================================
NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) return NULL;

	NodP* graf = NULL;
	while (1) {
		Masina m = citireMasinaDinFisier(f);
		if (m.id == -1) break; // Am terminat de citit sau am dat de rând gol
		inserareListaPrincipala(&graf, m);
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) return;

	int idStart, idStop;
	while (fscanf(f, "%d %d", &idStart, &idStop) == 2) {
		inserareMuchie(graf, idStart, idStop);
	}
	fclose(f);
}

void dezalocareNoduriGraf(NodP** listaPrincipala) {
	if (listaPrincipala == NULL || *listaPrincipala == NULL) return;

	NodP* curentP = *listaPrincipala;
	while (curentP != NULL) {
		// Dezalocam vecinii
		NodS* curentS = curentP->vecini;
		while (curentS != NULL) {
			NodS* tempS = curentS;
			curentS = curentS->next;
			free(tempS);
		}

		// Dezalocam stringurile masinii
		if (curentP->m.model) free(curentP->m.model);
		if (curentP->m.numeSofer) free(curentP->m.numeSofer);

		// Dezalocam nodul principal
		NodP* tempP = curentP;
		curentP = curentP->next;
		free(tempP);
	}
	*listaPrincipala = NULL;
}


// ============================================================================
// Funcție ajutătoare pentru determinarea dimensiunii vectorului de vizitați
// ============================================================================
int calculeazaIdMaximGraf(NodP* listaPrincipala) {
	int maxId = 0;
	NodP* temp = listaPrincipala;
	while (temp) {
		if (temp->m.id > maxId) {
			maxId = temp->m.id;
		}
		temp = temp->next;
	}
	return maxId;
}


// ============================================================================
// PARCURGERE ÎN ADÂNCIME (DFS)
// ============================================================================
struct NodStiva {
	int id;
	struct NodStiva* next;
};
typedef struct NodStiva NodStiva;

void push(NodStiva** varf, int id) {
	NodStiva* nou = malloc(sizeof(NodStiva));
	nou->id = id;
	nou->next = *varf;
	*varf = nou;
}

int pop(NodStiva** varf) {
	if (*varf == NULL) return -1;
	int extras = (*varf)->id;
	NodStiva* temp = *varf;
	*varf = (*varf)->next;
	free(temp);
	return extras;
}

void afisareGrafInAdancime(NodP* listaPrincipala, int idPlecare) {
	// Alocăm vectorul de vizitați în funcție de cel mai mare ID din graf
	int maxId = calculeazaIdMaximGraf(listaPrincipala);
	int* vizitat = (int*)calloc(maxId + 1, sizeof(int));

	NodStiva* stiva = NULL;

	// Marcam nodul de plecare ca fiind vizitat și îl adăugăm în STIVĂ
	vizitat[idPlecare] = 1;
	push(&stiva, idPlecare);

	printf("\n--- Parcurgere Adancime (DFS) incepand de la ID %d ---\n", idPlecare);

	// Pornim parcurgerea
	while (stiva != NULL) {
		// Extragem un nod
		int idCurent = pop(&stiva);

		// Procesam nodul extras (il afisam)
		NodP* nodCurent = cautaNodDupaID(listaPrincipala, idCurent);
		if (nodCurent) {
			printf("Nod parcurs -> ID: %d (%s)\n", nodCurent->m.id, nodCurent->m.model);
		}

		// Identificam vecinii si adaugam in stiva pe cei nevizitati
		NodS* vecin = nodCurent->vecini;
		while (vecin != NULL) {
			int idVecin = vecin->info->m.id;
			if (vizitat[idVecin] == 0) {
				vizitat[idVecin] = 1;
				push(&stiva, idVecin);
			}
			vecin = vecin->next;
		}
	}
	free(vizitat);
}


// ============================================================================
// PARCURGERE ÎN LĂȚIME (BFS)
// ============================================================================
struct NodCoada {
	int id;
	struct NodCoada* next;
};
typedef struct NodCoada NodCoada;

void enqueue(NodCoada** prim, NodCoada** ultim, int id) {
	NodCoada* nou = malloc(sizeof(NodCoada));
	nou->id = id;
	nou->next = NULL;
	if (*prim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

int dequeue(NodCoada** prim, NodCoada** ultim) {
	if (*prim == NULL) return -1;
	int extras = (*prim)->id;
	NodCoada* temp = *prim;
	*prim = (*prim)->next;
	if (*prim == NULL) {
		*ultim = NULL; // Daca s-a golit coada, ultimul devine null
	}
	free(temp);
	return extras;
}

void afisareGrafInLatime(NodP* listaPrincipala, int idPlecare) {
	int maxId = calculeazaIdMaximGraf(listaPrincipala);
	int* vizitat = (int*)calloc(maxId + 1, sizeof(int));

	NodCoada* coada_prim = NULL;
	NodCoada* coada_ultim = NULL;

	// Marcam nodul de plecare și îl adăugăm în COADĂ
	vizitat[idPlecare] = 1;
	enqueue(&coada_prim, &coada_ultim, idPlecare);

	printf("\n--- Parcurgere Latime (BFS) incepand de la ID %d ---\n", idPlecare);

	while (coada_prim != NULL) {
		// Extragem din coada
		int idCurent = dequeue(&coada_prim, &coada_ultim);

		// Procesam nodul
		NodP* nodCurent = cautaNodDupaID(listaPrincipala, idCurent);
		if (nodCurent) {
			printf("Nod parcurs -> ID: %d (%s)\n", nodCurent->m.id, nodCurent->m.model);
		}

		// Adaugam vecinii nevizitati
		NodS* vecin = nodCurent->vecini;
		while (vecin != NULL) {
			int idVecin = vecin->info->m.id;
			if (vizitat[idVecin] == 0) {
				vizitat[idVecin] = 1;
				enqueue(&coada_prim, &coada_ultim, idVecin);
			}
			vecin = vecin->next;
		}
	}
	free(vizitat);
}


// ============================================================================
// MAIN 
// ============================================================================
int main() {
	// Apelează aceste funcții presupunând că ai fișierele "masini.txt" și "muchii.txt" 
	// în același folder cu executabilul tău.

	NodP* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", graf);

	// Presupunând că ai un nod cu ID-ul 1 în graf, putem lansa parcurgerile:
	if (graf != NULL) {
		int idDeTest = graf->m.id; // Testam pornind de la primul nod citit

		afisareGrafInAdancime(graf, idDeTest);
		afisareGrafInLatime(graf, idDeTest);
	}
	else {
		printf("Graful nu a putut fi initializat. Verifica fisierul masini.txt\n");
	}

	dezalocareNoduriGraf(&graf);

	return 0;
}