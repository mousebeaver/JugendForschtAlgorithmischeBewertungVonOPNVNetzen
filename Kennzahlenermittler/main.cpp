#define tiid tuple<int, int, double>
#define INF numeric_limits<double>::max()/4
#define pdi pair<double, int>
#include <bits/stdc++.h>
using namespace std;

void readInput(int& crossings, int& stops, vector<tiid>& omega, vector<vector<pdi>>& g, double& u, ofstream& fileOut, vector<int>& positions)
{
    //Diese Funktion liest die Eingabe ein

    //Oeffnen der Ein- und Ausgabedatei
    char fileIn[1000];
    string outputName;
    cout<<"Name der Eingabedatei: ";
    cin>>fileIn;
    cout<<"Name der Ausgabedatei: ";
    cin>>outputName;
    freopen(fileIn, "r", stdin);
    fileOut.open(outputName, ios_base::out);

    //In der ersten Zeile der Eingabedatei stehen die Anzahl der Kreuzungen, der Haltestellen und u:
    cin>>crossings>>stops>>u;
    g.assign(crossings, vector<pdi> (0));

    //In der zweiten Zeile der Eingabedatei steht die Anzahl der Strassen und der Verbindungen
    int connections;
    int intersections;
    cin>>connections>>intersections;

    //In der dritten Zeile der Eingabedatei steht, an welcher Kreuzung sich welche Haltestelle befindet
    //Die i-te Zahl ist der Kreuzungsindex der i-ten Haltestelle
    for(int i = 0; i < stops; i++)
    {
        int a;
        cin>>a;
        a--;
        positions.push_back(a);
    }

    //In den folgenden Zeilen der Eingabedatei sind die Verbindungen im OPNV-Netz verzeichnet
    //Jede Zeile enthaelt die Indizes der beteiligten Haltestellen und die Frequenz der Busse
    for(int i = 0; i < connections; i++)
    {
        int a, b; //1-indizierte Indizes
        double f; //Frequenz
        cin>>a>>b>>f;
        omega.push_back({a-1, b-1, f});
    }

    //In den folgenden Zeilen der Eingabedatei sind die Strassen verzeichnet
    //Jede Zeile enthaelt die Indizes der beteiligten Kreuzungen und die Kosten der Kante
    for(int i = 0; i < intersections; i++)
    {
        int a, b; //1-indizierte Indizes
        double c; //Kosten
        cin>>a>>b>>c;
        a--; b--;
        g[a].push_back({c, b});
        g[b].push_back({c, a});
    }
}

int main()
{
    //Variablen fuer die zu berechnenden Kennzahlen:
    double snv;
    double bta;
    double vsm;

    //Variablen zum Speichern der Eingabe
    int crossings; //Anzahl der Kreuzungen
    int stops; //Anzahl der Haltestellen
    vector<tiid> omega(0); //Container fuer die Frequenz der Busse im OPNV-Netz, Format: Index1, Index2, Frequenz
    vector<int> positions(0); //Speichert fuer jede Haltestelle ihre Kreuzung
    vector<vector<pdi>> g(0); //Container fuer die Adjazenzliste des Graphen G, Format: Kosten, Index
    double u; //Faktor fuer die Kostenfunktion c_G
    ofstream fileOut; //Ausgabedatei

    readInput(crossings, stops, omega, g, u, fileOut, positions);

    //Berechnung von BTA als Summe der Kehrwerte der Frequenzen
    bta = 0;
    for(tiid t : omega)
    {
        int a, b;
        double d;
        tie(a, b, d) = t;
        bta += 1/d;
    }

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
    /*
    //DEBUG:
    for(int i = 0; i < stops; i++)
    {
        for(int j = 0; j < stops; j++)
        {
            cout<<sG[i][j]<<" ";
        }
        cout<<endl;
    }*/


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
    /*
    //DEBUG:
    for(int i = 0; i < stops; i++)
    {
        for(int j = 0; j < stops; j++)
        {
            cout<<sO[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"INF: "<<INF<<endl;*/

    //Umsetzung der Berechnung von SNV
    snv = 0;
    for(int i = 0; i < stops; i++)
    {
        for(int j = 0; j < stops; j++)
        {
            if(sG[i][j] != 0)
            {
                snv += sO[i][j]/sG[i][j];
                //cout<<"SNV: "<<snv<<endl;
                //cout<<"sO: "<<sO[i][j]<<" sG: "<<sG[i][j]<<", RES: "<<sO[i][j]/sG[i][j]<<endl;
            }

        }
    }
    snv /= (double) stops*stops - stops;
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

    //Ausgabe
    fileOut<<"Kennzahlen des gegebenen ÖPNV-Netzes:\n";
    fileOut<<"SNV: "<<setprecision(15)<<snv<<"\n";
    fileOut<<"BTA: "<<setprecision(15)<<bta<<"\n";
    fileOut<<"VSM: "<<setprecision(15)<<vsm<<"\n";

    return 0;
}
