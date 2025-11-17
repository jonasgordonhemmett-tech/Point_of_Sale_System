
#include "Item.h"
#include <string>
#include <iostream>
#include <iomanip>

#include "BaseInterface.h"
using namespace std;

Item::Item(int id, string name) : id(id), name(name), price(0), quantity(0),holdQuantity(0) {}
Item::Item(int id, string name, int price) : id(id), name(name), price(price), quantity(0), holdQuantity(0) {}
Item::Item(int id, string name, int price, int quantity) : id(id), name(name), price(price), quantity(quantity), holdQuantity(0) {}
Item::Item(string name, int price, int quantity) : name(name), price(price), quantity(quantity), holdQuantity(0) {}

string Item::getName() const {return name;}
void Item::setName(string name) {this->name = name;}

int Item::getId() const {return id;}
void Item::setId(int id) {
  if (this->id == -2) {
    this->id =id;
  }
}

int Item::getPrice() const {return price;}
void Item::setPrice(int price) {this->price = price;}

int Item::getQuantity() const {return quantity;}
void Item::setQuantity(int quantity) {this->quantity = quantity;}
bool Item::changeQuantity(int quantity){
  if (quantity >= 0)
  {
    this->quantity += quantity;
    return true;
  }

  if (quantity <= holdQuantity) {
    this->quantity += quantity;
    holdQuantity += quantity;
    return true;
  }

  return false;
}

int Item::getHoldQuantity() const {return holdQuantity;}
void Item::setHoldQuantity(int holdQuantity) {this->holdQuantity = holdQuantity;}
bool Item::changeHoldQuantity(int holdQuantity) {

  if (holdQuantity >= 0) {
    if (holdQuantity <= getAvailableQuantity()) {
      this->holdQuantity += holdQuantity;
      return true;
    }
    return false;
  }

  if (holdQuantity <= this->holdQuantity) {
    this->holdQuantity += holdQuantity;
    return true;
  }

  return false;
}

int Item::getAvailableQuantity() const{return quantity - holdQuantity;}

string Item::priceFormat(int price) {
  int dollars = price / 100;

  // Handles negative prices, if need be
  int cents = abs(price % 100);

  string fill = (cents < 10) ? "0" : "";

  return "$" + to_string(dollars) + "." +  fill + to_string(cents);
}

bool Item::operator==(const Item& other) const {
  return (id == other.id);
}
string Item::toString() const{
  ostringstream oss;
  oss << *this;
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Item& obj){
  os << obj.name << "(" << obj.id << ") " << obj.quantity - obj.holdQuantity << "x" << obj.priceFormat(obj.price);
  return os;
}

void to_json(nlohmann::json& j, const Item& t){
  j = nlohmann::json{
    // https://json.nlohmann.me/api/basic_json/array/
{"id", t.getId()},
{"name", t.getName()},
{"price", t.getPrice()},
{"quantity", t.getQuantity()},
        };

}

