#include "lib.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::vector<double>> readDataFromFile(const std::string &fileName)
{
    std::ifstream file(fileName);
    std::vector<std::vector<double>> data;
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<double> rowData;
        std::stringstream lineStream(line);
        double value;
        while (lineStream >> value)
        {
            rowData.push_back(value);
        }
        data.push_back(rowData);
    }
    return data;
}
