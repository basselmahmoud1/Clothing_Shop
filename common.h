#include <mqueue.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

// Message types
#define ORDER_MSG 1
#define RESTOCK_MSG 2


// Message Structure
typedef struct {
    int type;            
    char item[50];       
    int quantity;        
    float price;         
    int response;        
} mess;

// Open a message queue
mqd_t open_queue(int flags) {
    struct mq_attr attr = {0, 10, sizeof(mess), 0};
    mqd_t mq = mq_open("/shop_queue", flags, 0666, &attr);
    if (mq == -1) {
        perror("mq_open failed");
        exit(1);
    }
    return mq;
}
