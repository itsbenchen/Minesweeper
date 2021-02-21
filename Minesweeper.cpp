#include <iostream>
#include <time.h>

using namespace std;

#define maxSize 24

struct Cell {
    int row;
    int column;
    bool hasMine;
    bool isRevealed;
    int numberOfAdjacentMines;
};

enum Difficulty {   // Pre-determined difficulty (0, 1, 2 : Easy, Intermediate, Advanced)
    Easy,   // 8 x 8 board with 10 mines
    Intermediate, // 16 x 16 board with 40 mines
    Advanced, // 24 x 24 board with 99 mines
};

class Minesweeper {
    private:
        int maxRows; // Number of rows in board
        int maxColumns; // Number of columns in board
        int numberOfMines; // number of mines in grid
        Cell playerGameBoard[maxSize][maxSize];
        int revealedCount = 0;

    public:
        Minesweeper(int mode) {
            if (mode == Easy) {
                maxRows = 8;
                maxColumns = 8;
                numberOfMines = 10;
            }
            else if (mode == Intermediate) {
                maxRows = 16;
                maxColumns = 16;
                numberOfMines = 40;
            }   
            else if (mode == Advanced) {
                maxRows = 24;
                maxColumns = 24;
                numberOfMines = 99;
            }
            else {
                cout << mode << " is not a valid mode" << endl;
                throw;
            }
            init();
        }

        void start() {
            /* Initiates the game by asking for input and repeats till game is won or lost */
            while(true) {
                system("cls");
                int x, y;
                printGameBoard();
                // revealGameBoard();
                if (playerWon() == true) {
                    system("cls");
                    cout << "You won! Congrats" << endl;
                    revealGameBoard();
                    break;
                }
                cout << "Enter your move in the format -> row column" << endl;
                cout << "Rows range from 0 -> " << maxRows - 1 << endl;
                cout << "Columns range from 0 -> " << maxColumns - 1 << endl;
                scanf("%d %d", &x, &y);
                cout << "Applying click to (" << x << ", " << y << ")" << endl;
                nextState(x, y);
                cout << "Applied (" << x << ", " << y << ") to game board" << endl;
                if (playerGameBoard[x][y].hasMine == true) {    // Checks if you selected a bomb
                    system("cls");
                    printGameBoard();
                    cout << "You chose a spot with a mine! You lose." << endl;
                    revealGameBoard();
                    break;
                }
                system("pause");
            }
            return;
        }

    private:
        bool isInBounds(int row, int column) {
            /* Checks if the given row and column is within bound of game board */
            return (row >= 0) && (column >= 0) && (row < maxRows) && (column < maxColumns);
        }

        void init() {
            /* Initiates the board and fills them with cells */
            for (int i = 0; i < maxRows; i++) {
                for (int j = 0; j < maxColumns; j++) {
                    Cell cell = playerGameBoard[i][j];
                    cell.hasMine = false;
                    cell.isRevealed = false;
                    cell.row = i;
                    cell.column = j;
                    cell.numberOfAdjacentMines = 0;
                    playerGameBoard[i][j] = cell;
                }
            }

            /* Add mine to board */
            addMines();
            /* Calculates all adjacent mines */
            calculateAllAdjacentMines();
        }

        void addMines() {
            /* Add mines to the player board */

            srand (time(NULL)); // Randomizes the seed
            
            int minesAdded = 0;
            while (minesAdded != numberOfMines) {
                int randomRow = rand() % maxRows;
                int randomColumn = rand() % maxColumns;
                if (playerGameBoard[randomRow][randomColumn].hasMine == false) {
                    playerGameBoard[randomRow][randomColumn].hasMine = true;
                    minesAdded += 1;
                }
            }
        }

        void calculateAdjacentMines(Cell &cell) {
            /* Calculates the number of adjacent mines for the cell and set it to that value */
            int bombCounter = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (isInBounds(cell.row + i, cell.column + j)) {  // Check if within game board's bound
                        if (playerGameBoard[cell.row + i][cell.column + j].hasMine == true) {   // Checks if adjacent cell has a mine or not
                            bombCounter += 1;
                        }        
                    }
                }
            }
            if (cell.hasMine == true) { // Remove the count for the base cell's bomb
                bombCounter -= 1; 
            }
            cell.numberOfAdjacentMines = bombCounter;
        }

        void calculateAllAdjacentMines() {
            for (int i = 0; i < maxRows; i++) {
                for (int j = 0; j < maxColumns; j++) {
                    calculateAdjacentMines(playerGameBoard[i][j]);
                }
            }
        }

        void printGameBoard() {
            /* prints the player's view of baord */
            cout << "Current Board:" << endl;

            for (int i = 0; i < maxRows; i++) {
                for (int j = 0; j < maxColumns; j++) {
                    Cell cell = playerGameBoard[i][j];
                    if (cell.isRevealed) {
                        if (cell.hasMine) {
                            cout << "* ";
                        }
                        else {
                            cout << cell.numberOfAdjacentMines << " ";
                        }
                    }
                    else {
                        cout << "- ";
                    }
                }
                cout << endl; // End of row
            }
        }

        void revealGameBoard() {
            /* Reveals the entire game board */
            cout << "Revealed Game Board: " << endl;
            for (int i = 0; i < maxRows; i++) {
                for (int j = 0; j < maxColumns; j++) {
                    Cell cell = playerGameBoard[i][j];
                    if (cell.hasMine) {
                            cout << "* ";
                    }
                    else {
                        cout << cell.numberOfAdjacentMines << " ";
                    }
                }
                cout << endl; // End of row
            }
        }


        void nextState(int row, int col) {
            /* Takes the user's input (row, col) and apply it to the current game board; Uses recursion to apply logic to game state*/
            Cell* cell = &playerGameBoard[row][col];
            if (isInBounds(row, col) == true && cell->isRevealed == false) {
                cell->isRevealed = true;
                revealedCount += 1;

                // Bust case (Mine found)
                if (cell->hasMine == true) {
                    return;
                }

                // Base case
                if (cell->numberOfAdjacentMines != 0) {
                    return;
                }

                // Case where there's no adjacent mines (repeatable pattern)
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if ((isInBounds(row + i, col + j) == true)) { // If it's in bounds and an adjacent cell
                            nextState(row + i, col + j);
                        }   
                    }
                }
            }
        }

        bool playerWon() {
            /* Checks if all non-mine cells have been revealed */
            return (revealedCount == ((maxRows*maxColumns) - numberOfMines));
        }
};

int main() {
    /* Choose a difficulty:
        Easy: 8 by 8 cells with 10 mines
        Intermediate: 16 by 16 cells with 40 mines
        Advanced: 24 by 24 cells with 99 mines
    */
    int difficulty;
    cout << "Welcome to Minesweeper! Please select a difficulty..." << endl;
    do {
        cout << "Press 0 for Easy, 1 for Intermediate, and 2 for Advanced" << endl;
        cin >> difficulty;
    } while (difficulty < 0 || difficulty > 2);

    Minesweeper game = Minesweeper(difficulty);
    game.start();
    return 0;
}

