#pragma once

#include "Layer.h"
#include "Maths.h"
#include "Renderer/RendererVertex.h"

class PlayerLayer : public Layer
{
public:
	PlayerLayer();
	
	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual void ResetLayer() override;

	virtual void OnStart();
	virtual void OnUpdate(float deltaTime);	//in seconds

	virtual bool OnKey(int key);
	virtual bool OnKeyDown(int key);
	virtual bool OnKeyUp(int key);

	inline const RendererVertex* GetVertex() const { return m_Vertex; }
	inline RendererVertex* GetVertex() { return m_Vertex; }
	
	//Change this to non static if ever m_nVertexCount becomes a non static member
	inline static constexpr int GetVertexCount() { return m_nVertexCount; }

private:
	const int mc_nJumpKey = 32;
	
	const float mc_fEnginePower = 14;
	const float mc_fGravity = -27;

	const glm::vec2 mc_vSize = { 70,70 };
	const glm::vec4 mc_vCol =  { 0.5, 0.4, 0.8,1.0 };
private:	
	glm::vec2 m_vPos;
	glm::vec2 m_vVel;
	
	static constexpr unsigned int m_nVertexCount = 4;
	RendererVertex m_Vertex[m_nVertexCount];

	bool m_bIsFalling;
};   