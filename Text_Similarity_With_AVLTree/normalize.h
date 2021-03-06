#pragma once
#include <iostream>
#include <string>

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
        if (len >= 1 && IsPunctual(str[len-1])) {
            newStr += str[len - 1];
        }
        else if (len >= 2 && str[len - 1] == '"' && IsPunctual(str[len - 2])) {
            newStr += str[len - 2];
        }
    }
    str = newStr;
}
