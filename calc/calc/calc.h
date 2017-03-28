#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED

#define BASE (10)
#define MAX(x,y) ((x) > (y)?(x):(y))

typedef struct bignumber_s{
    //代表大数的符号，　1为负数，0为正数
    char sign;
    //代表大数的位数
    int len;
    //大数的数据内容，data[0]个位，　data[1]十位，　data[2]百位
    char data[];
} bignumber_s;

//构造大数模板，len指向大数的数据位数，sign表示该大数的符号
bignumber_s *make_bignumber_temp(int len, int sign);
//从字符串构造一个大数
bignumber_s *make_bignumber_fromstr(const char *str);
//以字符串的形式打印输出一个大数
void print_bignumber(bignumber_s *b);

#endif // CALC_H_INCLUDED
