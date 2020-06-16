#pragma once
#include <stdint.h>

typedef struct
{
    char*   filhos;
    int*    info_filhos;
}   lnk_zip;     // "zipped"


struct node
{
    char        letra; // a propria
    uint32_t    n_aqui; // chaves aqui
    char        ops; // indicadores
    union {
        struct node**   ch;
        lnk_zip*    lnkz;
    };
};
typedef struct node Node;


typedef struct
{
    uint16_t    max_nodes;
    uint16_t    niveis;
    char*       nome;
    char        ops;
    Node*       raiz;
    uint16_t    size;
    uint16_t    validos;
}   Trie;

int     _tr_busca(const char*, Trie*);
Trie*   _tr_cria(const char*, uint16_t, uint16_t);
int     _tr_insere(char*, int, Trie*);
int     _tr_navegador(Trie*);
