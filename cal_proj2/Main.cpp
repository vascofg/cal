#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;

int main()
{
	ifstream is;
	is.open("words.txt");
	string tmp;
	int size, tmpsize;
	cout << "Palavra: ";
	cin >> tmp;
	size=tmp.size();
	cout << size << endl;
	vector<string> v;
	while ( is.good() )
	{
		getline(is,tmp);
		tmpsize=tmp.size();
		if(tmpsize==size || tmpsize==size+1)
		{
			//cout << tmpsize << " - " << tmp << endl;
			v.push_back(tmp);
		}
	}
	/*for(int i=0;i<v.size();i++)
		cout << v[i] << endl;*/
	cout << "Encontradas " << v.size() << " palavras possíveis. Boa sorte." << endl;
}
