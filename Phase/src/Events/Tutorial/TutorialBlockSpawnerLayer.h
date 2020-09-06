#pragma once
#include "Events/BlockSpawnerLayer.h"
#include "Events/BackgroundLayer.h"
#include <vector>
#include <unordered_map>

class TutorialPlayerLayer;

struct TutorialStageProps
{
	TutorialStageProps(const char* str, const glm::vec2& pos, float scale, const glm::vec4& col);

	const char* strText;
	glm::vec2 strPos;
	float strScale;
	glm::vec4 strTextCol;

	BackgroundLayerProps backgroundProps;	//This gets set automatically
};

enum class TutorialStage : int
{
	TextIntro = 0,
	TextBasicMove,
	PlayMove,
	TextFriendlySpikes,
	PlayFriendlySpikes,

	TextPhasing,
	PlayPhasing,
	TextTutorialOver,
	Count
};

class TutorialBlockSpawnerLayer : public BlockSpawnerLayer
{	
public:
	TutorialBlockSpawnerLayer();
	~TutorialBlockSpawnerLayer();

	virtual void RegisterEvents(Application* pApp, int nIndex) override;

	virtual void OnStart() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual bool OnMouseMove(int x, int y) override;
	
	virtual bool OnMouseUp(int nButton) override;
	virtual bool OnKeyUp(int key) override;

	virtual bool OnWindowResize (int x, int y) override;

	inline TutorialStage GetCurrentStage() const { return m_CurStage; }

	void RestartCurrentStage();	//Eg when player dies

private:
	void CreateStages();
	void StartCurrentStage();
	void ShowPhaseRange();

private:
	TutorialPlayerLayer* m_pTutorialPlayer;
	BackgroundLayer* m_pBackgroundLayer;

	TutorialStage m_CurStage;

	using MapStages = std::unordered_map<TutorialStage, TutorialStageProps>;
	MapStages m_mapStages;
	MapStages::iterator m_CurIterator;

	std::unordered_map<TutorialStage, float> m_mapWaitTime;
	float m_dNextStageSwitchTime;
	bool m_bStageOver;
};