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

unsigned char bomb_mole[9][8] = {
  {0, 64, 32, 0, 0, 0, 0, 0},
  {0, 16, 8, 0, 0, 0, 0, 0},
  {0, 4, 2, 0, 0, 0, 0, 0},
  {0, 0, 0, 64, 32, 0, 0, 0},
  {0, 0, 0, 16, 8, 0, 0, 0},
  {0, 0, 0, 4, 2, 0, 0, 0},
  {0, 0, 0, 0, 0, 64, 32, 0},
  {0, 0, 0, 0, 0, 16, 8, 0},
  {0, 0, 0, 0, 0, 4, 2, 0}
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
    int count = 0;          // 맞춘 개수
    int score_mole = 0;     // 두더지 점수
    int score_player = 0;   // 플레이어 점수
    int bomb_mole_index;
    int is_bomb = 0;

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
    int stage = 0; //스테이지 변수
    int randomnumber = 0; //랜덤넘버 변수
    int timelimit; // 남은시간 검사변수
    int renumber = 0; //다음두더지 재지정
    int renumber2 = 0;
    int random_mole = 0; //두더지 고유번호
    int timeleft = 0; //두더지 남은시간
    int q2 = 0;
    int p2 = 0;
    int startmole = 1;
    int stagetime = 20; // 초기 스테이지 시간 설정
    int bomb_probability = 3; // 폭탄 생성 확률 (1/3)

    int clcd;
    
    srand(time(NULL));
    
    while(1)
    {
        //다음 단계로 넘어간다면
        if (isNext == 1)
        {
            isNext = 0;
            startmole = 1;  
            stage++;
            
            for (k = 0; k < 8; k++)
            {
                printmatrix[k] = 0;
            }
            
            // 스테이지 시간 감소
            stagetime -= 2;
            if (stagetime < 5)
                stagetime = 5;

            // 한 마리의 두더지 생성
            random_mole = (rand() % 9) + 1;
            timeleft = (rand() % 10);
            
            // 폭탄 생성 확률에 따라 폭탄 두더지 설정
            if (rand() % bomb_probability == 0)
                bomb_mole_index = random_mole - 1;
            else
                bomb_mole_index = -1;
        }
        
        //스테이지 시작시 매트릭스에 두더지 표시
        if (startmole == 1)
        {
            startmole = 0;
            
            for (p = 0; p < 8; p++)
            {
                if (bomb_mole_index == random_mole - 1)
                    printmatrix[p] = bomb_mole[bomb_mole_index][p];
                else
                    printmatrix[p] = mole[random_mole - 1][p];
            }
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
            case 1: num = 1; usleep(100000);break;
            case 2: num = 2; usleep(100000);break;
            case 3: num = 3; usleep(100000);break;
            case 4: num = 4; usleep(100000);break;
            case 5: num = 5; usleep(100000);break;
            case 6: num = 6; usleep(100000);break;
            case 7: num = 7; usleep(100000);break;
            case 8: num = 8; usleep(100000);break;
            case 9: num = 9; usleep(100000);break;
            case 12: isStop = 1; break; 
        }

        //두더지 맞췄는지 검사
        if (num == random_mole)
        {
            // 폭탄 두더지를 클릭한 경우
            if (bomb_mole_index == random_mole - 1)
            {
                is_bomb = 1;
                score_player -= 2; // 점수 2점 차감
                printf("폭탄을 클릭했습니다! 점수: %d\n", score_player);
            }
            else
            {
                is_bomb = 0;
                count++;
                score_player += 1;
                printf("점수: %d\n", score_player);
            }
            
            clcd_input2(score_player, score_mole);

            //매트릭스에서 맞은 두더지 제거
            for (q2 = 0; q2 < 8; q2++)
            {
                printmatrix[q2] = 0;
            }

            //두더지 번호 재지정, 남은시간 재지정
            timeleft = rand() % 10;
            num = 0;
            random_mole = rand() % 9 + 1;
            
            // 폭탄 생성 확률에 따라 폭탄 두더지 설정
            if (rand() % bomb_probability == 0)
                bomb_mole_index = random_mole - 1;
            else
                bomb_mole_index = -1;
            
            // 새로운 두더지 표시
            for (p = 0; p < 8; p++)
            {
                if (bomb_mole_index == random_mole - 1)
                    printmatrix[p] = bomb_mole[bomb_mole_index][p];
                else
                    printmatrix[p] = mole[random_mole - 1][p];
            }
        }

        // 스테이지 클리어 조건
        if (score_player >= stage * 10)
        {
            isNext = 1;
            continue;
        }

        //긴급종료 변수
        if(isStop == 1)
        {   
            break;
        }

        //두더지 못잡았을경우
        timeleft++;
        if (timeleft > stagetime)
        {
            // 폭탄이 아닌 경우에만 두더지 점수 증가
            if (bomb_mole_index != random_mole - 1)
            {
                score_mole += 1;
                clcd_input2(score_player, score_mole);
                printf("두더지점수: %d \n", score_mole);
            }
            
            //못잡은 두더지 매트릭스에서 제거
            for (q2 = 0; q2 < 8; q2++)
            {
                printmatrix[q2] = 0;
            }
            
            timeleft = 0;
            random_mole = rand() % 9 + 1;
            
            // 폭탄 생성 확률에 따라 폭탄 두더지 설정
            if (rand() % bomb_probability == 0)
                bomb_mole_index = random_mole - 1;
            else
                bomb_mole_index = -1;
            
            // 새로운 두더지 표시
            for (p = 0; p < 8; p++)
            {
                if (bomb_mole_index == random_mole - 1)
                    printmatrix[p] = bomb_mole[bomb_mole_index][p];
                else
                    printmatrix[p] = mole[random_mole - 1][p];
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
    return 0;
}