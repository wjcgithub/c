#include <stdio.h>
#include <stdlib.h>
#define MAX 100  //最大学生数
#define CMAX 4   //需要点名的次数

void RandomCall(char stuName[][20], char stuID[][10], int stuAbsentNum[]);
void SumScore(char stuName[][20], char stuID[][10], int stuAbsentNum[]);
void OutputInfo(char stuName[][20], char stuID[][10], int stuScore[]);

int InputInfo(char stuName[][20], char stuID[][10])
{
    int stuNum, i;
    printf("Please input the number of students. \n");
    scanf("%d", &stuNum);
    getchar();
    printf("Please input the information of students.\n");
    for (i=0; i<stuNum; i++)
    {
        scanf("%s",stuID[i]);
        scanf("%s",stuName[i]);
    }

    return stuNum;
}

int main()
{
    char stuName[MAX][20];
    char stuID[MAX][10];
    int stuScore[MAX];  //出勤分数
    int stuAbsentNum[MAX] = {0}; // 学生的缺勤次数
    int stuNum, courseHours, i;
    stuNum = InputInfo(stuName, stuID);
    printf("Please input the number of lessons. \n");
    scanf("%d", &courseHours);
    for (i=0; i<courseHours; i++)
    {
        // 点名
        RandomCall(stuName, stuID, stuAbsentNum);
    }
    //统计缺勤次数
    SumScore(stuName, stuID, stuAbsentNum);
    //输出缺勤信息
    OutputInfo(stuName, stuID, stuScore);

    return 0;
}
