#include <map>
#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

bool value_sorting(const pair<string, int> &a, const pair<string, int> &b)
{
    return (a.second > b.second);
}

int main()
{
    char c;
    string word = "";
    map <string, int> counters;
    vector <pair<string, int>> vect;
    set <char> sep{' ', ',', '.', '?', '!', '\n', '\t', ';'};
    FILE* input = fopen("hamlet.txt", "r");
    while (!feof(input))
    {
        c = fgetc(input);
        
        if (sep.find(c) == sep.end())
        {
            word.push_back(c);
        }
        else
        {
            if (word.length() > 0)
            {
                if (counters.find(word) == counters.end())
                    counters[word] = 1;
                else
                    counters[word] += 1;
                word.clear();
            }
        }
    }
    fclose(input);
    map <string, int> :: iterator it;
    for (it = counters.begin(); it != counters.end(); it++)
    {
        vect.push_back(make_pair(it->first, it->second));
    }
    sort(vect.begin(), vect.end(), value_sorting);
    
    

    ofstream output("table.txt");
    for (int i = 0; i < vect.size(); i++)
    {
        output << vect[i].first << " : " << vect[i].second << endl;
    }
    output.close();
}
