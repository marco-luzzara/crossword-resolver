#include <iostream>
#include <fstream>
#include <set>
#include <unordered_set>
#include <string>
#include <utility>
#include <algorithm>

#include "include/LetterRepository.h"
#include "include/InputFileReader.h"
#include "include/WordYielder.h"
#include <iterator>

std::vector<std::pair<unsigned int, std::set<std::string>>> readDictionaryFile(
    std::istream& ifs_dictionary,
    const std::vector<unsigned int> lengths_vect)
{
    std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets;
    std::set<unsigned int> lengths_set(lengths_vect.begin(), lengths_vect.end());

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

void findPhrases(const std::vector<std::pair<unsigned int, std::set<std::string>>>& dictionarySets,
    const std::vector<unsigned int>& wordsLength,
    LetterRepository& lr,
    int wordCounter, 
    std::unordered_set<std::string>& foundPhrases,
    std::string currentPhrase) 
{
    if (wordCounter == wordsLength.size())
    {
        currentPhrase.pop_back();
        foundPhrases.insert(std::move(currentPhrase));
        return;
    }

    unsigned int curLength = wordsLength[wordCounter];
    auto wordsSetPairIt = std::find_if(dictionarySets.begin(), dictionarySets.end(),
        [curLength](const auto& dictPair) {
            return dictPair.first == curLength;
        });

    if (wordsSetPairIt == dictionarySets.end())
        return;

    const std::set<std::string>& words = wordsSetPairIt->second;
    WordYielder wy(lr, words);

    for (const auto& word : wy)
    {
        findPhrases(dictionarySets, wordsLength, lr, wordCounter + 1, foundPhrases, currentPhrase + word + " ");
    }


}

std::unordered_set<std::string> startFindPhrases(const std::vector<std::pair<unsigned int, std::set<std::string>>>& dictionarySets,
    const std::vector<unsigned int>& wordsLength,
    LetterRepository& lr)
{
    std::unordered_set<std::string> foundPhrases;

    if (wordsLength.size() != 0)
        findPhrases(dictionarySets, wordsLength, lr, 0, foundPhrases, "");


    return foundPhrases;
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

    std::unordered_set<std::string> foundPhrases = startFindPhrases(dictionarySets, lengths_vect, repo);

    for (const auto& s : foundPhrases)
        std::cout << s << std::endl;

    return 0;
}
