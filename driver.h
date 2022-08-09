// Project 1 Starter Code - Personality Test
// Dwij Shetty UIN:657811511
// This file has functions that can be used to help process the values inputed
// from the user and the provided values in the .people file and helps find
// which character is the most similar to the user. These functions are plugged
// in main.cpp

#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include <utility>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator<(const Question &lhs, const Question &rhs) {
        return lhs.questionText < rhs.questionText;
    }

    friend bool operator==(const Question &lhs, const Question &rhs) {
        return lhs.questionText == rhs.questionText;
    }

    friend bool operator!=(const Question &lhs, const Question &rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator<(const Person &lhs, const Person &rhs) {
        return lhs.name < rhs.name;
    }

    friend bool operator==(const Person &lhs, const Person &rhs) {
        return lhs.name == rhs.name;
    }

    friend bool operator!=(const Person &lhs, const Person &rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question> &questions) {
    int ind = randomInteger(0, (int) questions.size() - 1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

// It takes a random integer from randomElement and uses it to select a questio
// n from the question set.
Question randomQuestionFrom(set<Question> &questions) {
    if (questions.empty()) {
        throw runtime_error("Question is empty");
    }
    Question randQuest = randomElement(questions);
    questions.erase(randQuest);
    return randQuest;
}

// It takes the vales of the answers and gives us the value of the scores by mu
// ltiplying it with respective multiplier.
map<char, int> scoresFrom(map<Question, int> &answers) {
    map<char, int> scores;
    for (const pair<const Question, int> &row : answers) {
        const Question &q = row.first;
        int a = row.second;
        int mult = a - 3;
        for (const pair<const char, int> &factor : q.factors) {
            int score = factor.second * mult;
            if (scores.count(factor.first) == 0) {
                scores[factor.first] = 0;
            }
            scores[factor.first] += score;
        }
    }
    return scores;
}

// This function takes the score from the previous function and applies the
// given values to a formula to normalize the scores of different people no
// matter the sample size.
map<char, double> normalize(const map<char, int> &scores) {
    long sumSquare = 0;
    for (pair<const char, int> score : scores) {
        int num = score.second;
        long square = num * num;
        sumSquare += square;
    }

    if (sumSquare == 0) {
        throw runtime_error("All scores must not be zero.");
    }

    double mag = sqrt(sumSquare);
    map<char, double> normScores;
    for (pair<const char, int> counts : scores) {
        int num = counts.second;
        double norm = num / mag;
        normScores[counts.first] = norm;
    }
    return normScores;
}

// This function takes the normalized values of the user and the values of
// of the characters in every .people file and creates a number from -1 to 1
// which tells us how similar they are.
double cosineSimilarityOf(const map<char, double> &lhs,
                          const map<char, double> &rhs) {
    double result = 0;
    for (pair<const char, double> l : lhs) {
        double lScore = l.second;
        char lKey = l.first;
        for (pair<const char, double> r : rhs) {
            double rScore = r.second;
            char rKey = r.first;
            if (lKey == rKey) {
                double totScore = lScore * rScore;
                result += totScore;
            }
        }
    }
    return result;
}

// This function basically uses all the functions mentioned above to take
// by using normalize score function and then the cosineSimilarity value and
// takes the closest value to determine which person from the .people
// file will the value be closest to.
Person mostSimilarTo(map<char, int> &scores, set<Person> &people) {
    map<char, double> rhs = normalize(scores);
    double hiScore = -100;
    Person bestP;

    if (people.empty()) {
        throw runtime_error("People set is empty");
    }

    for (const Person &p : people) {
        map<char, int> pScores = p.scores;
        map<char, double> lhs = normalize(p.scores);
        double sim = cosineSimilarityOf(lhs, rhs);
            if (sim > hiScore) {
            hiScore = sim;
            bestP = p;
        }
    }
    return bestP;
}
