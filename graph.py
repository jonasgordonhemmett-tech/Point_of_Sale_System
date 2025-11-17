import matplotlib.pyplot as plt
import json
def getCustomer(id):
    assert id

    # https://www.geeksforgeeks.org/python/read-json-file-using-python/
    with open('data/test_files/Customers.json', 'r') as file:
        customers = json.load(file)

    customer = None
    for c in customers:
        if str(c["customerId"]) == str(id):
            customer = c
            break
    
    if not customer:
        print(f"No customer found with id {id}")
        return

    return customer

def getTransaction(id):
    assert id

    # https://www.geeksforgeeks.org/python/read-json-file-using-python/
    with open('data/test_files/Transactions.json', 'r') as file:
        transactions = json.load(file)

    transaction = None
    for t in transactions:
        if str(t["id"]) == str(id):
            transaction = t
            break
    
    if not transaction:
        print(f"No transaction found with id {id}")
        return

    return transaction

def getItem(id):
    assert id

    # https://www.geeksforgeeks.org/python/read-json-file-using-python/
    with open('data/test_files/Items.json', 'r') as file:
        items = json.load(file)

    item = None
    for i in items:
        if str(i["id"]) == str(id):
            item = i
            break
    
    if not item:
        print(f"No item found with id {id}")
        return

    return item


def viewCustomerPurchases(size):
    assert size >= 0

    # https://www.geeksforgeeks.org/python/read-json-file-using-python/
    with open('data/test_files/Customers.json', 'r') as file:
        customers = json.load(file)

    # Lambda function sort helper
    getTransactionIndex = lambda c: len(c["transactions"])
    customers.sort(key = getTransactionIndex, reverse = True)

    names = [c["firstName"] for c in customers[:size]]
    ids = [c["customerId"] for c in customers[:size]]
    sales = [len(c["transactions"]) for c in customers[:size]]

    plt.title("Top Customers")
    plt.xlabel('Customer')
    plt.ylabel('# of Purchases')


    plt.bar(names[:size], sales[:size])

    # https://www.geeksforgeeks.org/python/matplotlib-pyplot-xticks-in-python/

    nr = range(len(names[:3]))
    plt.xticks(nr, [f"{names[i]}\n{ids[i]}" for i in nr])

    plt.show()

def customerPurchasesMenu():
    # Finds the number of customers, used 
    try:
        with open('data/test_files/Customers.json', 'r') as file:
            customers = json.load(file)
            maxNumCustomers = len(customers)
    except:
        print("Could not open C.json")
        maxNumCustomers = -1

    choice = None
    while True:
        try:
            if maxNumCustomers == -1:
                choice = int(input("How many customers?\n(Enter a number): "))
            elif maxNumCustomers == 0:
                print("There are 0 customers")
                return
            elif maxNumCustomers == 1:
                choice = 1
                break
            else:
                choice = int(input(f"How many customers?\n(Enter a number between 1 and {maxNumCustomers}): "))

        except:
            None
        
        if maxNumCustomers == -1:
            if not(choice < 0):
                break
        else:
            # Defaults to showing all customers
            if not(choice):
                choice = maxNumCustomers
                break
            if not(choice < 0 or choice > maxNumCustomers):
                break
        
        print("Invalid Choice")
    viewCustomerPurchases(choice)

def viewCustomer(id):
    customer = getCustomer(id)
    
    if not customer:
        return

    transactions = customer["transactions"]
    plt.xlabel(f"Purchases")
    plt.ylabel(f"$ Spent")

    ids = []
    costs = []

    for t in customer["transactions"]:
        tr = getTransaction(t)
        if tr is not None:
            ids.append(t)
            costs.append(tr["totalCost"])

    plt.bar(ids, costs)

    tr = range(len(customer["transactions"]))

    plt.subplot


    
    plt.subplots_adjust(top=0.75) 

    plt.figtext(0.1, 0.8, f"Name: {customer['firstName']} {customer['lastName']}\nID: {customer['customerId']}\nStore Credit: {customer['credit']}", ha = "left", va = "bottom", fontsize = 15)
    

    plt.show()

def customerMenu():
    #TODO: If customers are removed, the limit should not be based
     # Finds the number of customers, used 

    choice = None
    while True:
        choice = int(input(f"Who would you like to view?\n(Enter a customer ID (for testing sake, 1 works)): "))

        with open('data/test_files/Customers.json', 'r') as file:
            customers = json.load(file)
        
        found = False
        for c in customers:
            if c["customerId"] == choice:
                found = True
                break
        
        if found:
            break

    viewCustomer(choice)

if __name__ == "__main__":
    choice = None
    while True:
        try:
            choice = int(input("What graphs would you like to view?\n(1: Top Customers, 2: Customer info): "))


        except:
            None
        
        if not(choice < 0 or choice > 2):
            break
        
        print("Invalid Choice")
    
    match choice:
        case 1:
            customerPurchasesMenu()
        case 2:
            customerMenu()
    
    