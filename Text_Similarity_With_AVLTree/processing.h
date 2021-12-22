#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

#include "AVLTree.h"
#include "fileProcessing.h"
#include "normalize.h"
#include "tf_idf.h"


int DisplayListFileNameAndSelect(std::vector<std::string> listInputFileName, std::vector<AVLWordNode*> allWordsTree) {
	system("cls");
	std::cout << "List file text entered: " << std::endl;
	for (int i = 0; i < listInputFileName.size(); i++) {
		std::cout << i + 1 << ". " << listInputFileName[i] << "(Total Words: " << TotalSize(allWordsTree[i]) << ")" << std::endl;
	}
again:
	std::cout << "Choose file text you want to compare (enter its order): ";
	int select; std::cin >> select;
	if (select > listInputFileName.size()) {
		goto again;
	}
	return select - 1;
}
int DisplayListFileNameAndSelect(std::vector<std::string> listInputFileName, std::vector<std::vector<std::string>> allWords) {
	system("cls");
	std::cout << "List file text entered: " << std::endl;
	for (int i = 0; i < listInputFileName.size(); i++) {
		std::cout << i + 1 << ". " << listInputFileName[i] << "(Total Words: " << allWords[i].size() << ")" << std::endl;
	}
again:
	std::cout << "Choose file text you want to compare (enter its order): ";
	int select; std::cin >> select;
	if (select > listInputFileName.size()) {
		goto again;
	}
	return select - 1;
}
int GetNumberOfWordsEachSent(std::vector<AVLWordNode*> Sent) {
	int count = 0;
	int len = Sent.size();
	for (int i = 0; i < len; i++) {
		count += TotalSize(Sent[i]);
	}
	return count;
}

int DisplayListFileNameAndSelect(std::vector<std::string> listInputFileName, std::vector<std::vector<AVLWordNode*>> allSentTree) {
	system("cls");
	std::cout << "List file text entered: " << std::endl;
	for (int i = 0; i < listInputFileName.size(); i++) {
		std::cout << i + 1 << ". " << listInputFileName[i] << "(Total Sent: " << allSentTree[i].size() << ", Total Words: " << GetNumberOfWordsEachSent(allSentTree[i]) << ")" << std::endl;
	}
again:
	std::cout << "Choose file text you want to compare (enter its order): ";
	int select; std::cin >> select;
	if (select > listInputFileName.size()) {
		goto again;
	}
	return select - 1;
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

void DisplaySimilarity(std::vector<double> sim, std::vector<std::string> listInputFileName, int select) {
	system("cls");
	std::cout << "Document similarity:" << std::endl;
	int posmax = -1;
	double maxVal = -1;
	for (int i = 0; i < sim.size(); i++) {
		std::cout << listInputFileName[select] << " VS " << listInputFileName[i] << " : " << sim[i] * 100 << "%" << std::endl;
		if (i != select && sim[i] > maxVal) {
			posmax = i;
			maxVal = sim[i];
		}
	}
	if (posmax == -1) return;
	std::cout << "\n" << "===> " << listInputFileName[posmax] << " is the most similar to " << listInputFileName[select] << " with " << maxVal * 100 << "% (except itself)" << std::endl;
}

// Word Unit
std::vector<AVLWordNode*> GetWordsOnEachText(int numberOfFile, AVLWordNode* stopWords, std::vector<std::string> listInputFileName) {
	std::vector<AVLWordNode*> wordsTree(numberOfFile);
	for (int i = 0; i < numberOfFile; i++) {
		try {
			wordsTree[i] = GetAllWordFromFile(listInputFileName[i], stopWords);
		}
		catch (std::string e) {
			throw e;
		}
	}
	return wordsTree;
}

void Calculate_Similarity_With_Word_Unit_Use_AVLTree(AVLWordNode* stopWordsTree, int numberOfFile, std::vector<std::string> listInputFileName) {

	try {
		std::vector<AVLWordNode*> allWordsTree = GetWordsOnEachText(numberOfFile, stopWordsTree,
			listInputFileName);
		while (true) {
			int select = DisplayListFileNameAndSelect(listInputFileName, allWordsTree);

			std::vector<double> sim(numberOfFile);
			clock_t start, end;
			double duration;
			start = clock();
			sim[select] = 1;
			for (int i = 0; i < numberOfFile; i++) {
				if(i!=select)
					sim[i] = Get_Sim_Between_Two_Text_With_Word_Unit_And_Order(allWordsTree[select], allWordsTree[i]);
			}
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			DisplaySimilarity(sim, listInputFileName, select);
			std::cout << "\nElapsed Time: " << duration << "s" << std::endl;
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
std::vector<std::vector<AVLWordNode*>>  GetSentencesOnEachText(int numberOfFile, std::vector<std::string> listInputFileName, AVLWordNode* stopWordsTree) {

	std::vector<std::vector<AVLWordNode*>> sentsTree(numberOfFile);

	for (int i = 0; i < numberOfFile; i++) {
		try {
			sentsTree[i] = GetVectorSent(listInputFileName[i], stopWordsTree);
		}
		catch (std::string e) {
			throw e;
		}
	}
	return sentsTree;
}

void Calculate_Similarity_With_Sentence_Unit(AVLWordNode* stopWordsTree, int numberOfFile, std::vector<std::string> listInputFileName) {
	try {
		std::vector<std::vector<AVLWordNode*>> allSentTree = GetSentencesOnEachText(numberOfFile, listInputFileName, stopWordsTree);

		while (true) {
			int select = DisplayListFileNameAndSelect(listInputFileName, allSentTree);
			std::vector<double> sim(listInputFileName.size());
			sim[select] = 1;
			clock_t start, end;
			double duration;
			start = clock();
			for (int i = 0; i < numberOfFile; i++) {
				if (i != select)
					sim[i] = Get_Sim_Between_Two_Text_With_Sentence_Unit(allSentTree[select], allSentTree[i]);
			}
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;

			DisplaySimilarity(sim, listInputFileName, select);
			std::cout << "\nElapsed Time: " << duration << "s" << std::endl;
			std::cout << "\nPress ESC to return main menu\nPress any key to do again\n";
			char c = _getch();
			if (c == 27) return;
		}

	}
	catch (std::string e) {
		throw e;
	}
}
//End Sentence Unit

//Use Vector Structure
std::vector<std::vector<std::string>> GetWordsOnEachText(int numberOfFile, std::vector<std::string> stopWords, std::vector<std::string> listInputFileName) {
	std::vector<std::vector<std::string>> allWords(numberOfFile);
	for (int i = 0; i < numberOfFile; i++) {
		try {
			allWords[i] = GetAllWordFromFile(listInputFileName[i], stopWords);
		}
		catch (std::string e) {
			throw e;
		}
	}
	return allWords;
}
void Calculate_Similarity_With_Word_Unit_Use_Vector(std::vector<std::string> stopwords, int numberOfFile, std::vector<std::string> listInputFileName) {
	try {
		std::vector<std::vector<std::string>> allWords = GetWordsOnEachText(numberOfFile, stopwords,
			listInputFileName);
		while (true) {
			int select = DisplayListFileNameAndSelect(listInputFileName, allWords);

			std::vector<double> sim(numberOfFile);
			clock_t start, end;
			double duration;
			start = clock();
			sim[select] = 1;
			for (int i = 0; i < numberOfFile; i++) {
				if (i != select)
					sim[i] = Get_Sim_Between_Two_Text_With_Word_Unit_And_Order(allWords[select], allWords[i],stopwords);
			}
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			DisplaySimilarity(sim, listInputFileName, select);
			std::cout << "\nElapsed Time: " << duration << "s" << std::endl;
			std::cout << "\nPress ESC to return main menu\nPress any key to do again\n";
			char c = _getch();
			if (c == 27) return;
		}
	}
	catch (std::string e) {
		throw e;
	}
}
//End Use Vector Structure
void Start() {

	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
	std::vector<std::string> stopwords = GetStopWords("stopwords.txt");

	while (true) {
	enter:
		system("cls");
		std::cout << "Enter number of file: ";
		int numberOfFile;
		std::cin >> numberOfFile;
		if (numberOfFile <= 1) {
			std::cout << "Please enter number is greater than 1\n";

			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');

			char c =_getch();
			goto enter;
		}
		std::cout << std::endl;
	again:
		std::vector<std::string> listInputFileName = GetListFileName(numberOfFile);
	menu:
		system("cls");
		std::cout << "What unit do you want to use?\n\t1.Word\n\t2.Sentence\n";
		int select; std::cin >> select;
		try {
			if (select == 1) {
				system("cls");
				std::cout << "What data structure do you want to use?\n\t1.Vector\n\t2.AVL Tree\n";
				int options; std::cin >> options;
				if (options == 2)
					Calculate_Similarity_With_Word_Unit_Use_AVLTree(stopWordsTree, numberOfFile, listInputFileName);
				else if (options == 1)
					Calculate_Similarity_With_Word_Unit_Use_Vector(stopwords, numberOfFile, listInputFileName);
			}
			else if (select == 2)
				Calculate_Similarity_With_Sentence_Unit(stopWordsTree, numberOfFile, listInputFileName);
		}
		catch (std::string e) {
			std::cout << e << std::endl << " Please enter correctly your file name." << std::endl;
			char c = _getch();
			system("cls");
			goto again;
		}
		std::cout << "\nPress ESC to exit\nPress any key to do again\nPress Space to enter another file";
		char c = _getch();
		if (c == 27) return;
		else if (c == 32) goto enter;
		else {
			goto menu;
		}
	}
}
