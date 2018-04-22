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
		a[i] = agtail(e);
		i++;
	}

	int j = 0;
	// Vizinhos do b
	for (e = agfstedge(g,m); e; e = agnxtedge(g,e,m)) {	
		b[j] = agtail(e);
		j++;
	}
	int k,x;
	int numViz = 0;
	for(k = 0;k < i;k++) {
		for(x = 0;x < j;x++) {
			if(a[k] == b[x])
				numViz++;
		}
	}


	return numViz;
}


int main(void) {

	Agraph_t *g = agread(stdin, NULL);
	
	Agnode_t *c1,*c2,*n,*m;

	int i,j,numViz;

	for (n = agfstnode(g); n; n = agnxtnode(g,n)) {	
		if( agget(n,"tipo")[0] == 'c' ) {
			for (m = agfstnode(g); m; m = agnxtnode(g,m)) {			
				if( m != n && agget(m,"tipo")[0] == 'c' ) {
					printf("Testa %s e %s\n",agnameof(n),agnameof(m));
					numViz = numVizinhosComuns(g,n,m);	
					if( numViz  >= (agdegree(g,n,TRUE,TRUE)-agdegree(g,m,TRUE,TRUE)) ) {
						// Recomenda para m os produtos de n
						printf("Recomenda os produtos de %s para o %s\n",agnameof(n),agnameof(m));	
					}			
				}
			}
		}
	}
	
	//agwrite(g,stdout);
	
	agclose(g);
	return 0;
}
