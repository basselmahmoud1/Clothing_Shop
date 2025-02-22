#include "common.h"

#define LOW_STOCK_THRESHOLD 5  
#define AUTO_RESTOCK_AMOUNT 20  

typedef struct {
    char item[50];
    int quantity;
    float price;
} InventoryItem;

InventoryItem inventory[100];
int inventory_size = 0;

// Load inventory from file
void load_inventory() {
    FILE *file = fopen("inventory.txt", "r");
    if (!file) return;
    inventory_size = 0;
    while (fscanf(file, "%s %d %f", inventory[inventory_size].item, 
                  &inventory[inventory_size].quantity, 
                  &inventory[inventory_size].price) == 3) {
        inventory_size++;
    }
    fclose(file);
}

// Save inventory to file
void save_inventory() {
    FILE *file = fopen("inventory.txt", "w");
    if (!file) return;
    for (int i = 0; i < inventory_size; i++)
        fprintf(file, "%s %d %.2f\n", inventory[i].item, inventory[i].quantity, inventory[i].price);
    fclose(file);
}
char *to_lowercase(const char *str) {
    if (!str) return NULL; // Handle NULL input
    
    size_t len = strlen(str);
    char *lower_str = malloc(len + 1); // Allocate memory for new string
    if (!lower_str) return NULL; // Handle memory allocation failure

    for (size_t i = 0; i < len; i++) {
        lower_str[i] = tolower(str[i]); // Convert each character to lowercase
    }
    lower_str[len] = '\0'; // Null-terminate the string

    return lower_str; // Return the new lowercase string
}

// Find an item in inventory
int find_item(const char *name) {
    for (int i = 0; i < inventory_size; i++)
        if (strcmp(to_lowercase(inventory[i].item),to_lowercase(name)) == 0)
            return i;
    return -1;
}

// Log orders
void log_order(mess *msg, int status) {
    FILE *file = fopen("orders.log", "a+");
    if (!file) return;
    fprintf(file, "%s | Ordered: %d | %s\n", msg->item, msg->quantity, status ? "✅ Approved" : "❌ Rejected");
    fclose(file);
}
void initialize_order_log() {
    FILE *file = fopen("orders.log", "w");  // "w" clears the file if it exists
    if (!file) {
        perror("Error initializing order log");
        exit(1);
    }
    fclose(file);
}

// Process order
void process_order(mess *msg, mqd_t mq_response) {
    int index = find_item(msg->item);
    mess response;
    strcpy(response.item, msg->item);
    response.quantity = msg->quantity;
    
    if (index == -1 || inventory[index].quantity < msg->quantity) {
        printf("❌ Order Rejected: %s\n", msg->item);
        log_order(msg, 0);
        response.type = 0;  // 0 means rejected
    } else {
        inventory[index].quantity -= msg->quantity;
        printf("✅ Order Processed: %d x %s\n", msg->quantity, msg->item);
        response.type = 1;  // 1 means approved
        log_order(msg, 1);
        // Auto-restock if stock is low
        if (inventory[index].quantity < LOW_STOCK_THRESHOLD) {
            printf("🔴 Low stock warning! Auto-restocking\n");
            inventory[index].quantity += AUTO_RESTOCK_AMOUNT;
        }
        save_inventory();
    }

    // Send response to customer queue
    mq_send(mq_response, (char *)&response, sizeof(mess), 0);
}

// Process inventory update
void process_inventory_update(mess *msg) {
    int index = find_item(msg->item);
    if (index == -1) {
        strcpy(inventory[inventory_size].item, msg->item);
        inventory[inventory_size].quantity += msg->quantity;
        inventory[inventory_size].price += msg->price;
        inventory_size++;
    } else {
        inventory[index].quantity = msg->quantity;
        inventory[index].price = msg->price;
    }
    save_inventory();
}

// Main processing loop
int main() {
    initialize_order_log();
    load_inventory();

    mqd_t mq = open_queue(O_CREAT | O_RDONLY); // Shop queue (receives orders)
    mqd_t mq_response = open_queue(O_CREAT | O_WRONLY); // Response queue (to customers)
    
    mess msg;
    

    printf("🏪 Shop is open!\n");

    while (1) {
        ssize_t check = mq_receive(mq, (char *)&msg, sizeof(mess),NULL);
        if ( check > 0) {
            if (msg.type == ORDER_MSG) process_order(&msg, mq_response);
            else if (msg.type == RESTOCK_MSG) process_inventory_update(&msg);
        }
    }
    
    mq_unlink("/customer_queue");
    mq_unlink("/shop_queue");
}
