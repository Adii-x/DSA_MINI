#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>
using namespace std;

struct FoodItem {
    int id;
    string name;
    double price;
    int stock;
    bool available;
};

struct OrderedItem {
    int foodId;
    string name;
    double price;
    int quantity;
};

enum OrderStatus {
    PENDING,
    PROCESSING,
    COMPLETED,
    CANCELLED
};

struct Order {
    int token;
    string customerName;
    vector<OrderedItem> items;
    double total;
    OrderStatus status;
};

inline string statusToString(OrderStatus status) {
    switch (status) {
        case PENDING: return "Pending";
        case PROCESSING: return "Processing";
        case COMPLETED: return "Completed";
        case CANCELLED: return "Cancelled";
    }
    return "Unknown";
}

#endif
