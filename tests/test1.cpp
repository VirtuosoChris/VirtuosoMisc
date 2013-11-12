#include "../helperFunctions.h"
#include "../printf.h"

int main(void){


    std::string x("Hello Customer Number ");
    x += 5;

    std::string z("Pi is  ");
    std::string y = z + 3.14159;

    printLn(std::cout, "Concatenated string with plusequals test: ",x);
    printLn(std::cout, "Concatenated string with plus test: ",y);

    printDelEndl(std::cout, ' ', "The list of things is ", 1,2,3,4,5);
    printDelLn(std::cout, '\n',"Countdown:", 10,9,8,7,"...");
    printEndl(std::cout, "This printf will end in a newline.");

    printLn(std::cout, "This should be on its own line");

    print(std::cout, "This too");




}
