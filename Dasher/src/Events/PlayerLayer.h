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

	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;	//in seconds

	//virtual bool OnKey(int key) override;
	//virtual bool OnKeyDown(int key) override;
	//virtual bool OnKeyUp(int key) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnWindowResize(int x, int y) override;


	inline const RendererVertex* GetVertex() const { return m_Vertex; }
	inline RendererVertex* GetVertex() { return m_Vertex; }
	
	//Change this to non static if ever m_nVertexCount becomes a non static member
	inline static constexpr int GetVertexCount() { return m_nVertexCount; }

	void TakeDamage(double damage);

private:
	void DrawTrajectory(double timeIntoFuture);
	
private:
	//constants
	
	const float m_fAmplitudeOffset = 80;	//10 units less than half the screen... ie, at the peak of its motion, the player would have a distance of 10 units from the top of the screen

	const double m_dAngVelocityMin = 1;
	const double m_dAngVelocityMax = 6;

	const glm::vec2 mc_vSize = { 70,70 };
	glm::vec4 m_vCol =  { 0.5, 0.4, 0.8,1.0 };

private:	
	double m_dPhaseAngle;	//This is in radians
	double m_dAngVelocity;
	double m_dApparantVelocityX;	//This is the velocity the that the player is moving at. This is never used for actually moving. Its only used to calculate the trajectory
	
	glm::vec2 m_vPos;
	double m_dAmplitude;

	int m_nWidth;
	int m_nHeight;

	static constexpr unsigned int m_nVertexCount = 4;
	RendererVertex m_Vertex[m_nVertexCount];

};   