#include <iostream>
#include <string>
#include <algorithm>
#include <conio.h>
#include "fileProc.h"
#include "HashTable.h"
#include "LinkedList.h"
#include "AVLTree.h"

using namespace std;
int binarySearch(Node*& a, int l, int r, std::string& word)
{
    while (l <= r) {
        int m = l + (r - l) / 2;

        if (a[m].word.compare(word) == 0)
            return true;

        if (a[m].word.compare(word) < 0)
            l = m + 1;

        else
            r = m - 1;
    }

    return false;
}

std::string MaxVectorSizeInHST(HashTable* ht) {
    int max = 0;
    std::string res;
    for (int i = ht->minHash; i <= ht->maxHash; i++) {
        if (ht->ht[i].size() > max) {
            max = ht->ht[i].size();
            res = ht->ht[i].back().word;
        }
    }
    return res;
}
template<class T>
void deleteTree(T* node)
{
    if (!node) return;

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}

void DeleteHST(HashTable* ht) {
    for (int i = ht->minHash; i <= ht->maxHash; i++) {
        ht->ht[i].clear();
    }
    delete ht;
}

void CheckTimeReadFileAndInsert(std::vector<std::string> fileName) {
    //Cần thao tác trên cấu trúc dữ liệu nào thì bỏ comment của cấu trúc dữ liệu đó
    int size = fileName.size();
    std::vector<std::vector<double>> time(size);
    for (int i = 0; i < size; i++) {
        double max = 0, min = 1000, average = 0, sum = 0;
        system("cls");
        for (int j = 0; j < 1000; j++) {
            clock_t start, end;
            double duration;
            start = clock();
            //int n = 0;
            //Node* a = SaveArr(fileName[i], n);
            //std::sort(a, a + n, Node::great_than());
            HashTable* ht = SaveHS(fileName[i]);
            //AVLWordNode* avl = SaveAVL(fileName[i]);
            end = clock();
            DeleteHST(ht);
            //deleteTree(avl);
            //delete[] a;
            duration = ((double)end - start) / CLOCKS_PER_SEC;
            if (duration > max) {
                max = duration;
            }
            if (duration < min) {
                min = duration;
            }
            sum += duration;
            std::cout << j << ": " << duration << std::endl;
        }
        average = sum / 1000.0;
        time[i].push_back(min);
        time[i].push_back(max);
        time[i].push_back(average);
    }
    //Cần thao tác trên cấu trúc dữ liệu nào thì bỏ comment của cấu trúc dữ liệu đó
 
    //WriteResult(time, "insertAVL.txt");
    WriteResult(time, "insertHashTable.txt");
    //WriteResult(time, "insertArray.txt");
}
void CheckTimeSearch(std::vector<std::string> test, std::vector<std::string> fileName) {
    //Cần thao tác trên cấu trúc dữ liệu nào thì bỏ comment của cấu trúc dữ liệu đó
    bool res;
    int size = fileName.size();
    std::vector<std::vector<double>> time(size);
    for (int i = 0; i < size; i++) {
        double max = 0, min = 1000, average = 0, sum = 0;
        system("cls");
        AVLWordNode* avl = SaveAVL(fileName[i]);
        //HashTable* ht = SaveHS(fileName[i]);
        //int n = 0;
        //Node* a = SaveArr(fileName[i], n);
        //std::sort(a, a + n, Node::great_than());
        for (int j = 0; j < 1000; j++) {
            clock_t start, end;
            double duration;
            start = clock();
            for (int i = 0; i < 1000; i++) {
                //res = ht->Search(test[i]);
                res = Contain(avl, test[i]);
                //res = binarySearch(a, 0, n - 1, test[i]);
            }
            end = clock();
            duration = ((double)end - start) / CLOCKS_PER_SEC;
            if (duration > max) {
                max = duration;
            }
            if (duration < min) {
                min = duration;
            }
            sum += duration;
            std::cout << j << ": " << duration << std::endl;
        }
        deleteTree(avl);
        //DeleteHST(ht);
        //delete[] a;
        average = sum / 1000.0;
        time[i].push_back(min);
        time[i].push_back(max);
        time[i].push_back(average);
    }
    //Cần thao tác trên cấu trúc dữ liệu nào thì bỏ comment của cấu trúc dữ liệu đó
    WriteResult(time, "searchAVL.txt");
    //WriteResult(time, "searchHashTable.txt");
    //WriteResult(time, "searchArray.txt");
}
void CheckTimeOnAVLTree(std::vector<std::string> fileName, std::string fileSample) {
    //Cần thao tác trên cấu trúc dữ liệu nào thì bỏ comment của cấu trúc dữ liệu đó
    bool res, size1, size2, pos;
    std::vector<std::string> sample = SaveVector(fileSample);
    int size = fileName.size();
    std::vector<std::vector<double>> time(size);
    for (int i = 0; i < size; i++) {
        double max = 0, min = 1000, average = 0, sum = 0;
        AVLWordNode* avl = SaveAVL(fileName[i]);
        for (int j = 0; j < 1000; j++) {
            clock_t start, end;
            double duration;
            start = clock();
            //size1 = TotalSize(avl);
            //size2 = Size(avl);
            for (int k = 0; k < 1000; k++) {
                res = Contain(avl, sample[k]);
                //avl = Insert(avl, sample[k], -1);
                //pos = FindOrder(avl, sample[k]);
            }
            end = clock();
            duration = ((double)end - start) / CLOCKS_PER_SEC;
            if (duration > max) {
                max = duration;
            }
            if (duration < min) {
                min = duration;
            }
            sum += duration;
            std::cout << j << ": " << duration << std::endl;
        }
        deleteTree(avl);
        average = sum / 1000.0;
        time[i].push_back(min);
        time[i].push_back(max);
        time[i].push_back(average);
    }
    //Cần thao tác trên cấu trúc dữ liệu nào thì bỏ comment của cấu trúc dữ liệu đó
    WriteResult(time, "AVLContain.txt");
    //WriteResult(time, "AVLFindOrder.txt");
    //WriteResult(time, "AVLSize.txt");
    //WriteResult(time, "AVLTotalSize.txt");
    //WriteResult(time, "AVLInsert.txt");
}
int main()
{
    std::string fileName = "TextSample.txt";
    std::string fileSearch = "search.txt";

    std::vector<std::string> fileNames{ "1.txt" ,"2.txt" ,"3.txt" ,"4.txt" ,"5.txt" };

    //std::vector<std::string> test = SaveVector(fileSearch);
    //CheckTimeSearch(test, fileNames);

    CheckTimeReadFileAndInsert(fileNames);

    //CheckTimeOnAVLTree(fileNames,"search.txt");

    return 0;
}

