#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define         VALIDOS         (65)
#define         _PILHA_         (256)
#define         _LIMITE_        (256)

typedef struct Node Palavra;
struct Node
{
    char letra;
    Palavra* resto[VALIDOS];
    int aqui;
};

Palavra*    _t_apaga(Palavra*);
int         _t_busca(char*, Palavra*);
Palavra*    _t_cria(char);
int         _t_extrai_pos(Palavra*, char*, int level);
void        _t_insere(char*, int, Palavra*);
void        _t_navegador(Palavra*);

Palavra*    lista_dups(char*, Palavra*);
// fim

