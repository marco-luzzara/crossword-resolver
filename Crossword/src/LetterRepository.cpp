#pragma once

#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <functional>
#include <cassert>

#include "../include/LetterRepository.h"

LetterRepository::LetterRepository()
    : LetterRepository(std::vector<char>())
{
}

LetterRepository::LetterRepository(const std::initializer_list<char>& letters)
    : LetterRepository(std::vector<char>(letters))
{
}

LetterRepository::LetterRepository(const std::vector<char>& letters)
{
    for (auto letter : letters)
    {
        this->initLetterInRepo(std::tolower(letter));
    }

    this->initial_repo = this->internal_repo;
}

void LetterRepository::initLetterInRepo(const char letter)
{
    std::pair<std::map<char, int>::iterator, bool> emplace_result = this->internal_repo.emplace(letter, 1);

    if (emplace_result.second == false)
        (emplace_result.first)->second++;
}

template <size_t D>
LetterRepository::LetterRepository(const char(&letters)[D])
    : LetterRepository(std::vector<char>(std::begin(letters), std::end(letters)))
{
}

LetterRepository::operator std::string() const
{
    std::string resultStr;

    for (auto entry : this->internal_repo)
    {
        if (entry.second > 0)
        {
            resultStr += entry.first;
            resultStr += std::to_string(entry.second);
        }
    }

    return resultStr;
}

void LetterRepository::restoreInitialRepo()
{
    this->internal_repo = this->initial_repo;
}

bool LetterRepository::addLetter(char letter)
{
    letter = std::tolower(letter);

    auto foundEntryIt = this->internal_repo.find(letter);
    if (foundEntryIt == this->internal_repo.end() || foundEntryIt->second == this->initial_repo.at(letter))
        return false;

    foundEntryIt->second++;
    this->rollbackOps.push({ letter, false });

    return true;
}

bool LetterRepository::removeLetter(char letter)
{
    letter = std::tolower(letter);

    auto foundEntryIt = this->internal_repo.find(letter);
    if (foundEntryIt == this->internal_repo.end() || foundEntryIt->second == 0)
        return false;

    foundEntryIt->second--;
    this->rollbackOps.push({ letter, true });

    return true;

}

bool LetterRepository::operateOnString(std::string s, std::function<bool(char)> operation)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
    int letterCounter = 0;
    bool isStrRead = true;

    for (char c : s)
    {
        if (operation(c))
        {
            letterCounter++;
        }
        else
        {
            isStrRead = false;
            break;
        }
    }

    if (!isStrRead)
    {
        for (int i = 0; i < letterCounter; i++)
            this->undoOperation();
    }

    return isStrRead;
}

bool LetterRepository::addString(std::string s)
{
    return this->operateOnString(s, [this](char c) { return this->addLetter(c); });
}

bool LetterRepository::removeString(std::string s)
{
    return this->operateOnString(s, [this](char c) { return this->removeLetter(c); });
}

bool LetterRepository::undoOperation()
{
    if (this->rollbackOps.empty())
        return false;

    std::pair<char, bool> rollbackOp = this->rollbackOps.top();
    if (rollbackOp.second)
        this->internal_repo.at(rollbackOp.first)++;
    else
        this->internal_repo.at(rollbackOp.first)--;

    this->rollbackOps.pop();

    return true;
}

LetterRepository::~LetterRepository()
{
    //dtor
}