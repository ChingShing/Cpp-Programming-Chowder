/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Start code from Standford CS106X (Authum 2015) Assignment 2
 * Including all rewritten code for the assignment
 * Rewritten by Ching_Shing, junior student in SJTU
 * CS106B/X C++ style guideline adopted (ref:http://stanford.edu/class/archive/cs/cs106b/cs106b.1158/styleguide.shtml)
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
#include "console.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim

using namespace std;

static const string kGrammarsDirectory = "grammars/";
static const string kGrammarFileExtension = ".g";

static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
        if (filename.empty() || isValidGrammarFilename(filename)) return filename;
        cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;
    }
}

int main() {
    while (true) {
        string filename = getFileName();
        if (filename.empty()) break;
        cout << "Here's where you read in the \"" << filename << "\" grammar "
             << "and generate three random sentences." << endl;
    }
    
    cout << "Thanks for playing!" << endl;
    return 0;
}
