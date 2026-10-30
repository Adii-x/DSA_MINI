#ifndef ORDER_PROCESSING_H
#define ORDER_PROCESSING_H

#include <queue>
#include <stack>
#include "food_menu_management.h"
#include "token_lookup.h"
using namespace std;

struct CancellationRecord {
    int token;
    string customerName;
};

class CanteenSystem {
private:
    FoodMenu menu;
    queue<int> orderQueue;
    TokenTable tokenTable;
    stack<CancellationRecord> cancellationStack;
    int nextToken;

    void showOrder(const Order& order) const;

public:
    CanteenSystem();
    void displayMenu() const;
    void addFood();
    void placeOrder();
    void processNextOrder();
    void searchOrder() const;
    void cancelOrder();
    void showAllOrders() const;
    void showCancellationHistory() const;
    void run();
};

#endif
