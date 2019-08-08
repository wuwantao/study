#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct student { //代表一个实际节点的结构
    char name[100];
    int num;
    struct list_head node;   //内核链表里的节点结构
} student_t;

struct list_head student_list;

int main()
{
    int i = 0;
    student_t *p = NULL;
    struct list_head *pos = NULL;
    student_t *student = NULL;
    //初始化一个链表，其实就是把student_list的prev和next指向自身
    INIT_LIST_HEAD(&student_list);

    p = malloc(sizeof(student_t) * 5); //申请5个student结构空间
    memset(p, 0, sizeof(student_t) * 5); //清空

    for (i = 0; i < 5; i++) {
        //为结构体属性赋值
        sprintf(p[i].name, "Student%d", i + 1);
        p[i].num = i + 1;
        //加入链表节点，list_add的话是在表头插入，list_add_tail是在表尾插入
        list_add( &(p[i].node), &student_list);//参数1是要插入的节点地址，参数2是链表头地址
    }

    list_for_each(pos, &student_list) //list_for_each用来遍历链表，这是个宏定义
    //pos在上面有定义
    {
        //list_entry用来提取出内核链表节点对应的实际结构节点，即根据struct list_head来提取struct student
        //第三个参数list就是student结构定义里的属性list
        //list_entry的原理有点复杂，也是linux内核的一个经典实现，这个在上面那篇链接文章里也有讲解
        student = list_entry(pos, student_t, node);
        //打印一些信息，以备验证结果
        printf("student %d name: %s\n", student->num, student->name);
    }

    return 0;
}
