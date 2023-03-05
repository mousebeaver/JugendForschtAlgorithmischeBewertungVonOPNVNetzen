#ifndef RINGLINES_H_INCLUDED
#define RINGLINES_H_INCLUDED
#include "definitions.h"

//Implementiert die Funktion, die eine Reihe von Haltestellen mit gegebenem Zentrum zu einer Ringlinie verbindet:
void ringLine(vector<int>& nodeList, vector<pii>& buslines, pii center, map<int, pii>& coordinates, vector<int>& stopPositions)
{
    //Verbindet die Punkte in nodeList, speichert die Verbindungen in buslines
    int n = nodeList.size();
    vector<pdi> angleList(0); //Knoten in nodeList im Format: Winkel zwischen (Center->node) und X-Achse, node index
    for(int i : nodeList)
    {
        angleList.push_back({(double) atan2((double) coordinates[stopPositions[i]].second-(double)center.second, (double)coordinates[stopPositions[i]].first-(double)center.first), i});
    }
    sort(angleList.begin(), angleList.end());

    /*cout<<"Center: ("<<center.first<<", "<<center.second<<")\n";
    for(pdi p : angleList)
    {
        cout<<"("<<coordinates[p.second].first<<", "<<coordinates[p.second].second<<")\n";
    }
    exit(0);*/

    for(int i = 0; i < n; i++)
    {
        buslines.push_back({angleList[i].second, angleList[(i+1)%n].second});
    }
}


#endif // RINGLINES_H_INCLUDED
