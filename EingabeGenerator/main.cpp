/*
Dieses Programm generiert randomisiert beispielhafte Eingaben fuer den KennzahlenTester
In mehreren Schritten werden das OPNV- und das Strassennetz der Stadt generiert
*/
#define pii pair<int, int>
#define tiii tuple<int, int, int>
#define INF numeric_limits<double>::max()
#define pdi pair<double, int>
#include <bits/stdc++.h>
using namespace std;

int nodeIndex(pii node, map<pii, int>& nodes, map<int, pii>& coordinates) //Ordnet die Koordinaten eines Knotens seinem Index zu
{
    if(nodes.count(node) == 0) //Der Knoten ist neu!
    {
        int newind = nodes.size();
        nodes[node] = newind;
        coordinates[newind] = node;
    }
    return nodes[node];
}

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

int main()
{
    //Einrichten des Zufallsgenerators
    time_t t;
    srand((unsigned) time(&t));

    //Einrichten der Ausgabedatei:
    string namefileOut;
    cout<<"Name der Ausgabedatei: ";
    cin>>namefileOut;
    ofstream fileOut;
    fileOut.open(namefileOut, ios_base::out);

    /*
    Zuerst sollen ungefaehr 10^4 Kreuzungen im Strassennetz und die Strassen zwischen diesen erzeugt werden.
    Um dies moeglichst realistisch umzusetzen, wird zunaechst so getan, als haetten die Kreuzungen allesamt ganzzahlige Koordinaten in der Ebene
    Dann werden Quadrate an einer zufaelligen Stelle an den Rand des Stadtplans hinzugefuegt.
    Ein Quadrat hat dabei die Seitenlaenge 1. Die Ecken sind die Positionen von Kreuzungen, die Kanten sind die Positionen von Strassen
    Zu Beginn liegt genau ein Quadrat in der Ebene
    */

    cout<<"Creating Roadmap..."<<endl;
    const int goalCrossings = 10000; //zu erzeugende Anzahl an Kreuzungen
    set<pii> squareList; //Enthaelt alle schon hinzugefuegten Quadrate, repraesentiert durch die linke untere Ecke
    squareList.insert({0, 0}); //Das erste Quadrat
    vector<pii> newSquares(0); //moegliche neue Quadrate, die mindestens eine Seite mit schon vorhandenen teilen
    newSquares.push_back({1, 0});
    newSquares.push_back({0, 1});
    newSquares.push_back({-1, 0});
    newSquares.push_back({0, -1});
    set<pii> newCheck; //Enthaelt die gleichen Elemente wie newSquares, ermoeglicht aber einen schnelleren Existenzcheck
    newCheck.insert({1, 0});
    newCheck.insert({0, 1});
    newCheck.insert({-1, 0});
    newCheck.insert({0, -1});
    int crossings = 4; //Anzahl der schon vorhandenen Kreuzungen

    while(crossings < goalCrossings)
    {
        //Zufaelliges Auswaehlen eines neuen Quadrats
        int index = 0 + rand()%newSquares.size();
        pii toadd = newSquares[index];
        squareList.insert(toadd);
        newSquares.erase(newSquares.begin()+index);
        newCheck.erase(toadd);

        //Ueberpruefen eventueller neuer Ecken
        if(squareList.count({toadd.first-1, toadd.second}) == 0 && squareList.count({toadd.first, toadd.second-1}) == 0 && squareList.count({toadd.first-1, toadd.second-1}) == 0)
        {
            //Links unten
            crossings++;
        }
        if(squareList.count({toadd.first+1, toadd.second}) == 0 && squareList.count({toadd.first, toadd.second-1}) == 0 && squareList.count({toadd.first+1, toadd.second-1}) == 0)
        {
            //Rechts unten
            crossings++;
        }
        if(squareList.count({toadd.first-1, toadd.second}) == 0 && squareList.count({toadd.first, toadd.second+1}) == 0 && squareList.count({toadd.first-1, toadd.second+1}) == 0)
        {
            //Links oben
            crossings++;
        }
        if(squareList.count({toadd.first+1, toadd.second}) == 0 && squareList.count({toadd.first, toadd.second+1}) == 0 && squareList.count({toadd.first+1, toadd.second+1}) == 0)
        {
            //Rechts oben
            crossings++;
        }

        //Hinzufuegen neuer Quadrate in newSquares
        if(squareList.count({toadd.first+1, toadd.second}) == 0 && newCheck.count({toadd.first+1, toadd.second}) == 0)
        {
            //Rechtes
            newSquares.push_back({toadd.first+1, toadd.second});
            newCheck.insert({toadd.first+1, toadd.second});
        }
        if(squareList.count({toadd.first-1, toadd.second}) == 0 && newCheck.count({toadd.first-1, toadd.second}) == 0)
        {
            //Links
            newSquares.push_back({toadd.first-1, toadd.second});
            newCheck.insert({toadd.first-1, toadd.second});
        }
        if(squareList.count({toadd.first, toadd.second+1}) == 0 && newCheck.count({toadd.first, toadd.second+1}) == 0)
        {
            //Oben
            newSquares.push_back({toadd.first, toadd.second+1});
            newCheck.insert({toadd.first, toadd.second+1});
        }
        if(squareList.count({toadd.first, toadd.second-1}) == 0 && newCheck.count({toadd.first, toadd.second-1}) == 0)
        {
            //Unten
            newSquares.push_back({toadd.first, toadd.second-1});
            newCheck.insert({toadd.first, toadd.second-1});
        }
    }

    /*
    //DEBUG: Ausgabe aller Squares
    fileOut<<squareList.size()<<"\n";
    for(pii p : squareList)
    {
        fileOut<<p.first<<" "<<p.second<<"\n";
    }
    return 0;*/

    //Es folgt das Extrahieren der Knoten und Kanten
    cout<<"Extracting corresponding graph..."<<endl;
    map<pii, int> nodes; //Ordnet die Koordinaten eines Knotens seinem Index zu
    map<int, pii> coordinates; //Ordnet einem Index seine Koordinaten zu (fuer Debugging-Ausgaben)
    vector<tiii> outputEdges(0); //Ausgabe der Kanten in der Form <index1, index2, kosten>
    set<pii> edges; //Bereits bekannte Kanten mit <index1, index2> und index1 < index2
    set<pii>::iterator it = squareList.begin();
    while(it != squareList.end())
    {
        pii theSquare = *it;
        //Indizes der Knoten:
        int upleft = nodeIndex({theSquare.first, theSquare.second+1}, nodes, coordinates);
        int upright = nodeIndex({theSquare.first+1, theSquare.second+1}, nodes, coordinates);
        int lowleft = nodeIndex({theSquare.first, theSquare.second}, nodes, coordinates);
        int lowright = nodeIndex({theSquare.first+1, theSquare.second}, nodes, coordinates);
        //Behandeln der Kanten
        if(edges.count({min(upleft, upright), max(upleft, upright)}) == 0)
        {
            int cost = 1 + rand()%9; //Zufaellige Kosten fuer die Kante
            edges.insert({min(upleft, upright), max(upleft, upright)});
            outputEdges.push_back({min(upleft, upright), max(upleft, upright), cost});
        }
        if(edges.count({min(upleft, lowleft), max(upleft, lowleft)}) == 0)
        {
            int cost = 1 + rand()%9; //Zufaellige Kosten fuer die Kante
            edges.insert({min(upleft, lowleft), max(upleft, lowleft)});
            outputEdges.push_back({min(upleft, lowleft), max(upleft, lowleft), cost});
        }
        if(edges.count({min(lowright, upright), max(lowright, upright)}) == 0)
        {
            int cost = 1 + rand()%9; //Zufaellige Kosten fuer die Kante
            edges.insert({min(lowright, upright), max(lowright, upright)});
            outputEdges.push_back({min(lowright, upright), max(lowright, upright), cost});
        }
        if(edges.count({min(lowleft, lowright), max(lowleft, lowright)}) == 0)
        {
            int cost = 1 + rand()%9; //Zufaellige Kosten fuer die Kante
            edges.insert({min(lowleft, lowright), max(lowleft, lowright)});
            outputEdges.push_back({min(lowleft, lowright), max(lowleft, lowright), cost});
        }
        it++;
    }

    cout<<"Choosing the bus stops..."<<endl;
    //Jetzt werden die Haltestellen dne Kreuzunge zugewiesen
    const int goalStops = 150; //Anzahl der zu generierenden Haltestellen
    vector<int> stopPositions(goalStops); //i-te Variable: Position der i-ten Haltestelle
    vector<bool> available(crossings, true); //Kreuzung kann noch ausgewaehlt werden

    //Dafuer muss auch noch die Adjazenzliste des Strassennetz aufgebaut werden
    vector<vector<int>> adjlist(crossings, vector<int> (0));
    for(tiii t : outputEdges)
    {
        int a, b, c;
        tie(a, b, c) = t;
        adjlist[a].push_back(b);
        adjlist[b].push_back(a);
    }

    //Durchgehen aller Haltestellen
    for(int i = 0; i < goalStops; i++)
    {
        int index = rand()%crossings; //Index der Kreuzung fuer die aktuelle Haltestelle
        while(!available[index]) //Eine verfuegbare Kreuzung muss ausgewaehlt werden
        {
            index = rand()%crossings;
        }
        stopPositions[i] = index;

        //Damit sich die Haltestellen nicht an wenigen Punkten konzentrieren, werden alle
        //Kreuzungen mit einer geringen Distanz zu der aktuellen Kreuzung gesperrt
        const int blockingdistance = 4; //Maximale Distanz der zu sperrenden Kreuzungen (Distanz = Anzahl Kanten)
        //Sperren der Kreuzungen mit Breitensuche
        queue<pii> q; //Index, Distanz von der aktuellen Kreuzung
        q.push({index, 0});
        while(q.empty() && q.front().second <= blockingdistance)
        {
            pii p = q.front();
            q.pop();
            available[p.first] = false;
            for(int j : adjlist[p.first])
            {
                //Mehrfache Besuche koenne aufgrund der geringen Anzahl an Knoten zugelassen werden
                q.push({j, p.second+1});
            }
        }
    }

    /*
    //DEBUG: Ausgabe aller Knoten und Kanten im Strassennetz, Markierungen fuer alle Haltestellen
    fileOut<<outputEdges.size()<<"\n";
    for(tiii t : outputEdges)
    {
        int a, b, c;
        tie(a, b, c) = t;
        pii p1 = coordinates[a];
        pii p2 = coordinates[b]; //Ausgabe: Koordinaten des Ausgangspunkts, Drehung nach links (mit Startrichtung nach rechts)
        fileOut<<p1.first<<" "<<p1.second<<" ";
        if(p1.first+1 == p2.first)
        {
            fileOut<<0<<"\n";
        }
        else if(p1.first-1 == p2.first)
        {
            fileOut<<180<<"\n";
        }
        else if(p1.second+1 == p2.second)
        {
            fileOut<<90<<"\n";
        }
        else if(p1.second-1 == p2.second)
        {
            fileOut<<270<<"\n";
        }
    }
    fileOut<<goalStops<<"\n";
    for(int i : stopPositions)
    {
        fileOut<<coordinates[i].first<<" "<<coordinates[i].second<<"\n";
    }
    return 0;*/

    cout<<"Computing distances of bus stops..."<<endl;
    //Der folgende Abschnitt ist fast gleich aus dem Quellcode fuer den Kennzahlentester uebernommen:
    //Vorberechnung von s_G aller Haltestellenpaare
    vector<vector<pdi>> g(crossings, vector<pdi> (0));
    //Die Adjazenzliste g muss noch aufgebaut werden:
    for(tiii t : outputEdges)
    {
        int a, b; double c;
        tie(a, b, c) = t;
        g[a].push_back(make_pair(c, b));
        g[b].push_back(make_pair(c, a));
    }
    vector<vector<double>> sG(goalStops, vector<double> (goalStops));
    for(int i = 0; i < goalStops; i++)
    {
        int p = stopPositions[i]; //Kreuzung der i-ten Haltestelle
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
        for(int j = 0; j < goalStops; j++)
        {
            sG[i][j] = dist[stopPositions[j]];
        }
    }

    cout<<"Choosing bus lines..."<<endl;
    //An diesem Punkt sind das Strassen- und das OPNV-Netz erzeugt.
    //Es muessen noch die Verbindungen im OPNV-Netz bestimmt werden.
    const int kind = 2;
    /*Art des OPNV-Netzes:
    1 => Radial
    2 => Radial + tangentiale Linien (Spinnennetz)
    3 => MST
    4 => 3-Connect
    5 => 4-Connect
    6 => 5-Connect
    7 => TC-radial (Two centres)
    8 => TC-radial + tangentiale Linien (Spinnennetz
    */

    vector<pii> buslines(0); //Auszugebende Buslinien
    vector<pii> preconnections(0); //Kanten, die evtl. beim Kruskal vorgegeben werden
    vector<int> nodeList(goalStops); //Liste aller Bushaltestellenindizes
    iota(nodeList.begin(), nodeList.end(), 0);
    if(kind == 1 || kind == 2 || kind == 7 || kind == 8)
    {
        cout<<"Doing a radial net..."<<endl;
        //Es wird eine zentrale Haltestelle ausgewählt, von der alle Linien strahlenfoermig ausgehen sollen.
        //Auswaehlen des Zentrums:
        int high = -1;
        int right = -1;
        int low = numeric_limits<int>::max();
        int left = numeric_limits<int>::max(); //Variablen fuer die jeweils groesste/kleineste X/Y-Variable
        for(int i : stopPositions)
        {
            high = max(high, coordinates[i].second);
            low = min(low, coordinates[i].second);
            left = min(left, coordinates[i].first);
            right = max(right, coordinates[i].first);
        }

        //Auswaehlen des Knotens mit der geringsten Differenz zwischen den Distanz nach oben/unten bzw. links/rechts
        int center; //Sollte ungefaehr mittig liegen
        int best = numeric_limits<int>::max();
        for(int j = 0; j < goalStops; j++)
        {
            int i = stopPositions[j];
            pii c = coordinates[i];
            if(abs((right-c.first) - (c.first-left))+abs((high-c.second)-(c.second-low)) < best)
            {
                center = j;
                best = abs((right-c.first) - (c.first-left))+abs((high-c.second)-(c.second-low));
            }
        }
        //cout<<"CENTER: ("<<coordinates[stopPositions[center]].first<<", "<<coordinates[stopPositions[center]].second<<")"<<endl;

        set<int> leafs; //Verfuegbare Knoten
        vector<int> circle(goalStops, -1); //speichert den Abstand eines Knotens auf der Linie zum Zentrum
        vector<bool> visited(goalStops, false); //Schon im Netz
        int center1 = -1; int center2 = -1; //Variablen fuer das Zentrum in kind == 7 || kind == 8
        if(kind == 1 || kind == 2)
        {
            leafs.insert(center); //Der Knoten, der am ehesten in der Mitte liegt, wird als Zentrum verwendet
            visited[center] = true;
            circle[center] = 0;
        }
        else if(kind == 7 || kind == 8)
        {
            //Es muessen zwei Zentren ausgewaehlt werden!
            //Dazu wird das Drittel der Haltestellen mit der geringsten Distanz zum Zentrum untersucht
            //Von diesen werden die beiden mit der maximalen Entfernung ausgewaehlt und dreifach verbunden
            vector<pdi> distFromCenter(goalStops);
            for(int i = 0; i < goalStops; i++)
            {
                distFromCenter[i] = {sG[i][center], i};
            }
            sort(distFromCenter.begin(), distFromCenter.end()); //Haltestellen nach Entfernung vom Zentrum sortieren

            pii dcenter = {0, 0}; //Die beiden Zentren
            for(int i = 0; i < goalStops/5; i++)
            {
                for(int j = 0; j < goalStops/5; j++)
                {
                    //Alle Paare im naechsten Fuenftel durchgehen
                    int a = distFromCenter[i].second; int b = distFromCenter[j].second;
                    if(sG[a][b] > sG[dcenter.first][dcenter.second])
                    {
                        dcenter = {a, b};
                    }
                }
            }

            int a = dcenter.first;
            int b = dcenter.second;
            center1 = a; center2 = b;
            leafs.insert(a);
            leafs.insert(b);
            visited[a] = true;
            visited[b] = true;
            circle[a] = 0;
            circle[b] = 0;
            //Nun werden die beiden Zentren - ueber den kuerzesten Weg - dreifach miteinander verbunden
            buslines.push_back({a, b});
            buslines.push_back({a, b});
            buslines.push_back({a, b});
        }

        //Das Busnetz wird nun nach und nach erweitert.
        //Es wird immer nach der kuerzesten Verbindung zwischen einem Blatt und einem neuen Knoten gesucht!
        for(int i = 0; i < goalStops-1; i++) //Auswaehlen einer neuen Haltestelle
        {
            int bestdist = numeric_limits<int>::max();//Geringste Distanz zu einem unbekannten Knoten
            pii bestpair = {-1, -1}; //Paar mit diesem Knoten
            //Durchgehen aller Paare von Blatt und anderen Haltestellen:
            for(int a = 0; a < goalStops; a++)
            {
                if(!visited[a])
                {
                    for(int b : leafs)
                    {
                        if(sG[a][b] < bestdist)
                        {
                            bestdist = sG[a][b];
                            bestpair = {a, b};
                        }
                    }
                }
            }

            if(bestpair == make_pair(-1, -1))
            {
                break;
            }
            //Hinzufuegen der neuen Kante:
            buslines.push_back(bestpair);
            circle[bestpair.first] = circle[bestpair.second]+1;
            if(circle[bestpair.first] <= 12) //Die Linien sollen nicht zu lang werden!
            {
                leafs.insert(bestpair.first);
            }
            visited[bestpair.first] = true;
            if(bestpair.second != center && bestpair.second != center1 && bestpair.second != center2)
            {
                leafs.erase(bestpair.second);
            }
        }
        if(kind == 2 || kind == 8)
        {
            cout<<"Doing a spider net..."<<endl;
            //Das Spinnennetz wird auf dem radialen aufgebaut
            //Dazu werden immer die 3ten, 6ten Knoten usw. ausgewaehlt und durch einen MST ausgewaehlt
            int distcount = 1; //Nummer der aktuell hinzuzufuegenden Ringlinie
            while(distcount != -1) //Wird auf -1 gesetzt, wenn keine mehr gefunden wurden
            {
                bool found = false; //Wurden schon entsprechende Knoten gefunden
                vector<int> nodeList(0); //Knoten mit der passenden Distanz zum Zentrum
                for(int j = 0; j < goalStops; j++)
                {
                    if(circle[j] == 3*distcount)
                    {
                        found = true;
                        nodeList.push_back(j);
                    }
                }

                if(found) //Anwenden des MST auf die gefundenen Knoten
                {
                    kruskal(nodeList, sG, buslines, goalStops, preconnections);
                    distcount++; //Fuer den naechsten Ring
                }
                else
                {
                    //Abbruch, da alle gewollten Ringe gefunden wurden
                    distcount = -1;
                }
            }
        }
    }
    else if(kind == 3)
    {
        cout<<"Doing a MST..."<<endl;
        //Umsetzung des Algorithmus von Kruskal mithilfe von union find/dsu
        kruskal(nodeList, sG, buslines, goalStops, preconnections);
    }
    else if(kind == 4 || kind == 5 || kind == 6)
    {
        //Jede Bushaltestelle (mindestens) mit den Haltestellen verbinden, die ihr am naechsten sind
        int neighbours = kind-1; //minimale Anzahl von Verbindungen einer Bushaltestelle
        cout<<"Doing a "<<neighbours<<"-Connect..."<<endl;
        vector<vector<bool>> connected(goalStops, vector<bool> (goalStops, false)); //Bestehende Verbindungen
        for(int i = 0; i < goalStops; i++)
        {
            //durchlaufen aller zu verbindender Bushaltestellen
            vector<pdi> potNeighbours(0); //Distanz zu allen Nachbarn
            for(int j = 0; j < goalStops; j++)
            {
                if(i != j)
                {
                    potNeighbours.push_back({sG[i][j], j});
                }
            }
            sort(potNeighbours.begin(), potNeighbours.end()); //Nachbarn nach Distanz sortieren

            //Durchgehen der naechsten Nachbarn:
            for(int k = 0; k < neighbours; k++)
            {
                int j = potNeighbours[k].second; //Index des Nachbarn in stopPositions
                if(!connected[i][j]) //Es besteht keine direkte Verbindung
                {
                    connected[i][j] = true;
                    connected[j][i] = true;
                    preconnections.push_back({i, j});
                }
            }
        }
        kruskal(nodeList, sG, buslines, goalStops, preconnections);
    }
    cout<<"Done"<<endl;

    /*
    //Debug: Output aller Buslinien
    fileOut<<buslines.size()<<endl;
    for(int i = 0; i < (int) buslines.size(); i++)
    {
        buslines[i].first = stopPositions[buslines[i].first];
        buslines[i].second = stopPositions[buslines[i].second];
    }
    for(pii p : buslines)
    {
        //Format: Jeweils X- und Y-Koordinate des ersten und des zweiten Punkts
        fileOut<<coordinates[p.first].first<<" "<<coordinates[p.first].second<<" "<<coordinates[p.second].first<<" "<<coordinates[p.second].second<<"\n";
    }
    return 0;*/

    cout<<"Output..."<<endl;
    //Es wird eine Ausgabe gemaess des Eingabeformats des Kennzahlentesters gemacht
    fileOut<<crossings<<" "<<goalStops<<" "<<(double) 10/((double) 11+rand()%9)<<"\n";
    fileOut<<buslines.size()<<" "<<outputEdges.size()<<"\n";
    for(int i : stopPositions)
    {
        fileOut<<i+1<<" ";
    }
    fileOut<<"\n";
    for(pii p : buslines)
    {
        fileOut<<p.first+1<<" "<<p.second+1<<" "<<40<<"\n"; //Die Frequenz ist bei allen Linien gleich
    }
    for(tiii t : outputEdges)
    {
        int a, b, c;
        tie(a, b, c) = t;
        fileOut<<a+1<<" "<<b+1<<" "<<c<<"\n";
    }
    fileOut.close();
    cout<<"Done"<<endl;

    return 0;
}
