// See code from Demo

// https://www.youtube.com/watch?v=790aMkbsBm8&t=4014s
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Base.h"
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
#include <map>
#include <unordered_map>
#include <fstream>
#include "include/json.hpp"
#include <algorithm>
#include <queue>
#include <optional>
#include "settings.h"
#include "SPass.h"
ImVec2 BUTTON_SIZE_LARGE(500, 60);
ImVec2 BUTTON_SIZE(240, 30);

//TODO: GUEST TRANSACTIONS CANT BE RETRIEVED, cancel them always
//TODO: Create UI For this
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

//TODO: Create UI For this
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
using namespace std;

struct SPass2 {
    int currentScreen = 0;
    Customer* customerPtr = nullptr;
    Transaction* transactionPtr = nullptr;
    Item* itemPtr = nullptr;


};
// ------------------------------------------------------------
// Screen declarations
// ------------------------------------------------------------
void DrawMainMenu(SPass& sPass,SPass2& sPass2);
void DrawCartMenu(SPass& sPass,SPass2& sPass2);
void DrawItemScreen(SPass& sPass,SPass2& sPass2);
void DrawCreateCustomerScreen(SPass& sPass, SPass2& sPass2);

// ------------------------------------------------------------
// Error callback for GLFW
// ------------------------------------------------------------
static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}


// ------------------------------------------------------------
// MAIN
// ------------------------------------------------------------
int main(){
    unordered_map<int, Transaction> transactions;
    unordered_map<int, Transaction> transactionArchives;
    queue<int> transactionArchivesAge;

    unordered_map<int, Customer> customers;
    queue<int> customersAge;

    unordered_map<int, Item> items;

    Settings settings("live_files", true);

    PaymentPortal paymentPortal(settings.getFolder());
    SPass sPass(transactions, transactionArchives, transactionArchivesAge, customers, customersAge, settings, paymentPortal, items);
    Item carrot("carrot", 10, 10);
    addItem(carrot, sPass);
    Item carrot1("carrot1", 10, 10);
    addItem(carrot1, sPass);
    Item carrot2("carrot2", 10, 10);
    addItem(carrot2, sPass);
    Item carrot3("carrot3", 10, 10);
    addItem(carrot3, sPass);
    // --------------------------------------------------------
    // GLFW setup
    // --------------------------------------------------------
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Register", NULL, NULL);
    if (window == NULL)
        return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    // --------------------------------------------------------
    // ImGui setup
    // --------------------------------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(nullptr);
    SPass2 sPass2;


    // --------------------------------------------------------
    // Screen state
    // --------------------------------------------------------
    enum Screen {
        SCREEN_MAIN = 0,
        SCREEN_CART = 1,
        SCREEN_ITEM = 2,
        SCREEN_CREATE_CUSTOMER = 3,
    };
    sPass2.currentScreen = SCREEN_MAIN;


    // --------------------------------------------------------
    // Main loop
    // --------------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        // --------------------------------------------------------
        // Fullscreen window
        // --------------------------------------------------------
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGuiWindowFlags flags =
              ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoBringToFrontOnFocus
            | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("FullWindow", nullptr, flags);


        // --------------------------------------------------------
        // Call the current screen function
        // --------------------------------------------------------
        switch (sPass2.currentScreen)
        {
            case SCREEN_MAIN:
                DrawMainMenu(sPass, sPass2);
                break;
            case SCREEN_CART:
                DrawCartMenu(sPass, sPass2);
                break;

            case SCREEN_ITEM:
                DrawItemScreen(sPass, sPass2);
                break;
            case SCREEN_CREATE_CUSTOMER:
                DrawCreateCustomerScreen(sPass, sPass2);
        }

        ImGui::End();


        // --------------------------------------------------------
        // Render
        // --------------------------------------------------------
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }


    // --------------------------------------------------------
    // Cleanup
    // --------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


// ------------------------------------------------------------
// SCREEN IMPLEMENTATIONS
// ------------------------------------------------------------

// MAIN SCREEN
void DrawMainMenu(SPass& sPass, SPass2& sPass2) {
    // Resets all pointers
    sPass2.customerPtr = nullptr;
    sPass2.transactionPtr = nullptr;
    sPass2.itemPtr = nullptr;

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetWindowFontScale(2.0f);
    const char* text0 = "Main Menu";
    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - ImGui::CalcTextSize(text0).x) * 0.5f, 10));
    ImGui::Text("%s", text0);
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - BUTTON_SIZE_LARGE.x) / 2, 50));
    if (ImGui::Button("Login", BUTTON_SIZE_LARGE)) {
        if (checkCustomer(1, sPass)) {
            retrieveCustomer(1, sPass);
            sPass2.customerPtr = & sPass.customers.at(1);
        }
        sPass2.currentScreen = 1;
    }


    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - BUTTON_SIZE.x) * 1 * 0.25f + 10, 130));
    if (ImGui::Button("Create account", BUTTON_SIZE)) {
        sPass2.currentScreen = 3;
    }

    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - BUTTON_SIZE.x) * 3 * 0.25f - 10, 130));
    if (ImGui::Button("Shop as guest", BUTTON_SIZE)) {
        sPass2.currentScreen  = 1;
    }
}
// TODO: Stop reloading the entire vector every time, update it only when searched.
void DrawCartMenu(SPass& sPass, SPass2& sPass2)
{
    if (sPass2.customerPtr == nullptr) {
        Transaction transactionNew(&sPass.paymentPortal);
        int transactionId = createTransaction(transactionNew, sPass);
        sPass2.transactionPtr = &sPass.transactions.at(transactionId);
    } else {
        if (sPass2.customerPtr->getPendingTransaction() == nullptr) {
            Transaction transactionNew(&sPass.paymentPortal, sPass2.customerPtr->getCustomerId());
            int transactionId = createTransaction(transactionNew, sPass);
            sPass2.transactionPtr = &sPass.transactions.at(transactionId);
            sPass2.customerPtr->setpendingTransaction(sPass2.transactionPtr);
        } else {
            sPass2.transactionPtr = sPass2.customerPtr->getPendingTransaction();
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetCursorPos(ImVec2(io.DisplaySize.x / 2, 10));

    // ImGui stores text as character pointers
    ImGui::SetWindowFontScale(2.0f);
    const char* text0 = "Items";
    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - ImGui::CalcTextSize(text0).x) * 0.5f, 0+10));
    ImGui::Text("%s", text0);
    ImGui::SetWindowFontScale(1.0f);

    static char buffer[128] = ""; // empty C-string initially
    ImGui::SetNextItemWidth(240);
    ImGui::SetCursorPos(ImVec2(150, 50));
    ImGui::InputText("Search", buffer, IM_ARRAYSIZE(buffer));
    int typeSelect;
    try {
        typeSelect = stoi(buffer);
    } catch (...) {
        typeSelect = -1;
    }

    // ImGui::SetCursorPos(ImVec2(io.DisplaySize.x / 2, 10+));
    const char* text2 = "Items Available";
    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - ImGui::CalcTextSize(text2).x) * 0.5f, 130));
    ImGui::Text("%s", text2);


    static vector<string> items;

    // itemMap lets you find the itemId of the selected item from the shopping list
    unordered_map<int, int> itemMap;

    items.clear();
    for (const auto& pair : sPass.items) {

        if (typeSelect == -1) {
            items.push_back(pair.second.toString());
            itemMap[items.size()-1] = pair.first;
        } else {
            if (to_string(pair.second.getId()).find(to_string(typeSelect)) != std::string::npos) {
                items.push_back(pair.second.toString());
                itemMap[items.size()-1] = pair.first;
            }
        }
    }

    ImGui::SetCursorPos(ImVec2(150, 150));

    static int selected = -1;
    ImGui::BeginChild("storeCart", ImVec2(io.DisplaySize.x - 300, 180), true);
    for (int i = 0; i < items.size(); i++) {
        // c_str() gives pointer to chars inside string, ImGui needs it like this to work
        if (ImGui::Selectable(items[i].c_str(), selected == i)) {
            selected = i;
        }
    }
    ImGui::EndChild();

    // ImGui::SetCursorPos(ImVec2(io.DisplaySize.x / 2, 280));
    const char* text1 = "Your Cart";
    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - ImGui::CalcTextSize(text1).x) * 0.5f, 340));
    ImGui::Text("%s", text1);


    static vector<string> itemsCart;

    // itemMap lets you find the itemId of the selected item from the shopping list
    unordered_map<int, int> itemMapCart;


    if (sPass2.transactionPtr->getItems().size() == 0) {
        sPass2.transactionPtr->addItem(sPass.items.at(1));

    }
    itemsCart.clear();
    for (const auto& itemIn : sPass2.transactionPtr->getItems()) {
        itemsCart.push_back(itemIn.item->toString());
        itemMapCart[itemsCart.size()-1] = itemIn.item->getId();

    }
    ImGui::SetCursorPos(ImVec2(150, 360));
    static int selectedCart = -1;

    ImGui::BeginChild("yourCart", ImVec2(io.DisplaySize.x - 300, 100), true);
    for (int i = 0; i < itemsCart.size(); i++) {
        // c_str() gives pointer to chars inside string, ImGui needs it like this to work
        if (ImGui::Selectable(itemsCart[i].c_str(), selectedCart == i)) {
            selectedCart = i;
            selected = -1;

        }
    }
    ImGui::EndChild();


    string s = "Item Selected: ";
    int selectedId = -2;
    if (selected != -1) {
        selectedId = itemMap[selected];
        s += to_string(selectedId);
    } else if (selectedCart != -1) {
        selectedId = itemMapCart[selectedCart];
        s += to_string(selectedId);
    } else {
        s += "None";
    }
    ImGui::SetCursorPos(ImVec2(450, 60));

    ImGui::Text("%s", s.c_str());
    
    if (selectedId >= 0) {
        ImGui::SetCursorPos(ImVec2(150, 90));
        if (ImGui::Button("View selected", BUTTON_SIZE)) {
            if (sPass.items.count(selectedId)) {
                sPass2.itemPtr = &sPass.items.at(selectedId);
                sPass2.currentScreen = 2;
            } else {
                cerr << "Item not found" << endl;
                sPass2.itemPtr = nullptr;
            }
        }
    }

    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - BUTTON_SIZE.x) * 1 * 0.25f + 10, 480));
    if (ImGui::Button("Save Transaction", BUTTON_SIZE)) {
        sPass2.currentScreen = 0;
    }

    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - BUTTON_SIZE.x) * 3 * 0.25f - 10, 480));
    if (ImGui::Button("Cancel Transaction ", BUTTON_SIZE)) {
        sPass2.currentScreen  = 0;
    }

    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - BUTTON_SIZE_LARGE.x) / 2, 530));
    if (ImGui::Button("Checkout", BUTTON_SIZE_LARGE)) {
        sPass2.currentScreen = 0;
    }

}


void DrawItemScreen(SPass& sPass, SPass2& sPass2){
    if (sPass2.itemPtr  == nullptr) {
        cerr << "Item does not exist" << endl;
        sPass2.currentScreen = 1;
    } else if (sPass2.transactionPtr  == nullptr) {
        cerr << "Transaction does not exist" << endl;
        sPass2.currentScreen = 1;
    }

    ItemIn* itemIn = sPass2.transactionPtr->getItem(*sPass2.itemPtr);
    int cartQuantity = (itemIn == nullptr) ? 0 : itemIn->purchaseQuantity;
    ImGui::SetCursorPos(ImVec2(150, 140));
    cout << "cartQuantity: " << cartQuantity << endl;
    string s2 = "cartQuantity: " + to_string(cartQuantity);

    ImGui::Text("%s", s2.c_str());

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetWindowFontScale(2.0f);
    const char* text0 = "Item Menu";
    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - ImGui::CalcTextSize(text0).x) * 0.5f, 10));
    ImGui::Text("%s", text0);
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SetCursorPos(ImVec2(300, 50));
    string s = "Item selected: " + sPass2.itemPtr->toString();
    ImGui::Text("%s", s.c_str());


    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - BUTTON_SIZE.x) * 1 * 0.25f + 10, 80));
    if (ImGui::Button("Save Transaction", BUTTON_SIZE)) {
        sPass2.currentScreen = 0;
    }

    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - BUTTON_SIZE.x) * 3 * 0.25f - 10, 80));
    if (ImGui::Button("Cancel Transaction ", BUTTON_SIZE)) {
        sPass2.currentScreen  = 0;
    }



    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - BUTTON_SIZE_LARGE.x) / 2, 110));
    if (ImGui::Button("Go Back", BUTTON_SIZE_LARGE)) {
        sPass2.currentScreen = 1;
    }

}

void DrawCreateCustomerScreen(SPass& sPass, SPass2& sPass2)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetWindowFontScale(2.0f);
    const char* text0 = "New Account";
    ImGui::SetCursorPos(ImVec2((io.DisplaySize.x - ImGui::CalcTextSize(text0).x) * 0.5f, 10));
    ImGui::Text("%s", text0);
    ImGui::SetWindowFontScale(1.0f);

    Customer customer = createCustomerInput();
    int customerId = createCustomer(customer, sPass);
    if (customerId < 0) {
        cerr << "Error creating customer from input" << endl;
        sPass2.currentScreen = 0;
    }
    sPass2.customerPtr = &sPass.customers.at(customerId);
    sPass2.currentScreen = 1;
}