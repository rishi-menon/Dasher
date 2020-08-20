
#include "UIObject.h"
#include "AssetManagement/Texture.h"
#include "Log.h"

#include "Events/UILayer.h"

namespace UI {

	unsigned int UITexIds[static_cast<unsigned int>(UITypes::Count)];

	void UIInit()
	{
		UITexIds[static_cast<unsigned int>(UITypes::Button0)] = Texture::LoadTexture("Assets/Textures/UI/Button0.png", nullptr, nullptr);
	}
	void UICleanup()
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(UITypes::Count); i++)
		{
			Texture::DeleteTexture(UITexIds[i]);
		}
	}
}

UIObject::UIObject() :
	m_bIsActive (true)
{
	RegisterUIObject();
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