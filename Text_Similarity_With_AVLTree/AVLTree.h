#pragma once
#include <iostream>
#include <string>

struct AVLDifferNode {
	std::string word;
	int height;
	double weight;

	AVLDifferNode* left;
	AVLDifferNode* right;

	AVLDifferNode(std::string value) {
		word = value;
		height = 1;
		weight = 0;
		left = nullptr;
		right = nullptr;
	}

};

struct AVLWordNode {
	std::string word;
	int count;
	std::vector<int> order;

	int height;
	AVLWordNode* left;
	AVLWordNode* right;

	AVLWordNode(std::string value) {
		word = value;
		height = 1;
		count = 1;
		left = nullptr;
		right = nullptr;
	}

	AVLWordNode(std::string value, int x) {
		word = value;
		height = 1;
		count = 1;
		order.push_back(x);
		left = nullptr;
		right = nullptr;
	}
};


template<class T>
int Size(T root) {
	if (!root) return 0;
	return Size(root->right) + Size(root->left) + 1;
}

int TotalSize(AVLWordNode* root) {
	if (!root) return 0;
	return TotalSize(root->right) + TotalSize(root->left) + root->count;
}

template<class T>
int GetHeight(T root) {
	if (root) return root->height;
	return 0;
}

template<class T>
int GetMaxHeight(T root) {
	return std::max(GetHeight(root->left), GetHeight(root->right)) + 1;
}

template<class T>
T RotateRight(T root) {
	T l = root->left;
	T lr = l->right;

	l->right = root;
	root->left = lr;

	root->height = GetMaxHeight(root);
	l->height = GetMaxHeight(l);

	return l;
}

template<class T>
T RotateLeft(T root) {
	T r = root->right;
	T rl = r->left;

	r->left = root;
	root->right = rl;

	root->height = GetMaxHeight(root);
	r->height = GetMaxHeight(r);

	return r;
}

template<class T>
int GetBalanceWeight(T root) {
	if (root) return GetHeight(root->left) - GetHeight(root->right);
	return 0;
}

template<class T>
int Height(T root) {
	if (root == nullptr) {
		return 0;
	}
	return max(Height(root->left), Height(root->right)) + 1;
}

template<class T>
bool Contain(T root, std::string word) {
	if (!root) return false;

	if (word.compare(root->word) < 0) {
		return Contain(root->left, word);
	}
	else if (word.compare(root->word) > 0) {
		return Contain(root->right, word);
	}else
		return true;
}

template<class T>
void NLR(T root) {
	if (root == nullptr) return;
	std::cout << root->word << "- weight: " << root->weight << std::endl;
	NLR(root->left);
	NLR(root->right);
}

int FindOrder(AVLWordNode* root, std::string value) {
	if (!root) return 0;

	if (value.compare(root->word) < 0) {
		return FindOrder(root->left, value);
	}
	else if (value.compare(root->word) > 0) {
		return FindOrder(root->right, value);
	}
	else
		return root->order[0];
}

AVLWordNode* Insert(AVLWordNode* root, std::string value, int order) {
	if (!root) {
		return new AVLWordNode(value,order);
	}

	if(value.compare(root->word) == 0){
		(root->count)++;
		root->order.push_back(order);
		return root;
	}

	if (value.compare(root->word) < 0) {
		root->left = Insert(root->left, value, order);
	}
	else {
		root->right = Insert(root->right, value, order);
	}

	root->height = GetMaxHeight(root);

	int bal = GetBalanceWeight(root);

	//Trái - trái
	if (bal > 1 && value.compare(root->left->word) < 0) {
		return RotateRight(root);
	}
	//Phải - phải
	if (bal < -1 && value.compare(root->right->word) > 0) {
		return RotateLeft(root);
	}
	//Trái - phải
	if (bal > 1 && value.compare(root->left->word) > 0) {
		root->left = RotateLeft(root->left);
		return RotateRight(root);
	}
	//Phải - trái
	if (bal < -1 && value.compare(root->right->word) < 0) {
		root->right = RotateRight(root->right);
		return RotateLeft(root);
	}

	return root;
}

AVLWordNode* FindMostRightNodeAtSubLeftTree(AVLWordNode* root) {
	AVLWordNode* curr = root;
	while (curr->right != nullptr) {
		curr = curr->right;
	}
	return curr;
}

AVLWordNode* RemoveNode(AVLWordNode* root, std::string value) {
	if (!root) return root;
		
	if (value.compare(root->word) < 0) {
		root->left = RemoveNode(root->left, value);
	}
	else if (value.compare(root->word) > 0) {
		root->right = RemoveNode(root->right, value);
	}
	else {
		/*if (root->count > 1) {
			(root->count)--;
			return nullptr;
		}*/
		if ((root->left == nullptr) || (root->right == nullptr)) {
			AVLWordNode* temp = root->left ? root->left : root->right;

			if (temp == nullptr) {
				temp = root;
				root = nullptr;
			}
			else 
				*root = *temp;

			free(temp);
		}
		else {
			AVLWordNode* temp = FindMostRightNodeAtSubLeftTree(root->left);
			root->word = temp->word;
			root->count = temp->count;
			root->order = temp->order;
			root->left = RemoveNode(root->left, temp->word);
		}
	}
	if (!root) return root;

	root->height = GetMaxHeight(root);

	int bal = GetBalanceWeight(root);

	//Trái trái
	if (bal > 1 && GetBalanceWeight(root->left) >= 0) {
		return RotateRight(root);
	}
	//Trái phải
	else if (bal > 1 && GetBalanceWeight(root->left) < 0) {
		root->left = RotateLeft(root->left);
		return RotateRight(root);
	}
	//Phải phải
	else if (bal < -1 && GetBalanceWeight(root->right) <= 0) {
		return RotateLeft(root);
	}
	//Phải trái
	else if (bal < -1 && GetBalanceWeight(root->right) > 0) {
		root->right = RotateRight(root->right);
		return RotateLeft(root);
	}
	return root;
}

AVLDifferNode* Insert(AVLDifferNode* root, std::string value) {
	if (!root) {
		return new AVLDifferNode(value);
	}

	if (value.compare(root->word) < 0) {
		root->left = Insert(root->left, value);
	}
	else if(value.compare(root->word) > 0) {
		root->right = Insert(root->right, value);
	}
	else {
		return root;
	}

	root->height = GetMaxHeight(root);

	int bal = GetBalanceWeight(root);

	//Trái - trái
	if (bal > 1 && value.compare(root->left->word) < 0) {
		return RotateRight(root);
	}

	//Phải - phải
	if (bal < -1 && value.compare(root->right->word) > 0) {
		return RotateLeft(root);
	}

	//Trái - phải
	if (bal > 1 && value.compare(root->left->word) > 0) {
		root->left = RotateLeft(root->left);
		return RotateRight(root);
	}

	//Phải - trái
	if (bal < -1 && value.compare(root->right->word) < 0) {
		root->right = RotateRight(root->right);
		return RotateLeft(root);
	}

	return root;
}


struct AVLSentenceNode {

	int orderSentence;
	int height;
	AVLWordNode* wordRoot;

	AVLSentenceNode* left;
	AVLSentenceNode* right;

	AVLSentenceNode(std::vector<std::string> sents, int order) {
		int len = sents.size();
		int count = 1;
		wordRoot = nullptr;
		for (int i = 0; i < len; i++) {
			wordRoot = Insert(wordRoot, sents[i], count++);
		}
		orderSentence = order;
		height = 1;
		left = nullptr;
		right = nullptr;
	}
};



AVLSentenceNode* Insert(AVLSentenceNode* root, std::vector<std::string> sents, int order) {
	if (!root) {
		return new AVLSentenceNode(sents,order);
	}

	if (order < root->orderSentence) {
		root->left = Insert(root->left, sents, order);
	}
	else if(order > root->orderSentence) {
		root->right = Insert(root->right, sents, order);
	}
	else {
		return root;
	}

	root->height = GetMaxHeight(root);

	int bal = GetBalanceWeight(root);

	//Trái - trái
	if (bal > 1 && order < root->left->orderSentence) {
		return RotateRight(root);
	}
	//Phải - phải
	if (bal < -1 && order > root->right->orderSentence) {
		return RotateLeft(root);
	}
	//Trái - phải
	if (bal > 1 && order > root->left->orderSentence) {
		root->left = RotateLeft(root->left);
		return RotateRight(root);
	}
	//Phải - trái
	if (bal < -1 && order < root->right->orderSentence) {
		root->right = RotateRight(root->right);
		return RotateLeft(root);
	}

	return root;
}