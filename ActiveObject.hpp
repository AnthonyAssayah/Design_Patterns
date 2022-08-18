#include <iostream>
#include<stdio.h>
#include <pthread.h>
#include "queue.hpp"

struct message {

    // int fd;
    char text[1024];
    int fd;
};

/************* message struct *******************/
// typedef struct message {

//     int fd;
//     char text[4096];
//     int level;

// }mess, *mess_ptr;


/************* Active Object struct *************/
typedef struct ActiveObject {

    queue *queue;
    pthread_t my_thread;
    void*(*first_func)(void*);
    void*(*second_func)(void*);

} AO,*AO_ptr;

/********* AO functions (question 2) *************/
AO_ptr newAO(struct Queue *q, void* (*f_func)(void*), void* (*q_func)(void*));
void *thread_func(void *my_AO);
void destroyAO(ActiveObject *my_AO);


/********* pipeline functions (question 3)********/ 
// void ceasar(void* message);
void* ceasar(void* message);
void* convert_letters(void* message);
void* to_client(nodeptr my_node);
// void pass_func( nodeptr my_node);
void* first_Enqueue(void* message);
void* second_Enqueue(void* message);
void* third_Enqueue(void* message);


//        mess_ptr conver_mess = (mess_ptr)i;

//     printf("the string is %s\n", conver_mess);
//     printf("the first char is = %c\n",conver_mess->text[0]);
//     int j = 0;
//     while (j < strlen(conver_mess->text)) {
//         printf("the char #%d is = %c\n", j, conver_mess->text[j]);

//         if (conver_mess->text[j] >= 65 && conver_mess->text[j] <= 90) {
//             conver_mess->text[j]  = (conver_mess->text[j]  - 65) + 97;
//         }
//         else if (conver_mess->text[j] >= 97 && conver_mess->text[j]  <= 122) {
//             conver_mess->text[j]  = (conver_mess->text[j]  - 97) + 65;
//         } 
       
//         j++;  
//     }
//    conver_mess->level = 2;