#include "ElapsedTimer.h"
#include<time.h>
ElapsedTimer::ElapsedTimer()
    : m_startTicks(0)
    , m_pausedTicks(0)
    , m_paused(false)
    , m_started(false)
{
}

void ElapsedTimer::start()
{
    m_started = true;
    m_paused = false;

    //获取当前时钟时间
    m_startTicks = clock();
    m_pausedTicks = 0;
}

void ElapsedTimer::reStart()
{
}

void ElapsedTimer::stop()
{
    //停止计时器
    m_started = false;
    m_paused = false;

    m_startTicks = 0;
    m_pausedTicks = 0;
}

void ElapsedTimer::pause()
{
    //如果计时器正在运行，并且还没有暂停
    if (m_started && !m_paused)
    {
        m_paused = true;    //暂停计时器

        //计算暂停的刻度(当前时间 - 开始时间==经过多长时间)
        m_pausedTicks = clock() - m_startTicks;
        m_startTicks = 0;   //重置开始的时间
    }
}

void ElapsedTimer::resume()
{
    //如果计时器正在运行并暂停了
    if (m_started && m_paused)
    {
        //取消暂停
        m_paused = false;

        //重设开始时间
        m_startTicks = clock() - m_pausedTicks;
        
        //重置暂停时间
        m_pausedTicks = 0;
    }
}

Uint32 ElapsedTimer::elapsed()
{
    //如果程序正在运行
    if (m_started)
    {
        //如果计时器被暂停
        if (m_paused)
        {
            return m_pausedTicks;
        }
        else
        {
            return clock() - m_startTicks;
        }
    }

    return 0;
}

bool ElapsedTimer::isStarted()
{
    return m_started;
}

bool ElapsedTimer::isPaused()
{
    return m_paused;
}
