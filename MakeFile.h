#define _CRT_SUCURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <Windows.h>

char path[255];
char *fileName;

void creatDataDir() {
	char buffer[255];
	_getcwd(buffer, 255);
	strcat(buffer, "\\Data");
	_mkdir(buffer);
}

void creatScoreDir() {
	char buffer[255];
	_getcwd(buffer, 255);
	strcat(buffer, "\\Score");
	_mkdir(buffer);
}


void chooseFile() {
	char rubbish[255];
	FILE* fp =NULL;
	int len = 0;
	int index;
	struct fileIndex {
		int wordNumber;
		char fileName[255];
	} fileIndex[50];
	char buffer[255];
	_getcwd(buffer, 255);
	strcat(buffer, "\\Data\\*.wlb");
	HANDLE hFile;
	LPCTSTR lpFileName = buffer;
	WIN32_FIND_DATA pNextInfo;
	hFile = FindFirstFile(lpFileName, &pNextInfo);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	int wordNum1 = 0;
	char basePath1[255];
	_getcwd(basePath1, 255);
	strcat(basePath1, "\\Data\\");
	strcat(basePath1, pNextInfo.cFileName);
	fp = fopen(basePath1, "r");
	while (fgets(rubbish, 255, fp))
		wordNum1++;
	fileIndex[len].wordNumber = wordNum1;
	strcpy(fileIndex[len++].fileName, pNextInfo.cFileName);
	while (FindNextFile(hFile, &pNextInfo)) {
		int wordNum = 0;
		char basePath[255];
		_getcwd(basePath, 255);
		strcat(basePath, "\\Data\\");
		strcat(basePath, pNextInfo.cFileName);
		if (pNextInfo.cFileName[0] == '.')
			continue;
		fp = fopen(basePath, "r");
		while (fgets(rubbish, 255, fp))
			wordNum++;
		fileIndex[len].wordNumber = wordNum;
		strcpy(fileIndex[len++].fileName, pNextInfo.cFileName);
	}
	printf("现有词库(Data文件夹内):\n");
	for (int i = 0; i < len; i++)
		printf("#%d\t%s\t(%d词)\n", i + 1, fileIndex[i].fileName,fileIndex[i].wordNumber);
	
	do {
		printf("请输入要载入的词库:");
		scanf("%d", &index);
		if (index <= len)
		{
			fileName = fileIndex[index - 1].fileName;
			char basePath[255];
			_getcwd(basePath, 255);
			strcat(basePath, "\\Data\\");
			strcat(basePath, fileIndex[index - 1].fileName);
			strcpy(path, basePath);
		}
	} while (strlen(path)==0);
	fclose(fp);
}

void addDictionary(char *key, char* value) {
	if (value!="" && key!="")
	{
		int exist=0;
		char buffer[255];
		FILE* fp = NULL;
		fp = fopen(path, "a+");
		while (fgets(buffer,255,fp))
		{
			char* fileKey = strtok(buffer, "#");
			if (strcmp(fileKey,key)==0)
			{
				exist = 1;
			}
		}
		if(!exist){
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

void deleteDictionary(char* key) {
	if (key!="")
	{
		char tempPath[255];
		_getcwd(tempPath, 255);
		strcat(tempPath, "\\Data\\temp.wlb");
		FILE* fp = NULL;
		FILE* ft = NULL;
		fp = fopen(path, "r+");
		ft = fopen(tempPath, "w+");
		if (fp != NULL)
		{
			char buffer[255];
			while (fgets(buffer,255, fp))
			{
				char tempBuffer[255];
				strcpy(tempBuffer, buffer);
				char* fileKey = strtok(tempBuffer, "#");
				if (strcmp(fileKey,key))
				{
					fputs(buffer, ft);
				}
			}
			fclose(fp);
			fclose(ft);
			remove(path);
			rename(tempPath, path);
			remove(tempPath);
		}
	}
}

void displayDictionary() {
	FILE* fp=NULL;
	fp = fopen(path, "r");
	if (fp!=NULL)
	{
		char buffer[255];
		char *key;
		char *value;
		while(fgets(buffer, 255, fp)) {
			key = strtok(buffer, "#");
			value = strtok(NULL, "#");
			printf("单词:%s\t释义:%s", key, value);
		}
	}
	fclose(fp);
}


int getDictionaryIndex() {
	FILE* fp=NULL;
	int index=0;
	char rubbish[255];
	fp = fopen(path, "r");
	if (fp!=NULL)
	{
		while (fgets(rubbish, 255, fp))
			index++;
	}

	return index;
}