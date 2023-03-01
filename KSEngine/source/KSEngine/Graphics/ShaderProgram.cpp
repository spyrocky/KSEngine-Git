#include <fstream>
#include <sstream>
#include "KSEngine/Graphics/ShaderProgram.h"
#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram()
{
	ProgramID = 0;
}

ShaderProgram::~ShaderProgram()
{
	for (unInt VFShaderID : VFShaderIDs) {
		// remove the shader from the program
		glDetachShader(ProgramID, VFShaderID);
		// delete the shader from memory
		glDeleteShader(VFShaderID);
	}

	// this will delete the shader program from openGL
	glDeleteProgram(ProgramID);

	cout << "Shader Program" << ProgramID << "has been destroyed" << endl;
}

bool ShaderProgram::InitVFShader(VFShaderParams ShaderFilePaths)
{
	ProgramID = glCreateProgram();

	if (ProgramID < 1) {
		cout << "Shader Program | Can't assign program ID." << endl;
		return false;
	}

	bool VShader = AttachShader(ShaderFilePaths.VertexShaderPath, ShaderTypes::Vertex);
	bool FShader = AttachShader(ShaderFilePaths.FragmentShaderPath, ShaderTypes::Fragment);

	if (!VShader || !FShader) {
		return false;
	}
	// fail the whole function if Link() fails
	return Link();
}

void ShaderProgram::RunShader()
{
	glUseProgram(ProgramID);
}

void ShaderProgram::SetMat4(const char* ShaderVariable, glm::mat4 Value)
{
	// finding the uniform mat4 called ShaderVariable and overwritting the value
	glUniformMatrix4fv(
		glGetUniformLocation(ProgramID, ShaderVariable), 1, GL_FALSE, value_ptr(Value)
	);
}

void ShaderProgram::SetInt(const char* ShaderVarName, int Value)
{
	// find the uniform int value with the ShaderVarName and assign it the value
	glUniform1i(glGetUniformLocation(ProgramID, ShaderVarName), Value);
}

bool ShaderProgram::AttachShader(const wchar_t* ShaderFilePath, ShaderTypes Type)
{
	// store the shader code
	string ShaderCode;
	// store the file path in a file stream
	ifstream FilePathStream(ShaderFilePath);

	// check if the stream found the file
	if (!FilePathStream.is_open()) {
		wclog << "Shader Program | " << ShaderFilePath << " not found." << endl;
		return false;
	}

	// temporarily hold the shader for conversion in a string stream
	stringstream RawShaderCode;
	// convert the file to a single stream
	RawShaderCode << FilePathStream.rdbuf();
	// convert it into a readable string
	ShaderCode = RawShaderCode.str();
	// close the stream to the file
	FilePathStream.close();

	// initialise the shader ID variable for this specific shader
	unInt ShaderID = 0;
	// change what happens with the ID depending on the type of shader
	switch (Type)
	{
	case ShaderTypes::Vertex:
		ShaderID = glCreateShader(GL_VERTEX_SHADER);
		VFShaderIDs[0] = ShaderID;
		break;
	case ShaderTypes::Fragment:
		ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		VFShaderIDs[1] = ShaderID;
		break;
	default:
		break;
	}

	// put our string code into a character array
	const char* ShaderCodePtr = ShaderCode.c_str();
	// add the shader code to the shader that we created above
	glShaderSource(ShaderID, 1, &ShaderCodePtr, NULL);
	// complie the shader
	glCompileShader(ShaderID);

	// initialise a variable that will tell us if the shader has any logs
	int LogLength = 0;
	// check if there are any logs and add them to the LogLength
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &LogLength);
	// print the log if there were any
	if (LogLength > 0) {
		// store the message
		vector<char> LogMessage(LogLength + 1);
		// add the characters into the vector array
		glGetShaderInfoLog(ShaderID, LogLength, NULL, &LogMessage[0]);

		// check if the compile failed
		int CompileStatus = GL_TRUE;
		// get the complie status - return a GL_TRUE if successful or GL_FALSE
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &CompileStatus);

		if (CompileStatus == GL_TRUE) {
			wclog << "Shader Program | Compiler | " << ShaderFilePath <<
				" compiled successfully with the following log:" << endl
				<< &LogMessage[0] << endl;
		}
		else {
			wclog << "Shader Program | Compiler | " << ShaderFilePath <<
				" failed to compile with the following log:" << endl
				<< &LogMessage[0] << endl;
			return false;
		}
	}
	else {
		wclog << "Shader Program | " << ShaderFilePath << " compiled successfully." << endl;
	}

	// attach the shader to the program ID
	glAttachShader(ProgramID, ShaderID);

	return true;
}

bool ShaderProgram::Link()
{
	// add the program to openGL
	glLinkProgram(ProgramID);

	// check the link for logs and errors
	int LogLength = 0;
	glGetShaderiv(ProgramID, GL_INFO_LOG_LENGTH, &LogLength);

	if (LogLength > 0) {
		vector<char> LogMessage(LogLength + 1);
		glGetShaderInfoLog(ProgramID, LogLength, NULL, &LogMessage[0]);

		int CompileStatus = GL_TRUE;
		glGetShaderiv(ProgramID, GL_COMPILE_STATUS, &CompileStatus);

		if (CompileStatus == GL_TRUE) {
			wclog << "Shader Program | Successfully linked with the following log:" << endl
				<< &LogMessage[0] << endl;
		}
		else {
			wclog << "Shader Program | Failed to link with the following log:" << endl
				<< &LogMessage[0] << endl;
			return false;
		}
	}
	else {
		wclog << "Shader Program | Successfully linked with no logs:" << endl;
	}


	return true;
}