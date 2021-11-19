#pragma once
#include<iostream>
template <class T>
struct SingleNode {
	T value;
	SingleNode* next;
	SingleNode(T val) {
		value = val;
		next = nullptr;
	}
};

template <class T>
class Stack {
private:
	SingleNode<T>* top = nullptr;
	/*
	void AddTail(SingleNode*& head, SingleNode* p) {
		if (head == nullptr) {
			head = new SingleNode(p->value);
			head->next = nullptr;
			return;
		}

		SingleNode* travel = head;
		while (travel->next != nullptr) {
			travel = travel->next;
		}
		travel->next = p;
		p->next = nullptr;
	}
	void QuickSort(SingleNode*& head) {
		if (head == nullptr) return;

		SingleNode* head1 = nullptr;
		SingleNode* head2 = nullptr;

		SingleNode* pivot = head;
		SingleNode* travel = head->next;

		while (travel != nullptr) {
			SingleNode* temp = travel;
			travel = travel->next;

			if (temp->value < pivot->value) {
				AddTail(head1, temp);
			}
			else {
				AddTail(head2, temp);
			}
		}

		QuickSort(head1);
		QuickSort(head2);

		if (head1 != nullptr) {
			head = head1;
			SingleNode* temp = head1;
			while (temp->next != nullptr) {
				temp = temp->next;
			}
			temp->next = pivot;
		}
		else {
			head = pivot;
		}
		pivot->next = head2;

	}
	*/
	
	/*void SimpleSortStack(SingleNode*& head) {
		Stack temp;

		while (this->size() != 0) {
			int value = this->pop();

			while (temp.size() != 0 && temp.Top() < value) {
				this->push(temp.pop());
			}

			temp.push(value);
		}

		head = temp.top;
	}
	*/
public:
	bool Empty() {
		return top == nullptr;
	}

	int size() {
		int count = 0;
		SingleNode<T>* travel = top;
		while (travel != nullptr) {
			count++;
			travel = travel->next;
		}
		return count;
	}

	void push(T value) {
		if (top == nullptr) {
			SingleNode<T>* temp = new SingleNode<T>(value);
			top = temp;
			top->next = nullptr;
			return;
		}

		SingleNode<T>* temp = new SingleNode<T>(value);
		temp->next = top;
		top = temp;
	}

	T pop() {
		if (top == nullptr) exit(0);
		T value = top->value;
		SingleNode<T>* del = top;
		top = top->next;
		delete del;

		return value;
	}
	T Top() {
		if (Empty()) exit(0);
		return top->value;
	}

	void Sort() {
		Stack temp;
		T value;
		while (this->size() != 0) {
			value = this->pop();

			while (temp.size() != 0 && temp.Top() < value) {
				this->push(temp.pop());
			}

			temp.push(value);
		}

		this->top = temp.top;
	}
};

