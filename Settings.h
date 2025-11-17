#include <string>
#include <fstream>
#include <iostream>
#include "include/json.hpp"
#include <filesystem>

#ifndef SETTINGS_H
#define SETTINGS_H


/*
  Stores settings information that may be changed between runs.
*/
class Settings {
private:
  const std::string path;
  // The 0th customer is used for people not logged in, transactions start at 1 to match for simplicity’s sake
  int transactionIndex = 0;
  int customerIndex = 0;
  int itemIndex = 0;
  int maxNumTransactions = 2;
  int maxNumCustomers = 2;
  std::string folder;
  bool live;

  void updateFile();

public:
  Settings(const std::string folder, bool live);

  // Getters and setters
  int getTransactionIndex() const;
  void setTransactionIndex(int transactionIndex);
  void addTransactionIndex();

  int getCustomerIndex() const;
  void setCustomerIndex(int customerIndex);
  void addCustomerIndex();

  int getMaxNumTransactions() const;
  int getMaxNumCustomers() const;

  int getItemIndex() const;
  void setItemIndex(int itemIndex);
  void addItemIndex();

  std::string getFolder() const;

 // Tells us if the program is live.s
  bool getlive() const;

  // Resets all save files in the same folder
  void wipe();

};



#endif //SETTINGS_H
