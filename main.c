#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define numOfBoats 5

void printBoard(char** board);
void playGame(int x, int y, char** board);
bool inputCoordinates(int* x, int* y);
bool areValidXY(int x, int y);
bool endGame(int x, int y);
bool isValidPos(int x, int y, char ** board);
bool placeFirstShipTile(int x, int y, char** board);
bool isClearBoard(char** board);

int main() {
    char** board = malloc(10 * sizeof(char*));

    for(int i = 0; i < 10; i++) {
        board[i] = calloc(10, sizeof(char)); 
        
        for(int j = 0; j < 10; j++) board[i][j] = '.';
    }

    printf("Place your ships\n");

    int x, y;
    bool isFirstPlacement = true;
    for(int i = 0; i < numOfBoats; i++){
        printf("Ship %d: \n", (i+1));

        while(1){
            if (!inputCoordinates(&x, &y)){
                break;
            }

            if(isFirstPlacement){
                if(placeFirstShipTile(x,y,board)){
                    isFirstPlacement = false;
                }
                continue;
            }

            if (isValidPos(x,y, board)){
                if(board[y][x] == 'X'){
                    printf("Ships cannot overlap");
                    continue;
                }

                board[y][x] = 'X';
                printBoard(board);
            }
            else{
                printf("Ship hulls must be connected vertically or horizontally");
            }
        }
        isFirstPlacement = true;
    }

    // Game Portion
    printf("\nNow try to destroy the ships!\n");
    playGame(x,y,board);

    for(int i = 0; i < 10; i++) {
        free(board[i]);
    }
    free(board);

    printf("\nGame exited.\n\n");

    return 0;
}

bool inputCoordinates(int* x, int* y) {
    while (1) {
        printf("\nEnter X and Y coordinates (0-9) or -1 -1 to finish: ");
        
        if (scanf("%d %d", x, y) != 2) {
            printf("Invalid input. Please enter numbers.\n");
            while (getchar() != '\n'); 
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
        if (x > 9 || x < 0 || y < 0 || y > 9) {
            return false;
        }
        return true;
}

bool isValidPos(int x, int y, char** board){
    return ((x > 0 && board[y][x-1] == 'X') || (y>0 && board[y-1][x] == 'X') || (y<9 && board[y+1][x] == 'X') || (x<9 && board[y][x+1] == 'X'));
}

bool endGame(int x, int y){
    if (x == -1 && y == -1) {
        return true; 
    }
    return false;
}

void printBoard(char** board) {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
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
    for(int y = 0; y < 10; y++){
        for(int x = 0; x < 10; x++){
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