#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define numOfBoats 5

void printBoard(char** board);
bool inputCoordinates(int* x, int* y);
bool areValidXY(int x, int y);
bool endGame(int x, int y);
bool isValidPos(int x, int y, char ** board);
bool placeFirstShipTile(int x, int y, char** board);

int main() {
    char** board = malloc(10 * sizeof(char*));

    for(int i = 0; i < 10; i++) {
        board[i] = calloc(10, sizeof(char)); 
        
        for(int j = 0; j < 10; j++) board[i][j] = '.';
    }

    int x, y;
    bool isFirstPlacement = true;
    for(int i = 0; i < numOfBoats; i++){
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

        printf("Boat %d complete", (i+1));
        isFirstPlacement = true;
    }

    for(int i = 0; i < 10; i++) {
        free(board[i]);
    }
    free(board);

    printf("Game exited. Goodbye\n");
    return 0;
}

bool inputCoordinates(int* x, int* y) {
    while (1) {
        printf("\nEnter X and Y coordinates (0-9) or -1 -1 to finish boat: ");
        
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
        printf("cannot place on taken space");
        return false;
    }
    board[y][x] = 'X';
    printBoard(board);
    return true;
}