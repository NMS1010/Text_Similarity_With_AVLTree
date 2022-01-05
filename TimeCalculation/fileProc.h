#pragma once
#include <iostream>
#include <string.h>
#include <fstream>
#include <chrono>
#include <set>
#include <random>

#include "AVLTree.h"
#include "normalize.h"
#include "HashTable.h"

AVLWordNode* SaveAVL(const std::string& fileName) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw "Can't open " + fileName;
	}
	AVLWordNode* root = nullptr;
	std::string word;
	int count = 1;
	bool isRemovePunctual = true;
	while (in >> word) {
		StringTolowerAndRemoveCharacter(word, isRemovePunctual);
		if (word != "") {
			root = Insert(root, word, count);
			count++;
		}
	}
	in.close();
	return root;
}
HashTable* SaveHS(const std::string& fileName) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw "Can't open " + fileName;
	}
	HashTable* s = new HashTable;
	std::string word;
	int order = 1;
	bool isRemovePunctual = true;
	while (in >> word) {
		StringTolowerAndRemoveCharacter(word, isRemovePunctual);
		if (word != "") {
			s->Add(word, order);
			//s->AddLinear(word, order);
			order++;
		}
	}
	in.close();
	return s;
}

int insertSorted(Node* &arr, int& n, std::string& word, const int& order)
{
	int i;
	for (i = n - 1; (i >= 0 && arr[i].word.compare(word) > 0); i--)
		arr[i + 1] = arr[i];
	if (arr[i].word.compare(word) == 0) {
		arr[i].count++;
		arr[i].order.push_back(order);
		return n;
	}
	arr[i + 1].word = word;
	arr[i + 1].count = 1;
	arr[i + 1].order.push_back(order);
	return (n + 1);
}
int Contains(Node*& a, int l, int r, std::string& word) {
	while (l <= r) {
		int m = l + (r - l) / 2;

		if (a[m].word.compare(word) == 0)
			return m;

		if (a[m].word.compare(word) < 0)
			l = m + 1;

		else
			r = m - 1;
	}

	return -1;
}
Node* SaveArr(std::string& fileName, int &n) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw "Can't open " + fileName;
	}
	Node* a = new Node[100000];
	std::string line;

	int count = 1;

	while (in >> line) {
		StringTolowerAndRemoveCharacter(line, true);
		if (line != "") {
			//n = insertSorted(a, n, line, count);
			Node temp;
			temp.word = line;
			temp.count = 1;
			temp.order.push_back(count);
			a[n++] = temp;
			count++;
		}
	}
	in.close();
	return a;
}

std::vector<std::string> SaveVector(std::string fileName) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw "Can't open " + fileName;
	}
	std::vector<std::string> a;
	std::string line;

	int count = 1;
	while (in >> line) {
		StringTolowerAndRemoveCharacter(line, true);
		if (line != "") {
			a.push_back(line);
		}
	}
	in.close();
	return a;
}
//Write
void WriteToFile(std::string input, std::string output, int max) {
	int count = 0;
	std::ofstream out(output);
	std::ifstream in(input);
	std::string word;
	std::vector<std::string> words;
	while (in >> word) {
		words.push_back(word);
		count++;
		if (count == max) {
			in.close();
			break;
		}
	}
	unsigned seed = 0;
	std::shuffle(words.begin(), words.end(), std::default_random_engine(seed));
	int size = words.size();
	for (int i = 0; i < size; i++) {
		out << words[i] << std::endl;
	}
	out.close();
}
void WriteToFile(std::string input, std::string output, int start, int max) {
	int count = 0;
	std::ofstream out(output);
	std::ifstream in(input);
	std::string word;
	std::vector<std::string> words;
	while (in >> word) {
		if (count >= start)
			words.push_back(word);
		count++;
		if (count == max) {
			in.close();
			break;
		}
	}
	int size = words.size();
	for (int i = 0; i < size; i++) {
		out << words[i] << " ";
	}
	out.close();
}
void WriteToFile(std::string input, std::string output) {
	int count = 0;
	std::ofstream out(output);
	std::ifstream in(input);
	std::string word;
	std::set<std::string> words;
	while (in >> word) {
		words.insert(word);
	}
	std::vector<std::string> res;
	for (auto it = words.begin(); it != words.end(); ++it) {
		res.push_back(*(it));
	}

	unsigned seed = 0;
	std::random_shuffle(res.begin(), res.end());

	int size = res.size();
	for (int i = 0; i < size; i++) {
		out << res[i] << " ";
	}
	in.close();
	out.close();
}
void WriteResult(std::vector<std::vector<double>> res, std::string fileName) {
	int size = res.size();
	std::ofstream ou(fileName);

	for (int i = 0; i < size; i++) {
		ou << i + 1 << " " << res[i][0] << " " << res[i][1] << " " << res[i][2] << std::endl;
	}
	ou.close();
}