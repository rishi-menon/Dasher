
#include "Renderer.h"
#include "GL/glew.h"
#include <fstream>
#include <string>

#include "Log.h"
#include "Texture.h"
#include "Application/Application.h"

#include "gtc/matrix_transform.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////

struct RendererData
{
	unsigned int nVao;

	unsigned int nVbo;
	unsigned int nIbo;

	const int nMaxVertexCount = 52;
	const int nMaxIndexCount = (nMaxVertexCount * 6) / 4;

	unsigned int nShader;

	RendererVertex* localVertexBuffer = nullptr;
	int nCurrentVertexLocation = 0;

	unsigned int* localIndexBuffer = nullptr;
	int nCurrentIndexLocation = 0;

	unsigned int nTextureWhiteId;

	const int nMaxTexSlots = 32;
	int boundTextureSlots[32];
	int nCurrentTextureSlot = 0;


	glm::mat4 matProjection;
	int u_mvpLocation;
};

static RendererData data;

bool Renderer::Initialise()
{
	if (data.localVertexBuffer || data.localIndexBuffer)
	{
		ASSERT(false, "Renderer has already been initialsed");
		return false;
	}

	glcall(glGenVertexArrays(1, &data.nVao));
	glcall(glBindVertexArray(data.nVao));

	////////////////////////////////////////////////////////////////////////////////////////////////
	//										 generate buffers						  			  //
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	//OpenGl Buffers
	glcall(glGenBuffers(1, &data.nVbo));
	glcall(glBindBuffer(GL_ARRAY_BUFFER, data.nVbo));
	glcall(glBufferData(GL_ARRAY_BUFFER, data.nMaxVertexCount * sizeof(RendererVertex), nullptr, GL_DYNAMIC_DRAW));
	
	glcall(glGenBuffers(1, &data.nIbo));
	glcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.nIbo));
	glcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.nMaxIndexCount * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));

	//specify layout
	glcall(glEnableVertexAttribArray(0));
	glcall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RendererVertex), (const void*)offsetof(RendererVertex, m_pos)));

	glcall(glEnableVertexAttribArray(1));
	glcall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(RendererVertex), (const void*)offsetof(RendererVertex, m_col)));

	glcall(glEnableVertexAttribArray(2));
	glcall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RendererVertex), (const void*)offsetof(RendererVertex, m_textureCoord)));
	
	glcall(glEnableVertexAttribArray(3));
	glcall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(RendererVertex), (const void*)offsetof(RendererVertex, m_textureId)));

	data.localIndexBuffer = new unsigned int[data.nMaxIndexCount];
	data.localVertexBuffer = new RendererVertex[data.nMaxVertexCount];

	//create shader
	{
		std::string strVertex, strFrag;
		ParseShader("Assets\\Shaders\\vertex.shader", strVertex, strFrag);

		data.nShader = glCreateProgram();
		int nVertex = CompileShader(GL_VERTEX_SHADER, strVertex.c_str());
		int nFrag = CompileShader(GL_FRAGMENT_SHADER, strFrag.c_str());
		
		glcall(glAttachShader(data.nShader, nVertex));
		glcall(glAttachShader(data.nShader, nFrag));
		glcall(glLinkProgram(data.nShader));
		glcall(glValidateProgram(data.nShader));

		int nLinkStatus = 0, nValidateStatus = 0;

		glcall(glGetProgramiv(data.nShader, GL_LINK_STATUS, &nLinkStatus));
		glcall(glGetProgramiv(data.nShader, GL_VALIDATE_STATUS, &nValidateStatus));
		if (nValidateStatus == GL_FALSE || nLinkStatus == GL_FALSE)
		{
			ASSERT(false, "Could not link/validate shader");
			return false;
		}
		
		glcall(glUseProgram(data.nShader));
		//set uniforms

		glcall(int texSlotsLocation = glGetUniformLocation(data.nShader, "u_textureSlots"));
		if (texSlotsLocation != -1)
		{
			int u_TexSlots[32];
			for (int i = 0; i < 32; u_TexSlots[i] = i, i++);
			glcall(glUniform1iv(texSlotsLocation, 32, u_TexSlots));
		}
		else
		{
			LOG_WARN("uniform u_textureSlots was not found");
		}
	}

	glcall(data.u_mvpLocation = glGetUniformLocation(data.nShader, "u_mvp"));
	if (data.u_mvpLocation == -1)
	{
		LOG_WARN("uniform u_mvp was not found");
	}

	int nWidth = Application::GetCurrentApp()->GetWidth();
	int nHeight = Application::GetCurrentApp()->GetHeight();

	OnWindowResize(nWidth, nHeight);

	//Load the textures
	unsigned char whiteColor[4] = { 255,255,255,255 };
	data.nTextureWhiteId = Texture::LoadTexture(whiteColor, 1, 1);
	//data.nTextureWhiteId = Texture::LoadTexture("Assets\\Textures\\img1.jpg", 0, 0);

	memset(data.boundTextureSlots + 1, -1, 31 * sizeof(int));
	data.boundTextureSlots[0] = data.nTextureWhiteId;
	data.nCurrentTextureSlot = 1;
	return true;
}

void Renderer::Cleanup()
{
	delete[] data.localVertexBuffer;
	delete[] data.localIndexBuffer;
	data.localVertexBuffer = nullptr;
	data.localIndexBuffer = nullptr;

	glcall(glDeleteVertexArrays(1, &data.nVao));
	glcall(glDeleteBuffers(1, &data.nVbo));
	glcall(glDeleteBuffers(1, &data.nIbo));
}

void Renderer::Flush()
{

	if (!data.nCurrentVertexLocation || !data.nCurrentIndexLocation)
	{
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, data.nTextureWhiteId);

	glcall(glBufferSubData(GL_ARRAY_BUFFER, 0, data.nCurrentVertexLocation * sizeof(RendererVertex), data.localVertexBuffer));
	glcall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.nCurrentIndexLocation * sizeof(unsigned int), data.localIndexBuffer));

	glcall(glUniformMatrix4fv(data.u_mvpLocation, 1, GL_FALSE, &data.matProjection[0][0]));

	//Setup uniforms here ?
	glcall(glDrawElements(GL_TRIANGLES, data.nCurrentIndexLocation, GL_UNSIGNED_INT, 0));


	//reset data
	memset(data.boundTextureSlots+1, -1, 31 * sizeof(unsigned int));	//slot 0 is white texture... do not reset it
	data.nCurrentIndexLocation = 0;
	data.nCurrentVertexLocation = 0;
	data.nCurrentTextureSlot = 1;	//slot 0 is white texture

	data.boundTextureSlots[0] = data.nTextureWhiteId; //precaution
}

static void GenerateQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col, float texId, RendererVertex outQuad[4])
{
	//Set the position
	float halfx = size.x / 2, halfy = size.y / 2;
	outQuad[0].m_pos = { pos.x - halfx, pos.y - halfy, 0.0 };
	outQuad[1].m_pos = { pos.x + halfx, pos.y - halfy, 0.0 };
	outQuad[2].m_pos = { pos.x + halfx, pos.y + halfy, 0.0 };
	outQuad[3].m_pos = { pos.x - halfx, pos.y + halfy, 0.0 };

	outQuad[0].m_textureCoord = { 0.0, 0.0 };
	outQuad[1].m_textureCoord = { 1.0, 0.0 };
	outQuad[2].m_textureCoord = { 1.0, 1.0 };
	outQuad[3].m_textureCoord = { 0.0, 1.0 };

	//set col 
	for (int i = 0; i < 4; i++)
	{
		outQuad[i].m_col = col;
		outQuad[i].SetTexId(texId);
	}
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col)
{

	//RendererVertex v[3];
	//v[0] = { {0,0,0}, {0.9,0.9,0.4,1.0}, {0,0}, 0 };
	//v[1] = { {0.5,0,0}, {0.9,0.9,0.4,1.0}, {0,0}, 0 };
	//v[2] = { {0.5,0.5,0}, {0.9,0.9,0.4,1.0}, {0,0}, 0 };
	//
	//unsigned int index[3] = { 0,1,2 };
	//glcall(glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(RendererVertex), v));
	//glcall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 3 * sizeof(unsigned int), index));

	////Setup uniforms here ?
	//glcall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
	//
	//return;
	DrawQuad(pos, size, col, data.nTextureWhiteId);
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col, unsigned int nTexId)
{
	if (data.nCurrentVertexLocation + 4 > data.nMaxVertexCount || data.nCurrentIndexLocation + 6 > data.nMaxIndexCount)
	{
		Renderer::Flush();
	}
	
	//Calculate texture slot for the generated quad
	unsigned int nTextureSlot = 0xffff;
	for (unsigned int i = 0; i < 32; i++)
	{
		if ((unsigned int)data.boundTextureSlots[i] == nTexId)
		{
			//texture is already bound
			nTextureSlot = i;
			break;
		}
	}

	if (nTextureSlot == 0xffff)
	{
		//texture was not found... bind it to a slot and add it to the cache which keeps track of bound textures
		int& curSlot = data.nCurrentTextureSlot;
		if (curSlot >= data.nMaxTexSlots)
		{
			//there are no free spots available to bind... so flush and create a new batch
			Renderer::Flush();
		}
		
		glcall(glActiveTexture(GL_TEXTURE0 + curSlot));
		glcall(glBindTexture(GL_TEXTURE_2D, nTexId));
		data.boundTextureSlots[curSlot] = nTexId;
		nTextureSlot = curSlot;
		curSlot++;
	}

	RendererVertex quad[4];
	GenerateQuad(pos, size, col, (float)nTextureSlot, quad);

	//save to local buffer
	RendererVertex* curVertexPosition = data.localVertexBuffer + data.nCurrentVertexLocation;
	unsigned int* curIndexPosition = data.localIndexBuffer + data.nCurrentIndexLocation;

	unsigned int index[6] = { 0, 1, 2, 2, 3, 0 };

	for (char i = 0; i < 6; i++)
	{
		index[i] += data.nCurrentVertexLocation;
	}

	memcpy(curVertexPosition, quad, 4 * sizeof(RendererVertex));
	memcpy(curIndexPosition, index, 6 * sizeof(unsigned int));

	data.nCurrentVertexLocation += 4;
	data.nCurrentIndexLocation += 6;
}

void Renderer::DrawQuadColor(RendererVertex* vertexBuffer, unsigned int nVertexCount, unsigned int* indexBuffer, unsigned int nIndexCount)
{
	DrawQuadTexture(vertexBuffer, nVertexCount, indexBuffer, nIndexCount, data.nTextureWhiteId);
}
void Renderer::DrawQuadTexture(RendererVertex* vertexBuffer, unsigned int nVertexCount, unsigned int* indexBuffer, unsigned int nIndexCount, unsigned int nTextureId)
{
	if (data.nCurrentVertexLocation + nVertexCount > data.nMaxVertexCount || data.nCurrentIndexLocation + nIndexCount > data.nMaxIndexCount)
	{
		Renderer::Flush();
	}

	//Calculate texture slot for the generated quad
	unsigned int nTextureSlot = 0xffff;
	for (unsigned int i = 0; i < 32; i++)
	{
		if ((unsigned int)data.boundTextureSlots[i] == nTextureId)
		{
			//texture is already bound
			nTextureSlot = i;
			break;
		}
	}

	if (nTextureSlot == 0xffff)
	{
		//texture was not found... bind it to a slot and add it to the cache which keeps track of bound textures
		int& curSlot = data.nCurrentTextureSlot;
		if (curSlot >= data.nMaxTexSlots)
		{
			//there are no free spots available to bind... so flush and create a new batch
			Renderer::Flush();
		}

		glcall(glActiveTexture(GL_TEXTURE0 + curSlot));
		glcall(glBindTexture(GL_TEXTURE_2D, nTextureId));
		data.boundTextureSlots[curSlot] = nTextureId;
		nTextureSlot = curSlot;
		curSlot++;
	}

	for (int i = 0; i < nVertexCount; i++)
	{
		vertexBuffer[i].SetTexId(nTextureSlot);
	}

	//save to local buffer
	RendererVertex* curVertexPosition = data.localVertexBuffer + data.nCurrentVertexLocation;
	unsigned int* curIndexPosition = data.localIndexBuffer + data.nCurrentIndexLocation;

	const int nHeapAllocThreshhold = 40;	//max number of indices that would be created on the stack
	unsigned int* index;
	if (nIndexCount < nHeapAllocThreshhold)
		index = (unsigned int*)alloca(nIndexCount * sizeof(unsigned int));
	else {
		LOG_INFO("Dynamic memory allocation for the index buffer");
		index = new unsigned int[nIndexCount];
	}

	for (char i = 0; i < nIndexCount; i++)
	{
		index[i] = data.nCurrentVertexLocation + indexBuffer[i];
	}

	memcpy(curVertexPosition, vertexBuffer, nVertexCount * sizeof(RendererVertex));
	memcpy(curIndexPosition, index, nIndexCount * sizeof(unsigned int));

	if (nIndexCount >= nHeapAllocThreshhold)
		delete[] index;

	data.nCurrentVertexLocation += nVertexCount;
	data.nCurrentIndexLocation += nIndexCount;
}

void Renderer::OnWindowResize(int nWidth, int nHeight)
{
	data.matProjection = glm::ortho<float>(0, nWidth, 0, nHeight);
	glViewport(0, 0, nWidth, nHeight);
}