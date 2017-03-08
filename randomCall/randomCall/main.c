#include <stdio.h>
#include <stdlib.h>
#define MAX 100  //最大学生数
#define CMAX 4   //需要点名的次数

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
    OutputInfo(stuName, stuID, stuScore);

    return 0;
}
