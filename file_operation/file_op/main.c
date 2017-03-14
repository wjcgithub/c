#include <stdio.h>
#include <stdlib.h>
FILE *stream;

//测试fscanf函数的使用方法
void testFscanf()
{
    long l;
    float fp;
    char s[10];
    char c;
    stream = fopen("fscanf.out", "w+");
    if (stream == NULL)
    {
        printf("the file fscanf.out was not opened\n");
        exit(-1);
    } else {
        fprintf(stream, "%s\t%ld\t%f\t%c", "a-string", 65000, 3.1415, 'x');
        fseek(stream, 0L, SEEK_SET);

        fscanf(stream, "%s\t", s);
        fscanf(stream, "%ld\t",&l);
        fscanf(stream, "%f\t",&fp);
        fscanf(stream, "%c\t",&c);

        printf("%s\n%ld\n%f\n%c", s, l, fp, c);
        fclose(stream);
    }
}

void test_putc_fputc()
{
//    int putchar(int c); 向标准输出流输出一个字符
//    int getchar(void);　从标准输入流读入一个字符
//    int putc(int c, FILE *fp);  向任意流fp写一个字符
//    int fputc(intc, FILE *fp);　向任意流fp写一个字符
//    int getc(FILE *fp);  从任意流读入一个字符
//    int fgetc(FILE *fp);　从任意流读入一个字符　
//    fputc(int c, stdout) 相当于　putchar(int c)
//    getc(stdin) 相当于　getchar();
//
//    #define putchar(c) putc(c,stdout)
//    #define getchar(c) getc(c, stdout)
//        上面这几个都是通过宏来实现的，　一个是宏速度快，　但是宏是不能给参数设置类型的，　所以就有fputc（） fgetc（）　这种函数的处理方法可供选择

}


void test_write_char()
{
    FILE *fp;
    int ch;
    if ((fp = fopen("demo.txt", "w")) == NULL)
    {
        printf("\t Failure to open demo.txt!\n");
        exit(0);
    }
    printf("\n");
    ch = getchar();
    while (ch != '\n')
    {
        fputc(ch, fp);
        ch = getchar();
    }

    fclose(fp);
}


void test_read_char()
{
    FILE *fp;
    int ch;
    long pos;
    if ((fp = fopen("demo.txt", "r")) == NULL)
    {
        printf("\t Failure to open demo.txt!\n");
        exit(0);
    }

    printf("\n");
    pos = ftell(fp);
    ch = fgetc(fp);
    while (!feof(fp))
    {
    //        putchar(ch);
        printf("%c, %ld\n", ch, pos);
        pos = ftell(fp);
        ch = fgetc(fp);
    }

    fclose(fp);
}

void test_fgets()
{
//    int puts(const char *s);  将字符串s写人标准输出stdout，并在其后添加一个换行符
//    int fputs(const char *s, FILE *fp);  将字符串s写人fp所指向的文件中
//    与puts 相同的是：　若出现写入错误，　则返回EOF, 否则返回一个非负数
//    与puts不同的是：　它不会自动添加换行符，除非字符串本身含有换行符
//
//    char *gets(char *s);  从标准输入流stdin读取字符串存入到＊ｓ指定的缓存区，　然后返回首地址，　读到换行符时停止
//        容易发生缓存区溢出
//    char *fgets(char *s, int n, FILE *fp); 从fp所指定的文件中读取字符串，最多读取n-1个字符
//    用fgets(buf, sizeof(buf), stdin) 代替　gets(buf) 更安全
//    gets 和　fgets 相同点：
//        都会在字符串末尾添加'\0'
//        读取失败或者到达文件尾部都返回NULL
//    gets 和　fgets 不同点：
//        对换行符的处理不同
//                gets 不保留'\n'替换为'\0'
//                fgets 保留并存储'\n'
    FILE *fp;
    char str[80];
    if ((fp = fopen("fgets.txt", "w")) == NULL)
    {
        printf("\t Failure to open demo.txt!\n");
        exit(0);
    }
    printf("\n");
    fgets(str, sizeof(str), stdin);
    fputs(str, fp);
    fclose(fp);
}

void test_fputs()
{
    FILE *fp;
    char str[80];
    if ((fp = fopen("fgets.txt", "r")) == NULL)
    {
        printf("\t Failure to open demo.txt!\n");
        exit(0);
    }
    printf("\n");
    fgets(str, sizeof(str), fp);
    fputs(str, stdout);
    fclose(fp);
}

void test_fread()
{
//    size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
//    从fp所指文件中读取数据块到buffer指向的内存
//    buffer是存储数据块的内存首地址，如数组的地址
//    size是每个数据块大小（元素的大小，字节数）
//    count 是要读入的数据块个数，如数组元素的个数
//    返回值num: 实际读入的数据块个数
//        应等于count, 除非达到了文件末尾，或出现了错误

//    size_t fwrite(const void *buffer, size_t size, size_t count, FILE *stream);
//    将buffer指向的内存中的数据块写入fp所指向的文件
//    buffer是数据块的首地址，　如数组的地址
//    size是每个数据块的大小，（元素的大小，　字节数）
//    count 是要写入的数据块个数，如数组元素的个数
//    返回值num: 实际写入的数据块的个数
//        应等于count，除非出现写入错误
    FILE *stream;
    char list[30];
    int i, numread, numwrite;
    if ((stream = fopen("fread.out", "w+")) != NULL)
    {
        for (i=0; i < 26; i++) {
            list[i] = (char)('z'-i);
        }
        numwrite = fwrite(list, sizeof(char), 26, stream);
        printf("Wrote %d items\n", numwrite);

        fclose(stream);
    } else {
        printf("Problem openting the file\n");
    }

    if ( (stream = fopen("fread.out", "r+")) != NULL ) {
        numread = fread(list, sizeof(char), 26, stream);
        printf("Number of items read = %d\n", numread);
        printf("content is %s\n", list);
        fclose(stream);
    }
}

void test_random_read_file()
{
//    文件的随机读写是通过文件内部的指针完成的，文件从头到尾按照字节从０开始顺序编址，
//    用以表示数据的存储位置

//    使文件位置指针从新指向文件的开始位置
//    void rewind(FILE *fp);
//    返回当前文件位置指针（相对于文件起始位置的自己偏移量）
//    long gtell(FILE *fp);
//    改变文件位置指针，实现随机读写
//    int fseek(FILE *fp, long offset, int fromwhere);
//        将文件位置指针从fromwhere开始移动offset个字节，指示下一个要读取的数据的位置
//        fp:       指向fopen()返回的句柄
//        offset: 　位置指针的偏移量（字节数），long型，加l或者L
//        fromwhere: 起始位置
//            SEEK_SET或０　－－－　文件开始位置
//            SEEK_CUR或1 　－－－　文件当前位置
//            SEEK_END或2 　－－－　文件末尾
}

int main()
{
//   按字符读取文件
//    testFscanf();
//    test_putc_fputc();
//    test_write_char();
//    test_read_char();
//    test_puts_fputs();

// 按行读取文件
//    test_fgets();
//    test_fputs();

// 按数据块读写文件
    test_fread();

// 文件的随机读写
    test_random_read_file();

    return 0;
}
