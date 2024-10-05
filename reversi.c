#define REVERSI_H
#ifndef REVERSI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lab8part1.h"

void initializeBoard(char board[][26], int dimension);
void printBoard(char board[][26], int dimension);
bool positionInBounds(int dimension, int row, int col);
bool checkLegalInDirection(char board[][26], int dimension, int row, int col, char color, int dRow, int dCol, char availableMoves[][3], int* numMoves);
void findAllAvailableMoves(char board[][26], int dimension, char color, char availableMoves[][3], int* numMoves);
bool isValidMove(char move[], int numMoves, char availableMoves[][3]);
void applyMove(char move[], char board[][26], int dimension, char color);
void computerPlay(char board[][26], int dimension, char color);
bool isGameFinished(char board[][26], int dimension, char compColor, char availableMoves[][3], int numMoves);

#endif

void initializeBoard(char board[][26], int dimension) 
{
    for(int i = 0; i < dimension; i++)
    {
        for(int j = 0; j < dimension; j++)
        {  
            board[i][j] = 'U'; 
        }
    }

    int center = dimension / 2;
    board[center - 1][center - 1] = 'W';
    board[center][center] = 'W';
    board[center - 1][center] = 'B';
    board[center][center - 1] = 'B';
}

void printBoard(char board[][26], int dimension) 
{
    printf(" ");
    for(int c = 0; c < dimension; c++)
    {
        printf("%c", 'a' + c);
    }
    printf("\n");

    for(int r = 0; r < dimension; r++)
    {
        printf("%c", 'a' + r);
        for(int c = 0; c < dimension; c++)
        {
            printf("%c", board[r][c]);    
        }
        printf("\n");
    }
}

bool positionInBounds(int dimension, int row, int col) 
{
    return (row >= 0 && row < dimension && col >= 0 && col < dimension);
}

bool checkLegalInDirection(char board[][26], int dimension, int row, int col, char color, int dRow, int dCol, char availableMoves[][3], int* numMoves) 
{      
    int r = row + dRow;
    int c = col + dCol;
    int opponentCount = 0;

    while (positionInBounds(dimension, r, c) && board[r][c] != 'U') 
    {
        if (board[r][c] == color) 
        {
            if (opponentCount > 0) 
            {
                availableMoves[*numMoves][0] = row + 'a';
                availableMoves[*numMoves][1] = col + 'a';
                availableMoves[*numMoves][2] = '\0';
                (*numMoves)++;
                return true;
            }
            else 
            {
                return false;
            }
        }
        else 
        {
            opponentCount++;
        }
        r += dRow;
        c += dCol;
    }
    return false;
}

void findAllAvailableMoves(char board[][26], int dimension, char color, char availableMoves[][3], int* numMoves)
{
    for(int r = 0; r < dimension; r++) 
    {
        for(int c = 0; c < dimension; c++) 
        {
            if(board[r][c] == 'U') 
            {
                for(int i = 0; i < 8; i++)
                {
                    checkLegalInDirection(board, dimension, r, c, color, deltaRow[i], deltaCol[i], availableMoves, numMoves);
                }
            }
        }
    }
}

bool isValidMove(char move[], int numMoves, char availableMoves[][3])
{
    for(int i = 0; i < numMoves; i++)
    {
        if(availableMoves[i][0] == move[0] && availableMoves[i][1] == move[1]) 
        {
            return true;
        }
    }
    return false;
}

void applyMove(char move[], char board[][26], int dimension, char color)
{
    int row = move[0] - 'a';
    int col = move[1] - 'a';
    board[row][col] = color;

    for(int i = 0; i < 8; i++)
    {
        if(checkLegalInDirection(board, dimension, row, col, color, deltaRow[i], deltaCol[i], NULL, NULL))
        {
            int r = row + deltaRow[i];
            int c = col + deltaCol[i];

            while (board[r][c] != color)
            {
                board[r][c] = color;
                r += deltaRow[i];
                c += deltaCol[i];
            }
        }
    }
}

void computerPlay(char board[][26], int dimension, char color)
{
    char availableMoves[26*26][3];
    int numMoves = 0;

    findAllAvailableMoves(board, dimension, color, availableMoves, &numMoves);

    if (numMoves > 0) 
    {
        applyMove(availableMoves[0], board, dimension, color);
        printf("Computer places %c at %c%c.\n", color, availableMoves[0][0], availableMoves[0][1]);
    }

    printBoard(board, dimension);
}

bool isGameFinished(char board[][26], int dimension, char compColor, char availableMoves[][3], int numMoves)
{
    numMoves = 0;
    findAllAvailableMoves(board, dimension, (compColor == 'W' ? 'B' : 'W'), availableMoves, &numMoves);

    int numMoves2 = 0;
    findAllAvailableMoves(board, dimension, compColor, availableMoves, &numMoves2);

    return (numMoves == 0 && numMoves2 == 0);
}

int main(void) 
{
    int dimension;
    int numl = 0;
    char availableMoves[26*26][3];
    int numMoves = 0;
    char compColor;
    char move[3];

    printf("Enter the board dimension: ");
    scanf("%d", &dimension);

    printf("Computer plays (B/W): ");
    scanf(" %c", &compColor);

    char board[26][26];
    initializeBoard(board, dimension);
    printBoard(board, dimension);

    while (true)
    {
        if (compColor == 'W')
        {
            computerPlay(board, dimension, compColor);
            if (isGameFinished(board, dimension, compColor, availableMoves, numMoves))
            {
                break;
            }
            
            printf("Enter move for colour B (RowCol): ");
            scanf("%s", move);
            if (!isValidMove(move, numMoves, availableMoves))
            {
                printf("Invalid move.\n");
                printf("W player wins.");
                numl = 1;
                break;
            }
            applyMove(move, board, dimension, 'B');
        }
        else
        {
            printf("Enter move for colour W (RowCol): ");
            scanf("%s", move);
            if (!isValidMove(move, numMoves, availableMoves))
            {
                printf("Invalid move.\n");
                printf("B player wins.");
                numl = 1;
                break;
            }
            applyMove(move, board, dimension, 'W');
            if (isGameFinished(board, dimension, compColor, availableMoves, numMoves))
            {
                break;
            }

            computerPlay(board, dimension, compColor);
        }

        if (isGameFinished(board, dimension, compColor, availableMoves, numMoves))
        {
            break;
        }
    }

    int countB = 0, countW = 0;
    for(int i = 0; i < dimension; i++)
    {
        for(int j = 0; j < dimension; j++)
        {  
            if(board[i][j] == 'B')
            {
                countB++;
            }
            else if(board[i][j] == 'W')
            {
                countW++;
            }
        }
    }

    if(numl != 1)
    {
        if(countB > countW) 
        {
            printf("B player wins.\n");
        } 
        else if(countW > countB)
        {
            printf("W player wins.\n");
        } 
        else
        {
            printf("Draw!\n");
        }
    }

    return 0;
}
