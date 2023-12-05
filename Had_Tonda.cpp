#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define width 25
#define height 25
#define MAX_SCORES 10

int gameOver;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

void UlozitSkore(int topScores[], int n) {
    FILE* filePointer;
    filePointer = fopen("top_scores.txt", "w");
    if (filePointer == NULL) {
        printf("Chyba při otevírání souboru.\n");
        return;
    }

    int uniqueScores[MAX_SCORES] = { 0 };
    int uniqueCount = 0;

    // Hledání unikátních skóre pro zápis do souboru
    for (int i = 0; i < n && uniqueCount < MAX_SCORES; i++) {
        int isUnique = 1;
        for (int j = 0; j < uniqueCount; j++) {
            if (topScores[i] == uniqueScores[j]) {
                isUnique = 0;
                break;
            }
        }
        if (isUnique) {
            uniqueScores[uniqueCount] = topScores[i];
            uniqueCount++;
        }
    }

    // Zápis unikátních skóre do souboru
    for (int i = 0; i < uniqueCount; i++) {
        fprintf(filePointer, "%d\n", uniqueScores[i]);
    }

    fclose(filePointer);
}

void AktualizovatTopSkore(int skore, int topScores[], int n) {
    int i = n - 1;
    while (i > 0 && topScores[i - 1] < skore) {
        topScores[i] = topScores[i - 1];
        i--;
    }
    topScores[i] = skore;

    UlozitSkore(topScores, n); // Uložit celé pole do souboru
}

void Nastaveni() {
    gameOver = 0;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Vykreslit() {
    system("cls");
    for (int i = 0; i < width + 2; i++)
        printf("#");
    printf("\n");

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                printf("#");
            if (i == y && j == x)
                printf("O");
            else if (i == fruitY && j == fruitX)
                printf("F");
            else {
                int printTail = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        printTail = 1;
                    }
                }
                if (!printTail)
                    printf(" ");
            }
            if (j == width - 1)
                printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < width + 2; i++)
        printf("#");
    printf("\n");
    printf("Skore: %d\n", score);
}

void Vstup() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = 1;
            break;
        }
    }
}

void Logika(int topScores[], int n) {
    int predchoziX = tailX[0];
    int predchoziY = tailY[0];
    int predchozi2X, predchozi2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        predchozi2X = tailX[i];
        predchozi2Y = tailY[i];
        tailX[i] = predchoziX;
        tailY[i] = predchoziY;
        predchoziX = predchozi2X;
        predchoziY = predchozi2Y;
    }
    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= width || x < 0 || y >= height || y < 0) {
        gameOver = 1;
        printf("Konec hry - had narazil do zdi!\n");

        AktualizovatTopSkore(score, topScores, n);
        UlozitSkore(topScores, n);
    }

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

void NacistNejlepsiSkore(int topScores[], int n) {
    FILE* soubor;
    soubor = fopen("top_scores.txt", "r");
    if (soubor == NULL) {
        printf("Chyba při otevírání souboru.\n");
        return;
    }

    int i = 0;
    while (i < n && fscanf(soubor, "%d", &topScores[i]) == 1) {
        i++;
    }

    fclose(soubor);
}



void AktualizovatNejlepsiSkore(int noveSkore, int topScores[], int n) {
    topScores[n - 1] = noveSkore;
    int i, j, temp;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (topScores[i] < topScores[j]) {
                temp = topScores[i];
                topScores[i] = topScores[j];
                topScores[j] = temp;
            }
        }
    }
}

int main() {
    Nastaveni();
    int topScores[MAX_SCORES] = { 0 };
    NacistNejlepsiSkore(topScores, MAX_SCORES);

    printf("Skóre bylo načteno.\n");

    while (!gameOver) {
        Vykreslit();
        Vstup();
        Logika(topScores, MAX_SCORES);
        Sleep(100);
    }

    AktualizovatNejlepsiSkore(score, topScores, MAX_SCORES);


    system("pause");
    return 0;
}
