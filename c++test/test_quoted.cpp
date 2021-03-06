#include <iostream>
#include <iomanip>
#include <sstream>
 
int main()
{
    std::stringstream ss;
    std::string in = "String with spaces, and embedded \"quotes\" too";
    std::string out;
 
    ss << std::quoted(in);
    std::cout << "read in     [" << in << "]\n"
              << "stored as   [" << ss.str() << "]\n";
 
    ss >> std::quoted(out);
    std::cout << "written out [" << out << "]\n";
}

//read in     [String with spaces, and embedded "quotes" too]
//stored as   ["String with spaces, and embedded \"quotes\" too"]
//written out [String with spaces, and embedded "quotes" too]

