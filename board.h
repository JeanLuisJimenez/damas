
#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "moves.h"

void display_board (Position board[8][8]);
void build_board(Position (*board)[8][8]);
void SetColor(char *ForgC, char *BackC);
void highlight(int disable, Position (*board)[8][8], int *size, char ***list);

void display_board (Position board[8][8])
{
    clear_output();
    count_cols ();
    for (int row = 0; row < 8; row++)
    {
        printf("\t%d ", 8 - row);
        for (int col = 0; col < 8; col++)
        {
            SetColor(board[row][col].foreground, board[row][col].background);
            printf("%c ", board[row][col].value);
        }

        SetColor("\033[0m", "\033[0m");
        printf(" %d\n", 8 - row);
    }
    count_cols ();
}

void build_board(Position (*board)[8][8])
{
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) {
            //board[row][col].background = "\033[40m";
            //board[row][col].foreground = "\033[37m";
            if (!((col + row) % 2))
            {
                (*board)[row][col].background = "\033[47m";
                (*board)[row][col].foreground = "\033[30m";
                //board[row][col].value = 46;
                (*board)[row][col].value = 32;
            }
            else
            {
                (*board)[row][col].background = "\033[40m";
                (*board)[row][col].foreground = "\033[37m";

                if (row < 3) (*board)[row][col].value = 'p';
                else if (row > 4) (*board)[row][col].value = 'P';
                else (*board)[row][col].value = 32;
            }
        }
}

void SetColor(char *ForgC, char *BackC)
{
    printf("%s%s", ForgC, BackC);
}

void highlight(int disable, Position (*board)[8][8], int *size, char ***list)
{
    for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++)
            if (!disable) {
                for (int index = 0; index < *size; index++)
                    if ((getCol((*list)[index][4]) == col) && (getRow((*list)[index][5]) == row))
                        (*board)[row][col].background = "\033[44m";
            }
            else if ((col + row) % 2)
            {
                (*board)[row][col].background = "\033[40m";
                continue;
            }
}

#endif // BOARD_H
