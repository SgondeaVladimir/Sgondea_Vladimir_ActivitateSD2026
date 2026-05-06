//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
////punct 1
//
//struct Magazin {
//	int id;
//	char* denumire;
//	char* localitate;
//	float suprafata;
//	int numar_angajati;
//};
//typedef struct Magazin Magazin;
//
//struct Nod {
//	Magazin* info;
//	struct Nod* next;
//
//};
//typedef struct Nod Nod;
//
//
//
//void inserareOrdonata(Nod** cap, Magazin* m) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->next = NULL;
//	nou->info = m;
//
//	if (*cap == NULL || (*cap)->info->id >= m->id) { // Daca lista e goala sau magazinul are ID mai mic decat primul
//		nou->next = *cap;
//		*cap = nou;
//	}
//	else {
//		Nod* aux = *cap;
//		while (aux->next!=NULL && aux->next->info->id < m->id) {
//
//			aux = aux->next;
//		}
//		nou->next = aux->next;
//		aux->next = nou;
//
//	}
//}
//
//
//Nod* citireListaDinFisier(const char* numeFisier) {
//    Nod* lista = NULL;
//    FILE* f = fopen(numeFisier, "r");
//
//    if (f) {
//        char buffer[256];
//        char sep[3] = ",\n";
//
//        while (fgets(buffer, 256, f)) {
//            // Alocam memoria pentru magazin direct aici
//            Magazin* m = (Magazin*)malloc(sizeof(Magazin));
//            char* aux;
//
//            aux = strtok(buffer, sep);
//            m->id = atoi(aux);
//
//            aux = strtok(NULL, sep);
//            m->denumire = (char*)malloc(strlen(aux) + 1);
//            strcpy(m->denumire, aux);
//
//            aux = strtok(NULL, sep);
//            m->localitate = (char*)malloc(strlen(aux) + 1);
//            strcpy(m->localitate, aux);
//
//            // Pentru tipurile simple nu ne mai trebuie aux
//            m->suprafata = atof(strtok(NULL, sep));
//            m->numar_angajati = atoi(strtok(NULL, sep));
//
//            // Apelam inserarea ordonata
//            inserareOrdonata(&lista, m);
//        }
//        fclose(f);
//    }
//    return lista;
//}
//
////punct 2
//
//void modificareDenumire(Nod* cap, char* denumireCurenta, char* localitate, char* denumireNoua) {
//    Nod* temp = cap;
//    while (temp != NULL) {
//        if (strcmp(denumireCurenta, temp->info->denumire) == 0 && strcmp(localitate, temp->info->localitate)==0) {
//            free(temp->info->denumire);
//            temp->info->denumire = malloc(strlen(denumireNoua) + 1);
//            strcpy(temp->info->denumire, denumireNoua);
//            break;
//        }
//        temp = temp->next;
//
//    }
//
//}
//
////punct 3
//float medieAngajatiLocalitate(Nod* cap, char* localitate) {
//    Nod* temp=cap;
//    float suma = 0;
//    int contor = 0;
//    while (temp != NULL) {
//        if (strcmp(temp->info->localitate, localitate) == 0) {
//            suma += temp->info->numar_angajati;
//            contor++;
//        }
//        temp = temp->next;
//    }
//    if (contor == 0) {
//        printf("Nu exista localitatea aleaasa\n");
//        return 0;
//    }
//    else {
//        return suma / contor;
//    }
//
//}
//
////punct 4
//struct HashTable {
//    Magazin** vector;
//    int dim;
//};
//
//typedef struct HashTable HashTable;
//
//int calculeazaHashLocalitate(const char* localitate, int dim) {
//    int suma = 0;
//    for (int i = 0; i < strlen(localitate); i++) {
//        suma += localitate[i];
//    }
//    return suma % dim;
//
//
//}
//
//Magazin* copiazaMagazin(Magazin* m) {
//
//    Magazin* nou = (Magazin*)malloc(sizeof(Magazin));
//    nou->id = m->id;
//
//    nou->localitate = malloc(strlen(m->localitate) + 1);
//    strcpy(nou->localitate, m->localitate);
//
//    nou->denumire = malloc(strlen(m->denumire) + 1);
//    strcpy(nou->denumire, m->denumire);
//
//    nou->suprafata = m->suprafata;
//    nou->numar_angajati = m->numar_angajati;
//
//    return nou;
//
//}
//
////Inserare in Hash cu Linear Probing
//
//void inserareInHash(HashTable ht,Magazin* m) {
//    int index = calculeazaHashLocalitate(m->localitate,ht.dim);
//    //Daca este liber cluster-ul, punem elementul direct
//    if (ht.vector[index] == NULL)ht.vector[index] = m;
//    else {//coliziune. caut cu pasul -1
//
//        int initial = index;
//        int gasit = 0;
//
//        // Mergem spre stanga: (index - 1)
//        // Folosim formula (index - 1 + dim) % dim pentru a ramane in intervalul corect
//        index = (index - 1 + ht.dim) % ht.dim;
//
//        while (index != initial) {
//            if (ht.vector[index] == NULL) {
//                ht.vector[index] = m;
//                gasit = 1;
//                break;
//            }
//            index = (index - 1 + ht.dim) % ht.dim;
//
//        }
//        if (!gasit) {
//            printf("Tabela este plina!!!\n");
//            //Dezalocam magaziul daca nu s-a putut insera
//            free(m->denumire);
//            free(m->localitate);
//            free(m);
//        }
//    }
//}
//
////Secventa de copiere din lista in Hash
//HashTable copiereListaInHash(Nod* cap, int dimensiuneTabela) {
//    HashTable ht;
//    ht.dim = dimensiuneTabela;
//    ht.vector = (Magazin**)malloc(sizeof(Magazin*) * ht.dim);
//    //Initializarea hash-ului cu NULL
//    for (int i = 0; i < ht.dim; i++) ht.vector[i] = NULL;
//
//    Nod* temp = cap;
//    while (temp != NULL) {
//        // Cream o copie NOUA a magazinului (Deep Copy)
//        Magazin* copie = copiazaMagazin(temp->info);
//        // Introducem copia in hash
//        inserareInHash(ht, copie);
//
//        temp = temp->next;
//    }
//    return ht;
//}
//
////punctul 5
//void modificareLocalitateInHash(HashTable ht, char* denumire, char* localitate, char* localitateNoua) {
//    int index = calculeazaHashLocalitate(localitate, ht.dim);
//    int initial = index;
//
//    while (ht.vector[index] != NULL) {
//        if (strcmp(ht.vector[index]->denumire, denumire) == 0 && strcmp(ht.vector[index]->localitate, localitate) == 0) {
//            Magazin* magazinDeModificat = ht.vector[index];
//            ht.vector[index] = NULL;
//
//            free(magazinDeModificat->localitate);
//            magazinDeModificat->localitate = (char*)malloc(sizeof(char) * (strlen(localitateNoua) + 1));
//            strcpy(magazinDeModificat->localitate, localitateNoua);
//
//            inserareInHash(ht, magazinDeModificat);
//            return;
//        }
//        index = (index - 1 + ht.dim) % ht.dim;
//        if (index == initial) {
//            break;
//        }
//    }   
//
//    printf("Magazinul %s din %s nu a fost gasit in Hash Table.\n", denumire, localitate);
//}
//
//
//void dezalocareLista(Nod** cap) {
//    Nod* temp = *cap;
//    while (temp != NULL) {
//        Nod* nodDeSters = temp;
//
//        temp = temp->next;
//        free(nodDeSters->info->denumire);
//        free(nodDeSters->info->localitate);
//        free(nodDeSters->info);
//        free(nodDeSters);
//
//    }
//    *cap = NULL;
//}
//
//void dezalocareHashTable(HashTable* ht) {
//    for (int i = 0; i < ht->dim; i++) {
//        if (ht->vector[i] != NULL) {
//            free(ht->vector[i]->denumire);
//            free(ht->vector[i]->localitate);
//
//            free(ht->vector[i]);
//            ht->vector[i] = NULL;
//        }
//    }
//    free(ht->vector);
//    ht->vector = NULL;
//    ht->dim = 0;
//}
//
//int main() {
//    // --- PUNCTUL 1: Creare Lista Simpla din fisier ---
//    printf(">>> PUNCTUL 1: Incarcare date in Lista Simpla (Ordonata dupa ID)\n");
//    Nod* listaMagazine = citireListaDinFisier("magazine.txt");
//
//    // Afisam lista sa vedem ordonarea
//    printf("Continut lista initiala:\n");
//    Nod* temp = listaMagazine;
//    while (temp) {
//        printf("[ID: %d] %s (%s) - %d angajati\n",
//            temp->info->id, temp->info->denumire, temp->info->localitate, temp->info->numar_angajati);
//        temp = temp->next;
//    }
//    printf("--------------------------------------------------\n\n");
//
//
//    // --- PUNCTUL 2: Modificare denumire magazin ---
//    printf(">>> PUNCTUL 2: Modificam denumirea 'La Nicusor' din 'Nehoiu'\n");
//    modificareDenumire(listaMagazine, "La Nicusor", "Nehoiu", "Supermarket Nicusor");
//    printf("Denumire modificata cu succes in lista.\n\n");
//
//
//    // --- PUNCTUL 3: Media de angajati per localitate ---
//    const char* localitateCautata = "Nehoiu";
//    float medie = medieAngajatiLocalitate(listaMagazine, localitateCautata);
//    printf(">>> PUNCTUL 3: Media de angajati in %s este: %.2f\n", localitateCautata, medie);
//    printf("--------------------------------------------------\n\n");
//
//
//    // --- PUNCTUL 4: Copiere in Tabela de Dispersie (Linear Probing) ---
//    printf(">>> PUNCTUL 4: Copiem datele din Lista in Hash Table (Deep Copy)\n");
//    // Alegem o dimensiune de 15 pentru a avea loc de inserari fara prea multe coliziuni
//    HashTable tabelaHash = copiereListaInHash(listaMagazine, 15);
//    printf("Tabela de dispersie a fost creata.\n\n");
//
//
//    // --- PUNCTUL 5: Modificare localitate (cheie de cautare) in Hash ---
//    printf(">>> PUNCTUL 5: Modificam localitatea pentru 'Supermarket Nicusor'\n");
//    // Din Nehoiu il mutam in Buzau (asta va schimba si pozitia in tabela!)
//    modificareLocalitateInHash(tabelaHash, "Supermarket Nicusor", "Nehoiu", "Buzau");
//    printf("Localitate modificata si magazin repozitionat in Hash Table.\n\n");
//
//
//    // --- PUNCTUL 6: Dezalocare structuri ---
//    printf(">>> PUNCTUL 6: Dezalocare memorie\n");
//
//    dezalocareLista(&listaMagazine);
//    if (listaMagazine == NULL) printf("Lista a fost stearsa corect.\n");
//
//    dezalocareHashTable(&tabelaHash);
//    if (tabelaHash.vector == NULL) printf("Tabela Hash a fost stearsa corect.\n");
//
//    printf("\nProgram finalizat cu succes!");
//
//    return 0;
//}
//
//
//
//
//
//
