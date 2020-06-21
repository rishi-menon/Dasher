#include "Renderer.h"
#include <GL/glew.h>
#include <fstream>

#include "Shapes/BaseShape.h"
#include "Texture.h"

#include "Log.h"
#include "GlBuffer.h"

#include <unordered_map>

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

	//generic shader id
	static unsigned int nShader = 0;

	const int unsigned s_nMaxTextureSlots = 20;	//use slots 0 to 19... reset the counter when it hits 20.
	static unsigned int s_nCurrentTextureSlot = 0;
	static std::unordered_map <unsigned int, unsigned int> s_mapTexIdToTexSlot;

	Texture s_whiteTexture;
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


		if (status == GL_FALSE)
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
		s_vertexBuffer.AddLayout(3, 1, GL_FLOAT,   false, sizeof(Vertex), offsetof(Vertex, m_textureId));

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

		s_mapTexIdToTexSlot.reserve(s_nMaxTextureSlots);

		int u_textures = glGetUniformLocation(nShader, "u_textureSlots");
		if (u_textures == -1)
		{
			LOG_WARN("Uniform u_textureSlots was not found.");
		}
		else
		{
			//texture sampler uniform
			static const int u_samplerIndex[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
			glUniform1iv(u_textures, 32, u_samplerIndex);
		}
		unsigned char whiteRgba[4] = { 255,255,255,255 };
		s_whiteTexture.LoadTexture(whiteRgba, 1, 1);
		return true;
		
	}
	void Cleanup()
	{
		glcall(glDeleteVertexArrays(1, &nVao));
		glcall(glDeleteProgram(nShader));
		s_vertexBuffer.Cleanup();
		s_indexBuffer.Cleanup();
	}


	void InsertIndicesIntoBuffer(const BaseShape& shape)
	{
		int nIndicesToAdd = shape.GetIndicesCount();

		if (nCurrentIndex + nIndicesToAdd > nMaxIndices)
		{
			EndScene();
		}

		ASSERT(nIndicesToAdd < 200, "The values are getting too big, use heap allocation instead");

		unsigned int* indices = (unsigned int*)alloca(nIndicesToAdd * sizeof(unsigned int));

		//this is the array that basically contains the order in which to render the vertices
		//Eg: for a quad it would contain 0,1,2,2,3,0
		unsigned int* baseIndices = shape.GetIndicesBuffer();

#ifdef _DEBUG
		int finalIndicesDebug[6];
#endif // _DEBUG

		for (int i = 0; i < nIndicesToAdd; i++)
		{
			indices[i] = nCurrentVertex + baseIndices[i];
#if _DEBUG
			finalIndicesDebug[i] = indices[i];
#endif // _DEBUG

		}
		s_indexBuffer.UpdateData(nCurrentIndex * sizeof(unsigned int), nIndicesToAdd * sizeof(unsigned int), indices);
	}

	void InsertVerticesToBuffer(const Vertex* vertexBufferData, int nVertexToAdd)
	{
		if (nCurrentVertex + nVertexToAdd > nMaxVertices)
		{
			EndScene();
		}
		s_vertexBuffer.UpdateData(nCurrentVertex * sizeof(Vertex), nVertexToAdd * sizeof(Vertex), vertexBufferData);
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
			s_nCurrentTextureSlot = 0;
			s_mapTexIdToTexSlot.clear();
		}
	}

	//Use this function to render solid colour
	void DrawShape(BaseShape& shape)
	{
#if 0
		int nIndicesToAdd = shape.GetIndicesCount();
		int nVertexToAdd = shape.GetVertexCount();

		Vertex* vertexBuffer = shape.GetVertexBuffer();
		InsertVerticesToBuffer(vertexBuffer, nVertexToAdd);
		InsertIndicesIntoBuffer(shape);

		nCurrentIndex += nIndicesToAdd;
		nCurrentVertex += nVertexToAdd;
#else
		DrawShape(shape, s_whiteTexture);
#endif
	}

	//use this function to render textures
	void DrawShape(BaseShape& shape, const Texture& texture)
	{
		int nIndicesToAdd = shape.GetIndicesCount();
		int nVertexToAdd = shape.GetVertexCount();

		Vertex* vertexBuffer = shape.GetVertexBuffer();
		
		std::unordered_map<unsigned int, unsigned int>::iterator it = s_mapTexIdToTexSlot.find(texture.GetRendererId());

		unsigned int texSlot;
		if (it != s_mapTexIdToTexSlot.end())
		{
			//texture is already bound to a certain slot
			texSlot = it->second;
		}
		else
		{
			if (s_nCurrentTextureSlot >= s_nMaxTextureSlots)
			{
				EndScene();
			}

			s_mapTexIdToTexSlot.emplace(texture.GetRendererId(), (unsigned int)s_nCurrentTextureSlot);
			glcall(glBindTextureUnit(s_nCurrentTextureSlot, texture.GetRendererId()));
			texSlot = s_nCurrentTextureSlot;
			s_nCurrentTextureSlot++;
		}

		//Texture is bound, now you can add it to the vertex buffer
		for (int i = 0; i < nVertexToAdd; i++)
		{
			vertexBuffer[i].m_textureId = (float)texSlot;
		}

		//upload vertex buffer to the gpu
		InsertIndicesIntoBuffer(shape);
		InsertVerticesToBuffer(vertexBuffer, nVertexToAdd);
		nCurrentIndex += nIndicesToAdd;
		nCurrentVertex += nVertexToAdd;
	}

}