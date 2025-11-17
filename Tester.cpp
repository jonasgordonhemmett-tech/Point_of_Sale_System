#include <iostream>
#include <string>
#include <limits>
#include "Customer.h"
#include "Employee.h"
#include "Manager.h"
#include "Transaction.h"
#include "Item.h"
#include <list>
#include <algorithm>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include "ItemIn.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <fstream>
#include "include/json.hpp"
#include <algorithm>
#include <queue>
#include <optional>
#include "settings.h"
#include "Tester.h"

#include "base.cpp"

bool settingsTest();
bool itemTest();
bool paymentPortalTest();
bool transactionTest();
bool mainTest();


int main(){
  cout << "settingsTest(): " << settingsTest() << endl;
  cout << "itemTest():" << itemTest() << endl;
  cout << "paymentPortalTest()" << paymentPortalTest() << endl;
  // cout << "transactionTest()" << transactionTest() << endl;
  // cout << "mainTest(): " << mainTest() << endl;
  return 0;
}

bool settingsTest(){
  bool passed = true;
  // Wipes for testing.
  ofstream tF("data/test_files/transactions.json");
  ofstream sF("data/test_files/settings.json");
  ofstream cF("data/test_files/customers.json");
  tF.close();
  sF.close();
  cF.close();


  Settings settings("test_files", false);
  settings.setTransactionIndex(0);
  settings.setCustomerIndex(0);

  if (settings.getTransactionIndex()){
    cout << "getTransactionIndex() is not 0, but: " << settings.getTransactionIndex() << endl;
    passed = false;
  }

  if (settings.getCustomerIndex()){
    cout << "getCustomerIndex() is not 0, but: " << settings.getCustomerIndex() << endl;
    passed = false;
  }

  if (settings.getMaxNumTransactions() != 2){
    cout << "getMaxNumTransactions() is not 2, but: " << settings.getMaxNumTransactions() << endl;
    passed = false;
  }

  if (settings.getMaxNumCustomers() != 2){
    cout << "getMaxNumCustomers() is not 2, but: " << settings.getMaxNumCustomers() << endl;
    passed = false;
  }

  return passed;
}

bool itemTest(){
  bool passed = true;

  Item item(0, "name", 10, 10);


  // Default
  if (item.getId() != 0){
    passed = false;
    cout<<"1: Item getId() is not 0, but: "<<item.getId()<<endl;
  }

  if (item.getName() != "name"){
    passed = false;
    cout<<"2: Item getName() is not name, but: "<<item.getName()<<endl;
  }

  if (item.getQuantity() != 10){
    passed = false;
    cout<<"3: Item getQuantity() is not 10, but: "<<item.getQuantity()<<endl;
  }

  if (item.getHoldQuantity() != 0){
    passed = false;
    cout<<"4: Item getHoldQuantity() is not 0, but: "<<item.getHoldQuantity()<<endl;
  }

  if (item.getPrice() != 10){
    passed = false;
    cout<<"5: Item getPrice() is not 10, but: "<<item.getPrice()<<endl;
  }

  if (item.getAvailableQuantity() != 10){
    passed = false;
    cout<<"6: Item getAvailableQuantity() is not 10, but: "<<item.getAvailableQuantity()<<endl;
  }

  // Setters


  item.setName("name2");
  if (item.getName() != "name2"){
    passed = false;
    cout<<"Item getName(), after setName(), is not name2, but: "<<item.getName()<<endl;
  }

  item.setQuantity(11);
  if (item.getQuantity() != 11){
    passed = false;
    cout<<"8: Item getQuantity(), after setQuantity, is not 11, but: "<<item.getQuantity()<<endl;
  }

  item.setHoldQuantity(9);
  if (item.getHoldQuantity() != 9){
    passed = false;
    cout<<"9: Item getHoldQuantity(), after setId(), is not 9, but: "<<item.getHoldQuantity()<<endl;
  }

  item.setPrice(11);
  if (item.getPrice() != 11){
    passed = false;
    cout<<"10: Item getPrice(), after setPrice(), is not 11, but: "<<item.getPrice()<<endl;
  }

  // Changers
  item.changeQuantity(-1);
  if (item.getQuantity() != 10){
    passed = false;
    cout<<"11: Item getQuantity(), after changeQuantity(-1), is not 10, but: "<<item.getQuantity()<<endl;
  }

  if (item.getHoldQuantity() != 8){
    passed = false;
    cout<<"12: Item getHoldQuantity(), after changeQuantity(-1), is not 8, but: "<<item.getHoldQuantity()<<endl;
  }

  item.changeQuantity(1);
  if (item.getQuantity() != 11){
    passed = false;
    cout<<"13: Item getQuantity(), after changeQuantity(1), is not 11, but: "<<item.getQuantity()<<endl;
  }

  if (item.getHoldQuantity() != 8){
    passed = false;
    cout<<"14: Item getHoldQuantity(), after changeQuantity(1), is not 8, but: "<<item.getHoldQuantity()<<endl;
  }


  item.changeHoldQuantity(1);
  if (item.getHoldQuantity() != 9){
    passed = false;
    cout<<"15: Item getHoldQuantity(), after changeHoldQuantity(1), is not 9, but: "<<item.getHoldQuantity()<<endl;
  }

  ItemIn itemIn(&item);
  if (*itemIn.item == item){} else{
    passed = false;
    cout << "16: temIn.item does not match item" << endl;
  }

  if(itemIn.purchaseQuantity != 0){
    passed = false;
    cout << "17: temIn.item purchaseQuantity is not 0, but: " << itemIn.purchaseQuantity << endl;
  }

  if(itemIn.purchasePrice != 0){
    passed = false;
    cout << "18: itemIn.item purchasePrice is not 0, but: " << itemIn.purchasePrice << endl;
  }
  return passed;
}

bool paymentPortalTest(){
  bool passed = true;
  ofstream pF("data/test_files/paymentPortal.json");
  pF.close();
  PaymentPortal paymentPortal("test_files");


  if (paymentPortal.getBalance() != 0){
    passed = false;
    cout << "paymentPortal getBalance() is not 0, but: " << paymentPortal.getBalance() << endl;
  }

  if (paymentPortal.getOverDraft() != 0){
    passed = false;
    cout << "paymentPortal getOverDraft() is not 0, but: " << paymentPortal.getOverDraft() << endl;
  }

  paymentPortal.deposit(-1);

  if (paymentPortal.getBalance() != 0){
    passed = false;
    cout << "paymentPortal getBalance(), deposit(-1), after is not 0, but: " << paymentPortal.getBalance() << endl;
  }

  paymentPortal.deposit(20);

  if (paymentPortal.getBalance() != 20){
    passed = false;
    cout << "paymentPortal getBalance(), after deposit(20), is not 20, but: " << paymentPortal.getBalance() << endl;
  }

  if (paymentPortal.getOverDraft() != 0){
    passed = false;
    cout << "paymentPortal getOverDraft(), after deposit(20),  is not 0, but: " << paymentPortal.getOverDraft() << endl;
  }

  paymentPortal.withdraw(30);

  if (paymentPortal.getBalance() != 0){
    passed = false;
    cout << "paymentPortal getBalance(), after withdraw(30), is not 0, but: " << paymentPortal.getBalance() << endl;
  }

  if (paymentPortal.getOverDraft() != 10){
    passed = false;
    cout << "paymentPortal getOverDraft(), after withdraw(30),  is not 0, but: " << paymentPortal.getOverDraft() << endl;
  }

  PaymentPortal paymentPortal2("test_files");

  if (paymentPortal2.getBalance() != 0){
    passed = false;
    cout << "paymentPortal2 getBalance() is not 0, but:  " << paymentPortal2.getBalance() << endl;
  }

  if (paymentPortal2.getOverDraft() != 10){
    passed = false;
    cout << "paymentPortal2 getOverDraft() is not 10, but: " << paymentPortal2.getOverDraft() << endl;
  }

  return passed;

}

bool transactionTest(){
  bool passed = true;
  Settings settings("live_files", false);
  PaymentPortal paymentPortal(settings.getFolder());
  Customer customer("Jonas", "H", "8023632222");
  Item item(0, "name", 10, 10);
  ItemIn itemIn(&item);
  Transaction transaction(&paymentPortal, 2,1);

  if(transaction.getId() != 2){
    passed = false;
    cout << "Transaction getId() is not 2, but: " << transaction.getId() << endl;
  }
  if(transaction.isCompleted()){
    passed = false;
    cout << "Transaction isCompleted() is true" << endl;
  }

  if(transaction.getCustomerId() != 1){
    passed = false;
    cout << "transaction.getCustomerId() != 1, but: " << transaction.getCustomerId() << endl;
  }

  if(transaction.getTotalCost() != 0){
    passed = false;
    cout << "transaction.getTotalCost() is not 0, but: " << transaction.getTotalCost() << endl;
  }

  transaction.addItem(item);
  if(transaction.getTotalCost() != 10){
    passed = false;
    cout << "transaction.getTotalCost(), after transaction.addItem(item), is not 10, but: " << transaction.getTotalCost() << endl;
  }

  transaction.removeItem(item);
  if(transaction.getTotalCost() != 0){
    passed = false;
    cout << "transaction.getTotalCost(), after transaction.removeItem(item), is not 0, but: " << transaction.getTotalCost() << endl;
  }

  transaction.completeTransaction();
  if(!transaction.isCompleted()){
    passed = false;
    cout << "Transaction isCompleted(), after completeTransaction(), is false" << endl;
  }
  return passed;
}

bool mainTest(){

  bool passed = true;

  unordered_map<int, Transaction> transactions;
  unordered_map<int, Transaction> transactionArchives;
  queue<int> transactionArchivesAge;

  unordered_map<int, Customer> customers;
  queue<int> customersAge;

  unordered_map<int, Item> items;

  Settings settings("test_files", false);

  // Wipe data
  settings.wipe();
  ofstream("data/test_files/transactions.json", ios::trunc).close();
  ofstream("data/test_files/items.json", ios::trunc).close();
  ofstream("data/test_files/customers.json", ios::trunc).close();


  PaymentPortal paymentPortal(settings.getFolder());
  SPass sPass(transactions, transactionArchives, transactionArchivesAge, customers, customersAge, settings, paymentPortal, items);

  Item item("Carrot", 100, 12);

  if(items.size() != 0){
    passed = false;
    cout << "1: items.size() is " << items.size() << " not 0" << endl;
  }

  if(item.getId() != -2){
    passed = false;
    cout << "2: item.getId() is " << item.getId() << " not -2" << endl;
  }


  addItem(item,sPass);
  if(items.size() != 1){
    passed = false;
    cout << "3: items.size() is " << items.size() << " not 1" << endl;
  }

  if(!items.count(1)){
    passed = false;
    cout << "4: items.count(1) is " << items.count(1) << " not 2" << endl;
  }

  // Manually erase to test archive and retrieve
  items.erase(1);

  if(items.size() != 0){
    passed = false;
    cout << "5: items.size() is " << items.size() << " not 0" << endl;
  }

  retrieveItems(sPass);
  if(items.size() != 1){
    passed = false;
    cout << "6: items.size() is " << items.size() << " not 1" << endl;
  }

  if(!items.count(1)){
    passed = false;
    cout << "7: items.count(1) is " << items.count(1) << " not 1" << endl;
  }

  if(items.at(1).getName() != "Carrot"){
    passed = false;
    cout << "8: items.count(1) is '" << items.count(1) << "' not 'Carrot'" << endl;
  }


  if(customers.size() != 0){
    passed = false;
    cout << "9: customers.size() is " << customers.size() << " not 0" << endl;
  }

  Customer cust("Jonas", "H", "8023632222");
  createCustomer(cust, sPass);
  if(customers.size() != 1){
    passed = false;
    cout << "10: customers.size() is " << customers.size() << " not 1" << endl;
  }

  if(!customers.count(1)){
    passed = false;
    cout << "11: customers.count(2) is " << customers.count(1) << " not 1" << endl;
  }

  if(customers.at(1).getFirstName() != "Jonas"){
    passed = false;
    cout << "12: customers.count(2) is " << customers.at(1).getFirstName() << " not 'Jonas'" << endl;
  }

  // Manually erase to test archive and retrieve
  customers.erase(1);
  customersAge.pop();

  if(customers.size() != 0){
    passed = false;
    cout << "12: customers.size() is " << customers.size() << " not 0" << endl;
  }

  if(customers.size() != 0){
    passed = false;
    cout << "13: customers.size() is " << customers.size() << " not 0" << endl;
  }

  retrieveCustomer(1, sPass);
  if(customers.size() != 1){
    passed = false;
    cout << "14: customers.size() is " << customers.size() << " not 1" << endl;
  }

  if(!customers.count(1)){
    passed = false;
    cout << "15: customers.count(1) is " << customers.count(1) << " not 1" << endl;
  }

  if(customers.at(1).getFirstName() != "Jonas"){
    passed = false;
    cout << "16: customers.count(1) is '" << customers.count(1) << "' not 'Jonas'" << endl;
  }

  Transaction tr(&paymentPortal);

  if(transactions.size() != 0){
    passed = false;
    cout << "17: transactions.size() is " << transactions.size() << " not 0" << endl;
  }

  if(tr.getId() != -2){
    passed = false;
    cout << "18: transactions.getId() is " << tr.getId() << " not -2" << endl;

  }

  if(tr.getCustomerId() != -2){
    passed = false;
    cout << "19: transactions.getCustomerId() is " << tr.getId() << " not -2" << endl;
  }

  createTransaction(tr,sPass);

  if(transactions.size() != 1){
    passed = false;
    cout << "20: transactions.size() is " << transactions.size() << " not 1" << endl;

  }

  if(tr.getId() != 1){
    passed = false;
    cout << "21: transactions.getId() is " << tr.getId() << " not 2" << endl;

  }
  if(tr.getCustomerId() != -2){
    passed = false;
    cout << "22: transactions.getCustomerId() is " << tr.getId() << " not -2" << endl;
  }

  if(item.getAvailableQuantity() != 12){
    passed = false;
    cout << "23: item.getAvailableQuantity() is " << item.getAvailableQuantity() << " not 12" << endl;
  }

  tr.addItem(item);

  if(item.getAvailableQuantity() != 11){
    passed = false;
    cout << "24: item.getAvailableQuantity() is " << item.getAvailableQuantity() << " not 11" << endl;
  }

  cancelTransaction(1, sPass);

  if(transactions.size() != 0){
    passed = false;
    cout << "25: transactions.size() is " << transactions.size() << " not 0" << endl;

  }

  return passed;



}