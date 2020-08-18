#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX 40

struct Node {
    struct Node* next;
    int vertex;
};

void pushNode (struct Node *node,int vertex);
void printList (struct Node *node);


int main(int argc, const char * argv[]) {
    FILE *fp;
    struct Node *link = malloc(sizeof(struct Node*)*MAX);
    int **matrix = malloc(sizeof(int*)*MAX);
    for(int i=0;i<MAX;i++){
        matrix[i] = malloc(sizeof(int)*MAX);
    } //allocate matrix
    if ((fp = fopen("hw6_data.txt", "r")) == NULL) {
      printf("파일 읽기 오류! \n");
      return 0;
    } //open data file
    char temp;
    int i = 0;
    int j = 0;
    int vertex = -1;

    while(fscanf(fp,"%c",&temp)!= -1){
        if(temp == '\n'){
            if(i==0 && j == 0) continue;
            j = 0;
            i++;
            continue;
        }
        else if(temp == '0'){
            matrix[i][j] = 0;
            j++;
        }
        else if(temp == '1'){
            matrix[i][j] = 1;
            j++;
        }
        else if(i == 0 && j ==0 && ((temp>='A' && temp <='Z') || (temp>='a' && temp <='z')))
                     vertex++;

    } //read file
    for(int r = 0; r<vertex; r++){
        struct Node *head = malloc(sizeof(struct Node));
        head->vertex = r;
        head->next = NULL;
        link[r] = *head;
    }

    for(int r = 0;r<vertex;r++){
        for(int t = 0;t<vertex;t++){
            if(matrix[r][t]== 1){
                pushNode(&link[r],t);
            }
        }
    }//make adj list

    printf("\nArray of adjacency list of above graph\n");
    
    for(int r = 0;r<vertex;r++){
        printList(&link[r]);
        printf("\n");
    }
    free(link);

    int t_matrix[vertex][vertex];
    for(int r = 0;r<vertex;r++){
        for(int t = 0;t<vertex;t++)
            t_matrix[r][t] = matrix[t][r];
    } //trans
    for(int free_m = 0; free_m<vertex; free_m++)
      free(matrix[free_m]);
    free(matrix);
    
    struct Node *t_link = malloc(sizeof(struct Node*)*MAX);
    for(int r = 0; r<vertex; r++){
        struct Node *head = malloc(sizeof(struct Node));
        head->vertex = r;
        head->next = NULL;
        t_link[r] = *head;
    }
    
    for(int r = 0;r<vertex;r++){
        for(int t = 0;t<vertex;t++){
            if(t_matrix[r][t]== 1){
                pushNode(&t_link[r],t);
            }
        }
    }//make trans adj list

    printf("\n\nArray of adjacency list of transpose graph\n");
    for(int r = 0;r<vertex;r++){
        printList(&t_link[r]);
        printf("\n");
    }
    free(t_link);
    
    fclose(fp); //close data file
    return 0;
}


void pushNode (struct Node *node,int vertex){
    while(node->next !=NULL)
        node = node->next;
    struct Node *new_Node = malloc(sizeof(struct Node));
    new_Node->vertex = vertex;
    new_Node->next = NULL;
    node->next = new_Node;
}

void printList (struct Node *node){
    while(node->next !=NULL){
        printf("[%c | ] -> ",node->vertex+65);
        node = node->next;
    }
     printf("[%c | NIL]",node->vertex+65);
}
