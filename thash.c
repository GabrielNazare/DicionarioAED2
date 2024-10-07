#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "thash.h"
#include "lse_neutra.h"

typedef void (*t_imprimir_hash)(void*);

typedef struct elem_hash t_elem_hash;
struct elem_hash{
    int chave;
    void* carga;
};

t_elem_hash* criar_elem_hash(int chave, void* carga){
    t_elem_hash* e = malloc(sizeof(t_elem_hash));
    e->chave = chave;
    e->carga = carga;
    return e;
}

int comparar_elem_hash(void* e1, void* e2){
    t_elem_hash* ee1 = e1;
    t_elem_hash* ee2 = e2;
    return (ee1->chave - ee2->chave);
}

void imprimir_elem_hash(void* ee){
    t_elem_hash* e = ee;
    printf("%d %p\n", e->chave, e->carga);
}

void destroy_elem_hash(t_elem_hash* e){
    free(e);
}

struct hash {
    t_imprimir_hash impressora;
    t_lse* *vetor;
    int tamanho;
    double fc;
    int elementos;
};

t_hash* criar_hash(double fc){
    t_hash* nova = malloc(sizeof(t_hash));
    nova->fc = fc;
    nova->tamanho = 7;
    nova->elementos = 0;
    nova->vetor = malloc(sizeof(t_lse*) * nova->tamanho);
    for (int i = 0; i < nova->tamanho; i++) {
        nova->vetor[i] = criar_lse(imprimir_elem_hash, comparar_elem_hash);
    }
    return nova;
}

int funcao_hashing(t_hash* t, int chave){
    return chave % t->tamanho;
}

void inserir_hash(t_hash* t, int chave, void* carga){
    if ((double)(t->elementos + 1) / t->tamanho > t->fc) {
        t = rehashing(t);
    }

    int pos = funcao_hashing(t, chave);
    pos = encontrar_espaco_vazio(t, pos, chave);

    t_elem_hash* novo = criar_elem_hash(chave, carga);
    inserir_inicio_lse(t->vetor[pos], novo);
    t->elementos++;
}

void* buscar_hash(t_hash* t, int chave){
    int pos = funcao_hashing(t, chave);

    t_elem_hash* e = acessar_conteudo_lse(t->vetor[pos], &chave);
    if (e != NULL) {
        return e->carga;
    } else {
        return NULL;
    }
}

void* remover_hash(t_hash* t, int chave){
    void* carga = NULL;
    int pos = funcao_hashing(t, chave);

    t_elem_hash* e = remover_conteudo_lse(t->vetor[pos], &chave);
    if (e != NULL){
        carga = e->carga;
        destroy_elem_hash(e);
        t->elementos--;
    }
    return carga;
}

void imprimir_hash(t_hash* t){
   int K = t->tamanho;
   for (int k = 0; k < K; k++){
        t_lse* colisoes = t->vetor[k];
        imprimir_lse(colisoes);
   }
}

 t_hash* rehashing(t_hash* atual) {
     int novo_tamanho = atual->tamanho * 2;
     t_hash* nova = malloc(sizeof(t_hash));
     nova->fc = atual->fc;
     nova->tamanho = novo_tamanho;
     nova->elementos = 0;
     nova->vetor = malloc(sizeof(t_lse*) * novo_tamanho);
     for (int i = 0; i < novo_tamanho; i++) {
         nova->vetor[i] = criar_lse(imprimir_elem_hash, comparar_elem_hash);
     }
     for (int i = 0; i < atual->tamanho; i++) {
         t_lse* colisoes = atual->vetor[i];
         if (colisoes != NULL) {
             t_elem_hash* e = acessar_conteudo_lse(colisoes, NULL);
             if (e != NULL) {
                 inserir_hash(nova, e->chave, e->carga);
             }
         }
     }

     free(atual->vetor);
     free(atual);

     return nova;
}

int encontrar_espaco_vazio(t_hash* t, int pos, int chave) {
    while (t->vetor[pos] != NULL && pos < t->tamanho) {
        pos = (pos + 1) % t->tamanho;
    }
    return pos;
}
