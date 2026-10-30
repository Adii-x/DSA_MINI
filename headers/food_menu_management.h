#ifndef FOOD_MENU_MANAGEMENT_H
#define FOOD_MENU_MANAGEMENT_H

#include "models.h"

class FoodMenu {
private:
    struct Node {
        FoodItem item;
        Node* next;
        Node(const FoodItem& value) : item(value), next(nullptr) {}
    };

    Node* head;
    int nextFoodId;

public:
    FoodMenu();
    ~FoodMenu();
    FoodMenu(const FoodMenu&) = delete;
    FoodMenu& operator=(const FoodMenu&) = delete;

    int addFood(const string& name, double price, int stock);
    bool updateFood(int id, const string& name, double price, int stock);
    bool deleteFood(int id);
    FoodItem* findFood(int id);
    const FoodItem* findFood(int id) const;
    void displayMenu() const;
    bool changeStock(int id, int delta);
    bool isAvailable(int id, int quantity) const;
};

#endif
