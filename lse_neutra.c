#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "lse_neutra.h"

typedef struct elem_lse t_elemento_lse;

struct lse{
    t_elemento_lse* prim;
    t_elemento_lse* ult;
    int tamanho;
    t_imprimir_lse impressora;
    t_comparar_lse comparador;
};

struct elem_lse{
    void* carga_util;
    t_elemento_lse* prox;
};

t_elemento_lse* criar_elem_lse(void* carga){
    t_elemento_lse* novo = malloc(sizeof(t_elemento_lse));
    assert(novo!=NULL);
    novo->carga_util = carga;
    novo->prox=NULL;
    return novo; 
}

t_lse* criar_lse( t_imprimir_lse impressora, t_comparar_lse comparador ){
    t_lse* nova = malloc(sizeof(t_lse));
    assert(nova!=NULL);
    nova->prim = NULL;
    nova->ult = NULL;
    nova->tamanho =0;
    nova->impressora = impressora;
    nova->comparador = comparador;

    return nova;
}
/// @brief destroy uma lista vazia
/// @param lse 
void destroy_lse(t_lse* lse){
    assert(lse->prim == NULL);
    free(lse);
}

// ops baseada em posicao
void inserir_inicio_lse(t_lse* lse, void* carga){
    t_elemento_lse* novo = criar_elem_lse(carga);
    novo->prox = lse->prim;
    lse->prim = novo;
    if(lse->ult == NULL){
        lse->ult = novo;
    }
    lse->tamanho++;
}

void inserir_final_lse(t_lse* lse, void* carga){
    t_elemento_lse* novo = criar_elem_lse(carga);
    if (lse->prim == NULL){
        lse->prim = novo;
        lse->ult = novo;
    }else{
        lse->ult->prox = novo;
        lse->ult = novo;
    }
    lse->tamanho++;
}

void inserir_conteudo_lse(t_lse* lse, void* carga){
    t_elemento_lse* novo = criar_elem_lse(carga);
    t_elemento_lse* ant=NULL;
    t_elemento_lse* cam = lse->prim;
    
    while( (cam!=NULL) && ( lse->comparador(cam->carga_util, carga) <= 0) ){
        ant = cam;
        cam = cam->prox;
    }
    if(lse->prim == NULL){
        lse->prim = lse->ult = novo;
    }else if (cam == lse->prim){ //inicio da lista 
        novo->prox = lse->prim;
        lse->prim = novo;
    }else if(cam == NULL){
        lse->ult->prox = novo;
        lse->ult = novo;
    }else{
        ant->prox = novo;
        novo->prox = cam;
    }
    lse->tamanho++;
}


void* remover_inicio_lse(t_lse* lse){
    assert(lse->ult!= NULL);
    
    t_elemento_lse* sucessor = lse->prim->prox;
    void* carga = lse->prim->carga_util;

    free(lse->prim);
    
    lse->prim=sucessor;
    lse->tamanho--;
    if (sucessor == NULL){
        lse->ult = NULL;
    }

    return carga;
}
/// @brief remover o último elemento da lista
/// @param lse 
/// @return carga util armazenada
void* remover_final_lse(t_lse* lse){
    assert(lse->prim!=NULL);
    void* carga;
    if(lse->prim == lse->ult){
        carga = lse->prim->carga_util;
        free(lse->prim);
        lse->prim = lse->ult = NULL;
    }else{
        t_elemento_lse* cam = lse->prim;
        while(cam->prox != lse->ult){
            cam = cam->prox;
        }
        carga = lse->ult->carga_util;
        free(lse->ult);
        lse->ult = cam;
    }
    lse->tamanho--;
    return carga;
}

void* remover_conteudo_lse(t_lse* lse, void* chave){
    assert(lse->prim!=NULL);

    void* carga = NULL;
    t_elemento_lse* ant = NULL;
    t_elemento_lse* cam = lse->prim;
    while((cam!=NULL) && (lse->comparador(cam->carga_util,chave)!=0) ){
        ant = cam;
        cam = cam->prox;
    }
    //
    if (cam!=NULL){
        if (cam == lse->prim){
            lse->prim = cam->prox;
            if(lse->prim == NULL)
                lse->ult=NULL;
        }else if (cam == lse->ult){
            ant->prox = NULL;
            lse->ult = ant;
        }else{
            ant->prox = cam->prox;
        }
        carga = cam->carga_util;
        free(cam);
        lse->tamanho--;
    }

    return carga;
}

void* acessar_lse(t_lse* lse, int pos){
    assert(pos>=1 && pos<=lse->tamanho);
    t_elemento_lse* cam = lse->prim;
    for(int i=1;i<pos;i++){
        cam = cam->prox;
    }
    return cam->carga_util;
}
/// @brief acessar um elemento baseado em um valor
/// @param lse 
/// @param carga 
/// @return 
void* acessar_conteudo_lse(t_lse* lse, void* chave){
    void* carga=NULL;
    t_elemento_lse* cam = lse->prim;
    while( (cam!=NULL) && (lse->comparador(cam->carga_util, chave)!=0) ){
        cam = cam->prox;
    }
    if(cam!=NULL){
        carga = cam->carga_util;
    }
    return carga;
}

void imprimir_lse(t_lse* lse){
    t_elemento_lse* cam = lse->prim;
    while (cam!=NULL){
        //printf("%p\n", cam->carga_util);
        // chamar a função especialista 
        lse->impressora(cam->carga_util);
        cam = cam->prox;
    }
    
}


// int main(){
//     t_lse* lse = criar_lse();
//     inserir_conteudo_lse(lse, 30);
//     inserir_conteudo_lse(lse, 10);
//     inserir_conteudo_lse(lse, 15);

//     int carga = acessar_lse(lse, 1);
//     printf("Acesso: %d\n", carga);

//     carga = acessar_lse(lse, 3);
//     printf("Acesso: %d\n", carga);

//     carga = remover_conteudo_lse(lse, 30);
//     printf("remover: %d\n", carga);


//     carga = acessar_conteudo_lse(lse, 30);
//     printf("acessar: %d\n", carga);

//     // carga = acessar_lse(lse, 3);
//     // printf("Acesso: %d\n", carga);


//     // int carga=remover_inicio_lse(lse);
//     // printf("removido do inicio: %d\n", carga);

//     // carga = remover_final_lse(lse);
//     // printf("removido do final: %d\n", carga);

//     // inserir_final_lse(lse, 40);
//     // carga = acessar_lse(lse, 1);
//     // printf("Acesso: %d\n", carga);

//     destroy_lse(lse);
// }






