#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#pragma warning(disable:4996)

int mainarr[10][120];
int notouch[4][7] = { {0,3,0,2,0,3,0}, {3,0,2,2,2,0,3}, {0,2,2,2,2,2,0}, {2,2,2,2,2,2,2} }; // 큰 장애물 
int notouch2[7][5] = { {2,2,2,2,2}, {2,2,2,2,2}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,3,0,0}, {3,0,2,0,3}, {0,2,2,2,0} }; // 작은 장애물 
int i, j, height = 0, updown = 0, waitforstruct = 0, score = 0, curtime, jellywait = 0, health = 3, waitforheal = 0;
char jumpkey = 'w', jumpkey2 = 'q';

typedef struct coordin {
	int x;
	int y;
} coord;

coord player[4];

int isitgameover() // 장애물에 닿았는지 확인
{
	if ((mainarr[8 - height][4] == 2 || mainarr[8 - height][3] == 2 || mainarr[7 - height][4] == 2 || mainarr[7 - height][3] == 2) && waitforheal >= 15)
	{
		health--;
		waitforheal = 0;
		if (health <= 0)
		{
			return 1;
		}
	}
	return 0;
}

void isititem() // 아이템을 먹었는지 확인
{
	if (mainarr[8 - height][4] == 3 || mainarr[8 - height][3] == 3 || mainarr[7 - height][4] == 3 || mainarr[7 - height][3] == 3) // 아이템이 다음 프레임에 캐릭터한테 닿으면 
	{
		if (height > 0) // 장애물에 있는 아이템은 보너스 줌
		{
			score += 500;
		}
		else // 바닥에 있는 아이템 먹음
		{
			score += 30;
		}
	}
}

int wavegameover() // 장애물에 닿았는지 확인
{
	if (mainarr[player[0].y-12][player[0].x-5] == 2 && waitforheal >= 15)
	{
		health--;
		waitforheal = 0;
		if (health <= 0)
		{
			return 1;
		}
	}
	return 0;
}

void waveitem() // 아이템을 먹었는지 확인
{
	if (mainarr[player[0].y-12][player[0].x-5] == 3) // 아이템이 다음 프레임에 캐릭터한테 닿으면 
	{
		score += 50;
	}
}

void SetConsole() // 콘솔창 세팅 
{
	system("title ");
	system("mode con:cols=130 lines=28");
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 1;
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);
}

void gotoxy(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle;
	COORD Cur = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void jump() // 점프
{
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 4; j++)
		{
			mainarr[7 - height + i][1 + j] = 0;
		}
	}
	if (updown == 1 || updown == 3) // 올라갈때 
	{
		height++;
		for (i = 0; i < 4; i++)
		{
			player[i].y--;
		}
		if (updown == 3 && height >= 4)
		{
			updown++;
		}
		if (height >= 7)
		{
			updown++;
		}
	}
	if (updown == 2 || updown == 4)
	{
		height--;
		for (i = 0; i < 4; i++)
		{
			player[i].y++;
		}
		if (height == 0)
		{
			updown = 0;
		}
	}
}

void wave() // 웨이브 모드 점프함수
{
	if(updown == 1)
	{
		if(height < 9)
		{
			player[i].y--;
			height++;
		}
	}
	if(updown == 2)
	{
		if(height > 0)
		{
			player[i].y++;
			height--;
		}
	}
} 

int randomstruct() // 장애물 랜덤생성 (생성 성공하면 return 1)
{
	if (waitforstruct >= 25 && rand() % 8 == 0)
	{
		waitforstruct = 0;
		return 1;
	}
	waitforstruct++;
	return 0;
}

void settingthegame() // 게임 세팅하는거 (변수 초기화, 이차원 배열 초기화)
{
	height = 0;
	updown = 1;
	waitforstruct = 0;
	score = 0;
	health = 3;

	// 바닥 만들기

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 120; j++)
		{
			mainarr[i][j] = 0;
		}
	}
	for (i = 0; i < 120; i++)
	{
		mainarr[9][i] = 1;
	}
	for (i = 0; i < 40; i++)
	{
		mainarr[8][i * 3] = 3;
	}
}

void generate_jelly() // 아이템 생성 
{
	if (jellywait == 2)
	{
		mainarr[8][118] = 3;
		jellywait = 0;
	}
	else
	{
		jellywait++;
	}
}

void generate_badstuff() // 장애물 생성 
{
	if (rand() % 2 == 0) // 큰 장애물
	{
		for (i = 0; i < 9; i++) // 장애물 자리에 있는 아이템 없애기
		{
			mainarr[8][110 + i] = 0;
		}
		for (i = 0; i < 4; i++) // 장애물 생성 (아이템 포함) 
		{
			for (j = 0; j < 7; j++)
			{
				mainarr[5 + i][111 + j] = notouch[i][j];
			}
		}
	}
	else if (rand() % 2 == 1) // 작은 장애물
	{
		for (i = 0; i < 7; i++) // 장애물 자리에 있는 아이템 없애기
		{
			mainarr[8][111 + i] = 0;
		}
		for (i = 0; i < 7; i++) // 장애물 생성 (아이템 포함) 
		{
			for (j = 0; j < 5; j++)
			{
				mainarr[2 + i][112 + j] = notouch2[i][j];
			}
		}
	}
}

void health_and_score()
{
	gotoxy(1, 1);
	printf("score: \033[96m%d\033[0m", score); // score 출력 
	for (i = 0; i < 3; i++)
	{
		gotoxy(1 + i * 2, 3);
		printf("  ");
	}
	for (i = 0; i < health; i++)
	{
		gotoxy(1 + i * 2, 3);
		printf("\033[31m♥\033[0m");
	}
	gotoxy(5, 23);
	printf("Jump Keys : 'q' for small jump, 'w' for normal jump.");
	gotoxy(5, 24);
	printf("\033[33mif you can't jump, check if caps lock is on.\033[0m");
}

int main()
{
	srand(time(NULL));
	SetConsole();
	while (1)
	{
		printf("choose the mode\n");
		printf("1. jumping mode\n");
		printf("2. wave mode\n");
		int input = 0;
		scanf("%d", &input);
		if(input == 1) // 점핑모드 
		{
			system("cls");
			// 게임 시작할때 정수 초기화 
			for (i = 0; i < 2; i++)
			{
				player[i].x = 7;
				player[i].y = 19 + i;
			}
			for (i = 0; i < 2; i++)
			{
				player[i+2].x = 8;
				player[i+2].y = 19 + i;
			}
			int gameover = 0;
			generate_jelly();
			settingthegame();
			while (1)
			{
				int a = 0;
				if (_kbhit())
				{
					a = _getch();
				}
				if ((a == jumpkey) && height == 0) // 점프조건 
				{
					updown = 1;
				}
				if ((a == jumpkey2) && height == 0) // 작은점프조건 
				{
					updown = 3;
				}
				if (updown != 0)
				{
					jump(); // 점프 신호에 맞춰서 캐릭터 위치 조정 
				}
				generate_jelly(); // 일정 시간마다 아이템 추가 
				if (randomstruct() == 1) // 장애물 생성해야할때
				{
					generate_badstuff(); // 장애물을 생성시키는 함수 실행 
				}
				gameover = isitgameover(); // 장애물에 부딪혔는지 체크 
				isititem(); // 아이템을 먹었는지 체크 
				for (i = 0; i < 10; i++) // 오브젝트의 x좌표를 -1씩 바꾸기 
				{
					for (j = 0; j < 119; j++)
					{
						mainarr[i][j] = mainarr[i][j + 1];
					}
				}
				for (j = 0; j < 9; j++)
				{
					mainarr[j][0] = 0; // 플레이어를 지난 오브젝트는 삭제됨
				}
				for (i = 0; i < 10; i++) // mainarr 출력 
				{
					for (j = 0; j < 120; j++)
					{
						gotoxy(j + 5, i + 12);
						if (mainarr[i][j] == 0)
						{
							printf("  ");
							continue;
						}
	
						if (mainarr[i][j] == 1) // 바닥 
						{
							printf("￣");
							continue;
						}
						if (mainarr[i][j] == 2) // 장애물 
						{
							printf("\033[91m!\033[0m");
							continue;
						}
						if (mainarr[i][j] == 3) // 아이템 
						{
							printf("\033[33m*\033[0m");
							continue;
						}
					}
					printf("\n");
				}
				gotoxy(player[i].x, player[i].y);
				printf("@");
				for (i = 0; i < 4; i++) // 플레이어 출력
				{
					gotoxy(player[i].x, player[i].y);
					printf("@");
				}
				health_and_score();
				if (gameover == 1) // 게임오버면 while문 나가기 
				{
					break;
				}
				curtime += 1; // 프레임마다 추가되는 변수 
				waitforheal += 1; // 장애물에 닿으면 일정 시간동안 장애물 판정을 무시하는 변수 
				if (curtime > 2) // 일정프레임마다 스코어 추가 
				{
					score++;
				}
			}
			system("cls");
			Sleep(300);
			printf("Game Over\n");
			printf("your score : \033[96m%d\033[0m\n\nretry: 'r'\n", score);
			while (1)
			{
				char b = 0;
				b = _getch();
				if (b == 'r' || b == 'R') // retry
				{
					system("cls");
					break;
				}
			}
			Sleep(200);
		}
		if(input == 2) // 웨이브 모드 
		{
			system("cls");
			// 게임 시작할때 정수 초기화 
			player[0].x = 8;
			player[0].y = 20;
			int gameover = 0;
			settingthegame();
			while (1)
			{
				int a = 0;
				if (_kbhit())
				{
					a = _getch();
				}
				if (a == jumpkey) // 점프조건 
				{
					updown = 1;
				}
				else
				{
					updown = 2;
				}
				if (updown != 0)
				{
					wave(); // 점프 신호에 맞춰서 캐릭터 위치 조정 
				}
				generate_jelly(); // 일정 시간마다 아이템 추가 
				if (randomstruct() == 1) // 장애물 생성해야할때
				{
					generate_badstuff(); // 장애물을 생성시키는 함수 실행 
				}
				gameover = wavegameover(); // 장애물에 부딪혔는지 체크 
				waveitem(); // 아이템을 먹었는지 체크 
				for (i = 0; i < 10; i++) // 오브젝트의 x좌표를 -1씩 바꾸기 
				{
					for (j = 0; j < 119; j++)
					{
						mainarr[i][j] = mainarr[i][j + 1];
					}
				}
				for (j = 0; j < 9; j++)
				{
					mainarr[j][0] = 0; // 플레이어를 지난 오브젝트는 삭제됨
				}
				for (i = 0; i < 10; i++) // mainarr 출력 
				{
					for (j = 0; j < 120; j++)
					{
						gotoxy(j + 5, i + 12);
						if (mainarr[i][j] == 0)
						{
							printf("  ");
							continue;
						}
	
						if (mainarr[i][j] == 1) // 바닥 
						{
							printf("￣");
							continue;
						}
						if (mainarr[i][j] == 2) // 장애물 
						{
							printf("\033[91m!\033[0m");
							continue;
						}
						if (mainarr[i][j] == 3) // 아이템 
						{
							printf("\033[33m*\033[0m");
							continue;
						}
					}
					printf("\n");
				}
				gotoxy(player[0].x, player[0].y);
				printf("@");
				health_and_score();
				if (gameover == 1) // 게임오버면 while문 나가기 
				{
					break;
				}
				curtime += 1; // 프레임마다 추가되는 변수 
				waitforheal += 1; // 장애물에 닿으면 일정 시간동안 장애물 판정을 무시하는 변수 
				if (curtime > 2) // 일정프레임마다 스코어 추가 
				{
					score++;
				}
			}
			system("cls");
			Sleep(300);
			printf("Game Over\n");
			printf("your score : \033[96m%d\033[0m\n\nretry: 'r'\n", score);
			while (1)
			{
				char b = 0;
				b = _getch();
				if (b == 'r' || b == 'R') // retry
				{
					system("cls");
					break;
				}
			}
			Sleep(200);
		}
		else
		{
			printf("choose 1 or 2");
			Sleep(500);
			system("cls");
		}
	}
}
