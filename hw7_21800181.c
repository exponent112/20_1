#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 30
#define one_line 630
#define INF 2000000000


void swap_key(int *m1, int *m2);
void max_heapify(int i);
void change_pc(int n);
void I_function(int vertex_num, int key);
struct Member  D_function(void);
void P_function(void);
void R_function(void);
void Dijkstra(int** matrix,int vertex_index,int vernum);
void K_function(int index, int change_value);
void Floyd(int** m,int vertex_index);
void whiteErase(char* eraseString);



struct Member {
    int vertex_num;
    int key;
};

struct Member m[30];
int number = 0;

int main(int argc, const char * argv[]) {
    FILE *fp,*wp;
    char** vertex_name = malloc(sizeof(char*)*MAX);
    for(int i=0;i<MAX;i++){
        vertex_name[i] = (char*)malloc(sizeof(char)*20);
    } //allocate vertex
    int **matrix = malloc(sizeof(int*)*MAX);
    for(int i=0;i<MAX;i++){
        matrix[i] = malloc(sizeof(int)*MAX);
    } //allocate matrix
    if ((fp = fopen("hw7.data", "r")) == NULL) {
      printf("파일 읽기 오류! \n");
      return 0;
    } //open data file
    if ((wp = fopen("output.txt", "w")) == NULL) {
      printf("파일 읽기 오류! \n");
      return 0;
    } //open data file
    char buffer[one_line];
    fgets(buffer, sizeof(buffer),fp);
    int temp =0;

    //white제거 함수 넣기
    //printf("%s",buffer);
    int vertex_index = 0;
    char *split_buffer = strtok(buffer,"\t\n ");
    while(split_buffer!=NULL){
        strcpy(vertex_name[vertex_index],split_buffer);
        whiteErase(vertex_name[vertex_index]);
        //printf("%d = %s\n",vertex_index,vertex_name[vertex_index]);
        vertex_index++;
        split_buffer = strtok(NULL,"\t\n ");
    }

    for(int i = 0; i<vertex_index;i++){
        fgets(buffer, sizeof(buffer),fp);
        int mat_index = 0;
        split_buffer = strtok(buffer,"\t\n ");
        split_buffer = strtok(NULL,"\t\n ");
        while(split_buffer!=NULL){
            if(mat_index<vertex_index){
                matrix[i][mat_index] = atoi(split_buffer);
                if( i!=mat_index&& matrix[i][mat_index] == 0) matrix[i][mat_index] = INF;
                //printf("%d = %d\n",mat_index,matrix[i][mat_index]);
            }
            mat_index++;
            split_buffer = strtok(NULL,"\t\n ");
        }
    }
    int** matrix2 = matrix;
    clock_t start1, end1;
    float result1;
    start1 = clock();
    for(int vernum= 0;vernum<vertex_index;vernum++)
        Dijkstra(matrix,vertex_index,vernum);
    end1 = clock();
    result1 = (float)(end1 - start1)/CLOCKS_PER_SEC;
    fprintf(wp,"It took %f seconds to compute shortest path between cities with Dijkstra's algorithm as follows.\n\n",result1);
    printf("It took %f seconds to compute shortest path between cities with Dijkstra's algorithm as follows.\n\n",result1);
    fprintf(wp,"\t\t");
     for(int tn = 0; tn<vertex_index;tn++)
         fprintf(wp,"%s  ",vertex_name[tn]);
    printf("\t\t");
    for(int tn = 0; tn<vertex_index;tn++)
        printf("%s  ",vertex_name[tn]);
    fprintf(wp,"\n");
    printf("\n");
    for(int tn = 0; tn<vertex_index;tn++){
        fprintf(wp,"%s\t ",vertex_name[tn]);
        for(int in = 0;in<vertex_index;in++){
            fprintf(wp,"%4d\t",matrix[tn][in]);
        }
        fprintf(wp,"\n");
    }
    for(int tn = 0; tn<vertex_index;tn++){
        printf("%s\t ",vertex_name[tn]);
        for(int in = 0;in<vertex_index;in++){
            printf("%4d\t",matrix[tn][in]);
        }
        printf("\n");
    }
    fprintf(wp,"\n\n");
    printf("\n\n");
    clock_t start2, end2;
    float result2;
    start2 = clock();
    Floyd(matrix2,vertex_index);
    end2 = clock();
    result2 = (float)(end2 - start2)/CLOCKS_PER_SEC;
    fprintf(wp,"It took %f seconds to compute shortest path between cities with Floyd algorithm as follows.\n\n",result2);
     printf("It took %f seconds to compute shortest path between cities with Floyd algorithm as follows.\n\n",result2);
    fprintf(wp,"\t\t");
    printf("\t\t");
     for(int tn = 0; tn<vertex_index;tn++)
         fprintf(wp,"%s  ",vertex_name[tn]);
    fprintf(wp,"\n");
    for(int tn = 0; tn<vertex_index;tn++)
            printf("%s  ",vertex_name[tn]);
       printf("\n");
    for(int tn = 0; tn<vertex_index;tn++){
        fprintf(wp,"%s\t ",vertex_name[tn]);
        for(int in = 0;in<vertex_index;in++){
            fprintf(wp,"%4d\t",matrix2[tn][in]);
        }
        fprintf(wp,"\n");
    }
    for(int tn = 0; tn<vertex_index;tn++){
           printf("%s\t ",vertex_name[tn]);
           for(int in = 0;in<vertex_index;in++){
               printf("%4d\t",matrix2[tn][in]);
           }
           printf("\n");
       }
    

            
    //delloc
    fclose(fp); //close data file
    fclose(wp); //close data file
    for(int j=0;j<MAX;j++){
        free(matrix[j]);
    }
    free(matrix);
    for(int j=0;j<MAX;j++){
       free(vertex_name[j]);
    }
    free(vertex_name);
    
    return 0;
}

void whiteErase(char* eraseString){
    int temp = 0;
    while(eraseString[temp])
    { temp++; }
    eraseString[temp - 1] = '\0';
}

void Floyd(int** m,int vertex_index){
    for(int k = 0;k<vertex_index;k++)
        for(int i = 0;i<vertex_index;i++)
            for(int j = 0;j<vertex_index;j++){
                if(m[i][k]+m[k][j]<m[i][j]){
                    m[i][j]=m[i][k]+m[k][j];
                }
            }
}

void Dijkstra(int** matrix,int vertex_index,int vernum){
    for(int s_in = 0; s_in<vertex_index;s_in++){
        I_function(s_in,matrix[vernum][s_in]);
    }
    while(number >= 1){
        struct Member ver_ = D_function();
        matrix[vernum][ver_.vertex_num] = ver_.key;
        for(int index = 0; index<vertex_index;index++){
            if(m[index].key > ver_.key+matrix[ver_.vertex_num][m[index].vertex_num]){
                K_function(index, ver_.key+matrix[ver_.vertex_num][m[index].vertex_num]);
            }
        }
    }
}


void max_heapify(int i){
  int left = i*2;
  int right = i*2+1;
  int small;
  if(left <= number && m[left].key<= m[i].key){
    small = left;
  }
  else small = i;
  if(right <= number && m[right].key<= m[small].key){
    small = right;
  }
  if(small != i) {
       swap_key(&m[i].key, &m[small].key);
    swap_key(&m[i].vertex_num, &m[small].vertex_num);
    max_heapify(small);
  }
}


void swap_key(int *m1, int *m2){
  int temp = (*m2);
  (*m2) = (*m1);
  (*m1) = temp;
}

void I_function(int vertex_num, int key){
  if(number>MAX){
    printf("too much element!\n");
    return;
  }
  number++;
  m[number].vertex_num = number-1; //vertex_name_index
  m[number].key = key;
  change_pc(number);
}

void change_pc(int n){
  int child = n;
    int parent = child / 2;
    while (child > 1 && m[parent].key > m[child].key) {
        swap_key(&m[parent].key, &m[child].key);
        swap_key(&m[parent].vertex_num, &m[child].vertex_num);
        child = parent;
        parent = child / 2;
    }
}

void P_function(){
  int temp = 1;
  while(temp <= number){
    temp++;
  }
  printf("\n");
}

void K_function(int index, int change_value){
  if(change_value>=m[index].key)
    {
      return;
    }
    else{
       m[index].key = change_value;
      change_pc(index);
    }
}

struct Member D_function(){
    swap_key(&m[1].key, &m[number].key);
    swap_key(&m[1].vertex_num, &m[number].vertex_num);
  number--;
  max_heapify(1);
    return m[number+1];
}

