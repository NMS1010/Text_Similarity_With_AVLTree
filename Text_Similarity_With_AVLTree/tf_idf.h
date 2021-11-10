#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "AVLTree.h"

double TF(int numberOfOccur, int totalWord) {
	if (totalWord == 0) return 0;
	return (double)numberOfOccur / totalWord;
}

double IDF(int numberTextHaveWord, int numberOfText) {
	return 1 + std::log10((double)numberOfText / numberTextHaveWord);
}

int GetNumberOfTextContainWord(AVLNode* allWords1, AVLNode* allWords2, std::string word) {
	int count = 0;

	if (Contain(allWords1, word)) {
		count++;
	}

	if (Contain(allWords2, word)) {
		count++;
	}

	return count;
}

int GetNumberOfOccurWord(AVLNode* root, std::string word) {
	if (!root) return 0;

	if (word.compare(root->word) < 0) {
		return GetNumberOfOccurWord(root->left, word);
	}
	else if (word.compare(root->word) > 0) {
		return GetNumberOfOccurWord(root->right, word);
	}
	else {
		return root->count;
	}
}

void GetWords(AVLDifferNode*& root, AVLNode* tree) {
	if (!tree) return;
	root = Insert(root, tree->word);
	GetWords(root, tree->left);
	GetWords(root, tree->right);
}

AVLDifferNode* GetWordsNotDuplicatedBetweenTwoText(AVLNode* allWordsText1, AVLNode* allWordsText2) {
	AVLDifferNode* root = nullptr;

	GetWords(root, allWordsText1);
	GetWords(root, allWordsText2);

	return root;
}

void GetWeightOfVectorText(AVLNode* allWords1, AVLNode* allWords2, AVLDifferNode*& vectorWeight, AVLNode* root, int numberOfText) {
	if (!vectorWeight) return;
	double tf = TF(GetNumberOfOccurWord(root, vectorWeight->word), TotalSize(root));
	double idf = IDF(GetNumberOfTextContainWord(allWords1, allWords2, vectorWeight->word), numberOfText);
	vectorWeight->weight = tf * idf;
	GetWeightOfVectorText(allWords1, allWords2, vectorWeight->left, root, numberOfText);
	GetWeightOfVectorText(allWords1, allWords2, vectorWeight->right, root, numberOfText);
}

void Calculate(AVLDifferNode* vectorWeight1, AVLDifferNode* vectorWeight2, double &a, double &b, double &c) {
	if (!vectorWeight1 || !vectorWeight2) return;

	a += (vectorWeight1->weight * vectorWeight2->weight);
	b += (vectorWeight1->weight * vectorWeight1->weight);
	c += (vectorWeight2->weight * vectorWeight2->weight);

	Calculate(vectorWeight1->left, vectorWeight2->left, a, b, c);
	Calculate(vectorWeight1->right, vectorWeight2->right, a, b, c);
}

double Cosine(AVLDifferNode* vectorWeight1, AVLDifferNode* vectorWeight2) {
	double a = 0, b = 0, c = 0;
	Calculate(vectorWeight1, vectorWeight2, a, b, c);
	return (double)a / (std::sqrt(b) * std::sqrt(c));
}

double GetSimOfOrderedVector(std::vector<int> orderWord1, std::vector<int> orderWord2) {
	double a = 0, b = 0;
	int size = std::max(orderWord1.size(),orderWord2.size());

	for (int i = 0; i < size; i++) {
		if (i >= orderWord1.size()) {
			orderWord1.push_back(0);
		}
		if (i >= orderWord2.size()) {
			orderWord2.push_back(0);
		}
		a += ((double)orderWord2[i] - orderWord1[i]) * ((double)orderWord2[i] - orderWord1[i]);
		b += ((double)orderWord2[i] + orderWord1[i]) * ((double)orderWord2[i] + orderWord1[i]);
	}
	return 1 - sqrt((double)a / b);
}

double GetSimBetweenTwoText(AVLNode* textTree1, AVLNode* textTree2) {
	std::vector<AVLDifferNode*> weightVector(2);
	weightVector[0] = GetWordsNotDuplicatedBetweenTwoText(textTree1, textTree2);
	weightVector[1] = GetWordsNotDuplicatedBetweenTwoText(textTree1, textTree2);

	GetWeightOfVectorText(textTree1, textTree2, weightVector[0], textTree1, 2);
	GetWeightOfVectorText(textTree1, textTree2, weightVector[1], textTree2, 2);

	return Cosine(weightVector[0], weightVector[1]);
}

void GetOrder(AVLNode* wordsTree, std::vector<int>& orderVector) {
	if (!wordsTree) return;
	int size = wordsTree->order.size();
	for (int i = 0; i < size; i++) {
		orderVector.push_back(wordsTree->order[i]);
	}
	GetOrder(wordsTree->left, orderVector);
	GetOrder(wordsTree->right, orderVector);
}

std::vector<int> GetOrderVector(AVLNode* wordsTree) {
	std::vector<int> orderVector;
	GetOrder(wordsTree, orderVector);
	return orderVector;
}

double GetSimBetweenTwoTextWithOrder(AVLNode* textTree1, AVLNode* textTree2) {
	std::vector<std::vector<int>> weightOrderVector(2);
	
	weightOrderVector[0] = GetOrderVector(textTree1);
	weightOrderVector[1] = GetOrderVector(textTree2);

	return ( 0.5 * GetSimBetweenTwoText(textTree1, textTree2) + 0.5 * GetSimOfOrderedVector(weightOrderVector[0], weightOrderVector[1]) );
}