#pragma once
#include "UIObject.h"
#include "AssetManagement/StandardTexture.h"
#include "Renderer/RendererVertex.h"
#include "Maths.h"

class UIImage : public UIObject
{
public:
	UIImage();
	virtual ~UIImage() {}
	
	//Events
	virtual void OnUpdate(float deltaTime) override;
	virtual bool OnWindowResize(int x, int y) override;

	//Props
	inline void SetTextureId(unsigned int nId) { m_nTextureId = nId; }
	inline void SetTextureId(StandardTexture st) { m_nTextureId = GetStandardTextureId(st); }

	void SetProperties(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& col);
	
	//Use this only after the other properties are set
	void SetPos(const glm::vec3& pos);

	//void RegenerateVertexBuffer();

private:
	unsigned int m_nTextureId;
	glm::vec3 m_curPos;

	RendererVertex m_vertex[4];

};