#include <iostream>
#include <fstream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <utility>
#include <algorithm>
#include <cassert>

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

std::vector<std::string> findPhrases(std::unordered_map<std::string, std::vector<std::string>>& dp,
    const std::vector<std::pair<unsigned int, std::set<std::string>>>& dictionarySets,
    const std::vector<unsigned int>& wordsLength,
    LetterRepository& lr,
    int wordCounter, 
    std::string currentWord) 
{
    auto dpEntryIt = dp.find(lr);
    std::vector<std::string> foundPhrases;

    if (dpEntryIt != dp.end())
    {
        foundPhrases = dpEntryIt->second;
    }
    else
    {
        if (wordCounter == wordsLength.size())
            return { currentWord };

        unsigned int curLength = wordsLength[wordCounter];
        auto wordsSetPairIt = std::find_if(dictionarySets.begin(), dictionarySets.end(),
            [curLength](const auto& dictPair) {
                return dictPair.first == curLength;
            });

        if (wordsSetPairIt == dictionarySets.end())
            return std::vector<std::string>();

        const std::set<std::string>& words = wordsSetPairIt->second;
        WordYielder wy(lr, words);

        for (const auto& word : wy)
        {
            auto foundPhrasesFromWord = findPhrases(dp, dictionarySets, wordsLength, lr, wordCounter + 1, word);
            foundPhrases.insert(foundPhrases.end(),
                std::make_move_iterator(foundPhrasesFromWord.begin()),
                std::make_move_iterator(foundPhrasesFromWord.end()));
        }

        auto dpInsertResult = dp.emplace(lr, std::vector<std::string>(foundPhrases));
        assert(dpInsertResult.second == true);
    }

    std::transform(foundPhrases.begin(), foundPhrases.end(), foundPhrases.begin(),
        [&currentWord](const std::string& s) -> std::string {
            return currentWord + " " + s;
        });
    return foundPhrases;
}

std::vector<std::string> startFindPhrases(const std::vector<std::pair<unsigned int, std::set<std::string>>>& dictionarySets,
    const std::vector<unsigned int>& wordsLength,
    LetterRepository& lr)
{
    std::unordered_map<std::string, std::vector<std::string>> dp;
    std::vector<std::string> foundPhrases;

    if (wordsLength.size() != 0)
    {
        foundPhrases = findPhrases(dp, dictionarySets, wordsLength, lr, 0, "");
        std::transform(foundPhrases.begin(), foundPhrases.end(), foundPhrases.begin(),
            [](std::string& s) -> std::string {
                s.erase(s.begin());
                return s;
            });
    }

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

    auto foundPhrases = startFindPhrases(dictionarySets, lengths_vect, repo);

    for (const auto& s : foundPhrases)
        std::cout << s << std::endl;

    return 0;
}
