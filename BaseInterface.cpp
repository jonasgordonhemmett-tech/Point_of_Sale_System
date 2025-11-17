#include "Base.h"
#include "BaseInterface.h"

template <typename dataType>
dataType userInput(string messege){
    string inputString;
    dataType input;

    while(true)
    {

        bool noSpace = true;
        cout << messege;


        getline(cin, inputString);

        // Gets rid of invalid characters
        for (char c : inputString)
        {
            if (!(isalpha(c) || isdigit(c)) && c != ' ' && c != '\r'){
                noSpace = false;
                break;
            }
        }

        if (noSpace)
        {
            stringstream iss(inputString);

            if (iss >> input) return input;

        }
    }

    // End of userInput
}

Customer createCustomerInput(){
    string firstName, lastName, phoneNumber;

    while (true)
    {
        firstName = userInput<string>("Please enter your first name\n(E.G. Jonas):");

        if (!firstName.empty()) break;

        cout << "Invalid input!" << endl;
    }

    for (int i = 0; i < firstName.length(); i++)
    {
        firstName[i] = (!i) ? toupper(firstName[i]) : tolower(firstName[i]);
    }

    while (true)
    {
        lastName = userInput<string>("Please enter your last name\n(E.G. Hemmett):");

        lastName.erase(std::remove(lastName.begin(), lastName.end(), ' '), lastName.end());

        if (!lastName.empty()) break;

        cout << "Invalid input!" << endl;
    }

    // I added this to account for last names like McNee where 2 characters are capitalized.
    if (lastName.length() >= 2 && tolower(lastName[0]) == 'm' && tolower(lastName[1]) == 'c')
    {
        for (int i = 0; i < lastName.length(); i++)
        {
            lastName[i] = (!i || i == 2 ) ? toupper(lastName[i]) : tolower(lastName[i]);
        }

    } else if (lastName.length() >= 3 && tolower(lastName[0]) == 'v' && tolower(lastName[1]) == 'o'  && tolower(lastName[1]) == 'n'){
        for (int i = 0; i < lastName.length(); i++)
        {
            lastName[i] = (!i || i == 3) ? toupper(lastName[i]) : tolower(lastName[i]);
        }
    }


    else {
        for (int i = 0; i < lastName.length(); i++)
        {
            lastName[i] = (!i) ? toupper(lastName[i]) : tolower(lastName[i]);
        }

    }


    while (true)
    {
        int allChar = true;
        phoneNumber = userInput<string>("Please enter your phone number\n(Enter as 10 or 11 digits):");

        for (char c : phoneNumber)
        {
            if (!isdigit(c)) allChar = false;
        }

        if (allChar)
        {
            if (phoneNumber.length() == 10) phoneNumber.insert(phoneNumber.begin(), '1');
            if (phoneNumber.length() == 11)  break;
        }

        cout << "Phone: " << phoneNumber << endl;
        cout << "Invalid input!" << endl;
    }

    return Customer(firstName, lastName, phoneNumber);
// End of createCustomer
}

void transactionInput(Transaction& transaction, SPass& sPass){
    bool running = true;
    while (running){
        int menuOption= userInput<int>("What would you like to do?\n(1: Add items, 2: Remove items, 3: Checkout, 4: Cancel transaction)");

        if (menuOption == 1) {
            while (true) {
                cout << "Products are formatted like Name(ID) Quantityx $price" << endl;

                for (const auto& pair : sPass.items) {
                    cout << pair.second << endl;
                }

                int addOption = userInput<int>("What would you like to add\n(Item ID: Add item, 0: Exit)");

                if (addOption == 0) {
                    break;
                }

                if (addOption >= 1) {
                    if (findItem(addOption, sPass) != nullptr) {
                        if (transaction.addItem(sPass.items.at(addOption))) {
                        } else {
                            cout << "Could not add" << endl;
                        }
                    } else {
                        cout << "Could not add" << endl;
                    }

                }
                cout << ("called") << endl;
            }
        } else if (menuOption == 2) {
            while (true) {
                cout << "Products are formatted like Name(ID) Quantityx $price, holdQuantity" << endl;

                for (const auto& item : transaction.getItems()) {
                    cout << *item.item << endl;
                    cout << item.purchaseQuantity << endl;
                }

                int addOption = userInput<int>("What would you like to remove\n(Item ID: Remove item, 0: Exit)");

                if (addOption == 0) {
                    break;
                }

                if (addOption >= 1) {
                    if (findItem(addOption, sPass) != nullptr) {
                        if (transaction.removeItem(sPass.items.at(addOption))) {
                            cout << "Removed" << endl;
                        } else {
                            cout << "Could not Remove" << endl;
                        }
                    } else {
                        cout << "Could not Remove" << endl;
                    }

                }
            }


        } else if (menuOption == 3) {
            transaction.completeTransaction();
            running = false;
            cout << "transaction complete" << endl;
        } else if (menuOption == 4) {
            transaction.completeTransaction();
            running = false;
            cout << "transaction canceled" << endl;
        }

    }

}



void menuLogin(SPass& sPass) {
    cout << "-- Jonas's store --" << endl;

    int customerId = -2;
    while (true) {
        int menuOption = userInput<int>( "Would you like to login?\n(1: Existing account, 2: New account, 3: Logged out)");

        if (menuOption == 1) {
            customerId = menuGetCustomer(sPass);

            if (customerId != -2) {
                break;
            }
        } else if (menuOption == 2) {
            Customer customer = createCustomerInput();
            customerId = createCustomer(customer, sPass);
            if (customerId != -2) {
                break;
            }
        } else if (menuOption == 3) {
            break;
        }
        cout << "Invalid input!" << endl;
    }

    menuTransaction(customerId, sPass);

}

int menuGetCustomer(SPass& sPass) {
    while (true) {
        int menuOption = userInput<int>("Please enter your customer ID\n(Your customer ID, 0: Go back)");

        if (menuOption < 0) {
            continue;
        } else if (menuOption == 0) {
            return -2;
        } else if (checkCustomer(menuOption, sPass)) {
            retrieveCustomer(menuOption, sPass);
            cout << "Welcome Back " << sPass.customers.at(menuOption).getFirstName() << endl;

            return menuOption;
        }
    }
}

void menuTransaction(int customerId, SPass &sPass) {
    // TODO, Remove after testing
    // Item carrot("carrot", 10, 10);
    // addItem(carrot, sPass);
    Transaction* transaction = nullptr;
    int transactionId;
    if (customerId != -2 && sPass.customers.count(customerId)) {
        Customer& customer = sPass.customers.at(customerId);
        if (sPass.customers.at(customerId).getPendingTransaction() == nullptr) {
            Transaction transactionNew(&sPass.paymentPortal, customerId);
            transactionId = createTransaction(transactionNew, sPass);
            customer.setpendingTransaction(&sPass.transactions.at(transactionId));
        }
      transaction = sPass.customers.at(customerId).getPendingTransaction();
    } else {
        Transaction transactionNew(&sPass.paymentPortal);
        transactionId = createTransaction(transactionNew, sPass);
        transaction = &sPass.transactions.at(transactionId);
    }

    bool valid = true;
    if (transaction == nullptr) {
        cerr << "Transaction not found" << endl;
        valid = false;
    }

    enum menuOptions {VIEWCART = 1, VIEWITEMS = 2, ADDITEM = 3, REMOVEITEM = 4, CANCELTRANSACTION = 5, CHECKOUT = 6, EXIT = 0};
    while (valid) {

        int menuOption = userInput<int>("Transaction(1: View cart, 2: view items, 3: Add item, 4: Remove item, 5: Cancel Transaction, 6: Checkout, 0: Exit (Save for later))");

        if (menuOption == VIEWCART) {
            cout << "itemName(ID) quantity $price" << endl;
            cout << "----------------------------" << endl;
            if (!transaction->getItems().empty()){
                for (ItemIn itemIn : transaction->getItems()) {
                    cout << itemIn << endl;
                }
            } else{
                cout << "Your cart is empty" << endl;
            }
        } else if (menuOption == VIEWITEMS) {
            cout << "itemName(ID) quantity $price" << endl;
            cout << "----------------------------" << endl;
            for (auto item : sPass.items) {
                cout << item.second << endl;
            }
        } else if (menuOption == ADDITEM)
        {
            Item* item = nullptr;
            while (true) {
                int itemId = userInput<int>("(Enter item ID:, 0 to go back)");

                if (itemId < 0) {
                    continue;
                } else if (itemId == 0) {
                    item = nullptr;
                    break;
                } else {
                    item = findItem(itemId, sPass);
                    if (item != nullptr) {
                        break;
                    }

                }
            }
            if (item == nullptr){
                continue;
            }
            int numLimit = item->getAvailableQuantity();

            while (true)
            {
                int quantity = userInput<int>("How many would you like to add (0 exit, 1 to " + to_string(numLimit) + ")");
                if (quantity == 0){
                    break;
                } else if (quantity < 0 or quantity > numLimit) {
                    cout << "Invalid quantity" << endl;
                } else if (transaction->addItem(*item, quantity)){
                    cout << "Added  " << quantity << " " << item->getName() << (quantity == 1 ? "" : "s") << " to your shopping cart" << endl;
                    break;
                } else {
                    cerr << "Item could not be added!" << endl;
                    break;
                }
            }
        } else if (menuOption == REMOVEITEM) {
            ItemIn* itemIn = nullptr;
            while (true) {
                int itemId = userInput<int>("(Enter item ID:, 0 to go back)");

                if (itemId < 0) {
                    continue;
                } else if (itemId == 0) {
                    break;
                } else {
                    for (ItemIn& i : transaction->getItems())
                    {
                        cout << "1: " << i.item->getId() << endl;
                        if (i.item->getId() == itemId)
                        {
                            itemIn = &i;
                            break;
                        }
                    }
                if (itemIn != nullptr){
                    break;
                    }
                }
            }
            if (itemIn == nullptr)
            {
                cerr << "Invalid item";
                continue;
            }
            int numLimit = itemIn->purchaseQuantity;

            while (true)
            {
                int quantity = userInput<int>("How many would you like to remove (1 to " + to_string(numLimit) + "), 0: exit");
                if (quantity == 0){
                    break;
                } else if (quantity < 0 or quantity > numLimit) {
                    cout << "Invalid quantity" << endl;
                } else if (transaction->removeItem(*itemIn->item, quantity)){
                    cout << "Removed  " << quantity << " " << itemIn->item->getName() << (quantity == 1 ? "" : "s") << " to your shopping cart" << endl;
                    break;
                } else {
                    cerr << "Item could not be added!" << endl;
                    break;
                }
            }
        } else if (menuOption == CANCELTRANSACTION) {
            if (cancelTransaction(transactionId, sPass)) {
                cout << "Transaction canceled" << endl;
            } else {
                cerr << "Transaction could not be canceled" << endl;
            }
            menuOption = EXIT;
        } else if (menuOption == CHECKOUT) {
            if (transaction == nullptr) {
                cerr << "Transaction is empty" << endl;
                continue;
            }

            if (transaction->getItems().empty()) {
                cout << "Your cart is empty, please add some items before checking out" << endl;
                continue;
            }
            cout << "Your total is: " << priceFormat(transaction->getTotalCost()) << endl;
            while (true){
                int exitChoice = userInput<int>("Would you like to checkout?\n(1: Checkout, 0: Continue Shopping");

                if (exitChoice == 1) {
                    if (!archiveTransaction(transactionId, sPass)) {
                        cerr << "Transaction could not be finished" << endl;
                        break;
                    }
                    cout << "Thanks for shopping" << endl;
                    menuOption = EXIT;
                    break;
                } else if (exitChoice == 0) {
                    break;
                }

            }


        }

        if (menuOption == EXIT) {
            cout << "Goodbye" << endl;
            break;
        }
    }
}

string priceFormat(int price) {
    int dollars = price / 100;

    // Handles negative prices, if need be
    int cents = abs(price % 100);

    string fill = (cents < 10) ? "0" : "";

    return "$" + to_string(dollars) + "." +  fill + to_string(cents);
}
