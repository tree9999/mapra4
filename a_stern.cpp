// LocalEdgeT <VertexT,CostT>      Wohin & was kostet's   (nicht: Kante & Kosten)
// NeighborT  vector<LocalEdgeT>   enthaelt alle Nachbarn(inkl. Kosten) von einem festen Punkt


#include "text_visualizer.h"
#include "unit.h"
#include "maze.h"
#include <fstream>
#include <cmath>

using std::vector;
using std::cin;
using std::cout;

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph
{
    public:
        typedef std::pair<double,double> CoordiB;

    protected:
        vector<NeighborT> nachbarn;
        vector<CoordiB>   coordinaten;

    public:
        const NeighborT& getNeighbors( VertexT v) const override;

        //CostT estimatedCost( VertexT from, VertexT to) const override;

        CostT cost( VertexT from, VertexT to) const override;

        void setVertexCount(size_t cow) { vertexCount = cow; }

    friend ifstream& operator >> (ifstream& ifs, CoordinateGraph& graph);
};

const DistanceGraph::NeighborT& CoordinateGraph::getNeighbors( VertexT v) const
{
    return nachbarn[v];
}

CostT CoordinateGraph::cost( VertexT from, VertexT to) const
{
    NeighborT fromsNachbarn = nachbarn[from];
    cout << "Anz Nachbarn " << fromsNachbarn.size() << '\n';
    for(LocalEdgeT v : fromsNachbarn)
        if(v.first == to)
            return v.second;

    return infty;
}

ifstream& operator >> (ifstream& ifs, CoordinateGraph& graph)
{
    size_t knotenAnz = 0, kantenAnz;

    ifs >> knotenAnz >> kantenAnz;

    graph.setVertexCount(knotenAnz);
    graph.nachbarn.resize(knotenAnz);

    for(VertexT i = 0; i<kantenAnz; i++)
    {
        VertexT a,b;
        CostT c;
        ifs >> a >> b >> c;

        DistanceGraph::LocalEdgeT nachbarA(b,c);
        graph.nachbarn[a].push_back(nachbarA);
    }

    for(VertexT i = 0; i<knotenAnz; i++)
    {
        CoordinateGraph::CoordiB xy;
        ifs >> xy.first >> xy.second;

        graph.coordinaten.push_back(xy);
    }

    return ifs;
}

class EuclidGraph : public CoordinateGraph
{
    public:
        CostT estimatedCost( VertexT from, VertexT to) const override
        {
            CoordinateGraph::CoordiB f_xy = coordinaten[from];
            CoordinateGraph::CoordiB t_xy = coordinaten[to];

            double dx = f_xy.first - t_xy.first;
            double dy = f_xy.second - t_xy.second;

            return 0.5*sqrt(dx*dx + dy*dy);
        }
};

class GeoAbstandGraph : public CoordinateGraph
{
    //Breitengrade (111.3km Abstand)
    //Laengengrade (71.5km Abstand)
    public:
        CostT estimatedCost( VertexT from, VertexT to) const override
        {
            CoordinateGraph::CoordiB f_xy = coordinaten[from];
            CoordinateGraph::CoordiB t_xy = coordinaten[to];

            double dx = 110*(f_xy.first - t_xy.first);
            double dy = 70*(f_xy.second - t_xy.second);

            return sqrt(dx*dx + dy*dy);
        }
};

class TimeGraph : public CoordinateGraph
{
    //Breitengrade (111.3km Abstand)
    //Laengengrade (71.5km Abstand)
    //Durchschnittsgeschwindigkeit 192km/h  =  3.2km/min
    public:
        CostT estimatedCost( VertexT from, VertexT to) const override
        {
            CoordinateGraph::CoordiB f_xy = coordinaten[from];
            CoordinateGraph::CoordiB t_xy = coordinaten[to];

            double dx = 110/3.2*(f_xy.first - t_xy.first);
            double dy = 70/3.2*(f_xy.second - t_xy.second);

            return sqrt(dx*dx + dy*dy);
        }
};

void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start, std::vector<CostT>& kostenZumStart)
{
    vector<VertexT> R;

    cout << "anz Knoten " << g.numVertices() << '\n';

    // Initialisierung der Kosten
    for(size_t i = 0; i<g.numVertices(); i++)
    {
        kostenZumStart[i] = g.cost(start,i);
    }

    // Initalisierung von R (Rest Knoten)
    for(size_t i = 0; i<g.numVertices(); i++)
    {
        if(i != start)
            R.push_back(i);
    }

    while(R.size() > 0)
    {
        CostT   minKost = infty;
        VertexT minVert = start;

        // Suche minimale Kosten
        for(VertexT v : R)
        {
            if(kostenZumStart[v] <= minKost)
            {
                minKost=kostenZumStart[v];
                minVert=v;
            }
        }

        // loesche Minimum aus Rest
        for(size_t i = 0; i < R.size(); i++)
            if(R[i] == minVert)
                R.erase(R.begin()+i);


        vector<DistanceGraph::LocalEdgeT> neueNachbarn = g.getNeighbors(minVert);

        // aktualisiere Kosten
         for(DistanceGraph::LocalEdgeT v : neueNachbarn)
         {
             double alt = kostenZumStart[v.first];
             double neu = minKost + v.second;
             kostenZumStart[v.first] = std::min(alt,neu);
         }
    }
}

bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    // ...
    return false; // Kein Weg gefunden.
}

int main()
{
    // Frage Beispielnummer vom User ab
    int bspNummer;
    cout << "Hallo Hallihallo,\nIch bins wieder euer c++ Programm!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        << "Welche Datei darfs denn sein? Habe 1-4 (Graphen) und 5-9 (Labyrinthszor) oder 10 (random Labyrinth, aber is weird)\n"
        << "bitte jetzt eine Zahl von 1-10 eintippen (nichts anderes sonst kommen wir nicht klar). Jetzt bitte wirklich\n";
    cin >> bspNummer;

    DistanceGraph* graph;

    EuclidGraph eGraph;
    GeoAbstandGraph gGraph;
    TimeGraph tGraph;
    MazeGraph mGraph;
    ifstream ifs;

    // Lade die zugehoerige Textdatei in einen Graphen
    switch(bspNummer)
    {
    case 1:
        ifs.open("daten/Graph1.dat");
        ifs >> eGraph;
        graph = &eGraph;
        break;
    case 2:
        ifs.open("daten/Graph2.dat");
        ifs >> eGraph;
        graph = &eGraph;
        break;
    case 3:
        ifs.open("daten/Graph3.dat");
        ifs >> gGraph;
        graph = &gGraph;
        break;
    case 4:
        ifs.open("daten/Graph4.dat");
        ifs >> tGraph;
        graph = &tGraph;
        break;
    case 5:
        ifs.open("daten/Maze1.dat");
        ifs >> mGraph;
        graph = &mGraph;
        break;
    case 6:
        ifs.open("daten/Maze2.dat");
        ifs >> mGraph;
        graph = &mGraph;
        break;
    case 7:
        ifs.open("daten/Maze3.dat");
        ifs >> mGraph;
        graph = &mGraph;
        break;
    case 8:
        ifs.open("daten/Maze4.dat");
        ifs >> mGraph;
        graph = &mGraph;
        break;
    case 9:
        ifs.open("daten/Maze5.dat");
        ifs >> mGraph;
        graph = &mGraph;
        break;
    default:
        break;
    }
    ifs.close();

    // PruefeHeuristik
    PruefeHeuristik(*graph);

    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg
    vector<CostT> kosten(graph->numVertices());
    TextVisualizer tv;

    Dijkstra(*graph, tv, 0, kosten);
    PruefeDijkstra(bspNummer, 0, kosten);

    return 0;
}

