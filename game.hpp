#include <stdint.h>

struct gameState
{
    uint8_t correctNumber = 0;
    uint8_t lastGuess = 0;
    bool lastGuessMore = true;
    bool hasNumberBeenGuessed = false;
};

bool isNumberGuessed(gameState& gs)
{

    if (gs.correctNumber == gs.lastGuess)
    {
        return true;
    }
    
    return false;
}

bool isNumberMore(gameState& gs)
{
    if (gs.correctNumber > gs.lastGuess)
    {
        return true;
    }
    
    return false;
}

void printClue(gameState& gs)
{
    if(gs.hasNumberBeenGuessed == false)
    {
        if (gs.lastGuessMore == true)
        {
            printf("\nNumber is more than %d\n", gs.lastGuess);
        }
        else
        {
            printf("\nNumber is less than %d\n", gs.lastGuess);
        }
    }
}

void printstate(gameState& gs)
{
    printf("%d\n", gs.correctNumber);
    printf("%d\n", gs.lastGuess);
    printf("%d\n", gs.lastGuessMore);
    printf("%d\n", gs.hasNumberBeenGuessed);
}

uint8_t fixedInput()
{
    std::cout << "Enter guess: ";
    int temp;
    std::cin >> temp;
    return (uint8_t)temp;
}