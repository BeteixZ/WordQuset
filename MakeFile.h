//V1.0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

char path[255];
char *fileName;

void createDataDir()
{
    char buffer[255];
    getcwd(buffer, 255);
    strcat(buffer, "/Data");
    mkdir(buffer, 0755);  // 0755 gives read/write/execute permissions to owner and read/execute to others
}

void createScoreDir()
{
    char buffer[255];
    getcwd(buffer, 255);
    strcat(buffer, "/Score");
    mkdir(buffer, 0755);
}

void chooseFile()
{
    FILE *fp = NULL;
    int len = 0;
    int index;
    struct fileIndex
    {
        int wordNumber;
        char fileName[255];
    } fileIndex[50];

    char buffer[255];
    getcwd(buffer, 255);
    strcat(buffer, "/Data");

    struct dirent *ent;
    DIR *dir = opendir(buffer);
    if (dir == NULL)
        return;

    // Read directory contents
    while ((ent = readdir(dir)) != NULL)
    {
        // Skip . and .. directories
        if (ent->d_name[0] == '.')
            continue;

        // Check if file ends with .wlb
        const char *ext = strrchr(ent->d_name, '.');
        if (ext == NULL || strcmp(ext, ".wlb") != 0)
            continue;

        char filePath[255];
        snprintf(filePath, sizeof(filePath), "%s/%s", buffer, ent->d_name);

        // Count words in file
        fp = fopen(filePath, "r");
        if (fp != NULL)
        {
            int wordNum = 0;
            char rubbish[255];
            while (fgets(rubbish, 255, fp))
                wordNum++;
            fclose(fp);

            fileIndex[len].wordNumber = wordNum;
            strcpy(fileIndex[len].fileName, ent->d_name);
            len++;
        }
    }
    closedir(dir);

    printf("词库列表 (Data目录):\n");
    for (int i = 0; i < len; i++)
        printf("#%d\t%s\t(%d条)\n", i + 1, fileIndex[i].fileName, fileIndex[i].wordNumber);

    do
    {
        printf("请输入要打开的词库:");
        scanf("%d", &index);
        if (index <= len && index > 0)
        {
            fileName = fileIndex[index - 1].fileName;
            char basePath[255];
            getcwd(basePath, 255);
            strcat(basePath, "/Data/");
            strcat(basePath, fileIndex[index - 1].fileName);
            strcpy(path, basePath);
        }
    } while (strlen(path) == 0);
}

void addDictionary(const char *key, const char *value)
{
    if (strlen(value) > 0 && strlen(key) > 0)
    {
        FILE *fp = NULL;
        fp = fopen(path, "a+");
        if (fp != NULL)
        {
            char buffer[255];
            int exist = 0;
            while (fgets(buffer, 255, fp))
            {
                char *fileKey = strtok(buffer, "#");
                if (strcmp(fileKey, key) == 0)
                {
                    exist = 1;
                    break;
                }
            }
            if (!exist)
            {
                char dic[255];
                strcpy(dic, key);
                strcat(dic, "#");
                strcat(dic, value);
                strcat(dic, "\n");
                fputs(dic, fp);
            }
            fclose(fp);
        }
    }
}

void deleteDictionary(const char *key)
{
    if (strlen(key) > 0)
    {
        char tempPath[255];
        getcwd(tempPath, 255);
        strcat(tempPath, "/Data/temp.wlb");
        FILE *fp = NULL;
        FILE *ft = NULL;
        fp = fopen(path, "r");
        ft = fopen(tempPath, "w");
        if (fp != NULL && ft != NULL)
        {
            char buffer[255];
            while (fgets(buffer, 255, fp))
            {
                char tempBuffer[255];
                strcpy(tempBuffer, buffer);
                char *fileKey = strtok(tempBuffer, "#");
                if (strcmp(fileKey, key) != 0)
                {
                    fputs(buffer, ft);
                }
            }
            fclose(fp);
            fclose(ft);
            remove(path);
            rename(tempPath, path);
        }
    }
}

void displayDictionary()
{
    FILE *fp = NULL;
    fp = fopen(path, "r");
    if (fp != NULL)
    {
        char buffer[255];
        char *key;
        char *value;
        while (fgets(buffer, 255, fp))
        {
            key = strtok(buffer, "#");
            value = strtok(NULL, "#");
            printf("单词:%s\t解释:%s", key, value);
        }
        fclose(fp);
    }
}

int getDictionaryIndex()
{
    FILE *fp = NULL;
    int index = 0;
    fp = fopen(path, "r");
    if (fp != NULL)
    {
        char rubbish[255];
        while (fgets(rubbish, 255, fp))
            index++;
        fclose(fp);
    }
    return index;
}