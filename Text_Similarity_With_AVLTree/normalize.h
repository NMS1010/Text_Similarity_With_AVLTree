#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include "AVLTree.h"

bool IsAlphabetCharacter(char c) {
    if (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
        return true;
    }
    return false;
}

void StringTolowerAndRemoveCharacter(std::string& str)
{
    int len = str.length();

    std::string newStr="";
    for (int i = 0; i < len; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 32;
        }
        if (IsAlphabetCharacter(str[i])) {
            newStr += str[i];
        }
    }
    if (newStr != "")
        str = newStr;
}


void Normalize(AVLNode* &root, AVLNode* stopWords) {
    if (!stopWords) return;
    if (Contain(root, stopWords->word)) {
        root = RemoveNode(root, stopWords->word);
    }
    Normalize(root,stopWords->left);
    Normalize(root,stopWords->right);
}