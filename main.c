#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int rows, columns, shipCount = 0;
char** shipNames = NULL; // [name, name2]...
int** shipPositions = NULL; // [ship] -> [[0,1,1,1]]...
int* shipLengths = NULL;

void printBoard(char** board);
void playGame(int x, int y, char** board);
bool inputCoordinates(int* x, int* y);
bool areValidXY(int x, int y);
bool endGame(int x, int y);
bool isValidPos(int x, int y, int* minX, int* maxX, int* minY, int* maxY, char* axis, char** board);
bool placeFirstShipTile(int x, int y, char** board);
bool isClearBoard(char** board);
void adjacent(int x, int y, int* minX, int* maxX, int* minY, int* maxY, bool* isAdjY, bool* isAdjX);
void destroyTile(int x, int y);
void removeDestroyedTile(int* shipPos, int idx);

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

    int x, y, minX, maxX, minY, maxY;
    char axis = '-';
    bool isFirstPlacement = true;
    int newShip;
    int tileCount;
    while(1){
        printf("\nEnter 1 to start a new ship or 0 to finish: ");
        if (scanf("%d", &newShip) != 1) {
            while(getchar() != '\n');
            continue;
        }
        
        if(newShip != 1) break;

        shipCount++;
        char** temp = realloc(shipNames, shipCount * sizeof(char*));
            if (temp == NULL) {
                return 1; 
            }
            shipNames = temp;

        int ** tempPos = realloc(shipPositions, shipCount * sizeof(int*));

        if (tempPos == NULL){
            return 1;
        }
            
        shipPositions = tempPos;

        shipPositions[shipCount-1] = malloc(2 * sizeof(int));
        
        shipLengths = realloc(shipLengths, shipCount * sizeof(int));

        shipNames[shipCount-1] = malloc(50 * sizeof(char)); 

        int c;
        while ((c = getchar()) != '\n' && c != EOF); 

        printf("Enter ship name: ");
        fgets(shipNames[shipCount-1], 50, stdin);
        
        int i = 0;
        while (shipNames[shipCount-1][i] != '\0') {
            if (shipNames[shipCount-1][i] == '\n') {
                shipNames[shipCount-1][i] = '\0';
                break;
            }
            i++;
        }

        isFirstPlacement = true;
        axis = '-';

        while(getchar() != '\n');

        while(1){
            if (!inputCoordinates(&x, &y)){
                break; 
            }

            if(isFirstPlacement){
                if(placeFirstShipTile(x, y, board)){
                    isFirstPlacement = false;
                    minX = maxX = x;
                    minY = maxY = y;
                    shipPositions[shipCount-1][0] = x;
                    shipPositions[shipCount-1][1] = y;
                    tileCount = 1;
                }
                continue;
            }

            if (isValidPos(x, y, &minX, &maxX, &minY, &maxY, &axis, board)){
                tileCount++;
                int* temp = realloc(shipPositions[shipCount-1], tileCount * 2 * sizeof(int));
                if(temp)
                    shipPositions[shipCount-1] = temp;
                
                shipPositions[shipCount-1][(tileCount-1) * 2] = x;
                shipPositions[shipCount-1][((tileCount-1) * 2) + 1] = y;
                board[y][x] = 'X';
                printBoard(board);
            }
            else {
                printf("Invalid position, ship must be in a straight line.\n");
            }
        }
        shipLengths[shipCount-1] = tileCount;
    }

    // Game Portion
    printf("\nNow try to destroy the ships!\n");
    playGame(x, y, board);

    // Memory Cleanup
    for(int i = 0; i < rows; i++) free(board[i]);

    free(board);

    for(int i = 0; i < shipCount; i++) {
        free(shipNames[i]);
        free(shipPositions[i]);
    }

    free(shipNames);
    free(shipPositions);
    free(shipLengths);

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

bool isValidPos(int x, int y, int* minX, int* maxX, int* minY, int* maxY, char* axis, char** board) {
    if (board[y][x] == 'X') {
        printf("This spot is already taken!\n");
        return false;
    }

    bool isAdjacentX, isAdjacentY;
    adjacent(x, y, minX, maxX, minY, maxY, &isAdjacentY, &isAdjacentX);

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
        if (*axis == 'x' && isAdjacentX)
            return true;
        if (*axis == 'y' && isAdjacentY)
            return true;
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
            destroyTile(x, y);
            board[y][x] = '.';
        }
        else{
            printf("\nMiss\n");
        }
    }
    return;
}

void adjacent(int x, int y, int* minX, int* maxX, int* minY, int* maxY, bool* isAdjacentY, bool* isAdjacentX) {
    *isAdjacentX = false;
    *isAdjacentY = false;

    if (y == *minY && y == *maxY) { 
        if (x == *minX - 1) {
            *minX = x; 
            *isAdjacentX = true;
        } else if (x == *maxX + 1) {
            *maxX = x; 
            *isAdjacentX = true;
        }
    }

    if (x == *minX && x == *maxX) {
        if (y == *minY - 1) {
            *minY = y; 
            *isAdjacentY = true;
        } else if (y == *maxY + 1) {
            *maxY = y; 
            *isAdjacentY = true;
        }
    }
}

void removeDestroyedTile(int* shipPos, int idx) {
    shipPos[idx] = -1; 
    shipPos[idx + 1] = -1;
}

void destroyTile(int x, int y) {
    for (int i = 0; i < shipCount; i++) {
        bool shipSunk = true;
        bool hitInThisShip = false;

        for (int j = 0; j < shipLengths[i] * 2; j += 2) {
            if (shipPositions[i][j] == x && shipPositions[i][j+1] == y) {
                removeDestroyedTile(shipPositions[i], j);
                hitInThisShip = true;
            }

            if (shipPositions[i][j] != -1) {
                shipSunk = false;
            }
        }

        if (hitInThisShip && shipSunk) {
            printf("Ship: %s destroyed!\n", shipNames[i]);
        }
        if (hitInThisShip) break; 
    }
}