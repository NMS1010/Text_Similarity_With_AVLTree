#pragma once
#include <iostream>
#include <string>

struct AVLDifferNode {
	std::string word;
	double weight;

	int height;
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


struct AVLNode {
	std::string word;
	int count;
	std::vector<int> order;

	int height;
	AVLNode* left;
	AVLNode* right;

	AVLNode(std::string value) {
		word = value;
		height = 1;
		count = 1;
		left = nullptr;
		right = nullptr;
	}

	AVLNode(std::string value, int x) {
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

int TotalSize(AVLNode* root) {
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

AVLNode* Insert(AVLNode* root, std::string value, int order) {
	if (!root) {
		return new AVLNode(value,order);
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

AVLNode* FindMostRightNodeAtSubLeftTree(AVLNode* root) {
	/*if (!root->right) return root;
	FindMostRightNodeAtSubLeftTree(root->right);*/
	AVLNode* curr = root;
	while (curr->right != nullptr) {
		curr = curr->right;
	}
	return curr;
}

AVLNode* RemoveNode(AVLNode* root, std::string value) {
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
			AVLNode* temp = root->left ? root->left : root->right;

			if (temp == nullptr) {
				temp = root;
				root = nullptr;
			}
			else 
				*root = *temp;

			free(temp);
		}
		else {
			AVLNode* temp = FindMostRightNodeAtSubLeftTree(root->left);
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
