#pragma once

#include "Events/Layer.h"
#include "Maths.h"
#include "Renderer/RendererVertex.h"

struct TrajectoryPoint
{
	TrajectoryPoint() = default;
	TrajectoryPoint(TrajectoryPoint&) = default;
	glm::vec2 pos;
};

class AbstractPlayerLayer : public Layer
{
public:
	AbstractPlayerLayer();

	virtual void RegisterEvents(Application* pApp, int nIndex) override;

	virtual void OnUpdate(float deltaTime) override;	//in seconds

	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnWindowResize(int x, int y) override;


	inline const RendererVertex* GetVertex() const { return m_Vertex; }
	inline RendererVertex* GetVertex() { return m_Vertex; }

	//Change this to non static if ever m_nVertexCount becomes a non static member
	inline static constexpr int GetVertexCount() { return m_nVertexCount; }

	virtual void TakeDamage(double damage) {}
	virtual void TakeNoDamage() {}

	inline double GetPhasePercent() const { return Math::GetPercent(m_dAngVelocityMin, m_dAngVelocityMax, m_dAngVelocity); }

protected:
	void DrawTrajectory(double& startingPointX, double& startingPhase, double timeIntoFuture, int numOfPoints);
	void RenderPlayer();

	void RecalculateAngularVelocity();

protected:
	//constants

	const float m_fAmplitudeOffset = 80;	//10 units less than half the screen... ie, at the peak of its motion, the player would have a distance of 10 units from the top of the screen

	double m_dAngVelocityMin = 1;
	double m_dAngVelocityMax = 6;

	glm::vec2 m_vSize = { 70,70 };
	glm::vec4 m_vCol = { 0.5, 0.4, 0.8,1.0 };

	glm::vec4 m_vTrajectoryColor;

protected:
	double m_dPhaseAngle;	//This is in radians
	double m_dAngVelocity;
	
	//This is the velocity the that the player is moving at. This is never used for actually moving. Its only used to calculate the trajectory
	double m_dApparantVelocityX;	// To do: Move this variable to child class ??	

	glm::vec2 m_vPos;
	double m_dAmplitude;

	int m_nWidth;
	int m_nHeight;

	static constexpr unsigned int m_nVertexCount = 4;
	RendererVertex m_Vertex[m_nVertexCount];
};