#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 5
#define MAX_MESSAGE 64
char buffer[BUFFER_SIZE][MAX_MESSAGE];
int in = 0, out = 0, count = 0;
pthread_mutex_t mutex; //mutex 객체 선언
sem_t empty, full; //세마포어 생성
int repeat = 1;
// Producer and Consumer threads terminates
// when main() sets repeat to zero.
//무의미한 값 or 값 x -> default = 30sec

void DisplayBuffer();
void *Producer ();
void *Consumer ();

int main(int argc, const char * argv[]) {
    int duration, check_value, status;
    pthread_t p_thread[2];
    if(argc>1){
        duration = atoi(argv[1]);
        //Read duration from the command line arguments.
    }
    if(duration == 0) duration = 30;
    //Initialize
    srand(time(NULL));
    sem_init(&empty,0,BUFFER_SIZE);
    sem_init(&full,0,0);
    pthread_mutex_init(&mutex, NULL);

    DisplayBuffer();//Display the initial content of buffer.
    //Create threads for producer and consumer.
    pthread_create(&p_thread[0], NULL, Producer , NULL);

    pthread_create(&p_thread[1], NULL, Consumer , NULL);
    sleep(duration); //Wait for duration seconds.
    repeat = 0; //Set repeat to zero to terminate producer and Consumer.
    sem_getvalue(&empty, &check_value);
    if(check_value == 0){
        sem_post(&empty);
    }
    sem_getvalue(&full, &check_value);
    if(check_value == 0){
        sem_post(&full);
    }
    //Wait for the producer and consumer.
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
    //Destroy mutex, full, and empty.
    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);
    //Print a good-bye message.
    printf("Bye!\n");
    

}

void DisplayBuffer() {
    printf("Buffer contents:\n");
    printf("\tcount = %d, in = %d, out = %d\n",count,in,out);
    int p = out;
    for(int i = 0; i < count; i++){
        printf("\t%d] %s\n", p, buffer[p]);
        p = (p + 1) % BUFFER_SIZE;
    }
}

void *Producer (){
   do{
        sleep(rand() % 3 + 1);
        int no_messages = 10;
        char *messages[64] = {
        "Nice to see you!",
        "Aal izz well!",
        "I love you! God loves you!",
        "God loves you and has a wonderful plan for your life.",
        "God bless you!",
        "You are genius!",
        "Cheer up!",
        "Everything is gonna be okay.",
        "You are so precious!",
        "Hakuna matata!"
        };
        char nextp[64];
        strcpy(nextp, messages[rand() % no_messages]);
        printf("[Producer] Created a message \"%s\"\n",nextp);
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        printf("---------------------- PRODUCER ------------------------\n");
        printf("Producer is entering critical section.\n");
       
        strcpy(buffer[in], nextp);
        printf("[Producer] \"%s\" ==> buffer\n",buffer[in]);
        in = (in+1)%BUFFER_SIZE;
        count++;
        DisplayBuffer();
        printf("Producer is leaving critical section.\n");
        printf("--------------------------------------------------------\n");
        //add nextp to buffer

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
   } while(repeat == 1);
}

void *Consumer (){
    do{
        char text[64];
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        printf("---------------------- CONSUMER ------------------------\n");
        printf("Consumer is entering critical section.\n");
        strcpy(text, buffer[out]);
        printf("[Consumer] buffer ==> \"%s\"\n",text);
        out = (out+1)% BUFFER_SIZE;
        count--;
        //bufer prinf
        DisplayBuffer();
        printf("Consumer is leaving critical section.\n");
        printf("--------------------------------------------------------\n");
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        printf("[Consumer] Consumed a message \"%s\"\n",text);
        sleep(rand() % 3 + 2);
    }while(repeat == 1);
}
