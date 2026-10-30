#include <iostream>
#include <iomanip>
#include <limits>
#include "order_processing.h"
using namespace std;

CanteenSystem::CanteenSystem() : nextToken(1001) {}

void CanteenSystem::displayMenu() const {
    menu.displayMenu();
}

void CanteenSystem::addFood() {
    string name;
    double price;
    int stock;

    cout << "Food name: ";
    getline(cin, name);

    cout << "Price (Rs): ";
    if (!(cin >> price)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid price.\n";
        return;
    }

    cout << "Available quantity: ";
    if (!(cin >> stock)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid quantity.\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int id = menu.addFood(name, price, stock);
    if (id == -1) cout << "Food could not be added. Check the details.\n";
    else cout << "Food added successfully. Food ID: " << id << "\n";
}

void CanteenSystem::placeOrder() {
    string customer;
    int count;

    cout << "Customer name: ";
    getline(cin, customer);
    if (customer.empty()) {
        cout << "Customer name cannot be empty.\n";
        return;
    }

    cout << "How many different food items? ";
    if (!(cin >> count) || count < 1 || count > 20) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter a number from 1 to 20.\n";
        return;
    }

    Order order;
    order.token = nextToken;
    order.customerName = customer;
    order.total = 0;
    order.status = PENDING;

    // Validate all requested items before changing stock.
    vector<int> foodIds;
    vector<int> quantities;

    for (int i = 0; i < count; ++i) {
        int id, quantity;
        cout << "Food ID for item " << i + 1 << ": ";
        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid food ID. Order was not placed.\n";
            return;
        }

        cout << "Quantity: ";
        if (!(cin >> quantity) || quantity <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Quantity must be greater than zero.\n";
            return;
        }

        if (!menu.isAvailable(id, quantity)) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Food item not found or stock is insufficient.\n";
            return;
        }

        foodIds.push_back(id);
        quantities.push_back(quantity);
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Check duplicate food IDs together so stock cannot go below zero.
    for (size_t i = 0; i < foodIds.size(); ++i) {
        int totalQuantity = quantities[i];
        for (size_t j = i + 1; j < foodIds.size(); ++j) {
            if (foodIds[j] == foodIds[i]) totalQuantity += quantities[j];
        }
        if (!menu.isAvailable(foodIds[i], totalQuantity)) {
            cout << "Combined quantity exceeds available stock. Order not placed.\n";
            return;
        }
    }

    for (size_t i = 0; i < foodIds.size(); ++i) {
        FoodItem* food = menu.findFood(foodIds[i]);
        order.items.push_back({food->id, food->name, food->price, quantities[i]});
        order.total += food->price * quantities[i];
        menu.changeStock(foodIds[i], -quantities[i]);
    }

    tokenTable.insertOrUpdate(order);
    orderQueue.push(order.token);
    ++nextToken;

    cout << "\nOrder placed successfully!\n";
    cout << "Your token number: " << order.token << "\n";
    cout << "Total amount: Rs " << fixed << setprecision(2) << order.total << "\n";
}

void CanteenSystem::showOrder(const Order& order) const {
    cout << "\nToken: " << order.token
         << " | Customer: " << order.customerName
         << " | Status: " << statusToString(order.status) << "\n";

    cout << left << setw(25) << "Food Item"
         << right << setw(8) << "Qty"
         << setw(13) << "Price"
         << setw(14) << "Subtotal" << "\n";
    cout << string(60, '-') << "\n";

    for (const OrderedItem& item : order.items) {
        cout << left << setw(25) << item.name
             << right << setw(8) << item.quantity
             << setw(13) << fixed << setprecision(2) << item.price
             << setw(14) << item.price * item.quantity << "\n";
    }

    cout << string(60, '-') << "\n";
    cout << "Total: Rs " << fixed << setprecision(2) << order.total << "\n";
}

void CanteenSystem::processNextOrder() {
    while (!orderQueue.empty()) {
        int token = orderQueue.front();
        orderQueue.pop();

        Order* order = tokenTable.find(token);
        if (order == nullptr || order->status == CANCELLED) continue;

        if (order->status == PENDING) {
            order->status = PROCESSING;
            cout << "\nProcessing next order (First Come, First Served)\n";
            showOrder(*order);
            order->status = COMPLETED;
            cout << "Order completed successfully.\n";
            return;
        }
    }
    cout << "There are no pending orders.\n";
}

void CanteenSystem::searchOrder() const {
    int token;
    cout << "Enter token number: ";
    if (!(cin >> token)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid token.\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    const Order* order = tokenTable.find(token);
    if (order == nullptr) cout << "No order found for this token.\n";
    else showOrder(*order);
}

void CanteenSystem::cancelOrder() {
    int token;
    cout << "Enter token number to cancel: ";
    if (!(cin >> token)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid token.\n";
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Order* order = tokenTable.find(token);
    if (order == nullptr) {
        cout << "Token not found.\n";
        return;
    }
    if (order->status != PENDING) {
        cout << "Only pending orders can be cancelled.\n";
        return;
    }

    order->status = CANCELLED;
    for (const OrderedItem& item : order->items) {
        menu.changeStock(item.foodId, item.quantity);
    }
    cancellationStack.push({order->token, order->customerName});
    cout << "Order cancelled. Food stock has been restored.\n";
}

void CanteenSystem::showAllOrders() const {
    vector<Order> orders = tokenTable.getAll();
    if (orders.empty()) {
        cout << "No orders have been placed yet.\n";
        return;
    }

    cout << "\n" << left << setw(10) << "Token"
         << setw(22) << "Customer"
         << setw(14) << "Total (Rs)"
         << "Status\n";
    cout << string(65, '-') << "\n";

    for (const Order& order : orders) {
        cout << left << setw(10) << order.token
             << setw(22) << order.customerName
             << setw(14) << fixed << setprecision(2) << order.total
             << statusToString(order.status) << "\n";
    }
}

void CanteenSystem::showCancellationHistory() const {
    if (cancellationStack.empty()) {
        cout << "No cancellations in this session.\n";
        return;
    }

    stack<CancellationRecord> copy = cancellationStack;
    cout << "\nRecent cancellations (latest first):\n";
    while (!copy.empty()) {
        CancellationRecord record = copy.top();
        copy.pop();
        cout << "Token " << record.token << " - " << record.customerName << "\n";
    }
}

void CanteenSystem::run() {
    int choice;
    do {
        cout << "\n";
        cout << setfill('=') << setw(60) << "" << setfill(' ') << "\n";
        cout << "          SMART CANTEEN PRE-ORDER SYSTEM\n";
        cout << setfill('=') << setw(60) << "" << setfill(' ') << "\n";
        cout << " 1. Display food menu\n";
        cout << " 2. Add food item\n";
        cout << " 3. Place food order\n";
        cout << " 4. Process next order (FIFO queue)\n";
        cout << " 5. Search order by token (hash table)\n";
        cout << " 6. Cancel pending order (stack history)\n";
        cout << " 7. Display all orders\n";
        cout << " 8. Show cancellation history\n";
        cout << " 0. Exit\n";
        cout << setfill('-') << setw(60) << "" << setfill(' ') << "\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: displayMenu(); break;
            case 2: addFood(); break;
            case 3: placeOrder(); break;
            case 4: processNextOrder(); break;
            case 5: searchOrder(); break;
            case 6: cancelOrder(); break;
            case 7: showAllOrders(); break;
            case 8: showCancellationHistory(); break;
            case 0: cout << "Thank you for using Smart Canteen.\n"; break;
            default: cout << "Invalid choice. Please select from the menu.\n";
        }
    } while (choice != 0);
}
