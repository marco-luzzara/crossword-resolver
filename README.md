# crossword-resolver
It tries to find the words of an unknown phrase, given all the letters involved and the lengths of the words in the phrase.

## How to run it
It is a console application and the file to build is `main.cpp`.
After built, execute it using `main.exe input_file.txt`. 

## Input
the `input_file.txt` is a text file in this format.

```
e s t r o t f m e
4 3 2
your_dictionary_file.txt
```

where line:
1. contains all the letters included in the phrase
2. contains the word's lengths
3. specifies a dictionary file, basically a list of words, one for each line, something like [this](https://raw.githubusercontent.com/dwyl/english-words/master/words.txt),

Considering the previous file, one (and probably only) phrase is `test for me`.

## Output
As new phrases are found, they are printed in console.

## Helper For big dictionaries
If your dictionary file surely contains words that can never be put in a possible phrase, like words with a different length than the specified ones, it is recommended to execute the `main_filter_words.cpp` with the same input before. This `main` creates a new text file with only the words that could be possibly be part of the phrase, so they must match with one of the lengths and be composable using the letters in the first line (duplicates counted). This new text file is named `your_dictionary_file.txt` + `filtered.txt`.

