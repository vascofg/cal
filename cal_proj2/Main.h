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

/**
 * Adiciona ao vector a palavra anagrama da original.
 * @param: Vector dos anagramas da palavra original.
 * @param: Palavra anagrama.
 * @param: Distância de edição.
 * @param: Similaridade.
 */
void add_anagram(vector<word> *vec, string str, int edit_distance,
		float similarity);

/**
 * Calcula a distância de edição entre ambas as strings seguindo o algoritmo de Levenshtein
 * @param: Palavra original.
 * @param: Palavra a analisar.
 * @return: Distância de edição.
 */
unsigned int edit_distance(string s1, string s2);

/**
 * Calcula a similaridade tendo em conta o tamanho da palavra a analisar e a sua distância de edição em comparação com a palavra original
 * @param: Tamanho da palavra a analisar.
 * @param: Distância de Edição à original.
 * @return: Similaridade
 */
float similarity(float size, int dist);

/**
 * Compara a distância de edição entre ambas as palavras
 * @param: Palavra 1.
 * @param: Palavra 2.
 * @return: true se word1.edit_distance < word2.edit_distance, se não false
 */
bool compare_word(word word1, word word2);

#endif
