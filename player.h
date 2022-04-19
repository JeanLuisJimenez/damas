#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

typedef struct
{
    char name[100];
    int color; // 0 for white, 1 for black
    int pieces_left;
    int moves_count;
} Player;

typedef struct
{
    char player_name[100];
    int wins;
    int losts;
} PlayerRecord;

void create_player (Player *player, PlayerRecord *record_player, int num);
void save_player_record (PlayerRecord const *player_record);
void search_player_record (Player player, PlayerRecord *player_record);
void print_players_record ();

void create_player (Player *player, PlayerRecord *record_player, int num)
{
    printf("Insert player %d name: ", num);

    fgets(player->name, 50, stdin);
    player->name[strlen(player->name) - 1] = '\0';

    player->color = num == 1 ? 0 : 1;
    player->pieces_left = 12;
    player->moves_count = 0;

    search_player_record(*player, record_player);
}

void search_player_record (Player player, PlayerRecord *player_record)
{
    FILE* players_record = open_file("./player_record.dat", "r+b");

    fseek(players_record, 0, SEEK_END);
    int file_size = ftell(players_record) / sizeof(PlayerRecord);

    PlayerRecord temp_record;
    int found = 0;
    for (int index = 0; index < file_size; index++)
    {
        fseek(players_record, sizeof(PlayerRecord) * index, SEEK_SET);

        fread(&temp_record, sizeof(PlayerRecord), 1, players_record);
        if (!!temp_record.player_name && check_move(temp_record.player_name, player.name))
        {
            found = 1;
            fclose(players_record);
            break;
        }
    }

    if (!found)
    {
        fclose(players_record);
        strcpy(player_record->player_name, player.name);
        player_record->losts = 0;
        player_record->wins = 0;
    }
    else
    {
        *player_record = temp_record;
    }
}

void save_player_record (PlayerRecord const *player_record)
{
    FILE* players_record = open_file("./player_record.dat", "r+b");

    fseek(players_record, 0, SEEK_END);
    int file_size = ftell(players_record) / sizeof(PlayerRecord);
    PlayerRecord temp_record;

    int found = 0;
    for (int index = 0; index < file_size; index++)
    {
        fseek(players_record, index * sizeof(PlayerRecord), SEEK_SET);
        fread(&temp_record, sizeof(PlayerRecord), 1, players_record);
        if (!!temp_record.player_name && check_move(temp_record.player_name, player_record->player_name))
        {
            fseek(players_record, index * sizeof(PlayerRecord), SEEK_SET);
            fwrite(player_record, sizeof(PlayerRecord), 1, players_record);
            fclose(players_record);
            found = 1;
            return;
        }
    }

    fseek(players_record, 0, SEEK_END);
    fwrite(player_record, sizeof(PlayerRecord), 1, players_record);
    fclose(players_record);
}

void print_players_record () {
    clear_output();
    FILE* players_record = open_file("./player_record.dat", "rb");

    fseek(players_record, 0, SEEK_END);
    int file_size = ftell(players_record) / sizeof(PlayerRecord);

    PlayerRecord temp_record;

    for (int index = 0; index < file_size; index++)
    {
        fseek(players_record, sizeof(PlayerRecord) * index, SEEK_SET);

        fread(&temp_record, sizeof(PlayerRecord), 1, players_record);
        printf("Player Name: %s\nWins: %d\nLosts: %d\n\n", temp_record.player_name, temp_record.wins, temp_record.losts);
    }

    printf("Press enter to go back to the main menu\n");
    fgetc(stdin);

    menu();
}

#endif // PLAYER_H
