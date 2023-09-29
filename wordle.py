# The text file I used is from this website: https://gist.github.com/cfreshman/a7b776506c73284511034e63af1017ee

import copy
import itertools

with open("C:\\Users\\Tea\\Documents\\Code\\wordle\\words.txt") as f:
    possibleAnswers = f.readlines()
for x in range(len(possibleAnswers)):
    possibleAnswers[x] = possibleAnswers[x].strip()
allWords = set(possibleAnswers)


class Word:
    def __init__(self, letters, letterVal):
        self.letters = letters
        self.letterVal = letterVal


class Letter:
    def __init__(self, letter, count):
        self.letter = letter
        self.count = count


class AnswerWord:
    def __init__(self):
        self.trueLetters = ['~', '~', '~', '~', '~']
        self.yellowLetters = []

    def updateLetters(self, wordObj):
        for index in range(5):
            if wordObj.letterVal[index] == 2:
                self.trueLetters[index] = wordObj.letters[index]
            if wordObj.letterVal[index] == 1:
                self.yellowLetters.append(wordObj.letters[index])
        # Checks if you win
        if '~' not in self.trueLetters:
            print("Congrats, you solved the Wordle! (I mean, technically it wasn't you, but good job anyways)")
            exit()


answer = AnswerWord()


def canBeWord(wordObj, testWord):
    # Check wordObj letters to see if it can be the answer based on the current information
    for index in range(5):
        if wordObj.letterVal[index] == 0:
            if testWord.find(wordObj.letters[index]) != -1:
                return False
        elif wordObj.letterVal[index] == 1:
            if testWord.find(wordObj.letters[index]) == -1 or testWord[index] == wordObj.letters[index]:
                return False
        elif wordObj.letterVal[index] == 2:
            if testWord[index] != wordObj.letters[index]:
                return False

    return True


def reducePossibleAnswers(wordObj, wordList):
    # Loop through indexes of wordList from end to start, if a word in list can't be the answer, remove it from possible
    # answers list
    possibleWords = []
    for index in range(len(wordList)):
        if canBeWord(wordObj, wordList[index]):
            possibleWords.append(wordList[index])

    return possibleWords


def wordStrength(wordObj, possibleAnswers):
    # Checks the number of words left if all letters input are gray, thereby determining strength of that word
    strengthOfWord = 0
    for word in possibleAnswers:
        tmpWordObj = copy.copy(wordObj)
        for let in answer.trueLetters:
            tmpWordObj.letters = tmpWordObj.letters.replace(let, "-")
        if not canBeWord(tmpWordObj, word):
            strengthOfWord += 1

    return strengthOfWord


def wordsWithLetters(letters):
    # Checks the words that can be created with a given list of letters
    validWords = []
    for word in allWords:
        hasLetters = True
        for letter in letters:
            if word.find(letter) != -1:
                continue
            hasLetters = False
            break
        if hasLetters:
            validWords.append(word)

    return validWords


def findOtherLetters(possibleAnswers):
    # Gets the
    lettersList = []
    for word in possibleAnswers:
        lettersOfWord = []
        for index in range(5):
            if not (word[index] in answer.trueLetters or word[index] in answer.yellowLetters):
                lettersOfWord.append(word[index])
        lettersList.append(lettersOfWord)

    return lettersList


def countOtherLetters(lettersList):
    for word in lettersList:
        for letter in word:
            if word.count(letter) > 1:
                word.remove(letter)
    letterObjList = []
    for word in lettersList:
        for letter in word:
            letterIsThere = False
            for index in range(len(letterObjList)):
                if letterObjList[index].letter != letter:
                    continue
                letterIsThere = True
                letterObjList[index].count += 1
                break
            if not letterIsThere:
                letterObjList.append(Letter(letter, 1))
    letterObjList.sort(key=lambda x: x.count, reverse=True)

    return letterObjList


def getBestWord(possibleAnswers):
    if len(possibleAnswers) <= 2:
        return possibleAnswers[0]
    leftoverLetters = findOtherLetters(possibleAnswers)
    letterObjList = countOtherLetters(leftoverLetters)
    justLetters = []
    for obj in letterObjList:
        justLetters.append(obj.letter)

    actualAllCombs = []
    possibleWords = []

    for combSize in reversed(range(1, 6)):
        if combSize > len(justLetters):
            continue
        allCombs = list(itertools.combinations(justLetters, combSize))
        for indComb in allCombs:
            actualAllCombs.append(list(indComb))
        for comb in actualAllCombs:
            possibleWords.append(wordsWithLetters(comb))
        possibleWords = list(filter(None, possibleWords))
        cleanPossibleWords = []
        for groupOfWordsIndex in range(len(possibleWords)):
            cleanPossibleWords += possibleWords[groupOfWordsIndex]
        if len(cleanPossibleWords) > 0:
            break
    strengthList = []
    for word in cleanPossibleWords:
        strengthList.append(wordStrength(Word(word, [0, 0, 0, 0, 0]), possibleAnswers))

    return cleanPossibleWords[strengthList.index(max(strengthList))]


print("Wordle Solver\n\n"
      "Instructions: This program will automatically determine the word that will give you the most information. (At \n"
      "least, ideally). When prompted, enter a five letter word of your choice, and do the same in the actual wordle \n"
      "game/website. Afterwards, enter the results of the guess for each letter into this program. For each letter, \n"
      "enter either '0', '1', or '2'. '0' means that letter is gray, '1' means that letter is yellow, and '2' means \n"
      "that letter is green. The program will then display all possible answers based on those results. Repeat steps \n"
      "until you win!\n\n\n")

print("Do you want to skip generating the first best word? (It's raise) y/n")
skipFirst = input()
if skipFirst == "n":
    print("\nGenerating first word... (This will take about 40 seconds)\n")
    print("Suggested best word: ", getBestWord(possibleAnswers), "\n")
else:
    print("Skipped generating first word\n")

for wordNumber in range(6):
    while True:
        print("Enter your word: ", end="")
        userWord = input()
        if len(userWord) != 5:
            print("That's not five letters. Do it again.\n")
            continue
#        elif userWord not in allWords:
#            print("That's not a valid word.\n")
#            continue
        else:
            break

    badLetters = True
    while badLetters:
        badLetters = False
        userLetters = []
        for letter in userWord:
            print("Enter the result for the letter -", letter, ": ", end="")
            userLetters.append(int(input()))
        for val in userLetters:
            if val != 0 and val != 1 and val != 2:
                print("\nOne of those values weren't correct. Do it again.\n")
                badLetters = True

    print("\n\n")
    userWordObject = Word(userWord, userLetters)
    answer.updateLetters(userWordObject)
    possibleAnswers = reducePossibleAnswers(userWordObject, possibleAnswers)
    print("Possible words: ", possibleAnswers, "\n")
    print("Suggested best word: ", getBestWord(possibleAnswers), "\n")
