#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

vector<char> knownLetters;

map<char, int> getLetterWeights(vector<string> answers)
{
   map<char, int> letterCount = {
       {'a', 0},
       {'b', 0},
       {'c', 0},
       {'d', 0},
       {'e', 0},
       {'f', 0},
       {'g', 0},
       {'h', 0},
       {'i', 0},
       {'j', 0},
       {'k', 0},
       {'l', 0},
       {'m', 0},
       {'n', 0},
       {'o', 0},
       {'p', 0},
       {'q', 0},
       {'r', 0},
       {'s', 0},
       {'t', 0},
       {'u', 0},
       {'v', 0},
       {'w', 0},
       {'x', 0},
       {'y', 0},
       {'z', 0}};
   for (string ans : answers)
   {
      // use a map to only add unique letters
      map<char, int> uWord;
      for (char letter : ans)
      {
         uWord[letter]++;
      }
      for (auto let : uWord)
      {
         letterCount[let.first]++;
      }
   }
   // now weigh the vowels differently?
   letterCount['a'] *= 0.7;
   letterCount['e'] *= 0.7;
   letterCount['i'] *= 0.7;
   letterCount['o'] *= 0.7;
   letterCount['u'] *= 0.7;

   for (auto pair : letterCount)
   {
      if (pair.second == answers.size())
         letterCount[pair.first] = 0;
   }

   for (char let : knownLetters)
   {
      letterCount[let] *= 0.3;
   }

   return letterCount;
}

int getWordStrength(string word, map<char, int> letterWeights)
{
   // try to iterate over every character in every word (like old python) and sort of get total value
   int sum = 0;
   map<char, int> uWord;
   for (char c : word)
   {
      uWord[c];
   }

   for (auto c : uWord)
   {
      sum += letterWeights[c.first];
   }
   return sum;
}

vector<pair<int, string>> getWordStrengthsList(vector<string> &allWords, vector<string> &answers)
{
   map<char, int> letterWeights = getLetterWeights(answers);
   map<string, int> wordStrength;
   for (string word : allWords)
   {
      wordStrength[word] = getWordStrength(word, letterWeights);
   }

   vector<pair<int, string>> wordStrengthsList;
   for (auto pair : wordStrength)
   {
      wordStrengthsList.push_back(make_pair(pair.second, pair.first));
   }
   sort(wordStrengthsList.begin(), wordStrengthsList.end(), greater());

   return wordStrengthsList;
}

bool wordCanBeAns(string word, pair<char, int> input[5])
{
   for (int x = 0; x < 5; x++)
   {
      if (input[x].second == 0)
      {
         if (word.find(input[x].first) != string::npos)
         {
            return false;
         }
      }
      else if (input[x].second == 1)
      {
         if (word.find(input[x].first) == string::npos || word[x] == input[x].first)
         {
            return false;
         }
      }
      else if (input[x].second == 2)
      {
         if (word[x] != input[x].first)
         {
            return false;
         }
      }
   }

   return true;
}

void updateAnswers(vector<string> &answers, pair<char, int> input[5])
{
   for (int x = answers.size() - 1; x >= 0; x--)
   {
      if (!wordCanBeAns(answers[x], input))
      {
         answers.erase(answers.begin() + x);
      }
   }

   // add letters with val 2 to knowLetters to exclude them from getting best words
   for (int x = 0; x < 5; x++)
   {
      if (input[x].second == 2 || input[x].second == 1)
      {
         knownLetters.push_back(input[x].first);
      }
   }
}