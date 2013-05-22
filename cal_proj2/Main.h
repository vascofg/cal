#ifndef _MAIN_
#define _MAIN_

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

typedef struct word {
	string str;
	int edit_distance;
	float similarity;
} word;

void add_anagram(vector<word> *vec, string str, int edit_distance,
		float similarity);
unsigned int edit_distance(string s1, string s2);
float similarity(float size, int dist);
bool compare_word(word word1, word word2);

#endif
