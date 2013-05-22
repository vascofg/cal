#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>    // std::sort
using namespace std;

unsigned int edit_distance(string s1, string s2);
float GetSimilarity(string string1, string string2);

int main()
{
	int dist;
	float sim;
	ifstream is;
	is.open("words.txt");
	string tmp, original;
	int size, tmpsize;
	cout << "Palavra: ";
	cin >> original;
	size=original.size();
	cout << size << endl;
	vector<string> v;

	int dist2;
	string originalsorted = original;
	sort(originalsorted.begin(),originalsorted.end());
	string tmpsorted;
	while ( is.good() )
	{
		getline(is,tmp);
		tmpsize=tmp.size();
		if(tmpsize==size || tmpsize==size+1)
		{

			sim=GetSimilarity(tmp,original);
			dist=edit_distance(tmp,original);
			//cout << tmp << " | sim:" << sim << " | dist: " << dist << endl;
			tmpsorted=tmp;
			sort(tmpsorted.begin(),tmpsorted.end());
			//cout << "sorted " << tmpsorted << "|" << originalsorted << endl;
			dist2=edit_distance(tmpsorted,originalsorted);

			//Ordena ambas as strings
			//vê a distancia de edição entre elas[(original,palavra) e (original_ordenada,palavra_ordenada)]
			//Encontra-se dentro dos parâmetros do pdf se a distancia de edição da original à palavra for 0 ou 1, ou então quando ordenadas a distância é 1
			//É capaz é de não ser muito eficiente porque calcula distancia, ordena e volta a calcular distancia.
			if(dist==0	|| dist2==1 || dist==1)

		//	cout << ":" << tmp << endl;
		 	v.push_back(tmp);

		}
	}
	/*for(int i=0;i<v.size();i++)
		cout << v[i] << endl;*/
	cout << "Encontradas " << v.size() << " palavras possíveis. Boa sorte." << endl;
	cin >> tmp;
}

float GetSimilarity(string string1, string string2)
{
    float dis=(float)edit_distance(string1, string2);
    float maxLen=(float)string1.size();
    if (maxLen < string2.size())
    maxLen = (float)string2.size();
    if (maxLen == 0.0F)
    return 1.0F;
    else
    return 1.0F - dis/maxLen;
}

unsigned int edit_distance(string s1, string s2)
{
        const size_t len1 = s1.size(), len2 = s2.size();
        vector<vector<unsigned int> > d(len1 + 1, vector<unsigned int>(len2 + 1));

        d[0][0] = 0;
        for(unsigned int i = 1; i <= len1; ++i) d[i][0] = i;
        for(unsigned int i = 1; i <= len2; ++i) d[0][i] = i;

        for(unsigned int i = 1; i <= len1; ++i)
                for(unsigned int j = 1; j <= len2; ++j)

                      d[i][j] = min( min(d[i - 1][j] + 1,d[i][j - 1] + 1),
                                          d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) );
        return d[len1][len2];
}
