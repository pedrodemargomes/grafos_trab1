#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h> 
#include "grafo.h"


//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo pode ser direcionado ou não
// 
// o grafo tem um nome, que é uma "string"

struct grafo {
	Agraph_t *grafo;
	int numNodes;
};


// ++++++++++++++++++++++++++++++++

int numVizinhosComuns(grafo g, Agnode_t *n, Agnode_t *m){
	Agedge_t *e;
	Agnode_t *a[1000],*b[1000];
	
	// Vizinhos do a
	int i = 0;
	for (e = agfstedge(g->grafo,n); e; e = agnxtedge(g->grafo,e,n)) {
		a[i] = aghead(e);
		i++;
	}

	int j = 0;
	// Vizinhos do b
	for (e = agfstedge(g->grafo,m); e; e = agnxtedge(g->grafo,e,m)) {	
		b[j] = aghead(e);
		j++;
	}

	int k,x;
	
	/*printf("a = ");
	for(k = 0;k < i;k++) {
		printf("%s ",agnameof(a[k]));
	}
	printf("\nb = ");
	for(x = 0;x < j;x++) {
		printf("%s",agnameof(b[x]));
	}
	printf("\n");*/

	int numViz = 0;
	for(k = 0;k < i;k++) {
		for(x = 0;x < j;x++) {
			if(a[k] == b[x])
				numViz++;
		}
	}

	return numViz;
}

Agnode_t **sub(grafo g, Agnode_t *m,Agnode_t *n) {
	Agedge_t *e;
	Agnode_t *a[1000];
	Agnode_t **r = calloc(1000,sizeof(Agnode_t *));
	
	// Vizinhos do a
	int i = 0;
	for (e = agfstedge(g->grafo,n); e; e = agnxtedge(g->grafo,e,n)) {
		a[i] = aghead(e);
		i++;
	}

	int j,b,k;
	k = 0;
	for (e = agfstedge(g->grafo,m); e; e = agnxtedge(g->grafo,e,m)) {	
		b = 1;
		for(j=0;j<i;j++) {
			if( a[j] == aghead(e) ) {
				b=0;
			}
		}
		if(b == 1) {
			r[k] = aghead(e);
			k++;
		}
	}
	
	return r;	
}

// ++++++++++++++++++++++++++++++++


//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário

int destroi_grafo(grafo g) {
	if(g == NULL)
		return 0;
  	agclose(g->grafo);
  	free(g);
	return 1;
}
//------------------------------------------------------------------------------
// lê um grafo no formato dot de input
// 
// devolve o grafo lido,
//         ou 
//         NULL, em caso de erro 

grafo le_grafo(FILE *input) {
  	grafo g = malloc(sizeof(struct grafo) );
  	g->grafo = agread(input,NULL);
	Agnode_t *n;

  	int numNodes = 0;
	for (n = agfstnode(g->grafo); n; n = agnxtnode(g->grafo,n)) {	
		numNodes++;
	}
	g->numNodes = numNodes;
	return g;
}
//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g) {
	agwrite(g->grafo,output);
	return g;
}
//------------------------------------------------------------------------------
// devolve o grafo de recomendações de g
//
// cada aresta {c,p} de H representa uma recomendação do produto p
// para o consumidor c, e tem um atributo "weight" que é um inteiro
// representando a intensidade da recomendação do produto p para o
// consumidor c.

grafo recomendacoes(grafo g){
	
	grafo gReco = malloc(sizeof(struct grafo));
	gReco->grafo = agopen("Grafo recomendacoes",Agundirected,NULL);
	agattr(gReco->grafo,AGEDGE,"weight","0");
	agattr(gReco->grafo,AGEDGE,"label","0");

	Agnode_t *c1,*c2,*n,*m;
	Agnode_t **r;
	
	Agnode_t *cReco;
	Agnode_t *pReco; 
	Agedge_t *novaAresta;

	int i,j,numViz,k,peso;
	char str[1000];
	for (n = agfstnode(g->grafo); n; n = agnxtnode(g->grafo,n)) {	
		if( agget(n,"tipo")[0] == 'c' ) {
			for (m = agfstnode(g->grafo); m; m = agnxtnode(g->grafo,m)) {
				if( m != n && agget(m,"tipo")[0] == 'c' ) {
					//printf("Testa %s e %s\n",agnameof(n),agnameof(m));
					numViz = numVizinhosComuns(g,n,m);	
					//printf("numViz = %d\n",numViz);
					cReco = agnode(gReco->grafo,agnameof(m),TRUE); // c			
					if( numViz  >= (agdegree(g->grafo,n,TRUE,TRUE)-numViz) ) {
						// Recomenda para m os produtos de n
						//printf("Recomenda os produtos de %s para o %s\n",agnameof(n),agnameof(m));
						
						r = sub(g,n,m);
						k = 0;
						while(r[k] != NULL) {
							//printf("%s ",agnameof( r[k]) );
							pReco = agnode(gReco->grafo,agnameof(r[k]),TRUE); // p	
							novaAresta = agedge(gReco->grafo,cReco,pReco,"",TRUE);
							strcpy(str, agget(novaAresta,"weight")); 
							peso = atoi(str) + 1;
							sprintf(str,"%d",peso);
							agset(novaAresta,"weight",str);	
							agset(novaAresta,"label",str);
							k++;
						}
						//printf("\n");

					}			
				}
			}
		} else {
			agnode(gReco->grafo,agnameof(n),TRUE);
		}
	}

	gReco->numNodes = g->numNodes;
	return gReco;
}

//------------------------------------------------------------------------------
