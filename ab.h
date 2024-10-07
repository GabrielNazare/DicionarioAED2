#ifndef AB_H
#define AB_H
#include "stdio.h"

typedef int(*t_comparar_ab)(void*, void*);
typedef void(*t_imprimir_ab)(void*);

// Definição das structs
typedef struct no t_no;
typedef struct ab t_ab;

// Funções
t_no* criar_no(t_no* ancestral, void* info);
t_ab* criar_ab(t_imprimir_ab impressora, t_comparar_ab comparar);
void* buscar_ab(t_ab* ab, void* chave);
void inserir_ab(t_ab *a, void* carga);
void remover_ab(t_ab* ab, void* chave);
void imprimir_ab(void* info);
int comparar_ab(void* a, void* b);

struct no {
    void* info;
    t_no* sae;
    t_no* sad;
    t_no* ancestral;
};

struct ab {
    t_no* raiz;
    int tamanho;
    t_imprimir_ab impressora;
    t_comparar_ab comparar;
};

typedef struct {
    char chave[20];
    char valor[50];
} t_item;

#endif
