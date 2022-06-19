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

#define ESC 0x1b //ESC 누르면 종료

#define SPECIAL1 0xe0 //특수키는 0xe0+key 값으로 구성된다
#define SPECIAL2 0x00 //keypad의 경우 0x00+key로 구성된다

#define UP 0x48 //Up key는 0xe0+0x48 2개의 값이 들어온다
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define UP2 'w' //player2는 wsad로 방향키를 대신함
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
COORD snake[SNAKE_LENGTH_MAX]; // snake 의 좌표를 gotoxy2()에서 사용하기 위함.
int snake_length = SNAKE_LENGTH;
int snake_length2 = SNAKE_LENGTH;
int Delay = 100;
int Delay2 = 10;

int x[100], y[100]; //x,y 좌표값을 저장 총 100개 
int x2[100], y2[100];
int food_x, food_y; //food의 좌표값을 저장 
int food_x2, food_y2;
int length; //몸길이를 기억 
int length2;
int speed; //게임 속도 
int speed2;
int score; //점수 저장  --reset함수에 의해 초기화됨
int score2;
int best_score = 0; //최고 점수 저장 --reset함수에 의해 초기화 되지 않음 
int best_score2 = 0;
int last_score = 0; //마지막 점수 저장  --reset함수에 의해 초기화 되지 않음
int last_score2 = 0;
int dir; //이동방향 저장 
int dir2;
int key; //입력받은 키 저장 
int key2;
//int life = LIFE;
//int life2 = LIFE;

void gotoxy(int x, int y, char* s) { //x값을 2x로 변경, 좌표값에 바로 문자열을 입력할 수 있도록 printf함수 삽입  
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
    printf("%s", s);
}

void title(void); //게임 시작화면 
void reset(void); //게임을 초기화 
void draw_map(void); // 게임판 테두리를 그림 
void move(int dir); //뱀머리를 이동 
void move2(int dir);
void pause(void); //일시정지 
void game_over(void); //게임 오버를 확인 
void food(void); // 음식 생성 
void food2(void);

void removeCursor(void) { // 커서를 안보이게 한다

    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void removeCursor2(void) { // 커서를 안보이게 한다

    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy2(int x2, int y2, char* s)
{
    COORD pos = { 2 * x2,y2 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
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
        if (kbhit()) do { key = getch(); } while (key == 224); //키 입력받음
        Sleep(speed);

        switch (key) { //입력받은 키를 파악하고 실행  
        case LEFT:
        case RIGHT:
        case UP:
        case DOWN:
            if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) ||
                (dir == DOWN && key != UP))//180회전이동을 방지하기 위해 필요. 
                dir = key;
            key = 0; // 키값을 저장하는 함수를 reset 
            break;
        case 'a':
        case 'd':
        case 'w':
        case 's':
            if ((dir2 == 'a' && key2 != 'd') || (dir2 == 'd' && key2 != 'a') || (dir2 == 'w' && key2 != 's') ||
                (dir2 == 's' && key2 != 'w'))//180회전이동을 방지하기 위해 필요. 
                dir2 = key;
            key = 0; // 키값을 저장하는 함수를 reset 
            break;
        case PAUSE: // P키를 누르면 일시정지 
            pause();
            break;
        case ESC: //ESC키를 누르면 프로그램 종료 
            exit(0);
        }
        move(dir);
        move2(dir2);
    }
}

void title(void) {
    int i, j;

    while (kbhit()) getch(); //버퍼에 있는 키값을 버림 

    draw_map();    //맵 테두리를 그림 
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { // 맵 테두리 안쪽을 빈칸으로 채움 
        for (j = MAP_X + 1; j < MAP_X + MAP_WIDTH - 1; j++) gotoxy(j, i, "  ");
    }

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 3, "■■■■ ■■■■\n");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 4, "■       ■    ■\n");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 5, "■  ■■ ■    ■\n");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 6, "■    ■ ■    ■\n");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 7, "■■■■ ■■■■\n\n\n");

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 9, "▲Press any key to start ▲\n");

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 11, "   ◇ ←,→,↑,↓ : 1player Move    ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 12, "   ◆ a,d,w,s : 2player Move    ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 13, "   ◆ food=>♥: 1player, ♡: 2player  ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 14, "   ◇ P : Pause             ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 5, MAP_Y + 15, "   ◇ ESC : Quit              ");

    while (TRUE) {
        if (kbhit() == 1) { //키입력받음 
            key = getch();
            break; //아니면 그냥 계속 진행 
        }
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " ▲PRESS ANY KEY TO START▲ ");
        Sleep(400);
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, "                            ");
        Sleep(400);

    }
    reset(); // 게임을 초기화  

}


void reset(void) {
    int i;
    //life = 3; //만약 게임이 끝나고 재시작 했을 때를 생각
   // life2 = 3;
    system("cls"); //화면을 지움 
    draw_map(); //맵 테두리를 그림 
    while (kbhit()) getch(); //버퍼에 있는 키값을 버림 

    dir = LEFT; // 방향 초기화  
    speed = 100; // 속도 초기화 
    length = 5; //뱀 길이 초기화 
    score = 0; //점수 초기화 
    for (i = 0; i < length; i++) { //뱀 몸통값 입력 
        x[i] = MAP_WIDTH / 2 + i;
        y[i] = MAP_HEIGHT / 2;
        gotoxy(MAP_X + x[i], MAP_Y + y[i], "■");
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "★"); //뱀 머리 그림 
    dir2 = 'a'; // 방향 초기화  
    speed2 = 100; // 속도 초기화 
    length2 = 5; //뱀 길이 초기화 
    score2 = 0; //점수 초기화 
    for (i = 0; i < length2; i++) { //뱀 몸통값 입력 
        x2[i] = MAP_WIDTH / 2 + i;
        y2[i] = MAP_HEIGHT / 2 + 5;
        gotoxy(MAP_X + x2[i], MAP_Y + y2[i], "□");
    }
    gotoxy(MAP_X + x2[0], MAP_Y + y2[0], "☆"); //뱀 머리 그림 
    food2(); // food 생성  
    food(); // food 생성  

}


void draw_map(void) { //맵 테두리 그리는 함수 
    int i, j;
    for (i = 0; i < MAP_WIDTH; i++) {
        gotoxy(MAP_X + i, MAP_Y, "■");
    }
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) {
        gotoxy(MAP_X, i, "■");
        gotoxy(MAP_X + MAP_WIDTH - 1, i, "■");
    }
    for (i = 0; i < MAP_WIDTH; i++) {
        gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "■");
    }
}

void move(int dir) {
    int i;
    if (x[0] == food_x && y[0] == food_y) { //food1(♥)와 충돌했을 경우 
        score += 10; //점수 증가 
        food(); //새로운 food 추가 
        length++; //길이증가 
        x[length - 1] = x[length - 2]; //새로만든 몸통에 값 입력 
        y[length - 1] = y[length - 2];
    }
    if (x[0] == food_x2 && y[0] == food_y2) { //food2(♡)와 충돌했을 경우 
        score -= 10; //점수 감소 
        food(); //새로운 food 추가 
        length--; //길이 감소
        x[length + 1] = x[length + 2]; //새로만든 몸통에 값 입력 
        y[length + 1] = y[length + 2];
    }
    if (x[0] == 0 || x[0] == MAP_WIDTH - 1 || y[0] == 0 || y[0] == MAP_HEIGHT - 1) { //벽과 충돌했을 경우 
        game_over();
        return; //game_over에서 게임을 다시 시작하게 되면 여기서부터 반복되므로 
                //return을 사용하여 move의 나머지 부분이 실행되지 않도록 합니다. 
    }
    for (i = 1; i < length; i++) { //자기몸과 충돌했는지 검사 
        if (x[0] == x[i] && y[0] == y[i]) {
            game_over();
            return;
        }
    }

    gotoxy(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  "); //몸통 마지막을 지움 
    for (i = length - 1; i > 0; i--) { //몸통좌표를 한칸씩 옮김 
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "■"); //머리가 있던곳을 몸통으로 고침 
    if (dir == LEFT) --x[0]; //방향에 따라 새로운 머리좌표(x[0],y[0])값을 변경 
    if (dir == RIGHT) ++x[0];
    if (dir == UP) --y[0];
    if (dir == DOWN) ++y[0];
    gotoxy(MAP_X + x[i], MAP_Y + y[i], "★"); //새로운 머리좌표값에 머리를 그림 
}
void move2(int dir) {
    int i;
    if (x2[0] == food_x2 && y2[0] == food_y2) { //food2(♡)와 충돌했을 경우 
        score2 += 10; //점수 증가 
        food2(); //새로운 food 추가 
        length2++; //길이증가 
        x2[length2 - 1] = x2[length2 - 2]; //새로만든 몸통에 값 입력 
        y2[length2 - 1] = y2[length2 - 2];
    }
    if (x2[0] == food_x && y2[0] == food_y) { //food(♥)와 충돌했을 경우 
        score2 -= 10; //점수 감소 
        food2(); //새로운 food 추가 
        length2--; //길이 감소 
        x2[length2 + 1] = x2[length2 + 2]; //새로만든 몸통에 값 입력 
        y2[length2 + 1] = y2[length2 + 2];
    }
    if (x2[0] == 0 || x2[0] == MAP_WIDTH - 1 || y2[0] == 0 || y2[0] == MAP_HEIGHT - 1) { //벽과 충돌했을 경우 
        game_over();
        return; //game_over에서 게임을 다시 시작하게 되면 여기서부터 반복되므로 
                //return을 사용하여 move의 나머지 부분이 실행되지 않도록 합니다. 
    }
    for (i = 1; i < length2; i++) { //자기몸과 충돌했는지 검사 
        if (x2[0] == x2[i] && y2[0] == y2[i]) {
            game_over();
            return;
        }
    }

    gotoxy(MAP_X + x2[length2 - 1], MAP_Y + y2[length2 - 1], "  "); //몸통 마지막을 지움 
    for (i = length2 - 1; i > 0; i--) { //몸통좌표를 한칸씩 옮김 
        x2[i] = x2[i - 1];
        y2[i] = y2[i - 1];
    }
    gotoxy(MAP_X + x2[0], MAP_Y + y2[0], "□"); //머리가 있던곳을 몸통으로 고침 
    if (dir2 == 'a') --x2[0]; //방향에 따라 새로운 머리좌표(x[0],y[0])값을 변경 
    if (dir2 == 'd') ++x2[0];
    if (dir2 == 'w') --y2[0];
    if (dir2 == 's') ++y2[0];
    gotoxy(MAP_X + x2[i], MAP_Y + y2[i], "☆"); //새로운 머리좌표값에 머리를 그림 
}

void pause(void) { // p키를 눌렀을 경우 게임을 일시 정지 
    while (1) {
        if (key == PAUSE) {
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "▲PAUSE : PRESS ANY KEY TO RESUME▲ ");
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

void game_over(void) { //게임종료 함수 
   // life--;
    //life2--;
    //if (life == 0 || life2 == 0)
   // {
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 3, "■■■ ■    ■");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 4, "■     ■■  ■");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 5, "■■■ ■ ■ ■");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 6, "■     ■  ■■");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 7, "■     ■    ■");
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 8, " 1player SCORE: ");
        printf("%d", last_score = score);
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 9, " 2player SCORE: ");
        printf("%d", last_score2 = score2);

        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, " ▲Press any keys to restart▲ ");

        if (score > best_score) {
            best_score = score;
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 4, MAP_Y + 10, "▲1player WINNER▲");
        }
        else if (score2 > best_score2) {
            best_score2 = score2;
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 4, MAP_Y + 11, "▲2player WINNER▲");
        }
        Sleep(500);
        while (kbhit()) getch();
        key = getch();
        title();
   // }
}

void food(void) {
    int i;

    int food_crush_on = 0;//food가 뱀 몸통좌표에 생길 경우 on 
    int r = 0; //난수 생성에 사동되는 변수 
    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, " ◆1player SCORE: "); //점수표시 
    printf("%3d, 1player LAST SCORE: %3d, 1player BEST SCORE: %3d", score, last_score, best_score);
   // gotoxy(MAP_X, MAP_Y + MAP_HEIGHT + 1, " ◆1player 남은 목숨: "); //점수표시
   // printf("%3d개", life);

    while (1) {
        food_crush_on = 0;
        srand((unsigned)time(NULL) + r); //난수표생성 
        food_x = (rand() % (MAP_WIDTH - 2)) + 1;    //난수를 좌표값에 넣음 
        food_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++) { //food가 뱀 몸통과 겹치는지 확인  
            if (food_x == x[i] && food_y == y[i]) {
                food_crush_on = 1; //겹치면 food_crush_on 를 on 
                r++;
                break;
            }
        }
        if (food_crush_on == 1) continue; //겹쳤을 경우 while문을 다시 시작 
        for (i = length; i < 0; i--) { //food가 뱀 몸통과 겹치는지 확인  
            if (food_x2 == x[i] && food_y2 == y[i]) {
                food_crush_on = 1; //겹치면 food_crush_on 를 on 
                r--;
                break;
            }
        }
        if (food_crush_on == 1) continue; //겹쳤을 경우 while문을 다시 시작 

        gotoxy(MAP_X + food_x, MAP_Y + food_y, "♥"); //안겹쳤을 경우 좌표값에 food를 찍고 
        speed -= 3; //속도 증가 
        break;
    }
}
void food2(void) {
    int i;

    int food_crush_on2 = 0;//food가 뱀 몸통좌표에 생길 경우 on 
    int r = 0; //난수 생성에 사동되는 변수 
    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT + 3, " ◇2player SCORE: "); //점수표시 
    printf("%3d, 2player LAST SCORE: %3d, 2player BEST SCORE: %3d", score2, last_score2, best_score2);
    //gotoxy(MAP_X, MAP_Y + MAP_HEIGHT + 4, " ◇2player 남은 목숨: "); //점수표시
    //printf("%3d개", life);

    while (1) {
        food_crush_on2 = 0;
        srand((unsigned)time(NULL) + r); //난수표생성 
        food_x2 = (rand() % (MAP_WIDTH - 5)) + 4;    //난수를 좌표값에 넣음 
        food_y2 = (rand() % (MAP_HEIGHT - 5)) + 4;

        for (i = 0; i < length2; i++) { //food가 뱀 몸통과 겹치는지 확인  
            if (food_x2 == x2[i] && food_y2 == y2[i]) {
                food_crush_on2 = 1; //겹치면 food_crush_on 를 on 
                r++;
                break;
            }
        }
        if (food_crush_on2 == 1) continue; //겹쳤을 경우 while문을 다시 시작 
        for (i = length2; i < 0; i--) { //food가 뱀 몸통과 겹치는지 확인  
            if (food_x == x2[i] && food_y == y2[i]) {
                food_crush_on2 = 1; //겹치면 food_crush_on 를 on 
                r--;
                break;
            }
        }
        if (food_crush_on2 == 1) continue; //겹쳤을 경우 while문을 다시 시작

        gotoxy(MAP_X + food_x2, MAP_Y + food_y2, "♡"); //안겹쳤을 경우 좌표값에 food를 찍고 
        speed -= 3; //속도 증가 
        break;
    }
}