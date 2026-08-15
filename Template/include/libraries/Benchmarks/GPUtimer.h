//! DISCLAMER This is written by Claude, so be careful using this!
//
// GPU instrumentation profiler for OpenGL (GL_TIME_ELAPSED queries)
// Outputs chrome://tracing compatible JSON, same format as Cherno's CPU profiler.
//
// Usage:
//
// GPUInstrumentor::Get().BeginSession("Session Name");
// {
//     GPUInstrumentationTimer timer("Profiled GPU Scope");
//     // GL calls (draw calls / dispatches)
// }
// GPUInstrumentor::Get().EndSession();   // flushes all pending queries, must be called before program exit
//
// NOTE: Unlike the CPU version, results are NOT written immediately on scope exit.
// GPU queries are asynchronous, so timers are stashed and resolved later via
// GPUInstrumentor::Get().ResolvePending(), which you should call once per frame
// (or EndSession will drain everything at once, stalling).
//
#pragma once

#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <fstream>
#include <thread>

#include <glad/glad.h>  // swap for your GL loader (glew.h, etc.)

struct GPUProfileResult
{
    std::string Name;
    long long   Start, End;  // nanoseconds, GPU timeline
    uint32_t    ThreadID;
};

struct GPUInstrumentationSession
{
    std::string Name;
};

// One in-flight timer query pair (start/end timestamps).
struct PendingGPUQuery
{
    std::string Name;
    GLuint      StartQuery;
    GLuint      EndQuery;
    uint32_t    ThreadID;
};

class GPUInstrumentor
{
private:
    GPUInstrumentationSession*  m_CurrentSession;
    std::ofstream               m_OutputStream;
    int                         m_ProfileCount;
    std::deque<PendingGPUQuery> m_Pending;

public:
    GPUInstrumentor()
        : m_CurrentSession(nullptr), m_ProfileCount(0)
    {
    }

    void BeginSession(const std::string& name, const std::string& filepath = "gpu_results.json")
    {
        m_OutputStream.open(filepath);
        WriteHeader();
        m_CurrentSession = new GPUInstrumentationSession{ name };
    }

    void EndSession()
    {
        // Drain any queries still in flight. This will stall the CPU
        // (glGetQueryObjectiv with GL_QUERY_RESULT blocks), but it's
        // the only safe way to guarantee all results are written before close.
        while (!m_Pending.empty())
            ResolveOne(true);

        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount   = 0;
    }

    // Call this once per frame (or whenever convenient) to drain queries
    // that are already available, WITHOUT stalling the CPU.
    void ResolvePending()
    {
        while (!m_Pending.empty())
        {
            GLint available = 0;
            glGetQueryObjectiv(m_Pending.front().EndQuery, GL_QUERY_RESULT_AVAILABLE, &available);
            if (!available)
                break;  // oldest not ready yet -> newer ones won't be either, stop here

            ResolveOne(false);
        }
    }

    void PushPending(PendingGPUQuery&& q)
    {
        m_Pending.push_back(std::move(q));
    }

    void WriteProfile(const GPUProfileResult& result)
    {
        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"gpu\",";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) / 1000 << ',';  // ns -> us for chrome tracing
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":1,";  // separate pid so GPU lane doesn't collide with CPU trace pid 0
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start / 1000;  // ns -> us
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void WriteHeader()
    {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void WriteFooter()
    {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    static GPUInstrumentor& Get()
    {
        static GPUInstrumentor instance;
        return instance;
    }

private:
    void ResolveOne(bool blocking)
    {
        PendingGPUQuery& q = m_Pending.front();

        GLuint64 startTime, endTime;
        if (blocking)
        {
            glGetQueryObjectui64v(q.StartQuery, GL_QUERY_RESULT, &startTime);
            glGetQueryObjectui64v(q.EndQuery, GL_QUERY_RESULT, &endTime);
        }
        else
        {
            glGetQueryObjectui64v(q.StartQuery, GL_QUERY_RESULT, &startTime);
            glGetQueryObjectui64v(q.EndQuery, GL_QUERY_RESULT, &endTime);
        }

        WriteProfile({ q.Name, (long long)startTime, (long long)endTime, q.ThreadID });

        glDeleteQueries(1, &q.StartQuery);
        glDeleteQueries(1, &q.EndQuery);
        m_Pending.pop_front();
    }
};

class GPUInstrumentationTimer
{
public:
    GPUInstrumentationTimer(const char* name)
        : m_Name(name), m_Stopped(false)
    {
        glGenQueries(1, &m_StartQuery);
        glGenQueries(1, &m_EndQuery);
        glQueryCounter(m_StartQuery, GL_TIMESTAMP);
    }

    ~GPUInstrumentationTimer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        glQueryCounter(m_EndQuery, GL_TIMESTAMP);

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        GPUInstrumentor::Get().PushPending({ m_Name, m_StartQuery, m_EndQuery, threadID });

        m_Stopped = true;
    }

private:
    const char* m_Name;
    GLuint      m_StartQuery, m_EndQuery;
    bool        m_Stopped;
};