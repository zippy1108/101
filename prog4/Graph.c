//Graph.c
//Aaron Hom
//awhom
//pa4
//Graph ADT used to perform operations on graphs

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

//Color scale for BFS
#define white 1
#define gray 2
#define black 3

struct GraphObj{
  List *adj;
  int *color;
  int *parent;
  int *distance;
  int source;
  int order;
  int size;
};

//Constructors-Destructors -------------------------------
//Graph()
//Creates a new graph that has n vertices and no edges
  Graph newGraph(int n){
    Graph G = malloc(sizeof(struct GraphObj)); //allocate memory for all parts of graph
    G->adj = calloc(n + 1, sizeof(List));
    G->color = calloc(n + 1, sizeof(int));
    G->parent = calloc(n + 1, sizeof(int));
    G->distance = calloc(n + 1, sizeof(int));
    G->source = NIL;
    G->order = n;
    G->size = 0;
    for(int i = 0; i < (n+1); i++){ //makes graph 
      G->adj[i] = newList();
      G->color[i] = white;
      G->parent[i] = NIL;
      G->distance[i] = INF;
    }
    return G;
  }

//freeGraph()
//Frees all dynamic memory associated with *pG and sets *pG to null
  void freeGraph(Graph* pG){
    Graph useless = *pG;
    for(int i = 0; i < (useless->order + 1); i++){
      freeList(&(useless->adj[i]));
    }
    free(useless->adj);
    free(useless->color);
    free(useless->parent);
    free(useless->distance);
    free(*pG);
    *pG = NULL;
  }

//Access Functions ---------------------------------------
//getOrder()
//Returns number of vertices in graph
  int getOrder(Graph G){
    return G->order;
  }

//getSize()
//Returns number of edges in graph
  int getSize(Graph G){
    return G->size;
  }

//getSource()
//Returns most recent source from using BFS(G, s)
  int getSource(Graph G){
    return G->source;
  }

//getParent()
//Returns parent of given vertex (u) from graph G
//Pre: 1 <= u <= getOrder(G)
  int getParent(Graph G, int u){
  if(u > getOrder(G) || u < 1){
    printf("Graph Error: called getParent() with out of bounds vertex\n");
    exit(1);
  }
  return G->parent[u];
}
 
//getDistance()
//Returns distance from the source of a graph to a given vertex
//Pre: 1 <= u <= getOrder(G)
  int getDist(Graph G, int u){
    if(getSource(G) == NIL){
      return INF;
    }
    if(u > getOrder(G) || u < 1){
      printf("Graph Error: called getDist() with out of bounds vertex\n ");
      exit(1);
    }
    return G->distance[u];
  }

//getPath()
//Returns the path from source to vertex
//Pre: 1 <= u <= getOrder(G)
  void getPath(List L, Graph G, int u){
  if(u > getOrder(G) || u < 1){
    printf("Graph Error: called getPath() with out of bounds vertex\n");
    exit(1);
  }
  if(getSource(G) == NIL){
    printf("Graph Error: called getPath() with a null source\n");
    exit(1);
  }
  int center = G->source; //initalizes center as source
  if(u == center){ //if vertex = source   
    prepend(L, center); //add center to beginning of List L
  }else if(G->parent[u] == NIL){ //if there are no parents of u
    append(L, NIL); //add NIL to back of list L
  }else{
    prepend(L, u); //otherwise put u at front
    getPath(L, G, G->parent[u]); //and find path between vertex and source
  }
}

//Manipulation Procedures---------------------------------
//makeNull()
//Clears all adjacency lists for graph
  void makeNull(Graph G){
    for(int i = 0; i < (G->order + 1); i++){
      clear(G->adj[i]);
    }
  }

//addEdge()
//Adds an undirected edge to graph from vertex u to vertex g
//Pre: 1 <= u & v <= getOrder(G)
  void addEdge(Graph G, int u, int v){
    if(u > getOrder(G)||v > getOrder(G)||u < 1|| v < 1){
      printf("Graph Error: called addEdge() with out of bounds vertice(s)\n");
      exit(1);
    }
    addArc(G, u, v); //adds directed edges in both directions to link vertices 
    addArc(G, v, u);
    G->size--; //decrease # of edges 
  }

//addArc()
//Adds a directed edge to graph from vertex u to vertex g
//Pre: 1 <= u & v <= getOrder(G)
  void addArc(Graph G, int u, int v){
    if(u > getOrder(G)||v > getOrder(G)||u < 1|| v < 1){
      printf("Graph Error: called addEdge() with out of bounds vertice(s)\n");
      exit(1);
    }
    List L = G->adj[u];
    moveFront(L);
    while(index(L) > -1 && v > get(L)){
      moveNext(L); //move through list 
    } 
    if(index(L) == -1){ //once out of bounds
      append(L, v); //add v to beginning of list
    }else{
      insertBefore(L, v);
    }
      G->size++; //increase # of edges     
  }

//BFS()
//Performs Breadth First Search on graph using s as the source
  void BFS(Graph G, int s){
    for(int i = 0; i < (G->order +1); i++){ //initialize colors, parents, distance
      G->parent[i] = NIL;  
      G->distance[i] = INF;
      G->color[i] = white;
    }
    G->source = s; 
    G->distance[s] = 0;
    G->color[s] = gray;
    List L = newList(); //create new list
    prepend(L, s); //add element to front of list
    while(length(L) > 0){ 
      int val = back(L); 
      deleteBack(L);
      List adj = G->adj[val]; //establish adjacency list
      moveFront(adj);

      while(index(adj) > -1){
	int x = get(adj);
	if(G->color[x] == white){ 
	  G->parent[x] = val; 
	  G->distance[x] = G->distance[val] + 1;
	  G->color[x] = gray; //change color
	  prepend(L, x); //put x at front of list
	}
	moveNext(adj); //move next in adjacency list
      }
    }
    freeList(&L);
  }
//Other operations ---------------------------------------
//printGraph()
//Prints graph to an out file using iteration to print out each adjacency list
  void printGraph(FILE* out, Graph G){
    if(G == NULL||out == NULL){
      printf("Graph Error: called printGraph() on null reference(s)\n");
      exit(1);
    }
    for(int i = 1; i <= getOrder(G); i++){
      fprintf(out, "%d: ", i);
      printList(out, G->adj[i]);
      fprintf(out, "\n");
    }
  }

