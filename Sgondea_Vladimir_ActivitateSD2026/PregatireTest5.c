#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Implementați o aplicație în limbajul C care gestionează piloții de Formula 1 folosind o structură de tip Max - Heap(pentru a extrage rapid pilotul cu cel mai mare punctaj).
//
//Definiți structura Pilot ce conține : id(int), nume(char*), echipa(char*), și punctaj(float).Definiți structurile necesare pentru lucrul cu un Heap bazat pe un vector alocat dinamic(capacitate și dimensiune). (1 p.)
//
//Implementați funcția de inserare a unui pilot în Max - Heap, respectând proprietatea de heap(elementul maxim stă la rădăcină).Cheia de ordonare este atributul punctaj. (1.5 p.)
//
//Creați un Heap și inserați cel puțin 6 obiecte de tip Pilot.Informațiile pot fi citite dintr - un fișier text sau inițializate cu valori implicite prin apeluri succesive în main. (1 p.)
//
//Implementați funcția de parcurgere și afișare a tuturor elementelor din Heap(afișarea vectorului intern de la indexul 0 la dimensiune - 1). (1 p.)
//
//Implementați o funcție care calculează și returnează numărul total de piloți din Heap care aparțin unei anumite echipe primite ca parametru. (1 p.)
//
//Implementați o funcție care caută liniar un pilot în Heap după id și îl returnează printr - un deep - copy(alocare de memorie nouă pentru șirurile de caractere). (1.5 p.)
//
//Implementați funcția de extragere a maximului(rădăcinii) din Heap.Funcția va elimina pilotul cu cel mai mare punctaj, va reface structura de Max - Heap și va returna pilotul extras.Afișați pilotul în consolă. (1.5 p.)
//
//Implementați funcția de eliberare a întregii memorii alocate în HEAP(inclusiv deep - copy - ul și vectorul heap - ului). (1.5 p.)


struct Pilot {
	int id;
	float punctaj;
	char* nume;
	char* echipa;
};
typedef struct Pilot Pilot;

struct Heap {
	int lungime;     // Capacitatea totala alocata
	int nrElemViz;   // Numarul de elemente din Heap-ul activ
	Pilot* vector;
};
typedef struct Heap Heap;


void afisarePilot(Pilot p) {
	printf("Id: %d | Nume: %s | Echipa: %s | Punctaj: %.2f\n",
		p.id, p.nume, p.echipa, p.punctaj);
}



Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrElemViz = 0;
	heap.vector = (Pilot*)malloc(lungime*sizeof(Pilot));
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int stanga = 2 * pozitieNod + 1;
	int dreapta = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;

	// Cautam cel mai mare element dintre parinte si cei doi copii
	if (stanga < heap.nrElemViz && heap.vector[stanga].punctaj > heap.vector[pozMax].punctaj) {
		pozMax = stanga;
	}
	if (dreapta < heap.nrElemViz && heap.vector[dreapta].punctaj > heap.vector[pozMax].punctaj) {
		pozMax = dreapta;
	}

	// Daca parintele nu e cel mai mare, facem rocada si continuam coborarea
	if (pozMax != pozitieNod) {
		Pilot aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		// Verificam daca noul nod mai are macar posibilitatea de a avea copii
		if (pozMax <= ((heap.nrElemViz - 2) / 2)) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDePilotiDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(6); 
	if (f == NULL) {
		printf("Eroare la deschidere\n");
		return heap;
	}

		while (!feof(f)) {
			char buffer[100];
			char sep[3] = ",\n";
			Pilot p;

			fgets(buffer, 100, f);

			char* aux;

			p.id = atoi(strtok(buffer, sep));
			p.punctaj = atof(strtok(NULL, sep));

			aux = strtok(NULL, sep);
			p.nume = (char*)malloc(strlen(aux) + 1);
			strcpy(p.nume, aux);

			aux = strtok(NULL, sep);
			p.echipa = (char*)malloc(strlen(aux) + 1);
			strcpy(p.echipa, aux);

			heap.vector[heap.nrElemViz++] = p;
		}
		fclose(f);
	

	// Reparam subarborii incepand de la ultimul parinte pana la radacina
	for (int i = (heap.nrElemViz - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	return heap;
}



void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemViz; i++) {
		afisarePilot(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	// Aici se afla elementele care au fost extrase (sortate crescator la final)
	for (int i = heap.nrElemViz; i < heap.lungime; i++) {
		if (heap.vector[i].id != 0) { // Verificam sa nu afisam locuri goale din array
			afisarePilot(heap.vector[i]);
		}
	}
}

int numaraPilotiEchipaHeap(Heap heap, const char* echipaCautata) {
	int count = 0;
	for (int i = 0; i < heap.nrElemViz; i++) {
		if (strcmp(heap.vector[i].echipa, echipaCautata) == 0) count++;
	}
	return count;
}

Pilot cautaPilotDupaID(Heap heap,int id) {
	for (int i = 0; i < heap.nrElemViz; i++) {
		if (id == heap.vector[i].id) {
			Pilot p;
			p = heap.vector[i];
			p.nume = malloc(strlen(heap.vector[i].nume) + 1);
			strcpy(p.nume, heap.vector[i].nume);
			p.echipa = malloc(strlen(heap.vector[i].echipa) + 1);
			strcpy(p.echipa, heap.vector[i].echipa);
			return p;
		}
	}
	Pilot p;
	p.id = -1;
	p.echipa = NULL;
	p.nume = NULL;
	return p;
}

//Implementați funcția de extragere a maximului(rădăcinii) din Heap.Funcția va elimina pilotul cu cel mai mare punctaj, va reface structura de Max - Heap și va returna pilotul extras.Afișați pilotul în consolă. (1.5 p.)
Pilot extragePilot(Heap* heap) {
	Pilot aux;
	aux.id = -1;

	if (heap->nrElemViz > 0) {
		heap->nrElemViz--; // Micsoram limita Heap-ului activ

		// Salvam radacina (maximul)
		aux = heap->vector[0];

		// Facem rocada intre radacina si ultimul element (cel pe care tocmai l-am scos din vizibilitate)
		heap->vector[0] = heap->vector[heap->nrElemViz];
		heap->vector[heap->nrElemViz] = aux; // Il "ascundem" la finalul vectorului

		// Cernem noua radacina in jos
		filtreazaHeap(*heap, 0);
	}
	return aux;
}


void dezalocareHeap(Heap* heap) {
	// Parcurgem toata lungimea alocata pentru a curata inclusiv Heap-ul Ascuns
	for (int i = 0; i < heap->lungime; i++) {	
			free(heap->vector[i].nume);
			free(heap->vector[i].echipa);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->nrElemViz = 0;
	heap->lungime = 0;
}


int main() {
	

	printf("Citire si Construire\n");
	Heap heap = citireHeapDePilotiDinFisier("piloti1.txt");
	afisareHeap(heap);

	printf("\nNumarare piloti dintr-o echipa\n");
	printf("Total piloti Ferrari: %d\n", numaraPilotiEchipaHeap(heap, "Ferrari"));

	printf("\nExtrageri (Se scot pe rand cele mai mari punctaje)\n");
	afisarePilot(extragePilot(&heap));
	afisarePilot(extragePilot(&heap));

	printf("\nHeap-ul ramas activ\n");
	afisareHeap(heap);

	printf("\nHeap Ascuns (Pilotii extrasi, ajunsi la finalul vectorului)\n");
	afiseazaHeapAscuns(heap);

	dezalocareHeap(&heap);
	return 0;
}