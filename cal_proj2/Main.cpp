#include "Main.h"


/**
 * EXPLICAÇÃO DA IMPLEMENTAÇÃO
 * Perante a palavra original, o programa vai ao dicionário à procura de palavras anagramas.
 * Para tal, ele inicialmente verifica se o tamanho da palavra a analisar é de tamanho igual ou maior por uma unidade (caso da adição de uma letra) à original.
 * Se tal se verificar, calcula a distância de edição entre ela e a original. Se a distância de edição for apenas de 1 unidade, significa que é anagrama. No entanto
 * é ainda possível ser anagrama. Para tal, ordena-se quer a palavra original, quer a palavra a analisar, e calculam-se de novo as distâncias de edição. 
 * Desta forma, se a distância de edição das palavras for de 1 ou 0 significa que a palavra é anagrama e acrescenta-se ao vector com a sua distância de
 * edição (original, não a ordenada). No final ordena-se o vector pela distância de edições.
 */


int main() {
	int dist, dist_sorted;
	int original_size, tmp_size;
	unsigned int size;
	float sim;
	string tmp, tmp_sorted, original, original_sorted;
	ifstream dic;
	vector<word> anag_list;

	dic.open("words.txt");
	cout << "Palavra: ";
	cin >> original;
	cout << "Palavra | Distância de Edição | Similaridade\n" << endl;

	original_size = original.size();
	original_sorted = original;
	sort(original_sorted.begin(), original_sorted.end());

	while (dic.good()) {
		getline(dic, tmp);
		tmp_size = tmp.size();
		if (tmp_size == original_size || tmp_size == original_size + 1) {

			dist = edit_distance(tmp, original);

			if (dist == 1) {
				sim = similarity(tmp_size, dist);
				add_anagram(&anag_list, tmp, dist, sim);
			} else {
				tmp_sorted = tmp;
				sort(tmp_sorted.begin(), tmp_sorted.end());
				dist_sorted = edit_distance(tmp_sorted, original_sorted);
				if (dist_sorted == 1 || (dist_sorted == 0 && dist != 0)) {
					sim = similarity(tmp_size, dist);
					add_anagram(&anag_list, tmp, dist, sim);
				}
			}

		}
	}

	sort(anag_list.begin(), anag_list.end(), compare_word);
	size = anag_list.size();

	for (unsigned int i = 0; i < size; i++)
		cout << anag_list.at(i).str << " | " << anag_list.at(i).edit_distance
				<< " | " << anag_list.at(i).similarity << endl;

	cout << "Encontradas " << size << " palavras possíveis." << endl;
}

void add_anagram(vector<word> *vec, string str, int edit_distance,
		float similarity) {
	word anagram;
	anagram.str = str;
	anagram.edit_distance = edit_distance;
	anagram.similarity = similarity;
	(*vec).push_back(anagram);
}

bool compare_word(word word1, word word2) {
	if (word1.edit_distance < word2.edit_distance)
		return true;
	else
		return false;
}

float similarity(float size, int dist) {
	return 1.0f - (float) dist / size;
}

unsigned int edit_distance(string s1, string s2) {
	const size_t len1 = s1.size(), len2 = s2.size();
	vector<vector<unsigned int> > d(len1 + 1, vector<unsigned int>(len2 + 1));

	d[0][0] = 0;
	for (unsigned int i = 1; i <= len1; ++i)
		d[i][0] = i;
	for (unsigned int i = 1; i <= len2; ++i)
		d[0][i] = i;

	for (unsigned int i = 1; i <= len1; ++i)
		for (unsigned int j = 1; j <= len2; ++j)

			d[i][j] = min(min(d[i - 1][j] + 1, d[i][j - 1] + 1),
					d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1));
	return d[len1][len2];
}
