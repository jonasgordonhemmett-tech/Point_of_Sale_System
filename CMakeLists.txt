cmake_minimum_required(VERSION 3.8)
project(POS)

set(CMAKE_CXX_STANDARD 20)


add_executable(Main
        paymentPortal.cpp
        Main.cpp
        Customer.cpp
        Employee.cpp
        Manager.cpp
        Item.cpp
        Transaction.cpp
        Settings.cpp
)

add_executable(Tester
        paymentPortal.cpp
        Tester.cpp
        Customer.cpp
        Employee.cpp
        Manager.cpp
        Item.cpp
        Transaction.cpp
        Settings.cpp
)
