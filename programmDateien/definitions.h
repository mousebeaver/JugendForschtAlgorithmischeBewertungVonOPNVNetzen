#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

//Makros, Headerdateiein etc. fuer alle verwendeten Quelltextdateien:
#define pii pair<int, int>
#define pdi pair<double, int>
#define tiid tuple<int, int, double>
#define tiii tuple<int, int, int>
#define tiiii tuple<int, int, int, int>
#define INF numeric_limits<double>::max()/4
#include <bits/stdc++.h>
using namespace std;

string nettypeName(int i)//Liefert Name des Netztyps mit kind = i zurueck
{
    switch(i)
    {
    case 1:
        return "radial";
    case 2:
        return "spider";
    case 3:
        return "mst";
    case 4:
        return "3connect";
    case 5:
        return "4connect";
    case 6:
        return "5connect";
    case 7:
        return "tcRadial";
    case 8:
        return "tcSpider";
    }
    return "noValidType";
}

const int defaultFrequency = 40; //Standardfrequenz fuer Busverbindungen

#endif // DEFINITIONS_H_INCLUDED
