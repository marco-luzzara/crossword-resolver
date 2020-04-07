#include <iostream>
#include <fstream>
#include <set>
#include <unordered_set>
#include <string>
#include <utility>
#include <algorithm>

#include "include/LetterRepository.h"
#include "include/InputFileReader.h"

std::vector<std::pair<unsigned int, std::set<std::string>>> readDictionaryFile(
    std::istream& ifs_dictionary,
    const std::vector<unsigned int> lengths_vect)
{
    std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets;
    std::unordered_set<unsigned int> lengths_set(lengths_vect.begin(), lengths_vect.end());

    for (auto len : lengths_set)
        dictionarySets.push_back({len, std::set<std::string>()});

    if (lengths_vect.size() == 0)
        return dictionarySets;

    while (!ifs_dictionary.eof())
    {
        std::string word;
        ifs_dictionary >> word;

        auto iter_foundPair = std::find_if(dictionarySets.begin(), dictionarySets.end(),
                     [&word](const auto elem) { return elem.first == word.length(); });

        if (iter_foundPair != dictionarySets.end())
        {
            auto& targetSet = iter_foundPair->second;
            targetSet.insert(targetSet.end(), word);
        }
    }

    return dictionarySets;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
        throw std::invalid_argument("you must specify the input file name as argument to main function");

    std::ifstream ifs_in = std::ifstream(argv[1], std::ios::in);
    InputFileReader fileReader(ifs_in);
    ifs_in.close();

    auto letters_vect = fileReader.getLetters_vect();
    auto lengths_vect = fileReader.getLengths_vect();
    std::string dictionary_filename = fileReader.getDictionaryFileName();

    std::ifstream ifs_dictionary(dictionary_filename, std::ios::in);
    auto dictionarySets = readDictionaryFile(ifs_dictionary, lengths_vect);
    ifs_dictionary.close();

    LetterRepository repo(letters_vect);


}
