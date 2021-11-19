#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include "AVLTree.h"

bool IsAlphabetCharacter(const char &c) {
    if (('0' <= c && c <= '9') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
        return true;
    }
    return false;
}
bool IsPunctual(const char &c) {
    if (c == '.' || c == '!' || c == '?') {
        return true;
    }
    return false;
}
void StringTolowerAndRemoveCharacter(std::string& str, const bool &isRemovePunctual)
{
    int len = str.length();

    std::string newStr = "";
    for (int i = 0; i < len; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 32;
        }
        if (IsAlphabetCharacter(str[i])) {
            newStr += str[i];
        }
    }

    if (!isRemovePunctual) {
        if (IsPunctual(str[len-1])) {
            newStr += str[len-1];
        }
    }
    if (newStr != "")
        str = newStr;
}


void Normalize(AVLWordNode* &root, AVLWordNode* stopWords) {
    if (!stopWords) return;
    if (Contain(root, stopWords->word)) {
        root = RemoveNode(root, stopWords->word);
    }
    Normalize(root,stopWords->left);
    Normalize(root,stopWords->right);
}