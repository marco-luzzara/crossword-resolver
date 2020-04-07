#pragma once

#include <map>
#include <vector>
#include <initializer_list>
#include <stack>
#include <string>
#include <functional>

class LetterRepository
{
public:
    LetterRepository();
    LetterRepository(const std::initializer_list<char>& letters);
    LetterRepository(const std::vector<char>& letters);
    template <size_t D>
    LetterRepository(const char(&letters)[D]);

    operator std::string() const;

    ~LetterRepository();

    // calls undoOperation until all adds and removes are rollbacked
    void restoreInitialRepo();

    // return true when letter/string is add/removed, false otherwise
    bool addLetter(char letter);
    bool removeLetter(char letter);

    bool addString(std::string s);
    bool removeString(std::string s);

    // perform the opposite of the last operation (if last is an add, remove is executed)
    // returns false when stack of operation is empty, false otherwise
    bool undoOperation();

    std::map<const char, int> getRepoMap() const { return this->internal_repo; };
protected:
    void initLetterInRepo(char letter);
    bool operateOnString(std::string s, std::function<bool(char)> operation);

    // true when adding, false when removing
    std::stack<std::pair<char, bool>> rollbackOps;
    std::map<const char, int> internal_repo;
    std::map<const char, int> initial_repo;

private:
};

