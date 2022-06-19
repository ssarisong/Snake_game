#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLACK 0
#define BLUE1 1
#define GREEN1 2
#define CYAN1 3
#define RED1 4
#define MAGENTA1 5
#define YELLOW1 6
#define GRAY1 7
#define GRAY2 8
#define BLUE2 9
#define GREEN2 10
#define CYAN2 11
#define RED2 12
#define MAGENTA2 13
#define YELLOW2 14
#define WHITE 15

#define ESC 0x1b //ESC ������ ����

#define SPECIAL1 0xe0 //Ư��Ű�� 0xe0+key ������ �����ȴ�
#define SPECIAL2 0x00 //keypad�� ��� 0x00+key�� �����ȴ�

#define UP 0x48 //Up key�� 0xe0+0x48 2���� ���� ���´�
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define UP2 'w' //player2�� wsad�� ����Ű�� �����
#define DOWN2 's'
#define LEFT2 'a'
#define RIGHT2 'd'

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define PAUSE 112
#define ESC 27

#define MAP_X 3
#define MAP_Y 2
#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define WIDTH 80
#define HEIGHT 24

//#define LIFE 3;

#define SNAKE_LENGTH 6
#define SNAKE_LENGTH_MAX 20
COORD snake[SNAKE_LENGTH_MAX]; // snake �� ��ǥ�� gotoxy2()���� ����ϱ� ����.
int snake_length = SNAKE_LENGTH;
int snake_length2 = SNAKE_LENGTH;
int Delay = 100;
int Delay2 = 10;

int x[100], y[100]; //x,y ��ǥ���� ���� �� 100�� 
int x2[100], y2[100];
int food_x, food_y; //food�� ��ǥ���� ���� 
int food_x2, food_y2;
int length; //�����̸� ��� 
int length2;
int speed; //���� �ӵ� 
int speed2;
int score; //���� ����  --reset�Լ��� ���� �ʱ�ȭ��
int score2;
int best_score = 0; //�ְ� ���� ���� --reset�Լ��� ���� �ʱ�ȭ ���� ���� 
int best_score2 = 0;
int last_score = 0; //������ ���� ����  --reset�Լ��� ���� �ʱ�ȭ ���� ����
int last_score2 = 0;
int dir; //�̵����� ���� 
int dir2;
int key; //�Է¹��� Ű ���� 
int key2;
//int life = LIFE;
//int life2 = LIFE;

void gotoxy(int x, int y, char* s) { //x���� 2x�� ����, ��ǥ���� �ٷ� ���ڿ��� �Է��� �� �ֵ��� printf�Լ� ����  
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API �Լ��Դϴ�. �̰� ���ʿ� �����
    printf("%s", s);
}

void title(void); //���� ����ȭ�� 
void reset(void); //������ �ʱ�ȭ 
void draw_map(void); // ������ �׵θ��� �׸� 
void move(int dir); //��Ӹ��� �̵� 
void move2(int dir);
void pause(void); //�Ͻ����� 
void game_over(void); //���� ������ Ȯ�� 
void food(void); // ���� ���� 
void food2(void);

void removeCursor(void) { // Ŀ���� �Ⱥ��̰� �Ѵ�

    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void removeCursor2(void) { // Ŀ���� �Ⱥ��̰� �Ѵ�

    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy2(int x2, int y2, char* s)
{
    COORD pos = { 2 * x2,y2 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API �Լ��Դϴ�. �̰� ���ʿ� �����
    printf("%s", s);
}
void textcolor(int fg_color, int bg_color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

int main() {
    title();
    removeCursor();
    removeCursor2();

    while (1) {
        if (kbhit()) do { key = getch(); } while (key == 224); //Ű �Է¹���
        Sleep(speed);

        switch (key) { //�Է¹��� Ű�� �ľ��ϰ� ����  
        case LEFT:
        case RIGHT:
        case UP:
        case DOWN:
            if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) ||
                (dir == DOWN && key != UP))//180ȸ���̵��� �����ϱ� ���� �ʿ�. 
                dir = key;
            key = 0; // Ű���� �����ϴ� �Լ��� reset 
            break;
        case 'a':
        case 'd':
        case 'w':
        case 's':
            if ((dir2 == 'a' && key2 != 'd') || (dir2 == 'd' && key2 != 'a') || (dir2 == 'w' && key2 != 's') ||
                (dir2 == 's' && key2 != 'w'))//180ȸ���̵��� �����ϱ� ���� �ʿ�. 
                dir2 = key;
            key = 0; // Ű���� �����ϴ� �Լ��� reset 
            break;
        case PAUSE: // PŰ�� ������ �Ͻ����� 
            pause();
            break;
        case ESC: //ESCŰ�� ������ ���α׷� ���� 
            exit(0);
        }
        move(dir);
        move2(dir2);
    }
}

void title(void) {
    int i, j;

    while (kbhit()) getch(); //���ۿ� �ִ� Ű���� ���� 

    draw_map();    //�� �׵θ��� �׸� 
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { // �� �׵θ� ������ ��ĭ���� ä�� 
        for (j = MAP_X + 1; j < MAP_X + MAP_WIDTH - 1; j++) gotoxy(j, i, "  ");
    }

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 3, "����� �����\n");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 4, "��       ��    ��\n");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 5, "��  ��� ��    ��\n");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 6, "��    �� ��    ��\n");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 7, "����� �����\n\n\n");

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 9, "��Press any key to start ��\n");

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 11, "   �� ��,��,��,�� : 1player Move    ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 12, "   �� a,d,w,s : 2player Move    ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 13, "   �� food=>��: 1player, ��: 2player  ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 14, "   �� P : Pause             ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 15, "   �� ESC : Quit              ");

    while (TRUE) {
        if (kbhit() == 1) { //Ű�Է¹��� 
            key = getch();
            break; //�ƴϸ� �׳� ��� ���� 
        }
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " ��PRESS ANY KEY TO START�� ");
        Sleep(400);
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, "                            ");
        Sleep(400);

    }
    reset(); // ������ �ʱ�ȭ  

}


void reset(void) {
    int i;
    //life = 3; //���� ������ ������ ����� ���� ���� ����
   // life2 = 3;
    system("cls"); //ȭ���� ���� 
    draw_map(); //�� �׵θ��� �׸� 
    while (kbhit()) getch(); //���ۿ� �ִ� Ű���� ���� 

    dir = LEFT; // ���� �ʱ�ȭ  
    speed = 100; // �ӵ� �ʱ�ȭ 
    length = 5; //�� ���� �ʱ�ȭ 
    score = 0; //���� �ʱ�ȭ 
    for (i = 0; i < length; i++) { //�� ���밪 �Է� 
        x[i] = MAP_WIDTH / 2 + i;
        y[i] = MAP_HEIGHT / 2;
        gotoxy(MAP_X + x[i], MAP_Y + y[i], "��");
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); //�� �Ӹ� �׸� 
    dir2 = 'a'; // ���� �ʱ�ȭ  
    speed2 = 100; // �ӵ� �ʱ�ȭ 
    length2 = 5; //�� ���� �ʱ�ȭ 
    score2 = 0; //���� �ʱ�ȭ 
    for (i = 0; i < length2; i++) { //�� ���밪 �Է� 
        x2[i] = MAP_WIDTH / 2 + i;
        y2[i] = MAP_HEIGHT / 2 + 5;
        gotoxy(MAP_X + x2[i], MAP_Y + y2[i], "��");
    }
    gotoxy(MAP_X + x2[0], MAP_Y + y2[0], "��"); //�� �Ӹ� �׸� 
    food2(); // food ����  
    food(); // food ����  

}


void draw_map(void) { //�� �׵θ� �׸��� �Լ� 
    int i, j;
    for (i = 0; i < MAP_WIDTH; i++) {
        gotoxy(MAP_X + i, MAP_Y, "��");
    }
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) {
        gotoxy(MAP_X, i, "��");
        gotoxy(MAP_X + MAP_WIDTH - 1, i, "��");
    }
    for (i = 0; i < MAP_WIDTH; i++) {
        gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "��");
    }
}

void move(int dir) {
    int i;
    if (x[0] == food_x && y[0] == food_y) { //food1(��)�� �浹���� ��� 
        score += 10; //���� ���� 
        food(); //���ο� food �߰� 
        length++; //�������� 
        x[length - 1] = x[length - 2]; //���θ��� ���뿡 �� �Է� 
        y[length - 1] = y[length - 2];
    }
    if (x[0] == food_x2 && y[0] == food_y2) { //food2(��)�� �浹���� ��� 
        score -= 10; //���� ���� 
        food(); //���ο� food �߰� 
        length--; //���� ����
        x[length + 1] = x[length + 2]; //���θ��� ���뿡 �� �Է� 
        y[length + 1] = y[length + 2];
    }
    if (x[0] == 0 || x[0] == MAP_WIDTH - 1 || y[0] == 0 || y[0] == MAP_HEIGHT - 1) { //���� �浹���� ��� 
        game_over();
        return; //game_over���� ������ �ٽ� �����ϰ� �Ǹ� ���⼭���� �ݺ��ǹǷ� 
                //return�� ����Ͽ� move�� ������ �κ��� ������� �ʵ��� �մϴ�. 
    }
    for (i = 1; i < length; i++) { //�ڱ���� �浹�ߴ��� �˻� 
        if (x[0] == x[i] && y[0] == y[i]) {
            game_over();
            return;
        }
    }

    gotoxy(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  "); //���� �������� ���� 
    for (i = length - 1; i > 0; i--) { //������ǥ�� ��ĭ�� �ű� 
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); //�Ӹ��� �ִ����� �������� ��ħ 
    if (dir == LEFT) --x[0]; //���⿡ ���� ���ο� �Ӹ���ǥ(x[0],y[0])���� ���� 
    if (dir == RIGHT) ++x[0];
    if (dir == UP) --y[0];
    if (dir == DOWN) ++y[0];
    gotoxy(MAP_X + x[i], MAP_Y + y[i], "��"); //���ο� �Ӹ���ǥ���� �Ӹ��� �׸� 
}
void move2(int dir) {
    int i;
    if (x2[0] == food_x2 && y2[0] == food_y2) { //food2(��)�� �浹���� ��� 
        score2 += 10; //���� ���� 
        food2(); //���ο� food �߰� 
        length2++; //�������� 
        x2[length2 - 1] = x2[length2 - 2]; //���θ��� ���뿡 �� �Է� 
        y2[length2 - 1] = y2[length2 - 2];
    }
    if (x2[0] == food_x && y2[0] == food_y) { //food(��)�� �浹���� ��� 
        score2 -= 10; //���� ���� 
        food2(); //���ο� food �߰� 
        length2--; //���� ���� 
        x2[length2 + 1] = x2[length2 + 2]; //���θ��� ���뿡 �� �Է� 
        y2[length2 + 1] = y2[length2 + 2];
    }
    if (x2[0] == 0 || x2[0] == MAP_WIDTH - 1 || y2[0] == 0 || y2[0] == MAP_HEIGHT - 1) { //���� �浹���� ��� 
        game_over();
        return; //game_over���� ������ �ٽ� �����ϰ� �Ǹ� ���⼭���� �ݺ��ǹǷ� 
                //return�� ����Ͽ� move�� ������ �κ��� ������� �ʵ��� �մϴ�. 
    }
    for (i = 1; i < length2; i++) { //�ڱ���� �浹�ߴ��� �˻� 
        if (x2[0] == x2[i] && y2[0] == y2[i]) {
            game_over();
            return;
        }
    }

    gotoxy(MAP_X + x2[length2 - 1], MAP_Y + y2[length2 - 1], "  "); //���� �������� ���� 
    for (i = length2 - 1; i > 0; i--) { //������ǥ�� ��ĭ�� �ű� 
        x2[i] = x2[i - 1];
        y2[i] = y2[i - 1];
    }
    gotoxy(MAP_X + x2[0], MAP_Y + y2[0], "��"); //�Ӹ��� �ִ����� �������� ��ħ 
    if (dir2 == 'a') --x2[0]; //���⿡ ���� ���ο� �Ӹ���ǥ(x[0],y[0])���� ���� 
    if (dir2 == 'd') ++x2[0];
    if (dir2 == 'w') --y2[0];
    if (dir2 == 's') ++y2[0];
    gotoxy(MAP_X + x2[i], MAP_Y + y2[i], "��"); //���ο� �Ӹ���ǥ���� �Ӹ��� �׸� 
}

void pause(void) { // pŰ�� ������ ��� ������ �Ͻ� ���� 
    while (1) {
        if (key == PAUSE) {
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "��PAUSE : PRESS ANY KEY TO RESUME�� ");
            Sleep(400);
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "                                    ");
            Sleep(400);
        }
        else {
            draw_map();
            return;
        }
        if (kbhit()) {

            do {
                key = getch();
            } while (key == 224);
        }

    }
}

void game_over(void) { //�������� �Լ� 
   // life--;
    //life2--;
    //if (life == 0 || life2 == 0)
   // {
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 3, "���� ��    ��");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 4, "��     ���  ��");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 5, "���� �� �� ��");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 6, "��     ��  ���");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 7, "��     ��    ��");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 8, " 1player SCORE: ");
        printf("%d", last_score = score);
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 9, " 2player SCORE: ");
        printf("%d", last_score2 = score2);

        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, " ��Press any keys to restart�� ");

        if (score > best_score) {
            best_score = score;
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 4, MAP_Y + 10, "��1player WINNER��");
        }
        else if (score2 > best_score2) {
            best_score2 = score2;
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 4, MAP_Y + 11, "��2player WINNER��");
        }
        Sleep(500);
        while (kbhit()) getch();
        key = getch();
        title();
   // }
}

void food(void) {
    int i;

    int food_crush_on = 0;//food�� �� ������ǥ�� ���� ��� on 
    int r = 0; //���� ������ �絿�Ǵ� ���� 
    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, " ��1player SCORE: "); //����ǥ�� 
    printf("%3d, 1player LAST SCORE: %3d, 1player BEST SCORE: %3d", score, last_score, best_score);
   // gotoxy(MAP_X, MAP_Y + MAP_HEIGHT + 1, " ��1player ���� ���: "); //����ǥ��
   // printf("%3d��", life);

    while (1) {
        food_crush_on = 0;
        srand((unsigned)time(NULL) + r); //����ǥ���� 
        food_x = (rand() % (MAP_WIDTH - 2)) + 1;    //������ ��ǥ���� ���� 
        food_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++) { //food�� �� ����� ��ġ���� Ȯ��  
            if (food_x == x[i] && food_y == y[i]) {
                food_crush_on = 1; //��ġ�� food_crush_on �� on 
                r++;
                break;
            }
        }
        if (food_crush_on == 1) continue; //������ ��� while���� �ٽ� ���� 
        for (i = length; i < 0; i--) { //food�� �� ����� ��ġ���� Ȯ��  
            if (food_x2 == x[i] && food_y2 == y[i]) {
                food_crush_on = 1; //��ġ�� food_crush_on �� on 
                r--;
                break;
            }
        }
        if (food_crush_on == 1) continue; //������ ��� while���� �ٽ� ���� 

        gotoxy(MAP_X + food_x, MAP_Y + food_y, "��"); //�Ȱ����� ��� ��ǥ���� food�� ��� 
        speed -= 3; //�ӵ� ���� 
        break;
    }
}
void food2(void) {
    int i;

    int food_crush_on2 = 0;//food�� �� ������ǥ�� ���� ��� on 
    int r = 0; //���� ������ �絿�Ǵ� ���� 
    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT + 3, " ��2player SCORE: "); //����ǥ�� 
    printf("%3d, 2player LAST SCORE: %3d, 2player BEST SCORE: %3d", score2, last_score2, best_score2);
    //gotoxy(MAP_X, MAP_Y + MAP_HEIGHT + 4, " ��2player ���� ���: "); //����ǥ��
    //printf("%3d��", life);

    while (1) {
        food_crush_on2 = 0;
        srand((unsigned)time(NULL) + r); //����ǥ���� 
        food_x2 = (rand() % (MAP_WIDTH - 5)) + 4;    //������ ��ǥ���� ���� 
        food_y2 = (rand() % (MAP_HEIGHT - 5)) + 4;

        for (i = 0; i < length2; i++) { //food�� �� ����� ��ġ���� Ȯ��  
            if (food_x2 == x2[i] && food_y2 == y2[i]) {
                food_crush_on2 = 1; //��ġ�� food_crush_on �� on 
                r++;
                break;
            }
        }
        if (food_crush_on2 == 1) continue; //������ ��� while���� �ٽ� ���� 
        for (i = length2; i < 0; i--) { //food�� �� ����� ��ġ���� Ȯ��  
            if (food_x == x2[i] && food_y == y2[i]) {
                food_crush_on2 = 1; //��ġ�� food_crush_on �� on 
                r--;
                break;
            }
        }
        if (food_crush_on2 == 1) continue; //������ ��� while���� �ٽ� ����

        gotoxy(MAP_X + food_x2, MAP_Y + food_y2, "��"); //�Ȱ����� ��� ��ǥ���� food�� ��� 
        speed -= 3; //�ӵ� ���� 
        break;
    }
}