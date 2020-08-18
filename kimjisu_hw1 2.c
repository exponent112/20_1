/* This assignment was conducted on repl.it 
*/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Member { 
    char name[20];       
    int key;
};

void swap_key(int *m1, int *m2);
void swap_name (char **str1, char **str2);
void print_function();
void max_heapify(int i);
void change_pc(int n);
void I_function();
void D_function();
void P_function();
void R_function();
void K_function();

struct Member m[32];
int number = 0;

int main(void) {
  int flag = 0;
  while(flag == 0){
    print_function();
    char input[3];
    scanf("%s",&input);
    getchar();
    if(strlen(input)!= 1){
       printf("Wrong Input!\n");
    }
    else{
      switch(input[0]){
        case 'I': I_function(); break;
        case 'D': D_function(); break;
        case 'R': R_function(); break;
        case 'K': K_function(); break;
        case 'P': P_function(); break;
        case 'Q': printf("Thank you. Bye!\n"); flag = 1; break;
        default:  printf("Wrong Input!\n");
      }
    }
  }
  return 0;
}

void I_function(){
  if(number>=30){
    printf("too much element!\n");
    return;
  }
  number++;
  printf("Enter name of element: ");
  scanf("%s",&m[number].name);
  getchar();
  printf("Enter key value of element: ");
  scanf("%d",&m[number].key);
  getchar();
  printf("New element [%s, %d] is inserted.\n",m[number].name,m[number].key);
  change_pc(number);
}

void D_function(){
  if(number<1){
    printf("heap underflow\n");
    return;
  }
  swap_key(&m[1].key, &m[number].key);
  swap_name(&m[1].name, &m[number].name);
  number--;
  max_heapify(1);
  printf("[%s, %d] is deleted.\n",m[number+1].name,m[number+1].key);
}

void K_function(){
  int index;
  int change_value;
  printf("Enter index of element: ");
  scanf("%d",&index);
  printf("Enter new key value: ");
  scanf("%d",&change_value);
  if(change_value<m[index].key)
    {
      printf("new key is smaller than current key\n");
      return;
    }
    else if( m[index].key == change_value) return;
    else{
       m[index].key = change_value;
      change_pc(index);
    }
}

void P_function(){
  int temp = 1;
  while(temp <= number){
    printf("[%s, %d]\t",m[temp].name,m[temp].key);
    temp++;
  }
  printf("\n");
}

void R_function(){
  if(number<1){
    printf("There is no element\n");
    return;
  }
   printf("[%s, %d]\n",m[1].name,m[1].key);
}

void print_function(){
    printf("*********** MENU ****************\n");
    printf("I : Insert new element into queue\n");
    printf("D : Delete element with largest key from queue\n");
    printf("R : Retrieve new element into queue\n");
    printf("K : Increase key of element in queue\n");
    printf("P : Print out all elements in queue\n");
    printf("Q : Quit\n\n");
    printf("Choose menu: ");
}

void max_heapify(int i){
  int left = i*2;
  int right = i*2+1;
  int largest;
  if(left <= number && m[left].key> m[i].key){
    largest = left;
  }
  else largest = i;
  if(right <= number && m[right].key> m[largest].key){
    largest = right;
  }
  if(largest != i) {
   	swap_key(&m[i].key, &m[largest].key);
    swap_name(&m[i].name, &m[largest].name);
    max_heapify(largest);
  }
}

void change_pc(int n){
  int child = n;
	int parent = child / 2;
	while (child > 1 && m[parent].key < m[child].key) {
    swap_key(&m[parent].key, &m[child].key);
    swap_name(&m[parent].name, &m[child].name);
		child = parent;
		parent = child / 2;
	}
}

void swap_key(int *m1, int *m2){
  int temp = (*m2);
  (*m2) = (*m1);
  (*m1) = temp;
}

void swap_name (char **str1_ptr, char **str2_ptr) 
{ 
  char *temp = *str1_ptr; 
  *str1_ptr = *str2_ptr; 
  *str2_ptr = temp; 
}