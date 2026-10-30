#ifndef TOKEN_LOOKUP_H
#define TOKEN_LOOKUP_H

#include "models.h"

class TokenTable {
private:
    struct Entry {
        int key;
        Order value;
        bool occupied;
        bool deleted;
        Entry() : key(-1), occupied(false), deleted(false) {}
    };

    vector<Entry> table;
    int count;
    int hashFunction(int token) const;
    void rehash();

public:
    explicit TokenTable(int capacity = 101);
    void insertOrUpdate(const Order& order);
    Order* find(int token);
    const Order* find(int token) const;
    bool remove(int token);
    vector<Order> getAll() const;
    int size() const;
};

#endif
