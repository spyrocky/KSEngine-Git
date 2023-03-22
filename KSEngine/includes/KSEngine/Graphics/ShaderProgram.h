#pragma once
#include "KSEngine/CoreMinimal.h"
#include "glm/glm.hpp"

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	//this will initialise our shader as a vertex fragment shader
	bool InitVFShader(VFShaderParams ShaderFilePaths);

	//run the shader program as current shader to be interacted with OpenGL
	void RunShader();

	//returns Program ID
	UNint GetID() const { return ProgramID; }

	//allow changing mat4 variables in shader code
	void SetMat4(const char* ShaderVariable, glm::mat4 Value);
	//allow changing integer values in shader code
	void SetInt(const char* ShaderVarName, int Value);

private:
	//attach the shader to VAO
	bool AttachShader(const wchar_t* ShaderFilePath, ShaderTypes Type);

	//link the shader to OpenGL
	bool Link();

private:
	//OpenGL will assign an ID to our shader
	UNint ProgramID;
	//array to hold IDs to unique shaders
	//0 = Vertex
	//1 = Fragment
	UNint VFShaderIDs[2] = { 0 };
};