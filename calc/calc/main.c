#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc.h"

//构造大数模板，len指向大数的数据位数，sign表示该大数的符号
bignumber_s *make_bignumber_temp(int len, int sign)
{
    bignumber_s *temp = malloc(sizeof(bignumber_s)+len);
    if (NULL == temp) {
        perror("Malloc");
        exit(-1);
    }

    temp->sign = sign;
    temp->len = len;
    memset(temp->data, 0, len);
    return temp;
}

//处理数字字符串冗余的0, 即　00123 -> 123
const char *strip_str(const char *str)
{
    int i = 0;
    int len = strlen(str);
    for (i=0; i<len-1&&str[i]=='0'; i++);
    return str+i;
}

//将字符串数据填充到模板中
void fill_data_fromstr(bignumber_s *n, const char *str)
{
    int i=0;
    int len = n->len;
    for (i=0; i<len; i++) {
        int d = str[len-i-1]-'0';
        if (d>=0 && d<=9) {
            n->data[i] = d;
        } else {
            fprintf(stderr, "Invalid Number:%s\n", str);
            exit(-1);
        }
    }
}

//从字符串构造一个大数
bignumber_s *make_bignumber_fromstr(const char *str)
{
    int sign = 0;
    if (str[0] == '-') {
        sign = 1;
        str++;
    }

    //处理数字字符串冗余的0, 即　00123 -> 123
    const char *striped_str = strip_str(str);

    int len = strlen(striped_str);
    //指定数据位长度即符号，创建一个大数模板
    bignumber_s *temp = make_bignumber_temp(len, sign);
    //将字符串数据填充进模板中，完成大数创建
    fill_data_fromstr(temp, striped_str);
    return temp;
}

//以字符串的形式打印输出一个大数
void print_bignumber(bignumber_s *b) {
    int len = b->len;
    char *str = malloc(len+1);
    int i=0;
    for (i=0; i<len;i++) {
        str[i] = b->data[len-i-1]+'0';
    }
    str[len] = '\0';

    fprintf(stdout, "%s%s\n", b->sign==1?"-":"", strip_str(str));
    free(str);
}

void usage(const char *s)
{
    fprintf(stderr, "Usage:%s number1 op( + - * /) number2.\n", s);
    exit(-1);
}

void add_impl(bignumber_s *a, bignumber_s *b, bignumber_s *r)
{
    int i=0;
    char carry = 0;
    int len = r->len;
    for (i=0; i<len; i++) {
        if (i<a->len) carry += a->data[i];
        if (i<b->len) carry += b->data[i];
        r->data[i] = carry%BASE;
        carry /= BASE;
    }
}

bignumber_s *calc_add(bignumber_s *a, bignumber_s *b)
{
    //n位数+m位数，其和最大为max(n,m)+1位数
    int len = MAX(a->len, b->len) +1;
    bignumber_s *result = make_bignumber_temp(len, a->sign);

    add_impl(a,b,result);
    return result;
}

//bignumber_s * calc_add(bignumber_s *a, bignumber_s *b){}
bignumber_s * calc_sub(bignumber_s *a, bignumber_s *b){}
bignumber_s * calc_mul(bignumber_s *a, bignumber_s *b){}
bignumber_s * calc_div(bignumber_s *a, bignumber_s *b){}

int main(int argc, char *argv[])
{
    if (argc!=4) {
        usage(argv[0]);
    }

    bignumber_s *a = make_bignumber_fromstr(argv[1]);
    bignumber_s *b = make_bignumber_fromstr(argv[3]);

    if (0 == strcmp(argv[2], "+")) {
        print_bignumber(calc_add(a,b));
    } else if (0 == strcmp(argv[2], "-")) {
        print_bignumber(calc_sub(a,b));
    } else if (0 == strcmp(argv[2], "x")) {
        print_bignumber(calc_mul(a,b));
    } else if(0 == strcmp(argv[2], "/")) {
        print_bignumber(calc_div(a,b));
    } else {
        usage(argv[0]);
    }

    return 0;
}
