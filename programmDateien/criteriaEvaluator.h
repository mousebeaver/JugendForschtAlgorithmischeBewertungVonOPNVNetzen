#ifndef CRITERIAEVALUATOR_H_INCLUDED
#define CRITERIAEVALUATOR_H_INCLUDED
#include "definitions.h"


//Kennzahlenberechnung fuer eine gegebene Stadt
void evaluateSingleCity(double& snv, double& bta, double& vsm, //Kennzahlen
                        int crossings, int stops, vector<tiid>& omega, vector<int>& positions, vector<tiid>& streetDescription, double u)
{
    cout<<"Calculating the criteria..."<<endl;
    //Container fuer die Adjazenzliste des Graphen G, Format: Kosten, Index
    vector<vector<pdi>> g(crossings, vector<pdi> (0));
    for(tiid t : streetDescription)
    {
        int a, b; //1-indizierte Indizes
        double c; //Kosten
        tie(a, b, c) = t;
        a--; b--;
        g[a].push_back({c, b});
        g[b].push_back({c, a});
    }

    cout<<"Calculating BTA..."<<endl;
    //Berechnung von BTA als Summe der Kehrwerte der Frequenzen
    bta = 0;
    for(tiid t : omega)
    {
        int a, b;
        double d;
        tie(a, b, d) = t;
        bta += 1/d;
    }

    cout<<"Calculating sG..."<<endl;
    //Vorberechnung von s_G aller Haltestellenpaare
    vector<vector<double>> sG(stops, vector<double> (stops));
    for(int i = 0; i < stops; i++)
    {
        int p = positions[i]; //Kreuzung der i-ten Haltestelle
        vector<double> dist(crossings, INF); //Distanz von der i-ten Haltestelle

        //Umsetzung des Dijkstra-Algorithmus
        priority_queue<pdi, vector<pdi>, greater<pdi>> q;
        q.push({0, p});
        while(!q.empty())
        {
            int v; double d;
            tie(d, v) = q.top();
            q.pop();
            if(dist[v] > d)
            {
                dist[v] = d;
                for(auto j : g[v])
                {
                    if(dist[j.second] > dist[v]+j.first)
                    {
                        q.push({dist[v]+j.first,j.second});
                    }
                }
            }
        }

        //Speichern der Entfernungen zwichen Haltestellen in sG
        for(int j = 0; j < stops; j++)
        {
            sG[i][j] = dist[positions[j]];
        }
    }

    cout<<"Calculating sO..."<<endl;
    //Vorberechnung von s_O aller Haltestellenpaare
    vector<vector<double>> sO(stops, vector<double> (stops, INF));
    //Berechnen und Eintragen aller Werte der Kostenfunktion fuer Kanten von O
    for(int i = 0; i < stops; i++)
    {
        sO[i][i] = 0;
    }
    for(tiid t : omega)
    {
        int a, b; double d;
        tie(a, b, d) = t;
        a--; b--;
        double cost = sG[a][b]+u*d/2;
        sO[a][b] = cost;
        sO[b][a] = cost;
    }
    //Berechnen aller kuerzesten Wege zwischen Haltestellen im OPNV-Netz mit Floyd-Warshall
    for(int k = 0; k < stops; k++)
    {
        for(int i = 0; i < stops; i++)
        {
            for(int j = 0; j < stops; j++)
            {
                sO[i][j] = min(sO[i][j], sO[i][k]+sO[k][j]);
                sO[j][i] = sO[i][j];
            }
        }
    }

    cout<<"Calculating SNV..."<<endl;
    //Umsetzung der Berechnung von SNV
    snv = 0;
    for(int i = 0; i < stops; i++)
    {
        for(int j = 0; j < stops; j++)
        {
            if(sG[i][j] != 0)
            {
                snv += sO[i][j]/sG[i][j];
            }

        }
    }
    snv /= (double) stops*stops - stops;
    cout<<"Calculating VSM..."<<endl;
    //Die Berechnung von VSM koennte gleichzeitig mit der von SNV geschehen, wird zur besseren Uebersichtlichkeit aber separat behandelt
    vsm = -1;
    for(int i = 0; i < stops; i++)
    {
        for(int j = 0; j < stops; j++)
        {
            if(sG[i][j] != 0)
            {
                vsm = max(vsm, sO[i][j]/sG[i][j]);
            }
        }
    }
}

#endif // CRITERIAEVALUATOR_H_INCLUDED
