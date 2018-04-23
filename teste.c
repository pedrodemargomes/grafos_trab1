#include <stdlib.h>
#include <stdio.h>
#include <graphviz/cgraph.h>

//------------------------------------------------------------------------------

int numVizinhosComuns(Agraph_t *g, Agnode_t *n, Agnode_t *m ){
	Agedge_t *e;
	Agnode_t *a[1000],*b[1000];
	
	// Vizinhos do a
	int i = 0;
	for (e = agfstedge(g,n); e; e = agnxtedge(g,e,n)) {
		a[i] = aghead(e);
		i++;
	}

	int j = 0;
	// Vizinhos do b
	for (e = agfstedge(g,m); e; e = agnxtedge(g,e,m)) {	
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

Agnode_t **sub(Agraph_t *g, Agnode_t *m,Agnode_t *n) {
	Agedge_t *e;
	Agnode_t *a[1000];
	Agnode_t **r = calloc(1000,sizeof(Agnode_t *));
	
	// Vizinhos do a
	int i = 0;
	for (e = agfstedge(g,n); e; e = agnxtedge(g,e,n)) {
		a[i] = aghead(e);
		i++;
	}

	int j,b,k;
	k = 0;
	for (e = agfstedge(g,m); e; e = agnxtedge(g,e,m)) {	
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


int main(void) {

	Agraph_t *g = agread(stdin, NULL);

	Agraph_t *gReco = agopen("Grafo recomendacoes",Agundirected,NULL);

	Agnode_t *c1,*c2,*n,*m;
	Agnode_t **r;
	
	Agnode_t *cReco;
	Agnode_t *pReco; 

	int i,j,numViz,k;
	for (n = agfstnode(g); n; n = agnxtnode(g,n)) {	
		if( agget(n,"tipo")[0] == 'c' ) {
			for (m = agfstnode(g); m; m = agnxtnode(g,m)) {
				if( m != n && agget(m,"tipo")[0] == 'c' ) {
					printf("Testa %s e %s\n",agnameof(n),agnameof(m));
					numViz = numVizinhosComuns(g,n,m);	
					//printf("numViz = %d\n",numViz);
					cReco = agnode(gReco,agnameof(m),TRUE); // c			
					if( numViz  >= (agdegree(g,n,TRUE,TRUE)-numViz) ) {
						// Recomenda para m os produtos de n
						printf("Recomenda os produtos de %s para o %s\n",agnameof(n),agnameof(m));
						
						r = sub(g,n,m);
						k = 0;
						while(r[k] != NULL) {
							printf("%s ",agnameof( r[k]) );
							pReco = agnode(gReco,agnameof(r[k]),TRUE); // p	
							agedge(gReco,cReco,pReco,"",TRUE);
							k++;
						}
						printf("\n");

					}			
				}
			}
		} else {
			agnode(gReco,agnameof(n),TRUE);
		}
	}
	
	agwrite(gReco,stdout);
	
	agclose(g);
	return 0;
}
