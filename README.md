    int putchar(int c); 向标准输出流输出一个字符
    int getchar(void);　从标准输入流读入一个字符
    int putc(int c, FILE *fp);  向任意流fp写一个字符
    int fputc(intc, FILE *fp);　向任意流fp写一个字符
    int getc(FILE *fp);  从任意流读入一个字符
    int fgetc(FILE *fp);　从任意流读入一个字符　
    fputc(int c, stdout) 相当于　putchar(int c)
    getc(stdin) 相当于　getchar();

    #define putchar(c) putc(c,stdout)
    #define getchar(c) getc(c, stdout)
        上面这几个都是通过宏来实现的，　一个是宏速度快，　但是宏是不能给参数设置类型的，　所以就有fputc（） fgetc（）　这种函数的处理方法可供选择



    int puts(const char *s);  将字符串s写人标准输出stdout，并在其后添加一个换行符
    int fputs(const char *s, FILE *fp);  将字符串s写人fp所指向的文件中
    与puts 相同的是：　若出现写入错误，　则返回EOF, 否则返回一个非负数
    与puts不同的是：　它不会自动添加换行符，除非字符串本身含有换行符

    char *gets(char *s);  从标准输入流stdin读取字符串存入到＊ｓ指定的缓存区，　然后返回首地址，　读到换行符时停止
        容易发生缓存区溢出
    char *fgets(char *s, int n, FILE *fp); 从fp所指定的文件中读取字符串，最多读取n-1个字符
    用fgets(buf, sizeof(buf), stdin) 代替　gets(buf) 更安全
    gets 和　fgets 相同点：
        都会在字符串末尾添加'\0'
        读取失败或者到达文件尾部都返回NULL
    gets 和　fgets 不同点：
        对换行符的处理不同
                gets 不保留'\n'替换为'\0'
                fgets 保留并存储'\n'

