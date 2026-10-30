#include "token_lookup.h"

TokenTable::TokenTable(int capacity) : table(capacity), count(0) {}

int TokenTable::hashFunction(int token) const {
    return token % static_cast<int>(table.size());
}

void TokenTable::rehash() {
    vector<Entry> oldTable = table;
    table.assign(oldTable.size() * 2 + 1, Entry());
    count = 0;
    for (const Entry& entry : oldTable) {
        if (entry.occupied && !entry.deleted) insertOrUpdate(entry.value);
    }
}

void TokenTable::insertOrUpdate(const Order& order) {
    if ((count + 1) * 10 >= static_cast<int>(table.size()) * 7) rehash();
    int index = hashFunction(order.token);
    int firstDeleted = -1;
    for (int checked = 0; checked < static_cast<int>(table.size()); ++checked) {
        Entry& entry = table[index];
        if (entry.occupied && !entry.deleted && entry.key == order.token) {
            entry.value = order;
            return;
        }
        if (entry.deleted && firstDeleted == -1) firstDeleted = index;
        if (!entry.occupied) {
            int target = firstDeleted == -1 ? index : firstDeleted;
            table[target].key = order.token;
            table[target].value = order;
            table[target].occupied = true;
            table[target].deleted = false;
            ++count;
            return;
        }
        index = (index + 1) % static_cast<int>(table.size());
    }
    rehash();
    insertOrUpdate(order);
}

Order* TokenTable::find(int token) {
    int index = hashFunction(token);
    for (int checked = 0; checked < static_cast<int>(table.size()); ++checked) {
        Entry& entry = table[index];
        if (!entry.occupied) return nullptr;
        if (!entry.deleted && entry.key == token) return &entry.value;
        index = (index + 1) % static_cast<int>(table.size());
    }
    return nullptr;
}

const Order* TokenTable::find(int token) const {
    int index = hashFunction(token);
    for (int checked = 0; checked < static_cast<int>(table.size()); ++checked) {
        const Entry& entry = table[index];
        if (!entry.occupied) return nullptr;
        if (!entry.deleted && entry.key == token) return &entry.value;
        index = (index + 1) % static_cast<int>(table.size());
    }
    return nullptr;
}

bool TokenTable::remove(int token) {
    int index = hashFunction(token);
    for (int checked = 0; checked < static_cast<int>(table.size()); ++checked) {
        Entry& entry = table[index];
        if (!entry.occupied) return false;
        if (!entry.deleted && entry.key == token) {
            entry.deleted = true;
            --count;
            return true;
        }
        index = (index + 1) % static_cast<int>(table.size());
    }
    return false;
}

vector<Order> TokenTable::getAll() const {
    vector<Order> result;
    for (const Entry& entry : table) {
        if (entry.occupied && !entry.deleted) result.push_back(entry.value);
    }
    return result;
}

int TokenTable::size() const { return count; }
