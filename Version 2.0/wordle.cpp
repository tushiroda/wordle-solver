#include <iostream>
#include <fstream>
#include "wordle.h"
#include <chrono>

using namespace std;

int main()
{
   // intro
   cout << "Wordle Solver v2.0\nThis program will suggest a few good words to guess."
        << " Pick one (or not) and input it into the wordle website. After it shows "
        << "the colors for each letter, input the same word into the program and inp"
        << "ut a sequence of 5 numbers based on the colors. 2 means the letter was g"
        << "reen, 1 means yellow, and 0 means gray.\n"
        << "Example: You input 'above' and the colors are green, gray, gray, yellow,"
        << " gray. In the program, input '20010'\n\n";

   // read in words from file to list of words "answers/"
   fstream ans;
   ans.open("answers.txt");
   vector<string> answers;
   string word;

   if (ans.is_open())
   {
      while (ans >> word)
      {
         answers.push_back(word);
      }
   }
   else
   {
      cout << "Couldn't open answers.txt";
      return 0;
   }

   fstream all;
   all.open("allWords.txt");
   vector<string> allWords;
   if (all.is_open())
   {
      while (all >> word)
      {
         allWords.push_back(word);
      }
   }
   else
   {
      cout << "Couldn't open allWords.txt";
      return 0;
   }

   auto t1 = chrono::high_resolution_clock::now();
   vector<pair<int, string>> wordStrengthsList = getWordStrengthsList(allWords, answers);
   auto t2 = chrono::high_resolution_clock::now();
   auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
   cout << "Built list in " << ms_int.count() << "ms\n\n";

   // repeat for each guess
   for (int x = 0; x < 6; x++)
   {
      if (answers.size() < 1)
      {
         if (answers.size() == 1)
         {
            cout << "The answer is: " << answers[0];
            cout << "\nCongrats, you win!";
         }
         else
         {
            cout << "No answers left, likely due to input error. Rerun the program and check your inputs.\n";
            cout << "If you're not on the official wordle website, this program may not be able to solve very answer.";
         }
         return 0;
      }
      if (wordStrengthsList[0].first != 0)
      {
         cout << "Good words (bigger number means better)\n";
         for (int x = 0; x < 5; x++)
         {
            cout << wordStrengthsList[x].second << ": " << wordStrengthsList[x].first << "\n";
         }
      }

      if (answers.size() <= 2)
      {
         cout << "Suggested Word: " << answers[0] << "\n\n";
      }
      else
         cout << "Suggested Word: " << wordStrengthsList[0].second << "\n\n";
      string userInput;
      do
      {
         cout << "Enter a 5 letter word: ";
         cin >> userInput;
         if (userInput.size() != 5 || !isalpha(userInput[0]))
         {
            cout << "\nWord has to be length 5 and only letters\n";
         }
      } while (userInput.size() != 5 || !isalpha(userInput[0]));

      pair<char, int> input[5] = {{'-', 0}, {'-', 0}, {'-', 0}, {'-', 0}, {'-', 0}};
      string userNums;
      do
      {
         cout << "Enter 5 numbers, one for each letter value: ";
         cin >> userNums;
         for (int x = 0; x < 5; x++)
         {
            input[x].first = userInput[x];
            input[x].second = stoi(userNums.substr(x, 1));
         }
         if (!isdigit(userNums[0]) || userNums.size() != 5)
         {
            cout << "Input should be numeric and length 5\n";
         }
         cout << "\n";
      } while (!isdigit(userNums[0]) || userNums.size() != 5);

      updateAnswers(answers, input);
      wordStrengthsList = getWordStrengthsList(allWords, answers);

      cout << "\n\nPossible answers: ";
      for (string s : answers)
      {
         cout << s << "\t";
      }
      cout << "\n\n";
   }
}