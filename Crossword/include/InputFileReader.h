#pragma once

#include <string>
#include <vector>
#include <fstream>

class InputFileReader
{
public:
    InputFileReader(std::istream& fileStream);
    ~InputFileReader();

    const std::vector<char>& getLetters_vect() const { return this->letters_vect; }
    const std::vector<unsigned int>& getLengths_vect() const { return this->lengths_vect; }
    const std::string getDictionaryFileName() const { return this->dictionaryFileName; }

protected:
    template <typename T>
    std::vector<T> getValuesInLine(std::istream& ifs);

    std::vector<char> letters_vect;
    std::vector<unsigned int> lengths_vect;
    std::string dictionaryFileName;

private:
};

