#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "AVLTree.h"
#include "normalize.h"

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

AVLSentenceNode* GetAllSentenceFromFile(const std::string& fileName,AVLWordNode* stopWords) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw "Can't open " + fileName;
	}
	AVLSentenceNode* root = nullptr;
	std::string word;
	std::vector<std::string> sents;
	int countSent = 1;
	bool isRemovePunctual = false;

	while (in >> word) {
		StringTolowerAndRemoveCharacter(word, isRemovePunctual);
		if (!Contain(stopWords, word)) {
			sents.push_back(word);
		}
		if (IsPunctual(word[word.length() - 1])) {
			word.pop_back();
			if (!Contain(stopWords, word)) {
				sents.pop_back();
				sents.push_back(word);
			}
			root = Insert(root, sents, countSent++);
			sents.clear();
		}
	}

	in.close();
	return root;
}
/*
void WriteTree(AVLWordNode* root, std::ostream& os) {
	if (root) {
		WriteTree(root->left, os);
		os << root->word << std::endl;
		WriteTree(root->right, os);
	}
}

void Write(AVLWordNode* root) {
	std::ofstream ou("test.txt");
	WriteTree(root, ou);
	ou.close();
}
*/
