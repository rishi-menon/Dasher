#pragma once
#include <fstream>

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

static int mystrlen(const char* str)
{
	int i = 0;
	for (; str[i] != '\0'; i++);
	return i;
}

static bool mystrcmp(const char* strA, const char* strB, int len = -1)
{
	//return true if they are different strings
	int lenA = mystrlen(strA);
	int lenB = mystrlen(strB);

	if (len == -1)
	{
		if (lenA != lenB)	return true;
	}
	else
	{
		if (lenA < len || lenB < len) return true;
	}

	if (len == -1 || len > lenA)	len = lenA;

	for (int i = 0; i < len; i++)
	{
		if (strA[i] != strB[i])
			return true;
	}
	return false;
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

			if (!mystrcmp(buff, "#shader", 7))
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
