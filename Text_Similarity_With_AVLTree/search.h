#pragma once
#include <iostream>
#include <string>
#include <vector>

int BinarySearch(const std::string& str, const std::vector<std::string>& words, const int& left, const int& right) {
    if (left > right) return -1;
    int mid = left + (right - left) / 2;
    if (str == words[mid]) return mid;
    else if (str < words[mid]) return BinarySearch(str, words, left, mid - 1);
    return BinarySearch(str, words, mid + 1, right);
}

int LinearSearch(const std::string& str, const std::vector<std::string>& words) {
    int sizeStopWords = words.size();
    for (int i = 0; i < sizeStopWords; i++) {
        if (str == words[i]) {
            return i;
        }
    }
    return -1;
}