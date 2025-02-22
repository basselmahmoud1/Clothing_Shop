#include "common.h"

int main() {
    mqd_t mq = open_queue(O_WRONLY);
    mess msg;
    msg.type = RESTOCK_MSG;

    while (1) {
        printf("\n📦 Enter item name to update (or 'exit' to quit): ");
        scanf("%s", msg.item);
        if (strcmp(msg.item, "exit") == 0) break;

        printf("📈 Enter new quantity: ");
        scanf("%d", &msg.quantity);
        printf("💰 Enter new price: ");
        scanf("%f", &msg.price);

        mq_send(mq, (char*)&msg, sizeof(mess), 0);
        printf("✅ Inventory Updated!\n");
    }

    return 0;
}
