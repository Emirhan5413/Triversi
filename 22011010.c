#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 23
#define MIN_SIZE 3

int dx[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
int dy[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size);
void printBoard(char board[MAX_SIZE][MAX_SIZE], int size, int red, int yellow, int blue);
int  isMoveValid(char board[MAX_SIZE][MAX_SIZE], int size, int row, int col, char player);
void applyMove(char board[MAX_SIZE][MAX_SIZE], int size, int row, int col, char player);
int  checkEndGame(char board[MAX_SIZE][MAX_SIZE], int size);
void countStones(char board[MAX_SIZE][MAX_SIZE], int size, int stones[3]);

int main() {
    int size;
    printf("Oyun tahtasinin boyutunu giriniz (NxN, Nmax=%d): ", MAX_SIZE);
    scanf("%d", &size);

    if (size < 3 || size > MAX_SIZE) {
        printf("Boyut %d ile %d arasinda olmalidir!\n", MIN_SIZE, MAX_SIZE);
        return 1;
    }

    char board[MAX_SIZE][MAX_SIZE];
    initializeBoard(board, size);

    char players[] = {'K', 'S', 'M'};

    int center = size / 2;      
    board[center][center] = 'K';


    int currentPlayer = 0;

    int endGame = 0;

    while (!endGame) {
        int stones[3];
        countStones(board, size, stones);
        int red    = stones[0];
        int yellow = stones[1];
        int blue   = stones[2];

        system("clear");
        printBoard(board, size, red, yellow, blue);

        if (checkEndGame(board, size)) {
            printf("Oyun bitti!\n");
            printf("\033[31mKirmizi tas sayisi: %d\033[0m\n", red);
            printf("\033[33mSari tas sayisi: %d\033[0m\n", yellow);
            printf("\033[34mMavi tas sayisi: %d\033[0m\n", blue);

            if (red > yellow && red > blue)
                printf("\033[31mKirmizi kazandi!\033[0m\n");
            else if (yellow > red && yellow > blue)
                printf("\033[33mSari kazandi!\033[0m\n");
            else if (blue > red && blue > yellow)
                printf("\033[34mMavi kazandi!\033[0m\n");
            else
                printf("Beraberlik!\n");

            endGame = 1;
        } 
        else {
            currentPlayer = (currentPlayer + 1) % 3;

            int row, col;
            int done = 0;
            while (!done) {
                printf("Sira oyuncu %c. Hamle yapmak istediginiz koordinatlari giriniz (satir sutun): ",
                       players[currentPlayer]);
                scanf("%d %d", &row, &col);

                row--;
                col--;

                if (row < 0 || row >= size || col < 0 || col >= size) {
                    printf("Koordinatlar tahtanin disinda, tekrar deneyin.\n");
                }
                else if (board[row][col] != '.') {
                    printf("Bu hucre dolu, baska bir yer secin.\n");
                }
                else if (!isMoveValid(board, size, row, col, players[currentPlayer])) {
                    printf("Geçersiz hamle! Tas, bir tasin komsusuna koyulmali ve/veya cevrim yapabilmeli.\n");
                }
                else {
                    applyMove(board, size, row, col, players[currentPlayer]);
                    done = 1;
                }
            }
        }
    }
    return 0;
}

void initializeBoard(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board[i][j] = '.';
        }
    }
}

void printBoard(char board[MAX_SIZE][MAX_SIZE], int size, int red, int yellow, int blue) {
    int i, j;

    printf("      ");
    for (j = 1; j <= size; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    for (i = 0; i < size; i++) {
        printf("%3d   ", i + 1);
        for (j = 0; j < size; j++) {
            if (board[i][j] == 'K')
                printf("\033[31m %c \033[0m", board[i][j]);
            else if (board[i][j] == 'S')
                printf("\033[33m %c \033[0m", board[i][j]);
            else if (board[i][j] == 'M')
                printf("\033[34m %c \033[0m", board[i][j]);
            else
                printf(" %c ", board[i][j]);
        }
        printf("\n");
    }

    printf("\033[31mKirmizi tas sayisi: %d\033[0m\n", red);
    printf("\033[33mSari tas sayisi: %d\033[0m\n", yellow);
    printf("\033[34mMavi tas sayisi: %d\033[0m\n", blue);
}


int isMoveValid(char board[MAX_SIZE][MAX_SIZE], int size, int row, int col, char player) {
    int i = 0;
    int foundNeighbor = 0;

    while (i < 8 && foundNeighbor == 0) {
        int nr = row + dx[i];
        int nc = col + dy[i];

        if (nr >= 0 && nr < size && nc >= 0 && nc < size) {
            if (board[nr][nc] != '.') {
                foundNeighbor = 1;
            }
        }
        i++;
    }

    if (!foundNeighbor) {
        return 0;
    }
    return 1;
}

void applyMove(char board[MAX_SIZE][MAX_SIZE], int size, int row, int col, char player) {
    board[row][col] = player;

    int d;
    for (d = 0; d < 8; d++) {
        int r = row + dx[d];
        int c = col + dy[d];
        int step = 1;
        int sameColorPos = -1;

        int keepGoing = 1;
        while (keepGoing) {
            if (r < 0 || r >= size || c < 0 || c >= size) {
                keepGoing = 0;
            }
            else if (board[r][c] == '.') {
                keepGoing = 0;
            }
            else if (board[r][c] == player) {
                sameColorPos = step;
                keepGoing = 0;
            }
            else {
                r += dx[d];
                c += dy[d];
                step++;
            }
        }

        if (sameColorPos != -1) {
            int flipRow = row;
            int flipCol = col;
            int k = 0;
            while (k < sameColorPos) {
                flipRow += dx[d];
                flipCol += dy[d];
                board[flipRow][flipCol] = player;
                k++;
            }
        }
    }
}

int checkEndGame(char board[MAX_SIZE][MAX_SIZE], int size) {
    int i, j;
    int gameEnd = 1;
    for (i = 0; i < size; i++) {
        j = 0;
        while (j < size && gameEnd == 1) {
            if (board[i][j] == '.') {
                gameEnd = 0;
            }
            j++;
        }
    }
    return gameEnd;
}

void countStones(char board[MAX_SIZE][MAX_SIZE], int size, int stones[3]) {
    stones[0] = 0; // K
    stones[1] = 0; // S
    stones[2] = 0; // M

    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == 'K') {
                stones[0]++;
            }
            else if (board[i][j] == 'S') {
                stones[1]++;
            }
            else if (board[i][j] == 'M') {
                stones[2]++;
            }
        }
    }
}
