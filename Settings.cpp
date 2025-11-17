#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "include/json.hpp"
#include "Settings.h"
using namespace std;
Settings::Settings(const string folder, bool live) : live(live) {
  if (!filesystem::exists(filesystem::path("data") / folder)){
    cerr << "The folder '" << folder << "' does not exist in 'data'" << endl;
  }
  this->folder = folder;
  nlohmann::json j;
  ifstream in(filesystem::path("data") / folder / "settings.json");
  if (in.is_open()) {
    try{
      in >> j;
      transactionIndex = j.value("transactionIndex", transactionIndex);
      customerIndex = j.value("customerIndex", customerIndex);
      customerIndex = j.value("itemIndex", customerIndex);
      maxNumTransactions = j.value("maxNumTransactions", maxNumTransactions);
      maxNumCustomers = j.value("maxNumCustomers", maxNumCustomers);

    } catch(...) {
      updateFile();
    }
  } else{
    updateFile();
  }
}

void Settings::updateFile() {
  nlohmann::json j;
  j["transactionIndex"] = transactionIndex;
  j["customerIndex"] = customerIndex;
  j["itemIndex"] = itemIndex;
  j["maxNumTransactions"] = maxNumTransactions;
  j["maxNumCustomers"] = maxNumCustomers;

  ofstream out(filesystem::path("data") / folder / "settings.json");
  if (out.is_open()) {
    out << j.dump(5);
    out.close();
  }
}

int Settings::getTransactionIndex() const {return transactionIndex;}
void Settings::setTransactionIndex(int transactionIndex) {
  this->transactionIndex = transactionIndex;
  updateFile();
}

void Settings::addTransactionIndex() {
  transactionIndex++;
  updateFile();
}

int Settings::getCustomerIndex() const {return customerIndex;}
void Settings::setCustomerIndex(int customerIndex) {
  this->customerIndex = customerIndex;
  updateFile();
}

void Settings::addCustomerIndex() {
  customerIndex++;
  updateFile();
}

int Settings::getMaxNumTransactions() const {return maxNumTransactions;}
int Settings::getMaxNumCustomers() const {return maxNumCustomers;}

int Settings::getItemIndex() const {return itemIndex;}
void Settings::setItemIndex(int itemIndex) {
  this->itemIndex=itemIndex;
  updateFile();
}
void Settings::addItemIndex() {
  itemIndex++;
  updateFile();
}

string Settings::getFolder() const{
  return folder;
}

bool Settings::getlive() const{
  return live;
}

void Settings::wipe() {
  transactionIndex = 0;
  customerIndex = 0;
  itemIndex = 0;
  maxNumTransactions = 2;
  maxNumCustomers = 2;
  updateFile();
}