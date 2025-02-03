#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 40
#define HEIGHT 20
#define PACMAN 'C'
#define WALL '#'
#define FOOD '.'
#define EMPTY ' '
#define DEMON 'X'
#define ENEMY 'E'
#define BONUS '$'
#define MAX_DEMONS 5
#define MAX_ENEMIES 5 

int res = 0;
int score = 0;
int pacman_x, pacman_y;
char board[HEIGHT][WIDTH];
int food = 0;
int curr = 0;
int prize = 0;
int double_move = 0;
int initial_food = 0;

typedef struct {
    int x, y;
} Demon;

typedef struct {
    int x, y;
} Enemy;

Demon demons[MAX_DEMONS];
Enemy enemies[MAX_ENEMIES];

void save_game() {
    FILE *file = fopen("D:/pacman.bin", "wb");
    if (file != NULL) {
        fwrite(&pacman_x, sizeof(int), 1, file);
        fwrite(&pacman_y, sizeof(int), 1, file);
        fwrite(&score, sizeof(int), 1, file);
        fwrite(&food, sizeof(int), 1, file);
        fwrite(&curr, sizeof(int), 1, file);
        fwrite(&prize, sizeof(int), 1, file);

        fwrite(demons, sizeof(Demon), MAX_DEMONS, file);
        fwrite(enemies, sizeof(Enemy), MAX_ENEMIES, file);

        for (int i = 0; i < HEIGHT; i++) {
            fwrite(board[i], sizeof(char), WIDTH, file);
        }
        fclose(file);
    }
}

int load_game() {
    FILE *file = fopen("D:/pacman.bin", "rb");
    if (file != NULL) {
        fread(&pacman_x, sizeof(int), 1, file);
        fread(&pacman_y, sizeof(int), 1, file);
        fread(&score, sizeof(int), 1, file);
        fread(&food, sizeof(int), 1, file);
        fread(&curr, sizeof(int), 1, file);
        fread(&prize, sizeof(int), 1, file);

        fread(demons, sizeof(Demon), MAX_DEMONS, file);
        fread(enemies, sizeof(Enemy), MAX_ENEMIES, file);

        for (int i = 0; i < HEIGHT; i++) {
            fread(board[i], sizeof(char), WIDTH, file);
        }

        fclose(file);
        return 1;
    }
    return 0;
}

void initialize() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || j == WIDTH - 1 || j == 0 || i == HEIGHT - 1) {
                board[i][j] = WALL;
            } else {
                board[i][j] = EMPTY;
            }
        }
    }

    int count = 50;
    while (count != 0) {
        int i = (rand() % HEIGHT);
        int j = (rand() % WIDTH);

        if (board[i][j] != WALL && board[i][j] != PACMAN && board[i][j] != FOOD && board[i][j] != DEMON && board[i][j] != ENEMY) {
            board[i][j] = WALL;
            count--;
        }
    }

    
    int val = 5;
    while (val--) {
        int row = (rand() % (HEIGHT + 1));
        for (int j = 3; j < WIDTH - 3; j++) {
            if (board[row][j] != WALL && board[row][j] != PACMAN) {
                board[row][j] = WALL;
            }
        }
    }


    int count_demons = MAX_DEMONS;
    while (count_demons != 0) {
        int i = (rand() % HEIGHT);
        int j = (rand() % WIDTH);

        if (board[i][j] != WALL && board[i][j] != PACMAN && board[i][j] != FOOD && board[i][j] != DEMON && board[i][j] != ENEMY) {
            board[i][j] = DEMON;
            demons[count_demons - 1].x = j;
            demons[count_demons - 1].y = i;
            count_demons--;
        }
    }

    int count_enemies = MAX_ENEMIES;
    while (count_enemies != 0) {
        int i = (rand() % HEIGHT);
        int j = (rand() % WIDTH);

        if (board[i][j] != WALL && board[i][j] != PACMAN && board[i][j] != FOOD && board[i][j] != DEMON && board[i][j] != ENEMY) {
            board[i][j] = ENEMY;
            enemies[count_enemies - 1].x = j;
            enemies[count_enemies - 1].y = i;
            count_enemies--;
        }
    }

    int count_BONUS = 1;
    while (count_BONUS != 0) {
        int i = (rand() % HEIGHT);
        int j = (rand() % WIDTH);

        if (board[i][j] != WALL && board[i][j] != PACMAN && board[i][j] != DEMON && board[i][j] != ENEMY) {
            board[i][j] = BONUS;
            count_BONUS--;
        }
    }

    pacman_x = WIDTH / 2;
    pacman_y = HEIGHT / 2;
    board[pacman_y][pacman_x] = PACMAN;

    food = 0;
    initial_food = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i % 2 == 0 && j % 2 == 0 && board[i][j] != WALL && board[i][j] != DEMON && board[i][j] != ENEMY && board[i][j] != PACMAN && board[i][j] != BONUS) {
                board[i][j] = FOOD;
                food++;
                initial_food++;
            }
        }
    }
}

void draw() {
    system("cls");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }

    printf("Score: %d\n", score);
    printf("Food Remaining: %d\n", food);
    printf("Total Food Eaten: %d\n", curr);
    printf("Total Food Count: %d\n", initial_food);

    if (double_move) {
        printf("Remaining double move: %d\n", prize);
    }
}

void move(int move_x, int move_y) {
    int x = pacman_x + move_x;
    int y = pacman_y + move_y;

    if (board[y][x] != WALL) {
        if (board[y][x] == FOOD) {
            score++;
            food--;
            curr++;
            if (food == 0) {
                res = 2;
                return;
            }
        } else if (board[y][x] == DEMON || board[y][x] == ENEMY) {
            res = 1;
        } else if (board[y][x] == BONUS) {
            double_move = 1;
            prize = 10;
        }

        board[pacman_y][pacman_x] = EMPTY;
        pacman_x = x;
        pacman_y = y;
        board[pacman_y][pacman_x] = PACMAN;
    }
}

void move_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        int direction = rand() % 4;

        int new_x = enemies[i].x;
        int new_y = enemies[i].y;

        switch (direction) {
            case 0: new_y--; break;
            case 1: new_y++; break;
            case 2: new_x--; break;
            case 3: new_x++; break;
        }

        if (new_y >= 0 && new_y < HEIGHT && new_x >= 0 && new_x < WIDTH && board[new_y][new_x] != WALL && board[new_y][new_x] != DEMON && board[new_y][new_x] != ENEMY && board[new_y][new_x] != PACMAN) {
            board[enemies[i].y][enemies[i].x] = ' ';
            enemies[i].x = new_x;
            enemies[i].y = new_y;
            board[enemies[i].y][enemies[i].x] = ENEMY;
        }
    }
}

int main() {
    char ch;
    int load = 0;

    FILE *file = fopen("D:/pacman.bin", "r");
    if (file != NULL) {
        fseek(file, 0, SEEK_END); 
        long file_size = ftell(file);
        fclose(file);

        if (file_size > 0) {
            printf("A saved game is found. Do you want to continue from the saved game? (Y/N): ");
            ch = getch();
            if (ch == 'Y' || ch == 'y') {
                load = load_game();
            }
        }
    }

    if (!load) {
        initialize();
    }

    printf("Use buttons for w(up), a(left), d(right), and s(down)\nAlso, Press q for quit\n");
    printf("Enter Y to continue: \n");

    ch = getch();
    if (ch != 'Y' && ch != 'y') {
        printf("Exit Game! ");
        return 1;
    }

    while (1) {
        draw();
        move_enemies();

        if (res == 1) {
            system("cls");
            printf("Game Over! Dead by Demon or Enemy\nYour Score: %d\n", score);
            return 1;
        }

        if (res == 2) {
            system("cls");
            printf("You Win!\nYour Score: %d\n", score);
            return 1;
        }

        ch = getch();

        if (double_move && prize > 0) {
            switch (ch) {
                case 'w':
                    move(0, -1);
                    move(0, -1);
                    prize--;
                    break;
                case 's':
                    move(0, 1);
                    move(0, 1);
                    prize--;
                    break;
                case 'a':
                    move(-1, 0);
                    move(-1, 0);
                    prize--;
                    break;
                case 'd':
                    move(1, 0);
                    move(1, 0);  
                    prize--;
                    break;
                case 'q':
                    printf("Game Over! Your Score: %d\n", score);
                    save_game();
                    return 0;
            }
        } else {
            switch (ch) {
                case 'w':
                    move(0, -1);
                    break;
                case 's':
                    move(0, 1);
                    break;
                case 'a':
                    move(-1, 0);
                    break;
                case 'd':
                    move(1, 0);
                    break;
                case 'q':
                    printf("Game Over! Your Score: %d\n", score);
                    save_game();
                    return 0;
            }
        }
    }

    return 0;
}