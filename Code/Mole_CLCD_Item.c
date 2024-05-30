#include <asm/ioctls.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define dot_d "/dev/dot"
#define tact_d "/dev/tactsw"
#define clcd_d "/dev/clcd"
#define led "/dev/led"

unsigned char mole[9][8] = {
    // 도트 매트릭스 화면
    {0, 96, 96, 0, 0, 0, 0, 0}, //1번두더지
    {0, 24, 24, 0, 0, 0, 0, 0}, //2번두더지
    {0, 6, 6, 0, 0, 0, 0, 0},   //3번두더지
    {0, 0, 0, 96, 96, 0, 0, 0}, //4번두더지
    {0, 0, 0, 24, 24, 0, 0, 0}, //5번두더지
    {0, 0, 0, 6, 6, 0, 0, 0},   //6번두더지
    {0, 0, 0, 0, 0, 96, 96, 0}, //7번두더지
    {0, 0, 0, 0, 0, 24, 24, 0}, //8번두더지
    {0, 0, 0, 0, 0, 6, 6, 0}    //9번두더지
}; 

unsigned char printmatrix[8] = {0};

void clcd_input2(int score_player, int score_mole)  // CLCD 
{
    int clcd;
    char player_score[5];
    char mole_score[5];
    char clcd_text1[30] = "Player_Score: ";
    char clcd_text2[30] = "Mole_Score: ";
    
    clcd = open(clcd_d, O_RDWR);
    if (clcd < 0)                    // 예외처리
    {
        printf("clcd error\n");
    }
    sprintf(player_score, "%d  ", score_player);
    sprintf(mole_score, "%d", score_mole);
    strcat(clcd_text1, player_score);
    strcat(clcd_text1, clcd_text2);
    strcat(clcd_text1, mole_score);

    write(clcd, clcd_text1 , strlen(clcd_text1)); // 두번째부터 각각 문자열, 문자열 크기
}

int main() {
    int dot = 0;
    int tact = 0;
    int i = 0;
    int random = 0;         // 두더지 
    int item = 0;           // 아이템 개수
    int count = 0;          // 맞춘 개수
    int score_mole = 0;     // 두더지 점수
    int score_player = 0;   // 플레이어 점수
    unsigned char c;
    int num, isStop = 0;    // 스위치 버튼
    int x;                  // 10번스위치 for문

    int clcd;
    int dev;
    
    unsigned char data = 0;     // led에 사용
    
    srand(time(NULL));
    random = (rand()%9)+1; //난수 생성
    while(1)
    { 
        dot = open(dot_d, O_RDWR);
        if(dot < 0 | tact <0)
        {
            printf("오류 발생");
            exit(0);
        }
        write(dot, &mole[random-1], sizeof(mole));
        usleep(250000);
        close(dot);

        tact = open(tact_d, O_RDWR);
        read(tact, &c, sizeof(c));
        close(tact);
        switch(c)       // 스위치 1~9번
        {
            case 1: num = 1; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 2: num = 2; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 3: num = 3; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 4: num = 4; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 5: num = 5; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 6: num = 6; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 7: num = 7; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 8: num = 8; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 9: num = 9; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 10: num = 10; printf("num : %d random : %d c : %d\n", num, random, c); usleep(250000);break;
            case 12: isStop = 1; printf("num : %d random : %d c : %d\n", num, random, c); break; 
        }
        if(num == random)       // 두더지를 맞췄을 때
        {
            count++;
            score_player += 1;
            printf("점수 : %d\n", score_player);
            clcd_input2(score_player, score_mole);
            random = (rand()%9) + 1;
            continue;
        }

        if (count == 10)        //10번 맞췄을때
        {
            item ++;
            if(item >= 4)
            {
                item = 3;
            }
            count = 0;
        }

        if(item == 1)
        {
            dev = open(led, O_RDWR);
            if(dev < 0) {printf("Can't Open\n"); exit(0);}  
            data = 0x80;
            write(dev, &data, sizeof(unsigned char));
            usleep(3000000);
            if (num == 10)              // 10번 스위치 아이템 사용
            {
                close(dev);
                item--;
                num == 0;
                int x = 1;
                for(x;x<11;x++)        // 두더지 10마리 자동으로 잡기
                {
                    random = (rand()%9) + 1;
                    dot = open(dot_d, O_RDWR);
                    write(dot, &mole[random-1], sizeof(mole));
                    usleep(250000);
                    close(dot);
                    score_player += 1;
                    printf("score_player : %d\n", score_player);   
                }
            }
        }
        else if(item == 2)
        {
            data = 0xc0;
            write(dev, &data, sizeof(unsigned char));
            if (num == 10)
            {
                data = 0x80;
                write(dev, &data, sizeof(unsigned char));
                item--;
                num == 0;
                int x = 1;
                for(x;x<11;x++)        
                {
                    random = (rand()%9) + 1;
                    dot = open(dot_d, O_RDWR);
                    write(dot, &mole[random-1], sizeof(mole));
                    usleep(250000);
                    close(dot);
                    score_player += 1;
                    printf("score_player : %d\n", score_player);   
                }
            }
        }
        else if(item == 3)
        {
            data = 0xe0;
            write(dev, &data, sizeof(unsigned char));
            if (num == 10)
            {
                data = 0x80;
                write(dev, &data, sizeof(unsigned char));
                item--;
                num == 0;
                int x = 1;
                for(x;x<11;x++)        
                {
                    random = (rand()%9) + 1;
                    dot = open(dot_d, O_RDWR);
                    write(dot, &mole[random-1], sizeof(mole));
                    usleep(250000);
                    close(dot);
                    score_player += 1;
                    printf("score_player : %d\n", score_player);   
                }
            }
        }  
        if(isStop == 1)     // 12번 스위치 = 게임 종료
        {
            break;
        }

        i += 1;
        if(i>4)
        {
            i = 0;
            random = (rand()%9) + 1;
        }
    }
    close(clcd);
    close(dev);
    return 0;
}
