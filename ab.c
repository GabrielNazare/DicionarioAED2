#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "time.h"
#include "ab.h"

t_no* criar_no(t_no* ancestral, void* info){
    t_no* novo = malloc(sizeof(t_no));
    novo->info = info;
    novo->sad = NULL;
    novo->sae = NULL;
    novo->ancestral = ancestral;
    return novo;
}

t_ab* criar_ab(t_imprimir_ab impressora, t_comparar_ab comparar){
    t_ab* nova = malloc(sizeof(t_ab));
    nova->raiz = NULL;
    nova->tamanho = 0;
    nova->impressora = impressora;
    nova->comparar = comparar;
    return nova;
}

static void* __buscar_ab(t_no* no, void* chave, t_comparar_ab comparar){
    if (no == NULL)
        return NULL;
    if (comparar(no->info, chave) == 0){
        return no->info;
    } else if (comparar(no->info, chave) > 0) {
        return __buscar_ab(no->sae, chave, comparar);
    } else {
        return __buscar_ab(no->sad, chave, comparar);
    }
}

void* buscar_ab(t_ab* ab, void* chave){
    return __buscar_ab(ab->raiz, chave, ab->comparar);
}

void* __inserir_ab(t_no* no, t_no* ancestral, void* carga, t_comparar_ab comparar){
    if (no == NULL) {
        return criar_no(ancestral, carga);
    }
    if (comparar(carga, no->info) < 0) {
        no->sae = __inserir_ab(no->sae, no, carga, comparar);
    } else {
        no->sad = __inserir_ab(no->sad, no, carga, comparar);
    }
    return no;
}

void inserir_ab(t_ab *a, void* carga){
    a->tamanho++;
    a->raiz = __inserir_ab(a->raiz, NULL, carga, a->comparar);
}

t_no* encontrar_minimo(t_no* no) {
    while (no->sae != NULL) {
        no = no->sae;
    }
    return no;
}

t_no* __remover_ab(t_no* no, void* chave, t_comparar_ab comparar) {
    if (no == NULL) {
        return NULL;
    }
    if (comparar(chave, no->info) < 0) {
        no->sae = __remover_ab(no->sae, chave, comparar);
    } else if (comparar(chave, no->info) > 0) {
        no->sad = __remover_ab(no->sad, chave, comparar);
    } else {
        if (no->sae == NULL && no->sad == NULL) {
            free(no);
            return NULL;
        }
        else if (no->sae == NULL) {
            t_no* temp = no->sad;
            free(no);
            return temp;
        } else if (no->sad == NULL) {
            t_no* temp = no->sae;
            free(no);
            return temp;
        }
        else {
            t_no* temp = encontrar_minimo(no->sad);
            no->info = temp->info;
            no->sad = __remover_ab(no->sad, temp->info, comparar);
        }
    }
    return no;
}

void remover_ab(t_ab* ab, void* chave) {
    if (ab->raiz != NULL) {
        ab->raiz = __remover_ab(ab->raiz, chave, ab->comparar);
        ab->tamanho--;
    }
}

int comparar_ab(void* a, void* b) {
    t_item *item_a = (t_item*)a;
    t_item *item_b = (t_item*)b;
    return strcmp(item_a->chave, item_b->chave);
}

void imprimir_ab(void* info) {
    t_item *item = (t_item*)info;
    printf("Chave: %s, Valor: %s\n", item->chave, item->valor);
}
