#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thash.h"
#include "ab.h"

void executar_comandos(const char* nome_arquivo, t_hash *hash_table, t_ab *arvore_binaria) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return;
    }

    char comando[10];
    char chave[20];
    char valor[50];

    while (fscanf(arquivo, "%s", comando) != EOF) {
        if (strcmp(comando, "INSERT") == 0) {
            fscanf(arquivo, "%s %s", chave, valor);
            t_item *item = malloc(sizeof(t_item));
            strcpy(item->chave, chave);
            strcpy(item->valor, valor);
            inserir_ab(arvore_binaria, item);
            int chave_int = atoi(chave);
            inserir_hash(hash_table, chave_int, valor);
        } else if (strcmp(comando, "SEARCH") == 0) {
            fscanf(arquivo, "%s", chave);
            buscar_ab(arvore_binaria, chave);
            int chave_int = atoi(chave);
            buscar_hash(hash_table, chave_int);
        } else if (strcmp(comando, "DELETE") == 0) {
            fscanf(arquivo, "%s", chave);
            remover_ab(arvore_binaria, chave);
            int chave_int = atoi(chave);
            remover_hash(hash_table, chave_int);
        } else if (strcmp(comando, "DELETE") == 0) {
        fscanf(arquivo, "%s", chave);
        printf("Deletando chave: %s\n", chave);
        remover_ab(arvore_binaria, chave);
        remover_hash(hash_table, atoi(chave));
    } else {
        printf("Comando desconhecido: %s\n", comando);
    }
    fclose(arquivo);
    }   
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo de entrada>\n", argv[0]);
        return 1;
    }

    t_hash *hash_table = criar_hash(0.75);
    t_ab *arvore_binaria = criar_ab(imprimir_ab, comparar_ab); 

    executar_comandos(argv[1], hash_table, arvore_binaria);

    return 0;
}