#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "moves.h"

typedef struct
{
    char *background;
    char *foreground;
    char value;
} Position;

typedef struct
{
    Player winner;
    Player losser;
    char moves_list[100][7];
    int moves_count;
    char win_condition[25];
} GameRecord;

void win(Player winner, PlayerRecord *winner_record, Player losser, PlayerRecord *losser_record, char *condition, GameRecord *game_record);
void save_game_record(GameRecord *game_record);
void game();
void print_games_record();
void display_moves(int option);
void turn_options(Player *player, Player *enemy, int *turn, Position (*board)[8][8], PlayerRecord *record_player_1, PlayerRecord *record_player_2, GameRecord *game_record, int *size_required_moves, int *size_available_moves, char ***available_moves, char ***required_moves, int *turn_counter);

void win(Player winner, PlayerRecord *winner_record, Player losser, PlayerRecord *losser_record, char *condition, GameRecord *game_record)
{
    winner_record->wins++;

    if (check_move(condition, "Evens")) losser_record->wins++;
    else losser_record->losts++;

    game_record->winner = winner;
    game_record->losser = losser;
    strcpy(game_record->win_condition, condition);

    save_player_record(winner_record);
    save_player_record(losser_record);
    save_game_record(game_record);

    printf("Player: %s\nPieces Left: %d\nMoves count: %d\nTotal Moves: %d\nCondition: %s\n\n(Press enter to return to the main menu)\n", winner.name, winner.pieces_left, winner.moves_count, game_record->moves_count, condition);
    fgetc(stdin);

    menu();
}

void save_game_record(GameRecord *game_record) {
    FILE* games_record = open_file("./games_record.dat", "ab");
    fwrite(game_record, sizeof(GameRecord), 1, games_record);
    fclose(games_record);
}

void game(Position (*board)[8][8], Player *player_1, Player *player_2, PlayerRecord *record_player_1, PlayerRecord *record_player_2, GameRecord *game_record, int *turn, int *size_required_moves, int *size_available_moves, char ***available_moves, char ***required_moves, int *turn_counter)
{
    display_board(*board);

    if (!player_1->pieces_left) win(*player_2, record_player_2, *player_1, record_player_1, "Out of pieces", game_record);
    else if (!player_2->pieces_left) win(*player_1, record_player_1, *player_2, record_player_2, "Out of pieces", game_record);

    fill_moves_lists(1, *turn, board, size_required_moves, required_moves);

    if (!(*size_required_moves))
    {
        fill_moves_lists(0, *turn, board, size_available_moves, available_moves);

        if (!size_available_moves) win((*turn) ? *player_1 : *player_2, (*turn) ? record_player_1 : record_player_2, !(*turn) ? *player_1 : *player_2, !(*turn) ? record_player_1 : record_player_2, "Out of options", game_record);
    }

    if (!(*turn)) turn_options(player_1, player_2, turn, board, record_player_1, record_player_2, game_record, size_required_moves, size_available_moves, available_moves, required_moves, turn_counter);
    else turn_options(player_2, player_1, turn, board, record_player_1, record_player_2, game_record, size_required_moves, size_available_moves, available_moves, required_moves, turn_counter);;
}

void print_games_record() {
    clear_output();
    FILE* games_record = open_file("./games_record.dat", "rb");

    fseek(games_record, 0, SEEK_END);
    int file_size = ftell(games_record) / sizeof(GameRecord);

    GameRecord temp_record;

    for (int index = 0; index < file_size; index++)
    {
        fseek(games_record, sizeof(GameRecord) * index, SEEK_SET);

        fread(&temp_record, sizeof(GameRecord), 1, games_record);
        printf("%d -\n\tWinner: %s\n\tLosser: %s\n\tMoves count: %d\n\tWin condition: %s\n\n", index, temp_record.winner.name, temp_record.losser.name, temp_record.moves_count, temp_record.win_condition);
    }

    printf("Chose on of the options to display algebraic notation or a -1 to go back to the main menu\n");
    int option;
    scanf("%d", &option);
    fgetc(stdin);
    if (option < 0) {
        menu();
        return;
    } else {
        display_moves(option);
    }

    menu();
}

void display_moves(int option) {
    clear_output();
    FILE* games_record = open_file("./games_record.dat", "rb");

    GameRecord temp_record;
    fseek(games_record, sizeof(GameRecord) * option, SEEK_SET);
    fread(&temp_record, sizeof(GameRecord), 1, games_record);

    printf("\n\tTurn\tBlacks\tWhites\n");
    int turn = 1;
    for (int index = 0; index < temp_record.moves_count; index++) {
        if (!(index % 2)) printf("\t%d\t", turn++);
        printf("%s\t", temp_record.moves_list[index]);

        if (index % 2) printf("\n");
    }

    printf("\nPress enter to go back to the main menu");
    fgetc(stdin);
}

void turn_options(Player *player, Player *enemy, int *turn, Position (*board)[8][8], PlayerRecord *record_player_1, PlayerRecord *record_player_2, GameRecord *game_record, int *size_required_moves, int *size_available_moves, char ***available_moves, char ***required_moves, int *turn_counter)
{
    char *player_color;
    if (!(*turn)) player_color = "Blacks";
    else player_color = "Whites";

    printf("Turn #%d, Player %s, %s:\n\t1- Play\n\t2- Show moves available\n\t3- Give up\n\t4- Ask for evens\n\n\t", *turn_counter, player->name, player_color);

    switch (fgetc(stdin))
    {
    case '1':
        if (*size_required_moves) highlight(1, board, size_required_moves, required_moves);
        else highlight(1, board, size_available_moves, available_moves);
        move(player, enemy, turn, board, game_record, turn_counter, size_required_moves, size_available_moves, available_moves, required_moves, record_player_1, record_player_2);
        break;
    case '2':
        getchar();
        if (*size_required_moves) highlight(0, board, size_required_moves, required_moves);
        else highlight(0, board, size_available_moves, available_moves);
        game(board, player, enemy, record_player_1, record_player_2, game_record, turn, size_required_moves, size_available_moves, available_moves, required_moves, turn_counter);
        break;
    case '3':
        getchar();
        win((*turn) ? *player : *enemy, (*turn) ? record_player_1 : record_player_2, !(*turn) ? *player : *enemy, !(*turn) ? record_player_1 : record_player_2, "Give up", game_record);
        break;
    case '4':
        getchar();
        printf("\n%s wants to call this an even, are you ok with it? (1 for Yes, anything else for No)", player_color);
        if (fgetc(stdin) == '1') {
            getchar();
            win(!(*turn) ? *player : *enemy, !(*turn) ? record_player_1 : record_player_2, (*turn) ? *player : *enemy, turn ? record_player_1 : record_player_2, "Evens", game_record);
        }
        else game(board, player, enemy, record_player_1, record_player_2, game_record, turn, size_required_moves, size_available_moves, available_moves, required_moves, turn_counter);

        break;
    default:
        printf("Please select one of the options on the screen (Press enter to continue)\n");
        fgetc(stdin);
        game(board, player, enemy, record_player_1, record_player_2, game_record, turn, size_required_moves, size_available_moves, available_moves, required_moves, turn_counter);
        break;
    }
}


#endif // GAME_H
