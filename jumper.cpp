#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>

#define WIDTH 800
#define HEIGHT 400
#define GROUND_HEIGHT 50
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 40
#define OBSTACLE_WIDTH 40
#define OBSTACLE_HEIGHT 40
#define GROUND_COLOR GREEN
#define PLAYER_COLOR RED
#define OBSTACLE_COLOR LIGHTRED
#define BACKGROUND_COLOR LIGHTBLUE
#define GRAVITY 1
#define JUMP_POWER 15

int score = 0;

void draw_ground() {
    setcolor(GROUND_COLOR);
    line(0, HEIGHT - GROUND_HEIGHT, WIDTH, HEIGHT - GROUND_HEIGHT);
}

void draw_player(int x, int y) {
    setcolor(PLAYER_COLOR);
    rectangle(x, y, x + PLAYER_WIDTH, y + PLAYER_HEIGHT);
    floodfill(x + PLAYER_WIDTH / 2, y + PLAYER_HEIGHT / 2, PLAYER_COLOR);
}

void draw_obstacle(int x, int y) {
    setcolor(OBSTACLE_COLOR);
    rectangle(x, y, x + OBSTACLE_WIDTH, y + OBSTACLE_HEIGHT);
    floodfill(x + OBSTACLE_WIDTH / 2, y + OBSTACLE_HEIGHT / 2, OBSTACLE_COLOR);
}

void draw_score() {
    char scoreStr[20];
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    sprintf(scoreStr, "Score: %d", score);
    outtextxy(10, 10, scoreStr);
}

void draw_menu() {
    setcolor(YELLOW);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(WIDTH / 2 - 150, HEIGHT / 2 - 30, "GAME OVER!");
    outtextxy(WIDTH / 2 - 150, HEIGHT / 2 + 10, "1. Retry");
    outtextxy(WIDTH / 2 - 150, HEIGHT / 2 + 50, "2. Quit");
}

void move_player(int* y, int* vy, int* isJumping) {
    if (*isJumping) {
        *y -= *vy;
        *vy -= GRAVITY;
        if (*y >= HEIGHT - GROUND_HEIGHT - PLAYER_HEIGHT) {
            *y = HEIGHT - GROUND_HEIGHT - PLAYER_HEIGHT;
            *isJumping = 0;
        }
    }
}

void jump(int* y, int* vy, int* isJumping) {
    if (!(*isJumping)) {
        *vy = JUMP_POWER;
        *isJumping = 1;
    }
}

int check_collision(int playerX, int playerY, int obstacleX, int obstacleY) {
    if (playerX + PLAYER_WIDTH >= obstacleX &&
        playerX <= obstacleX + OBSTACLE_WIDTH &&
        playerY + PLAYER_HEIGHT >= obstacleY &&
        playerY <= obstacleY + OBSTACLE_HEIGHT) {
        return 1;
    }
    return 0;
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int playerX = 50, playerY = HEIGHT - GROUND_HEIGHT - PLAYER_HEIGHT;
    int vy = 0;
    int isJumping = 0;
    int obstacleX = WIDTH - OBSTACLE_WIDTH;
    int obstacleY = HEIGHT - GROUND_HEIGHT - OBSTACLE_HEIGHT;

    while (1) {
        cleardevice();

        draw_ground();
        draw_player(playerX, playerY);
        draw_obstacle(obstacleX, obstacleY);
        draw_score();

        if (check_collision(playerX, playerY, obstacleX, obstacleY)) {
            draw_menu();
            int option;
            do {
                option = getch() - '0';
            } while (option != 1 && option != 2);

            if (option == 1) {
                score = 0;
                playerY = HEIGHT - GROUND_HEIGHT - PLAYER_HEIGHT;
                vy = 0;
                isJumping = 0;
                obstacleX = WIDTH - OBSTACLE_WIDTH;
                obstacleY = HEIGHT - GROUND_HEIGHT - OBSTACLE_HEIGHT;
                continue;
            } else if (option == 2) {
                break;
            }
        }

        move_player(&playerY, &vy, &isJumping);
        if (obstacleX <= 0) {
            obstacleX = WIDTH;
            score++;
        }
        obstacleX -= 5;

        if (kbhit()) {
            char key = getch();
            if (key == ' ') {
                jump(&playerY, &vy, &isJumping);
            }
        }

        delay(20);
    }

    getch();
    closegraph();
    return 0;
}
