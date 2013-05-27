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

/** @struct word
 *  @brief Responsible for storing the anagram.
 *  @var word::str
 *  Anagram String
 *  @var word::edit_distance
 *  Anagram edit distance
 *  @var word::similarity
 *  Anagram Similarity
 */
typedef struct word {
	string str;
	int edit_distance;
	float similarity;
} word;

/**
 * Adiciona ao vector a palavra anagrama da original.
 * @param vec Vector dos anagramas da palavra original.
 * @param str Palavra anagrama.
 * @param edit_distance Distancia de edicao.
 * @param similarity Similaridade.
 */
void add_anagram(vector<word> *vec, string str, int edit_distance,
		float similarity);

/**
 * Calcula a distancia de edicao entre ambas as strings seguindo o algoritmo de Levenshtein
 * @param s1 Palavra original.
 * @param s2 Palavra a analisar.
 * @return: Distancia de edicao.
 */
unsigned int edit_distance(string s1, string s2);

/**
 * Calcula a similaridade tendo em conta o tamanho da palavra a analisar e a sua distancia de edicao em comparacao com a palavra original
 * @param size Tamanho da palavra a analisar.
 * @param dist Distancia de edicao à original.
 * @return: Similaridade
 */
float similarity(float size, int dist);

/**
 * Compara a distancia de edicao entre ambas as palavras
 * @param word1 Palavra 1.
 * @param word2 Palavra 2.
 * @return: true se word1.edit_distance < word2.edit_distance, se não false
 */
bool compare_word(word word1, word word2);

#endif
