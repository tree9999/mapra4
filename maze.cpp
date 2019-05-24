#include "maze.h"
#include<string>

const NeighborT& MazeGraph::getNeighbors( VertexT v) const
{
    return nachbarn[v];
}

CostT MazeGraph::estimatedCost( VertexT from, VertexT to) const
{
    size_t zeileFrom = from/breit;
    size_t spaltFrom = from%breit;
    size_t zeileTo = to/breit;
    size_t spaltTo = to%breit;

    double dx = zeileFrom - zeileTo;
    double dy = spaltFrom - spaltTo;

    return sqrt(dx*dx + dy*dy);
}

CostT MazeGraph::cost( VertexT from, VertexT to) const
{
    NeighborT fromsNachbarn = nachbarn[from];
    for(LocalEdgeT v : fromsNachbarn)
        if(v.first == to)
            return v.second;

    return infty;
}

ifstream& operator >> (ifstream& ifs, const MazeGraph& graph)
{
    size_t hoe, breit;

    ifs >> hoe >> breit;

    graph.vertexCount = hoe*breit;
    graph.nachbarn.resize(hoe*breit);
    graph.breit = breit;

    string mazeString = "";
    for(int i = 0; i < hoe; i++)
    {
        string zeile;
        ifs >> zeile;

        mazeString += zeile;
    }

    for(size_t i = 0; i < hoe*breit; i++)
    {
        if(mazeString[i] == '#')
            graph.maze.push_back(CellType::Wall);
        else if(mazeString[i] == '.')
            graph.maze.push_back(CellType::Ground);
    }


    // wer ist von wem Nachbar?
    for(size_t i = 0; i < hoe*breit; i++)
    {
        size_t zeile = i/breit;
        size_t spalt = i%breit;

        if(zeile - breit >= 0)
            if(graph.maze[zeile - breit] == CellType::Ground) {
                DistanceGraph::LocalEdgeT oben(zeile-breit,1);
                graph.nachbarn[i].push_back(oben);
            }

        if(zeile + breit < graph.vertexCount)
            if(graph.maze[zeile + breit] == CellType::Ground) {
                DistanceGraph::LocalEdgeT unten(zeile+breit,1);
                graph.nachbarn[i].push_back(unten);
            }

        if((i-1)/breit == zeile && i-1 >= 0)
            if(graph.maze[i-1] == CellType::Ground) {
                DistanceGraph::LocalEdgeT links(i-1);
                graph.nachbarn[i].push_back(links);
            }

        if((i+1)/breit == zeile && i+1 < graph.vertexCount)
            if(graph.maze[i+1] == CellType::Ground) {
                DistanceGraph::LocalEdgeT rechts(i+1);
                graph.nachbarn[i].push_back(rechts);
            }
    }
}
