#include "FadeoutScreenLayer.h"
#include "Renderer/Renderer.h"
#include "Application/Application.h"

FadeoutScreenLayer::FadeoutScreenLayer() : 
	m_bIsActive (false),
	m_bIsRunning (false),
	m_bCallbackFunctionActivated(false),

	m_fAnimationPercent (0.0f),
	m_fCallbackTime(0.0f),

	m_vAlphaRange (0.0f, 0.0f),
	m_fAnimationTime (1.0f),
	m_vColor (0.0f, 0.0f, 0.0f, 1.0f),

	m_userData(nullptr),
	m_CallbackFunction(nullptr)
{
}

void FadeoutScreenLayer::GenerateVertexBuffer()
{
	int w = Application::GetWidth();
	int h = Application::GetHeight();

	m_vertex[0].SetPosCol({ 0.0f, 0.0f, 0.7f }, m_vColor);
	m_vertex[1].SetPosCol({ w,    0.0f, 0.7f }, m_vColor);
	m_vertex[2].SetPosCol({ w,	  h,    0.7f }, m_vColor);
	m_vertex[3].SetPosCol({ 0.0f, h,    0.7f }, m_vColor);
}

void FadeoutScreenLayer::OnUpdate(float deltaTime)
{
	if (m_bIsActive)
	{
		if (m_bIsRunning)
		{
			m_fAnimationPercent += deltaTime / m_fAnimationTime;
		
			if (m_fAnimationPercent >= 1.0f)
			{
				m_fAnimationPercent = 1.0f;
				m_bIsRunning = false; //Animation is over
				m_fCallbackTime = (float)(Application::GetGameTime()) + m_fWaitTimeBeforeCallback;
			}

			m_vColor.a = (float)Math::Lerp(m_vAlphaRange.x, m_vAlphaRange.y, m_fAnimationPercent);
			GenerateVertexBuffer();
		}
		else if (m_CallbackFunction && !m_bCallbackFunctionActivated && Application::GetGameTime() > m_fCallbackTime)
		{
			m_CallbackFunction(m_userData);
			m_bCallbackFunctionActivated = true;	//Prevents it from getting called twice
		}

		Renderer::DrawQuadColor(m_vertex, RendererShapes::Shape::ShapeQuad);
	}
}

void FadeoutScreenLayer::RegisterEvents(Application* pApp, int nIndex)
{
	pApp->RegisterEvents(LayerWindowResize, nIndex);
}
bool FadeoutScreenLayer::OnWindowResize(int x, int y)
{
	GenerateVertexBuffer();
	return false;
}
