#pragma once
#include "Layer.h"
#include "Maths.h"
#include "Log.h"
#include "Renderer/RendererVertex.h"

class FadeoutScreenLayer : public Layer
{
	typedef void (*FadeoutScreenFunction)(void*);
public:
	FadeoutScreenLayer();
	virtual void OnUpdate(float deltaTime) override;
	virtual void RegisterEvents(Application* pApp, int nIndex) override;
	virtual bool OnWindowResize(int x, int y) override;

	inline void BeginFading() { m_fAnimationPercent = 0.0f; m_bIsRunning = true; m_bIsActive = true; m_bCallbackFunctionActivated = false; GenerateVertexBuffer(); }

	void SetAlphaRange(const glm::vec2& range) {
		ASSERT(!m_bIsActive, "Set properties before starting animation");
		if (!m_bIsActive) {
			m_vAlphaRange = range;
		}
	}
	void SetAnimationTime(float time) { 
		ASSERT(!m_bIsActive, "Set properties before starting animation");
		if (!m_bIsActive) {
			m_fAnimationTime = time;
		}
	}
	void SetColor(const glm::vec3& col) { 
		ASSERT(!m_bIsActive, "Set properties before starting animation");
		if (!m_bIsActive) {
			m_vColor.r = col.r; m_vColor.g = col.g; m_vColor.b = col.b;
		}
	}

	void SetOptionalCallback(FadeoutScreenFunction func) {
		ASSERT(!m_bIsActive, "Set properties before starting animation");
		if (!m_bIsActive) {
			m_CallbackFunction = func;
		}
	}

	void SetOptionalWaitTimeBeforeCallback(float time)
	{
		ASSERT(!m_bIsActive, "Set properties before starting animation");
		if (!m_bIsActive) {
			m_fWaitTimeBeforeCallback = time;
		}
	}

	void SetUserData(void* data)
	{
		ASSERT(!m_bIsActive, "Set properties before starting animation");
		if (!m_bIsActive) {
			m_userData = data;
		}
	}

private:
	void GenerateVertexBuffer();

private:
	bool m_bIsActive;	//Layer is active or not
	bool m_bIsRunning;	//animation is currently running or not, once it gets over, the callback function will soon get called
	bool m_bCallbackFunctionActivated;	//callback function was called or not

	float m_fAnimationPercent;
	float m_fCallbackTime;

	RendererVertex m_vertex[4];
	
	//Properties
	glm::vec2 m_vAlphaRange;	//x: start, y: end
	float m_fAnimationTime;		//In seconds
	glm::vec4 m_vColor;
	float m_fWaitTimeBeforeCallback;
	void* m_userData;

	FadeoutScreenFunction m_CallbackFunction;
};