#ifndef KRUSKAL_H_INCLUDED
#define KRUSKAL_H_INCLUDED
#include "definitions.h"

//Die folgenden beiden Funktionen implementieren die fuer den Algorithmus von Kruskal noetige Union-Find-Datenstruktur
int findPar(int i, vector<int>& uf)
{
    if(uf[i] != i)
    {
        uf[i] = findPar(uf[i], uf);
    }
    return uf[i];
}

void unite(int i, int j, vector<int>& uf)
{
    i = findPar(i, uf);
    j = findPar(j, uf);
    uf[i] = j;
}

//Der Algorithmus von Kruskal
void kruskal(vector<int>& nodeList, vector<vector<double>>& sG, vector<pii>& buslines, int goalStops, vector<pii>& preconnections)
{
    //Verbindet alle Haltestellen in nodeList durch einen MST
    //Dazu wird der Algorithmus von Kruskal umgesetzt
    //preconnections speichert vorgegebene Kanten
    vector<tiii> edges(0);
    for(int i = 0; i < (int) nodeList.size(); i++)
    {
        for(int j = i+1; j < (int) nodeList.size(); j++)
        {
            edges.push_back({sG[nodeList[i]][nodeList[j]], nodeList[i], nodeList[j]});
        }
    }
    sort(edges.begin(), edges.end());
    vector<int> uf(goalStops);
    iota(uf.begin(), uf.end(), 0);

    for(pii p : preconnections)
    {
        unite(p.first, p.second, uf);
        buslines.push_back({p.first, p.second});
    }

    for(tiii t : edges)
    {
        int d, i, j;
        tie(d, i, j) = t;
        if(findPar(i, uf) != findPar(j, uf))
        {
            buslines.push_back({i, j});
            unite(i, j, uf);
        }
    }
}

#endif // KRUSKAL_H_INCLUDED
