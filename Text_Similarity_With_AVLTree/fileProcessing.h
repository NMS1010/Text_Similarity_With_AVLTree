#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "AVLTree.h"
#include "normalize.h"

//Use AVL Tree
AVLWordNode* GetStopWordsFromFile(const std::string& fileName) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw "Can't open " + fileName;
	}
	AVLWordNode* root = nullptr;
	std::string word;
	int count = 1;
	while (in >> word) {
		root = Insert(root, word, count);
		count++;
	}
	in.close();
	return root;
}

AVLWordNode* GetAllWordFromFile(const std::string& fileName, AVLWordNode*& stopWords) {
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
		if (word != ""){
			if (!Contain(stopWords, word)) {
				root = Insert(root, word, count);
			}
			count++;
		}
	}
	in.close();
	return root;
}

std::vector<AVLWordNode*> GetVectorSent(const std::string& fileName, AVLWordNode*& stopWords) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw "Can't open " + fileName;
	}
	std::vector<AVLWordNode*> words;
	std::string word;
	AVLWordNode* allWords = nullptr;
	bool isRemovePunctual = false;
	int orderWord = 1;

	while (in >> word) {
		StringTolowerAndRemoveCharacter(word, isRemovePunctual);
		if (word != "") {
			if (IsPunctual(word[word.length() - 1])) {
				word.pop_back();
				if (!Contain(stopWords, word)) {
					allWords = Insert(allWords, word, orderWord++);
				}
				if (allWords)
					words.push_back(allWords);
				orderWord = 1;
				allWords = nullptr;
			}
			else {
				if (!Contain(stopWords, word)) {
					allWords = Insert(allWords, word, orderWord);
				}
				orderWord++;
			}
		}
	}
	if(allWords) words.push_back(allWords);
	in.close();
	return words;
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
		a.push_back(line);
	}
	in.close();
	return a;
}
void WriteResult(std::vector<std::vector<double>> res, std::string fileName) {
	int size = res.size();
	std::ofstream ou(fileName);

	for (int i = 0; i < size; i++) {
		ou << i + 1 << " " << res[i][0] << " " << res[i][1] << " " << res[i][2] << std::endl;
	}
	ou.close();
}
void WriteSim(std::vector<std::vector<std::string>> res, std::string fileName) {
	int size = res.size();
	std::ofstream ou(fileName);

	for (int i = 0; i < size; i++) {
		ou << res[i][0] << ": " << res[i][1]  << std::endl;
	}
	ou.close();
}