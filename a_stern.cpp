// LocalEdgeT <VertexT,CostT>      Wohin & was kostet's   (nicht: Kante & Kosten)
// NeighborT  vector<LocalEdgeT>   enthaelt alle Nachbarn(inkl. Kosten) von einem festen Punkt


#include "text_visualizer.h"
#include "unit.h"
#include <fstream>

using std::vector;

// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph
{
    public:
        typedef std::pair<double,double> CoordiB;

    protected:
        vector<NeighborT> nachbarn;
        vector<CoordiB> coordinaten;

    public:
        const NeighborT& getNeighbors( VertexT v) const override;

        //CostT estimatedCost( VertexT from, VertexT to) const override;

        CostT cost( VertexT from, VertexT to) const override;

    friend ifstream& operator >> (ifstream& ifs, const CoordinateGraph& graph);
};

ifstream& operator >> (ifstream& ifs, const CoordinateGraph& graph)
{
    int knotenAnz, kantenAnz;

    ifs >> knotenAnz >> kantenAnz;

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

            return sqrt(dx*dx + dy*dy);
        }
};


void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start, std::vector<CostT>& kostenZumStart) {
    // ...
}

bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    // ...
    return false; // Kein Weg gefunden.
}

int main()
{
    // Frage Beispielnummer vom User ab

    // Lade die zugehoerige Textdatei in einen Graphen
    // PruefeHeuristik

    // Loese die in der Aufgabenstellung beschriebenen Probleme fuer die jeweilige Datei
    // PruefeDijkstra / PruefeWeg

    return 0;
}

