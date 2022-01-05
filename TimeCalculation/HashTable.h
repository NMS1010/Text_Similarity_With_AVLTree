#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
struct Node {
	std::string word = "";
	std::vector<int> order;
	int count;
	Node(std::string word, int order) {
		this->word = word;
		this->order.push_back(order);
		count = 1;
	}
	Node() {
		count = 1;
	}
	struct great_than {
		bool operator()( const Node& a, const Node& b) {
			return a.word.compare(b.word) < 0;
		}
	};
};

struct HashTable {
	int total;
	int maxHash;
	int minHash;
	std::vector<std::vector<Node>> ht;
	std::vector<Node> linear;
	HashTable() {
		total = 0;
		maxHash = 0;
		minHash = 100000;
		linear.resize(500000);
		ht.resize(5000);
	}
	int Hash(std::string value) {
		int hash = 0;
		for (int i = 0; i < value.length(); i++) {
			hash = hash + value[i];
		}
		return hash;
	}
	int Contains(std::vector<Node>& a, int l, int r, std::string& word) {
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
	bool Search(std::string& value) {
		int key = Hash(value);
		if (ht[key].size() == 0) 
			return false;
		if (ht[key][0].word.compare(value) == 0) {
			return true;
		}
		return Contains(ht[key], 0, ht[key].size() - 1, value);
	}
	void AddLinear(const std::string& value, const int& order) {
		int key = Hash(value);
		if (key > maxHash) {
			maxHash = key;
		}
		if (key < minHash) {
			minHash = key;
		}
		Node temp;
		temp.word = value;
		temp.order.push_back(order);
		temp.count = 1;
		if (linear[key].word == "") {
			linear[key] = temp;
		}
		else {
			int i = 1;
			while (linear[key].word != "") {
				key = key + i;
				i++;
			}
			linear[key] = temp;
		}
	}
	void Add(const std::string& value, const int& order) {
		int key = Hash(value);
		if (key > maxHash) {
			maxHash = key;
		}
		if (key < minHash) {
			minHash = key;
		}
		Node temp;
		temp.word = value;
		temp.order.push_back(order);
		temp.count = 1;

		if (ht[key].size() == 0) {
			ht[key].push_back(temp);
			total++;
			return;
		}
		std::vector<Node>::iterator it = std::upper_bound(ht[key].begin(), ht[key].end(), temp, Node::great_than());
		if (it != ht[key].begin() && (*(it - 1)).word.compare(value) == 0) {
			(*(it - 1)).count++;
			(*(it - 1)).order.push_back(order);
			return;
		}
		ht[key].insert(it, temp);
		total++;
	}
};