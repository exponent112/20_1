//My program works for greedy, DP solution and BB solution 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 10001

struct item{
    int benefit;
    int weight;
    float value;
};

typedef struct NodeStruct
{
int weight;
int benefit;
int index;
float bound;
}Node;


void MakeTreasure (int n,struct item treasure[]);
void Greedy (int W,struct item treasure[],int n);
int compare(const void *A, const void  *B);
void DP_knapSack(int W, struct item treasure[], int n);
int max(int a, int b);
void heap_init();
void h_swap(Node *a, Node *b) ;
int push(Node value);
Node pop() ;
float BB_knapsack(struct item treasure[], int n, int w);
void printTableName(FILE* fp);
void soultionPrint(float BB_value, FILE* fp,int n);

float greedy_time = 0; //Greedy running time
float greedy_benefit = 0; //Greedy total benefit

float DP_time = 0; //DP running time
int DP_benefit = 0; //DP total benefit

float BB_time = 0; //BB running time

Node heap[MAX_SIZE];
int size; //heap size

int main(void) {
    srand(time(NULL));
    int nArray[10] = {10,100,500,1000,3000,5000,7000,9000,10000};
    FILE *fp = fopen("output.txt", "wt"); //open file
    printTableName(fp);
    for(int i = 0;i<9;i++){
        int n = nArray[i];
        struct item* treasure;
        treasure = malloc(sizeof(struct item)*n);
        int W = n*40;
        MakeTreasure(n,treasure); //make treasure array
        DP_knapSack(W, treasure,n); //DP solution
        qsort( treasure,n,sizeof(struct item),compare);
        Greedy (W,treasure,n); //greedy knapsack solution
        float BB_value = BB_knapsack(treasure,n,W); //BB solution
        soultionPrint(BB_value,fp,n);
    }
    fclose(fp);
    return 0;
}


float BB_knapsack(struct item treasure[], int n, int w){
    clock_t start, end;
    float result;
    start = clock(); // start time
    
    heap_init();
    float max_benefit = 0;
    int weight = 0;
    Node root;
    root.weight =  root.index = root.benefit = 0; //root initial
    for(int i = 0; i < n; i++){
        if(weight+treasure[i].weight <= w){
            weight += treasure[i].weight;
            root.bound += treasure[i].benefit;
        }
        else {
            root.bound += (w-weight)*treasure[i].value;
            break;
        }
    }
    push(root);

    while(1){
        clock_t checkTime = clock();
        float checkT = (float)(checkTime - start)/CLOCKS_PER_SEC;
        if(checkT>900) {
            printf("The time is over 15 minutes\n");
            return 0;
        } //Check running time
        Node temp = pop();
        Node child[2];
        if(temp.benefit == -1) break; //size <= 0
        int index = temp.index + 1;
        if(temp.bound < max_benefit || temp.index == n-1) continue;
        child[1].weight = temp.weight;
        child[0].weight = temp.weight + treasure[index-1].weight;
        child[1].benefit = temp.benefit;
        child[0].benefit = temp.benefit +treasure[index-1].benefit;
        
        for(int i = 0; i < 2; i++){
            child[i].index = index;
            child[i].bound = child[i].benefit;
            if(w < child[i].weight) continue;
            weight = child[i].weight;
            for(int j = index; j < n; j++){
                if(weight+treasure[j].weight <= w){
                   child[i].bound += treasure[j].benefit;
                   weight += treasure[j].weight;
                }
                else {
                    child[i].bound += (w-weight)*treasure[j].value;
                    break;
                }
            } //children bound
            if(child[i].bound < max_benefit) continue;
            if(child[i].benefit > max_benefit) max_benefit = child[i].benefit;
            push(child[i]);
        }
    }
    end = clock();
    result = (float)(end - start)/CLOCKS_PER_SEC;
    result*=1000;
    BB_time = result;
    return max_benefit;
}

void DP_knapSack(int W, struct item treasure[], int n)
{
    clock_t start, end;
    float result;
     start = clock(); // start time
   int i, w;
    int **B = malloc(sizeof(int*)*(n+1));
    for(int t=0;t<n+1;t++){
        B[t] = malloc(sizeof(int)*(W+1));
    }
   for (i = 0; i <= n; i++)
   {
       for (w = 0; w <= W; w++)
       {
           if (i==0 || w==0)
               B[i][w] = 0;
           else if (treasure[i-1].weight <= w)
                 B[i][w] = max(treasure[i-1].benefit + B[i-1][w-treasure[i-1].weight],  B[i-1][w]);
           else
                 B[i][w] = B[i-1][w];
       }
       if(i>1) {
           free(B[i-1]);
       }
       clock_t checkTime = clock();
       float checkT = (float)(checkTime - start)/CLOCKS_PER_SEC;
       if(checkT>900) {
           printf("The time is over 15 minutes\n");
           return;
       } //Check running time
   }
    DP_benefit = B[n][W];
    free(B[n]);
    free(B);
     end = clock();
     result = (float)(end - start)/CLOCKS_PER_SEC;
     result*=1000;
     DP_time = result;
}

void Greedy (int W,struct item treasure[],int n) {
    clock_t start, end;
    float result;
    start = clock(); // start time
    float b = 0;
    for(int i = 0 ; i<n; i++){
        if(treasure[i].weight<=W){
            b+= treasure[i].benefit;
            W-= treasure[i].weight;
            if(W == 0) break;
        }
        else{
            b += treasure[i].value * W;
            break;
        }
        clock_t checkTime = clock();
        float checkT = (float)(checkTime - start)/CLOCKS_PER_SEC;
        if(checkT>900) {
            printf("The time is over 15 minutes\n");
            return;
        } //Check running time
    }
    greedy_benefit = b;
    end = clock();
    result = (float)(end - start)/CLOCKS_PER_SEC;
    result*=1000;
    greedy_time = result;
}//greedy fractional knapsack problem

int compare(const void *A, const void  *B)
{
    struct item* ptrA = (struct item*)A;
    struct item* ptrB = (struct item*)B;
    if (ptrA->value > ptrB->value) return -1;
    else if (ptrA->value == ptrB->value) return 0;
    else return 1;
}


void MakeTreasure (int n, struct item treasure[]) {
    for(int i = 0; i<n; i++){
        treasure[i].benefit = rand()%300+1;
        treasure[i].weight = rand()%100+1;
        treasure[i].value =  (float)treasure[i].benefit/treasure[i].weight;
    }
} //make treasure weight and benefit

int max(int a, int b) { return (a > b)? a : b; }

void heap_init() {
    size = 0;
}

void h_swap(Node *a, Node *b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

int push(Node value) {
    if (size + 1 > MAX_SIZE) {
        return 0;
    }
    heap[size] = value;
    int child = size;
    int parent = (size - 1) / 2;
    while (child > 0 && heap[child].bound > heap[parent].bound) {
        h_swap(&heap[child], &heap[parent]);
        child = parent;
        parent = (parent - 1) / 2;
    }
    size++;
    return 1;
}

Node pop() {
    if (size <= 0) {
        Node temp;
        temp.benefit = -1;
        return temp;
    }
    Node hp = heap[0];
    size-=1;
    heap[0] = heap[size];
    int c_Node = 0;
    int leftChild = c_Node * 2 + 1;
    int rightChild = c_Node * 2 + 2;
    int maxNode = c_Node;
    while (leftChild < size) {
        if (heap[maxNode].bound < heap[leftChild].bound)
            maxNode = leftChild;
        if (rightChild < size && heap[maxNode].bound < heap[rightChild].bound)
            maxNode = rightChild;

        if (maxNode == c_Node)
            break;
        else {
            h_swap(&heap[c_Node], &heap[maxNode]);
            c_Node = maxNode;
            leftChild = c_Node * 2 + 1;
            rightChild = c_Node * 2 + 2;
        }
    }

    return  hp;
}

void printTableName(FILE* fp){
    fprintf(fp, "number of items | processing time in miliseconds /maximum benefit value \n");
    fprintf(fp,"________________________________________________________________________________________________________\n");
    fprintf(fp, "                |       greedy             |             D.P             |            B&B      \n");
    fprintf(fp,"________________________________________________________________________________________________________\n");
}

void soultionPrint(float BB_value, FILE* fp,int n){
    fprintf(fp,"  n = %6d\t|%7.3f ms/  %12.3f |%12.3f ms/%12d |  %12.3f ms/%12.f \n",n,greedy_time,greedy_benefit,DP_time,DP_benefit,BB_time,BB_value);
    fprintf(fp,"________________________________________________________________________________________________________\n");
}
