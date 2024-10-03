#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int i, j;
int row = 16;
int column = 16;
int board_size = 16;
int counter = 0;
int counter1 = 0;
char board[256];
int ind;
int random2 = 0;
int random1 = 0;
int playerPosition;
int totalECTS = 0;

void print_board() /* Printing all 16x16 board*/
{
    int sign = 0;
    int j = 0;
    int k = 0;
    for (j = 0; j < 16; j++)
    {
        for (k = 0; k < 16; k++)
        {
            printf("%c ", board[sign]);
            sign++;
        }
        printf("\n");
    }
}

int get_index(int row, int col) /* Using 1D array for look like 2D array*/
{
    return row * board_size + col;
}

void initialize_game()
{
    srand(time(NULL));

    int w1_space_c = 8;  /* Spaces between wall 1 and player */
    int w2_space_c = 24; /* Spaces between wall 2 and wall 1 */

    for (i = 0; i < board_size; i++)
    {
        for (j = 0; j < board_size; j++)
        {
            /*Making 2 br wall away from P letter*/
            if ((abs(i - row / 2) == 2 && abs(j - column / 2) <= 2) || (abs(j - column / 2) == 2 && abs(i - row / 2) <= 2))
            {
                /*Not including 1 br away from P letter*/
                if (!(abs(i - row / 2) == 1 && abs(j - column / 2) <= 1) && !(abs(j - column / 2) == 1 && abs(i - row / 2) <= 1))
                {
                    board[ind] = '#';
                    ind++;
                }
                else
                {
                    board[ind] = '.';
                    ind++;
                }
            }
            /*Making 4 br second wall away from P letter*/
            else if ((abs(i - row / 2) == 4 && abs(j - column / 2) <= 4) || (abs(j - column / 2) == 4 && abs(i - row / 2) <= 4))
            {
                board[ind] = '#';
                ind++;
            }
            /* Printing P letter*/
            else if (i == row / 2 && j == column / 2)
            {
                board[ind] = 'P';
                ind++;
            }
            /* Printing X letter*/
            else if (i == 15 && j == 15)
            {
                board[ind] = 'X';
                ind++;
            }
            else if ((abs(j - column / 2) <= 1) && abs(i - row / 2) <= 1) /*Making 8 br area */
            {
                random2 = rand() % 2; /*making random numbers*/

                if (random2 == 1 && counter < 4) /*initalize true number for area*/
                {
                    board[ind] = '1';
                    ind++;
                    counter++;
                }
                else if (w1_space_c <= 4 && counter < 4)
                {
                    board[ind] = '1';
                    ind++;
                    counter++;
                }
                else
                {
                    board[ind] = '.';
                    ind++;
                }

                w1_space_c--;
            }
            else if ((abs(j - column / 2) <= 4 && abs(i - row / 2) <= 4)) /*Making 24 br area */
            {
                random1 = rand() % 2; /*making random numbers*/

                if (random1 == 1)
                {
                    board[ind] = '2';
                    ind++;
                    counter1++;
                }
                else if (w2_space_c <= 3 && counter1 <= 3) /*initalize true number for area*/
                {
                    board[ind] = '2';
                    ind++;
                    counter1++;
                }
                else
                {
                    board[ind] = '.';
                    ind++;
                }

                w2_space_c--;
            }
            else
            {
                board[ind] = '.';
                ind++;
            }
        }
    }
}
int can_move(int newRow, int newCol)
{
    /* Check if the new position is within the board boundaries*/
    if (newRow < 0 || newRow >= board_size || newCol < 0 || newCol >= board_size)
    {
        return 0;
    }
    /*Check if the new position is a wall*/
    if (board[get_index(newRow, newCol)] == '#')
    {
        /*Check if the player has enough ECTS to pass the wall*/
        if ((newRow == 6 || newRow == 10 || newCol == 6 || newCol == 10) && totalECTS < 32)
        {
            printf("You need at least 32 ECTS to pass the first year wall.\n");
            return 0;
        }

        if ((newRow == 4 || newRow == 12 || newCol == 4 || newCol == 12) && totalECTS < 56)
        {
            printf("You need at least 24 ECTS to pass the second year wall.\n");
            return 0;
        }
    }
    else if (board[get_index(newRow, newCol)] == '1')
    {
        totalECTS += 8;
    }
    else if (board[get_index(newRow, newCol)] == '2')
        totalECTS += 8;

    /*If the move is valid*/
    return 1;
}

void move_player(char direction)
{
    /* Calculate the new position based on the direction */
    int newRow = playerPosition / board_size;
    int newCol = playerPosition % board_size;

    switch (direction)
    {
    case 'w':
    case 'W':
        newRow--;
        break;
    case 'a':
    case 'A':
        newCol--;
        break;
    case 's':
    case 'S':
        newRow++;
        break;
    case 'd':
    case 'D':
        newCol++;
        break;
    default:
        printf("Invalid move. Please enter W, A, S, or D.\n");
        return;
    }

    /*Check if the move is valid*/
    if (can_move(newRow, newCol))
    {
        /*Update the board*/
        if (board[playerPosition] == '1' || board[playerPosition] == '2')
        {
            totalECTS += 8;
            board[playerPosition] = 'P';
        }
        board[playerPosition] = '.';
        playerPosition = get_index(newRow, newCol);
        board[playerPosition] = 'P';
    }
    else
    {
        printf("You cannot move there.\n");
    }
}

int main()
{
    /*Printing board*/
    initialize_game();
    playerPosition = row / 2 * board_size + column / 2;
    print_board();
    char move;

    while (1)
    {
        printf("Total ECTS: %d Enter your move (W/A/S/D): ", totalECTS);
        scanf(" %c", &move); /*Space before %c to consume any whitespace*/
        move_player(move);
        print_board();

        /*Check if the player has reached the exit*/
        if (playerPosition == 255)
        {
            printf("Congratulations! You've completed the game.\n");
            printf("Total ECTS: %d\n", totalECTS);
            break;
        }
    }

    return 0;
}