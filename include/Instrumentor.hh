#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <functional>

#include <future>
#include <cstdio>
#include <unistd.h>
#include <cmath>

using namespace std;

struct ProfileResult
{
    string Name;
    long long Start, End;
    uint32_t ThreadID;
};
struct InstrumentationSession
{
    std::string Name;
};

class Instrumentor
{
    private:
        InstrumentationSession* m_CurrentSession;
        ofstream m_OutputStream;
        int m_ProfileCount;

    public:
        Instrumentor()
            : m_CurrentSession(nullptr), m_ProfileCount(0)
        {
            
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            m_OutputStream.open(filepath);
            WriteHeader();
            m_CurrentSession = new InstrumentationSession;
            m_CurrentSession->Name = name;
            // m_CurrentSession = new InstrumentationSession{name};
        }

        void EndSession()
        {
            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        void WriteProfile(const struct ProfileResult& result)
        {
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
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

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }
};

class InstrumentationTimer
{
    public:
    InstrumentationTimer(const char* name)
    : m_Name(name), m_Stopped(false)
    {
        m_StartTimepoint = chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if(!m_Stopped)
            Stop();
    }
    
    void Stop()
    {
        auto endTimepoint = chrono::high_resolution_clock::now();
        long long start = chrono::time_point_cast<chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = chrono::time_point_cast<chrono::microseconds>(endTimepoint).time_since_epoch().count();

        // uint32_t threadID = hash<thread::id>{}(this_thread::get_id());
        // Instrumentor::Get().WriteProfile({m_Name,start,end,threadID});
        //uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        uint32_t threadID =  std::hash<std::thread::id>()(std::this_thread::get_id());
        struct ProfileResult profile_result = {m_Name, start, end, threadID};
        Instrumentor::Get().WriteProfile(profile_result);
        // Instrumentor::Get().WriteProfile({m_Name,start,end});
        // cout << m_Name << ": " << (end-start) << " ms\n";

        m_Stopped = true;
    }

    private:
    const char * m_Name;
    chrono::time_point<chrono::steady_clock> m_StartTimepoint;
    bool m_Stopped;
};

#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__Line__(name)
// #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif