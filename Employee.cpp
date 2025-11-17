#include "Employee.h"
#include <iostream>

using namespace std;
// Must match declaration exactly
Employee::Employee(const std::string& firstName, const std::string& lastName, const std::string& phoneNumber)
    : Customer(firstName, lastName, phoneNumber){}

Employee::Employee(const Employee& employee): Customer(employee){}

Employee::Employee(const Customer& customer): Customer(customer){}

/*
 * This function shortens a hashed ID.
 * The input ID contains only numbers (9 digits).
 * I wanted to use numbers and both cases of letters (61 digits).
 */

// This took way too long.
string Employee::hashID2(){
    string code = hashID();

    int INTIALS_SPACE = 2;
    int ROLE_SPACE = 1;
    string codeOut = string(1, code[0]) + string(1, code[1]);

    // IDs start and end with extra characters
    int counter = INTIALS_SPACE;

  int limit;
  while (counter < code.length() - ROLE_SPACE)
  {
    // int innerCounter = counter;
    // cout << "counter = " << counter << endl;

    int digit = (int)code[counter] - 48;
    limit = digit;
    if (counter < code.length() - ROLE_SPACE - 1){
      int digitNext = (int)code[counter + 1] - 48;

      if (digit * 10 + digitNext <= 62){
        limit = digit * 10 + digitNext;
        counter++;
      }
    }
    counter++;


    // while (innerCounter < code.length() - ROLE_SPACE){
    //   int digit = (int)code[innerCounter] - 48;
    //
    //   if (limit * 10 + digit <= 62)
    //   {
    //     limit = limit * 10 + digit;
    //     counter++;
    //     innerCounter++;
    //   } else break;


    counter++;
        char asciiConvert;

        if (limit <= 9){
            asciiConvert = char(limit + 48);
        } else if (limit <= 9 + 26){
            asciiConvert = char(limit + 55);
        } else{
            asciiConvert = char(limit + 55 + 6);
        }

        codeOut += asciiConvert;

    }

    return codeOut;
}
string Employee::hashID(){
      return string(1, getFirstName()[0]) + string(1, getLastName()[0]) +  to_string(hash<string>{}(getPhoneNumber())) + "e";
}
