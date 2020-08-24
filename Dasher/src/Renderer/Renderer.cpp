
#include "Renderer.h"
#include "GL/glew.h"
#include <fstream>
#include <string>

#include "Log.h"
#include "AssetManagement/Texture.h"
#include "AssetManagement/Font.h"
#include "Application/Application.h"

#include "gtc/matrix_transform.hpp"

#include "RendererUtils.h"

struct RendererData
{
	unsigned int nVao;

	unsigned int nVbo;
	unsigned int nIbo;

	const int nMaxVertexCount = 1000;
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

	Font fontDefault;
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

	int nWidth = Application::GetWidth();
	int nHeight = Application::GetHeight();

	OnWindowResize(nWidth, nHeight);

	//Load the textures
	unsigned char whiteColor[4] = { 255,255,255,255 };
	data.nTextureWhiteId = Texture::LoadTextureBuffer(whiteColor, 1, 1);
	//data.nTextureWhiteId = Texture::LoadTexture("Assets\\Textures\\img1.jpg", 0, 0);

	memset(data.boundTextureSlots + 1, -1, 31 * sizeof(int));
	data.boundTextureSlots[0] = data.nTextureWhiteId;
	data.nCurrentTextureSlot = 1;

	data.fontDefault.LoadFont("Assets/Fonts/Quicksand/Quicksand Regular 400.ttf", 120);

	return true;
}

void Renderer::Cleanup()
{

	data.fontDefault.Cleanup();

	delete[] data.localVertexBuffer;
	delete[] data.localIndexBuffer;
	data.localVertexBuffer = nullptr;
	data.localIndexBuffer = nullptr;

	glcall(glDeleteVertexArrays(1, &data.nVao));
	glcall(glDeleteBuffers(1, &data.nVbo));
	glcall(glDeleteBuffers(1, &data.nIbo));

	Texture::DeleteTexture(data.nTextureWhiteId);
}

void Renderer::Flush()
{
	if (!data.nCurrentVertexLocation || !data.nCurrentIndexLocation)
	{
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	//bind this here because while creating new textures, texture slot 0 is used and that could potentially unbind the blank white color texture thats used for rendering color.
	glBindTexture(GL_TEXTURE_2D, data.nTextureWhiteId);	

	glcall(glBufferSubData(GL_ARRAY_BUFFER, 0, data.nCurrentVertexLocation * sizeof(RendererVertex), data.localVertexBuffer));
	glcall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.nCurrentIndexLocation * sizeof(unsigned int), data.localIndexBuffer));


	//Setup uniforms here ?
	glcall(glDrawElements(GL_TRIANGLES, data.nCurrentIndexLocation, GL_UNSIGNED_INT, 0));


	//reset data
	memset(data.boundTextureSlots+1, -1, 31 * sizeof(unsigned int));	//slot 0 is white texture... do not reset it
	data.nCurrentIndexLocation = 0;
	data.nCurrentVertexLocation = 0;
	data.nCurrentTextureSlot = 1;	//slot 0 is white texture

	data.boundTextureSlots[0] = data.nTextureWhiteId; //precaution
}


void Renderer::DrawRectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col)
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
	DrawRectangle(pos, size, col, data.nTextureWhiteId);
}

void Renderer::DrawRectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& col, unsigned int nTexId)
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
	RendererShapes::Rectangle(quad, { pos.x, pos.y, 0.0f }, size, col);
	for (int i = 0; i < 4; quad[i].SetTexId((float)nTextureSlot), i++);

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


void Renderer::DrawQuadColor(RendererVertex* vertexBuffer, RendererShapes::Shape shape, glm::mat4* transformation /*= nullptr*/)
{
	DrawQuadTexture(vertexBuffer, RendererShapes::genericVertexBufferCount[shape], RendererShapes::genericIndexBuffer[shape], RendererShapes::genericIndexBufferCount[shape], data.nTextureWhiteId, transformation);
}
void Renderer::DrawQuadTexture(RendererVertex* vertexBuffer, RendererShapes::Shape shape, unsigned int nTextureId, glm::mat4* transformation /*= nullptr*/)
{
	DrawQuadTexture(vertexBuffer, RendererShapes::genericVertexBufferCount[shape], RendererShapes::genericIndexBuffer[shape], RendererShapes::genericIndexBufferCount[shape], nTextureId, transformation);
}

void Renderer::DrawQuadColor(RendererVertex* vertexBuffer, unsigned int nVertexCount, const unsigned int* indexBuffer, unsigned int nIndexCount, glm::mat4* transformation)
{
	DrawQuadTexture(vertexBuffer, nVertexCount, indexBuffer, nIndexCount, data.nTextureWhiteId, transformation);
}
void Renderer::DrawQuadTexture(RendererVertex* vertexBuffer, unsigned int nVertexCount, const unsigned int* indexBuffer, unsigned int nIndexCount, unsigned int nTextureId, glm::mat4* transformation)
{
	if (data.nCurrentVertexLocation + (int)nVertexCount > data.nMaxVertexCount || data.nCurrentIndexLocation + (int)nIndexCount > data.nMaxIndexCount)
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

	for (unsigned int i = 0; i < nVertexCount; i++)
	{
		vertexBuffer[i].SetTexId((float)nTextureSlot);
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

	for (unsigned int i = 0; i < nIndexCount; i++)
	{
		index[i] = data.nCurrentVertexLocation + indexBuffer[i];
	}
	
	memcpy(curVertexPosition, vertexBuffer, nVertexCount * sizeof(RendererVertex));
	memcpy(curIndexPosition, index, nIndexCount * sizeof(unsigned int));

	if (transformation)
	{
		//Apply the transformation on the vertices... in the future, consider doing this in the shader
		for (unsigned int i = 0; i < nVertexCount; i++)
		{
			glm::vec3& vertexPos = curVertexPosition[i].m_pos;
			glm::vec4 pos = { vertexPos.x, vertexPos.y, vertexPos.z, 1.0f };
			pos = *transformation * pos;
			vertexPos = { pos.x, pos.y, pos.z };
			//vertexPos *= 1/pos.w; //Keep or delete ?
		}
	}

	if (nIndexCount >= nHeapAllocThreshhold)
		delete[] index;

	data.nCurrentVertexLocation += nVertexCount;
	data.nCurrentIndexLocation += nIndexCount;
}

void Renderer::OnWindowResize(int nWidth, int nHeight)
{
	data.matProjection = glm::ortho<float>(0.0f, (float)nWidth, 0.0f, (float)nHeight, -1.0f, +1.0f);
	glcall(glUniformMatrix4fv(data.u_mvpLocation, 1, GL_FALSE, &data.matProjection[0][0]));

	glViewport(0, 0, nWidth, nHeight);
}

#if 0
void Renderer::DrawTextColor(const std::string& text, const glm::vec2& pos, float scale, const glm::vec4& col, Font* font /*= nullptr*/)
{
	DrawTextColor(text, pos.x, pos.y, scale, col, font);
}
void Renderer::DrawTextColor(const std::string& text, float PosX, float PosY, float scale, const glm::vec4& col, Font* font /*= nullptr*/)
{
	if (text.empty()) { return; }
	if (!font) { font = &data.fontDefault; }

	RendererVertex vertex[4];

	for (std::size_t i = 0; i < text.size(); i++)
	{
		const FontCharacter& fontChar = font->GetFontChar(text.at(i));
		//x,y are the coordinates of the bottom left point of the quad that will render the character texture
		float x = PosX + fontChar.bearing.x * scale;
		float y = PosY - (fontChar.size.y - fontChar.bearing.y) * scale;
		float width = fontChar.size.x * scale;
		float height = fontChar.size.y * scale;

		vertex[0].SetPosColTex({ x, y, 0 },					col, { 0.0f, 0.0f });
		vertex[1].SetPosColTex({ x + width, y, 0 },			col, { 1.0f, 0.0f });
		vertex[2].SetPosColTex({ x + width, y + height, 0 },col, { 1.0f, 1.0f });
		vertex[3].SetPosColTex({ x, y + height, 0 },		col, { 0.0f, 1.0f });
		
		//RendererShapes::Rectangle({}, {}, col);

		Renderer::DrawQuadTexture(vertex, RendererShapes::Shape::ShapeQuad, fontChar.texId);

		PosX += fontChar.advance * scale;
	}
}
#endif

void Renderer::DrawTextColor(const char* const strText, int nSize, const glm::vec2& pos, float scale, const glm::vec4& col, Font* font)
{
	DrawTextColor(strText, nSize, pos.x, pos.y, scale, col, font);
}
void Renderer::DrawTextColor(const char* const strText, int nSize, float PosX, float PosY, float scale, const glm::vec4& col, Font* font)
{
	if (!nSize)
	{
		return;
	}
	else if (nSize == -1)
	{ 
		nSize = mystrlen(strText);
	}

	if (!font) { font = &data.fontDefault; }

	RendererVertex vertex[4];

	for (int i = 0; i < nSize; i++)
	{
		if (strText[i] == '\0')	break;

		const FontCharacter& fontChar = font->GetFontChar(strText[i]);
		//x,y are the coordinates of the bottom left point of the quad that will render the character texture
		float x = PosX + fontChar.bearing.x * scale;
		float y = PosY - (fontChar.size.y - fontChar.bearing.y) * scale;
		float width = fontChar.size.x * scale;
		float height = fontChar.size.y * scale;

		vertex[0].SetPosColTex({ x, y, 0.8f }, col, { 0.0f, 0.0f });
		vertex[1].SetPosColTex({ x + width, y, 0.8f }, col, { 1.0f, 0.0f });
		vertex[2].SetPosColTex({ x + width, y + height,0.8f }, col, { 1.0f, 1.0f });
		vertex[3].SetPosColTex({ x, y + height, 0.8f }, col, { 0.0f, 1.0f });

		//RendererShapes::Rectangle({}, {}, col);

		Renderer::DrawQuadTexture(vertex, RendererShapes::Shape::ShapeQuad, fontChar.texId);
		PosX += fontChar.advance * scale;
	}
}