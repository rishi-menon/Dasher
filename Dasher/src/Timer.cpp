
#include "Timer.h"
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

#include "Log.h"

TimeManager timer;

#if 0

static const double dMaxDeltaTime = 1.0 / 30.0;
static const double dTargetDeltaTime = 1.0 / 60.0;

void TimeManager::SetTime()
{
	double dCurrentTime = glfwGetTime();
	double dDeltaTime = dCurrentTime - timer.GetGameTime();
	if (dDeltaTime > dMaxDeltaTime)
	{
		LOG_INFO("Frame rate is slowing down: {0} ms", dDeltaTime * 1000);
		dDeltaTime = dMaxDeltaTime;
	}

	deltaTime = dDeltaTime;
	currentTime = dCurrentTime;
}

void TimeManager::Sleep()
{
	if (deltaTime < dTargetDeltaTime)
	{
		double dSleepTime = (dTargetDeltaTime - deltaTime) * 1000;
		std::chrono::milliseconds sleepDuration(static_cast<long>(dSleepTime));
		std::this_thread::sleep_for(sleepDuration);
	}
}
#endif