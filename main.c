#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
    int x, y;
} PIX;

PIX bird;
PIX pipes[3];
int score;

void Draw();
void UpdatePipes();
void HitTest();
void ResetGame();
void GameOverScreen();
void SetCursorPosition(int x, int y);
void SetColor(int color);
void ShowIntroduction();

void SetCursorPosition(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Draw() {
    SetCursorPosition(0, 0);
    SetColor(1);

    for (int y = 0; y <= 16; y++) {
        for (int x = 0; x <= 32; x++) {
            if (y == 0 || y == 16 || x == 0 || x == 32) {
                printf("[]");
            } else if (bird.x == x && bird.y == y) {
                SetColor(2);
                printf(")>");
                SetColor(1);
            } else {
                int isPipe = 0;
                for (int i = 0; i < 3; i++) {
                    if (pipes[i].x == x && (y < pipes[i].y - 2 || y > pipes[i].y + 2)) {
                        SetColor(4);
                        printf("][");
                        SetColor(1);
                        isPipe = 1;
                        break;
                    }
                }
                if (!isPipe) printf("  ");
            }
        }
        printf("\n");
    }

    SetColor(7);
    printf("Score: %d\n", score);
}

void UpdatePipes() {
    for (int i = 0; i < 3; i++) {
        if (pipes[i].x <= 0) {
            pipes[i].x = 32;
            pipes[i].y = (rand() % (16 - 4)) + 3;
            score++;
        } else {
            pipes[i].x--;
        }
    }
}

void HitTest() {
    if (bird.y >= 16) {
        GameOverScreen();
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (bird.x == pipes[i].x && (bird.y < pipes[i].y - 2 || bird.y > pipes[i].y + 2)) {
            GameOverScreen();
            return;
        }
    }
}

void ResetGame() {
    bird.x = 5;
    bird.y = 16 / 2;
    score = 0;

    for (int i = 0; i < 3; i++) {
        pipes[i].x = 32 + i * 10;
        pipes[i].y = (rand() % (16 - 4)) + 3;
    }
}

void GameOverScreen() {
    SetCursorPosition(0, 16 + 2);
    SetColor(12);
    printf("Game Over! Final Score: %d\n", score);
    printf("Press R to restart or Q to quit.\n");

    while (1) {
        if (GetAsyncKeyState('R') & 0x8000) {
            ResetGame();
            break;
        }
        if (GetAsyncKeyState('Q') & 0x8000) {
            exit(0);
        }
    }
}

void ShowIntroduction() {
    SetCursorPosition(0, 0);
    SetColor(14);
    printf("Welcome to the Flappy Bird Game!\n");

    printf("\n");

    printf("Use the UP arrow key to jump.\n");
    printf("Avoid the pipes and try to survive as long as you can!\n");

    printf("\n");

    printf("Press Enter to start the game...\n");

    while (1) {
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            break;
        }
    }
    system("cls");
}

int main() {
    srand(time(NULL));
    ResetGame();

    ShowIntroduction();

    Sleep(500);

    int frame = 0;
    while (1) {
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            bird.y -= 2;
        }
        if (GetAsyncKeyState('Q') & 0x8000) {
            break;
        }

        if (frame == 3) {
            bird.y++;
            UpdatePipes();
            frame = 0;
        }

        HitTest();
        Draw();

        frame++;
        Sleep(50);
    }

    return 0;
}
