#ifndef MOVES_H
#define MOVES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "board.h"

void move(Player *player, Player *enemy, int *turn, Position (*board)[8][8], GameRecord *game_record, int *turn_counter, int *size_required_moves, int *size_available_moves, char ***available_moves, char ***required_moves, PlayerRecord *record_player_1, PlayerRecord *record_player_2);
char* get_move(int row, int col, int m_row, int m_col, Position board[8][8]);
void check(int row, int col, int eat, int up, char ***list, char* search, int *size, Position (*board)[8][8]);
void check_queen(int row, int col, int eat, int up, int right, char ***list, char* search, int *size, Position (*board)[8][8]);
void fill_moves_lists (int required, int turn, Position (*board)[8][8], int* size, char ***list);
int check_moves_list (char *movement, char **moves_list, int size);
int check_move(char *movement, char *required_move);
int getCol(char col);
int getRow(char row);

void move(Player *player, Player *enemy, int *turn, Position (*board)[8][8], GameRecord *game_record, int *turn_counter, int *size_required_moves, int *size_available_moves, char ***available_moves, char ***required_moves, PlayerRecord *record_player_1, PlayerRecord *record_player_2)
{
    char current_position[4];
    char next_position[3];
    printf("Specify your move (Example: Pa1 b2)");
    scanf("%s %s", current_position, next_position);
    // current_position: Pa3, next_position: b4
    getchar();

    char *valid_piece;
    if (!(*turn)) valid_piece = "PD";
    else valid_piece = "pd";

    if ((current_position[0] == valid_piece[0]) || (current_position[0] == valid_piece[1]))
    {
        char *movement = (char*) calloc(7, sizeof(char));
        sprintf(movement, "%s %s", current_position, next_position);
        // movement: Pa3 b4
        int current_col = getCol(current_position[1]), current_row = getRow(current_position[2]), next_col = getCol(next_position[0]), next_row = getRow(next_position[1]);

        if ((*size_required_moves) && check_moves_list(movement, *required_moves, *size_required_moves))
        {
            int row_direction;
            int col_direction;

            if ((next_row - current_row) > 0) row_direction = 1;
            else if ((next_row - current_row) < 0) row_direction = -1;

            if ((next_col - current_col) > 0) col_direction = 1;
            else if ((next_col - current_col) < 0) col_direction = -1;

            (*board)[current_row][current_col].value = ' ';
            (*board)[next_row][next_col].value = current_position[0];
            (*board)[next_row - row_direction][next_col - col_direction].value = ' ';

            if (!(*turn))
                if (!next_row) (*board)[next_row][next_col].value = 'D';
            else
                if (next_row == 7) (*board)[next_row][next_col].value = 'd';

            enemy->pieces_left--;

            player->moves_count++;
            strcpy(game_record->moves_list[game_record->moves_count++], movement);
        }
        else if (*size_available_moves && check_moves_list(movement, *available_moves, *size_available_moves))
        {
            (*board)[current_row][current_col].value = ' ';
            (*board)[next_row][next_col].value = current_position[0];

            if (!(*turn) && !next_row)
                (*board)[next_row][next_col].value = 'D';
            else if ((*turn) && (next_row == 7))
                (*board)[next_row][next_col].value = 'd';

            player->moves_count++;
            strcpy(game_record->moves_list[game_record->moves_count++], movement);
        }
        else
        {
            printf("Try with a valid movement (Press enter to try again)\n");
            fgetc(stdin);

            move(player, enemy, turn, board, game_record, turn_counter, size_required_moves, size_available_moves, available_moves, required_moves, record_player_1, record_player_2);
            return;
        }
    }
    else
    {
        printf("Try with a valid movement (Press enter to try again)\n");
        fgetc(stdin);

        move(player, enemy, turn, board, game_record, turn_counter, size_required_moves, size_available_moves, available_moves, required_moves, record_player_1, record_player_2);
        return;
    }


    if (*size_required_moves)
    {
        fill_moves_lists(1, *turn, board, size_required_moves, required_moves);

        if (*size_required_moves)
        {
            highlight(1, board, size_required_moves, required_moves);
            game(board, !(*turn) ? player : enemy, (*turn) ? player : enemy, record_player_1, record_player_2, game_record, turn, size_required_moves, size_available_moves, available_moves, required_moves, turn_counter);
            return;
        }
    }

    *turn_counter += *turn;
    *turn = !(*turn);
    game(board, !(*turn) ? player : enemy, (*turn) ? player : enemy, record_player_1, record_player_2, game_record, turn, size_required_moves, size_available_moves, available_moves, required_moves, turn_counter);
}

void fill_moves_lists (int required, int turn, Position (*board)[8][8], int* size, char ***list)
{
    char *search;
    if (!required) search = "  ";
    else
        if (!turn) search = "pd";
        else search = "PD";

    free(*list);
    *size = 0;
    *list = (char **) calloc(50, sizeof(char**));

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            // 0 negras, 1 blancas
            if (!turn && ((*board)[row][col].value == 'P'))
                check(row, col, required, -1, list, search, size, board);
            else if (turn && ((*board)[row][col].value == 'p'))
                check(row, col, required, 1, list, search, size, board);
            else if (!turn && ((*board)[row][col].value == 'D'))
            {
                check_queen(row, col, required, 1, 1, list, search, size, board);
                check_queen(row, col, required, 1, -1, list, search, size, board);
                check_queen(row, col, required, -1, 1, list, search, size, board);
                check_queen(row, col, required, -1, -1, list, search, size, board);
            }
            else if (turn && ((*board)[row][col].value == 'd'))
            {
                check_queen(row, col, required, 1, 1, list, search, size, board);
                check_queen(row, col, required, 1, -1, list, search, size, board);
                check_queen(row, col, required, -1, 1, list, search, size, board);
                check_queen(row, col, required, -1, -1, list, search, size, board);
            }
        }
    }
}

void check_queen(int row, int col, int eat, int up, int right, char ***list, char* search, int *size, Position (*board)[8][8]) {
    for (int offset = 1; (row + (offset * up) < 8) && (row + (offset * up) >= 0); offset++) {
        int n_row = row + (offset * up);
        int n_col = col + (offset * right);

        if (((*board)[n_row][n_col].value == search[0] || (*board)[n_row][n_col].value == search[1]) && (*board)[n_row + (eat * up)][n_col + (eat * right)].value == ' ') {
            (*list)[*size] = get_move(row, col, n_row + (eat * up), n_col + (eat * right), board);
            (*size)++;
        } else if (!eat && ((*board)[n_row][n_col].value != search[0] && (*board)[n_row][n_col].value != search[1]))
            break;
    }
}

void check(int row, int col, int eat, int up, char ***list, char* search, int *size, Position (*board)[8][8])
{
    if ((row + (up * (eat + 1)) >= 0) && (row + (up * (eat + 1)) < 8))
    {
        if ((col - (1 + eat) >= 0) && (((*board)[row + up][col - 1].value == search[0]) || ((*board)[row + up][col - 1].value == search[1])) && ((*board)[row + (up * (eat + 1))][col - (1 + eat)].value == ' '))
        {
            (*list)[*size] = get_move(row, col, row + (up * (eat + 1)), col - (1 + eat), board);
            (*size)++;
        }
        if ((col + (1 + eat) < 8) && (((*board)[row + up][col + 1].value == search[0]) || ((*board)[row + up][col + 1].value == search[1])) && ((*board)[row + (up * (eat + 1))][col + (1 + eat)].value == ' '))
        {
            (*list)[*size] = get_move(row, col, row + (up * (eat + 1)), col + (1 + eat), board);
            (*size)++;
        }
    }
}

char* get_move(int row, int col, int m_row, int m_col, Position board[8][8])
{
    char* return_move = (char *) calloc(7, sizeof(char));
    sprintf(return_move, "%c%c%d %c%d", board[row][col].value, col + 97, (8 - row), m_col + 97, 8 - m_row);

    return return_move;
}

int check_moves_list (char *movement, char **moves_list, int size)
{
    if (size)
    {
        for (int index = 0; index < size; index++)
        {
            if (check_move(movement, moves_list[index])) return 1;
        }
        return 0;
    }

    return -1;
}

int check_move(char *movement, char *required_move)
{
    for (int w_index = 0; w_index < strlen(movement); w_index++)
        if (!(movement[w_index] == required_move[w_index]))
            return 0;

    return 1;
}

int getCol(char col)
{
    return tolower(col) - 97;
}

int getRow(char row)
{
    return 8 - (row - 48);
}

#endif // MOVES_H
