#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <cstring>
#include <cassert>

using namespace std;

int customMin(int a, int b) {
    return (a < b) ? a : b; //Takes the minimum value between two integers
}

int render(int lineLength, istream& inf, ostream& outf) {
    if (lineLength < 1) {
        return 2; //return 2 for invalid linelength
    }

    char word[180]; //all the words stored
    bool newParagraph = true;
    bool firstWord = true;
    bool largeWord = false;
    int charCount = lineLength;
    bool previousParagraph = false;

    while (inf >> word) {
        if (strcmp(word, "@P@") == 0) { //check for @p@
            if (!newParagraph && !previousParagraph) {
                outf << '\n' << '\n'; //create two new lines
            }
            newParagraph = true;
            firstWord = true;
            charCount = lineLength;
            previousParagraph = true; //for consecutive @p@
        } else {
            char* ptr = word; //create pointer at word
            while (*ptr != '\0') {
                char* end = ptr;
                while (*end != '\0' && *end != '\n' && !isspace(*end) && *end != '-') {
                    end++;
                }

                int wordLength = int(end - ptr); //Calculate the word length

                if (!firstWord && charCount > 0 && !isspace(*ptr) && *ptr != '-' && *ptr != '\0') {
                    if (end - ptr + 1 <= charCount) {
                        outf << ' ';
                        charCount--; //Outfile a space and delete a character count
                    } else {
                        outf << '\n';
                        charCount = lineLength; //Create new line after hitting limit count and if there is not space after it
                    }
                } else if (!firstWord && *ptr != '\0') {
                    outf << '\n';
                    charCount = lineLength; //Create new line after hitting limit count
                }
                
                if (wordLength > lineLength) {
                    while (wordLength > 0) {
                        int cappedWord = customMin(wordLength, charCount); //Create word portions if the word is larger than line length
                        outf.write(ptr, cappedWord);
                        ptr += cappedWord;
                        wordLength -= cappedWord;
                        charCount -= cappedWord;

                        if (wordLength > 0) {
                            outf << '\n';
                            charCount = lineLength; //Reset line length
                        }
                        largeWord = true;
                    }
                } else if (charCount >= wordLength) {
                    outf.write(ptr, wordLength);
                    charCount -= wordLength; //Write the word
                } else {
                    outf << '\n';
                    outf.write(ptr, wordLength);
                    charCount = lineLength - wordLength; //Start new line if the word portion is bigger
                }

                char lastChar = *(end - 1);
                if (lastChar == ':' || lastChar == '.' || lastChar == '!' || lastChar == '?') {
                    if (charCount > 0) {
                        outf << ' '; //Adding spaces when its specified puncuation
                        charCount--;
                    } else {
                        outf << '\n';
                        charCount = lineLength;
                    }
                }

                firstWord = false;
                ptr = end;

                if (*ptr == '-') {
                    if (charCount > 0) {
                        outf << '-'; //Counting hypens as word characters
                        charCount--;
                    } else {
                        outf << '\n';
                        charCount = lineLength;
                    }
                    ptr++;
                }

                if (*ptr == '\n') {
                    outf << '\n'; //Creating new line after new line
                    charCount = lineLength;
                    firstWord = true;
                    newParagraph = true;
                    ptr++;
                }
            }
        }

        newParagraph = false;
    }

    outf << '\n'; //Ending output with new line

    if (largeWord) {
        return 1; //Return this when theres a word bigger than line length
    } else {
        return 0; //Return this wehn there is no line length
    }
}

void testRender(int lineLength, const char input[], const char expectedOutput[], int expectedReturnValue)
{
    istringstream iss(input);
    ostringstream oss;
    ostringstream dummy;
    streambuf* origCout = cout.rdbuf(dummy.rdbuf());
    int retval = render(lineLength, iss, oss);
    cout.rdbuf(origCout);
    if ( ! dummy.str().empty())
        cerr << "WROTE TO COUT INSTEAD OF THIRD PARAMETER FOR: " << input << endl;
    else if (retval != expectedReturnValue)
        cerr << "WRONG RETURN VALUE FOR: " << input << endl;
    else if (retval == 2)
    {
        if ( ! oss.str().empty())
            cerr << "WROTE OUTPUT WHEN LINELENGTH IS " << lineLength << endl;
    }
    else if (oss.str() != expectedOutput)
        cerr << "WRONG RESULT FOR: " << input << endl;
}

int main()
    {
        const char* filepath = "/Users/nagilull/Desktop/data.txt"; // Replace with the actual path

        ifstream infile(filepath);
        if (!infile) {
            perror("Error opening file");
            cerr << "Cannot open " << filepath << "!" << endl;
            return 1;  // Exit the program with an error code
        }

        cout << "Enter maximum line length: ";
        int len;
        cin >> len;
        cin.ignore(10000, '\n');
        int returnCode = render(len, infile, cout);
        cout << "Return code is " << returnCode << endl;
    
            testRender(7, "This\n\t\tis a\ntest\n", "This is\na test\n", 0);
            testRender(8, "  This is a test  \n", "This is\na test\n", 0);
            testRender(6, "Testing it\n", "Testin\ng it\n", 1);
            testRender(-5, "irrelevant", "irrelevant", 2);
            cerr << "Tests complete" << endl;
    }
