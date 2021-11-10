#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "AVLTree.h"
#include "normalize.h"
AVLNode* GetAllWordFromFile(const std::string& fileName) {
	std::ifstream in(fileName);
	if (!in.is_open()) {
		throw "Can't open " + fileName;
	}
	AVLNode* root = nullptr;
	std::string word;
	int count = 1;
	while (in >> word) {
		StringTolowerAndRemoveCharacter(word);
		if (word != "") {
			root = Insert(root, word, count);
			count++;
		}
	}

	in.close();
	return root;
}
/*
void WriteTree(AVLNode* root, std::ostream& os) {
	if (root) {
		WriteTree(root->left, os);
		os << root->word << std::endl;
		WriteTree(root->right, os);
	}
}

void Write(AVLNode* root) {
	std::ofstream ou("test.txt");
	WriteTree(root, ou);
	ou.close();
}
*/
