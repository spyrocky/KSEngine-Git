#pragma once
#include "KSEngine/CoreMinimal.h"
#include "glm/glm.hpp"

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	// this will initialise our shader as a Vertex Fragment Shader
	bool InitVFShader(VFShaderParams ShaderFilePaths);

	// run the shader program as the current shader to be interacted within openGL
	void RunShader();

	//
	unInt GetID() const { return ProgramID; };

	// allow changing mat 4 variables in shader code
	void SetMat4(const char* ShaderVariable, glm::mat4 Value);
	// allow changing integer values in shader code
	void SetInt(const char* ShaderVarName, int Value);

private:
	// attach the shader to the VAO
	bool AttachShader(const wchar_t* ShaderFilePath, ShaderTypes Type);

	// link the shader to openGL
	bool Link();

	// openGL will assign an ID to our Shader
	unInt ProgramID;

	// array that will hold IDs to our unique shaders
	// 0 - Vertex
	// 1 - Fragment
	unInt VFShaderIDs[2] = { 0 };
};