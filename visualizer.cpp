#include "visualizer.h"
#include <iostream>
#include <cmath>

using std::cout;
using std::string;

// Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void CoordVisualizer::markVertex(VertexT vertex, VertexStatus status)
{
    vStatus[vertex] = status;
}

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void CoordVisualizer::markEdge(EdgeT e, EdgeStatus status)
{
    for(std::pair<VertexT, EdgeStatus>& p : eStatus[e.first] )
        if(p.first == e.second)
            p.second = status;

}

// Aktualisiere jegliche Daten eines Knotens.
void CoordVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{
    costos[vertex]    = cost;
    estimatos[vertex] = estimate;
    vStatus[vertex]    = status;

    // EdgeStatus je nach VertexStatus
    if(parent != vertex)
    {
        switch (status)
        {
        case VertexStatus::Active :
            markEdge(EdgeT(parent,vertex),EdgeStatus::Active);
            break;
        case VertexStatus::Done :
            markEdge(EdgeT(parent,vertex),EdgeStatus::Optimal);            
            break;
        case VertexStatus::InQueue :
            markEdge(EdgeT(parent,vertex),EdgeStatus::Visited);
            break;
        case VertexStatus::UnknownVertex :
            markEdge(EdgeT(parent,vertex),EdgeStatus::UnknownEdge);
            break;
        default: // VertexStatus::Destination
            break;
        }
    }
}

// Zeichne den aktuellen Zustand des Graphen.
void CoordVisualizer::draw() {
    // run the program as long as the window is open

    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (w->pollEvent(event)) // event loop
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            w->close();
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::C)
            {
                showCost = !showCost;
            }
        }
    }

    // start the current frame: clear the window with black color
    w->clear(sf::Color::Black);

    // Kanten
    size_t from = 0;
    for (DistanceGraph::NeighborT nn : nachbarn )
    {
        for (size_t i = 0; i < nn.size(); i++)
        {
            DistanceGraph::LocalEdgeT e = nn[i]; // Kante (nachbarn-Index "from" -> e.first)
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(coordinaten[from].first , coordinaten[from].second )),
                sf::Vertex(sf::Vector2f(coordinaten[e.first].first ,coordinaten[e.first].second ))
            };

            // Farbe
            switch(eStatus[from][i].second)
            {
            case EdgeStatus::UnknownEdge:
                line[0].color = sf::Color(50, 50, 50);
                break;
            case EdgeStatus::Visited:
                line[0].color = (sf::Color(255, 100, 100));
                break;
            case EdgeStatus::Active:
                line[0].color = (sf::Color(80, 200, 80));
                break;
            case EdgeStatus::Optimal:
                line[0].color = (sf::Color(100, 100, 255));
                break;
            default:
                break;
            }
            line[1].color = line[0].color;

            bool draw = true;
            // Unbekannte Kante nicht zeichnen falls Rueckrichtung bekannt
            // Rueckrichtung: e.first -> from
            if(eStatus[from][i].second == EdgeStatus::UnknownEdge)
                for(auto p : eStatus[e.first])
                    if(p.first == from && p.second != EdgeStatus::UnknownEdge)
                        draw = false;

            if(draw)
                w->draw(line, 2, sf::Lines); //draws the line

            // Arrow
            drawArrowTxt(from,e.first, line[0].color , std::to_string(e.second));
        }
        from++;
    }




    // Knoten
    int i = 0;
    for (CoordiB c: coordinaten)
    {
        sf::CircleShape shape(20);

        // Farbe
        switch(vStatus[i])
        {
            case VertexStatus::UnknownVertex:
                shape.setRadius(shape.getRadius()-4);
                shape.setFillColor(sf::Color(20,20,20));
                shape.setOutlineThickness(2);
                shape.setOutlineColor(sf::Color(120, 120, 120));
                break;
            case VertexStatus::InQueue:
                shape.setFillColor(sf::Color(255, 100, 100));
                break;
            case VertexStatus::Active:
                shape.setFillColor(sf::Color(80, 200, 80));
                break;
            case VertexStatus::Done:
                shape.setFillColor(sf::Color(120, 80, 80));
                break;
            case VertexStatus::Destination:
                shape.setFillColor(sf::Color(100, 100, 255));
                break;
            default:
                break;
        }


        //sets the (absolute) position of the circle, (0,0) is the top-left corner.
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(c.first,c.second);

        w->draw(shape);


        //Text  g+h
        if(showCost)
        {
            string txt = "?";
            if(costos[i] != infty) {
                txt = std::to_string(costos[i]);
                txt = txt.substr(0, txt.find(".") + 2);
            }
            txt.append(" + ");
            txt.append(std::to_string(estimatos[i]));
            txt = txt.substr(0, txt.find_last_of(".") + 2);

            sf::Text text(txt, myFont, 16);
            text.setFillColor(sf::Color(22,250,229));
            text.setPosition(c.first-8,c.second-8);
            w->draw(text);
        }
        
        i++;
    }

    sf::sleep(sf::milliseconds(400));
    w->display();

}

void CoordVisualizer::tranformCoordis(size_t breit, size_t hoe)
{
    double minix=coordinaten[0].second;
    double miniy=coordinaten[0].first;
    double maxix=coordinaten[0].second;
    double maxiy=coordinaten[0].first;

    for (CoordiB c: coordinaten)
    {
        if(c.second < minix)
            minix = c.second;
        if(c.first < miniy)
            miniy = c.first;
        if(c.second > maxix)
            maxix = c.second;
        if(c.first > maxiy)
            maxiy = c.first;
    }

    double scalex= breit/(maxix - minix);
    double scaley= hoe/(maxiy-miniy);
    for (CoordiB& c: coordinaten)
    {
        double h = c.second*scalex - minix*scalex + 50;
        c.second = c.first*scaley - miniy*scaley + 50;
        c.first  = h;
    }
}


void CoordVisualizer::setWindow(sf::RenderWindow& window)
{
    w = &window;
}

void CoordVisualizer::setCoordis(const vector<CoordiB>& c)
{
    coordinaten = c;
    costos.resize(c.size(), infty);
    estimatos.resize(c.size());    
    vStatus.resize(c.size(), VertexStatus::UnknownVertex);
    if(!myFont.loadFromFile("font/BebasNeue-Regular.ttf")) {    }
}

void CoordVisualizer::setEdges(const vector<DistanceGraph::NeighborT>& n) {
    nachbarn = n;
    
    eStatus.resize(nachbarn.size());

    for (size_t i = 0; i < nachbarn.size(); i++)
    {
        eStatus[i].resize(nachbarn[i].size());

        for (size_t j = 0; j < nachbarn[i].size(); j++)
        {
            eStatus[i][j] = std::pair<VertexT,EdgeStatus>
                                (nachbarn[i][j].first, EdgeStatus::UnknownEdge);
        }
    }
}

void CoordVisualizer::drawArrowTxt(VertexT from, VertexT to, sf::Color col, std::string txt)
{
    double dx   = coordinaten[to].first  - coordinaten[from].first; 
    double dy   = coordinaten[to].second - coordinaten[from].second;
    double norm = std::sqrt(dx*dx+dy*dy);
    double nx = dx / norm; // Vektor (nx,ny) hat
    double ny = dy / norm; // Laenge 1
    int tx = -8*(dy / norm);
    int ty = 8*(dx / norm);

    double x1 = coordinaten[from].first + 0.80*dx  + tx;
    double y1 = coordinaten[from].second + 0.80*dy  + ty;

    double x2 = coordinaten[from].first + 0.80*dx  - tx;
    double y2 = coordinaten[from].second + 0.80*dy  - ty;

    double x3 = coordinaten[from].first + 0.80*dx + 8*nx;
    double y3 = coordinaten[from].second + 0.80*dy + 8*ny;


    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1,y1)),
        sf::Vertex(sf::Vector2f(x2,y2)),
        sf::Vertex(sf::Vector2f(x3,y3))
    };
    for(sf::Vertex& v : line)
        v.color = col;

    w->draw(line, 3, sf::Triangles);

    // Text
    txt = txt.substr(0, txt.find(".") + 2); // loesche Nachkommastellen
    sf::Text text(txt, myFont, 12);
    text.setFillColor(sf::Color(150,150,149));
    text.setPosition(x1+tx-5,
                     y1+ty-5);
    w->draw(text);
}









// Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void MazeVisualizer::markVertex(VertexT vertex, VertexStatus status)
{
    vStatus[vertex] = status;
}

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void MazeVisualizer::markEdge(EdgeT e, EdgeStatus status)
{ } // nicht benutzt


// Aktualisiere jegliche Daten eines Knotens.
void MazeVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{
    costos[vertex]    = cost;
    estimatos[vertex] = estimate;
    vStatus[vertex]   = status;
}

// Zeichne den aktuellen Zustand des Graphen.
void MazeVisualizer::draw()
{
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (w->pollEvent(event)) // event loop
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            w->close();
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::C)
            {
                showCost = !showCost;
            }
        }
    }

    

    // start the current frame: clear the window with black color
    w->clear(sf::Color::Black);

    // Knoten
    for (size_t i = 0; i < maze.size(); i++)
    {
        // Farbe
        if(maze[i] != CellType::Wall)
        {
            switch(vStatus[i])
            {
                case VertexStatus::UnknownVertex:
                    drawCell(i, sf::Color(120, 120, 120));
                    break;
                case VertexStatus::InQueue:
                    drawCell(i, sf::Color(255, 100, 100));
                    break;
                case VertexStatus::Active:
                    drawCell(i, sf::Color(80, 200, 80));
                    break;
                case VertexStatus::Done:
                    drawCell(i, sf::Color(120, 80, 80));
                    break;
                case VertexStatus::Destination:
                    drawCell(i, sf::Color(100, 100, 255));
                    break;
                default:
                    break;
            }
        }
    }

    sf::sleep(sf::milliseconds(40));
    w->display();

}

void MazeVisualizer::drawCell(VertexT i, sf::Color col)
{
    double cellWidth = (w->getSize().x - 20) / breit;
    size_t zeile  = i / breit;
    size_t spalte = i % breit;

    double x1 = spalte*cellWidth;
    double y1 = zeile*cellWidth;

    double x2 = x1 + cellWidth - 1;
    double y2 = y1;

    double x3 = x2;
    double y3 = y2 + cellWidth - 1;   

    double x4 = x1;
    double y4 = y1 + cellWidth - 1;

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1,y1)),
        sf::Vertex(sf::Vector2f(x2,y2)),
        sf::Vertex(sf::Vector2f(x3,y3)),
        sf::Vertex(sf::Vector2f(x4,y4))
    };
    for(sf::Vertex& v : line)
        v.color = col;

    w->draw(line, 4, sf::Quads);


    // Text  g+h
    if(showCost)
    {
        string txt = "?";
        if(costos[i] != infty) {
            txt = std::to_string(costos[i]);
            txt = txt.substr(0, txt.find(".") + 2);
        }
        txt.append("\n+ ");
        txt.append(std::to_string(estimatos[i]));
        txt = txt.substr(0, txt.find_last_of(".") + 2);

        sf::Text text(txt, myFont, 16);
        if(breit > 25)
            text.setCharacterSize(12);

        text.setFillColor(sf::Color(22,250,229));
        text.setPosition(x1+3,y1+3);
        w->draw(text);
    }
}

void MazeVisualizer::setMaze(const vector<CellType>& maze)
{
    this->maze = maze;
    vStatus.resize(maze.size());
    costos.resize(maze.size());
    estimatos.resize(maze.size());
    if(!myFont.loadFromFile("font/BebasNeue-Regular.ttf")) {    }
}