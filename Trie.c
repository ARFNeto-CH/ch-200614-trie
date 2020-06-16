#define _CRT_SECURE_NO_WARNINGS

#include "header.h"
#include "trecho.h"
#include "Trie.h"

// prototipos locais

Node* _tr_cria_node(const char);
void  _z_explica(Node*, Trie*);


Palavra* _t_apaga(Palavra* uma)
{
    // antes de apagar essa apaga as que vem depois
    // apaga essa na volta
    if (uma == NULL) return NULL;
    for (int i = 0; i < VALIDOS; i++)
    {
        if (uma->resto[i] != NULL) _t_apaga(uma->resto[i]);
    };  // for() 
    free(uma);
    return NULL;
};   // _apaga()


void _z_explica(Node* d, Trie* A)
{
    if (d->ch == NULL)
    {
        printf("\tNode sem descendentes\n");
        return;
    };  // if()

    char n = 0;
    for (int i = 1; i < A->validos; i += 1)
    {
        if (d->ch[i] != NULL)
        {
            if (d->ch[i]->n_aqui > 0)
                n = '*';
            else
                n = ' ';
            printf("%c%c ", d->ch[i]->letra, n);
        };  // if()
    };
    printf("\n");
    return;
};  // _z_explica()


// fim

int         _tr_busca(const char* sub, Trie* arv)
{
    //printf("_busca('%s')\n", sub);
    if (arv == NULL) return -1;
    Node* t = arv->raiz; // no inicio
    int L = strlen(sub);
    for (int i = 0; i < L; i++)
    {
        //printf("indice %d: Letra: '%c'\n", i, sub[i]);
        int idx = i;
        if (t->ch[ref[sub[idx]]] == NULL)
            return -1; // fim da linha
        t = t->ch[ref[sub[idx]]];
    };  // for()
    return t->n_aqui;
}   // _busca()


Trie*       _tr_cria(const char* nome, uint16_t lim, uint16_t validos)
{
    Trie* t = (Trie*)malloc(sizeof(Trie));
    if (t == NULL) return NULL;
    t->validos = validos;
    t->max_nodes = lim;
    t->niveis = 0;

    // grava o nome
    t->nome = (char*)malloc(strlen(nome) + 1);
    if (t->nome == NULL)
    {
        free(t);
        return NULL;
    };  // if()
    strcpy(t->nome, nome);
    t->ops = 0; // sem comprimir

    // prepara o node raiz
    t->raiz = _tr_cria_node(0);
    if (t->raiz == NULL)
    {
        free(t->nome);
        free(t);
        return NULL;
    };  // if()
    return t;
};  // _tr_cria()


inline Node* _tr_cria_node(const char letra) 
{
    // criar um node normal implica em criar o vetor
    // de ponteiros com um ponteiro para cada possivel
    // letra. O ponteiro para a struct lnk_normal;

    // antes cria um node
    Node* node = (Node*)malloc(sizeof(Node));
    node->n_aqui = 0; // nada termina aqui
    node->letra = letra;
    node->ops = 0;
    node->lnkz = NULL;
    return node;
};  // _tr_cria_node()


int         _tr_insere(char* sub, int L, Trie* trie)
{
    // insere a palavra toda na arvore
    if (trie == NULL)  return -1; // sem string
    if (trie->raiz == NULL) return -2; // sem arvore

    // monta a string para mostrar durante os testes
    char* str = malloc(L + 1);
    memcpy(str, sub, L);
    *(str + L) = 0;
    printf("insere('%s') len = %d\n", str, L);
    free(str); // ok mostrou e liberou

    // agora navega em busca da palavra. Pode ser preciso criar
    // nodes por todo o caminho, ou pode ser preciso criar apenas
    // um sufixo, ou pode ser que a palavra ja esteja na arvore
    // e entao so aumenta o contados n_aqui para ela

    Node* pTr = trie->raiz; // raiz
    int aceitos = 0;
    for (int i = 0; i < L; i++)
    {   // as letras todas ate L
        char letra = *(sub + i);
        int idx = ref[letra]; // o indice corrigido
        if (idx == 0) continue; // fora dos 64
        printf("Pos %d Letra: '%c', Ref: %d\n", i, letra, idx);
        // vamos identificar o node para essa letra
        // o node pode nao existir ainda. Se ops == '0' entao
        // tem um vetor com um ponteiro para cada letra
        // se diferente tem um vetor de strings e um vetor
        // de indicadores para cada letra a partir desse no

        // node nao comprimido
        if (pTr->ch == NULL)
        {
            printf("Criando ponteiros a partir de '%c'\n",
                letra);
            pTr->ch = (Node**)malloc(trie->validos * sizeof(Node*));
            for (int i = 0; i < trie->validos; i++) pTr->ch[i] = NULL;
            printf("Criados %d ponteiros\n", trie->validos);
        }
        // pTr agora aponta para um vetor de nodes
        if (pTr->ch[idx] == NULL)
        {   // nova letra 
            pTr->ch[idx] = _tr_cria_node(letra);
            printf("Criado node para '%c'\n", letra);
        };  // criou node
        aceitos = aceitos + 1;
        pTr = pTr->ch[idx];
    };  // for()
    // se a linha era toda invalida pode nao inserido nada
    if (aceitos > 0)
    {
        pTr->n_aqui += 1;
        printf("\tinseridos %d de %d. Letra %c Ocorrencias: %d\n",
            aceitos, L, pTr->letra, pTr->n_aqui);
    };  // if()
    return 0;
};  // _insere()


int         _tr_navegador(Trie* A)
{
    char str[50];
    char* ps = &str[0];
    // navega na arvore trie
    Node* t = A->raiz;
    char c = 0;
    int nivel = 0;
    *(ps + nivel) = c;
    *(ps + nivel + 1) = 0;

    printf("Trie: '%s' Nodes: %d validos: %d niveis: %d limite: %d\n\n",
        A->nome, A->size, A->validos, A->niveis, A->max_nodes);
   
    while (c != ';')
    {
        // mostra o node corrente
        if (t->letra == 0)
        {
            printf("Na raiz da Trie '%s'\n", A->nome);
        }
        else
        {
            printf("\n\n[%d]\t%s\n\n==> %c [%d]\n",
                A->niveis, ps, t->letra, t->n_aqui);
        };
        _z_explica(t, A);
        switch (c)
        {
        case 10:
            // newline
            break;

        case '@':
            t = A->raiz;
            nivel = 0;
            break;

        default:
            printf("Letra: %d Ref:%d\n", c, ref[c]);
            if (ref[c] != 0)
            {   // pula pra esse
                // se tem alguem la
                if ((t->ch[ref[c]]) != 0)
                {
                    *(ps + nivel) = (t->ch[ref[c]])->letra;
                    nivel += 1;
                    *(ps + nivel) = 0; // termina string
                    t = t->ch[ref[c]]; // avanca
                };
            };
            break;
        };  // switch()
        printf("\n[@,;,(nivel)] : ");
        c = fgetc(stdin);
    };  // while()
    return 0;
};
