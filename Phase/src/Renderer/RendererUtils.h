#pragma once
#include <fstream>
#include "CommandLineFeatures.h"

static unsigned int CompileShader(unsigned int nShaderType, const char* strCode)
{
	glcall(unsigned int id = glCreateShader(nShaderType));
	glcall(glShaderSource(id, 1, &strCode, nullptr));
	glcall(glCompileShader(id));

	int status = 0;
	glcall(glGetShaderiv(id, GL_COMPILE_STATUS, &status));


	if (status == GL_FALSE)
	{
		int size = 0;
		glcall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size));
		char* buff = new char[size];

		std::string str = (nShaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment";

		glcall(glGetShaderInfoLog(id, size, &size, buff));
		LOG_ERROR("Failed to compile {0} shader: {1}", str.c_str(), buff);
		ASSERT(false, "");
		delete[] buff;
		return 0;
	}
	return id;
}

static void ParseShader(const char* const filePath, std::string& outVertex, std::string& outFrag)
{
	std::ifstream file;
	file.open(filePath);
	if (file.is_open())
	{
		outVertex.reserve(2000);
		outFrag.reserve(2000);
		std::string* pCurrent = nullptr;

		while (!file.eof())
		{
			char buff[300];
			file.getline(buff, 300);

			if (!StringHelper::mystrcmp(buff, "#shader", 7))
			{
				//change type
				char c = buff[8];
				pCurrent = (c == 'v') ? &outVertex : &outFrag;
			}
			else
			{
				ASSERT(pCurrent, "Error: Currnet shader was nullptr while processing shader.");
				if (pCurrent)
				{
					pCurrent->append(buff);
					pCurrent->push_back('\n');
				}
			}
		}

		file.close();
	}
}
