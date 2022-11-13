#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char ALIVE = 'o', ALIVE_PRINT = 'o', DEAD = 'x', DEAD_PRINT = 'x', file_name[] = "./cells.txt";
    int GEN_DELAY = 1;

    int x = 0;
    int y = 0;

    printf("Starting Game...\n");
    printf("Enter field width and confirm with enter:\n");
    scanf("%d", &x);
    printf("Enter height and confirm with enter: \n");
    scanf("%d", &y);

    // list of all cells. If cell value is 'd', cell is dead. If it is 'a', it will be alive
    char cells[y][x];
    char cellsTemp[y][x];

    printf("Grid will be of size %dx%d.\n", x, y);

    // load board data from file
    printf("Loading board data...\n");
    {
        char ch;
        FILE *fp;

        fp = fopen(file_name, "r"); // read mode

        if (fp == NULL) {
            perror("Error while opening the file.\n");
            exit(EXIT_FAILURE);
        }

        // init status for every cell from loaded file
        int i = 0;
        int j = 0;
        short int err;
        while ((ch = fgetc(fp)) != EOF) {
            // reset error status
            err = 0;

            // set ALIVE or DEAD status for cell
            if (ch == ALIVE) {
                cells[i][j] = ALIVE;
            } else if (ch == DEAD) {
                cells[i][j] = DEAD;
            } else {
                err = 1;
            }

            // if an illegal character was read, don't process it further
            if (err) {
                continue;
            }

            j++;
            // if maximum row length is reached, move to next row
            if (j >= x) {
                j = 0;
                i++;
            }
            // if last row was reached, abort to avoid going out of bounds
            if (i >= y) {
                break;
            }
        }

        // close file
        fclose(fp);

        // if board isn't full yet, fill it with dead cells
        if (i < x || j < y) {
            while (1) {
                cells[i][j] = DEAD;
                j++;
                // if maximum row length is reached, move to next row
                if (j >= x) {
                    j = 0;
                    i++;
                }
                // if last row was reached, abort to avoid going out of bounds
                if (i >= y) {
                    break;
                }
            }
        }
    }
    printf("Board was initialized.\n");

    // set number of cycles
    int cycles = 0;
    printf("How many cycles do you want to play?\n");
    scanf("%d", &cycles);
    printf("Game will last for %d cycles.\n\n", cycles);

    // print initial cells
    {
        printf("\nGen 0\n");
        char symbol;
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                if (cells[i][j] == ALIVE) {
                    symbol = ALIVE_PRINT;
                } else {
                    symbol = DEAD_PRINT;
                }

                printf("%c  ", symbol);
            }
            printf("\n");
        }
    }

    // actual game
    for (int gen = 1; gen <= cycles; gen++) {
        // delay printing next line by one second
        sleep(GEN_DELAY);

        printf("\nGen %d\n", gen);

        // apply rules for game
        {
            // check if cells survive and store in temporary array
            for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                    // count live neighbors
                    short int liveNeighbours = 0;
                    {
                        //printf("(%dx%d)", i, j);
                        if (j > 0 && i > 0 && cells[i - 1][j - 1] == ALIVE) {
                            //printf(" Upper left lives!");
                            liveNeighbours++;
                        }
                        if (j > 0 && cells[i][j - 1] == ALIVE) {
                            //printf(" Left lives!");
                            liveNeighbours++;
                        }
                        if ((i + 1) < x && j > 0 && cells[i + 1][j - 1] == ALIVE) {
                            //printf(" Lower left lives!");
                            liveNeighbours++;
                        }
                        if (i > 0 && cells[i - 1][j] == ALIVE) {
                            //printf(" Upper middle lives!");
                            liveNeighbours++;
                        }
                        if ((i + 1) < x && cells[i + 1][j] == ALIVE) {
                            //printf(" Lower Middle lives!");
                            liveNeighbours++;
                        }
                        if (i > 0 && (j + 1) < y && cells[i - 1][j + 1] == ALIVE) {
                            //printf(" Upper right lives!");
                            liveNeighbours++;
                        }
                        if ((j + 1) < y && cells[i][j + 1] == ALIVE) {
                            //printf(" Right lives!");
                            liveNeighbours++;
                        }
                        if ((i + 1) < x && (j + 1) < y && cells[i + 1][j + 1] == ALIVE) {
                            //printf(" Lower right lives!");
                            liveNeighbours++;
                        }
                        //printf(" %d\n", liveNeighbours);
                    }

                    // Any live cell with two or three live neighbours survives.
                    if (cells[i][j] == ALIVE && (liveNeighbours == 2 || liveNeighbours == 3)) {
                        cellsTemp[i][j] = ALIVE;
                    }
                    // Any dead cell with three live neighbours becomes a live cell.
                    else if (cells[i][j] == DEAD && liveNeighbours == 3) {
                        cellsTemp[i][j] = ALIVE;
                    }
                    // All other live cells die in the next generation. Similarly, all other dead cells stay dead.
                    else {
                        cellsTemp[i][j] = DEAD;
                    }
                }
            }

            // copy from temporary array to actual cell array
            for (int i = 0; i < y; i++) {
                for (int j = 0; j < x; j++) {
                    cells[i][j] = cellsTemp[i][j];
                }
            }
        }

        // print current cells
        char symbol;
        for (int i = 0; i < y; i++) {
            for (int j = 0; j < x; j++) {
                if (cells[i][j] == ALIVE) {
                    symbol = ALIVE_PRINT;
                } else {
                    symbol = DEAD_PRINT;
                }

                printf("%c  ", symbol);
            }
            printf("\n");
        }
    }

    printf("\nGame has ended.\n");
    return 0;
}