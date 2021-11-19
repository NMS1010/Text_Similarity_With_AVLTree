#pragma once
#include<iostream>
template<class T>
struct DoubleNode {
	T value;
	DoubleNode* next;
	DoubleNode* prev;
	DoubleNode(T val) {
		value = val;
		next = nullptr;
		prev = nullptr;
	}
};

template<class T>
class Queue {
private:
	DoubleNode<T>* top = nullptr;
	DoubleNode<T>* bottom = nullptr;
public:
	bool Empty() {
		return bottom == nullptr;
	}

	int size() {
		int count = 0;
		DoubleNode<T>* travel = top;
		while (travel != nullptr) {
			count++;
			travel = travel->next;
		}
		return count;
	}

	void enqueue(T value) {
		if (top == nullptr) {
			DoubleNode<T>* temp = new DoubleNode<T>(value);
			top = temp;
			top->next = nullptr;
			top->prev = nullptr;
			bottom = top;
			return;
		}

		DoubleNode<T>* temp = new DoubleNode<T>(value);
		temp->next = top;
		top->prev = temp;
		top = temp;
	}

	T dequeue() {
		if (bottom != nullptr) {
			T value = bottom->value;
			DoubleNode<T>* del = bottom;
			if (bottom == top) {
				top = nullptr;
			}
			bottom = bottom->prev;
			if (bottom != nullptr)
				bottom->next = nullptr;
			delete del;

			return value;
		}
	}
	T front() {
		return bottom->value;
	}
	void Sort() {
		int n = this->size();
		for (int i = 0; i < n; i++) {
			int posmin = -1;
			T minVal = this->front();
			for (int j = 0; j < n ; j++) {
				T val = this->dequeue();
				if (val < minVal && j<n-i) {
					minVal = val;
					posmin = j;
				}
				this->enqueue(val);
			}

			for (int k = 0; k < n; k++) {
				if (k != posmin) {
					this->enqueue(this->dequeue());
				}
				else {
					minVal = this->dequeue();
				}
			}
			this->enqueue(minVal);
		}
	}
};

