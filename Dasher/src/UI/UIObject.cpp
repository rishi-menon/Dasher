
#include "UIObject.h"
#include "AssetManagement/Texture.h"

#include "Log.h"
#include "Maths.h"
#include "Events/UILayer.h"

namespace UI {

	unsigned int UITexIds[static_cast<unsigned int>(UITypes::Count)];
	TextureDimensions UITexDimension[(unsigned int)UITypes::Count];

	void UIInit()
	{
		unsigned int index = static_cast<unsigned int>(UITypes::Button0);
		UITexIds[index] = Texture::LoadTexturePreserve("Assets/Textures/UI/Button0.png", UITexDimension[index]);

		index = static_cast<unsigned int>(UITypes::PushButton0);
		UITexIds[index] = Texture::LoadTexturePreserve("Assets/Textures/UI/PushButton0.png", UITexDimension[index]);

		index = static_cast<unsigned int>(UITypes::PushButton0_E);
		UITexIds[index] = Texture::LoadTexturePreserve("Assets/Textures/UI/PushButton0_E.png", UITexDimension[index]);
	}
	void UICleanup()
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(UITypes::Count); i++)
		{
			Texture::DeleteTexture(UITexIds[i]);
			Texture::FreeImageBuffer(UITexDimension[i].buffer);
		}
	}

	//This is only for Quads. Most UI images would anyway be rendererd to quads 
	bool CheckCollision(int mousex, int mousey, const TextureDimensions& curDim, const RendererVertex vertexQuad[4])
	{
		double percentX = Math::GetPercent(vertexQuad[0].m_pos.x, vertexQuad[1].m_pos.x, mousex);
		double percentY = Math::GetPercent(vertexQuad[0].m_pos.y, vertexQuad[2].m_pos.y, mousey);

		//LOG_WARN("PX: {0}, PY:{1}", percentX, percentY);
		if (percentX < 0 || percentX > 1 || percentY < 0 || percentY > 1)
		{
			return false;
		}

		int textureCoordX = static_cast<int>(percentX * curDim.width);
		int textureCoordY = static_cast<int>(percentY * curDim.height);

		int index = textureCoordY * curDim.width + textureCoordX;

		if (curDim.bpp == 4)
		{
			//check alpha value 
			return (curDim.buffer[curDim.bpp * index + 3] > 0);
		}
		else
		{
			//check the color channel ie check if its black or not
			int rawIndex = curDim.bpp * index;
			for (int i = 0; i < curDim.bpp; i++)
			{
				if (curDim.buffer[rawIndex + i] > 0) { return true; }
			}
			return false;
		}
	}
}

UIObject::UIObject() :
	m_bIsActive (true)
{
	RegisterUIObject();
}
UIObject::~UIObject()
{
	//Remove the object from the UI Layer
	if (g_pCurrentUILayer)
	{
		g_pCurrentUILayer->DeRegisterUIObject(this);
	}
}

void UIObject::RegisterUIObject()	//Registers this object to the layer
{
	if (!g_pCurrentUILayer)
	{
		ASSERT(false, "Could not register UI Object as UI layer does not exist");
		return;
	}

	g_pCurrentUILayer->RegisterUIObject(this);
	//To do: Find the UI Layer over here
}