#pragma once


class TimeManager
{
public:

	inline void SetGameTime(double dTime) {
		currentTime = dTime;
	}

	inline double GetGameTime() const
	{
		return currentTime;
	}

	inline void SetDeltaTime(double dTime) {
		deltaTime = dTime;
	}

	inline double GetDeltaTime() const
	{
		return deltaTime;
	}
#if 0
	void SetTime();
	void Sleep();
#endif
private:
	//Both are in seconds
	double currentTime;
	double deltaTime;
};

extern TimeManager timer;