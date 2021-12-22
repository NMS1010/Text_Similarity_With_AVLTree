#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "AVLTree.h"
#include "normalize.h"
#include "search.h"

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

AVLWordNode* GetAllWordFromFile(const std::string& fileName, AVLWordNode* stopWords) {
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
		if (word != "" && !Contain(stopWords,word)) {
			root = Insert(root, word, count);
			count++;
		}
	}

	in.close();
	return root;
}

std::vector<AVLWordNode*> GetVectorSent(const std::string& fileName, AVLWordNode* stopWords) {
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
		if (IsPunctual(word[word.length() - 1])) {
			word.pop_back();
			if (!Contain(stopWords, word)) {
				allWords = Insert(allWords, word, orderWord++);
			}
			if(allWords)
				words.push_back(allWords);
			orderWord = 1;
			allWords = nullptr;	
		}
		else if (!Contain(stopWords, word)) {
			allWords = Insert(allWords, word, orderWord++);
		}
	}
	in.close();
	return words;
}
//
//Use Vector
std::vector<std::string> GetStopWords(std::string fileName) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		std::cout << "Cannot open file";
		exit(0);
	}

	std::vector<std::string> words;

	std::string line;
	while (in >> line) {
		words.push_back(line);
	}
	in.close();
	return words;
}

std::vector<std::string> GetAllWordFromFile(std::string fileName, std::vector<std::string> stopwords) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		std::cout << "Cannot open file";
		exit(0);
	}

	std::vector<std::string> words;

	std::string line;
	int stopwordSize = stopwords.size();
	while (in >> line) {
		StringTolowerAndRemoveCharacter(line, true);
		if (BinarySearch(line, stopwords, 0, stopwordSize - 1) == -1)
			words.push_back(line);
	}
	in.close();
	return words;
}

//