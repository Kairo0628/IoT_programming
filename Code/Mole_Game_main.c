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
#define led_d "/dev/led"

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

void clcd_input2(int score_player, int score_mole)
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
    int i = 0;
    int item = 0;           // 아이템 개수
    int count = 0;          // 맞춘 개수
    int score_mole = 0;     // 두더지 점수
    int score_player = 0;   // 플레이어 점수
    int random1 = 0;        // 아이템 사용했을때 두더지 랜덤
    int x;                  // 10번스위치 for문

    int dot = 0; //dotmatrix 변수
    int tact = 0; //tact 변수
    int j = 0; //두더지 번호, 남은시간 초기화 변수
    int k = 0; //두더지 번호 중복검사 변수
    int q = 0; //출력할 매트릭스 재정의
    int p = 0; //출력할 매트릭스 재정의
    int z = 0; //스위치 입력 정답 검사
    int d = 0; //두더지번호 중복검사변수
    int u = 0; //두더지번호 중복검사변수2
    
    unsigned char c; //tactsw 조작 변수
    int num, isStop = 0; //숫자, 12번조작변수
    int isNext = 1; //다음스테이지 판단 변수
    int gonext = 0; //다음 두더지 생성 판단변수
    int stage = 0; //스테이지 변수
    int size = 0; //두더지 마릿수 지정변수
    int randomnumber = 0; //랜덤넘버 변수
    int timelimit; // 남은시간 검사변수;
    int renumber = 0; //다음두더지 재지정
    int renumber2 = 0;
    int nextmole = 0; //다음두더지 생성 판단변수
    int random[8] = { 0 }; //두더지들의 고유번호
    int timeleft[8] = { 0 }; //두더지들의 남은시간
    int w = 0;
    int q2 = 0;
    int p2 = 0;
    int startmole = 1;
    int k2 = 0;
    int q3 = 0;
    int stagetime = 20;

    int clcd;
    int led;
    
    unsigned char data;     // led에 사용
    
    srand(time(NULL));
    
    while(1)
    {
        //다음 단계로 넘어간다면
        if (isNext == 1)
        {
            nextmole = 0;
            isNext = 0;
            startmole = 1;  
            stage++;
            //1stage에는 한마리, 2stage에는 2마리...
            for (k2 = 0; k2 < 8; k2++)
            {
                printmatrix[k2] = 0;
            }

            for (j = 0; j < stage; j++)
            {
                renumber = (rand() % 9) + 1;
                timeleft[j] = (rand() % 10);
                for (k = 1; k < stage + 1; k++)
                {
                    if (renumber == random[k - 1])
                    {
                        renumber = (rand() % 9) + 1;
                        k--;
                    }
                }
                random[j] = renumber;
                renumber = 0;
            }
        }
        
        //스테이지 시작시 매트릭스에 두더지 표시
        if (startmole == 1)
        {
            startmole = 0;
            for (q = 0; q < stage; q++)
            {
                for (p = 0; p < 8; p++)
                {
                    printmatrix[p] += mole[random[q] - 1][p];
                    //printf("%d", printmatrix[p]);
                }
                //printf("\n");
            }
        }

        //다음 두더지를 매트릭스에 추가
        if (nextmole == 1)
        {
            nextmole = 0;
            for (p2 = 0; p2 < 8; p2++)
            {
                printmatrix[p2] += mole[renumber-1][p2];
                //printf("%d", printmatrix[p2]);
            }
            //printf("\n");
        }

        dot = open(dot_d, O_RDWR);
        if(dot < 0 | tact <0)
        {
            printf("오류 발생");
            exit(0);
        }
        write(dot, &printmatrix, sizeof(printmatrix));
        usleep(100000);
        close(dot);

        tact = open(tact_d, O_RDWR);
        read(tact, &c, sizeof(c));
        close(tact);
        switch(c)
        {
            case 1: num = 1; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 2: num = 2; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 3: num = 3; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 4: num = 4; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 5: num = 5; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 6: num = 6; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 7: num = 7; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 8: num = 8; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 9: num = 9; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 10: num = 10; printf("num : %d random : %d c : %d\n", num, random[0], c); usleep(100000);break;
            case 12: isStop = 1; printf("num : %d random : %d c : %d\n", num, random, c); break; 
        }

        //두더지 맞췄는지 검사
        for (z = 0; z < stage; z++)
        {
            //두더지가 맞았으면
            if (num == random[z])
            {
                count++;
                score_player += 1;
                printf("점수 : %d\n", score_player);
                clcd_input2(score_player, score_mole);

                //매트릭스에서 맞은 두더지 제거
                for (q2 = 0; q2 < 8; q2++)
                {
                    printmatrix[q2] = printmatrix[q2] - mole[num-1][q2];
                    //printf("%d", printmatrix[q2]);
                }
                //printf("\n");

                //두더지 번호 재지정, 남은시간 재지정
                timeleft[z] = rand() % 10;
                num = 0;
                renumber = rand() % 9 + 1;

                //두더지 중복뽑기 방지
                for (d = 0; d < stage; d++)
                {
                    if (renumber == random[d])
                    {
                        renumber = rand() % 9 + 1;
                        d--;
                    }
                }
                random[z] = renumber;
                gonext = 1;
                break;
            }
        }

        //다음 두더지 생성
        if (gonext == 1)
        {
            nextmole = 1;
            gonext = 0;
            continue;
        }

        // 스테이지 1 클리어
        if (score_player >= 10 && stage == 1)
        {
            isNext = 1;
            stagetime--;
            continue;
        }

        // 스테이지 2 클리어
        if (score_player > 20 && stage == 2)
        {
            isNext = 1;
            stagetime--;
            continue;
        }

        // 스테이지 3 클리어
        if (score_player > 40 && stage == 3)
        {
            isNext = 1;
            stagetime--;
            continue;
        }

        // 스테이지 4 클리어
        if (score_player > 50 && stage == 4)
        {
            isNext = 1;
            stagetime--;
            continue;
        }

        //긴급종료 변수
        if(isStop == 1)
        {
            
            break;
        }

        //두더지 못잡았을경우
        for (timelimit = 0; timelimit < stage; timelimit++)
        {
            //두더지 시간 + 1
            timeleft[timelimit] = timeleft[timelimit] + 1;
            //두더지를 못잡은채 시간이 지났는지 검사
            if (timeleft[timelimit] > stagetime)
            {
                score_mole += 1;
                clcd_input2(score_player, score_mole);
                printf("두더지점수 : %d \n", score_mole);
                //못잡은 두더지 매트릭스에서 제거
                for (q3 = 0; q3 < 8; q3++)
                {
                    printmatrix[q3] = printmatrix[q3] - mole[random[timelimit] -1][q3];    
                }
                //printf("\n");
                timeleft[timelimit] = 0;
                renumber = rand() % 9 + 1;
                for (u = 0; u < stage; u++)
                {
                    if (renumber == random[u])
                    {
                        renumber = rand() % 9 + 1;
                        u--;
                    }
                }
                random[timelimit] = renumber;
                nextmole = 1;
                break;
            }
        }

        if (count == 10)
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
            led = open(led_d, O_RDWR);
            if(led < 0) {printf("Can't Open\n"); exit(0);}  
            data = 0x7f;
            write(led, &data, sizeof(unsigned char));
            usleep(1);
            if (num == 10)
            {
                data = 0xff;
                item--;
                num = 0; // num값 초기화
                int x = 1;
                for(x;x<11;x++)        
                {
                    random1 = (rand()%9) + 1;
                    dot = open(dot_d, O_RDWR);
                    write(dot, &mole[random1-1], sizeof(mole));
                    usleep(250000);
                    close(dot);
                    score_player += 1;
                    printf("score_player : %d\n", score_player);   
                }
            }
        }
        else if(item == 2)
        {
            led = open(led_d, O_RDWR);
            if(led < 0) {printf("Can't Open\n"); exit(0);}
            data = 0x3f;
            write(led, &data, sizeof(unsigned char));
            usleep(1);
            if (num == 10)
            {
                data = 0x7f;
                write(led, &data, sizeof(unsigned char));
                item--;
                num = 0;
                int x = 1;
                for(x;x<11;x++)        
                {
                    random1 = (rand()%9) + 1;
                    dot = open(dot_d, O_RDWR);
                    write(dot, &mole[random1-1], sizeof(mole));
                    usleep(250000);
                    close(dot);
                    score_player += 1;
                    printf("score_player : %d\n", score_player);   
                }
            }
        }
        else if(item == 3)
        {
            close(led);
            led = open(led_d, O_RDWR);
            if(led < 0) {printf("Can't Open\n"); exit(0);}
            data = 0x1f;
            write(led, &data, sizeof(unsigned char));
            if (num == 10)
            {
                data = 0x3f;
                write(led, &data, sizeof(unsigned char));
                item--;
                num = 0;
                int x = 1;
                for(x;x<11;x++)        
                {
                    random1 = (rand()%9) + 1;
                    dot = open(dot_d, O_RDWR);
                    write(dot, &mole[random1-1], sizeof(mole));
                    usleep(250000);
                    close(dot);
                    score_player += 1;
                    printf("score_player : %d\n", score_player);   
                }
            }
        } 

        //두더지 혹은 플레이어가 50점넘으면 종료
        if(score_mole > 50 || score_player > 50)
        {
            if(score_mole >= 30)
            {
                printf("두더지 승리!\n");
                clcd_input2(score_player, score_mole);
                close(clcd);
            }  

            if(score_player >= 50)
            {
                printf("플레이어 승리!\n");
                clcd_input2(score_player, score_mole);
                close(clcd);
            }
            printf("게임을 종료합니다.\n");
            break;
        }
    }
    close(clcd);
    close(led);
    return 0;
}
