#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define ALLEGRO_NO_MAGIC_MAIN
// gcc main.c -lallegro -lallegro_main -I /usr/local/include
// gcc main.c -lglog -lceres -I /usr/local/include

#define TRUE 1
#define FALSE 0

#define row 9
#define col 9

enum Card
{
    closed_door,
    multiply,
    limit,
    repeat_toss,
};

struct Player
{
    char name[12];
    int position_first_piece[2];
    int position_second_piece[2];
    enum Card lucky_card_list[10];
};

struct Cell
{
    int row_index;
    int col_index;
    int is_lucky;
    enum Card lucky_card;
    int is_middle;
    int is_corridor;
    int corridor_to[2];
};

int dice()
{
    int dice_numbers[6] = {-3, -2, -1, 1, 2, 3};
    int random_number = rand() % 6;
    return dice_numbers[random_number];
}

void print_table(struct Cell table[row][col])
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            struct Cell cell = table[i][j];
            printf("[%d, %d] | is Middle? %d | is Lucky? %d | is Corridor? %d", cell.row_index, cell.col_index, cell.is_middle, cell.is_lucky, cell.is_corridor);
            if (cell.is_corridor)
            {
                printf(" | Corridor to [%d, %d]\n", cell.corridor_to[i], cell.corridor_to[j]);
            }
            else
            {
                printf("\n");
            }
            if (j == col - 1)
                printf("\n");
        }
    }
}

int main(void)
{

    srand(time(NULL));

    al_init();
    // al_init_font_addon();
    // al_init_ttf_addon();

    ALLEGRO_DISPLAY *display = al_create_display(400, 640);
    // ALLEGRO_FONT *font = al_load_ttf_font("./ROBOTO.ttf", 64, 0);

    struct Player player1;
    struct Player player2;

    struct Cell table[row][col];

    /////////////// This needs to go into a function, called create table

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            struct Cell cell;
            cell.is_middle = 0;
            cell.is_corridor = 0;
            cell.is_lucky = 0;

            cell.row_index = i;
            cell.col_index = j;

            if (i == 5 && j == 5)
                cell.is_middle = 1;

            int cell_is_lucky = !(rand() % 5);
            if (cell_is_lucky)
            {
                cell.is_lucky = 1;
                int which_card = rand() % 4;
                enum Card cards[4] = {closed_door, multiply, limit, repeat_toss};
                cell.lucky_card = cards[which_card];
            }

            table[i][j] = cell;
        }
    }

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            struct Cell cell = table[i][j];

            if (!cell.is_corridor)
            {
                int is_cell_corridor = !(rand() % 10);
                if (is_cell_corridor)
                {
                    cell.is_corridor = TRUE;
                    int corridor_row = rand() % 7 + 1;
                    int corridor_col = rand() % 7 + 1;

                    cell.corridor_to[0] = corridor_row;
                    cell.corridor_to[1] = corridor_col;

                    struct Cell pair_cell = table[corridor_row][corridor_col];
                    pair_cell.is_corridor = TRUE;
                    pair_cell.corridor_to[0] = i;
                    pair_cell.corridor_to[1] = j;

                    table[i][j] = cell;
                    table[corridor_row][corridor_col] = pair_cell;
                }
            }
        }
    }

    ///////////////

    /////////////// This needs to go into a function, called print table

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            struct Cell cell = table[i][j];
            printf("[%d, %d] | is Middle? %d | is Lucky? %d", cell.row_index, cell.col_index, cell.is_middle, cell.is_lucky);
            if (cell.is_lucky)
            {
                printf(" | Lucky card is [%d]", cell.lucky_card);
            }
            printf(" | is Corridor? %d", cell.is_corridor);
            if (cell.is_corridor)
            {
                printf(" | Corridor to [%d, %d]\n", cell.corridor_to[0], cell.corridor_to[1]);
            }
            else
            {
                printf("\n");
            }
            if (j == col - 1)
                printf("\n");
        }
    }

    while (1)
    {
        al_clear_to_color(al_map_rgb(255, 255, 255));
        // al_draw_text(font, al_map_rgb(3, 3, 3), 0, 0, 0, "Hello world!");
    }

    // al_destroy_font(font);
    al_destroy_display(display);

    return 1;
}