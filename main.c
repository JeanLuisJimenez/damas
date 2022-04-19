#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "utils.h"
#include "player.h"
#include "moves.h"
#include "game.h"

void menu();

Position board[8][8];
Player player_1;
Player player_2;
GameRecord game_record;
PlayerRecord record_player_1;
PlayerRecord record_player_2;
int turn = 0; // 1 for whites, 0 for blacks
int turn_counter = 1;

char **required_moves;
int size_required_moves = 0;

char **available_moves;
int size_available_moves = 0;

int main()
{
    fclose(open_file("./player_record.dat", "ab"));
    menu();

    return 0;
}

void menu()
{
    clear_output();
    printf("\n\t\tWelcome to Checkers Game\n\t");
    printf("Menu:\n\t\t1- Play\n\t\t2- See player records log\n\t\t3- See games records\n\n\t\t0- Exit\n\n");

    switch (fgetc(stdin))
    {
    case '1':
        clear_output();
        getchar();
        create_player(&player_1, &record_player_1, 1);
        create_player(&player_2, &record_player_2, 2);

        build_board(&board);
        game(&board, &player_1, &player_2, &record_player_1, &record_player_2, &game_record, &turn, &size_required_moves, &size_available_moves, &available_moves, &required_moves, &turn_counter);
        break;
    case '2':
        getchar();
        print_players_record();
        break;
    case '3':
        getchar();
        print_games_record();
        break;
    case '0':
        exit(0);
    default:
        printf("Please select one of the options on the screen (Press any key to continue)\n");
        fgetc(stdin);

        menu();

        break;
    }
}
