#include <stdio.h>
#include <stdlib.h>

void printBoard(char** board);

int main() {
    char** board = malloc(10 * sizeof(char*));

    for(int i = 0; i < 10; i++) {
        board[i] = calloc(10, sizeof(char)); 
        
        for(int j = 0; j < 10; j++) board[i][j] = '.';
    }

    int x = 0;
    int y = 0;

    while(1) {
        printf("\nEnter X and Y coordinates (0-9) or -1 -1 to quit: ");
        if (scanf("%d %d", &x, &y) != 2) {
            printf("Invalid input. Please enter numbers.\n");
            while(getchar() != '\n'); 
            continue;
        }

        if (x == -1 && y == -1) break;

        if (x > 9 || x < 0 || y < 0 || y > 9) {
            printf("Numbers out of bounds\n");
            continue;
        }

        board[y][x] = 'X';

        printBoard(board);
    }

    for(int i = 0; i < 10; i++) {
        free(board[i]);
    }
    free(board);

    return 0;
}

void printBoard(char** board) {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            printf("%c  ", board[i][j]);
        }
        printf("\n");
    }
}