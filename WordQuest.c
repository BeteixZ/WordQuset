//V1.0
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "MakeFile.h"

#define InterfaceX 50
#define InterfaceY 50

#define KeyESC 27
#define KeyLeft 'h'  // 使用vim风格的键位
#define KeyRight 'l'

// ANSI转义序列用于颜色设置
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_GREY    "\x1b[90m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_BG_GREEN     "\x1b[42m"
#define ANSI_BG_RED       "\x1b[41m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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

// Linux系统下获取单个字符输入
int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

// 清屏函数
void clearScreen() {
    printf("\033[2J\033[H");
}

// 移动光标
void moveTo(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

// 隐藏光标
void hiddenCursor() {
    printf("\033[?25l");
}

// 显示光标
void showCursor() {
    printf("\033[?25h");
}

// 设置颜色
void setColour(int colour) {
    switch (colour) {
        case white:
            printf(ANSI_COLOR_WHITE);
            break;
        case grey:
            printf(ANSI_COLOR_GREY);
            break;
        case green:
            printf(ANSI_COLOR_GREEN);
            break;
        case red:
            printf(ANSI_COLOR_RED);
            break;
        case bgGreen:
            printf(ANSI_BG_GREEN);
            break;
        case bgRed:
            printf(ANSI_BG_RED);
            break;
        default:
            printf(ANSI_COLOR_RESET);
            break;
    }
}

void putEmpty(int x, int y, int block) {
    moveTo(x, y);
    for (int i = 0; i < block; i++) {
        printf(" ");
    }
    moveTo(x, y);
}

// 初始化界面
void initInterface() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    clearScreen();
    setColour(red);
    printf("================");
    setColour(green);
    printf(" 单词背记 V1.0 ");
    setColour(red);
    printf("===============\n");
    setColour(white);
}

void loadDictionary() {
    int index = 0;
    char buffer[255];
    FILE *fp = NULL;
    fp = fopen(path, "r");
    if (fp == NULL) return;
    while (fgets(buffer, 255, fp)) {
        strcpy((words + index)->chinese, strtok(buffer, "#"));
        char *eng = strtok(NULL, "#");
        if (eng != NULL) {
            // 移除可能的换行符
            eng[strcspn(eng, "\n")] = 0;
            strcpy((words + index)->english, eng);
        }
        index++;
    }
    fclose(fp);
}

float readScore() {
    char basePath[255];
    getcwd(basePath, 255);
    strcat(basePath, "/Score/");
    strcat(basePath, fileName);
    strcat(basePath, "sc");
    FILE *fp = NULL;
    fp = fopen(basePath, "r");
    int score = 0;
    if (fp != NULL) {
        fscanf(fp, "%d", &score);
        fclose(fp);
    }
    return (float)(score / 100.0);
}

void writeScore(float score) {
    char basePath[255];
    getcwd(basePath, 255);
    strcat(basePath, "/Score/");
    strcat(basePath, fileName);
    strcat(basePath, "sc");
    FILE *fp = NULL;
    fp = fopen(basePath, "w");
    if (fp != NULL) {
        double scoreDb;
        modf(score * 100, &scoreDb);
        int scoreInt = (int)scoreDb;
        fprintf(fp, "%d", scoreInt);
        fclose(fp);
    }
}

int *generateRandomArray(const int length) {
    srand(time(NULL));
    int *orderedArray = malloc(wordIndex * sizeof(int));
    int *randomArray = malloc(wordIndex * sizeof(int));
    int *returnedArray = malloc(length * sizeof(int));

    for (int i = 0; i < wordIndex; i++)
        orderedArray[i] = i;

    for (int i = 0; i < wordIndex; i++) {
        const int random = rand() % (wordIndex - i);
        randomArray[i] = orderedArray[random];

        for (int j = random; j < (wordIndex - i - 1); j++) {
            orderedArray[j] = orderedArray[j + 1];
        }
    }

    for (int i = 0; i < length; i++) {
        returnedArray[i] = randomArray[i];
    }

    free(orderedArray);
    free(randomArray);
    return returnedArray;
}

void dictionaryPreview() {
    int index = 0;
    if (wordIndex == 0) {
        hiddenCursor();
        setColour(red);
        moveTo(4, 1);
        printf("无单词!\n");
        setColour(white);
        printf("按任意键返回菜单!");
        getch();
        return;
    }

    while (1) {
        hiddenCursor();
        moveTo(5, 3);
        putEmpty(5, 3, 40);
        printf("第%d/%d个单词:", index + 1, wordIndex);
        moveTo(5, 5);
        putEmpty(5, 5, 40);
        printf("英文:%s", (words + index)->english);
        moveTo(5, 6);
        putEmpty(5, 7, 40);
        printf("中文:%s", (words + index)->chinese);
        moveTo(0, 7);
        putEmpty(0, 9, 40);
        printf("ESC:退出预览\th:上一个单词\tl:下一个单词");

        const char ch = getch();
        switch (ch) {
            case KeyESC:
                return;
            case KeyRight:
                if (index + 1 < wordIndex) {
                    index++;
                    putEmpty(0, 8, 40);
                } else {
                    moveTo(0, 10);
                    printf("已翻到最后一个单词。");
                    index = 0;
                }
                break;
            case KeyLeft:
                if (index > 0) {
                    index--;
                }
                break;
        }
    }
}

// 英文测试功能
void testEnglish() {
    float correctRate;
    int testWordIndex = 0;
    int rightSum = 0;
    char answer[255];
    char keyHit;

    setColour(red);
    moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 10);
    printf("---请输入要测试的单词数开始测试---");
    setColour(green);
    moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 9);
    printf("总单词数:%d", wordIndex);
    moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 8);
    setColour(white);
    printf("本次测试的单词个数(默认测试全部单词):");
    scanf("%d", &testWordIndex);

    if (testWordIndex <= 0 || testWordIndex > wordIndex)
        testWordIndex = wordIndex;

    int *randomArray = generateRandomArray(testWordIndex);
    moveTo(5, 2);
    printf("请写出中文的英文:");
    moveTo(5, 3);
    putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 10, 40);
    putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 9, 40);
    putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 8, 40);

    for (int i = 0; i < testWordIndex; i++) {
        showCursor();
        putEmpty(5, 3, 40);
        putEmpty(40, 5, 5);
        putEmpty(5, 5, 40);
        moveTo(5, 3);
        printf("中文:%s", (words + (*(randomArray + i)))->chinese);
        moveTo(5, 5);
        printf("输入英文答案:");
        scanf("%s", answer);

        // 处理比较答案
        char answerWithEnter[255];
        strcpy(answerWithEnter, answer);
        strcat(answerWithEnter, "\n");

        // 去除可能的换行符
        char correctAnswer[255];
        strcpy(correctAnswer, (words + (*(randomArray + i)))->english);
        correctAnswer[strcspn(correctAnswer, "\n")] = 0;
        answer[strcspn(answer, "\n")] = 0;

        if (strcmp(answer, correctAnswer) == 0) {
            hiddenCursor();
            moveTo(40, 5);
            setColour(bgGreen);
            printf("正确");
            setColour(white);
            rightSum++;
        } else {
            hiddenCursor();
            moveTo(40, 5);
            setColour(bgRed);
            printf("错误");
            setColour(white);
            // 显示正确答案
            moveTo(5, 6);
            printf("正确答案: %s", correctAnswer);
        }

        keyHit = getch();
        if (keyHit == KeyESC) {
            hiddenCursor();
            moveTo(10, 10);
            setColour(bgRed);
            printf("再按一次ESC退出,否则继续记录成绩!");
            setColour(white);
            keyHit = getch();
            if (keyHit == KeyESC) {
                free(randomArray);
                return;
            }
        }
    }

    correctRate = (float)rightSum / (float)testWordIndex;
    float highScore = readScore();
    initInterface();
    moveTo(5, 5);
    printf("测试完成!");
    moveTo(5, 6);
    printf("本次对了%d个,正确率为:%.2f%%", rightSum, correctRate * 100);

    if (testWordIndex >= 5) {
        moveTo(5, 7);
        setColour(red);
        printf("本次测试记录成绩,");
        if (correctRate > highScore) {
            moveTo(5, 8);
            setColour(green);
            printf("恭喜你创造最高成绩!");
            writeScore(correctRate);
        } else {
            moveTo(5, 8);
            setColour(green);
            printf("最高记录为%.2f%%,下次继续努力!", highScore * 100);
        }
    }

    free(randomArray);
    setColour(white);
    getch();
}

// 中文测试功能
void testChinese() {
    float correctRate;
    int testWordIndex = 0;
    int rightSum = 0;
    char answer[255];
    char keyHit;

    setColour(red);
    moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 10);
    printf("---请输入要测试的单词数开始测试---");
    setColour(green);
    moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 9);
    printf("总单词数:%d", wordIndex);
    moveTo((InterfaceX / 2) - 20, (InterfaceY / 2) - 8);
    setColour(white);
    printf("本次测试的单词个数(默认测试全部单词):");
    scanf("%d", &testWordIndex);

    if (testWordIndex <= 0 || testWordIndex > wordIndex)
        testWordIndex = wordIndex;

    int *randomArray = generateRandomArray(testWordIndex);
    moveTo(5, 2);
    printf("请写出英文的中文:");
    moveTo(5, 3);
    putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 10, 40);
    putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 9, 40);
    putEmpty((InterfaceX / 2) - 20, (InterfaceY / 2) - 8, 40);

    for (int i = 0; i < testWordIndex; i++) {
        showCursor();
        putEmpty(5, 3, 40);
        putEmpty(40, 5, 5);
        putEmpty(5, 5, 40);
        moveTo(5, 3);
        printf("英文:%s", (words + (*(randomArray + i)))->english);
        moveTo(5, 5);
        printf("输入中文答案:");
        scanf("%s", answer);

        // 处理比较答案
        char answerWithEnter[255];
        strcpy(answerWithEnter, answer);
        strcat(answerWithEnter, "\n");

        // 去除可能的换行符
        char correctAnswer[255];
        strcpy(correctAnswer, (words + (*(randomArray + i)))->chinese);
        correctAnswer[strcspn(correctAnswer, "\n")] = 0;
        answer[strcspn(answer, "\n")] = 0;

        if (strcmp(answer, correctAnswer) == 0) {
            hiddenCursor();
            moveTo(40, 5);
            setColour(bgGreen);
            printf("正确");
            setColour(white);
            rightSum++;
        } else {
            hiddenCursor();
            moveTo(40, 5);
            setColour(bgRed);
            printf("错误");
            setColour(white);
            // 显示正确答案
            moveTo(5, 6);
            printf("正确答案: %s", correctAnswer);
        }

        keyHit = getch();
        if (keyHit == KeyESC) {
            hiddenCursor();
            moveTo(10, 10);
            setColour(bgRed);
            printf("再按一次ESC退出,否则继续记录成绩!");
            setColour(white);
            keyHit = getch();
            if (keyHit == KeyESC) {
                free(randomArray);
                return;
            }
        }
    }

    correctRate = (float)rightSum / (float)testWordIndex;
    float highScore = readScore();
    initInterface();
    moveTo(5, 5);
    printf("测试完成!");
    moveTo(5, 6);
    printf("本次对了%d个,正确率为:%.2f%%", rightSum, correctRate * 100);

    if (testWordIndex >= 5) {
        moveTo(5, 7);
        setColour(red);
        printf("本次测试记录成绩,");
        if (correctRate > highScore) {
            moveTo(5, 8);
            setColour(green);
            printf("恭喜你创造最高成绩!");
            writeScore(correctRate);
        } else {
            moveTo(5, 8);
            setColour(green);
            printf("最高记录为%.2f%%,下次继续努力!", highScore * 100);
        }
    }

    free(randomArray);
    setColour(white);
    getch();
}

// 主要修改为使用Linux下的终端控制代码

void menuSelect() {
    while (1) {
        initInterface();
        int index;
        moveTo(25, 25);
        printf("1.预览单词\n2.英文测试\n3.中文测试\n4.添加单词\n5.删除单词\n6.预览词库\n7.更换词库\n");
        printf("请输入你的选择:");
        scanf("%d", &index);

        switch (index) {
            case 1:
                initInterface();
                dictionaryPreview();
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
                printf("输入中文:");
                scanf("%s", chinese);
                moveTo(5, 4);
                printf("输入英文:");
                scanf("%s", english);
                addDictionary(chinese, english);
                loadDictionary();
                break;
            case 5:
                initInterface();
                moveTo(5, 3);
                printf("输入英文:");
                scanf("%s", english);
                deleteDictionary(english);
                loadDictionary();
                break;
            case 6:
                initInterface();
                displayDictionary();
                printf("\n按任意键继续");
                getchar();
                getchar();
                break;
            case 7:
                initInterface();
                chooseFile();
                break;
        }
    }
}

int main() {
    // 设置终端为非规范模式
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    createDataDir();
    createScoreDir();
    initInterface();
    chooseFile();
    wordIndex = getDictionaryIndex();

    if (wordIndex == 0) {
        moveTo(5, 3);
        setColour(bgRed);
        printf("找不到词库或无单词，按任意键退出！");
        getch();
        // 恢复终端设置
        term.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        return 0;
    }

    words = (struct Dictionary *)malloc(wordIndex * sizeof(struct Dictionary));
    loadDictionary();
    menuSelect();

    // 恢复终端设置
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    free(words);
    return 0;
}