#pragma once
#include "KSEngine/CoreMinimal.h"

const PositionMatrix TrianglePositions{

	//vertex/single point position of the shape
	
	// x	// y	// z	//Colour			// tex coords
	-0.5f,	-0.5f,	0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// bottom left 0
	 0.5,	-0.5f,	0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom right 1
	 0.0f,	 0.5f,	0.0f,	0.0f, 0.0f, 1.0f,	0.5f, 1.0f	// top 2
};

const IndicesMatrix TriangleIndices{
	0, 1, 2
};

const PositionMatrix SquarePositions{
	// x	// y	// z	//Colour			// tex coords
	-0.5f,	-0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 0.0f,	// bottom left 0
	 0.5,	-0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom right 1
	 0.5f,	 0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f,	// top right 2
	-0.5f,	 0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// top left 3
};

const IndicesMatrix SquareIndices{
	//3 dimention so, we need to use 2 triangle to do square
	0, 3, 1,		//  bottom left 0 , top left 3 , bottom right 1 = Triangle 1 and,
	3, 2, 1			//  top left 3 , top right 2, bottom right 1 = Triangle 2
};





const PositionMatrix CirclePositions{
	// x		// y		// z		//Colour			// tex coords
	0.0f,		0.0f,		0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f,	// Center Point 0
	0.1825f,	0.5f,		0.0f,		0.5f, 0.0f, 0.0f,	0.0f, 0.0f,	// Top Right 1
	0.5f,		0.1825f,	0.0f,		0.5f, 0.0f, 0.0f,	0.0f, 0.0f,	// Middle Top Right 2
	0.5f,		-0.1825f,	0.0f,		0.5f, 0.0f, 0.0f,	0.0f, 0.0f,	// Middle Bottom Right 3
	0.1825f,	-0.5f,		0.0f,		0.5f, 0.0f, 0.0f,	0.0f, 0.0f,	// Bottom Right 4
	-0.1825f,	-0.5f,		0.0f,		0.5f, 0.0f, 0.0f,	0.0f, 0.0f,	// Bottom Left 5
	-0.5f,		-0.1825f,	0.0f,		0.5f, 0.0f, 0.0f,	0.0f, 0.0f,	// Bottom Middle Right 6
	-0.5f,		0.1825f,	0.0f,		0.5f, 0.0f, 0.0f,	0.0f, 0.0f,	// Top Middle Left 7
	-0.1825f,	0.5f,		0.0f,		0.5f, 0.0f, 0.0f,	0.0f, 0.0f,	// Top Left 8
};

const IndicesMatrix CircleIndices{
	0, 8, 1,	// Top Triangle
	0, 1, 2,	// Top Right Triangle
	0, 2, 3,	// Right Triangle
	0, 3, 4,	// Bottom Right Triangle
	0, 4, 5,	// Bottom Triangle
	0, 5, 6,	// Bottom Left Triangle
	0, 6, 7,	// Left Triangle
	0, 7, 8,	// Top Left Triangle
};


const PositionMatrix HeartPositions{

	//vertex/single point position of the shape

	// x	    // y	    // z	    //Colour			// tex coords
	// 1st triangle
	0.125f,		1.0f,		0.0f,		0.45f, 0.0f, 0.8f,	1.0f, 1.0f,	// top 0
	-0.5f,		0.0f,		0.0f,		0.45f, 0.0f, 0.8f,	0.0f, 1.0f,	// bottom left 1
	 0.5f,	    0.0f,	    0.0f,	    0.0f, 0.0f, 1.0f,	0.5f, 1.0f,	// bottom right 2

	// 2nd triangle +1
	0.75f,		1.0f,		0.0f,		0.45f, 0.0f, 0.8f,	1.0f, 1.0f,	// top 3
	1.5f,		0.0f,		0.0f,		0.45f, 0.0f, 0.8f,	0.0f, 1.0f,	// bottom right 4

	// 3rd
	0.5f,	 -1.5f,			0.0f,		0.0f, 0.0f, 1.0f,	0.5f, 1.0f	// bottom 5


};

const IndicesMatrix HeartIndices{
	
	0, 1, 2,  // Top left Triangle
	2, 4, 3,  // Top right Triangle
	1, 4, 5   // Bottom Triangle
};


class VertexArrayObject {
public:
	VertexArrayObject(GeometricShapes ChosenShape);
	~VertexArrayObject();

	void Draw();

private:
	ShapeData Shape;

	// this is our version of unsigned int
	// ID - ID for the VAO
	// VAB - ID for the Vertices
	// EAB - ID for the Indices
	unInt ID, VAB, EAB;
};