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
#include <unistd.h>
#include <time.h>

#define dot_d "/dev/dot"
#define tact_d "/dev/tactsw"


unsigned char mole[9][8] = {        // 두더지 배열
    // 도트 매트릭스 화면
    {0, 96, 96, 0, 0, 0, 0, 0}, //1번두더지
    {0, 24, 24, 0, 0, 0, 0, 0}, //2번두더지
    {0, 6, 6, 6, 0, 0, 0, 0}, //3번두더지
    {0, 0, 0, 96, 96, 0, 0, 0}, //4번두더지
    {0, 0, 0, 24, 24, 0, 0, 0}, //5번두더지
    {0, 0, 0, 6, 6, 0, 0, 0}, //6번두더지
    {0, 0, 0, 0, 0, 96, 96, 0}, //7번두더지
    {0, 0, 0, 0, 0, 24, 24, 0}, //8번두더지
    {0, 0, 0, 0, 0, 6, 6, 0}  //9번두더지
};

unsigned char printmatrix[8] = {0};     // 실제로 dot matrix에 출력할 

int main() {
    int dot = 0; //dotmatrix 변수
    int tact = 0; //tact 변수
    int j = 0; //두더지 번호, 남은시간 초기화 변수
    int k = 0; //두더지 번호 중복검사 변수1
    int q = 0; //출력할 매트릭스 재정의1
    int p = 0; //출력할 매트릭스 재정의2
    int z = 0; //스위치 입력 정답 검사
    int d = 0; //두더지번호 중복검사 변수2
    int u = 0; //두더지번호 중복검사 변수3
    int item = 0; //item변수
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
    int nextmole = 0;
    int random[8] = { 0 };
    int timeleft[8] = { 0 };
    int w = 0;
    int q2 = 0;
    int p2 = 0;
    int startmole = 1;
    int k2 = 0;
    int score = 0;
    int q3 = 0;
    int molescore = 0;

    srand(time(NULL)); //시간 랜덤 초기화
    while(1)
    { 
        //다음 단계로 넘어간다면
        if (isNext == 1)
        {
            nextmole = 0;
            isNext = 0;
            startmole = 1;  
            stage++;
            //1stage에는 한마리, 2stage에는 2마리...동적할당

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
        if (startmole == 1)
        {
            startmole = 0;
            for (q = 0; q < stage; q++)
            {
                for (p = 0; p < 8; p++)
                {
                    printmatrix[p] += mole[random[q] - 1][p];
                    printf("%d", printmatrix[p]);
                }
                printf("\n");
            }
        }

        if (nextmole == 1)
        {
            nextmole = 0;
            for (p2 = 0; p2 < 8; p2++)
            {
                printmatrix[p2] += mole[renumber-1][p2];
                printf("%d", printmatrix[p2]);
            }
            printf("\n");
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
        switch(c)               // 스위치 1~9번
        {
            case 1: num = 1; printf("num : %d random : %d c : %d\n", num, random[0], c); break;
            case 2: num = 2; printf("num : %d random : %d c : %d\n", num, random[0], c); break;
            case 3: num = 3; printf("num : %d random : %d c : %d\n", num, random[0], c); break;
            case 4: num = 4; printf("num : %d random : %d c : %d\n", num, random[0], c); break;
            case 5: num = 5; printf("num : %d random : %d c : %d\n", num, random[0], c); break;
            case 6: num = 6; printf("num : %d random : %d c : %d\n", num, random[0], c); break;
            case 7: num = 7; printf("num : %d random : %d c : %d\n", num, random[0], c); break;
            case 8: num = 8; printf("num : %d random : %d c : %d\n", num, random[0], c); break;
            case 9: num = 9; printf("num : %d random : %d c : %d\n", num, random[0], c);break;
            case 12: isStop = 1; printf("num : %d random : %d c : %d\n", num, random, c); break; 
        }

        for (z = 0; z < stage; z++)
        {
            if (num == random[z])           // 두더지를 맞췄을 때
            {
                printf("맞춤\n");
                score += 10;
                for (q2 = 0; q2 < 8; q2++)
                {
                    printmatrix[q2] = printmatrix[q2] - mole[num-1][q2];
                    printf("%d", printmatrix[q2]);
                }
                printf("\n");
                timeleft[z] = rand() % 10;
                num = 0;
                renumber = rand() % 9 + 1;
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

        if (gonext == 1)        //다음 두더지 생성 판단
        {
            nextmole = 1;
            gonext = 0;
            continue;
        }

        if (score >= 10 && stage == 1)
        {
            isNext = 1;
            continue;
        }

        if (score > 20 && stage == 2)
        {
            isNext = 1;
            continue;
        }

        if(isStop == 1)     // 12번 스위치 = 게임 종료
        {
            break;
        }

        for (timelimit = 0; timelimit < stage; timelimit++)     // 두더지를 놓치면 두더지가 점수를 획득
        {
            timeleft[timelimit] = timeleft[timelimit] + 1;
            if (timeleft[timelimit] > 10)
            {
                printf("두더지 %d 10점 get\n", timelimit+1);
                for (q3 = 0; q3 < 8; q3++)
                {
                    printmatrix[q3] = printmatrix[q3] - mole[random[timelimit] -1][q3];    
                }
                printf("\n");
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
    
    }


    return 0;
}
