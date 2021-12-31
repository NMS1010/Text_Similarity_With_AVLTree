#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "AVLTree.h"
#include "normalize.h"
#include "linkedList.h"

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
	//double max = 0, min = 1000, average = 0, sum = 0, test;
	//for (int i = 0; i < 1000; i++) {
	//	clock_t start, end;
	//	double duration;
	//	start = clock();
	//	root = Insert(root, "sun", count);
	//	end = clock();
	//	duration = ((double)end - start) / CLOCKS_PER_SEC;
	//	if (duration > max) {
	//		max = duration;
	//	}
	//	if (duration < min) {
	//		min = duration;
	//	}
	//	sum += duration;
	//	std::cout << i << ": " << duration << std::endl;
	//}
	//average = sum / 1000.0;
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
		//double max = 0, min = 1000, average = 0, sum = 0, test;
		//for (int i = 0; i < 1000; i++) {
		//	clock_t start, end;
		//	double duration;
		//	start = clock();
		//	StringTolowerAndRemoveCharacter(word, isRemovePunctual);
		//	end = clock();
		//	duration = ((double)end - start) / CLOCKS_PER_SEC;
		//	if (duration > max) {
		//		max = duration;
		//	}
		//	if (duration < min) {
		//		min = duration;
		//	}
		//	sum += duration;
		//	std::cout << i << ": " << duration << std::endl;
		//}
		//average = sum / 1000.0;
		if (word != ""){
			//double max = 0, min = 1000, average = 0, sum = 0, test;
			//for (int i = 0; i < 1000; i++) {
			//	clock_t start, end;
			//	double duration;
			//	start = clock();
			//	Contain(stopWords, word);
			//	end = clock();
			//	duration = ((double)end - start) / CLOCKS_PER_SEC;
			//	if (duration > max) {
			//		max = duration;
			//	}
			//	if (duration < min) {
			//		min = duration;
			//	}
			//	sum += duration;
			//	std::cout << i << ": " << duration << std::endl;
			//}
			//average = sum / 1000.0;
			if (!Contain(stopWords, word)) {
				root = Insert(root, word, count);
			}
			count++;
		}
	}
	//double max = 0, min = 1000, average = 0, sum = 0, test;
	//for (int i = 0; i < 1000; i++) {
	//	clock_t start, end;
	//	double duration;
	//	start = clock();
	//	root = Insert(root, "the", count);
	//	end = clock();
	//	duration = ((double)end - start) / CLOCKS_PER_SEC;
	//	if (duration > max) {
	//		max = duration;
	//	}
	//	if (duration < min) {
	//		min = duration;
	//	}
	//	sum += duration;
	//	std::cout << i << ": " << duration << std::endl;
	//}
	//average = sum / 1000.0;
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
	in.close();
	return words;
}
//
//Use LinkedList
LinkedList* GetWordsFromFile(std::string fileName, AVLWordNode* stopWordsTree) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw "Can't open " + fileName;
	}
	LinkedList* lst = new LinkedList;
	std::string line;

	int count = 1;
	while (in >> line) {
		StringTolowerAndRemoveCharacter(line, true);
		if (line != "") {
			if (!Contain(stopWordsTree, line))
				lst->AddTail(new SNode(line, count));
			count++;
		}
	}
	in.close();
	return lst;
}