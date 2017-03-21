#include <stdio.h>
#include <stdlib.h>

int main()
{
    //节点的定义
//    struct node
//    {
//        DATA data;                //数据
//        struct node *next;　//下一个指针
//    }

//    链表：
//        一系列节点
//        每个节点至少包括
//            一条数据（任意类型）
//            一个该节点类型的指针
//        头指针：指向第一个节点的指针
//        尾节点：节点指针指向空（NULL）

    typedef struct player
    {
        int account;
        char name[20];
        int HP;
        int power;
        int intelligence;
        struct player *next;
    } PLAYER;

    PLAYER pl1 = {1,"a",1,1,1};
    PLAYER pl2 = {2,"b",2,2,2};
    PLAYER pl3 = {3,"c",3,3,3};

    PLAYER *head;
    head = &pl1;
    pl1.next = &pl2;
    pl2.next = &pl3;
    pl3.next = NULL;

    printf("This first playeris %s", head->next->name);
    return 0;
}
