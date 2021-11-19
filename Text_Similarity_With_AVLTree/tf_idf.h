#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "stack.h"
#include "AVLTree.h"


double TF(const int &numberOfOccur, const int &totalWord) {
	if (totalWord == 0) return 0;
	return (double)numberOfOccur / totalWord;
}

double IDF(const int& numberTextHaveWord, const int& numberOfText) {
	if (numberTextHaveWord == 0) return 0;
	return 1 + std::log10((double)numberOfText / numberTextHaveWord);
}

int GetNumberOfTextContainWord(AVLWordNode* allWords1,AVLWordNode* allWords2,const std::string& word) {
	int count = 0;

	if (Contain(allWords1, word)) {
		count++;
	}

	if (Contain(allWords2, word)) {
		count++;
	}

	return count;
}

int GetNumberOfOccurWord(AVLWordNode*& root,const std::string& word) {
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


void GetWords(AVLDifferNode*& root, AVLWordNode* tree) {
	if (!tree) return;
	root = Insert(root, tree->word);
	GetWords(root, tree->left);
	GetWords(root, tree->right);
}

AVLDifferNode* GetWordsNotDuplicatedBetweenTwoText(AVLWordNode* allWordsText1, AVLWordNode* allWordsText2) {
	AVLDifferNode* root = nullptr;

	GetWords(root, allWordsText1);
	GetWords(root, allWordsText2);

	return root;
}

void GetWeightOfVectorText(AVLWordNode* allWords1, AVLWordNode* allWords2, AVLDifferNode*& vectorWeight, AVLWordNode* root, const int& numberOfText) {
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
	double x, y;
	for (int i = 0; i < size; i++) {
		if (i >= orderWord1.size()) {
			x = 0;
		}
		else {
			x = orderWord1[i];
		}
		if (i >= orderWord2.size()) {
			y = 0;
		}
		else {
			y = orderWord2[i];
		}
		a += ((double)y - x) * ((double)y - x);
		b += ((double)y + x) * ((double)y + x);
	}
	return 1 - sqrt((double)a / b);
}

//Word Unit
double Get_Sim_Between_Two_Text_With_Word_Unit(AVLWordNode* textTree1, AVLWordNode* textTree2) {

	AVLDifferNode* weightVector1 = GetWordsNotDuplicatedBetweenTwoText(textTree1, textTree2);
	AVLDifferNode* weightVector2 = GetWordsNotDuplicatedBetweenTwoText(textTree1, textTree2);

	GetWeightOfVectorText(textTree1, textTree2, weightVector1, textTree1, 2);
	GetWeightOfVectorText(textTree1, textTree2, weightVector2, textTree2, 2);

	return Cosine(weightVector1, weightVector2);
}

void GetOrder(AVLWordNode* wordsTree, std::vector<int>& orderVector, AVLDifferNode* commonWordVector) {
	if (!commonWordVector) return;
	orderVector.push_back(FindOrder(wordsTree, commonWordVector->word));
	GetOrder(wordsTree, orderVector, commonWordVector->left);
	GetOrder(wordsTree, orderVector, commonWordVector->right);
}

std::vector<int> GetOrderVector(AVLWordNode* wordsTree, AVLDifferNode* commonWordVector) {
	std::vector<int> orderVector;
	GetOrder(wordsTree, orderVector, commonWordVector);
	return orderVector;
}

double Get_Sim_Between_Two_Text_With_Word_Unit_And_Order(AVLWordNode* textTree1, AVLWordNode* textTree2) {

	AVLDifferNode* commonWordVector = GetWordsNotDuplicatedBetweenTwoText(textTree1, textTree2);

	std::vector<int> weightOrderVector1 = GetOrderVector(textTree1, commonWordVector);
	std::vector<int> weightOrderVector2 = GetOrderVector(textTree2, commonWordVector);

	return ( 0.5 * Get_Sim_Between_Two_Text_With_Word_Unit(textTree1, textTree2) + 0.5 * GetSimOfOrderedVector(weightOrderVector1, weightOrderVector2) );
}
//End Word Unit

//Sentence Unit

double Cosine(const std::vector<double>& text1, const std::vector<double>& text2) {
	double a = 0, b = 0, c = 0;
	int size = std::max(text1.size(),text2.size());
	double x, y;
	for (int i = 0; i < size; i++) {
		x = i < text1.size() ? text1[i] : 0;
		y = i < text2.size() ? text2[i] : 0;
		a += (x * y);
		b += (x * x);
		c += (y * y);
	}
	if (a == 0) return 0;
	return (double)a / (sqrt(b) * sqrt(c));
}
/*
void Calculate(AVLDifferNode* vectorWeight1, AVLDifferNode* vectorWeight2, double& weight) {
	if (!vectorWeight1 || !vectorWeight2) return;

	weight += (vectorWeight1->weight * vectorWeight2->weight);

	Calculate(vectorWeight1->left, vectorWeight2->left, weight);
	Calculate(vectorWeight1->right, vectorWeight2->right, weight);
}
double GetSimBetweenTwoSentence(AVLWordNode* sent1, AVLWordNode* sent2) {

	AVLDifferNode* weightVector1 = GetWordsNotDuplicatedBetweenTwoText(sent1, sent2);
	AVLDifferNode* weightVector2 = GetWordsNotDuplicatedBetweenTwoText(sent1, sent2);

	GetWeightOfVectorText(sent1, sent2, weightVector1, sent1, 2);
	GetWeightOfVectorText(sent1, sent2, weightVector2, sent2, 2);
	
	double result = 0;
	Calculate(weightVector1, weightVector2, result);

	return result;
}

void GetWeightEachSent(AVLWordNode* sentence, AVLSentenceNode* textTree, double& weightBetweenTwoSent) {
	if (!textTree) return;

	GetWeightEachSent(sentence, textTree->left, weightBetweenTwoSent);

	weightBetweenTwoSent = std::max(weightBetweenTwoSent,Get_Sim_Between_Two_Text_With_Word_Unit_And_Order(sentence, textTree->wordRoot));
	
	GetWeightEachSent(sentence, textTree->right, weightBetweenTwoSent);
		
}
*/

double GetWeightEachSent(AVLWordNode* sentence, AVLSentenceNode* textTree, std::vector<double>& orderSent) {
	Stack<AVLSentenceNode*> st;

	AVLSentenceNode* curr = textTree;
	double simBetweenTwoSent = 0;
	while (curr != nullptr || !st.Empty()) {
		while (curr != nullptr) {
			st.push(curr);
			curr = curr->left;
		}
		AVLSentenceNode* temp = st.pop();
		simBetweenTwoSent = std::max(simBetweenTwoSent, Get_Sim_Between_Two_Text_With_Word_Unit_And_Order(sentence, temp->wordRoot));
		if (simBetweenTwoSent == 1) {
			orderSent.push_back(temp->orderSentence);
			return simBetweenTwoSent;
		}
		curr = temp->right;
	}
	return simBetweenTwoSent;
}

void GetSumSimilarityEachText(AVLSentenceNode* textTree, AVLSentenceNode* textTreeSample, double &sum, int &numberSent, std::vector<double>& orderSent) {

	if (!textTree) return;
	GetSumSimilarityEachText(textTree->left, textTreeSample, sum, numberSent, orderSent);
	
	double temp = GetWeightEachSent(textTree->wordRoot, textTreeSample, orderSent);
	sum += temp;
	numberSent++;

	GetSumSimilarityEachText(textTree->right, textTreeSample, sum, numberSent, orderSent);

}


double Get_Sim_Between_Two_Text_With_Sentence_Unit(AVLSentenceNode* textTree1, AVLSentenceNode* textTree2) {
	std::vector<double> orderSent1;
	std::vector<double> orderSent2;

	double sum1 = 0;
	int numberSent1 = 0;
	GetSumSimilarityEachText(textTree1, textTree2, sum1, numberSent1, orderSent1);

	double sum2 = 0;
	int numberSent2 = 0;
	GetSumSimilarityEachText(textTree2, textTree1, sum2, numberSent2, orderSent2);

	int total = std::max(numberSent1, numberSent2);
	double temp1 = (sum1 + sum2) / (2 * (double)total);
	double temp2 = Cosine(orderSent1, orderSent2);
	if (temp1 == 1) {
		return temp2;
	}
	return 0.85*temp1 + 0.15*temp2 ;
}
