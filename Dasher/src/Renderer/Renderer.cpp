#include "Renderer.h"
#include <GL/glew.h>
#include <fstream>

#include "Shapes/BaseShape.h"
#include "Texture.h"

#include "Log.h"
#include "GlBuffer.h"


namespace Renderer
{
	//'Private' equivalent stuff
	
	static const int nMaxVertices = 8;
	static const int nMaxIndices = ( 6 * nMaxVertices) / 4;


	static int nCurrentVertex = 0;
	static int nCurrentIndex = 0;

	static unsigned int nVao = 0;

	static GlBuffer s_vertexBuffer;
	static GlBuffer s_indexBuffer;

	static unsigned int nShader = 0;
}

namespace Renderer
{

	///////////////////////////////////////////////////////////////////////////////////////
	//								Shader Helper Functions							     //
	///////////////////////////////////////////////////////////////////////////////////////

	static unsigned int CompileShader(unsigned int nShaderType, const char* strCode)
	{
		glcall(unsigned int id = glCreateShader(nShaderType));
		glcall(glShaderSource(id, 1, &strCode, nullptr));
		glcall(glCompileShader(id));

		int status = 0;
		glcall(glGetShaderiv(id, GL_COMPILE_STATUS, &status));


		if (id == GL_FALSE)
		{
			int size = 0;
			glcall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size));
			char* buff = new char[size];

			std::string str = (nShaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment";

			glcall(glGetShaderInfoLog(id, size, &size, buff));
			LOG_ERROR("Failed to compile {0} shader: {1}", str.c_str(), buff);
			ASSERT(false, "");
			delete buff;
			return 0;
		}
		return id;
	}

	int strlen(const char* str)
	{
		int i = 0;
		for (; str[i] != '\0'; i++);
		return i;
	}

	bool strcmp(const char* strA, const char* strB, int len = -1)
	{
		//return true if they are different strings
		int lenA = ::Renderer::strlen(strA);
		int lenB = ::Renderer::strlen(strB);
		
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
	void ParseShader(const char* const filePath, std::string& outVertex, std::string& outFrag)
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

				if (!::Renderer::strcmp (buff, "#shader", 7))
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

	///////////////////////////////////////////////////////////////////////////////////////
	bool Initialise()
	{
		glcall(glGenVertexArrays(1, &nVao));
		glcall(glBindVertexArray(nVao));

		//Create a large vertex buffer
		s_vertexBuffer.Create(GL_ARRAY_BUFFER, nMaxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		s_vertexBuffer.AddLayout(0, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, m_pos));
		s_vertexBuffer.AddLayout(1, 4, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, m_col));
		s_vertexBuffer.AddLayout(2, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, m_textureCoord));
		s_vertexBuffer.AddLayout(3, 1, GL_INT,   false, sizeof(Vertex), offsetof(Vertex, m_textureId));

		//create index buffer
		s_indexBuffer.Create(GL_ELEMENT_ARRAY_BUFFER, nMaxIndices * sizeof (unsigned int), nullptr, GL_DYNAMIC_DRAW);

		{
			//Create shaders
			std::string vertexCode, fragCode;
			ParseShader("Assets\\Shaders\\vertex.shader", vertexCode, fragCode);

			unsigned int vertexId = CompileShader(GL_VERTEX_SHADER, vertexCode.c_str());
			unsigned int fragId = CompileShader(GL_FRAGMENT_SHADER, fragCode.c_str());

			glcall(nShader = glCreateProgram());
			glAttachShader(nShader, vertexId);
			glAttachShader(nShader, fragId);
			glLinkProgram(nShader);
			glValidateProgram(nShader);

			//check if the shader compiled properly
			int linkStatus = 0, validateStatus = 0;
			glGetProgramiv(nShader, GL_LINK_STATUS, &linkStatus);
			glGetProgramiv(nShader, GL_VALIDATE_STATUS, &validateStatus);
			ASSERT(linkStatus == GL_TRUE, "Error: Could not link program");
			ASSERT(validateStatus == GL_TRUE, "Error: Could not validate program");

			if (linkStatus != GL_TRUE || validateStatus != GL_TRUE)
			{
				LOG_ERROR("Error Could not link/validate the shaders");
				return false;
			}

			glcall(glUseProgram(nShader));
		}
		
	}
	void Cleanup()
	{
		glcall(glDeleteVertexArrays(1, &nVao));
		glcall(glDeleteProgram(nShader));
		s_vertexBuffer.Cleanup();
		s_indexBuffer.Cleanup();
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//								   Core Renderer Functions						    //
	//////////////////////////////////////////////////////////////////////////////////////

	void EndScene()
	{
		if (nCurrentIndex)
		{
			glcall(glDrawElements(GL_TRIANGLES, nCurrentIndex, GL_UNSIGNED_INT, 0));
			nCurrentVertex = 0;
			nCurrentIndex = 0;
		}
	}

	//Use this function to render solid colour
	void DrawShape(const BaseShape& shape)
	{
		int nIndicesToAdd = shape.GetIndicesCount();
		int nVertexToAdd = shape.GetVertexCount();

		if (nCurrentIndex + nIndicesToAdd > nMaxIndices ||
			nCurrentVertex + nVertexToAdd > nMaxVertices)
		{
			EndScene();
		}

		ASSERT(nIndicesToAdd < 200 && nVertexToAdd < 200, "The values are getting too big, use heap allocation instead");
		
		unsigned int* indices = (unsigned int*)alloca(nIndicesToAdd * sizeof(unsigned int));
		
		//this is the array that basically contains the order in which to render the vertices
		//Eg: for a quad it would contain 0,1,2,2,3,0
		unsigned int* baseIndices = shape.GetIndicesBuffer();

		int temp[6];
		for (int i = 0; i < nIndicesToAdd; i++)
		{
			temp[i] = indices[i] = nCurrentVertex + baseIndices[i];
		
		}

		const Vertex* vertexBufferData = shape.GetVertexBuffer();

		s_vertexBuffer.UpdateData(nCurrentVertex * sizeof (Vertex), nVertexToAdd * sizeof(Vertex), vertexBufferData);
		s_indexBuffer.UpdateData(nCurrentIndex * sizeof (unsigned int), nIndicesToAdd * sizeof(unsigned int), indices);

		nCurrentIndex += nIndicesToAdd;
		nCurrentVertex += nVertexToAdd;
	}

	//use this function to render textures
	void DrawShape(const BaseShape& shape, const Texture& texture)
	{
	}

}