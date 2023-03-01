#include "KSEngine/Graphics/VertexArrayObject.h"
#include "GL/glew.h"

VertexArrayObject::VertexArrayObject(GeometricShapes ChosenShape)
{
	ID = VAB = EAB = 0;

	// localised version of the chosen matrices
	PositionMatrix ChosenPositions = PositionMatrix();
	IndicesMatrix ChosenIndices = IndicesMatrix();

	// switch the chosen matrices depending on the selected geometry type
	switch (ChosenShape)
	{
	case GeometricShapes::Triangle:
		ChosenPositions = TrianglePositions;
		ChosenIndices = TriangleIndices;
		break;
	case GeometricShapes::Square:
		ChosenPositions = SquarePositions;
		ChosenIndices = SquareIndices;
		break;
	case GeometricShapes::Trapezium:
		ChosenPositions = TrapeziumPositions;
		ChosenIndices = TrapeziumIndices;
		break;
	case GeometricShapes::Circle:
		ChosenPositions = CirclePositions;
		ChosenIndices = CircleIndices;
		break;
	default:
		break;
	}

	Shape.PositionMatrix = ChosenPositions;
	Shape.IndicesMatrix = ChosenIndices;

	// create the ID for our VAO
	glGenVertexArrays(1, &ID);

	// bind the data to this vertex array
	glBindVertexArray(ID);

	// HANDLE THE POSITIONS
	// create an ID for our vertex array buffer
	glGenBuffers(1, &VAB);
	// bind the above IDs to OpenGL as the array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VAB);
	// Run through the data and attach the vertices to our VAB
	glBufferData(
		GL_ARRAY_BUFFER,
		Shape.PositionMatrix.size() * sizeof(float),
		&Shape.PositionMatrix[0],
		GL_STATIC_DRAW
	);

	// HANDEL THE INDICES
	// create an ID for our element array buffer
	glGenBuffers(1, &EAB);
	// bind the above IDs to OpenGL as the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB);
	// Run through the data and attach the vertices to our VAB
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		Shape.IndicesMatrix.size() * sizeof(unInt),
		&Shape.IndicesMatrix[0],
		GL_STATIC_DRAW
	);

	// Assign the vertices and indices to the VAO
	glVertexAttribPointer(
		0,					// Data Set - 0 = the first data set in the array
		3,					// How many numbers in our matrix to make a triangle
		GL_FLOAT, GL_FALSE,	// data type, whether you want to normalise the values
		sizeof(float) * 8,	// stride - the length it takes to get to each number
		(void*)0			//offset of how many numbers we skip in the matrix
	);

	// enable the vertex array
	glEnableVertexAttribArray(0);

	// Assign the colour to the shader
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT, GL_FALSE,
		sizeof(float) * 8,
		(void*)(3 * sizeof(float))
	);

	// enabling the colour array
	glEnableVertexAttribArray(1);

	// Assign the texture coordinates to the shader
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT, GL_FALSE,
		sizeof(float) * 8,
		(void*)(6 * sizeof(float))
	);

	// enabling the texture coordinate array
	glEnableVertexAttribArray(2);

	// clear the buffer
	glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject()
{
	// clean up the VAO in OpenGL
	glDeleteVertexArrays(1, &ID);

	// Clean up the vectors
	Shape.PositionMatrix.clear();
	Shape.IndicesMatrix.clear();

	cout << "Deleted VAO..." << endl;
}

void VertexArrayObject::Draw()
{
	// bind our VAO to the current buffer
	glBindVertexArray(ID);
	// Draw the 3D object/VAO
	glDrawElements(
		GL_TRIANGLES,					// what type of objects are we drawing
		Shape.IndicesMatrix.size(),		// how many vertices do we draw
		GL_UNSIGNED_INT,				// what is the type of data that's being input
		(void*)0						// how many vertices should we skip
	);
	// clear the VAO from the current array for the next object
	glBindVertexArray(0);
}