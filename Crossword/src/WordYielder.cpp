#include <stdexcept>

#include "../include/WordYielder.h"

WordYielder::WordYielder(LetterRepository& _lr, const std::set<std::string>& _words)
	: lr(_lr), words(_words)
{
}

WordYielder::Iterator WordYielder::begin()
{
	return WordYielder::Iterator(this->words.begin(), this);
}

WordYielder::Iterator WordYielder::end()
{
	return WordYielder::Iterator(this->words.end(), this);
}

WordYielder::Iterator::Iterator(std::set<std::string>::iterator wordIterator, WordYielder* _wy)
	: word_to_process_it(wordIterator), wy(_wy), last_word("")
{
	if (this->word_to_process_it != this->wy->words.end())
		this->updateNextWordIterator();
}

std::string WordYielder::Iterator::operator*() const
{
	return *this->word_to_process_it;
}

WordYielder::Iterator& WordYielder::Iterator::operator++()
{
	this->wy->lr.addString(this->last_word);

	if (this->word_to_process_it != this->wy->words.end())
	{
		this->word_to_process_it++;
		this->updateNextWordIterator();
	}

	return *this;
}

bool WordYielder::Iterator::operator==(const Iterator& it)
{
	return this->word_to_process_it == it.word_to_process_it;
}

bool WordYielder::Iterator::operator!=(const Iterator& it)
{
	return !(*this == it);
}

void WordYielder::Iterator::updateNextWordIterator()
{
	auto& iter = this->word_to_process_it;
	while (iter != this->wy->words.end())
	{
		if (!this->wy->lr.removeString(*iter))
			iter++;
		else
		{
			this->last_word = *iter;
			break;
		}
	}
}