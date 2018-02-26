#include "scanner.h"
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	std::string sourceFile = "source.txt";
	if(argc == 2)
	{
		sourceFile = std::string(argv[1]);
	}
	std::cout << "processing source file: " << sourceFile << std::endl;
	Scanner scanner(sourceFile);
	scanner.scan();
    scanner.print();
	auto atoccStr = scanner.getAtoCC();
	std::cout << "AtoCC code: \n"  << atoccStr << std::endl;

	std::ofstream atocc_file ("atocc.txt");
	if (atocc_file.is_open())
	{
	  atocc_file << atoccStr; 
	  atocc_file.close();
	}

    auto jsonStr = scanner.toJson();
    //std::cout << "token json: \n"  << jsonStr << std::endl;
    std::ofstream json_file ("token.json");
    if (json_file.is_open())
    {
      json_file << jsonStr;
      json_file.close();
    }
	return 0;
}

