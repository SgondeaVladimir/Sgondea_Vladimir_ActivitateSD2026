//#define _CRT_SECURE_NO_WARNINGS
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//
////1. Definirea Structurii(1p)
////Definiți structura Colet care să conțină următoarele câmpuri :
////
////cod_colet(unsigned int)
////
////nume_destinatar(char*)
////
////adresa_destinatie(char*) – va fi folosită drept cheie de căutare
////
////greutate(float)
////
////valoare_asigurata(float)
////
////termen_livrare_zile(unsigned char)
////
////2. Tabela de Dispersie cu Chaining(Lista Dublă) (2p)
////Scrieți și apelați funcția de inserare a unui colet într - o Tabelă de Dispersie.
////
////Structura tabelei va avea cel puțin 10 clustere.
////
////Mecanismul de tratare a coliziunilor este Chaining, dar fiecare cluster va fi o Listă Dublu Înlănțuită.
////
////Cheia de căutare este adresa_destinatie.
////
////Afișați conținutul tabelei la consolă.
////
////3. Calcul Valoare Totală(1p)
////Scrieți și apelați o funcție care determină valoarea totală asigurată a tuturor coletelor care trebuie livrate într - o anumită localitate(specificată ca parametru).
////
////4. Copiere în Lista Dublă Independentă(2p)
////Scrieți și apelați o funcție care copiază din Tabela de Dispersie într - o Listă Dublu Înlănțuită separată  
////Afișați conținutul listei duble rezultate.
////
////5. Modificare Destinatar în Listă(1.5p)
////Scrieți și apelați o funcție care caută un colet în Lista Dublă de la punctul 4 (după cod_colet) și îi modifică numele destinatarului.Noul nume este primit ca parametru.
////
////6. Căutare Colet Greu în Hash(1p)
////Scrieți și apelați o funcție care returnează(prin adresa de memorie) coletul cu greutatea maximă dintr - un anumit cluster al Tabelei de Dispersie(indexul clusterului este primit ca parametru).
////
////7. Dezalocare Memorie(1p + 1p)
////Scrieți secvența de cod care dezalocă complet memoria pentru ambele structuri : Tabela de Dispersie și Lista Dublă.
//
//
//
//struct Colet {
//	unsigned int cod_colet;
//	char* nume_destinatar;
//	char* adresa_destinatie;
//	float greutate;
//	float valoare_asigurata;
//	unsigned char termen_livrare_zile;
//};
//typedef struct Colet Colet;
//
////structura pt lista dubla
//struct Nod {
//	Colet info;
//	struct Nod* next;
//	struct Nod* prev;
//
//};
//typedef struct Nod Nod;
//struct ListaDubla {
//	Nod* prim;
//	Nod* ultim;
//
//};
//typedef struct ListaDubla ListaDubla;
//
//struct HashTable {
//	ListaDubla* vector;
//	int dim;
//};
//typedef struct HashTable HashTable;
//
//int calculeazaHash(const char* adresa, int dim) {
//	int suma = 0;
//	for (int i = 0; i < strlen(adresa); i++) {
//		suma += adresa[i];
//	}
//	return suma % dim;
//}
//
//HashTable initializareHashTable(int dimensiune) {
//	HashTable ht;
//	ht.dim = dimensiune;
//	ht.vector = (ListaDubla*)malloc(sizeof(ListaDubla) * dimensiune);
//	for (int i = 0; i < dimensiune; i++) {
//		ht.vector[i].prim = NULL;
//		ht.vector[i].ultim = NULL;
//
//	}
//	return ht;
//}
//
//void inserareInceputListaDubla(ListaDubla* lista, Colet c) {
//	Nod* nou = malloc(sizeof(Nod));
//	nou->info = c;
//	nou->prev = NULL;
//	nou->next = lista->prim;
//	if (lista->prim == NULL) {
//		lista->prim = nou;
//		lista->ultim = nou;
//	}
//	else {
//		lista->prim->prev = nou;
//		lista->prim = nou;
//	}
//
//}
//
//void inserareInHash(HashTable ht, Colet c) {
//	int index = calculeazaHash(c.adresa_destinatie, ht.dim);
//	inserareInceputListaDubla(&ht.vector[index], c);
//}
//
//void afisareHashTable(HashTable ht) {
//	for (int i = 0; i < ht.dim; i++) {
//		if (ht.vector[i].prim != NULL) {
//			printf("Cluster[%d]\n", i);
//			Nod* temp = ht.vector[i].prim;
//			while (temp != NULL) {
//				printf("Cod colet: %d, Numele destinatiei: %s, Greutatea: %8.2f\n", temp->info.cod_colet, temp->info.adresa_destinatie, temp->info.greutate);
//				temp = temp->next;
//			}
//			
//		}
//	}
//
//	printf("-------------------\n");
//}
//
//
//float calculeazaValoareTotala(HashTable ht,char* destinatie) {
//	float valoare_totala=0;
//	for (int i = 0; i < ht.dim; i++) {
//		Nod* aux = ht.vector[i].prim;
//		while (aux != NULL) {
//			if (strcmp(destinatie, aux->info.adresa_destinatie) == 0) {
//				valoare_totala = valoare_totala + aux->info.valoare_asigurata;
//			}
//			aux = aux->next;
//		}
//	 }
//
//	return valoare_totala;
//}
//
//
////vrem sa facem deep copy
//Colet copiazaColet(Colet c) {
//	Colet nou;
//	nou.greutate = c.greutate;
//	nou.termen_livrare_zile = c.termen_livrare_zile;
//	nou.cod_colet = c.cod_colet;
//	nou.valoare_asigurata = c.valoare_asigurata;
//
//	nou.adresa_destinatie = malloc(strlen(c.adresa_destinatie) + 1);
//	strcpy(nou.adresa_destinatie, c.adresa_destinatie);
//
//	nou.nume_destinatar = malloc(strlen(c.nume_destinatar) + 1);
//	strcpy(nou.nume_destinatar, c.nume_destinatar);
//
//	return nou;
//}
//
//
//ListaDubla filtreazaColeteleGrele(HashTable ht, float pragGreutate) {
//	ListaDubla listaNoua;
//	listaNoua.prim = NULL;
//	listaNoua.ultim = NULL;
//
//	for (int i = 0; i < ht.dim; i++) {
//		Nod* temp = ht.vector[i].prim;
//		while (temp != NULL) {
//			if (temp->info.greutate > pragGreutate) {
//				Colet copie = copiazaColet(temp->info);
//				inserareInceputListaDubla(&listaNoua, copie);
//			}
//
//			temp = temp->next;
//		}
//
//
//	}
//	return listaNoua;
//}
//
//void modificaDestinatarInLista(ListaDubla* lista, unsigned int codCautat, char* numeNou) {
//	Nod* temp = lista->prim;
//	
//	while (temp != NULL) {
//		if (temp->info.cod_colet == codCautat) {
//			free(temp->info.nume_destinatar);
//			temp->info.nume_destinatar = malloc(strlen(numeNou) + 1);
//			strcpy(temp->info.nume_destinatar, numeNou);
//		}
//
//		temp = temp->next;
//	}
//
//}
//
//Colet* gasesteColetMaximInCluster(HashTable ht, int indexCluster) {
//	Nod* temp = ht.vector[indexCluster].prim;
//	float greutateMaxima = -1;
//	Nod* maxim = ht.vector[indexCluster].prim;
//	while (temp != NULL) {
//		if (temp->info.greutate > greutateMaxima) {
//			greutateMaxima = temp->info.greutate;
//			maxim = temp;
//		}
//
//		temp = temp->next;
//		}
//	return &(maxim->info);
//	}
//
//
//void dezalocareListaDubla(ListaDubla* ls) {
//	if (ls == NULL || ls->prim == NULL)return;
//
//	Nod* temp = ls->prim;
//	while (temp != NULL) {
//		Nod* deSters = temp;
//temp = temp->next;
//
//free(deSters->info.nume_destinatar);
//free(deSters->info.adresa_destinatie);
//free(deSters);
//
//		
//	}
//	ls->prim = NULL;
//	ls->ultim = NULL;
//
//}
//
//void dezalocareHashTable(HashTable* ht) {
//	if (ht == NULL || ht->vector == NULL) {
//		return;
//	}
//	for (int i = 0; i < ht->dim; i++) {
//		Nod* temp = ht->vector[i].prim;
//		while (temp != NULL) {
//			Nod* aux = temp->next;
//			free(temp->info.nume_destinatar);
//			free(temp->info.adresa_destinatie);
//			free(temp);
//
//
//
//			temp = aux;
//		}
//		ht->vector[i].prim = NULL;
//		ht->vector[i].ultim = NULL;
//	}
//	free(ht->vector);
//	ht->vector = NULL;
//	ht->dim = 0;
//
//}