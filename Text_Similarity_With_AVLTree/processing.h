#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

#include "AVLTree.h"
#include "fileProcessing.h"
#include "normalize.h"
#include "tf_idf.h"

std::vector<AVLNode*> GetWordsOnEachText(int numberOfFile, AVLNode* stopWords, std::vector<std::string>& listInputFileName) {
	std::vector<AVLNode*> wordsTree(numberOfFile);
	std::string inputFileName;
	for (int i = 0; i < numberOfFile; i++) {
		again:
		try {
			inputFileName = "";
			std::cout << "Nhap ten van ban thu " << i + 1 << "(bao gom ca phan extension) :";
			std::cin >> inputFileName;
			wordsTree[i] = GetAllWordFromFile(inputFileName);
			Normalize(wordsTree[i], stopWords);
			listInputFileName.push_back(inputFileName);
		}
		catch(std::string e){
			std::cout << e << std::endl;
			goto again;
		}
	}
	return wordsTree;
}


void Intro() {

}

int DisplayListFileNameAndSelect(std::vector<std::string> listInputFileName) {
	system("cls");
	Intro();
	std::cout << "Danh sach cac file text da nhap" << std::endl;
	for (int i = 0; i < listInputFileName.size(); i++) {
		std::cout << i + 1 << ". " << listInputFileName[i] << std::endl;
	}
	again:
	std::cout << "Chon file text can so sanh (Nhap so thu tu): ";
	int select; std::cin >> select;
	if (select > listInputFileName.size()) {
		goto again;
	}
	std::cout << "Cac file con lai duoc dung lam mau de so sanh voi file ban chon\n" << std::endl;
	return select - 1;
}

void DisplaySimilarity(std::vector<double> sim, std::vector<std::string> listInputFileName, int select) {
	system("cls");
	std::cout << "Do giong nhau giua tung van ban:" << std::endl;
	for (int i = 0; i < sim.size(); i++) {
		std::cout << listInputFileName[select] << " VS " << listInputFileName[i] << " : " << sim[i] * 100 <<"%"<< std::endl;
	}
}

bool GetOpinion() {
	bool isOrder;
	Again:
	std::cout << "Ban co muon so khop ve mat thu tu? (Y/N): ";
	char c;
	std::cin >> c;
	if (c == 'Y' || c == 'y') isOrder = true;
	else if (c == 'N' || c == 'n') isOrder = false;
	else {
		std::cout << "Ki tu khong hop le! Vui long chon lai\n";
		goto Again;
	}
	system("cls");
	return isOrder;
}

void Calculate_Similarity_With_Word_Unit(AVLNode* stopWordsTree, int numberOfFile) {
	
	std::vector<std::string> listInputFileName;

	std::vector<AVLNode*> allWordsTree = GetWordsOnEachText(numberOfFile, stopWordsTree, listInputFileName);

	while (true) {
		int select = DisplayListFileNameAndSelect(listInputFileName);

		bool isOrder = GetOpinion();

		std::vector<double> sim(numberOfFile);
		for (int i = 0; i < numberOfFile; i++) {
			sim[i] = isOrder ? GetSimBetweenTwoTextWithOrder(allWordsTree[select], allWordsTree[i]) : GetSimBetweenTwoText(allWordsTree[select], allWordsTree[i]);
		}

		DisplaySimilarity(sim, listInputFileName, select);
		std::cout << "\nPress ESC to exit\nPress any key to do again\n";
		char c =_getch();
		if (c == 27) return;
	}
}

void Calculate_Similarity_With_Sentence_Unit() {
	
}

void Start() {
	/*
	clock_t start, end;
	double duration;
	start = clock();
	*/

	AVLNode* stopWordsTree = GetAllWordFromFile("stopwords.txt");

	
	std::cout << "Chon so van ban can so khop: ";
	int numberOfFile;
	std::cin >> numberOfFile;
	std::cout << std::endl;
	Calculate_Similarity_With_Word_Unit(stopWordsTree, numberOfFile);

	







	/*
	Calculate_Similarity_With_Sentence_Unit();
	end = clock();
	duration = ((double)end - start) / CLOCKS_PER_SEC;
	std::cout << duration;
	*/

}
