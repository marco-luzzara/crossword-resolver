#pragma once
#include <set>

#include "LetterRepository.h"

class WordYielder
{
public:
	WordYielder(LetterRepository& lr, const std::set<std::string>& words);

	class Iterator
	{
	public:
		friend class WordYielder;

		std::string operator*() const;
		Iterator& operator++();
		bool operator==(const Iterator& it);
		bool operator!=(const Iterator& it);

	protected:
		Iterator(std::set<std::string>::iterator wordIterator, WordYielder* _wy);
		void updateNextWordIterator();

		WordYielder* wy;
		std::string last_word;
		std::set<std::string>::iterator word_to_process_it;
	};

	Iterator begin();
	Iterator end();

protected:
	LetterRepository& lr;
	const std::set<std::string>& words;
};

