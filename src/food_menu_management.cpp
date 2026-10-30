#include <iostream>
#include <iomanip>
#include "food_menu_management.h"
using namespace std;

FoodMenu::FoodMenu() : head(nullptr), nextFoodId(1) {
    addFood("Veg Sandwich", 60.00, 20);
    addFood("Cheese Burger", 90.00, 15);
    addFood("Masala Dosa", 80.00, 12);
    addFood("Cold Coffee", 50.00, 18);
    addFood("Paneer Roll", 75.00, 10);
}

FoodMenu::~FoodMenu() {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int FoodMenu::addFood(const string& name, double price, int stock) {
    if (name.empty() || price <= 0 || stock < 0) return -1;
    FoodItem item{nextFoodId++, name, price, stock, stock > 0};
    Node* node = new Node(item);
    if (head == nullptr) head = node;
    else {
        Node* current = head;
        while (current->next != nullptr) current = current->next;
        current->next = node;
    }
    return item.id;
}

bool FoodMenu::updateFood(int id, const string& name, double price, int stock) {
    FoodItem* item = findFood(id);
    if (!item || name.empty() || price <= 0 || stock < 0) return false;
    item->name = name;
    item->price = price;
    item->stock = stock;
    item->available = stock > 0;
    return true;
}

bool FoodMenu::deleteFood(int id) {
    Node* current = head;
    Node* previous = nullptr;
    while (current != nullptr && current->item.id != id) {
        previous = current;
        current = current->next;
    }
    if (!current) return false;
    if (!previous) head = current->next;
    else previous->next = current->next;
    delete current;
    return true;
}

FoodItem* FoodMenu::findFood(int id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->item.id == id) return &current->item;
        current = current->next;
    }
    return nullptr;
}

const FoodItem* FoodMenu::findFood(int id) const {
    Node* current = head;
    while (current != nullptr) {
        if (current->item.id == id) return &current->item;
        current = current->next;
    }
    return nullptr;
}

void FoodMenu::displayMenu() const {
    cout << "\n" << setfill('=') << setw(76) << "" << setfill(' ') << "\n";
    cout << left << setw(6) << "ID" << setw(25) << "Food Item"
         << right << setw(12) << "Price (Rs)" << setw(12) << "Stock"
         << setw(15) << "Availability" << "\n";
    cout << setfill('-') << setw(76) << "" << setfill(' ') << "\n";
    Node* current = head;
    while (current != nullptr) {
        cout << left << setw(6) << current->item.id
             << setw(25) << current->item.name
             << right << setw(12) << fixed << setprecision(2) << current->item.price
             << setw(12) << current->item.stock
             << setw(15) << (current->item.stock > 0 ? "Available" : "Sold out") << "\n";
        current = current->next;
    }
    cout << setfill('=') << setw(76) << "" << setfill(' ') << "\n";
}

bool FoodMenu::changeStock(int id, int delta) {
    FoodItem* item = findFood(id);
    if (!item || item->stock + delta < 0) return false;
    item->stock += delta;
    item->available = item->stock > 0;
    return true;
}

bool FoodMenu::isAvailable(int id, int quantity) const {
    const FoodItem* item = findFood(id);
    return item && quantity > 0 && item->stock >= quantity;
}
