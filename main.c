#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

// #include <allegro5/allegro.h>
// #include <allegro5/allegro_color.h>
// #include <allegro5/allegro_font.h>
// #include <allegro5/allegro_ttf.h>

// #define ALLEGRO_NO_MAGIC_MAIN
// gcc main.c -lallegro -lallegro_main -I /usr/local/include
// gcc main.c -lglog -lceres -I /usr/local/include

#define TRUE 1
#define FALSE 0

#define row 9
#define col 9
#define total 81

enum Card
{
    closed_door,
    multiply,
    limit,
    repeat_toss,
};

struct Player
{
    char name[50];
    int pieces[2];
    enum Card lucky_card_list[10];
    int lucky_cards_count;
};

struct Cell
{
    int index;
    int is_lucky;
    enum Card lucky_card;
    int is_middle;
    int is_corridor;
    int corridor_to;
};

int dice()
{
    int dice_numbers[6] = {-3, -2, -1, 1, 2, 3};
    int random_number = rand() % 6;
    return dice_numbers[random_number];
}

int player_can_move(int dice_number, struct Player player)
{
    if (player.pieces[0] + dice_number >= 0 && player.pieces[0] + dice_number < total)
        return 1;
    else if (player.pieces[1] + dice_number >= 0 && player.pieces[1] + dice_number < total)
        return 1;
    return 0;
}

int is_valid_move(int dice_number, int position)
{
    if (position + dice_number >= 0 && position + dice_number < total)
        return 1;
    return 0;
}

void move(int dice_number, struct Player *player, int piece_number, struct Cell table[])
{

    player->pieces[piece_number - 1] += dice_number;
    if (table[player->pieces[piece_number - 1]].is_lucky)
    {
        player->lucky_card_list[player->lucky_cards_count] = table[player->pieces[piece_number - 1]].lucky_card;
        player->lucky_cards_count++;
        printf("Oh you got a lucky card! your cards now are as follow:\n");
        for (int i = 0; i < player->lucky_cards_count; i++)
        {
            printf("|%d", player->lucky_card_list[i]);
        }
        printf("|\n");
    }
    if (table[player->pieces[piece_number - 1]].is_corridor)
    {
        printf("Oh you are in a corridor!\n");
        player->pieces[piece_number - 1] = table[player->pieces[piece_number - 1]].corridor_to;
    }
}

int check_for_win(struct Player player1, struct Player player2)
{
    if (player1.pieces[0] == 40 && player1.pieces[1] == 40 || player2.pieces[0] == 40 && player2.pieces[1] == 40)
        return 1;

    return 0;
}

void create_table(struct Cell table[total])
{
    for (int i = 0; i < total; ++i)
    {
        struct Cell cell;
        cell.is_middle = FALSE;
        cell.is_corridor = FALSE;
        cell.is_lucky = FALSE;
        cell.index = i;

        if (i == total / 2)
            cell.is_middle = TRUE;

        int cell_is_lucky = !(rand() % 5);
        if (cell_is_lucky)
        {
            cell.is_lucky = TRUE;
            int which_card = rand() % 4;
            enum Card cards[4] = {closed_door, multiply, limit, repeat_toss};
            cell.lucky_card = cards[which_card];
        }

        table[i] = cell;
    }

    for (int i = 0; i < total; ++i)
    {
        struct Cell cell = table[i];

        if (!cell.is_corridor)
        {
            int is_cell_corridor = !(rand() % 10);
            if (is_cell_corridor)
            {
                cell.is_corridor = TRUE;
                int corridor_index = rand() % 80 + 1;
                cell.corridor_to = corridor_index;

                struct Cell pair_cell = table[corridor_index];
                pair_cell.is_corridor = TRUE;
                pair_cell.corridor_to = i;

                table[i] = cell;
                table[corridor_index] = pair_cell;
            }
        }
    }
}

void print_table(struct Cell table[total])
{
    for (int i = 0; i < row; ++i)
    {
        struct Cell cell = table[i];
        printf("[%d] | is Middle? %d | is Lucky? %d", cell.index, cell.is_middle, cell.is_lucky);
        if (cell.is_lucky)
        {
            printf(" | Lucky card is [%d]", cell.lucky_card);
        }
        printf(" | is Corridor? %d", cell.is_corridor);
        if (cell.is_corridor)
        {
            printf(" | Corridor to [%d]\n", cell.corridor_to);
        }
        else
        {
            printf("\n");
        }
        if (i % col - 1 == 0)
            printf("\n");
    }
}

int main(void)
{

    srand(time(NULL));

    // al_init(); ok
    // al_init_font_addon();
    // al_init_ttf_addon();

    // ALLEGRO_DISPLAY *display = al_create_display(400, 640); ok
    // ALLEGRO_FONT *font = al_load_ttf_font("./ROBOTO.ttf", 64, 0);

    struct Player player1;
    struct Player player2;

    int player_turn = 1;
    int is_someone_middle = 0;

    do
    {
        printf("Enter name of player1: ");
        scanf("%s", player1.name);
        player1.pieces[0] = 0;
        player1.pieces[1] = 0;
        player1.lucky_cards_count = 0;

        printf("Enter name of player2: ");
        scanf("%s", player2.name);
        player2.pieces[0] = 80;
        player2.pieces[1] = 80;
        player2.lucky_cards_count = 0;

        !strcmp(player1.name, player2.name) ? printf("Enter names of different players!\n\n") : FALSE;
    } while (!strcmp(player1.name, player2.name));

    struct Cell table[total];

    create_table(table);

    do
    {
        player_turn = ((player_turn % 2) ? 1 : 2);
        if (player_turn == 1)
            printf("\ndicing for %s --- It's number ", player1.name);
        else
            printf("\ndicing for %s --- it's number ", player2.name);

        int dice_number = dice();
        printf("%d\n", dice_number);

        int piece_number;

        if (player_turn == 1)
        {
            if (!player_can_move(dice_number, player1))
                printf("You have no valid moves, try next round!\n");

            while (player_can_move(dice_number, player1))
            {

                printf("Type which piece you want to move? (1 or 2)  ");
                scanf("%d", &piece_number);

                if (piece_number == 1)
                {
                    if (is_valid_move(dice_number, player1.pieces[0]))
                    {
                        move(dice_number, &player1, piece_number, table);
                        break;
                    }
                    else
                        printf("You can't movie this piece! please choose the other one..!\n");
                }
                else if (piece_number == 2)
                {
                    if (is_valid_move(dice_number, player1.pieces[1]))
                    {
                        move(dice_number, &player1, piece_number, table);
                        break;
                    }
                    else
                        printf("You can't movie this piece! please choose the other one..!\n");
                }
                else
                {
                    printf("Wrong Selection... try again!\n");
                }
            }
        }
        else if (player_turn == 2)
        {
            if (!player_can_move(dice_number, player2))
                printf("You have no valid moves, try next round!\n");

            while (player_can_move(dice_number, player2))
            {

                printf("Type which piece you want to move? (1 or 2)  ");
                scanf("%d", &piece_number);

                if (piece_number == 1)
                {
                    if (is_valid_move(dice_number, player2.pieces[0]))
                    {
                        move(dice_number, &player2, piece_number, table);
                        break;
                    }
                    else
                        printf("You can't movie this piece! please choose the other one..!\n");
                }
                else if (piece_number == 2)
                {
                    if (is_valid_move(dice_number, player2.pieces[1]))
                    {
                        move(dice_number, &player2, piece_number, table);
                        break;
                    }
                    else
                        printf("You can't movie this piece! please choose the other one..!\n");
                }
                else
                {

                    printf("Wrong Selection... try again!\n");
                }
            }
        }

        printf("%s's pieces: [%d] | [%d]\n", player1.name, player1.pieces[0], player1.pieces[1]);
        printf("%s's pieces: [%d] | [%d]\n", player2.name, player2.pieces[0], player2.pieces[1]);

        is_someone_middle = check_for_win(player1, player2);
        // TODO update allegro board (to be reminded)
        player_turn++;
    } while (!is_someone_middle);

    if (is_someone_middle)
    {
        if (player_turn == 1)
            printf("%s has won the game *.*;''**^\n", player1.name);
        else
            printf("%s has won the game *.*;''**^\n", player2.name);
    }

    /////////////// This needs to go into a function, called print table

    // for (int i = 0; i < total; ++i)
    // {
    //     struct Cell cell = table[i];
    //     printf("[%d] | is Middle? %d | is Lucky? %d", cell.index, cell.is_middle, cell.is_lucky);
    //     if (cell.is_lucky)
    //     {
    //         printf(" | Lucky card is [%d]", cell.lucky_card);
    //     }
    //     printf(" | is Corridor? %d", cell.is_corridor);
    //     if (cell.is_corridor)
    //     {
    //         printf(" | Corridor to [%d]\n", cell.corridor_to);
    //     }
    //     else
    //     {
    //         printf("\n");
    //     }
    // }

    // printf("/n/n/n");

    // print_table(table);

    // while (1)
    // {
    //     al_clear_to_color(al_map_rgb(255, 255, 255));
    //     // al_draw_text(font, al_map_rgb(3, 3, 3), 0, 0, 0, "Hello world!"); nk!
    // } ok

    // al_destroy_font(font);
    // al_destroy_display(display); ok

    return 1;
}