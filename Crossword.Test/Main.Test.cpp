#include <vector>
#include <sstream>
#include <string>
#include <iostream> 

#include "pch.h"
#include "CppUnitTest.h"
#include "../Crossword/main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MainTest
{
	TEST_CLASS(ReadDictionaryFile)
	{
	public:
		
		TEST_METHOD(lengthEmpty_returnEmpty)
		{
			std::vector<unsigned int> vect{ };
			std::string s = "a\nab";
			std::istringstream strs(s);

			auto res = readDictionaryFile(strs, vect);

			Assert::AreEqual(0, (int)res.size());
		}

		TEST_METHOD(noWordWithSpecifiedLength_setEmpty)
		{
			std::vector<unsigned int> vect{ 1, 2, 3, 4 };
			std::string s = "aaaaa\nbbbbbb";
			std::istringstream strs(s);

			auto res = readDictionaryFile(strs, vect);

			Assert::IsTrue(res == std::vector<std::pair<unsigned int, std::set<std::string>>>{ {1, {}}, { 2, {} }, { 3, {} }, { 4, {} }});
		}

		TEST_METHOD(multipleWordWithDifferentLengths_someSetsAreFilled)
		{
			std::vector<unsigned int> vect{ 1, 3, 4 };
			std::string s = "aaaaa\nabcd\n123\n345";
			std::istringstream strs(s);

			auto res = readDictionaryFile(strs, vect);

			Assert::IsTrue(res == std::vector<std::pair<unsigned int, std::set<std::string>>>{ {1, {}}, { 3, {"123", "345"} }, { 4, {"abcd"} }});
		}

		TEST_METHOD(duplicateLength_takenUnique)
		{
			std::vector<unsigned int> vect{ 1, 3, 4, 3 };
			std::string s = "aaaaa\nabcd\n123\n345";
			std::istringstream strs(s);

			auto res = readDictionaryFile(strs, vect);

			Assert::IsTrue(res == std::vector<std::pair<unsigned int, std::set<std::string>>>{ {1, {}}, { 3, {"123", "345"} }, { 4, {"abcd"} }});
		}
	};

	TEST_CLASS(StartFindPhrases)
	{
	public:
		TEST_METHOD(noWordsInDictionarySet_returnEmpty)
		{
			std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets;
			std::vector<unsigned int> wordsLength{ 4 };
			LetterRepository lr{ 't', 'e', 's', 't' };

			auto foundPhrases = startFindPhrases(dictionarySets, wordsLength, lr);

			Assert::IsTrue(foundPhrases == std::vector<std::string>());
		}

		TEST_METHOD(notEnoughLetterInRepo_returnEmpty)
		{
			std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets{ { 4, { "test" } } };
			std::vector<unsigned int> wordsLength{ 4 };
			LetterRepository lr{ 't', 'e', 's' };

			auto foundPhrases = startFindPhrases(dictionarySets, wordsLength, lr);

			Assert::IsTrue(foundPhrases == std::vector<std::string>());
		}

		TEST_METHOD(wordsLengthEmpty_returnEmpty)
		{
			std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets{ { 4, { "test" } } };
			std::vector<unsigned int> wordsLength;
			LetterRepository lr{ 't', 'e', 's', 't' };

			auto foundPhrases = startFindPhrases(dictionarySets, wordsLength, lr);

			Assert::IsTrue(foundPhrases == std::vector<std::string>());
		}

		TEST_METHOD(letterRepositoryEmpty_returnEmpty)
		{
			std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets{ { 4, { "test" } } };
			std::vector<unsigned int> wordsLength{ 4 };
			LetterRepository lr;

			auto foundPhrases = startFindPhrases(dictionarySets, wordsLength, lr);

			Assert::IsTrue(foundPhrases == std::vector<std::string>());
		}

		TEST_METHOD(oneWord_returnThatWord)
		{
			std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets{ { 4, { "test" } } };
			std::vector<unsigned int> wordsLength{ 4 };
			LetterRepository lr{ 't', 'e', 's', 't' };

			auto foundPhrases = startFindPhrases(dictionarySets, wordsLength, lr);

			Assert::IsTrue(foundPhrases == std::vector<std::string>{ "test" });
		}

		TEST_METHOD(permutationsOfSingleWord_returnPermutations)
		{
			std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets{ { 4, { "test", "estt" } } };
			std::vector<unsigned int> wordsLength{ 4 };
			LetterRepository lr{ 't', 'e', 's', 't' };

			auto foundPhrases = startFindPhrases(dictionarySets, wordsLength, lr);

			Assert::IsTrue(foundPhrases == std::vector<std::string>{ "estt", "test" });
		}

		TEST_METHOD(permutationsOfMultipleWordWithDP_returnPermutations)
		{
			std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets{ { 4, { "test", "estt" } }, { 2, { "ok", "ko" } } };
			std::vector<unsigned int> wordsLength{ 4, 2 };
			LetterRepository lr{ 't', 'e', 's', 't', 'o', 'k' };

			auto foundPhrases = startFindPhrases(dictionarySets, wordsLength, lr);

			Assert::IsTrue(foundPhrases == std::vector<std::string>{ "estt ko", "estt ok", "test ko", "test ok"  });
		}

		TEST_METHOD(permutationsOfMultipleWordWithoutDP_returnPermutations)
		{
			std::vector<std::pair<unsigned int, std::set<std::string>>> dictionarySets{ { 4, { "teso", "estk" } }, { 2, { "ot", "kt", "tk", "to" } } };
			std::vector<unsigned int> wordsLength{ 4, 2 };
			LetterRepository lr{ 't', 'e', 's', 't', 'o', 'k' };

			auto foundPhrases = startFindPhrases(dictionarySets, wordsLength, lr);

			Assert::IsTrue(foundPhrases == std::vector<std::string>{ "estk ot", "estk to", "teso kt", "teso tk"  });
		}
	};
}
