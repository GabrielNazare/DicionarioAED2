typedef struct hash t_hash;
t_hash* criar_hash(double fc);
t_hash* rehashing(t_hash *atual);
int encontrar_espaco_vazio(t_hash* t, int pos, int chave);
int funcao_hashing(t_hash* t, int chave);
void inserir_hash(t_hash* t, int chave, void* carga);
void* buscar_hash(t_hash* t, int chave);
void* remover_hash(t_hash* t, int chave);
void imprimir_hash(t_hash* t);

typedef void(*t_imprimir_lse)(void*);
typedef int(*t_comparar_lse)(void*, void*);
