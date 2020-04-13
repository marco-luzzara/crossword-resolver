#include "pch.h"
#include "CppUnitTest.h"
#include "../Crossword/src/WordYielder.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WordYielderTest
{
	TEST_CLASS(Iteration)
	{
	public:
		TEST_METHOD(emptyWordsSet_yieldNothing)
		{
			LetterRepository lr{ 'a', 'b' };
			std::set<std::string> words;
			WordYielder wy(lr, words);

			for (auto word : wy)
			{
				Assert::Fail(L"should not enter here");
			}

			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 1 }, { 'b', 1 } });
		}

		TEST_METHOD(noWordsCompatibleWithLetters_yieldNothing)
		{
			LetterRepository lr{ 'a', 't', 'e', 't' };
			std::set<std::string> words{ "test" };
			WordYielder wy(lr, words);

			for (auto word : wy)
			{
				Assert::Fail(L"should not enter here");
			}

			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 't', 2 }, { 'a', 1 }, { 'e', 1 } });
		}

		TEST_METHOD(someAccepted_yieldSome)
		{
			LetterRepository lr{'t', 'e', 't', 'o' };
			std::set<std::string> words{ "tt", "ok" };
			WordYielder wy(lr, words);

			int i = 0;
			for (auto word : wy)
			{
				if (i == 0)
				{
					Assert::AreEqual(word, std::string("tt"));
					Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 't', 0 }, { 'o', 1 }, { 'e', 1 } });
				}
				else
					Assert::Fail(L"should not be executed again");

				i++;
			}

			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 't', 2 }, { 'o', 1 }, { 'e', 1 } });
		}

		TEST_METHOD(allAccepted_yieldAll)
		{
			LetterRepository lr{ 't', 't', 'o' };
			std::set<std::string> words{ "tt", "to" };
			WordYielder wy(lr, words);

			int i = 0;
			for (auto word : wy)
			{
				if (i == 0)
				{
					Assert::AreEqual(word, std::string("to"));
					Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 't', 1 }, { 'o', 0 } });
				}
				else if (i == 1)
				{
					Assert::AreEqual(word, std::string("tt"));
					Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 't', 0 }, { 'o', 1 } });
				}
				else
					Assert::Fail(L"should not be executed again");

				i++;
			}

			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 't', 2 }, { 'o', 1 } });
		}
	};
}
