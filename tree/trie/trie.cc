#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<sstream>
#include<vector>
#include<time.h>
#include<string.h>

using namespace std;
class trienode
{
public:
    char *word;
    int count;
    trienode *branch[37];
public:
    trienode()
    {
        word = NULL;
        count = 0;//词频
        memset(branch, NULL, sizeof(trienode*) * 26);
    }
};

class trie
{
public:
    trienode *root;
public:
    trie();
    ~trie();
    void Insert(char *str);
    bool Search(char*str, int &count);//索引
    void printall(trienode *root);//字符排序
    void printpre(char *str);//前缀匹配
};
trie::trie()
{
    root = new trienode();
}
trie::~trie() {}
void trie::Insert(char *str)
{
    int index;
    trienode *tt = root;
    
    for (int i = 0; str[i]; i++) {
        index = str[i] - 'a';
        
        if (index < 0 || index > 26) {
            return;
        }
        
        if (tt->branch[index] == NULL) {
            tt->branch[index] = new trienode();
        }
        
        tt = tt->branch[index];
    }
    
    if (tt->word) {
        tt->count++;
        return;
    } else {
        tt->count++;
        tt->word = new char[strlen(str) + 1];
        strncpy(tt->word, str, strlen(str) + 1);
    }
    
}
bool trie::Search(char *str, int &count)
{
    int index = -1;
    trienode *tt = root;
    
    while (tt && *str) {
        index = *str - 'a';
        
        if (index < 0 || index > 26) return false;
        
        tt = tt->branch[index];
        str++;
    }
    
    if (tt && tt->word) {
        count = tt->count;
        return true;
    }
    
    return false;
}
void trie::printall(trienode *root)
{
    trienode *t = root;
    
    if (!t) return;
    
    if (t->word) {
        cout << t->word << endl;
    }
    
    for (int i = 0; i < 26; i++) {
        printall(t->branch[i]);
    }
    
}
void trie::printpre(char *str)
{
    trienode *t = root;
    int index = -1;
    
    while (t && *str) {
        index = *str - 'a';
        
        if (index < 0 || index > 26) return;
        
        t = t->branch[index];
        str++;
    }
    
    if (t) {
        printall(t);
    }
}
int main()
{
    clock_t startTime, endTime;
    startTime = clock();
    trie *t = new trie();
    ifstream it("file.txt");
    string sline;
    string str = "";
    
    while (it && getline(it, sline)) {
        str += sline + " ";
    }
    it.close();
    
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ',' || str[i] == '(' || str[i] == '(') {
            str.erase(i, 1);
        }
    }
    
    string word;
    stringstream ss(str);
    vector<string> vec;
    
    while (ss >> word) {
        vec.push_back(word);
    }
    
    vector<string>::iterator iter;
    
    for (iter = vec.begin(); iter != vec.end(); iter++) {
        cout << "iter:" << *iter <<endl;
        t->Insert((char*)(*iter).data());
    }
    
    int val = -1;
    
    if (t->Search("the", val)) {
        cout << val << endl;
    } else {
        cout << "empty" << endl;
    }
    
    endTime = clock();
    cout << "the running time is " << (double)(endTime - startTime) << endl;
    getchar();
    return 0;
}

