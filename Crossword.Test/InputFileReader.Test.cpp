#include <vector>
#include <sstream>
#include <string>
#include <iostream> 

#include "pch.h"
#include "CppUnitTest.h"
#include "../Crossword/src/InputFileReader.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InputFileReaderTest
{
	TEST_CLASS(Constructor)
	{
	public:

		void AssertConstructorFromFileContent(const std::string fileContent) 
		{
			Assert::ExpectException<std::invalid_argument>([fileContent]() {
				std::istringstream strs(fileContent);

				InputFileReader ifr(strs);
			});
		}

		TEST_METHOD(emptyFile_throwInvalidArgument)
		{
			AssertConstructorFromFileContent("");
		}

		TEST_METHOD(oneLine_throwInvalidArgument)
		{
			AssertConstructorFromFileContent("a b c");
		}

		TEST_METHOD(twoLine_throwInvalidArgument)
		{
			AssertConstructorFromFileContent("a b c\n4 5 6");
		}

		TEST_METHOD(firstLineContainsNotOnlyLetters_throwInvalidArgument)
		{
			AssertConstructorFromFileContent("a b c _ 3 f A g\n3 5\ntest.txt");
		}

		TEST_METHOD(secondLineContainsNotOnlyNumbers_throwInvalidArgument)
		{
			AssertConstructorFromFileContent("a b c s h e\n2 k 4\ntest.txt");
		}

		TEST_METHOD(totalSumNotEqualToNumLetters_throwInvalidArgument)
		{
			AssertConstructorFromFileContent("a b c s h e\n2 3 4\ntest.txt");
		}

		TEST_METHOD(correctInput_allGettersCorrect)
		{
			std::string fileContent = "a b C s h e\n2 4\ntest.txt";
			std::istringstream strs(fileContent);

			InputFileReader ifr(strs);

			Assert::AreEqual(ifr.getDictionaryFileName(), std::string("test.txt"));
			Assert::IsTrue(ifr.getLengths_vect() == std::vector<unsigned int>({ 2, 4 }));
			Assert::IsTrue(ifr.getLetters_vect() == std::vector<char>({ 'a', 'b', 'C', 's', 'h', 'e' }));
		}
	};
}
