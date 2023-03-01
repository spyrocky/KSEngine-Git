#pragma once
#include "KSEngine/CoreMinimal.h"

const PositionMatrix TrianglePositions{
	// x	// y	// z	//Colour			// tex coords
	-0.5f,	-0.5f,	0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// bottom left
	 0.5,	-0.5f,	0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom right
	 0.0f,	 0.5f,	0.0f,	0.0f, 0.0f, 1.0f,	0.5f, 1.0f	// top
};

const IndicesMatrix TriangleIndices{
	0, 1, 2
};

const PositionMatrix SquarePositions{
	// x	// y	// z	//Colour			// tex coords
	-0.5f,	-0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 0.0f,	// bottom left
	 0.5,	-0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom right
	 0.5f,	 0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f,	// top right
	-0.5f,	 0.5f,	0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,	// top left
};

const IndicesMatrix SquareIndices{
	0, 3, 1,		// Triangle 1
	3, 2, 1			// Triangle 2
};

const PositionMatrix TrapeziumPositions{
	// x	// y	// z	//Colour			// tex coorcs
	-0.5f,	-0.5f,	0.0f,	0.45f, 0.0f, 0.8f,	0.0f, 0.0f,	// bottom left
	 0.5,	-0.5f,	0.0f,	0.45f, 0.0f, 0.8f,	1.0f, 0.0f,	// bottom right
	 0.25f,	 0.25f,	0.0f,	0.45f, 0.0f, 0.8f,	1.0f, 1.0f,	// top right
	-0.25f,	 0.25f,	0.0f,	0.45f, 0.0f, 0.8f,	0.0f, 1.0f,	// top left
};

const IndicesMatrix TrapeziumIndices{
	0, 2, 3,		// Triangle 1
	0, 1, 2			// Triangle 2
};

const PositionMatrix CirclePositions{
	// x		// y		// z		//Colour			// tex coords
	0.0f,		0.0f,		0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// Center Point
	0.1825f,	0.5f,		0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// Top Right
	0.5f,		0.1825f,	0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// Middle Top Right
	0.5f,		-0.1825f,	0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// Middle Bottom Right
	0.1825f,	-0.5f,		0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// Bottom Right
	-0.1825f,	-0.5f,		0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// Bottom Left
	-0.5f,		-0.1825f,	0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// Bottom Middle Right
	-0.5f,		0.1825f,	0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// Top Middle Left
	-0.1825f,	0.5f,		0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,	// Top Left
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