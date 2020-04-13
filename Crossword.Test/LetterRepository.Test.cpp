#include <vector>
#include <sstream>
#include <string>
#include <iostream> 

#include "pch.h"
#include "CppUnitTest.h"
#include "../Crossword/src/LetterRepository.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LetterRepositoryTest
{
	TEST_CLASS(Constructor)
	{
	public:
		TEST_METHOD(emptyList_repoMapEmpty)
		{
			LetterRepository lr;
			Assert::AreEqual(0, (int)lr.getRepoMap().size());
		}

		TEST_METHOD(listWithNoDuplicates_repoMapHasListSize)
		{
			LetterRepository lr({ 'a', 'b', 'c' });
			Assert::AreEqual(3, (int)lr.getRepoMap().size());
			Assert::AreEqual(1, lr.getRepoMap().at('a'));
			Assert::AreEqual(1, lr.getRepoMap().at('b'));
			Assert::AreEqual(1, lr.getRepoMap().at('c'));
		}

		TEST_METHOD(listWithDuplicates_repoMapHasEntryGreaterThanZero)
		{
			const char letters[] = { 'a', 'b', 'b' };
			LetterRepository lr(letters);
			Assert::AreEqual(2, (int)lr.getRepoMap().size());
			Assert::AreEqual(1, lr.getRepoMap().at('a'));
			Assert::AreEqual(2, lr.getRepoMap().at('b'));
		}

		TEST_METHOD(listCapitalizedLetters_toLowerForAll)
		{
			const char letters[] = { 'A', 'B', 'B' };
			LetterRepository lr(letters);
			Assert::AreEqual(2, (int)lr.getRepoMap().size());
			Assert::AreEqual(1, lr.getRepoMap().at('a'));
			Assert::AreEqual(2, lr.getRepoMap().at('b'));
		}
	};

	TEST_CLASS(StringCast)
	{
	public:
		TEST_METHOD(emptyRepo_emptyString)
		{
			LetterRepository lr({ 'a' });
			lr.removeLetter('a');

			Assert::AreEqual(std::string(), (std::string)lr);
		}

		TEST_METHOD(repoWithNoDuplicates_correctString)
		{
			LetterRepository lr({ 'b', 'c', 'a' });
			Assert::AreEqual(std::string("a1b1c1"), (std::string)lr);
		}

		TEST_METHOD(repoWithDuplicates_correctString)
		{
			LetterRepository lr({ 'a', 'c', 'c', 'b' });
			Assert::AreEqual(std::string("a1b1c2"), (std::string)lr);
		}
	};

	TEST_CLASS(restoreInitialRepo)
	{
	public:
		TEST_METHOD(noChange_returnSameRepo)
		{
			LetterRepository lr({ 'a', 'b' });
			auto firstRepo = lr.getRepoMap();

			lr.restoreInitialRepo();

			Assert::IsTrue(firstRepo == lr.getRepoMap());
		}

		TEST_METHOD(lettersChange_returnInitialRepo)
		{
			LetterRepository lr({ 'a', 'b' });
			auto firstRepo = lr.getRepoMap();

			lr.removeLetter('a');
			lr.restoreInitialRepo();

			Assert::IsTrue(firstRepo == lr.getRepoMap());
		}
	};

	TEST_CLASS(addLetter)
	{
	public:
		TEST_METHOD(letterNotExist_returnFalse)
		{
			LetterRepository lr({ 'a', 'b' });

			Assert::IsFalse(lr.addLetter('c'));
			Assert::AreEqual(2, (int)lr.getRepoMap().size());
		}

		TEST_METHOD(reachedMax_returnFalse)
		{
			LetterRepository lr({ 'a', 'b' });

			Assert::IsFalse(lr.addLetter('a'));
			Assert::AreEqual(1, lr.getRepoMap().at('a'));
		}

		TEST_METHOD(letterAdded_returnTrue)
		{
			LetterRepository lr({ 'a', 'a', 'b' });

			Assert::IsTrue(lr.removeLetter('a'));
			Assert::IsTrue(lr.addLetter('a'));
			Assert::AreEqual(2, lr.getRepoMap().at('a'));
		}

		TEST_METHOD(letterCapitalizedAdded_returnTrue)
		{
			LetterRepository lr({ 'a', 'a', 'b' });

			Assert::IsTrue(lr.removeLetter('a'));
			Assert::IsTrue(lr.addLetter('A'));
			Assert::AreEqual(2, lr.getRepoMap().at('a'));
		}
	};

	TEST_CLASS(removeLetter)
	{
	public:
		TEST_METHOD(letterNotExist_returnFalse)
		{
			LetterRepository lr({ 'a', 'b' });

			Assert::IsFalse(lr.removeLetter('c'));
			Assert::AreEqual(2, (int)lr.getRepoMap().size());
		}

		TEST_METHOD(noLetterLeft_returnFalse)
		{
			LetterRepository lr({ 'a', 'b' });

			Assert::IsTrue(lr.removeLetter('a'));
			Assert::IsFalse(lr.removeLetter('a'));
			Assert::AreEqual(0, lr.getRepoMap().at('a'));
		}

		TEST_METHOD(letterRemoved_returnTrue)
		{
			LetterRepository lr({ 'a', 'a', 'b' });

			Assert::IsTrue(lr.removeLetter('a'));
			Assert::AreEqual(1, lr.getRepoMap().at('a'));
		}

		TEST_METHOD(letterCapitalizedRemoved_returnTrue)
		{
			LetterRepository lr({ 'a', 'a', 'b' });

			Assert::IsTrue(lr.removeLetter('A'));
			Assert::AreEqual(1, lr.getRepoMap().at('a'));
		}
	};

	TEST_CLASS(addString)
	{
	public:
		TEST_METHOD(stringEmpty_doNothing)
		{
			LetterRepository lr({ 'a', 'b' });

			auto result = lr.addString("");

			Assert::IsTrue(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 1 }, { 'b', 1 } });
		}

		TEST_METHOD(addedPartialString_returnTrue)
		{
			LetterRepository lr({ 'a', 'b', 'c', 'c', });	
			lr.removeLetter('a');
			lr.removeLetter('c');
			lr.removeLetter('c');

			auto result = lr.addString("ac");

			Assert::IsTrue(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 1 }, { 'b', 1 }, { 'c', 1 } });
		}

		TEST_METHOD(addedDuplicates_returnTrue)
		{
			LetterRepository lr({ 'a', 'a' });
			lr.removeLetter('a');
			lr.removeLetter('a');

			auto result = lr.addString("aa");

			Assert::IsTrue(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 2 } });
		}

		TEST_METHOD(addedNotExistingLetters_returnFalse)
		{
			LetterRepository lr({ 'a', 'b' });
			lr.removeLetter('a');

			auto result = lr.addString("ac");

			Assert::IsFalse(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 0 }, { 'b', 1 } });
		}

		TEST_METHOD(addedCapitalizedLetters_returnTrue)
		{
			LetterRepository lr({ 'a', 'b' });
			lr.removeLetter('a');
			lr.removeLetter('b');

			auto result = lr.addString("AB");

			Assert::IsTrue(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 1 }, { 'b', 1 } });
		}
	};

	TEST_CLASS(removeString)
	{
	public:
		TEST_METHOD(stringEmpty_doNothing)
		{
			LetterRepository lr({ 'a', 'b' });

			auto result = lr.removeString("");

			Assert::IsTrue(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 1 }, { 'b', 1 } });
		}

		TEST_METHOD(removedPartialString_returnTrue)
		{
			LetterRepository lr({ 'a', 'b', 'c', 'c', });

			auto result = lr.removeString("ac");

			Assert::IsTrue(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 0 }, { 'b', 1 }, { 'c', 1 } });
		}

		TEST_METHOD(removedDuplicates_returnTrue)
		{
			LetterRepository lr({ 'a', 'a' });

			auto result = lr.removeString("aa");

			Assert::IsTrue(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 0 } });
		}

		TEST_METHOD(removedNotExistingLetter_returnFalse)
		{
			LetterRepository lr({ 'a', 'b' });

			auto result = lr.removeString("ac");

			Assert::IsFalse(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 1 }, { 'b', 1 } });
		}

		TEST_METHOD(removedCapitalizedLetters_returnTrue)
		{
			LetterRepository lr({ 'a', 'b' });

			auto result = lr.removeString("AB");

			Assert::IsTrue(result);
			Assert::IsTrue(lr.getRepoMap() == std::map<const char, int>{ { 'a', 0 }, { 'b', 0 } });
		}
	};

	TEST_CLASS(undoOperation)
	{
	public:
		TEST_METHOD(noOperationToExecute_returnFalse)
		{
			LetterRepository lr({ 'a', 'b' });

			Assert::IsFalse(lr.undoOperation());
		}

		TEST_METHOD(someOperationRestored_returnTrue)
		{
			LetterRepository lr({ 'a', 'b' });
			lr.removeLetter('a');
			lr.removeLetter('b');

			Assert::IsTrue(lr.undoOperation());

			Assert::AreEqual(0, lr.getRepoMap().at('a'));
			Assert::AreEqual(1, lr.getRepoMap().at('b'));
		}

		TEST_METHOD(allOperationRestored_returnFalseOnlyLast)
		{
			LetterRepository lr({ 'a', 'b' });
			lr.removeLetter('a');
			lr.removeLetter('b');

			Assert::IsTrue(lr.undoOperation());
			Assert::IsTrue(lr.undoOperation());
			Assert::IsFalse(lr.undoOperation());

			Assert::AreEqual(1, lr.getRepoMap().at('a'));
			Assert::AreEqual(1, lr.getRepoMap().at('b'));
		}
	};
}
