#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

#include "AVLTree.h"
#include "fileProcessing.h"
#include "normalize.h"
#include "tf_idf.h"

void GetSimSent(std::string fileDes, std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	for (int j = 0; j < size; j++) {
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;
			start = clock();
			AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
			std::vector<AVLWordNode*> textTree1 = GetVectorSent(fileDes, stopWordsTree);
			std::vector<AVLWordNode*> textTree2 = GetVectorSent(fileNames[j], stopWordsTree);
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
			AVLDifferNode* weightVector1 = nullptr;
			AVLDifferNode* weightVector2 = nullptr;
			std::vector<double> weightOrderVector1;
			std::vector<double> weightOrderVector2;
			for (int i = 0; i < len1; i++) {
				max1 = 0.0;

				for (int j = 0; j < len2; j++) {
					weightVector1 = GetWordsNotDuplicatedBetweenTwoText(textTree1[i], textTree2[j]);
					weightVector2 = GetWordsNotDuplicatedBetweenTwoText(textTree1[i], textTree2[j]);
					GetWeightOfVectorText(textTree1[i], textTree2[j], weightVector1, textTree1[i], TotalSize(textTree1[i]));
					GetWeightOfVectorText(textTree1[i], textTree2[j], weightVector2, textTree2[j], TotalSize(textTree2[j]));
					double cs = Cosine(weightVector1, weightVector2);
					weightOrderVector1 = GetOrderVector(textTree1[i], weightVector1);
					weightOrderVector2 = GetOrderVector(textTree2[j], weightVector1);
					double order = GetSimOfOrderedVector(weightOrderVector1, weightOrderVector2);
					double res = cs * 0.5 + order * 0.5;
					simMatrix[i][j] = res;
					if (simMatrix[i][j] > max1) {
						max1 = simMatrix[i][j];
					}
					if (simMatrix[i][j] == 1) {
						orderVec1.push_back(j + 1.0);
					}
					deleteTree(weightVector1);
					deleteTree(weightVector2);
					weightOrderVector1.clear();
					weightOrderVector2.clear();
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
						orderVec2.push_back(i + 1.0);
					}
				}
				sumSimText2 += max2;
			}
			int total = std::max(len1, len2);
			double sim = (sumSimText1 + sumSimText2) / (2.0 * total);
			double orderCosine = Cosine(orderVec1, orderVec2);
			end = clock();

			duration = ((double)end - start) / CLOCKS_PER_SEC;
			ReleasePointer(len1, len2, simMatrix);
			deleteTree(stopWordsTree);
			for (int i = 0; i < textTree1.size(); i++) {
				deleteTree(textTree1[i]);
			}
			for (int i = 0; i < textTree2.size(); i++) {
				deleteTree(textTree2[i]);
			}
			textTree1.clear();
			textTree2.clear();
			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
	}
	WriteResult(time, "timeGetSimSent.txt");
}
void GetSent(std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
	for (int j = 0; j < size; j++) {
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;
			start = clock();
			std::vector<AVLWordNode*> temp = GetVectorSent(fileNames[j], stopWordsTree);
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			for (int i = 0; i < temp.size(); i++) {
				deleteTree(temp[i]);
			}
			temp.clear();
			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
	}
	WriteResult(time, "timeGetSent.txt");
}
void GetSim(std::string fileDes, std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	for (int j = 0; j < size; j++) {
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;
			start = clock();
			AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
			AVLWordNode* text1 = GetAllWordFromFile(fileDes, stopWordsTree);
			AVLWordNode* text2 = GetAllWordFromFile(fileNames[j], stopWordsTree);
			AVLDifferNode* weightVector1 = GetWordsNotDuplicatedBetweenTwoText(text1, text2);
			AVLDifferNode* weightVector2 = GetWordsNotDuplicatedBetweenTwoText(text1, text2);
			GetWeightOfVectorText(text1, text2, weightVector1, text1, TotalSize(text1));
			GetWeightOfVectorText(text1, text2, weightVector2, text2, TotalSize(text2));
			double cs = Cosine(weightVector1, weightVector2);
			std::vector<double> weightOrderVector1 = GetOrderVector(text1, weightVector1);
			std::vector<double> weightOrderVector2 = GetOrderVector(text2, weightVector1);
			double order = GetSimOfOrderedVector(weightOrderVector1, weightOrderVector2);
			double res = cs * 0.5 + order * 0.5;
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			deleteTree(stopWordsTree);
			deleteTree(text1);
			deleteTree(text2);
			deleteTree(weightVector1);
			deleteTree(weightVector2);
			weightOrderVector1.clear();
			weightOrderVector2.clear();
			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
	}
	WriteResult(time, "timeGetSim.txt");
}
void GetSimOrdered(std::string fileDes, std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
	AVLWordNode* des = GetAllWordFromFile(fileDes, stopWordsTree);
	AVLWordNode* word;
	AVLDifferNode* temp;
	double a;
	double cs;
	for (int j = 0; j < size; j++) {
		word = GetAllWordFromFile(fileNames[j], stopWordsTree);
		temp = GetWordsNotDuplicatedBetweenTwoText(des, word);
		std::vector<double> order1 = GetOrderVector(des, temp);
		std::vector<double> order2 = GetOrderVector(word, temp);
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;

			start = clock();
			a = GetSimOfOrderedVector(order1, order2);
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;

			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
		deleteTree(word);
		deleteTree(temp);
		order1.clear();
		order2.clear();
	}
	WriteResult(time, "timeGetSimOrdered.txt");
}
void GetOrdered(std::string fileDes, std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
	AVLWordNode* des = GetAllWordFromFile(fileDes, stopWordsTree);
	AVLWordNode* word;
	AVLDifferNode* temp;
	double cs;
	for (int j = 0; j < size; j++) {
		word = GetAllWordFromFile(fileNames[j], stopWordsTree);
		temp = GetWordsNotDuplicatedBetweenTwoText(des, word);
		std::vector<double> order;
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;

			start = clock();
			order = GetOrderVector(word, temp);
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;

			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
		deleteTree(word);
		deleteTree(temp);
		order.clear();
	}
	WriteResult(time, "timeGetOrdered.txt");
}
void GetCosine(std::string fileDes, std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
	AVLWordNode* des = GetAllWordFromFile(fileDes, stopWordsTree);
	AVLWordNode* word;
	AVLDifferNode* temp1;
	AVLDifferNode* temp2;
	double cs;
	for (int j = 0; j < size; j++) {
		word = GetAllWordFromFile(fileNames[j], stopWordsTree);
		temp1 = GetWordsNotDuplicatedBetweenTwoText(des, word);
		temp2 = GetWordsNotDuplicatedBetweenTwoText(des, word);
		GetWeightOfVectorText(des, word, temp1, word, 2);
		GetWeightOfVectorText(des, word, temp2, des, 2);
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;

			start = clock();
			cs = Cosine(temp1, temp2);
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;

			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
		deleteTree(word);
		deleteTree(temp1);
		deleteTree(temp2);
	}
	WriteResult(time, "timeGetCosine.txt");
}
void GetWeight(std::string fileDes, std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
	AVLWordNode* des = GetAllWordFromFile(fileDes, stopWordsTree);
	AVLWordNode* word;
	AVLDifferNode* temp;
	for (int j = 0; j < size; j++) {
		word = GetAllWordFromFile(fileNames[j], stopWordsTree);
		temp = GetWordsNotDuplicatedBetweenTwoText(des, word);
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;

			start = clock();
			GetWeightOfVectorText(des, word, temp, word, 2);
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;

			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
		deleteTree(word);
		deleteTree(temp);
	}
	WriteResult(time, "timeGetWeight.txt");
}
void GetNumberContain(std::vector<std::string> fileNames, std::string fileSearch, std::string fileDes) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
	std::vector<std::string> search = SaveVector(fileSearch);
	AVLWordNode* des = GetAllWordFromFile(fileDes, stopWordsTree);
	int temp;
	for (int j = 0; j < size; j++) {
		AVLWordNode* word = GetAllWordFromFile(fileNames[j], stopWordsTree);
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;
			start = clock();
			for (int k = 0; k < 1000; k++) {
				temp = GetNumberOfTextContainWord(des, word, search[k]);
			}
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
		deleteTree(word);
	}
	WriteResult(time, "timeGetNumberContain.txt");
}
void GetNumberOccur(std::vector<std::string> fileNames, std::string fileSearch) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
	std::vector<std::string> search = SaveVector(fileSearch);
	int temp;
	for (int j = 0; j < size; j++) {
		AVLWordNode* word = GetAllWordFromFile(fileNames[j], stopWordsTree);
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;
			start = clock();
			for (int k = 0; k < 1000; k++) {
				temp = GetNumberOfOccurWord(word, search[k]);
			}
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
		deleteTree(word);
	}
	WriteResult(time, "timeGetNumberOccur.txt");
}
void GetWordsToFindCommonWords(std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");

	AVLDifferNode* temp;
	for (int j = 0; j < size; j++) {
		AVLWordNode* word = GetAllWordFromFile(fileNames[j], stopWordsTree);
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;
			start = clock();
			temp = nullptr;
			GetWords(temp, word);
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			deleteTree(temp);

			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
		deleteTree(word);
	}
	WriteResult(time, "timeGetWordsToFindCommonWords.txt");
}
void FindCommonWords(std::string fileDes, std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");
	AVLWordNode* des = GetAllWordFromFile(fileDes, stopWordsTree);

	AVLDifferNode* temp;
	for (int j = 0; j < size; j++) {
		AVLWordNode* word = GetAllWordFromFile(fileNames[j], stopWordsTree);
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;
			start = clock();
			temp = GetWordsNotDuplicatedBetweenTwoText(des, word);
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			deleteTree(temp);
			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
		deleteTree(word);
	}
	WriteResult(time, "timeFindCommonWords.txt");
}
void ReadFile(std::vector<std::string> fileNames) {
	int size = fileNames.size();
	std::vector<std::vector<double>> time(size);
	AVLWordNode* stopWordsTree = GetStopWordsFromFile("stopwords.txt");

	AVLWordNode* temp;
	for (int j = 0; j < size; j++) {
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;
			start = clock();
			temp = GetAllWordFromFile(fileNames[j], stopWordsTree);
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			deleteTree(temp);
			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
	}
	WriteResult(time, "timeReadFileText.txt");
}
void ProcessString() {
	std::vector<std::string> a = SaveVector("chuoi.txt");
	std::vector<std::vector<double>> time(10);

	for (int j = 0; j < 10; j++) {
		double max = 0, min = 1000, average = 0, sum = 0;
		for (int i = 0; i < 1000; i++) {
			clock_t start, end;
			double duration;
			start = clock();
			StringTolowerAndRemoveCharacter(a[j], true);
			end = clock();
			duration = ((double)end - start) / CLOCKS_PER_SEC;
			if (duration > max) {
				max = duration;
			}
			if (duration < min) {
				min = duration;
			}
			sum += duration;
			std::cout << i << ": " << duration << std::endl;
		}
		average = sum / 1000.0;
		time[j].push_back(min);
		time[j].push_back(max);
		time[j].push_back(average);
	}
	WriteResult(time, "timeString.txt");
}

void CheckTime() {
	std::vector<std::string> fileNames{ "vb1.txt" ,"vb2.txt" ,"vb3.txt" ,"vb4.txt" ,"vb5.txt" };
	std::string fileDes = "example.txt";
	//ProcessString();
	//ReadFile(fileNames);
	//GetWordsToFindCommonWords(fileNames);
	//FindCommonWords(fileDes, fileNames);
	//GetNumberOccur(fileNames, "search.txt");
	//GetNumberContain(fileNames, "search.txt", fileDes);
	//GetWeight(fileDes, fileNames);
	//GetCosine(fileDes, fileNames);
	//GetOrdered(fileDes, fileNames);
	//GetSimOrdered(fileDes, fileNames);
	//GetSim(fileDes, fileNames);
	//GetSent(fileNames);
	//GetSimSent(fileDes, fileNames);

}