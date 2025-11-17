# Jonas Hemmett

# Requirements
pyplot: pip install matplotlib
JSON: included in folder

# Summary
My program is a grocery store POS system

My first programming project was a POS system, so I thought I’d be interested in how much better I can make one starting from the ground up (No code was reused).

The past few weeks have been spent primarily on the backend of the POS. Having to read and write between RAM and JSON files was much harder and more frustrating than I first thought, but it is basically all done. For the next assignment I will finish the memory allocation stuff and build a user interface.

## Features include:
* Modification, and addition of items, customers, and transactions at runtime
* Removal of items at runtime
* Dynamically adding and removing items and transactions to and from memory
* Persistent JSON Storage
* Efficient item, customer, and transaction lookup
* Testing interface
* Basic UI

# Languages
*Python
* C++

The bulk of the program is run in C++, Python is used for graphing, data is transferred through the JSON save files.

# Bugs and Future Development
## Bugs
Input validation works, but it requires some deal of coaxing (E.G. Input validation is needed to call a function that is supposed to validate inputs.

##  Future development 
* Improved input validation works
* Removal of unused code
* Adding a proper UI
* Tidying up unused code
* Streamlining JSON reading/writing
* Replacing index input with reference input. E.G. archiveTransaction takes in an index but I want it to take in a reference. This is not as easy as it sounds as not all the data is in memory

# Sources
* https://json.nlohmann.me
* https://www.youtube.com/watch?v=Sa8bdVogGIo
* https://www.w3schools.com/cpp/cpp_queues.asp
* https://www.w3schools.com/cpp/cpp_files.asp
* https://www.geeksforgeeks.org/python/read-json-file-using-python/
* https://www.geeksforgeeks.org/python/matplotlib-pyplot-xticks-in-python/
* https://matplotlib.org/stable/tutorials/pyplot.html 
