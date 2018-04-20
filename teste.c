#include <stdlib.h>
#include <stdio.h>
#include <graphviz/cgraph.h>

//------------------------------------------------------------------------------

int numVizinhosComuns( Agnode_t *a, Agnode_t *b ){
	Agedge_t *e;
	int a[1000],b[1000];
	// Vizinhos do a
	int i = 0;
	for (e = agfstedge(g,a); e; e = agnxtedge(g,e,a)) {
		sscanf(agnameof( agtail(e) ),"%d",&a[i] ); 
		printf("%s",agnameof( agtail(e) ));
		i++;
	}

	int j = 0;
	// Vizinhos do b
	for (e = agfstedge(g,b); e; e = agnxtedge(g,e,b)) {	
		sscanf(agnameof( agtail(e) ),"%d",&b[j] ); 
		printf("%s",agnameof( agtail(e) ));
		j++
	}
	int k,x;
	for(k = 0;k < i;k++) {
		for(k = 0;k < i;k++) {
			
		}
	}


}


int main(void) {

	Agraph_t *g = agread(stdin, NULL);

	int num_nodes = agnnodes(g);		
	int num_cons = 9;

	Agnode_t *c1,*c2;
		
	int i,j;

	printf("%d\n", agdegree(g,p,TRUE,TRUE) );	
	printf("%d\n", agedge(g,c,p,NULL,FALSE) );	

	char str[5000];
	for(i=1;i<=num_cons;i++) {	
		sprintf(str,"C%d",i);	
		c1 = agnode(g,str,FALSE);
		for(j=i+1;j<=num_cons;j++) {			
			sprintf(str,"C%d",j);
			c2 = agnode(g,str,FALSE);
			
			numViz = numVizinhosComuns(c1,c2);	
			if( numViz  > (agdegree(g,c1,TRUE,TRUE)-agdegree(g,c2,TRUE,TRUE)) ) {
				// Recomenda para c2 os produtos de c1
				
			}
			if( numViz > (agdegree(g,c2,TRUE,TRUE)-agdegree(g,c1,TRUE,TRUE)) ) {
				// Recomenda para c1 os produtos de c2
				
			}
		
		}
		
	}


	//agwrite(g,stdout);
	
	agclose(g);
	return 0;
}
