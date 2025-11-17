# Requirements
pyplot: pip install matplotlib
JSON: included in folder

# Summary
I designed this program primarily as a proof of concept for dynamically managing memory. Here only the most recently accesed customers and transactions are stored in live memory while the rest are stored in JSON files. This keeps the memory usage manageable, as it wouldn't make sense for a store to keep possibly millions of customers in memory all at once. In addition, having  JSON files has the added benefit of saving data (E.G., item quantities) between runs. A more detailed description is in [MemoryManagmentSlideShow.pdf](MemoryManagmentSlideShow.pdf)

Although most of the work for this project was done on the backend, I did design a rudimentary POS system for the Python terminal.

# Key Concepts
* Memory management
* Usage of pointers and references
* JSON file IO
* Input validation
* OOP relationships like composition and inheritance
* Exception handling
* UI development


# How to Run
* Reload CMakeLists.txt
* compile
* Run "Main"



# Sources
* https://json.nlohmann.me





