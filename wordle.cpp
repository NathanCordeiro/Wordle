/// WORDLE GAME

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>  // For rand()
#include <ctime>    // For seeding rand()

#ifdef _WIN32
    #include <windows.h>  // For Windows-specific system calls
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

const int WORD_LENGTH = 5;
const int NOT_MATCHED = 0;
const int PARTIAL_MATCHED = 1;
const int MATCHED = 2;
const int NUMBER_OF_GUESSES = 6;

void toUpperCase(std::string &input) {
    std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c){ return std::toupper(c); });
}

// Add more words here
std::string getRandomWord() {
    std::vector<std::string> wordList = {"READY", "APPLE", "TABLE", "HAPPY", "SHINY"};
    srand(time(0));  // Seed random generator
    return wordList[rand() % wordList.size()];
}

bool isValidWord(std::string word) {
    return word.length() == WORD_LENGTH && word.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos;
}

void markMatches(std::vector<std::vector<int>> &matches, int currentGuessIndex, std::string targetWord, std::string guessWord) {
    std::vector<bool> matchedPositions(WORD_LENGTH, false);

    // First, mark exact matches
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guessWord[i] == targetWord[i]) {
            matches[currentGuessIndex][i] = MATCHED;
            matchedPositions[i] = true;
        } else {
            matches[currentGuessIndex][i] = NOT_MATCHED;
        }
    }

    // Now mark partial matches
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (matches[currentGuessIndex][i] == MATCHED) continue;

        for (int j = 0; j < WORD_LENGTH; j++) {
            if (!matchedPositions[j] && guessWord[i] == targetWord[j]) {
                matches[currentGuessIndex][i] = PARTIAL_MATCHED;
                matchedPositions[j] = true;
                break;
            }
        }
    }
}

bool isAllMatched(std::string targetWord, std::string guessWord) {
    return targetWord == guessWord;
}

void printWordle(std::vector<std::vector<int>> matches, std::vector<std::string> guesses, int currentGuessIndex) {
    system(CLEAR_SCREEN);

    std::cout << "\033[33m" << R"(
         ,-.-.     _,.---._                                              ,----.
,-..-.-./  \==\  ,-.' , -  `.    .-.,.---.    _,..---._     _.-.      ,-.--` , \
|, \=/\=|- |==| /==/_,  ,  - \  /==/  `   \ /==/,   -  \  .-,.'|     |==|-  _.-`
|- |/ |/ , /==/|==|   .=.     ||==|-, .=., ||==|   _   _\|==|, |     |==|   `.-.
 \, ,     _|==||==|_ : ;=:  - ||==|   '='  /|==|  .=.   ||==|- |    /==/_ ,    /
 | -  -  , |==||==| , '='     ||==|- ,   .' |==|,|   | -||==|, |    |==|    .-'
  \  ,  - /==/  \==\ -    ,_ / |==|_  . ,'. |==|  '='   /|==|- `-._ |==|_  ,`-._
  |-  /\ /==/    '.='. -   .'  /==/  /\ ,  )|==|-,   _`/ /==/ - , ,//==/ ,     /
  `--`  `--`       `--`--''    `--`-`--`--' `-.`.____.'  `--`-----' `--`-----``
)" << "\033[0m" << std::endl;

    for (int i = 0; i <= currentGuessIndex && i < guesses.size(); i++) {
        std::string topBorder = "╔═══╦═══╦═══╦═══╦═══╗";
        std::string middleRow = "║ ";
        std::string bottomBorder = "╚═══╩═══╩═══╩═══╩═══╝";

        for (int j = 0; j < WORD_LENGTH; j++) {
            char value = guesses[i][j];

            if (matches[i][j] == PARTIAL_MATCHED) {
                middleRow += "\033[33m";  // Yellow for partial match
            } else if (matches[i][j] == MATCHED) {
                middleRow += "\033[32m";  // Green for correct match
            }

            middleRow += value;
            middleRow += "\033[0m ║ ";  // Reset color and continue box

        }

        std::cout << topBorder << std::endl;
        std::cout << middleRow << std::endl;
        std::cout << bottomBorder << std::endl;
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::vector<std::string> guesses(NUMBER_OF_GUESSES);
    std::vector<std::vector<int>> matches(NUMBER_OF_GUESSES, std::vector<int>(WORD_LENGTH));
    std::string targetWord = getRandomWord();
    toUpperCase(targetWord);

    std::string input;
    int currentGuessCount = 0;

    printWordle(matches, guesses, -1);

    while (currentGuessCount < NUMBER_OF_GUESSES) {
        do {
            std::cout << "\nEnter your guess (" << WORD_LENGTH << " letters) or type 'Q/q' to quit: ";
            std::getline(std::cin, input);
            toUpperCase(input);
        } while (input != "Q" && !isValidWord(input));

        if (input == "Q") {
            std::cout << "😹 You quit the game. The correct word was: " << targetWord << std::endl;
            break;
        }

        guesses[currentGuessCount] = input;
        markMatches(matches, currentGuessCount, targetWord, input);
        printWordle(matches, guesses, currentGuessCount);

        if (isAllMatched(targetWord, input)) {
            std::cout << "😻🎉 Congratulations! You guessed the word: " << targetWord << " 💗" << std::endl;
            break;
        }

        currentGuessCount++;
    }

    if (currentGuessCount == NUMBER_OF_GUESSES) {
        std::cout << "🤡❌💩 Game over! The correct word was: " << targetWord << std::endl;
    }

    return 0;
}
