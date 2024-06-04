// unsigned char bomb_mole[9][8] = {
//   {0, 64, 32, 0, 0, 0, 0, 0},
//   {0, 16, 8, 0, 0, 0, 0, 0},
//   {0, 4, 2, 0, 0, 0, 0, 0},
//   {0, 0, 0, 64, 32, 0, 0, 0},
//   {0, 0, 0, 16, 8, 0, 0, 0},
//   {0, 0, 0, 4, 2, 0, 0, 0},
//   {0, 0, 0, 0, 0, 64, 32, 0},
//   {0, 0, 0, 0, 0, 16, 8, 0},
//   {0, 0, 0, 0, 0, 4, 2, 0}
// };
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define dot_d "/dev/dot"
#define tact_d "/dev/tactsw"
#define clcd_d "/dev/clcd"
#define led_d "/dev/led"
#define dip_d "/dev/dipsw"

unsigned char mole[9][8] = {
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

void clcd_input2(int score_player, int score_mole) {
    int clcd;
    char player_score[5];
    char mole_score[5];
    char clcd_text1[30] = "Player_Score: ";
    char clcd_text2[30] = "Mole_Score: ";

    clcd = open(clcd_d, O_RDWR);
    if (clcd < 0) {
        printf("clcd error\n");
        return;
    }
    sprintf(player_score, "%d  ", score_player);
    sprintf(mole_score, "%d", score_mole);
    strcat(clcd_text1, player_score);
    strcat(clcd_text1, clcd_text2);
    strcat(clcd_text1, mole_score);

    write(clcd, clcd_text1, strlen(clcd_text1));
    close(clcd);
}

int main() {
    int bomb_mole_index;
    int is_bomb = 0;

    int dot = 0;
    int tact = 0;
    int num, isStop = 0;
    int random_mole = 0;
    int timeleft = 0;
    int startmole = 1;
    int stagetime = 20;
    int bomb_probability = 3;

    unsigned char c;

    int dip = 0;
    unsigned char dipval = 0;
    dip = open(dip_d, O_RDONLY);
    if (dip < 0) {
        printf("dip error\n");
        return 1;
    }

    srand(time(NULL));
    printf("DIP 스위치를 켜면 게임이 시작됩니다.\n");

    while (1) {
        // DIP 스위치 값 읽기
        read(dip, &dipval, sizeof(dipval));

        if (dipval != 0) {
            // DIP 스위치 값에 따라 난이도 설정
            if (dipval & 0x01) {
                stagetime = 40; // DIP 스위치 1번 ON: 난이도 1 (가장 쉬움)
            } else if (dipval & 0x02) {
                stagetime = 35; // DIP 스위치 2번 ON: 난이도 2
            } else if (dipval & 0x04) {
                stagetime = 30; // DIP 스위치 3번 ON: 난이도 3
            } else if (dipval & 0x08) {
                stagetime = 25; // DIP 스위치 4번 ON: 난이도 4
            } else if (dipval & 0x10) {
                stagetime = 20; // DIP 스위치 5번 ON: 난이도 5
            } else if (dipval & 0x20) {
                stagetime = 15; // DIP 스위치 6번 ON: 난이도 6
            } else if (dipval & 0x40) {
                stagetime = 10; // DIP 스위치 7번 ON: 난이도 7
            } else if (dipval & 0x80) {
                stagetime = 5;  // DIP 스위치 8번 ON: 난이도 8 (가장 어려움)
            }

        // 게임 초기화
        int count = 0;
        int score_mole = 0;
        int score_player = 0;
        int startmole = 1;

        while (1) {
            // 두더지 생성
            if (startmole == 1) {
                startmole = 0;
                random_mole = (rand() % 9) + 1;
                timeleft = (rand() % 10);

                // 폭탄 생성 확률에 따라 폭탄 두더지 설정
                if (rand() % bomb_probability == 0)
                    bomb_mole_index = random_mole - 1;
                else
                    bomb_mole_index = -1;

                int p;
                for (p = 0; p < 8; p++) {
                    if (bomb_mole_index == random_mole - 1)
                        printmatrix[p] = bomb_mole[bomb_mole_index][p];
                    else
                        printmatrix[p] = mole[random_mole - 1][p];
                }
            }

            dot = open(dot_d, O_RDWR);
            if (dot < 0) {
                printf("dot error\n");
                exit(0);
            }
            write(dot, &printmatrix, sizeof(printmatrix));
            usleep(100000);
            close(dot);

            tact = open(tact_d, O_RDWR);
            read(tact, &c, sizeof(c));
            close(tact);
            switch (c) {
                case 1: num = 1; usleep(100000); break;
                case 2: num = 2; usleep(100000); break;
                case 3: num = 3; usleep(100000); break;
                case 4: num = 4; usleep(100000); break;
                case 5: num = 5; usleep(100000); break;
                case 6: num = 6; usleep(100000); break;
                case 7: num = 7; usleep(100000); break;
                case 8: num = 8; usleep(100000); break;
                case 9: num = 9; usleep(100000); break;
                case 12: isStop = 1; break;
            }

            // 두더지 맞췄는지 검사
            if (num == random_mole) {
                // 폭탄 두더지를 클릭한 경우
                if (bomb_mole_index == random_mole - 1) {
                    is_bomb = 1;
                    score_player -= 2; // 점수 2점 차감
                    printf("폭탄을 클릭했습니다! 점수: %d\n", score_player);
                } else {
                    is_bomb = 0;
                    count++;
                    score_player += 1;
                    printf("점수: %d\n", score_player);
                }

                clcd_input2(score_player, score_mole);

                // 매트릭스에서 맞은 두더지 제거
                int q2;
                for (q2 = 0; q2 < 8; q2++) {
                    printmatrix[q2] = 0;
                }

                // 두더지 번호 재지정, 남은시간 재지정
                timeleft = rand() % 10;
                num = 0;
                random_mole = rand() % 9 + 1;

                // 폭탄 생성 확률에 따라 폭탄 두더지 설정
                if (rand() % bomb_probability == 0)
                    bomb_mole_index = random_mole - 1;
                else
                    bomb_mole_index = -1;

                // 새로운 두더지 표시
                int p;
                for (p = 0; p < 8; p++) {
                    if (bomb_mole_index == random_mole - 1)
                        printmatrix[p] = bomb_mole[bomb_mole_index][p];
                    else
                        printmatrix[p] = mole[random_mole - 1][p];
                }
            }

            // 두더지 못잡았을 경우
            timeleft++;
            if (timeleft > stagetime) {
                // 폭탄이 아닌 경우에만 두더지 점수 증가
                if (bomb_mole_index != random_mole - 1) {
                    score_mole += 1;
                    clcd_input2(score_player, score_mole);
                    printf("두더지점수: %d \n", score_mole);
                }

                // 못잡은 두더지 매트릭스에서 제거
                int q2;
                for (q2 = 0; q2 < 8; q2++) {
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
                int p;
                for (p = 0; p < 8; p++) {
                    if (bomb_mole_index == random_mole - 1)
                        printmatrix[p] = bomb_mole[bomb_mole_index][p];
                    else
                        printmatrix[p] = mole[random_mole - 1][p];
                }
            }

            // 두더지 혹은 플레이어가 50점넘으면 종료
            if (score_mole > 50 || score_player > 50) {
                if (score_mole >= 30) {
                    printf("두더지 승리!\n");
                    clcd_input2(score_player, score_mole);
                }

                if (score_player >= 50) {
                    printf("플레이어 승리!\n");
                    clcd_input2(score_player, score_mole);
                }
                printf("게임을 종료합니다.\n");
                break;
            }

                // DIP 스위치 값 다시 읽기
                read(dip, &dipval, sizeof(dipval));

                // DIP 스위치가 꺼지면 게임 종료
                if (dipval == 0) {
                    printf("게임을 종료합니다.\n");
                    break;
                }
            }
        } else {
            // DIP 스위치가 꺼져 있으면 대기
            
            sleep(2); // 1초 대기
        }
    }

    close(dip);
    return 0;
}