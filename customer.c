#include "common.h"
void display_inventory() {
    FILE *file = fopen("inventory.txt", "r");
    if (!file) {
        printf("Unable to load inventory!\n");
        return;
    }

    printf("\n🛒 Available Items in Shop:\n");
    char item[50];
    int qty;
    float price;
    
    printf("----------------------------\n");
    printf("| %-15s | %-6s | %-6s |\n", "Item", "Stock", "Price");
    printf("----------------------------\n");
    
    while (fscanf(file, "%s %d %f", item, &qty, &price) == 3) {
        printf("| %-15s | %-6d | $%-5.2f |\n", item, qty, price);
    }
    printf("----------------------------\n");
    fclose(file);
}
int send_order(mqd_t mq, mqd_t mq_response) {
    mess msg;
    msg.type = ORDER_MSG;
    int status=1 ; 
    
    while (1) {
        display_inventory();
        printf("\n🛒 Enter item name (or type 'exit' to quit): ");
        scanf("%s", msg.item);
        if (strcmp(msg.item, "exit") == 0)
        {
            status = 0 ;
            break;
        }       
        printf("📦 Enter quantity: ");
        scanf("%d", &msg.quantity);
        
        mq_send(mq, (char *)&msg, sizeof(mess), 1);

        // Wait for response
        mess response;
        mq_receive(mq_response, (char *)&response, sizeof(mess), NULL);
        
        if (response.type == 1)
            printf("✅ Order Approved: %d x %s\n", response.quantity, response.item);
        else if (response.type == 0)
        {
            printf("❌ Order Rejected: %s (Not enough stock)\n", response.item);
        }
        // else if(response.type == 2)
        // {
        //     printf("Update in Inventory Prices and Items!");
        //     display_inventory();
        // }
    }
}


int main() {
    mqd_t mq = open_queue(O_WRONLY);  // Send orders
    mqd_t mq_response = open_queue(O_CREAT | O_RDONLY);  // Receive responses
    int status = 1  ; 
    while(status){
        // display_inventory();
        status = send_order(mq, mq_response);
    }
    mq_unlink("/customer_queue");
}
