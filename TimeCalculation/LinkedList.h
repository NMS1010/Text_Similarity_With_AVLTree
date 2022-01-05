#pragma once

#include <iostream>
#include <string>

struct SNode {
    std::string word;
    int order;

    SNode* next;
    SNode(std::string word, int order) {
        this->word = word;
        this->order = order;
        next = nullptr;
    }
    SNode(std::string word) {
        this->word = word;
        this->order = -1;
        next = nullptr;
    }
};

struct LinkedList {
    SNode* head = nullptr;
    SNode* tail = nullptr;
    int size = 0;
    void AddHead(SNode* p) {
        p->next = head;
        head = p;
        size++;
    }
    void AddTail(SNode* p) {
        if (head == nullptr) {
            AddHead(p);
            tail = head;
            return;
        }
        tail->next = p;
        tail = p;
        size++;
    }
    void Add(SNode* p) {
        if (head == nullptr) {
            AddHead(p);
            tail = head;
            return;
        }
        SNode* temp = head;
        while (temp->next) {
            if (temp->word.compare(p->word) == 0) {
                return;
            }
            temp = temp->next;
        }
        temp->next = p;
        size++;
    }
    void InsertAndSort(SNode*& head, SNode* p) {
        if (!head) {
            AddHead(p);
            tail = head;
            return;
        }
        if (p->word.compare(head->word) <= 0) {
            AddHead(p);
            return;
        }
        SNode* temp = head;
        while (temp->next != nullptr) {
            if (p->word > temp->word && p->word <= temp->next->word) {
                p->next = temp->next;
                temp->next = p;
                size++;
                return;
            }
            temp = temp->next;
        }
        AddTail(p);
    }
    int FindOrder(std::string word) {
        SNode* temp = head;
        while (temp) {
            if (temp->word.compare(word) == 0) {
                return temp->order;
            }
            temp = temp->next;
        }
        return -1;
    }
    bool Search(std::string word) {
        SNode* temp = head;
        while (temp) {
            if (temp->word.compare(word) == 0) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
};