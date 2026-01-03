#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int rows, columns;

void printBoard(char** board);
void playGame(int x, int y, char** board);
bool inputCoordinates(int* x, int* y);
bool areValidXY(int x, int y);
bool endGame(int x, int y);
bool isValidPos(int x, int y, int prevX, int prevY, char* axis, char ** board);
bool placeFirstShipTile(int x, int y, char** board);
bool isClearBoard(char** board);

int main() {

    //Start Game with user board sizes
    printf("Enter the number of rows and columns: ");
    if (scanf("%d %d", &rows, &columns) != 2) {
        return 1;
    }

    char** board = malloc(rows * sizeof(char*));

    for(int i = 0; i < rows; i++) {
        board[i] = calloc(columns, sizeof(char)); 
        for(int j = 0; j < columns; j++) board[i][j] = '.';
    }

    printf("Place your ships\n");

    int x, y, prevX, prevY;
    char axis = '-';
    bool isFirstPlacement = true;
    int newShip;

    while(1){
        printf("\nEnter 1 to start a new ship or 0 to finish: ");
        if (scanf("%d", &newShip) != 1) {
            while(getchar() != '\n');
            continue;
        }
        
        if(newShip != 1) break;

        isFirstPlacement = true;
        axis = '-';

        while(1){
            if (!inputCoordinates(&x, &y)){
                break; 
            }

            if(isFirstPlacement){
                if(placeFirstShipTile(x, y, board)){
                    isFirstPlacement = false;
                    prevX = x;
                    prevY = y;
                }
                continue;
            }

            if (isValidPos(x, y, prevX, prevY, &axis, board)){
                board[y][x] = 'X';
                printBoard(board);

                prevX = x;
                prevY = y;
            }
            else {
                printf("Invalid position, ship must be in a straight line.\n");
            }
        }
    }

    // Game Portion
    printf("\nNow try to destroy the ships!\n");
    playGame(x, y, board);

    // Memory Cleanup
    for(int i = 0; i < rows; i++) free(board[i]);
    free(board);

    printf("\nGame exited.\n\n");
    return 0;
}

bool inputCoordinates(int* x, int* y) {
    while (1) {
        printf("\nEnter X (0-%d) and Y (0-%d) or -1 -1 to finish: ", columns-1, rows-1);
        
        if (scanf("%d %d", x, y) != 2) {
            printf("Invalid input. Please enter numbers.\n");
            continue;
        }

        if(endGame(*x, *y)){
            return false;
        }

        if (!areValidXY(*x, *y)) {
            printf("Out of bounds, Please stay within 0-9.\n");
            continue;
        }

        return true; 
    }
}

bool areValidXY(int x, int y){
        if (x > (columns-1) || x < 0 || y < 0 || y > (rows-1)) {
            return false;
        }
        return true;
}

bool isValidPos(int x, int y, int prevX, int prevY, char* axis, char** board) {
    if (board[y][x] == 'X') {
        printf("This spot is already taken!\n");
        return false;
    }

    bool isAdjacentX = (y == prevY) && (x == prevX + 1 || x == prevX - 1);
    bool isAdjacentY = (x == prevX) && (y == prevY + 1 || y == prevY - 1);

    if (*axis == '-') {
        if (isAdjacentX) {
            *axis = 'x';
            return true;
        }
        if (isAdjacentY) {
            *axis = 'y';
            return true;
        }
    } else {
        if (*axis == 'x' && isAdjacentX) {
            return true;
        }
        if (*axis == 'y' && isAdjacentY) {
            return true;
        }
    }

    printf("Invalid placement. Ships must be in a straight line and connected.\n");
    return false;
}

bool endGame(int x, int y){
    if (x == -1 && y == -1) {
        return true; 
    }
    return false;
}

void printBoard(char** board) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            printf("%c  ", board[i][j]);
        }
        printf("\n");
    }
}

bool placeFirstShipTile(int x, int y, char** board){
    if(board[y][x] == 'X'){
        printf("cannot place on taken space \n");
        return false;
    }
    board[y][x] = 'X';
    printBoard(board);
    return true;
}

bool isClearBoard(char** board){
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < columns; x++){
            if(board[y][x] == 'X'){
                return false;
            }
        }
    }
    return true;
}

void playGame(int x, int y, char** board){
    while(1){
        if(isClearBoard(board)){
            printf("\nYou Win!\n");
            break;
        }

        if (!inputCoordinates(&x, &y)){
            printf("\nYou Lose\n");
            break;
        }

        if(board[y][x] == 'X'){
            printf("\nYou have successfully hit a ship\n");
            board[y][x] = '.';
        }
        else{
            printf("\nMiss\n");
        }
    }
    return;
}