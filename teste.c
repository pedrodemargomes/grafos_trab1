#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"

int main(void) {

	grafo g = le_grafo(stdin);

 	if ( !g )
 		return 1;


 	grafo r = recomendacoes(g);

	escreve_grafo(stdout, r);
	return !destroi_grafo(g);
}
