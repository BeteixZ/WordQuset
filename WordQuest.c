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
	white,grey,red,green, bgGreen,bgRed
}COLOUR;

struct Dictionary {
	char english[255];
	char chinese[255];
};

int wordIndex = 0;
struct Dictionary* words = NULL;

void initInterface();
void setColour(int colour);
void moveTo(int x,int y);
void hiddenCursor();
void showCursor();
void loadDictionary();
void dictionaryPreview();
void putEmpty(int x,int y,int blocks);
void menuSelect();
void testEnglish();
int* generateRandomArray(int length);
float readScore();
void writeScore(float score);
void testChinese();

int main() {
	creatDataDir();
	initInterface();
	chooseFile();
	wordIndex = getDictionaryIndex();
	words = (struct Dictionary*) malloc(wordIndex * sizeof(struct Dictionary));
	loadDictionary();
	menuSelect();
	return 0;
}
void menuSelect() {
	while (1) {
		initInterface();
		int index;
		moveTo(25, 25);
		printf("1.浏览单词\n2.看中文背英文\n3.看英文背中文\n4.添加单词\n5.删除单词\n6.预览词库\n7.更改词库\n");
		printf("请输入你的选择:");
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
			printf("请输入中文:");
			scanf("%s", chinese);
			moveTo(5, 4);
			printf("请输入英文:");
			scanf("%s", english);
			addDictionary(chinese,english);
			loadDictionary();
			break;
		case 5:
			initInterface();
			moveTo(5, 3);
			printf("请输入英文:");
			scanf("%s", english);
			deleteDictionary(english);
			loadDictionary();
			break;
		case 6:
			initInterface();
			displayDictionary();
			printf("\n按任意键返回");
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
void loadDictionary() {
	int index = 0;
	char buffer[255];
	FILE* fp=NULL;
	fp = fopen(path, "r");
	while (fgets(buffer,255,fp))
	{
		strcpy((words +index)->chinese,strtok(buffer, "#"));
		strcpy((words +index)->english,strtok(NULL, "#"));
		index++;
	}
}
void dictionaryPreview() {
	int index=0;
	char ch;
	if (wordIndex ==0)
	{
		hiddenCursor();
		setColour(red);
		moveTo(4, 1);
		printf("无单词!\n");
		setColour(white);
		printf("按任意键返回菜单!");
		getch();
		menuSelect();
	}
	while (1) {
		hiddenCursor();
		moveTo(5, 3);
		putEmpty(5, 3, 40);
		printf("第%d/%d个单词:", index + 1, wordIndex);
		moveTo(5, 5);
		putEmpty(5, 5, 40);
		printf("单词:%s", (words + index)->english);
		moveTo(5, 6);
		putEmpty(5, 7, 40);
		printf("释义:%s", (words + index)->chinese);
		moveTo(0, 7);
		putEmpty(0, 9, 40);
		printf("ESC:退出浏览\t→:下一个单词\t←:上一个单词");
		ch = getch();
		switch (ch)
		{
		case KeyESC:
			menuSelect();
			break;
		case KeyRight:
			if (index+1<wordIndex)
			{
				index++;
				putEmpty(0, 8, 40);
			}
			else
			{
				moveTo(0, 10);
				printf("已转到第一个单词。");
				index = 0;
			}
			break;
		case KeyLeft:
			if (index+1>1)
			{
				index--;
			}
		default:
			continue;
		}
	}
}
void initInterface() {
	system("title 请背单词 by BKnStar");
	system("mode con cols=50 lines=25");
	system("cls");
	setColour(red);
	printf("================");
	setColour(green);
	printf(" 请 背 单 词  V1.0 ");
	setColour(red);
	printf("===============\n");
	setColour(white);
}
void testEnglish() {
	int score;
	float correctRate;
	int testWordIndex=0;
	int rightSum=0;
	char answer[255];
	char keyHit;
	setColour(red);
	moveTo((InterfaceX / 2)-20,( InterfaceY / 2)-10);
	printf("---输入要测试的单词数开始测试---");
	setColour(green);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 9);
	printf("总单词数:%d", wordIndex);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 8);
	setColour(white);
	printf("本次测试单词个数(默认测试全部单词):");
	scanf("%d", &testWordIndex);
	if (testWordIndex<=0|| testWordIndex > wordIndex)
		testWordIndex = wordIndex;
	int* randomArray = generateRandomArray(testWordIndex);
	moveTo(5, 2);
	printf("请写出它的英文:");
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
		printf("中文:%s", (words+(*(randomArray+i)))->chinese);
		moveTo(5, 5);
		printf("它的英文是:");
		scanf("%s",answer);
		char answerWithEnter[255];
		strcat(answer, "\n");
		if (strcmp(answer, (words + (*(randomArray + i)))->english)==0|| strcmp(answerWithEnter, (words + (*(randomArray + i)))->english) == 0)
		{
			hiddenCursor();
			moveTo(40, 5);
			setColour(bgGreen);
			printf("正确");			
			setColour(white);
			rightSum++;
		}
		else
		{
			hiddenCursor();
			moveTo(40, 5);
			setColour(bgRed);
			printf("错误");
			setColour(white);
		}
		keyHit = getch();
		switch (keyHit)
		{
		case KeyESC:
			hiddenCursor();
			moveTo(10, 10);
			setColour(bgRed);
			printf("再按一次ESC退出,现在退出不会记录成绩!");
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
	printf("测试完成!");
	moveTo(5, 6);
	printf("本次答对%d题,正确概率为:%.2f\%", rightSum, correctRate*100);
	if (testWordIndex>=50)
	{
		moveTo(5, 7);
		setColour(red);
		printf("本次测试记录成绩,");
		if (correctRate>highScore)
		{
			moveTo(5, 8);
			setColour(green);
			printf("恭喜你获得了最高成绩!");
			writeScore(correctRate);
		}
		else
		{
			moveTo(5, 8);
			setColour(green);
			printf("最高纪录为%.2f,下次继续努力!",highScore);
		}
	}
	getch();
}
void testChinese() {
	int score;
	float correctRate;
	int testWordIndex = 0;
	int rightSum = 0;
	char answer[255];
	char keyHit;
	setColour(red);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 10);
	printf("---输入要测试的单词数开始测试---");
	setColour(green);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 9);
	printf("总单词数:%d", wordIndex);
	moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 8);
	setColour(white);
	printf("本次测试单词个数(默认测试全部单词):");
	scanf("%d", &testWordIndex);
	if (testWordIndex <= 0 || testWordIndex > wordIndex)
		testWordIndex = wordIndex;
	int* randomArray = generateRandomArray(testWordIndex);
	moveTo(5, 2);
	printf("请写出它的英文:");
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
		printf("英语:%s", (words + (*(randomArray + i)))->english);
		moveTo(5, 5);
		printf("它的中文是:");
		scanf("%s", answer);
		char answerWithEnter[255];
		strcat(answer, "\n");
		if (strcmp(answer, (words + (*(randomArray + i)))->chinese) == 0 || strcmp(answerWithEnter, (words + (*(randomArray + i)))->chinese) == 0)
		{
			hiddenCursor();
			moveTo(40, 5);
			setColour(bgGreen);
			printf("正确");
			setColour(white);
			rightSum++;
		}
		else
		{
			hiddenCursor();
			moveTo(40, 5);
			setColour(bgRed);
			printf("错误");
			setColour(white);
		}
		keyHit = getch();
		switch (keyHit)
		{
		case KeyESC:
			hiddenCursor();
			moveTo(10, 10);
			setColour(bgRed);
			printf("再按一次ESC退出,现在退出不会记录成绩!");
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
	printf("测试完成!");
	moveTo(5, 6);
	printf("本次答对%d题,正确概率为:%.2f\%", rightSum, correctRate * 100);
	if (testWordIndex >= 50)
	{
		moveTo(5, 7);
		setColour(red);
		printf("本次测试记录成绩,");
		if (correctRate > highScore)
		{
			moveTo(5, 8);
			setColour(green);
			printf("恭喜你获得了最高成绩!");
			writeScore(correctRate);
		}
		else
		{
			moveTo(5, 8);
			setColour(green);
			printf("最高纪录为%.2f,下次继续努力!", highScore);
		}
	}
	getch();
}
int* generateRandomArray(int length) {						//Use Fisher & Yates to shuffle array
	srand(time(NULL));
	int* orderedArray = (int*)malloc(length * sizeof(int));
	int* randomArray = (int*)malloc(length * sizeof(int));
	for (int i = 0; i < length; i++)
		*(orderedArray + i) = i;
	for (int i = 0; i < length; i++)
	{
		int random = rand() % (length-i);
		*(randomArray + i) = *(orderedArray + random);
		for (int j = random; j < (length-i); j++)
		{
			*(orderedArray + j) = *(orderedArray + j+1);
		}
	}
	return randomArray;
}
void putEmpty(int x, int y, int block) {
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
void moveTo(int x, int y) {
	COORD coord = { x,y };
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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),39);
		break;
	case bgRed:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 71);
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		break;
	}
}
float readScore() {
	char basePath[255];
	_getcwd(basePath, 255);
	strcat(basePath, "\\Data\\");
	strcat(basePath, fileName);
	strcat(basePath, "sc");
	FILE* fp = NULL;
	fp = fopen(basePath, "r");
	int score=0;
	if (fp!=NULL)
	{
		fgets(score, sizeof(int), fp);

	}
	return (float)(score/100);
}
void writeScore(float score) {
	char basePath[255];
	_getcwd(basePath, 255);
	strcat(basePath, "\\Data\\");
	strcat(basePath, fileName);
	FILE* fp = NULL;
	fp = fopen(basePath, "w");
	int scoreInt;
	modf(score*100, scoreInt);
	fputs(scoreInt, fp);
}