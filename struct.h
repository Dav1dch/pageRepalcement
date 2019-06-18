#pragma once
#include <iostream>
#include <cstring>
#include <random>
using namespace std;

class programe
{
public:
    int address;
    int processNum;
    programe(int add, int num);
};

programe::programe(int add, int num)
{
    address = add;
    processNum = num;
}

class PhyscisMemoryBlock
{
public:
    int flag;
    int pageNum;
    int existTime;
    PhyscisMemoryBlock *next;
    PhyscisMemoryBlock();
};

PhyscisMemoryBlock::PhyscisMemoryBlock()
{
    flag = -1;
    pageNum = -1;
    existTime = 0;
    next = NULL;
}

class replacement
{
public:
    int *address;
    int *pagenum;
    PhyscisMemoryBlock *head;
    int instructmentNum;
    int pagesize;
    int memNum;
    void transform(programe p);
    void FIFO();
    void OPT();
    void LRU();
    PhyscisMemoryBlock *inMemory(int num);
    PhyscisMemoryBlock *getFreepage();
    PhyscisMemoryBlock *getLongestSurvive();
};

void replacement::transform(programe p)
{
    srand(time(0));
    bool flagRandom[321];
    memset(flagRandom, 0, 320);
    head = new PhyscisMemoryBlock;
    int *newPagenum;
    instructmentNum = p.processNum;
    address = new int[instructmentNum];
    pagenum = new int[instructmentNum];
    address[0] = 10000;
    pagenum[0] = address[0] / pagesize;
    int a;
    int j = 0;
    int count = 0;
    for (int i = 1; i < instructmentNum; i++)
    {
        a = 1 + rand() % 320;
        while (flagRandom[a])
        {
            a = 1 + rand() % 320;
        }
        flagRandom[a] = true;
        if (a <= 160)
        {
            address[i] = 1 + address[i - 1];
        }
        else if (a <= 240)
        {
            address[i] = 1 + rand() % address[i - 1];
        }
        else
        {
            address[i] = address[i - 1] + rand() % (p.address - address[i - 1]);
        }
        int q, r;
        q = address[i] / pagesize;
        r = address[i] % pagesize;
        if (r)
        {
            pagenum[i] = q;
        }
        else
        {
            pagenum[i] = q - 1;
        }
        //if(pagenum[i] == 1) count++;
    }
    // cout<<count<<endl;
    newPagenum = new int[instructmentNum];
    newPagenum[j] = pagenum[0];
    for (int k = 0; k < instructmentNum - 1; k++)
    {
        if (pagenum[k] != pagenum[k + 1])
        {
            j++;
            newPagenum[j] = pagenum[k];
            //cout<<newPagenum[j]<<"  ";
            if (newPagenum[j] == 0)
            {
                count++;
            }
        }
    }
    pagenum = newPagenum;
    instructmentNum = j + 1;
    cout << count << " " << instructmentNum;
    cout << endl;
}

PhyscisMemoryBlock *replacement::inMemory(int num)
{
    PhyscisMemoryBlock *temp = head;
    while (temp)
    {
        if (temp->pageNum == num)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

PhyscisMemoryBlock *replacement::getFreepage()
{
    PhyscisMemoryBlock *temp = head->next;
    while (temp)
    {
        if (temp->flag == -1)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

PhyscisMemoryBlock *replacement::getLongestSurvive()
{
    PhyscisMemoryBlock *temp = head->next;
    PhyscisMemoryBlock *target = NULL;
    int n = 0;
    while (temp)
    {
        if (temp->existTime > n)
        {
            n = temp->existTime;
            target = temp;
        }
        temp = temp->next;
    }
    return target;
}

void replacement::FIFO()
{
    int breaktimes = 0;
    PhyscisMemoryBlock *temp = head;
    for (int i = 0; i < memNum; i++)
    {
        PhyscisMemoryBlock *node = new PhyscisMemoryBlock;
        temp->next = node;
        temp = temp->next;
    }
    temp = head;
    PhyscisMemoryBlock *target;
    for (int i = 0; i <= instructmentNum; i++)
    {
        target = inMemory(pagenum[i]);
        if (target == NULL)
        {
            breaktimes++;
            target = getFreepage();
            if (target != NULL)
            {
                target->flag = 1;
                target->pageNum = pagenum[i];
            }
            else
            {
                target = getLongestSurvive();
                target->existTime = 0;
                target->pageNum = pagenum[i];
            }
        }
        for (int k = 0; k < memNum; k++)
        {
            temp = temp->next;
            temp->existTime++;
            //cout<<temp->pageNum<<endl;
        }
        temp = head;
    }
    cout << "页面大小： " << pagesize << " FIFO "
         << " 内存块数：" << memNum << " 中断率: " << (float)breaktimes / (float)instructmentNum << endl;
    return;
}

void replacement::LRU()
{
    int breaktimes = 0;
    PhyscisMemoryBlock *temp = head;
    for (int i = 0; i < memNum; i++)
    {
        PhyscisMemoryBlock *node = new PhyscisMemoryBlock;
        temp->next = node;
        temp = temp->next;
    }
    temp = head;
    PhyscisMemoryBlock *target;
    for (int i = 0; i <= instructmentNum; i++)
    {
        target = inMemory(pagenum[i]);
        if (target == NULL)
        {
            breaktimes++;
            target = getFreepage();
            if (target != NULL)
            {
                target->flag = 1;
                target->pageNum = pagenum[i];
            }
            else
            {
                target = getLongestSurvive();
                target->existTime = 0;
                target->pageNum = pagenum[i];
            }
        }
        else
            target->existTime = 0;
        for (int k = 0; k < memNum; k++)
        {
            temp = temp->next;
            temp->existTime++;
            //cout<<temp->pageNum<<endl;
        }
        temp = head;
    }
    cout << "页面大小： " << pagesize << " LRU  "
         << " 内存块数：" << memNum << " 中断率: " << (float)breaktimes / (float)instructmentNum << endl;
    return;
}

void replacement::OPT()
{
    int breaktimes = 0;
    PhyscisMemoryBlock *temp = head;
    for (int i = 0; i < memNum; i++)
    {
        PhyscisMemoryBlock *node = new PhyscisMemoryBlock;
        temp->next = node;
        temp = temp->next;
    }
    temp = head;
    PhyscisMemoryBlock *target;
    for (int i = 0; i <= instructmentNum; i++)
    {
        target = inMemory(pagenum[i]);
        int sum = -1;
        int count = 0;
        if (target == NULL)
        {
            breaktimes++;
            target = getFreepage();
            if (target != NULL)
            {
                target->flag = 1;
                target->pageNum = pagenum[i];
            }
            else
            {
                PhyscisMemoryBlock *temp1 = head;
                for (int j = 0; j < memNum; j++)
                {
                    temp1 = temp1->next;
                    int flag1 = 0;
                    for (int k = i + 1; k < instructmentNum; k++)
                    {
                        if (temp1->pageNum == pagenum[k])
                        {
                            if (k - i > sum)
                            {
                                sum = k - i;
                                count = temp1->pageNum;
                                flag1 = 1;
                                break;
                            }
                        }
                    }
                    if (flag1 == 0)
                    {
                        count = temp1->pageNum;
                        break;
                    }
                }
                temp1 = head->next;
                if (sum == -1)
                {
                    temp1->pageNum = pagenum[i];
                }
                else
                {
                    while (count != temp1->pageNum)
                    {

                        temp1 = temp1->next;
                    }
                    temp1->pageNum = pagenum[i];
                }
            }
        }
    }
    cout << "页面大小： " << pagesize << " OPT  "
         << " 内存块数：" << memNum << " 中断率: " << (float)breaktimes / (float)instructmentNum << endl;
}