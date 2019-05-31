#include "visualizer.h"
#include <iostream>
#include <cmath>

using std::cout;
using std::string;

// Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
void CoordVisualizer::markVertex(VertexT vertex, VertexStatus status)
{

}

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void CoordVisualizer::markEdge(EdgeT e, EdgeStatus status)
{

}

// Aktualisiere jegliche Daten eines Knotens.
void CoordVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{
    costos[vertex]    = cost;
    estimatos[vertex] = estimate;
    vStatus[vertex]    = status;


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
    }

    // start the current frame: clear the window with black color
    w->clear(sf::Color::Black);

    // Kanten
    int from = 0;
    for (DistanceGraph::NeighborT nn : nachbarn )
    {
        for (DistanceGraph::LocalEdgeT e : nn)
        {
             sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(coordinaten[from].first , coordinaten[from].second )),
                sf::Vertex(sf::Vector2f(coordinaten[e.first].first ,coordinaten[e.first].second ))
            };
            w->draw(line, 2, sf::Lines); //draws the line

            // Arrow
            drawArrowTxt(from,e.first,sf::Color(255, 10, 125), std::to_string(e.second));



        }
        from++;
    }




    // Knoten
    int i = 0;
    for (CoordiB c: coordinaten)
    {
        sf::CircleShape shape(10);

        // set the shape color to green
        switch(vStatus[i])
        {
            case VertexStatus::UnknownVertex:
                shape.setFillColor(sf::Color(255, 255, 255));
                break;
            case VertexStatus::InQueue:
                shape.setFillColor(sf::Color(186, 214, 228));
                break;
            case VertexStatus::Active:
                shape.setFillColor(sf::Color(213, 255, 255));
                break;
            case VertexStatus::Done:
                shape.setFillColor(sf::Color(227, 199, 220));
                break;
            case VertexStatus::Destination:
                shape.setFillColor(sf::Color(213, 255, 155));
                break;
            default:
                break;
        }



        shape.setOrigin(shape.getRadius(), shape.getRadius());
        //sets the (absolute) position of the circle, (0,0) is the top-left corner.
        shape.setPosition(c.first,c.second); // positioning the circle 200 units right from the top left corner

        w->draw(shape);


        //Text  g+h

        string txt = "?";
        if(costos[i] != infty)
            txt = std::to_string(costos[i]);
        txt.append(" + ");
        txt.append(std::to_string(estimatos[i]));

        sf::Text text(txt, myFont, 12);
        text.setFillColor(sf::Color(22,250,229));
        text.setPosition(c.first-6,c.second-6);
        w->draw(text);

        i++;
    }

    sf::sleep(sf::milliseconds(200));
    w->display();

}

void CoordVisualizer::tranformCoordis(size_t breit, size_t hoe)
{
    double minix=coordinaten[0].first;
    double miniy=coordinaten[0].second;
    double maxix=coordinaten[0].first;
    double maxiy=coordinaten[0].second;

    for (CoordiB c: coordinaten)
    {
        if(c.first < minix)
            minix = c.first;
        if(c.second < miniy)
            miniy = c.second;
        if(c.first > maxix)
            maxix = c.first;
        if(c.second > maxiy)
            maxiy = c.second;
    }

    double scalex= breit/(maxix - minix);
    double scaley= hoe/(maxiy-miniy);
    cout << scalex << ", " <<scaley << '\n';
    for (CoordiB& c: coordinaten)
    {
        c.first=c.first*scalex - minix*scalex + 50;
        c.second=c.second*scaley - miniy*scaley + 50;
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
    eStatus.resize(c.size());
    if(!myFont.loadFromFile("font/BebasNeue-Regular.ttf")) {    }
}


void CoordVisualizer::drawArrowTxt(VertexT from, VertexT to, sf::Color col, std::string txt)
{
    double dx   = coordinaten[to].first  - coordinaten[from].first;
    double dy   = coordinaten[to].second - coordinaten[from].second;
    double norm = std::sqrt(dx*dx+dy*dy);
    int tx = -8*(dy / norm);
    int ty = 8*(dx / norm);

    double x1 = coordinaten[from].first + 0.84*dx  + tx;
    double y1 = coordinaten[from].second + 0.84*dy  + ty;

    double x2 = coordinaten[from].first + 0.84*dx  - tx;
    double y2 = coordinaten[from].second + 0.84*dy  - ty;

    double x3 = coordinaten[from].first + 0.90*dx;
    double y3 = coordinaten[from].second + 0.90*dy;


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
void markVertex(VertexT vertex, VertexStatus status) ;

// Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
void markEdge(EdgeT e, EdgeStatus status) ;

// Aktualisiere jegliche Daten eines Knotens.
void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) ;

// Zeichne den aktuellen Zustand des Graphen.
void draw() ;

