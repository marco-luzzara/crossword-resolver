#include <iostream>
#include <fstream>
#include <array>
#include <unordered_set>
#include <string>
#include <sstream>
#include <vector>

#include "include/LetterRepository.h"
#include "include/InputFileReader.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
        throw std::invalid_argument("you must specify the input file name as argument to main function");

    std::ifstream ifs_in = std::ifstream(argv[1], std::ios::in);
    InputFileReader fileReader(ifs_in);
    ifs_in.close();

    auto letters_vect = fileReader.getLetters_vect();
    auto lengths_vect = fileReader.getLengths_vect();
    std::unordered_set<unsigned int> accepted_lengths(lengths_vect.begin(), lengths_vect.end());

    std::string dictionary_filename = fileReader.getDictionaryFileName();

    std::ifstream ifs_dictionary(dictionary_filename, std::ios::in);
    std::ofstream ofs(dictionary_filename + ".filtered.txt", std::ios::out);

    LetterRepository repo(letters_vect);

    while (!ifs_dictionary.eof())
    {
        std::string word = "";
        bool is_word_accepted = true;

        std::getline(ifs_dictionary, word);

        if (accepted_lengths.find(word.length()) == accepted_lengths.end())
            continue;

        repo.restoreInitialRepo();
        for (auto c : word)
        {
            if (!repo.removeLetter(c))
            {
                is_word_accepted = false;
                break;
            }
        }

        if (is_word_accepted)
            ofs << word << std::endl;
    }

    ifs_dictionary.close();
    ofs.close();
}
