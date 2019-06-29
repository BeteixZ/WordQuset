//V1.0
#define _CRT_SUCURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <direct.h>

#include "MakeFile.h"

#define InterfaceX 50
#define InterfaceY 50

#define KeyESC 27
#define KeyLeft 75
#define KeyRight 77

enum _COLOUR_
{
	white,
	grey,
	red,
	green,
	bgGreen,
	bgRed
} COLOUR;

struct Dictionary
{
	char english[255];
	char chinese[255];
};

int wordIndex = 0;
struct Dictionary *words = NULL;

void initInterface();
void setColour(int colour);
void moveTo(int x, int y);
void hiddenCursor();
void showCursor();
void loadDictionary();
void dictionaryPreview();
void putEmpty(int x, int y, int blocks);
void menuSelect();
void testEnglish();
int *generateRandomArray(int length);
float readScore();
void writeScore(float score);
void testChinese();

int main()
{
	creatDataDir();
	creatScoreDir();
	initInterface();
	chooseFile();
	wordIndex = getDictionaryIndex();
	if (wordIndex == 0)
	{
		moveTo(5, 3);
		setColour(bgRed);
		printf("�Ҳ����ʿ���޵��ʣ���������˳���");
		getch();
		return 0;
	}
	words = (struct Dictionary *)malloc(wordIndex * sizeof(struct Dictionary));
	loadDictionary();
	menuSelect();
	return 0;
}
void menuSelect()
{
	while (1)
	{
		initInterface();
		int index;
		moveTo(25, 25);
		printf("1.�������\n2.�����ı�Ӣ��\n3.��Ӣ�ı�����\n4.���ӵ���\n5.ɾ������\n6.Ԥ���ʿ�\n7.���Ĵʿ�\n");
		printf("���������ѡ��:");
		scanf("%d", &index);
		switch (index)
		{
		case 1:
			initInterface();
			dictionaryPreview();
			getchar();
			break;
		case 2:
			initInterface();
			testEnglish();
			break;
		case 3:
			initInterface();
			testChinese();
			break;
		case 4:
			initInterface();
			char chinese[255];
			char english[255];
			moveTo(5, 3);
			printf("����������:");
			scanf("%s", chinese);
			moveTo(5, 4);
			printf("������Ӣ��:");
			scanf("%s", english);
			addDictionary(chinese, english);
			loadDictionary();
			break;
		case 5:
			initInterface();
			moveTo(5, 3);
			printf("������Ӣ��:");
			scanf("%s", english);
			deleteDictionary(english);
			loadDictionary();
			break;
		case 6:
			initInterface();
			displayDictionary();
			printf("\n�����������");
			getchar();
			getchar();
			break;
		case 7:
			initInterface();
			chooseFile();
			break;
		default:
			break;
		}
	}
}
void loadDictionary()
{
	int index = 0;
	char buffer[255];
	FILE *fp = NULL;
	fp = fopen(path, "r");
	while (fgets(buffer, 255, fp))
	{
		strcpy((words + index)->chinese, strtok(buffer, "#"));
		strcpy((words + index)->english, strtok(NULL, "#"));
		index++;
	}
}
void dictionaryPreview()
{
	int index = 0;
	char ch;
	if (wordIndex == 0)
	{
		hiddenCursor();
		setColour(red);
		moveTo(4, 1);
		printf("�޵���!\n");
		setColour(white);
		printf("����������ز˵�!");
		getch();
		menuSelect();
	}
	while (1)
	{
		hiddenCursor();
		moveTo(5, 3);
		putEmpty(5, 3, 40);
		printf("��%d/%d������:", index + 1, wordIndex);
		moveTo(5, 5);
		putEmpty(5, 5, 40);
		printf("����:%s", (words + index)->english);
		moveTo(5, 6);
		putEmpty(5, 7, 40);
		printf("����:%s", (words + index)->chinese);
		moveTo(0, 7);
		putEmpty(0, 9, 40);
		printf("ESC:�˳����\t��:��һ������\t��:��һ������");
		ch = getch();
		switch (ch)
		{
		case KeyESC:
			menuSelect();
			break;
		case KeyRight:
			if (index + 1 < wordIndex)
			{
				index++;
				putEmpty(0, 8, 40);
			}
			else
			{
				moveTo(0, 10);
				printf("��ת����һ�����ʡ�");
				index = 0;
			}
			break;
		case KeyLeft:
			if (index + 1 > 1)
			{
				index--;
			}
		default:
			continue;
		}
	}
}
void initInterface()
{
	system("title �뱳���� by ����");
	system("mode con cols=50 lines=25");
	system("cls");
	setColour(red);
	printf("================");
	setColour(green);
	printf(" �� �� �� ��  V1.0 ");
	setColour(red);
	printf("===============\n");
	setColour(white);
}
void testEnglish()
{
	int score;
	float correctRate;
	int testWordIndex = 0;
	int rightSum = 0;
	char answer[255];
	char keyHit;
	setColour(red);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 10);
	printf("---����Ҫ���Եĵ�������ʼ����---");
	setColour(green);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 9);
	printf("�ܵ�����:%d", wordIndex);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 8);
	setColour(white);
	printf("���β��Ե��ʸ���(Ĭ�ϲ���ȫ������):");
	scanf("%d", &testWordIndex);
	if (testWordIndex <= 0 || testWordIndex > wordIndex)
		testWordIndex = wordIndex;
	int *randomArray = generateRandomArray(testWordIndex);
	moveTo(5, 2);
	printf("��д������Ӣ��:");
	moveTo(5, 3);
	putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 10, 40);
	putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 9, 40);
	putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 8, 40);
	for (int i = 0; i < testWordIndex; i++)
	{
		showCursor();
		putEmpty(5, 3, 40);
		putEmpty(40, 5, 5);
		putEmpty(5, 5, 40);
		moveTo(5, 3);
		printf("����:%s", (words + (*(randomArray + i)))->chinese);
		moveTo(5, 5);
		printf("����Ӣ����:");
		scanf("%s", answer);
		char answerWithEnter[255];
		strcat(answer, "\n");
		if (strcmp(answer, (words + (*(randomArray + i)))->english) == 0 || strcmp(answerWithEnter, (words + (*(randomArray + i)))->english) == 0)
		{
			hiddenCursor();
			moveTo(40, 5);
			setColour(bgGreen);
			printf("��ȷ");
			setColour(white);
			rightSum++;
		}
		else
		{
			hiddenCursor();
			moveTo(40, 5);
			setColour(bgRed);
			printf("����");
			setColour(white);
		}
		keyHit = getch();
		switch (keyHit)
		{
		case KeyESC:
			hiddenCursor();
			moveTo(10, 10);
			setColour(bgRed);
			printf("�ٰ�һ��ESC�˳�,�����˳������¼�ɼ�!");
			setColour(white);
			keyHit = getch();
			if (keyHit == KeyESC)
				menuSelect();
			break;
		default:
			break;
		}
	}
	correctRate = (float)rightSum / (float)testWordIndex;
	int highScore = readScore();
	initInterface();
	moveTo(5, 5);
	printf("�������!");
	moveTo(5, 6);
	printf("���δ��%d��,��ȷ����Ϊ:%.2f ", rightSum, correctRate * 100);
	putchar('%');
	if (testWordIndex >= 5)
	{
		moveTo(5, 7);
		setColour(red);
		printf("���β��Լ�¼�ɼ�,");
		if (correctRate > highScore)
		{
			moveTo(5, 8);
			setColour(green);
			printf("��ϲ��������߳ɼ�!");
			writeScore(correctRate);
		}
		else
		{
			moveTo(5, 8);
			setColour(green);
			printf("��߼�¼Ϊ%.2f,�´μ���Ŭ��!", highScore);
		}
	}
	getch();
}
void testChinese()
{
	int score;
	float correctRate;
	int testWordIndex = 0;
	int rightSum = 0;
	char answer[255];
	char keyHit;
	setColour(red);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 10);
	printf("---����Ҫ���Եĵ�������ʼ����---");
	setColour(green);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 9);
	printf("�ܵ�����:%d", wordIndex);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 8);
	setColour(white);
	printf("���β��Ե��ʸ���(Ĭ�ϲ���ȫ������):");
	scanf("%d", &testWordIndex);
	if (testWordIndex <= 0 || testWordIndex > wordIndex)
		testWordIndex = wordIndex;
	int *randomArray = generateRandomArray(testWordIndex);
	moveTo(5, 2);
	printf("��д������Ӣ��:");
	moveTo(5, 3);
	putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 10, 40);
	putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 9, 40);
	putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 8, 40);
	for (int i = 0; i < testWordIndex; i++)
	{
		showCursor();
		putEmpty(5, 3, 40);
		putEmpty(40, 5, 5);
		putEmpty(5, 5, 40);
		moveTo(5, 3);
		printf("Ӣ��:%s", (words + (*(randomArray + i)))->english);
		moveTo(5, 5);
		printf("����������:");
		scanf("%s", answer);
		char answerWithEnter[255];
		strcat(answer, "\n");
		if (strcmp(answer, (words + (*(randomArray + i)))->chinese) == 0 || strcmp(answerWithEnter, (words + (*(randomArray + i)))->chinese) == 0)
		{
			hiddenCursor();
			moveTo(40, 5);
			setColour(bgGreen);
			printf("��ȷ");
			setColour(white);
			rightSum++;
		}
		else
		{
			hiddenCursor();
			moveTo(40, 5);
			setColour(bgRed);
			printf("����");
			setColour(white);
		}
		keyHit = getch();
		switch (keyHit)
		{
		case KeyESC:
			hiddenCursor();
			moveTo(10, 10);
			setColour(bgRed);
			printf("�ٰ�һ��ESC�˳�,�����˳������¼�ɼ�!");
			setColour(white);
			keyHit = getch();
			if (keyHit == KeyESC)
				menuSelect();
			break;
		default:
			break;
		}
	}
	correctRate = rightSum / testWordIndex;
	int highScore = readScore();
	initInterface();
	moveTo(5, 5);
	printf("�������!");
	moveTo(5, 6);
	printf("���δ��%d��,��ȷ����Ϊ:%.2f\%", rightSum, correctRate * 100);
	if (testWordIndex >= 50)
	{
		moveTo(5, 7);
		setColour(red);
		printf("���β��Լ�¼�ɼ�,");
		if (correctRate > highScore)
		{
			moveTo(5, 8);
			setColour(green);
			printf("��ϲ��������߳ɼ�!");
			writeScore(correctRate);
		}
		else
		{
			moveTo(5, 8);
			setColour(green);
			printf("��߼�¼Ϊ%.2f,�´μ���Ŭ��!", highScore);
		}
	}
	getch();
}
int *generateRandomArray(int length)
{ //Use Fisher & Yates to shuffle array
	srand(time(NULL));
	int *orderedArray = (int *)malloc(wordIndex * sizeof(int));
	int *randomArray = (int *)malloc(wordIndex * sizeof(int));
	int *returnedArray = (int *)malloc(length * sizeof(int));
	for (int i = 0; i < wordIndex; i++)
		*(orderedArray + i) = i;
	for (int i = 0; i < wordIndex; i++)
	{
		int random = rand() % (wordIndex - i);
		*(randomArray + i) = *(orderedArray + random);
		for (int j = random; j < (wordIndex - i); j++)
		{
			*(orderedArray + j) = *(orderedArray + j + 1);
		}
	}
	for (int i = 0; i < length; i++)
	{
		*(returnedArray + i) = *(randomArray + i);
	}

	return returnedArray;
}
void putEmpty(int x, int y, int block)
{
	moveTo(x, y);
	for (int i = 0; i < block; i++)
	{
		printf(" ");
	}
	moveTo(x, y);
}
void hiddenCursor()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = 0;
	SetConsoleCursorInfo(hOut, &cci);
}
void showCursor()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = 1;
	SetConsoleCursorInfo(hOut, &cci);
}
void moveTo(int x, int y)
{
	COORD coord = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void setColour(int colour)
{
	switch (colour)
	{
	case white:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;
	case grey:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		break;
	case green:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		break;
	case red:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		break;
	case bgGreen:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 39);
		break;
	case bgRed:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 71);
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;
	}
}
float readScore()
{
	char basePath[255];
	_getcwd(basePath, 255);
	strcat(basePath, "\\Score\\");
	strcat(basePath, fileName);
	strcat(basePath, "sc");
	FILE *fp = NULL;
	fp = fopen(basePath, "r");
	int score = 0;
	if (fp != NULL)
	{
		fscanf(fp, "%d", &score);
		fclose(fp);
	}
	return (float)(score / 100);
}
void writeScore(float score)
{
	char basePath[255];
	_getcwd(basePath, 255);
	strcat(basePath, "\\Score\\");
	strcat(basePath, fileName);
	strcat(basePath, "sc");
	FILE *fp = NULL;
	fp = fopen(basePath, "w");
	double scoreDb;
	modf(score * 100, &scoreDb);
	int scoreInt = (int)scoreDb;

	fwrite(&scoreInt, sizeof(int), 1, fp);
	fclose(fp);
}