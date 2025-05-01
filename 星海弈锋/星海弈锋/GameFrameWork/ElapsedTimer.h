#pragma once
using  Uint32 = unsigned int;
class ElapsedTimer
{
public:
	ElapsedTimer();

	void start();		//开始
	void reStart();		//重开
	void stop();		//停止
	void pause();		//暂停
	void resume();		//暂停之后恢复

	Uint32  elapsed();	//返回自SElapsedTimer上次启动以来的毫秒数 
	bool isStarted();	//是否启动了
	bool isPaused();	//是否暂停了
private:
	Uint32 m_startTicks;	//定时器开始时的时钟时间
	Uint32 m_pausedTicks;	//计时暂停时存储的时间
	
	bool m_paused;			//是否暂停了
	bool m_started;			//是否开始了
};

