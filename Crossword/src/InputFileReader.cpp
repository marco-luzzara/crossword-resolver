#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <numeric>

#include "../include/InputFileReader.h"

InputFileReader::InputFileReader(std::istream& fileStream)
{
    this->letters_vect = getValuesInLine<char>(fileStream);
    if (std::find_if_not(this->letters_vect.begin(), this->letters_vect.end(), [](char c) {
            return std::isalpha((int)c);
        }) != this->letters_vect.end() || this->letters_vect.size() == 0)
    {
        throw std::invalid_argument("first line must contain at least 1 alphabetical character [a-zA-Z]");
    }

    this->lengths_vect = getValuesInLine<unsigned int>(fileStream);
    if (this->lengths_vect.size() == 0)
        throw std::invalid_argument("second line must contain at least 1 numerical character [0-9]");

    if (std::accumulate(this->lengths_vect.begin(), this->lengths_vect.end(), 0) != this->letters_vect.size())
        throw std::invalid_argument("the sum of lengths must equal to the number of letters");

    std::getline(fileStream, this->dictionaryFileName);
}

template <typename T>
std::vector<T> InputFileReader::getValuesInLine(std::istream& ifs)
{
    std::vector<T> values;

    std::string values_line = "";
    std::getline(ifs, values_line);
    std::istringstream values_stream(values_line);

    T value;
    while (values_stream >> value)
    {
        values.push_back(value);
    }

    return values;
}

InputFileReader::~InputFileReader()
{
    //dtor
}
