// LocalEdgeT <VertexT,CostT>      Wohin & was kostet's   (nicht: Kante & Kosten)
// NeighborT  vector<LocalEdgeT>   enthaelt alle Nachbarn(inkl. Kosten) von einem festen Punkt


#include "text_visualizer.h"
#include "unit.h"
#include "visualizer.h"
#include "maze.h"
#include <fstream>
#include <cmath>
#include "queue"

using std::vector;
using std::cin;
using std::cout;
using std::ifstream;

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

        vector<CoordiB> getCoordis() const { return coordinaten; }

        vector<NeighborT> getAllNeighbors() const { return nachbarn; }

    friend ifstream& operator >> (ifstream& ifs, CoordinateGraph& graph);
};

const DistanceGraph::NeighborT& CoordinateGraph::getNeighbors( VertexT v) const
{
    return nachbarn[v];
}

CostT CoordinateGraph::cost( VertexT from, VertexT to) const
{
    if(from==to)
        return 0;

    NeighborT fromsNachbarn = nachbarn[from];
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

    // Initialisierung der Kosten
    for(size_t i = 0; i<g.numVertices(); i++)
    {
        kostenZumStart[i] = g.cost(start,i);
    }

    // Initalisierung von R (restliche Knoten)
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

class CompareClass {
    public:
        bool operator() (DistanceGraph::LocalEdgeT a, DistanceGraph::LocalEdgeT b)
        {
            return a.second > b.second;
        }
};

bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    cout << "A* start: " << start << " -> " << ziel << " ...\n";
    typedef DistanceGraph::LocalEdgeT LocalEdgeT;
    typedef DistanceGraph::NeighborT NeighborT;

    // speichert Knoten & Kosten da sortiert und dynamisch befuellt wird
    vector<LocalEdgeT> kostenMitH; // mit Heuristik.

    vector<CostT>   kostenOhneH(g.numVertices(), infty); //ohne Heuristik. Index entspr. Knoten
    vector<VertexT> vorgaengers(g.numVertices(), start);
    vector<VertexStatus> status(g.numVertices(), VertexStatus::UnknownVertex);

    // Initalisiere Status und Kostenliste
    status[ziel]  = VertexStatus::Destination;
    status[start] = VertexStatus::Active;
    kostenOhneH[start] = 0;
    kostenMitH.push_back(LocalEdgeT(start,0));

    // Grafik initialisieren
    for(size_t i = 0; i<g.numVertices(); i++)
        v.updateVertex(i, kostenOhneH[i], g.estimatedCost(i, ziel), i, status[i]);
    
    for (size_t i = 0; i < g.numVertices(); i++)
    {
        NeighborT neiV = g.getNeighbors(i);
        for (LocalEdgeT& ne : neiV)
        {            
            v.markEdge(EdgeT(i,ne.first), EdgeStatus::UnknownEdge);
        }        
    }
    v.draw();


    while(kostenMitH.size() > 0) {
        //ermittle minimale Kosten
        std::make_heap(kostenMitH.begin(), kostenMitH.end(), CompareClass());
        VertexT minVert     = kostenMitH[0].first;
        CostT   minKost     = kostenOhneH[minVert];

        // Knoten abgearbeitet
        kostenMitH.erase(kostenMitH.begin());
        status[minVert] = VertexStatus::Done;
        v.markEdge(EdgeT(vorgaengers[minVert],minVert),EdgeStatus::Optimal);
        v.markVertex(minVert, VertexStatus::Active);
        v.draw();

        if(minVert == ziel) // Ziel schon erreicht
            break;

        //aktualisiere Kosten und VorgaengerIn
        NeighborT neiV = g.getNeighbors(minVert);

        for(LocalEdgeT& ne: neiV) {
            if(status[ne.first] == VertexStatus::Done)
                continue;


            CostT alt = kostenOhneH[ne.first]; //ne.first = neighbor
            CostT neu = minKost + ne.second; //ne.second = cost(minVert,neighbor)
            if(neu < alt) {
                kostenOhneH[ne.first] = std::min(alt,neu);
                vorgaengers[ne.first] = minVert;

                if(status[ne.first] != VertexStatus::InQueue)
                    kostenMitH.push_back(LocalEdgeT(ne.first,0));
                    // Kosten = 0. Werden im Anschluss aktualisiert

                status[ne.first] = VertexStatus::InQueue;
            }
            v.updateVertex(ne.first, kostenOhneH[ne.first], g.estimatedCost(ne.first,ziel),
                    vorgaengers[ne.first],status[ne.first]);
            // v.markEdge(EdgeT(minVert,ne.first),EdgeStatus::Active);
            // v.draw();
            v.markEdge(EdgeT(minVert,ne.first),EdgeStatus::Visited);
        }

        for(LocalEdgeT& leH: kostenMitH) {
            leH.second = kostenOhneH[leH.first] + g.estimatedCost(leH.first,ziel);
        }

        v.draw();
        v.markVertex(minVert, VertexStatus::Done);
    }

    if(kostenOhneH[ziel] < infty) {
        VertexT vorgaenger = ziel;
        v.markVertex(ziel, VertexStatus::Destination);
        while (vorgaenger != start)
        {
            weg.push_front(vorgaenger);
            vorgaenger = vorgaengers[vorgaenger];
            v.markVertex(vorgaenger, VertexStatus::Destination);
        }
        weg.push_front(start);
        // Warte bis Ende
        for (size_t i = 0; i < 5; i++)
            v.draw();
        
        
        return true;
    }

    cout << "es scheint keinen Weg " << start << " -> " << ziel << " zu geben\n";
    return false; // Kein Weg gefunden.
}

void inintCoordVisu(CoordVisualizer& cVisu, const CoordinateGraph& cGraph, size_t breit, size_t hoe, sf::RenderWindow*& w)
{
    cVisu.setCoordis(cGraph.getCoordis());
    cVisu.setEdges(cGraph.getAllNeighbors());
    w = new sf::RenderWindow(sf::VideoMode(breit, hoe), "fensterchen");
    cVisu.setWindow(*w);
    cVisu.tranformCoordis(breit-100,hoe-100);
}

void inintMazeVisu(MazeVisualizer& mVisu, const MazeGraph& mGraph, size_t breit, size_t hoe, sf::RenderWindow*& w)
{
    mVisu.setMaze(mGraph.getMaze());
    mVisu.setBreite(mGraph.getBreite());
    w = new sf::RenderWindow(sf::VideoMode(breit, hoe), "fensterchen");
    mVisu.setWindow(*w);
}

int main()
{
    // Frage Beispielnummer vom User ab
    int bspNummer;
    cout << "Hallo Hallihallo,\nIch bins wieder euer c++ Programm!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        << "Welche Datei darfs denn sein? Habe 1-4 (Graphen) und 5-9 (Labyrinthszor) oder 10 (random Labyrinth, aber is weird)\n"
        << "bitte jetzt eine Zahl von 1-10 eintippen (nichts anderes sonst kommen wir nicht klar). Jetzt bitte wirklich\n";
    cin >> bspNummer;

    DistanceGraph*    graph;
    GraphVisualizer*  visu;

    EuclidGraph     eGraph;
    GeoAbstandGraph gGraph;
    TimeGraph       tGraph;
    MazeGraph       mGraph;
    CoordVisualizer cVisu;
    MazeVisualizer  mVisu;

    ifstream ifs;
    EdgeT pair10; // Start und Ziel Paar fuer Bsp. 10

    sf::RenderWindow* winnie;

    // Lade die zugehoerige Textdatei in einen Graphen
    switch(bspNummer)
    {
    case 1:
        ifs.open("daten/Graph1.dat");
        ifs >> eGraph;
        inintCoordVisu(cVisu, eGraph, 600, 600, winnie);
        graph = &eGraph;
        break;
    case 2:
        ifs.open("daten/Graph2.dat");
        ifs >> eGraph;
        inintCoordVisu(cVisu, eGraph, 1000, 1000, winnie);
        graph = &eGraph;
        break;
    case 3:
        ifs.open("daten/Graph3.dat");
        ifs >> gGraph;
        inintCoordVisu(cVisu, gGraph, 1400, 900, winnie);
        graph = &gGraph;
        break;
    case 4:
        ifs.open("daten/Graph4.dat");
        ifs >> tGraph;
        inintCoordVisu(cVisu, tGraph, 850, 1000, winnie);
        graph = &tGraph;
        break;
    case 5:
        ifs.open("daten/Maze1.dat");
        ifs >> mGraph;
        inintMazeVisu(mVisu, mGraph, 500, 500, winnie);
        graph = &mGraph;
        break;
    case 6:
        ifs.open("daten/Maze2.dat");
        ifs >> mGraph;
        inintMazeVisu(mVisu, mGraph, 500, 500, winnie);
        graph = &mGraph;
        break;
    case 7:
        ifs.open("daten/Maze3.dat");
        ifs >> mGraph;
        inintMazeVisu(mVisu, mGraph, 850, 850, winnie);
        graph = &mGraph;
        break;
    case 8:
        ifs.open("daten/Maze4.dat");
        ifs >> mGraph;
        inintMazeVisu(mVisu, mGraph, 1000, 1000, winnie);
        graph = &mGraph;
        break;
    case 9:
        ifs.open("daten/Maze5.dat");
        ifs >> mGraph;
        inintMazeVisu(mVisu, mGraph, 1000, 1000, winnie);
        graph = &mGraph;
        break;
    case 10:
        mGraph.setzeMaze(ErzeugeLabyrinth(256, 256, 1));
        mGraph.setzeHoehe(256);
        mGraph.setzeBreite(256);
        mGraph.setzeNachbarn();
        pair10.first  = mGraph.getStart();
        pair10.second = mGraph.getZiel();
        inintMazeVisu(mVisu, mGraph, 1200, 1000, winnie);
        graph = &mGraph;
        break;
    default:
        break;
    }
    ifs.close();

    if (bspNummer < 5)
        visu = &cVisu;        
    else visu = &mVisu;

    // PruefeHeuristik
    if (bspNummer != 10) PruefeHeuristik(*graph);

    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg
    vector<CostT> kosten(graph->numVertices());
    TextVisualizer tv;

    if(bspNummer < 5)
    {
        // fuer alle Startknoten
        for(VertexT start = 0; start < graph->numVertices(); start++)
        {
            Dijkstra(*graph, tv, start, kosten);
            PruefeDijkstra(bspNummer, start, kosten);

            for(VertexT ziel = 0; ziel < graph->numVertices(); ziel++)
            {
                std::list<VertexT> weg;
                if(A_star(*graph, cVisu, start, ziel, weg))
                    PruefeWeg(bspNummer, weg);
            }
        }
    }

    // fuer Beispiele 5-9
    for ( auto pair : StartZielPaare(bspNummer)) {
        auto start = pair.first;
        auto goal  = pair.second;
        std::list<VertexT> weg;

        // (Berechne den kuerzesten Weg von start zu goal)
        if(A_star(*graph, mVisu, start, goal, weg))
            PruefeWeg(bspNummer, weg);
    }

    if(bspNummer == 10)
    {
        auto start = pair10.first;
        auto goal  = pair10.second;
        std::list<VertexT> weg;

        // (Berechne den kuerzesten Weg von start zu goal)
        if(A_star(*graph, mVisu, start, goal, weg))
            PruefeWeg(bspNummer, weg);
    }



    while (winnie->isOpen())
    {
        visu->draw();
    }

    return 0;
}

