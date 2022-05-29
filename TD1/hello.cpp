//Incude the library that defines input/output streams and operation on them
#include <iostream>

int main()
{
    //print "Hello, world!" on the standard output
    std::cout << "Hello, world!" // <- Notice: no ';' here : 1 statement / 2 lines

    // ...then advance to the next line
        << std::endl;   // <- 2nd line of the statement

    // Declare the variable 'name' to be an array of 256 characters 
    char name[256];

    std::cout << "How many are you?" << std::endl;
    int count = 0;
    std::cin >> count;

    for(int i = 0; i < count; i++){
    
    std::cout << "What is your name?" << std::endl;
                         
    // Read the name from the standard input
    std::cin >> name;

    std::cout << "Hello, " << name << std::endl;
    }
  
    return 0;
}