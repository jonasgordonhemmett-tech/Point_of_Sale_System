#include <string>
#include "Item.h"
#include <iostream>
using namespace std;
#ifndef ITEMIN_H
#define ITEMIN_H

/*
  Used as the base for items in a transaction.
  The methods and variables here are just per transaction, E.G. If you subtract 1 banana the transaction has 1 less banana.
 */
struct ItemIn
{
    Item* item;
    int purchasePrice = 0;
    int purchaseQuantity = 0;

    ItemIn(Item* item) : item(item) {}

    // Prints object info
    friend std::ostream& operator<<(std::ostream& os, const ItemIn& obj) {
        os << obj.item->getName() << "(" << obj.item->getId() << ") " << obj.purchaseQuantity << "x $" << obj.purchasePrice;
        return os;
    }

    // Checks if two objects share the same ID.
    bool operator==(const ItemIn& other) const {
        return (item->getId() == other.item->getId());
    }

};
#endif //ITEMIN_H
