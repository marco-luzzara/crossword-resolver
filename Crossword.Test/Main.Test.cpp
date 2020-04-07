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
	};
}
