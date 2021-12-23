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
};