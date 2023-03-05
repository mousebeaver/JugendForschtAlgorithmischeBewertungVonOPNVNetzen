#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED

#include "definitions.h"
#include "exampleGenerator.h"
#include "criteriaEvaluator.h"

/*Datei fuer das Ausfuehren der moeglichen Funktionalitaeten:
- Visualisierung aller Netztypen
- Bewertung aller Netztypen
*/

void visualize() //Visualisiert alle OPNV-Netztypen
{
    //Kommando fuer das Ausfuehren des Pythonscripts
    cout<<"Path to visualizinig script: ";
    string path;
    cin>>path;
    path = "python3 "+path;

    //Variablen zum Abfangen nicht benoetigter Ausgaben:
    int bufferInt;
    double bufferDouble;
    vector<int> bufferVecInt;
    vector<tiid> bufferVecTiid;

    //Typen von OPNV-Netzen durchgehen
    for(int i = 1; i <= 8; i++)
    {
        cout<<"\nVisualizing a new net..."<<endl;
        //Berechnen der zu visualisierenden Netztypen
        vector<tiiii> buslines(0);
        srand((unsigned) 1); //Einrichten des Zufallsgenerators mit festem Seed
        exampleGenerator(i, true, bufferInt, bufferInt, bufferDouble, bufferInt, bufferInt, bufferVecInt, bufferVecTiid, bufferVecTiid, buslines);

        //Ausgabe in eine Datei fuer das PythonSkript:
        ofstream fout;
        fout.open("buslineDescription.txt", ios_base::out);
        fout<<nettypeName(i)+".eps\n";
        fout<<buslines.size()<<"\n";
        for(tiiii t : buslines)
        {
            int a, b, c, d;
            tie(a, b, c, d) = t;
            fout<<a<<" "<<b<<" "<<c<<" "<<d<<"\n";
        }
        fout.close();

        //Ausfuehren des PythonSkripts:
        system(path.c_str());
        cout<<"Done"<<endl;
    }
    cout<<"End of visualization"<<endl;
}

void evaluate() //Bewertet alle OPNV-Netztypen
{
    //Einrichten des Zufallsgenerators mit "zufaelligem" Seed
    time_t t;
    srand((unsigned) time(&t));

    //Anzahl der untersuchten Staedte:
    int sampleSize;
    cout<<"Anzahl der untersuchten Staedte pro Netztyp: ";
    cin>>sampleSize;

    //Ausgabedatei:
    ofstream fout;
    fout.open("criteriaResult.txt", ios_base::out);

    for(int i = 1; i <= 8; i++) //Alle Netztypen durchgehen
    {
        //Summe der Kriterien ueber alle Beispielstaedte eines Typs
        cout<<"\nEvaluating net type "<<nettypeName(i)<<"..."<<endl;
        double snvSum = 0;
        double btaSum = 0;
        double vsmSum = 0;

        for(int j = 0; j < sampleSize; j++)
        {
            cout<<"\nEvaluating city "<<j+1<<" of "<<sampleSize<<" from type "<<i<<" of "<<8<<"..."<<endl;
            //Variablen zum Speichern der Staedte bzw. Abfangen nicht benoetigter Ausgaben:
            int crossingCounter, stopCounter, connectionCounter, intersectionCounter;
            double uCoefficient;
            vector<int> stopCrossings(0);
            vector<tiid> connectionDescription(0);
            vector<tiid> streetDescription(0);
            vector<tiiii> vtiiiiDummy;

            //Generieren einer Beispielstadt:
            exampleGenerator(i, false, crossingCounter, stopCounter, uCoefficient, connectionCounter, intersectionCounter, stopCrossings,
                             connectionDescription, streetDescription, vtiiiiDummy);

            //Bewerten der Beispielstadt:
            double snv;
            double bta;
            double vsm;
            evaluateSingleCity(snv, bta, vsm, crossingCounter, stopCounter, connectionDescription, stopCrossings, streetDescription, uCoefficient);
            snvSum += snv;
            btaSum += bta;
            vsmSum += vsm;
        }

        //Mittelwerte berechnen:
        snvSum /= (double) sampleSize;
        btaSum /= (double) sampleSize;
        vsmSum /= (double) sampleSize;

        //Ausgabe:
        fout<<"Durchschnittswerte der Kriterien ueber "<<sampleSize<<" Staedte des Typs "<<nettypeName(i)<<":\n\n";
        fout<<"SNV: "<<setprecision(15)<<snvSum<<"\n";
        fout<<"BTA: "<<setprecision(15)<<btaSum<<"\n";
        fout<<"VSM: "<<setprecision(15)<<vsmSum<<"\n\n";
    }
}

#endif // CONTROL_H_INCLUDED
