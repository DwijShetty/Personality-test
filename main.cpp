// Project 1 Starter Code - Personality Test
// Dwij Shetty UIN:657811511
// The main file uses the functions from the driver.h file and uses them to
// find the most similar character to it and the functions in the main file
// which parses the data in the questions file and the .people file which
// brings everything that we have worked on together.

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <utility>
#include "driver.h"

using namespace std;

// This function is used to split the information in the small.txt and
// large.txt file in order to put different parts of the file in different
// ourvectors like header, names, sequences,etc. It uses the ',' as a
// delimiter and tried to take out contents between each delimiter.
// It uses parameter string s to take in the string with a delimiter
// (string "," in this situation). string del is to recognize the delimiter
// seperating the different values. ourvector<string> &output is a reference
// to save the string values without the delimiter.
void split(const string &s, const string &del, vector<string> &output) {
    unsigned long long start = 0;
    unsigned long long end = s.find(del);
    while (end != -1) {
        output.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    output.push_back(s.substr(start));
}

// This function takes the string and gives it an index which is readable by the
// menu.
map<char, int> readFactors(const string &factStr) {
    map<char, int> factors;
    vector<string> factPairs;
    split(factStr.substr(1), " ", factPairs);
    for (string pairStr : factPairs){
        vector<string> pair;
        split(pairStr,":",pair);
        factors[pair[0][0]] = stoi(pair[1]);

    }
    return factors;
}

// This function gets each line from the question file and splits the question
// and the option values assigned to it.
Question readQuestion(const string &qLine) {
    Question q;
    vector<string> dotSplit;
    split(qLine, ".", dotSplit);
    q.questionText = dotSplit[0];
    q.questionText.push_back('.');
    q.factors = readFactors(dotSplit[1]);
    return q;
}

// This function prints the answer options.
void printQ(const Question &q) {
    cout << "How much do you agree with this statement?" << endl;
    cout << "\"" << q.questionText << "\"" << endl;
    cout << endl;
    cout << "1. Strongly disagree" << endl;
    cout << "2. Disagree" << endl;
    cout << "3. Neutral" << endl;
    cout << "4. Agree" << endl;
    cout << "5. Strongly agree" << endl;
    cout << endl;
    cout << "Enter your answer here (1-5): ";
}

// This function does the same thing as readQuestion but to .people but takes
// the files' ocean value so the question sentence is in the given format and
// save the values for later use.
Person readPerson(const string &qLine) {
    Person p;
    vector<string> dotSplit;
    split(qLine, ".", dotSplit);
    p.name = dotSplit[0];
    p.scores = readFactors(dotSplit[1]);
    return p;
}

// This function was created to see if the values from the struct are displayed
// correctly.
void printP(const Person &p) {
    cout << "<*Person name=" << p.name << " scores={";
    for (auto factor: p.scores) {
        cout << factor.first << ":";
        cout << factor.second << " ";
    }
    cout << "}*>" << endl;
}

// This function prints the test menu from the testName vector.
void printTestMenu(const vector<string> &testNames) {
    for (int i = 0; i < testNames.size(); i++) {
        cout << i + 1 << ". " << testNames[i] << endl;

    }
    cout << "0. To end program." << endl;
    cout << endl;
    cout << "Choose test number (1-" << testNames.size()
         << ", or 0 to end): ";
}

// This function reads the given question file name and saves the lines in
// set<Question> questions.
set<Question> readQuestionFile(const string &fName) {
    string fileLine;
    ifstream tfile;
    tfile.open(fName);

    if (!tfile.is_open()) {
        throw runtime_error("Problem opening file");
    }

    set<Question> questions;
    getline(tfile, fileLine);
    while (!fileLine.empty()) {
        Question q = readQuestion(fileLine);
        questions.insert(q);
//        printQ(q);
        getline(tfile, fileLine);
    }
    return questions;
}

// This function does the same thing as readQuestionFile but with .people
// files.
set<Person> readPeople(const string &pName) {
    string fileLine;
    ifstream tfile;
    tfile.open(pName);

    if (!tfile.is_open()) {
        throw runtime_error("Problem opening file");
    }
    getline(tfile, fileLine);
    set<Person> people;
    while (!fileLine.empty()) {
        Person p = readPerson(fileLine);
        people.insert(p);
        getline(tfile, fileLine);
//        printQ(p);
    }
    return people;
}

// This function takes the .people file name and adds it to ps so that it
// can be read when addressing it and is finally stored in vector<set<Person>>
// tests.
vector<set<Person>> readPeopleFiles(const vector<string> &testNames) {
    vector<set<Person>> tests;
    for (const string &people: testNames) {
        set<Person> ps = readPeople(people + ".people");
        tests.push_back(ps);
    }
    return tests;
}

int main() {
    set<Question> questions = readQuestionFile("questions.txt");
    vector<string> testNames{"BabyAnimals", "Brooklyn99", "Disney", "Hogwarts",
                             "MyersBriggs", "SesameStreet", "StarWars",
                             "Vegetables", "mine"};
    vector<set<Person>> tests = readPeopleFiles(testNames);
    map<Question, int> answers;
    int numQues;
    cout << "Welcome to the Personality Quiz!" << endl;
    cout << endl;
    cout << "Choose number of questions: ";
    cin >> numQues;
    cout << endl;
    for (int i = 0; i < numQues; i++) {
        int answer;
        Question randQ = randomQuestionFrom(questions);
        printQ(randQ);
        cin >> answer;
        answers[randQ] = answer;
        cout << endl;
    }
    while (true) {
        int choice;
        printTestMenu(testNames);
        cin >> choice;
        if (choice == 0) {
            cout << "Goodbye!" << endl;
            break;
        }
        set<Person> test = tests[choice - 1];
        map<char, int> qScore = scoresFrom(answers);
        Person simChar = mostSimilarTo(qScore, test);

        cout << "You got " << simChar.name << "!" << endl;
    }


    return 0;
}
