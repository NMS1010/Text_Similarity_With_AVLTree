#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

#include "AVLTree.h"
#include "fileProcessing.h"
#include "normalize.h"
#include "tf_idf.h"


void Intro() {

}

int DisplayListFileNameAndSelect(std::vector<std::string> listInputFileName) {
	system("cls");
	Intro();
	std::cout << "List file text entered: " << std::endl;
	for (int i = 0; i < listInputFileName.size(); i++) {
		std::cout << i + 1 << ". " << listInputFileName[i] << std::endl;
	}
again:
	std::cout << "Choose file text you need to compare (enter its order): ";
	int select; std::cin >> select;
	if (select > listInputFileName.size()) {
		goto again;
	}
	//std::cout << "Cac file con lai duoc dung lam mau de so sanh voi file ban chon\n" << std::endl;
	return select - 1;
}

void DisplaySimilarity(std::vector<double> sim, std::vector<std::string> listInputFileName, int select) {
	system("cls");
	std::cout << "Document similarity:" << std::endl;
	for (int i = 0; i < sim.size(); i++) {
		std::cout << listInputFileName[select] << " VS " << listInputFileName[i] << " : " << sim[i] * 100 << "%" << std::endl;
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
// Word Unit
std::vector<AVLWordNode*> GetWordsOnEachText(int numberOfFile, AVLWordNode* stopWords, std::vector<std::string> listInputFileName) {
	std::vector<AVLWordNode*> wordsTree(numberOfFile);
	for (int i = 0; i < numberOfFile; i++) {
		try {
			wordsTree[i] = GetAllWordFromFile(listInputFileName[i], stopWords);
		}
		catch(std::string e){
			throw e;
		}
	}
	return wordsTree;
}


std::vector<std::string> GetListFileName(int numberOfFile) {
	std::string inputFileName;
	std::vector<std::string> listInputFileName;
	for (int i = 0; i < numberOfFile; i++) {
		std::cout << "Enter your file name " << i + 1 << "(default extension is .txt, you don't need to enter it) :";
		std::cin >> inputFileName;
		listInputFileName.push_back(inputFileName + ".txt");
	}
	return listInputFileName;
}

void Calculate_Similarity_With_Word_Unit(AVLWordNode* stopWordsTree, int numberOfFile, std::vector<std::string> listInputFileName) {
	
	try {
		std::vector<AVLWordNode*> allWordsTree = GetWordsOnEachText(numberOfFile, stopWordsTree, listInputFileName);
		while (true) {
			int select = DisplayListFileNameAndSelect(listInputFileName);

			bool isOrder = true;

			std::vector<double> sim(numberOfFile);
			for (int i = 0; i < numberOfFile; i++) {
				sim[i] = isOrder ? Get_Sim_Between_Two_Text_With_Word_Unit_And_Order(allWordsTree[select], allWordsTree[i]) : Get_Sim_Between_Two_Text_With_Word_Unit(allWordsTree[select], allWordsTree[i]);
			}

			DisplaySimilarity(sim, listInputFileName, select);
			std::cout << "\nPress ESC to return main menu\nPress any key to do again\n";
			char c = _getch();
			if (c == 27) return;
		}
	}
	catch (std::string e) {
		throw e;
	}

	
}

//End Word Unit


//Sentence Unit
std::vector<AVLSentenceNode*> GetSentencesOnEachText(int numberOfFile, std::vector<std::string> listInputFileName, AVLWordNode* stopWordsTree) {
	std::vector<AVLSentenceNode*> sentsTree(numberOfFile);
	for (int i = 0; i < numberOfFile; i++) {
		try {
			sentsTree[i] = GetAllSentenceFromFile(listInputFileName[i], stopWordsTree);
		}
		catch (std::string e) {
			throw e;
		}
	}
	return sentsTree;
}

void Calculate_Similarity_With_Sentence_Unit(AVLWordNode* stopWordsTree,int numberOfFile,std::vector<std::string> listInputFileName) {
	try {
		std::vector<AVLSentenceNode*> allSentTree = GetSentencesOnEachText(numberOfFile, listInputFileName, stopWordsTree);

		while (true) {
			int select = DisplayListFileNameAndSelect(listInputFileName);
			std::vector<double> sim(listInputFileName.size());
			sim[select] = 1;
			for (int i = 0; i < numberOfFile; i++) {
				if(i != select)
					sim[i] = Get_Sim_Between_Two_Text_With_Sentence_Unit(allSentTree[select], allSentTree[i]);
			}

			DisplaySimilarity(sim, listInputFileName, select);
			std::cout << "\nPress ESC to return main menu\nPress any key to do again\n";
			char c = _getch();
			if (c == 27) return;
		}
	}
	catch (std::string e) {
		throw e;
	}
}

void Start() {
	/*
	clock_t start, end;
	double duration;
	start = clock();
	*/

	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");

	while (true) {
		system("cls");
		std::cout << "Enter number of file: ";
		int numberOfFile;
		std::cin >> numberOfFile;
		std::cout << std::endl;
		again:
		std::vector<std::string> listInputFileName = GetListFileName(numberOfFile);
		menu:
		system("cls");
		std::cout << "What unit do you want to use?\n\t1.Word\n\t2.Sentence\n";
		int select; std::cin >> select;
		try {
			if (select == 1)
				Calculate_Similarity_With_Word_Unit(stopWordsTree, numberOfFile, listInputFileName);
			else if (select == 2)
				Calculate_Similarity_With_Sentence_Unit(stopWordsTree, numberOfFile, listInputFileName);
		}
		catch (std::string e) {
			std::cout << e << std::endl << " Please enter correctly your file name." << std::endl;
			_getch();
			system("cls");
			goto again;
		}
		std::cout << "\nPress ESC to exit\nPress any key to do again\n";
		char c = _getch();
		if (c == 27) return;
		else {
			goto menu;
		}
	}







	/*
	Calculate_Similarity_With_Sentence_Unit();
	end = clock();
	duration = ((double)end - start) / CLOCKS_PER_SEC;
	std::cout << duration;
	*/

}
