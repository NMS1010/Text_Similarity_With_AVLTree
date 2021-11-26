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

int GetNumberOfTextContainWord(AVLWordNode* allWords1,
	AVLWordNode* allWords2,const std::string& word) {

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

double Cosine(const std::vector<double>& text1, const std::vector<double>& text2) {
	double a = 0, b = 0, c = 0;
	int size = std::max(text1.size(), text2.size());
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

void GetWords(AVLDifferNode*& root, AVLWordNode* tree) {
	if (!tree) return;
	root = Insert(root, tree->word);
	GetWords(root, tree->left);
	GetWords(root, tree->right);
}

AVLDifferNode* GetWordsNotDuplicatedBetweenTwoText(AVLWordNode* allWordsText1,
	AVLWordNode* allWordsText2) {

	AVLDifferNode* root = nullptr;

	GetWords(root, allWordsText1);
	GetWords(root, allWordsText2);

	return root;
}

void GetWeightOfVectorText(AVLWordNode* allWords1, AVLWordNode* allWords2, 
	AVLDifferNode*& vectorWeight, AVLWordNode* root, const int& numberOfText) {

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
	return (double)a / (std::sqrt(b*c));
}

double GetSimOfOrderedVector(std::vector<int> orderWord1,
	std::vector<int> orderWord2) {

	double a = 0, b = 0;
	int size = std::max(orderWord1.size(),orderWord2.size());
	double x, y;
	for (int i = 0; i < size; i++) {
		x = i >= orderWord1.size() ? 0 : orderWord1[i];
		y = i >= orderWord2.size() ? 0 : orderWord2[i];

		double temp1 = (double)y - x;
		double temp2 = (double)y + x;
		a += temp1 * temp1;
		b += temp2 * temp2;
	}
	return 1.0 - std::sqrt((double)a / b);
}

//Word Unit
double Get_Sim_Between_Two_Text_With_Word_Unit(AVLWordNode* textTree1, AVLWordNode* textTree2) {

	AVLDifferNode* weightVector1 = GetWordsNotDuplicatedBetweenTwoText(textTree1, textTree2);
	AVLDifferNode* weightVector2 = GetWordsNotDuplicatedBetweenTwoText(textTree1, textTree2);

	GetWeightOfVectorText(textTree1, textTree2, weightVector1, textTree1, 2);
	GetWeightOfVectorText(textTree1, textTree2, weightVector2, textTree2, 2);

	return Cosine(weightVector1, weightVector2);
}

void GetOrder(AVLWordNode* wordsTree, std::vector<int>& orderVector,
	AVLDifferNode* commonWordVector) {

	if (!commonWordVector) return;
	orderVector.push_back(FindOrder(wordsTree, commonWordVector->word));
	GetOrder(wordsTree, orderVector, commonWordVector->left);
	GetOrder(wordsTree, orderVector, commonWordVector->right);
}

std::vector<int> GetOrderVector(AVLWordNode* wordsTree,
	AVLDifferNode* commonWordVector) {

	std::vector<int> orderVector;
	GetOrder(wordsTree, orderVector, commonWordVector);
	return orderVector;
}

double Get_Sim_Between_Two_Text_With_Word_Unit_And_Order(AVLWordNode* textTree1, AVLWordNode* textTree2) {

	AVLDifferNode* commonWordVector = GetWordsNotDuplicatedBetweenTwoText(textTree1, textTree2);

	std::vector<int> weightOrderVector1 = GetOrderVector(textTree1, commonWordVector);
	std::vector<int> weightOrderVector2 = GetOrderVector(textTree2, commonWordVector);

	return ( 0.5 * Get_Sim_Between_Two_Text_With_Word_Unit(textTree1, textTree2) + 
		0.5 * GetSimOfOrderedVector(weightOrderVector1, weightOrderVector2) );
}
//End Word Unit

//Sentence Unit
//1
/*
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

void GetSumSimilarityEachText(AVLSentenceNode* textTree, AVLSentenceNode* textTreeSample,
	double &sum, int &numberSent, std::vector<double>& orderSent) {

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

	return 0.8*temp1 + 0.2*temp2;
}
*/
//2
double Get_Sim_Between_Two_Text_With_Sentence_Unit(std::vector<AVLWordNode*> textTree1, std::vector<AVLWordNode*> textTree2) {
	int len1 = textTree1.size();
	int len2 = textTree2.size();
	
	double** simMatrix = new double* [len1];
	for (int i = 0; i < len1; i++) {
		simMatrix[i] = new double[len2];
	}

	std::vector<double> orderVec1;
	std::vector<double> orderVec2;

	double sumSimText1 = 0;
	double sumSimText2 = 0;
	double max1;
	double max2;

	for (int i = 0; i < len1; i++) {
		max1 = 0.0;
		for (int j = 0; j < len2; j++) {
			simMatrix[i][j] = Get_Sim_Between_Two_Text_With_Word_Unit_And_Order(textTree1[i], textTree2[j]);
			if (simMatrix[i][j] > max1) {
				max1 = simMatrix[i][j];
			}
			if (simMatrix[i][j] == 1) {
				orderVec1.push_back(j);
			}
		}
		sumSimText1 += max1;
	}

	for (int j = 0; j < len2; j++) {
		max2 = 0.0;
		for (int i = 0; i < len1; i++) {
			if (simMatrix[i][j] > max2) {
				max2 = simMatrix[i][j];
			}
			if (simMatrix[i][j] == 1) {
				orderVec2.push_back(i);
			}
		}
		sumSimText2 += max2;
	}


	int total = std::max(len1, len2);
	double sim = (sumSimText1 + sumSimText2) / (2.0 * total);
	double orderCosine = Cosine(orderVec1, orderVec2);

	if (sim == 1) {
		return orderCosine;
	}
	return sim * 0.8 + orderCosine * 0.2;
}
