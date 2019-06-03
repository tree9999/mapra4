#ifndef VISUALIZER_H_INCLUDED
#define VISUALIZER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "unit.h"
#include <string>

using std::vector;

class CoordVisualizer : public GraphVisualizer
{
    private:
        sf::RenderWindow* w;
        sf::Font myFont;

        bool showCost = true;
        typedef std::pair<double,double> CoordiB;
        vector<CoordiB>   coordinaten;
        //VertexStatus:  UnknownVertex, InQueue, Active, Done, Destination
        vector<DistanceGraph::NeighborT> nachbarn;
        vector<VertexStatus>        vStatus;
        vector<vector<std::pair<VertexT, EdgeStatus>>>  eStatus;
        vector<double>   costos;
        vector<double>   estimatos;


    public:
        // Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
        virtual void markVertex(VertexT vertex, VertexStatus status) ;

        // Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
        virtual void markEdge(EdgeT e, EdgeStatus status) ;

        // Aktualisiere jegliche Daten eines Knotens.
        virtual void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) ;

        // Zeichne den aktuellen Zustand des Graphen.
        virtual void draw() ;

        void setWindow(sf::RenderWindow&);

        void setCoordis(const vector<CoordiB>& c);

        void setEdges(const vector<DistanceGraph::NeighborT>& n);

        void tranformCoordis(size_t hoe, size_t breit);

        void drawArrowTxt(VertexT from, VertexT to, sf::Color col, std::string);
};





class MazeVisualizer : public GraphVisualizer
{
  private:
    sf::RenderWindow* w;
    sf::Font myFont;

    size_t breit;
    bool showCost = true;

    vector<CellType> maze;
    vector<VertexStatus> vStatus;
    vector<double>   costos;
    vector<double>   estimatos;

  public:
    // Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
    virtual void markVertex(VertexT vertex, VertexStatus status) ;

    // Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
    virtual void markEdge(EdgeT e, EdgeStatus status) ;

    // Aktualisiere jegliche Daten eines Knotens.
    virtual void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) ;

    // Zeichne den aktuellen Zustand des Graphen.
    virtual void draw() ;

    void drawCell(VertexT, sf::Color col);

    void setWindow(sf::RenderWindow& window) { w = &window; }

    void setBreite(size_t breit) { this->breit = breit; }
    
    void setMaze(const vector<CellType>& maze);
};

#endif // VISUALIZER_H_INCLUDED

