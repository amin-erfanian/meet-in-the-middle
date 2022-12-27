#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define row 9
#define col 9

struct Player
{
    char name[12];
    int position[2];
};

struct Cell
{
    int row_index;
    int col_index;
    int is_lucky;
    int is_middle;
    int is_corridor;
    int corridor_to[2];
};

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

    struct Player player1;
    struct Player player2;

    struct Cell table[row][col];

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

            int cell_is_not_lucky = rand() % 4;
            if (!cell_is_not_lucky)
                cell.is_lucky = 1;

            int cell_is_not_corridor = rand() % 8;
            if (!cell_is_not_corridor)
            {
                cell.is_corridor = 1;
                int corridor_row = rand() % 7 + 1;
                int corridor_col = rand() % 7 + 1;

                cell.corridor_to[0] = corridor_row;
                cell.corridor_to[1] = corridor_col;
            }

            table[i][j] = cell;
        }
    }

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            struct Cell cell = table[i][j];
            printf("[%d, %d] | is Middle? %d | is Lucky? %d | is Corridor? %d", cell.row_index, cell.col_index, cell.is_middle, cell.is_lucky, cell.is_corridor);
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

    return 1;
}