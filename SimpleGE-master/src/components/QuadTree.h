#pragma once

#include <iostream>
#include <cmath>
#include <simplege/components/collider.h> 

struct Rectangle
{
    //Position en haut à gauche du rectangle
    float x ; 
    float y ;
    //dimension du rectangle
    float width ;
    float height ;

	Rectangle(float _x, float _y, float _width, float _height)
	{
		x = _x;
		y = _y;
        width = _width ;
        height = _height ;
	}
};

class QuadTree
{
    public :
    Rectangle rectangle ;

    private :
    int maxComponent = 2;
    int maxLevels = 5 ;
    int level ;
    std::vector<ColliderComponent>;
    QuadTree topLeft;
    QuadTree topRight;
    QuadTree botLeft;
    QuadTree botRight;
    

    

    /*
      
  private List objects;
  private Quadtree[] nodes;
    */
};
