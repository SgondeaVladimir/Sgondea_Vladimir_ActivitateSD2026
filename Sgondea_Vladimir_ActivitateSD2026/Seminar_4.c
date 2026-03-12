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
typedef struct Nod Nod;


struct Nod {
    Masina info;
    Nod* next;
};



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
    while (lista != NULL) {
        afisareMasina(lista->info);
        lista = lista->next;
    }
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;
    if (*lista) {
        Nod* aux = *lista;
        while (aux->next) {
            aux = aux->next;
        }
        aux->next = nou;
    }
    else {
        *lista = nou;
    }
}

void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = *lista;
    *lista = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    Nod* lista = NULL;
    FILE* f = fopen(numeFisier, "r");
    if (f) {
        while (!feof(f)) {
            adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
        }
    }
    fclose(f);
    return lista;
}

void dezalocareListaMasini(Nod** lista) {
    while (*lista) {
        Nod* p = *lista;
        (*lista) = p->next;
        if (p->info.model) {
            free(p->info.model);
        }
        if (p->info.numeSofer) {
            free(p->info.numeSofer);
        }
        free(p);
    }
}

float calculeazaPretMediu(Nod* lista) {
    float suma = 0;
    int count = 0;
    while (lista) {
        suma += lista->info.pret;
        count++;
        lista = lista->next;
    }
    if (count > 0) {
        return suma / count;
    }
    return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
    float suma = 0;
    while (lista) {
        if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
            suma += lista->info.pret;
        }
        lista = lista->next;
    }
    return suma;
}
void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
    while ((*lista) && (*lista)->info.serie == serieCautata) {
        Nod* aux = *lista;
        (*lista) = aux->next;
        if (aux->info.numeSofer) {
            free(aux->info.numeSofer);
        }
        if (aux->info.model) {
            free(aux->info.model);
        }
        free(aux);
    }
    if ((*lista)) {
        Nod* p = *lista;
        while (p) {
            while (p->next && p->next->info.serie != serieCautata) {
                p = p->next;
            }
            if (p->next) {
                Nod* aux = p->next;
                p->next = aux->next;
                if (aux->info.numeSofer) {
                    free(aux->info.numeSofer);
                }
                if (aux->info.model) {
                    free(aux->info.model);
                }
                free(aux);
            }
            else {
                p = NULL;
            }
        }
    }

}


int main() {

    Nod* lista = citireListaMasiniDinFisier("masini.txt");
    afisareListaMasini(lista);
    printf("Pretul mediu este:%.2f\n", calculeazaPretMediu(lista));
    printf("Pretul masinilor unui Sofer este:%.2f\n",
        calculeazaPretulMasinilorUnuiSofer(lista, "Gigel"));

    printf("\nStergere seria A\n");
    stergeMasiniDinSeria(&lista, 'A');
    afisareListaMasini(lista);
    printf("\nStergere seria B\n");
    stergeMasiniDinSeria(&lista, 'B');
    afisareListaMasini(lista);
    dezalocareListaMasini(&lista);
    return 0;
}