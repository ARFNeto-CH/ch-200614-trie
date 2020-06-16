#define _CRT_SECURE_NO_WARNINGS

#include "header.h"
#include "trecho.h"
#include "Trie.h"


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


int _t_busca(char* sub, Palavra* pal)
{
    printf("_busca('%s')\n", sub);
    if (pal == NULL) return -1;
    Palavra* t = pal; // no inicio
    int L = strlen(sub);
    for (int i = 0; i < L; i++)
    {
        printf("indice %d: Letra: '%c'\n", i, sub[i]);
        int idx = i;
        if (t->resto[sub[idx]] == NULL)
            return 0; // fim da linha
        t = t->resto[sub[idx]];
    };  // for()
    return t->aqui;
}   // _busca()


Palavra* _t_cria(const char letra)
{
    Palavra* palavra = (Palavra*)malloc(sizeof(Palavra));
    for (int i = 0; i < VALIDOS; i++) palavra->resto[i] = NULL;
    palavra->aqui = 0; // nada termina aqui
    palavra->letra = letra;
    return palavra;
};  // _cria()


int         _t_extrai_pos(Palavra* trie, char* str, int nivel)
{
    if (trie == NULL) return -1;
    Palavra* p = trie;
    if (nivel == 0)
    {
        printf("***** extrai_pos(%c)\n", p->letra);
    };
    if (p->aqui > 1)
    {
        str[nivel] = p->letra;
        str[nivel + 1] = 0;
        printf("\t|=> %s (*%d)\n", str, p->aqui);
    };
    for (int i = 1; i < VALIDOS; i++)
    {   // Post Order
        if (p->resto[i] != NULL)
        {
            str[nivel] = p->letra;
            str[nivel + 1] = 0;
            _t_extrai_pos(p->resto[i], str, nivel + 1);
        }
    };
    return 0;
};


void        _t_insere(char* sub, int L, Palavra* Subs)
{
    if (sub == NULL)  return; // sem string
    if (Subs == NULL) return; // sem arvore

    // monta a string para teste
    char* str = malloc(L + 1);
    memcpy(str, sub, L);
    *(str + L) = 0;
    printf("insere('%s') len = %d\n", str, L);
    free(str);
    // ok mostrou
    Palavra* t = Subs; // raiz
    int aceitos = 0;
    for (int i = 0; i < L; i++)
    {
        char letra = *(sub + i);
        int idx = ref[letra]; // o indice corrigido
        if (idx == 0) continue; // fora dos 64
        printf("Pos %d Letra: '%c', Ref: %d\n", i, letra, idx);
        if (t->resto[idx] == NULL)
        {   // nova letra 
            t->resto[idx] = _t_cria(letra);
            printf("Criado node para '%c'\n", letra);
        };  // criou node
        aceitos = aceitos + 1;
        t = t->resto[idx];
    };  // for()
    // se a linha era toda invalida pode nao inserido nada
    if (aceitos > 0)
    {
        t->aqui += 1;
        printf("\tinseridos %d de %d. Letra %c Aqui %d\n",
            aceitos, L, t->letra, t->aqui);
    };
    return;
};  // _insere()


void        _t_navegador(Palavra* dic)
{
    char str[50];
    char* ps = &str[0];
    // navega na arvore a partir de dic
    Palavra* t = dic;
    Palavra* antes = dic;
    char c = '@';
    int nivel = 0;
    *(ps + nivel) = c;
    *(ps + nivel + 1) = 0;
    while (c != ';')
    {
        // mostra o node corrente
        printf("\n\n[%d]\t%s\n\n==> %c [%d]\n",
            nivel, ps, t->letra, t->aqui);
        _z_explica(t);
        switch (c)
        {
        case 10:
            // newline
            break;

        case '@':
            t = dic; // volta pra raiz
            antes = t;
            nivel = 0;
            break;

        default:
            //printf("Letra: %d Ref:%d\n", c, ref[c]);
            if (ref[c] != 0)
            {   // pula pra esse
                // se tem alguem la
                if ((t->resto[ref[c]]) != 0)
                {
                    *(ps + nivel) = (t->resto[ref[c]])->letra;
                    nivel += 1;
                    *(ps + nivel) = 0; // termina string
                    antes = t;
                    t = t->resto[ref[c]]; // avanca
                };
            };
            break;
        };  // switch()
        printf("\n[@,;,(nivel)] : ");
        c = fgetc(stdin);
    };  // while()
};


Palavra* lista_dups(char* linha, Palavra* subs)
{
    int n = 0;
    int len = strlen(linha);
    int limite = len / 2;
    printf("\n\nEntrada: '%s' [%d]\n", linha, len);
    if (len > 1)
    {   // vai gerar as sequencias e inserir na arvore
        for (int l = len / 2; l >= 1; l = l - 1)
            _t_insere(linha, l, subs);
        return lista_dups(linha + 1, subs);
    }
    else
        return subs;
};  // lista_dups()


void _z_explica(Palavra* d)
{
    char n = 0;
    for (int i = 1; i < VALIDOS; i += 1)
    {
        if (d->resto[i] != NULL)
        {
            if (d->resto[i]->aqui > 0)
                n = '*';
            else
                n = ' ';
            printf("[ %c  %c ]", d->resto[i]->letra, n);
        }
        else
        {
            printf("[      ]");
        };  // if()
        if (i % 16 == 0) printf("\n");
    };
};


// fim

Trie* _tr_cria(const char* nome, uint16_t lim, uint16_t validos)
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
    // ao criar a arvore o ponteiro para os 
    // descendentes e null
    // para o primeiro link podera ser o ponteiro
    // para o compactado e a partir dai a logica segue
    t->raiz->lnkn = NULL;
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
    lnk_normal* ndt = NULL;
    return node;
};  // _tr_cria_node()


int        _tr_insere(char* sub, int L, Trie* trie)
{
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
        // pode nao ter ainda ponteiro algum depois dessa letra
        // em que estamos
        lnk_normal* p = (lnk_normal*)pTr->lnkn;
        if (p == NULL)
        {
            printf("Criando ponteiros a partir de '%c'\n",
                letra);
            p = (lnk_normal*)malloc(sizeof(lnk_normal));
            for (int i = 0; i < VALIDOS; i++) p->ch[i] = NULL;
        };  // if()
        if (p->ch[idx] == NULL)
        {   // nova letra 
            p->ch[idx] = _tr_cria_node(letra);
            printf("Criado node para '%c'\n", letra);
        };  // criou node
        aceitos = aceitos + 1;
        pTr = p->ch[idx];
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
