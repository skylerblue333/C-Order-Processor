#include <stdio.h>
#include <string.h>

#define MAX_ORDERS 256

typedef enum { BUY, SELL } OrderSide;
typedef enum { PENDING, FILLED, CANCELLED } OrderStatus;

typedef struct {
    int id;
    OrderSide side;
    double price;
    int quantity;
    OrderStatus status;
} Order;

Order order_book[MAX_ORDERS];
int order_count = 0;

int submit_order(OrderSide side, double price, int qty) {
    if (order_count >= MAX_ORDERS) return -1;
    int id = order_count + 1;
    order_book[order_count++] = (Order){id, side, price, qty, PENDING};
    printf("[SUBMIT] Order #%d: %s %.2f x %d\n",
           id, side == BUY ? "BUY" : "SELL", price, qty);
    return id;
}

void match_orders() {
    printf("\n[MATCHING ENGINE]\n");
    for (int i = 0; i < order_count; i++) {
        if (order_book[i].status != PENDING) continue;
        for (int j = i + 1; j < order_count; j++) {
            if (order_book[j].status != PENDING) continue;
            if (order_book[i].side != order_book[j].side &&
                order_book[i].price >= order_book[j].price) {
                order_book[i].status = FILLED;
                order_book[j].status = FILLED;
                printf("[FILL]   Order #%d matched with Order #%d @ %.2f\n",
                       order_book[i].id, order_book[j].id, order_book[j].price);
                break;
            }
        }
    }
}

void print_book() {
    printf("\n[ORDER BOOK STATUS]\n");
    const char *statuses[] = {"PENDING", "FILLED", "CANCELLED"};
    for (int i = 0; i < order_count; i++) {
        printf("  #%d %s %.2f x%d [%s]\n",
               order_book[i].id,
               order_book[i].side == BUY ? "BUY " : "SELL",
               order_book[i].price,
               order_book[i].quantity,
               statuses[order_book[i].status]);
    }
}

int main() {
    printf("=== Order Processing Engine ===\n\n");
    submit_order(BUY,  100.50, 10);
    submit_order(SELL, 100.50, 10);
    submit_order(BUY,   99.00,  5);
    submit_order(SELL, 101.00,  5);
    match_orders();
    print_book();
    return 0;
}
