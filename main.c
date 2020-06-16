#include "Header.h"
#include "Trie.h"

int main(int argc, char** argv)
{
	const char exemplo[13][50] =
	{	
		"ama",
		"amar",
		"amarela",
		"amarelas",
		"amarelo",
		"amarelos",
		"modelo",
		"verde",
		"ver",
		"verdade",
		"item",
		"itens",
		"*"
	};
	const char* final = "*"; // marca de final
	int i = 0, n = 0;
	Trie* arv = _tr_cria("Teste", 200, 65);

	// insere

	i = 0;
	while (strcmp(exemplo[i], final) != 0)
	{	// ate ler "*"
		printf("    %s\n", exemplo[i]);
		n = _tr_insere((char*)exemplo[i], strlen(exemplo[i]), arv);
		printf("insere() retornou %d\n", n);
		i = i + 1; // ve o proximo
	};	// while

	// agora busca os mesmos valores

	i = 0;
	while (strcmp(exemplo[i], final) != 0)
	{	// ate ler "*"
		n = _tr_busca(exemplo[i], arv);
		printf("busca('%s') retornou %d\n", exemplo[i], n);
		i = i + 1; // ve o proximo
	};	// while

	n = _tr_busca("teste", arv);
	printf("busca('teste') retornou %d\n", n);

	n = _tr_busca("", arv);
	printf("busca('') retornou %d\n", n);

	n = _tr_busca("amarelada", arv);
	printf("busca('amarelada') retornou %d\n", n);

	_tr_navegador(arv);
	return 0;
};	// main()

// https://www.urionlinejudge.com.br/judge/pt/problems/view/1377