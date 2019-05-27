#include "maze.h"
#include<string>
#include<iostream>
#include<cmath>

const DistanceGraph::NeighborT& MazeGraph::getNeighbors( VertexT v) const
{
    return nachbarn[v];
}

CostT MazeGraph::estimatedCost( VertexT from, VertexT to) const
{
    size_t zeileFrom = from/breit;
    size_t spaltFrom = from%breit;
    size_t zeileTo = to/breit;
    size_t spaltTo = to%breit;

    double dx = (double)spaltFrom - (double)spaltTo;
    double dy = (double)zeileFrom - (double)zeileTo;

    return sqrt(dx*dx + dy*dy);
}

CostT MazeGraph::cost( VertexT from, VertexT to) const
{
    if(from == to)
        return 0;
    
    NeighborT fromsNachbarn = nachbarn[from];
    for(LocalEdgeT v : fromsNachbarn)
        if(v.first == to)
            return v.second;

    return infty;
}

// liest maze-Vektor um Nachbarn zu merken
void MazeGraph::setzeNachbarn() 
{
    nachbarn.resize(hoe*breit);
    setVertexCount(hoe*breit);

    // ermittle Start und Ziel
    for(size_t i = 0; i < hoe*breit; i++) {
        if ( maze[i] == CellType::Start )
            start   = i;
        else if( maze[i] == CellType::Destination )
            ziel    = i;
    }

    // wer ist von wem Nachbar?
    for(size_t i = 0; i < hoe*breit; i++)
    {
        size_t zeile = i/breit;
        size_t spalt = i%breit;

        if( maze[i] != CellType::Wall )
        {
            if(zeile >= 1)
                if(maze[i - breit] != CellType::Wall) {
                    DistanceGraph::LocalEdgeT oben(i-breit,1);
                    nachbarn[i].push_back(oben);
                }

            if(zeile < hoe-1)
                if(maze[i + breit] != CellType::Wall) {
                    DistanceGraph::LocalEdgeT unten(i+breit,1);
                    nachbarn[i].push_back(unten);
                }

            if(spalt >= 1)
                if(maze[i-1] != CellType::Wall) {
                    DistanceGraph::LocalEdgeT links(i-1,1);
                    nachbarn[i].push_back(links);
                }

            if(spalt < breit-1)
                if(maze[i+1] != CellType::Wall) {
                    DistanceGraph::LocalEdgeT rechts(i+1,1);
                    nachbarn[i].push_back(rechts);
                }
        }
    }
}

ifstream& operator >> (ifstream& ifs, MazeGraph& graph)
{
    size_t hoe, breit;

    ifs >> hoe >> breit;

    graph.hoe   = hoe;
    graph.breit = breit;

    std::string mazeString = "";
    for(size_t i = 0; i < hoe; i++)
    {
        std::string zeile;
        ifs >> zeile;
        std::cout << zeile << '\n';
        mazeString.append(zeile);
    }

    for(size_t i = 0; i < hoe*breit; i++)
    {
        if(mazeString[i] == '#')
            graph.maze.push_back(CellType::Wall);
        else if(mazeString[i] == '.')
            graph.maze.push_back(CellType::Ground);
    }

    graph.setzeNachbarn();
    return ifs;
}
