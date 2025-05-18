#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    int i, j, k;

    // Se verifican las filas
    for (i = 0; i < 9; i++) {
        int seen[10] = {0}; // Para marcar números del 1 al 9
        for (j = 0; j < 9; j++) {
            int num = n->sudo[i][j]; 
            if (num == 0) continue; // Ignorar espacios vacíos
            if (seen[num]) return 0; // Si ya se vio el número, no es válido
            seen[num] = 1; // Marcar el número como visto
        }
    }

    // Se verifican las columnas con la misma lógica
    for (j = 0; j < 9; j++) {
        int seen[10] = {0};
        for (i = 0; i < 9; i++) {
            int num = n->sudo[i][j];
            if (num == 0) continue;
            if (seen[num]) return 0;
            seen[num] = 1;
        }
    }

    // Verificar submatrices de 3x3
    for (k = 0; k < 9; k++) {
        int seen[10] = {0};
        for (int p = 0; p < 9; p++) {
            int i = 3*(k/3) + p/3; // Fila de la submatriz
            int j = 3*(k%3) + p%3; // Columna de la submatriz
            int num = n->sudo[i][j];
            if (num == 0) continue;
            if (seen[num]) return 0;
            seen[num] = 1;
        }
    }

    return 1;
}


List* get_adj_nodes(Node* n){
   List* list = createList();

   int i, j;
   int found = 0; // Variable para marcar si se encontró un espacio vacío
   for (i = 0; i < 9 && !found; i++) { // Buscar la primera fila con un espacio vacío
      for (j = 0; j < 9; j++) {
         if (n->sudo[i][j] == 0) {
            found = 1; // Se encontró un espacio vacío
            break;
         }
      }
   }

   if (!found) return list; // Si no se encontró un espacio vacío, retornar lista vacía

   i--; // Ajustar i y j a la posición del espacio vacío

   for (int k = 1; k <= 9; k++) { // Probar números del 1 al 9
      Node* new_node = copy(n); 
      new_node->sudo[i][j] = k; // Asignar el número k al espacio vacío
      pushBack(list, new_node);
   }

   return list;
}




int is_final(Node* n){
   for (int i = 0; i < 9; i++) { // Itera por todas las filas
      for (int j = 0; j < 9; j++) { // Itera por todas las columnas
         if (n->sudo[i][j] == 0) return 0; // Si hay un espacio vacío, es nulo
      }
   }
   return 1;
}

Node* DFS(Node* initial, int* cont){
    Stack* S = createStack();
    push(S, initial);
    *cont = 0;

    while (!is_empty(S)) {
        Node* current = top(S); // Obtener el nodo del tope
        pop(S);                 // Sacarlo de la pila
        (*cont)++;

        if (is_final(current)) {
            return current;    // Fin
        }

        List* adj = get_adj_nodes(current); // Obtener nodos adyacentes
        Node* adjNode = first(adj);

        while (adjNode != NULL) {
            push(S, adjNode); // Agregar nodo a la pila
            adjNode = next(adj);
        }

        free(current);
    }

    return NULL; 
}