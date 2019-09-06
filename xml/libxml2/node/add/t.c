#include <stdio.h>
#include <stdlib.h>
#include <parser.h>
#include <tree.h>
#include <xpath.h>
#include <xpathInternals.h>
#include <xmlmemory.h>
#include <xpointer.h>

int tim_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
    //int nsec;
    if (x->tv_sec > y->tv_sec)
        return -1;
    if ((x->tv_sec == y->tv_sec) && (x->tv_usec > y->tv_usec))
        return -1;
    result->tv_sec = (y->tv_sec - x->tv_sec);
    result->tv_usec = (y->tv_usec - x->tv_usec);
    if (result->tv_usec < 0)
    {
        result->tv_sec--;
        result->tv_usec += 1000000;
    }
    return 0;
}
//使用xpath来获得目标结点
xmlXPathObjectPtr getNodeset(xmlDocPtr pdoc, const xmlChar *xpath)
{
    xmlXPathContextPtr context = NULL; //定义XPath上下文指针
    context = xmlXPathNewContext(pdoc);//用xml文档指针来初始化context指针
    xmlXPathObjectPtr result = NULL; //定义XPath结果指针

    if (pdoc == NULL)
    {
        printf("pdoc is NULL\n");
        return NULL;
    }

    if (xpath)
    {
        if (context == NULL)
        {
            printf("context is NULL\n");
            return NULL;
        }

        result = xmlXPathEvalExpression(xpath, context);//计算xpath的查询结果,并存入result中
        if (result == NULL)
        {
            printf("xmlXPathEvalExpression return NULL\n");
            return NULL;
        }


        if (xmlXPathNodeSetIsEmpty(result->nodesetval))
            //使用result->nodesetval得到节点集合指针,其中包含了所有符合Xpath查询结果的节点
        {
            xmlXPathFreeObject(result);//使用释放Xpath对象指针；
            printf("nodeset is empty\n");
            return NULL;
        }
    }

    return result;
}

int main(int argc, char **argv)
{
    struct timeval t1,t2,diff;
    gettimeofday(&t1,0);
    xmlDocPtr pdoc = NULL;//文档指针
    xmlNodePtr proot = NULL;//节点指针

    /*****************打开xml文档********************/
    //参数为文件名、编码类型和枚举变量(下面一个或多个的组合）
    //XML_PARSE_RECOVER    = 1<<0,    recover on errors    尝试修复错误语法
    //XML_PARSE_NOERROR    = 1<<5,    suppress error reports    不输出错误日志
    //XML_PARSE_NOWARNING    = 1<<6,    suppress warning reports    不输出警告日志
    //XML_PARSE_PEDANTIC    = 1<<7,          pedantic error reporting */    输出详细的错误日志
    //XML_PARSE_NOERROR和XML_PARSE_NOWARNING的功能是“当运行命令行程序时不输出错误/警告日志”。
    //pdoc = xmlReadFile("1.xml", "UTF-8", XML_PARSE_RECOVER); 
    pdoc = xmlReadFile("1.xml", "UTF-8", XML_PARSE_NOBLANKS); 
    if (pdoc == NULL)
    {
        printf("error:can't open file!\n");
        exit(1);
    }

    /*****************获取xml文档对象的根节对象********************/
    proot = xmlDocGetRootElement(pdoc);

    if (proot == NULL) 
    {
        printf("error: file is empty!\n");
        exit(1);
    }

    /*****************查找书店中所有书籍的名称********************/
    xmlChar *xpath = BAD_CAST ("/producttype/product[1]/writer"); //xpath语句,路径为product的所有子元素
    xmlXPathObjectPtr result = getNodeset(pdoc, xpath); //调用函数,查询XPath表达式,得到一个查询结果
    if (result == NULL)
    {
        printf("result is NULL\n");
        exit(1);
    }

    if (result)
    {
        xmlNodeSetPtr nodeset = result->nodesetval; //获取查询到的节点指针集合,其中包含了所有符合Xpath查询结果的节点；
        xmlNodePtr cur;//声明一个临时的当前结点指针

        //nodeset->nodeNr是集合元素总数,for遍历每个符合结果的结点
        int i;
        for (i = 0; i < nodeset->nodeNr; i++)
        {
            cur = nodeset->nodeTab[i];//遍历

            /**************接下来就是使用libxml2函数进行结点的操作**********/
            while (cur != NULL)
            {
                //查找名为write的结点,并输出其内容
                if (!xmlStrcmp(cur->name, BAD_CAST ("writer")))//xmlStrcmp为对比,相同时返回值为0
                {
                    printf("writer: %s\n",
                            ((char*) xmlNodeGetContent(cur)));//xmlNodeGetContent为获得某结点的内容
                    //创建结点,结点名称为CHILD
                    xmlNodePtr child=xmlNewNode(NULL,BAD_CAST"CHILD");
                    //设置其内容和属性
                    xmlNodeSetContent(child,BAD_CAST"子节点");
                    xmlSetProp(child,BAD_CAST"属性名",BAD_CAST"属性内容");
                    xmlAddChild(cur,child);
                    break;
                }
                /*
                //移除属性,返回0成功,-1失败,每次只能移除一个属性
                int j;
                j=xmlRemoveProp(cur);
                //将child添加给cur为子结点

                //创建并直接添加子节点
                xmlNewTextChild(cur, NULL, BAD_CAST "结点名称", BAD_CAST "结点内容");
                //删除newNode1
                if (!xmlStrcmp(cur->name, BAD_CAST "newGrandSon"))
                {
                    xmlNodePtr tempNode;
                    tempNode = cur->next; //使用一个临时变量来存储断链节点的后续节点
                    xmlUnlinkNode(cur); //将当前节点从文档中断链（unlink）,这样本文档就不会再包含这个子节点
                    xmlFreeNode(cur); //手动删除断链节点的内存
                    cur = tempNode;
                    continue;
                }
                */
                cur = cur->next;
            }
        }

        xmlXPathFreeObject(result); //释放结果指针
    }
    gettimeofday(&t2,0);
    int ret=tim_subtract(&diff,&t1,&t2);
    if(ret>-1)
    {
        printf("用时%dsec\t%dusec\n",diff.tv_sec,diff.tv_usec);
    }
    //存储xml文档
    //int nRel = xmlSaveFile("NewFile.xml", pdoc);
    int nRel = xmlSaveFormatFile("NewFile.xml", pdoc, 1);
    /*****************释放资源********************/
    xmlFreeDoc(pdoc);
    xmlCleanupParser();
    xmlMemoryDump();

    return 0;
}
