#include <cassert>
#include <iostream>
#include "food_menu_management.h"
#include "token_lookup.h"
using namespace std;

int main() {
    FoodMenu menu;
    assert(menu.findFood(1) != nullptr);
    assert(menu.isAvailable(1, 1));
    assert(menu.changeStock(1, -1));
    assert(!menu.isAvailable(1, 100000));
    int id = menu.addFood("Test Tea", 15.0, 3);
    assert(id > 0);
    assert(menu.findFood(id)->name == "Test Tea");
    assert(menu.changeStock(id, -2));
    assert(menu.findFood(id)->stock == 1);
    assert(menu.changeStock(id, 2));
    assert(menu.findFood(id)->stock == 3);
    assert(menu.deleteFood(id));
    assert(menu.findFood(id) == nullptr);

    TokenTable table;
    Order order;
    order.token = 1234;
    order.customerName = "Test";
    order.total = 25.0;
    order.status = PENDING;
    order.items.push_back({1, "Tea", 25.0, 1});
    table.insertOrUpdate(order);
    assert(table.size() == 1);
    assert(table.find(1234) != nullptr);
    assert(table.find(1234)->customerName == "Test");
    order.status = COMPLETED;
    table.insertOrUpdate(order);
    assert(table.find(1234)->status == COMPLETED);
    assert(table.remove(1234));
    assert(table.find(1234) == nullptr);
    cout << "All basic DSA module tests passed.\n";
    return 0;
}
