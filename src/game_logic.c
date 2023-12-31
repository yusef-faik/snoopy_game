//
// Created by Youssef Faik on 1/2/2024.
//

#include "../include/game_logic.h"

// Constants and Macros
// Keyboard key numbers
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define ENTER_KEY 13

// Matrix characters
#define EMPTY ' '
#define SNOOPY 'S'
#define BIRD 'B'
#define BALL 'O'
#define INVINCIBLE_BLOC '#'
#define PUSHABLE_BLOC 'P'
#define BREAKABLE_BLOC 'C'
#define TRAPPED_BLOC 'T'

// Colors
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"

// Symbols
#define BIRD_SYMBOL "♫"
#define SNOOPY_SYMBOL "☺"
#define BALL_SYMBOL "♂"
#define STAR_SYMBOL "★"
#define INVINCIBLE_BLOC_SYMBOL "☼"
#define PUSHABLE_BLOC_SYMBOL "▬"
#define BREAKABLE_BLOC_SYMBOL "♠"
#define TRAPPED_BLOC_SYMBOL "♣"

#define UP_ARROW_SYMBOL "↑"
#define DOWN_ARROW_SYMBOL "↓"
#define LEFT_ARROW_SYMBOL "←"
#define RIGHT_ARROW_SYMBOL "→"

#define DEFAULT_GAME_DURATION 60

// main functions implementations
void runGameApp() {
    int globalScore = 0;
    int highestScore = readHighestScore();
    int selectedOption;

    do {
        displayWelcomeBanner();
        selectedOption = displayMenuOption();

        switch (selectedOption) {
            case 0:
                displayGameRules();
                break;
            case 1:
                startGame(1, &globalScore, &highestScore);
                break;
            case 2:
                loadSavedGame();
                break;
            case 3:
                displayLevels(&globalScore, &highestScore);
                break;
            case 4:
                displayRecordedScores(highestScore);
                break;
            case 5:
                displayGameControls();
                break;
            case 6:
                quitGame();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (selectedOption != 6);
}

void displayGameRules() {
    clearScreen();

    printf("Game rules\n\n");
    printf("Welcome to Snoopy!\n\n");
    printf("The rules are as follows:\n");
    printf("- Use arrow keys to move Snoopy.\n");
    printf("- Collect all the birds to advance to the next level.\n");
    printf("- Avoid the ball and don't get hit.\n");
    printf("- 120 seconds for each level.\n");
    printf("\nPress Any Key To Go Back to The Menu Screen...");

    waitForKeyHit();
}

void startGame(int level, int *globalScore, int *highestScore) {
    int remainingLives = 3;

    while (remainingLives > 0) {
        enum LeveLResult isLevelWon = playLevel(level, globalScore, highestScore, remainingLives);

        if (isLevelWon == LOST) {
            remainingLives--;
        } else if (isLevelWon == WON) {
            if (level == AVAILABLE_LEVELS) {
                moveCursor(0, 17);

                printf(ANSI_COLOR_YELLOW"★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n");
                printf(STAR_SYMBOL ANSI_COLOR_RESET " YOU HAVE SUCCESSFULLY COMPLETED ALL LEVELS. YOU ARE A GAMING MASTER! " ANSI_COLOR_YELLOW STAR_SYMBOL);
                printf("\n★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n\n"ANSI_COLOR_RESET);

                printf("Press \"enter\" to Go Back to The Menu Screen....");
                while ((int) waitForKeyHit() != ENTER_KEY);

                return;
            }

            level++;
        } else {
            break;
        }
    }

    printf("\nPress Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayGameControls() {
    clearScreen();
    printf(ANSI_COLOR_GREEN".......................Controls.......................\n");
    printf(ANSI_COLOR_RESET"Movement:\n");
    printf("  ↑ : Move Up\n");
    printf("  ↓ : Move Down\n");
    printf("  ← : Move Left\n");
    printf("  → : Move Right\n");
    printf("\n");
    printf("Game Actions:\n");
    printf("  P : Pause/Unpause the game\n");
    printf("  Special keys to interact with blocks\n");
    printf("\n");
    printf("Winning Conditions:\n");
    printf("  - Collect all 4 birds without getting hit by the ball\n");
    printf("  - Finish the level within 120 seconds.\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayRecordedScores(int highestScore) {
    clearScreen();
    printf("Recorded highest Score\n\n");

    printf(ANSI_COLOR_YELLOW STAR_SYMBOL " Current highest Score : "ANSI_COLOR_RESET "%d\n", highestScore);

    printf("\nPress Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void displayLevels(int *globalScore, int *highestScore) {
    clearScreen();
    printf("Levels\n\n");

    char input[20];

    while (1) {
        displayAvailableLevels();

        printf("Enter the level number (or 'q' to quit): ");
        scanf("%s", input);

        if (input[0] == 'q' || input[0] == 'Q') {
            break;
        }

        int level = atoi(input);

        if (isLevelAvailable(level)) {
            char password[20];

            while (1) {
                printf("Enter the password for level %d (or 'q' to quit): ", level);
                scanf("%s", password);

                if (password[0] == 'q' || password[0] == 'Q') {
                    printf("\nQuitting the game. Press Any Key To Go Back to The Menu Screen...");
                    waitForKeyHit();
                    return;
                }

                if (validatePassword(level, password)) {
                    printf(ANSI_COLOR_GREEN"Password correct!  Press Any Key To Start...\n"ANSI_COLOR_RESET);
                    waitForKeyHit();
                    startGame(level, globalScore, highestScore);
                    return;
                } else {
                    printf(ANSI_COLOR_RED"Wrong password. Please try again.\n\n"ANSI_COLOR_RESET);
                }
            }
        } else {
            printf(ANSI_COLOR_RED "Invalid level. Please choose a valid level.\n\n"ANSI_COLOR_RESET);
        }
    }

    printf("\nQuitting the game. Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void loadSavedGame() {
    clearScreen();
    printf("Load Saved Game\n\n");
    printf("Press Any Key To Go Back to The Menu Screen...");
    waitForKeyHit();
}

void quitGame() {
    clearScreen();
    printf("Press Any Key To Go Back to The close the game...");
    waitForKeyHit();
}


// helper functions implementations
void displayWelcomeBanner() {
    clearScreen();
    printf(ANSI_COLOR_YELLOW "★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n"ANSI_COLOR_RESET);
    printf("   _____                                         \n");
    printf("  / ___/ ____   ____   ____   ____   __  __      \n");
    printf("  \\__ \\ / __ \\ / __ \\ / __ \\ / __ \\ / / / /\n");
    printf(" ___/ // / / // /_/ // /_/ // /_/ // /_/ /       \n");
    printf("/____//_/ /_/ \\____/ \\____// .___/ \\__, /     \n");
    printf("                          /_/     /____/         \n");
    printf(ANSI_COLOR_YELLOW "★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n"ANSI_COLOR_RESET);
}

void clearScreen() {
    system("cls");
}

int displayMenuOption() {
    int x = 4, y = 10;
    int yStart = y;
    int selectedOption;

    moveCursor(x, y++);
    printf("- Game rules\n");
    moveCursor(x, y++);
    printf("- Start game \n");
    moveCursor(x, y++);
    printf("- Load Saved Game\n");
    moveCursor(x, y++);
    printf("- Levels \n");
    moveCursor(x, y++);
    printf("- Recorded Scores  \n");
    moveCursor(x, y++);
    printf("- Game Controls \n");
    moveCursor(x, y++);
    printf("- Exit\n");
    moveCursor(x, y++);

    selectedOption = menuSelector(x, y, yStart);

    return (selectedOption);
}

char waitForKeyHit() {
    int pressed;
    while (!kbhit());
    pressed = getch();
    return ((char) pressed);
}

int menuSelector(int x, int y, int yStart) {
    char key;
    int selectedOptionIndex = 0;
    x = x - 2;
    moveCursor(x, yStart);
    printf(">");
    moveCursor(x + 1, yStart);

    do {
        key = waitForKeyHit();

        if (key == (char) UP_ARROW) {
            moveCursor(x, yStart + selectedOptionIndex);
            printf(" ");

            if (yStart >= yStart + selectedOptionIndex)
                selectedOptionIndex = y - yStart - 2;
            else
                selectedOptionIndex--;
            moveCursor(x, yStart + selectedOptionIndex);
            printf(">");
        } else if (key == (char) DOWN_ARROW) {

            moveCursor(x, yStart + selectedOptionIndex);
            printf(" ");

            if (selectedOptionIndex + 2 >= y - yStart)
                selectedOptionIndex = 0;
            else
                selectedOptionIndex++;
            moveCursor(x, yStart + selectedOptionIndex);
            printf(">");
        }

    } while (key != (char) ENTER_KEY);
    return (selectedOptionIndex);
}

void moveCursor(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

void delay() {
    struct timespec delay;
    delay.tv_sec = 0;
    // 100,000,000 nanoseconds = 0.1 seconds
    delay.tv_nsec = 100000000 / 4;

    nanosleep(&delay, NULL);
}

void printSymbol(char value) {
    if (value == BIRD) {
        printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET, BIRD_SYMBOL);
    } else if (value == SNOOPY) {
        printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, SNOOPY_SYMBOL);
    } else if (value == BALL) {
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, BALL_SYMBOL);
    } else if (value == INVINCIBLE_BLOC) {
        printf(ANSI_COLOR_RED "%s"ANSI_COLOR_RESET, INVINCIBLE_BLOC_SYMBOL);
    } else if (value == PUSHABLE_BLOC) {
        printf(ANSI_COLOR_BLUE "%s"ANSI_COLOR_RESET, PUSHABLE_BLOC_SYMBOL);
    } else if (value == BREAKABLE_BLOC) {
        printf(ANSI_COLOR_YELLOW "%s"ANSI_COLOR_RESET, BREAKABLE_BLOC_SYMBOL);
    } else if (value == TRAPPED_BLOC) {
        printf(ANSI_COLOR_MAGENTA "%s"ANSI_COLOR_RESET, TRAPPED_BLOC_SYMBOL);
    } else {
        printf("%c", value);
    }
}

void printGameBoard(char boardGame[ROWS][COLS], int score, int remainingLives) {
    printf(
            ANSI_COLOR_BLUE"♫" ANSI_COLOR_RESET ": %d  | "
            ANSI_COLOR_RED"♥" ANSI_COLOR_RESET": %d  | "
            ANSI_COLOR_GREEN "★"ANSI_COLOR_RESET": %d\n",
            score,
            remainingLives,
            DEFAULT_GAME_DURATION
    );

    int i, j;
    printf("╔════════════════════╗\n");
    for (i = 0; i < ROWS; i++) {
        printf("║");
        for (j = 0; j < COLS; j++) {
            printSymbol(boardGame[i][j]);
        }
        printf("║\n");
    }
    printf("╚════════════════════╝\n");

    printf("\nEnter direction (" UP_ARROW_SYMBOL"/" DOWN_ARROW_SYMBOL "/" RIGHT_ARROW_SYMBOL "/" LEFT_ARROW_SYMBOL "), \"p\" to pause or \"q\" to quit. ");
}

void initializeCountdownTimer(int *remainingTime) {
    *remainingTime = DEFAULT_GAME_DURATION;
}

void checkRemainingTimeAndUpdate(int *remainingTime, clock_t *lastCheckTime) {
    clock_t currentTime = clock();
    double timeElapsed = ((double) (currentTime - *lastCheckTime)) / CLOCKS_PER_SEC;
    const int TIME_X = 20;
    const int TIME_Y = 3;

    if (timeElapsed >= 1.0) {
        *lastCheckTime = currentTime;
        if (*remainingTime >= 0) {
            moveCursor(TIME_X, TIME_Y);
            printf("%d    ", *remainingTime);

            moveCursor(0, 18);
            (*remainingTime)--;
        }
    }
}

enum LeveLResult playLevel(int level, int *globalScore, int *highestScore, int remainingLives) {
    int score = 0;
    enum LeveLResult isLevelWon = QUIT;
    Snoopy snoopy;
    Ball ball;

    ball.directionX = 1;
    ball.directionY = 1;

    printf("%d", level);
    // Time-based ball movement variables
    int ballTimer = 0;
    int ballMoveInterval = 10;

    int remainingTime;
    initializeCountdownTimer(&remainingTime);
    clock_t lastCheckTime = clock();

    char boardGame[ROWS][COLS];
    readGameBoardElementsFromFile(level, boardGame, &snoopy, &ball);

    clearScreen();
    printf("Level: %d\n\n", level);
    printGameBoard(boardGame, score, remainingLives);

    int numberUpdates = 0;
    Update updates[100000];

    // Get user input for direction
    while (score < 4 && remainingTime >= 0) {
        checkRemainingTimeAndUpdate(&remainingTime, &lastCheckTime);
        addUpdate(ball.y, ball.x, BALL, &numberUpdates, updates);

        numberUpdates = 0;

        if (kbhit()) {
            char key = (char) getch();

            if (key == 'q') { return QUIT; }
            else if (key == 'p') {
                moveCursor(0, 17);
                printf("GAME PAUSED PRESS \"p\" TO RESUME                                   \n\n");

                while ((int) waitForKeyHit() != 'p');

                moveCursor(0, 17);
                printf("Enter direction (" UP_ARROW_SYMBOL"/" DOWN_ARROW_SYMBOL "/" RIGHT_ARROW_SYMBOL "/" LEFT_ARROW_SYMBOL "), \"p\" to pause or \"q\" to quit. ");

            }

            moveSnoopy(boardGame, &snoopy, key, &score, &isLevelWon, &numberUpdates, updates);
            fflush(stdin);
        }

        if (ballTimer % ballMoveInterval == 0) {
            updateBallPlacement(boardGame, &ball, &isLevelWon, updates, &numberUpdates);

            if (isLevelWon == LOST) {
                return LOST;
            }
        }

        ballTimer++;

        updateElementsDisplay(boardGame, updates, numberUpdates);

        delay();
    }

    if (score == 4) {
        isLevelWon = WON;
        *globalScore += remainingTime * level;
        if (*globalScore > *highestScore) {
            *highestScore = *globalScore;
            saveNewHighestScore(*highestScore);
        }
    }

    if (score < 4 && remainingTime < 0) {
        isLevelWon = LOST;
    }

    displayLevelResult(isLevelWon, level, &score);

    return isLevelWon;
}

void displayLevelResult(enum LeveLResult isLevelWon, int level, int *score) {
    if (isLevelWon == WON && level < AVAILABLE_LEVELS) {
        moveCursor(0, 17);

        char passwords[MAX_NUM_PASSWORDS][MAX_PASSWORD_LENGTH];
        loadPasswordsFile(passwords);
        printf(ANSI_COLOR_GREEN "CONGRATS! YOU WON LEVEL %d!                                   \n" ANSI_COLOR_RESET,
               level);

        printf(ANSI_COLOR_YELLOW STAR_SYMBOL " Your Score is : "ANSI_COLOR_RESET "%d\n", &score);


        printf("LEVEL %d PASSWORD: " ANSI_COLOR_GREEN "%s                                   \n\n" ANSI_COLOR_RESET,
               level + 1, passwords[level]);

        printf("Press \"enter\" to continue.");
        while ((int) waitForKeyHit() != ENTER_KEY);
    }

    if (isLevelWon == LOST) {
        moveCursor(0, 17);
        printf(ANSI_COLOR_RED "TIME IS UP!                                                       \n\n" ANSI_COLOR_RESET);

        printf("Press \"enter\" to continue.");
        while ((int) waitForKeyHit() != ENTER_KEY);
    }


}


void moveBallDiagonally(char boardGame[10][20], Ball *ball, enum LeveLResult *isLevelWon) {
    int nextX = ball->x + ball->directionX;
    int nextY = ball->y + ball->directionY;
    char next;

    // Collision checks
    if (nextX < 0 || nextX >= ROWS) {
        ball->directionX = -ball->directionX;
    } else if (nextY < 0 || nextY >= COLS) {
        ball->directionY = -ball->directionY;
    } else if (boardGame[nextX][nextY] == BIRD) {
        ball->directionX = -ball->directionX;
        ball->directionY = -ball->directionY;
    } else if (boardGame[nextX][nextY] == SNOOPY) {
        *isLevelWon = LOST;
        return;
    } else if (boardGame[nextX][nextY] == PUSHABLE_BLOC || boardGame[nextX][nextY] == BREAKABLE_BLOC ||
               boardGame[nextX][nextY] == INVINCIBLE_BLOC || boardGame[nextX][nextY] == TRAPPED_BLOC) {
        next = boardGame[nextX][nextY];

        if (boardGame[nextX - ball->directionX][nextY] == next ||
            boardGame[nextX + ball->directionX][nextY] == next)
            ball->directionY = -ball->directionY;

        else if (boardGame[nextX][nextY + ball->directionY] == next ||
                 boardGame[nextX][nextY - ball->directionY] == next)
            ball->directionX = -ball->directionX;

        else {
            ball->directionX = -ball->directionX;
            ball->directionY = -ball->directionY;
        }
    } else if (boardGame[nextX - ball->directionX][nextY] == next &&
               boardGame[nextX][nextY - ball->directionY] == next) {
        ball->directionX = -ball->directionX;
        ball->directionY = -ball->directionY;
    }
    // Update ball's position if no collision
    ball->x += ball->directionX;
    ball->y += ball->directionY;
}

void
updateBallPlacement(char boardGame[10][20], Ball *ball, enum LeveLResult *isLevelWon, Update updates[100000],
                    int *numberUpdates) {
    boardGame[(ball->x)][(ball->y)] = EMPTY;
    addUpdate((ball->y), (ball->x), EMPTY, numberUpdates, updates);

    moveBallDiagonally(boardGame, ball, isLevelWon);

    boardGame[(ball->x)][(ball->y)] = BALL;
    addUpdate((ball->y), (ball->x), BALL, numberUpdates, updates);
}

void addUpdate(int x, int y, char newValue, int *index, Update *updates) {
    updates[*index].newValue = newValue;
    updates[*index].x = x;
    updates[*index].y = y;

    (*index)++;
}

void updateElementsDisplay(char (*boardGame)[20], Update *updates, int numberUpdates) {
    for (int i = 0; i < numberUpdates; i++) {
        int x = updates[i].x;
        int y = updates[i].y;
        char newValue = updates[i].newValue;
        boardGame[y][x] = newValue;

        int y_offset = 5;
        int x_offset = 2;
        moveCursor(x + x_offset, y + y_offset); // Adjust the coordinates based on your console's cursor positioning

        printSymbol(newValue);
    }

    moveCursor(0, 18);
}

void moveSnoopy(char (*board)[20], Snoopy *snoopy, char key, int *score, enum LeveLResult *isLevelWon,
                int *numberUpdates, Update *updates) {
    int number;
    const int SCORE_X = 2;
    const int SCORE_Y = -2;

    switch (key) {
        case UP_ARROW:
            if (snoopy->x > 0) {
                if (board[snoopy->x - 1][snoopy->y] == BALL || board[snoopy->x - 1][snoopy->y] == TRAPPED_BLOC) {
                    *isLevelWon = LOST;
                    return;
                } else if (board[snoopy->x - 1][snoopy->y] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;
                    addUpdate(SCORE_X, SCORE_Y, charRepresentation, numberUpdates, updates);
                } else if (board[snoopy->x - 1][snoopy->y] == BREAKABLE_BLOC) {
                    addUpdate(SCORE_X, SCORE_Y, EMPTY, numberUpdates, updates);
                }

                if (board[snoopy->x - 1][snoopy->y] == PUSHABLE_BLOC && snoopy->x - 1 > 0 &&
                    board[snoopy->x - 2][snoopy->y] != INVINCIBLE_BLOC
                    && board[snoopy->x - 2][snoopy->y] != BIRD && board[snoopy->x - 2][snoopy->y] != PUSHABLE_BLOC) {

                    board[snoopy->x][snoopy->y] = EMPTY;
                    addUpdate(snoopy->y, snoopy->x, EMPTY, numberUpdates, updates);
                    board[snoopy->x - 1][snoopy->y] = EMPTY;
                    addUpdate(snoopy->y, snoopy->x - 1, EMPTY, numberUpdates, updates);
                    board[--(snoopy->x)][snoopy->y] = SNOOPY;
                    addUpdate(snoopy->y, snoopy->x, SNOOPY, numberUpdates, updates);
                    board[snoopy->x - 1][snoopy->y] = PUSHABLE_BLOC;
                    addUpdate(snoopy->y, snoopy->x - 1, PUSHABLE_BLOC, numberUpdates, updates);

                }

                if (board[snoopy->x - 1][snoopy->y] != INVINCIBLE_BLOC &&
                    board[snoopy->x - 1][snoopy->y] != PUSHABLE_BLOC) {
                    board[snoopy->x][snoopy->y] = EMPTY;
                    addUpdate(snoopy->y, snoopy->x, EMPTY, numberUpdates, updates);
                    board[--(snoopy->x)][snoopy->y] = SNOOPY;
                    addUpdate(snoopy->y, snoopy->x, SNOOPY, numberUpdates, updates);
                }
            }
            break;
        case DOWN_ARROW:
            if (snoopy->x < ROWS - 1) {
                if (board[snoopy->x + 1][snoopy->y] == BALL || board[snoopy->x + 1][snoopy->y] == TRAPPED_BLOC) {
                    *isLevelWon = LOST;
                    return;
                }

                if (board[snoopy->x + 1][snoopy->y] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;
                    addUpdate(SCORE_X, SCORE_Y, charRepresentation, numberUpdates, updates);
                }
                if (board[snoopy->x + 1][snoopy->y] == BREAKABLE_BLOC) {
                    addUpdate(SCORE_X, SCORE_Y, EMPTY, numberUpdates, updates);
                }
                if (board[snoopy->x + 1][snoopy->y] == PUSHABLE_BLOC && snoopy->x + 1 < ROWS - 1 &&
                    board[snoopy->x + 2][snoopy->y] != INVINCIBLE_BLOC
                    && board[snoopy->x + 2][snoopy->y] != BIRD && board[snoopy->x + 2][snoopy->y] != PUSHABLE_BLOC) {

                    board[snoopy->x][snoopy->y] = EMPTY;
                    addUpdate(snoopy->y, snoopy->x, EMPTY, numberUpdates, updates);
                    board[snoopy->x + 1][snoopy->y] = EMPTY;
                    addUpdate(snoopy->y - 1, snoopy->x, EMPTY, numberUpdates, updates);
                    board[++(snoopy->x)][snoopy->y] = SNOOPY;
                    addUpdate(snoopy->y, snoopy->x, SNOOPY, numberUpdates, updates);
                    board[snoopy->x + 1][snoopy->y] = PUSHABLE_BLOC;
                    addUpdate(snoopy->y, snoopy->x + 1, PUSHABLE_BLOC, numberUpdates, updates);
                }

                if (board[snoopy->x + 1][snoopy->y] != INVINCIBLE_BLOC &&
                    board[snoopy->x + 1][snoopy->y] != PUSHABLE_BLOC) {
                    addUpdate(snoopy->y, snoopy->x, EMPTY, numberUpdates, updates);
                    board[snoopy->x][snoopy->y] = EMPTY;
                    board[++(snoopy->x)][snoopy->y] = SNOOPY;
                    addUpdate(snoopy->y, snoopy->x, SNOOPY, numberUpdates, updates);
                }
            }
            break;
        case LEFT_ARROW:
            if (snoopy->y > 0) {
                if (board[snoopy->x][snoopy->y - 1] == BALL || board[snoopy->x][snoopy->y - 1] == TRAPPED_BLOC) {
                    *isLevelWon = LOST;
                    return;
                }

                if (board[snoopy->x][snoopy->y - 1] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;
                    addUpdate(SCORE_X, SCORE_Y, charRepresentation, numberUpdates, updates);
                }
                if (board[snoopy->x][snoopy->y - 1] == BREAKABLE_BLOC) {
                    addUpdate(SCORE_X, SCORE_Y, EMPTY, numberUpdates, updates);
                }
                if (board[snoopy->x][snoopy->y - 1] == PUSHABLE_BLOC && snoopy->y - 1 > 0 &&
                    board[snoopy->x][snoopy->y - 2] != INVINCIBLE_BLOC
                    && board[snoopy->x][snoopy->y - 2] != BIRD && board[snoopy->x][snoopy->y - 2] != PUSHABLE_BLOC) {

                    board[snoopy->x][snoopy->y] = EMPTY;
                    addUpdate(snoopy->y, snoopy->x, EMPTY, numberUpdates, updates);
                    board[snoopy->x][snoopy->y - 1] = EMPTY;
                    addUpdate(snoopy->y - 1, snoopy->x, EMPTY, numberUpdates, updates);
                    board[(snoopy->x)][--(snoopy->y)] = SNOOPY;
                    addUpdate(snoopy->y, snoopy->x, SNOOPY, numberUpdates, updates);
                    board[snoopy->x][snoopy->y - 1] = PUSHABLE_BLOC;
                    addUpdate(snoopy->y - 1, snoopy->x, PUSHABLE_BLOC, numberUpdates, updates);
                }

                if (board[snoopy->x][snoopy->y - 1] != INVINCIBLE_BLOC &&
                    board[snoopy->x][snoopy->y - 1] != PUSHABLE_BLOC) {
                    addUpdate(snoopy->y, snoopy->x, EMPTY, numberUpdates, updates);
                    board[snoopy->x][snoopy->y] = EMPTY;
                    board[snoopy->x][--(snoopy->y)] = SNOOPY;
                    addUpdate(snoopy->y, snoopy->x, SNOOPY, numberUpdates, updates);
                }
            }
            break;
        case RIGHT_ARROW:
            if (snoopy->y < COLS - 1) {
                if (board[snoopy->x][snoopy->y + 1] == BALL || board[snoopy->x][snoopy->y + 1] == TRAPPED_BLOC) {
                    *isLevelWon = LOST;
                    return;
                }

                if (board[snoopy->x][snoopy->y + 1] == BIRD) {
                    (*score)++;

                    number = *score;
                    char charRepresentation = '0' + number;

                    addUpdate(SCORE_X, SCORE_Y, charRepresentation, numberUpdates, updates);
                }
                if (board[snoopy->x][snoopy->y + 1] == BREAKABLE_BLOC) {

                    addUpdate(SCORE_X, SCORE_Y, EMPTY, numberUpdates, updates);
                }
                if (board[snoopy->x][snoopy->y + 1] == PUSHABLE_BLOC && snoopy->y + 1 < COLS - 1 &&
                    board[snoopy->x][snoopy->y + 2] != INVINCIBLE_BLOC
                    && board[snoopy->x][snoopy->y + 2] != BIRD && board[snoopy->x][snoopy->y + 2] != PUSHABLE_BLOC) {

                    board[snoopy->x][snoopy->y] = EMPTY;
                    addUpdate(snoopy->y, snoopy->x, EMPTY, numberUpdates, updates);
                    board[snoopy->x][snoopy->y + 1] = EMPTY;
                    addUpdate(snoopy->y + 1, snoopy->x, EMPTY, numberUpdates, updates);
                    board[(snoopy->x)][++(snoopy->y)] = SNOOPY;
                    addUpdate(snoopy->y, snoopy->x, SNOOPY, numberUpdates, updates);
                    board[snoopy->x][snoopy->y + 1] = PUSHABLE_BLOC;
                    addUpdate(snoopy->y + 1, snoopy->x, PUSHABLE_BLOC, numberUpdates, updates);
                }

                if (board[snoopy->x][snoopy->y + 1] != INVINCIBLE_BLOC &&
                    board[snoopy->x][snoopy->y + 1] != PUSHABLE_BLOC) {
                    addUpdate(snoopy->y, snoopy->x, EMPTY, numberUpdates, updates);
                    board[snoopy->x][snoopy->y] = EMPTY;
                    board[snoopy->x][++(snoopy->y)] = SNOOPY;
                    addUpdate(snoopy->y, snoopy->x, SNOOPY, numberUpdates, updates);
                }
            }
            break;
    }
}

void readGameBoardElementsFromFile(int level, char boardGame[ROWS][COLS], Snoopy *snoopy, Ball *ball) {
    char element;
    char filename[26]; // Assuming the file names are like "level1.txt", "level2.txt", ..., "level100.txt"
    snprintf(filename, sizeof(filename), "../data/levels/level%d.txt", level);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    // Read values from the file and update the boardGame matrix and positions
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(file, " %c", &element);
            switch (element) {
                case '0':
                    element = EMPTY;
                    break;
                case '1':
                    element = BREAKABLE_BLOC;
                    break;
                case '2':
                    element = PUSHABLE_BLOC;
                    break;
                case '3':
                    element = TRAPPED_BLOC;
                    break;
                case '9':
                    element = BIRD;
                    break;
                case '4':
                    element = INVINCIBLE_BLOC;
                    break;
                case '7':
                    element = SNOOPY;
                    snoopy->x = i;
                    snoopy->y = j;
                    break;
                case '8':
                    element = BALL;
                    ball->x = i;
                    ball->y = j;
                    break;
                default:
                    break;
            }
            boardGame[i][j] = element;
        }
    }
    fclose(file);
}

int isLevelAvailable(int level) {
    char filename[26]; // Assuming the file names are like "level1.txt", "level2.txt", ..., "level100.txt"
    snprintf(filename, sizeof(filename), "../data/levels/level%d.txt", level);


    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

void displayAvailableLevels() {
    printf("Available Levels: ");
    for (int i = 1; i <= AVAILABLE_LEVELS; ++i) {
        if (isLevelAvailable(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void loadPasswordsFile(char passwords[MAX_NUM_PASSWORDS][MAX_PASSWORD_LENGTH]) {
    int numPasswords = 0;

    const char *file_path = "../data/levels/level_passwords.txt";

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while (numPasswords < MAX_NUM_PASSWORDS && fscanf(file, "%5s", passwords[numPasswords]) == 1) {
        numPasswords++;
    }

    fclose(file);
}

int validatePassword(int level, const char *enteredPassword) {
    char passwords[MAX_NUM_PASSWORDS][MAX_PASSWORD_LENGTH];
    loadPasswordsFile(passwords);

    return strcmp(enteredPassword, passwords[level - 1]) == 0;
}

void saveNewHighestScore(int score) {
    FILE *file;

    // If file doesn't exist, create it and write the new value
    file = fopen("../data/high_score.txt", "w");
    fprintf(file, "%d", score);
    fclose(file);
}

int readHighestScore() {
    FILE *file;
    int highestScore = 0;

    // Open file for reading
    file = fopen("../data/high_score.txt", "r");
    if (file == NULL) {
        return highestScore; // Return -1 to indicate an error or no score available
    }

    // Read the highest score from the file
    fscanf(file, "%d", &highestScore);
    fclose(file);

    return highestScore;
}
