
#include "PaymentPortal.h"
PaymentPortal::PaymentPortal(std::string folder) {
  if (!filesystem::exists(filesystem::path("data") / folder)){
    cerr << "The folder '" << folder << "' does not exist in 'data'" << endl;
  }
  this->folder = folder;

  ifstream in(filesystem::path("data") / folder / "paymentPortal.json");
  nlohmann::json p;

  if (in.is_open()) {
    try{
      in >> p;
      balance = p.value("balance", 0);
      overDraft = p.value("overDraft", 0);
    } catch(...) {
      updateFile();
      balance = 0;
      overDraft = 0;

    }
  } else{
    balance = 0;
    overDraft = 0;
  }

}
void PaymentPortal::updateFile() {
  nlohmann::json p;
  p["balance"] = balance;
  p["overDraft"] = overDraft;
  ofstream out(filesystem::path("data") / folder / "paymentPortal.json");
  if (out.is_open()) {
    out << p.dump(5);
    out.close();
  }
}

int PaymentPortal::getBalance(){return balance;}
int PaymentPortal::getOverDraft(){return overDraft;};

void PaymentPortal::deposit(int amount){
 if (amount > 0) {
   if (!overDraft){
     balance += amount;
   } else if (overDraft - amount < 0){
     balance = amount - overDraft;
     overDraft = 0;
   } else {
     overDraft -= amount;
   }
 }
  updateFile();

}
void PaymentPortal::withdraw(int amount){
  if (amount > 0) {
    if (overDraft){
      overDraft += amount;
    } else if (balance - amount >= 0){
      balance -= amount;
    } else {
      amount -= balance;
      balance = 0;

      overDraft = amount;
    }
  }
  updateFile();

}