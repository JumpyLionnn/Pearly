#pragma once
#include "prpch.h"
#include "Pearly/Core/Core.h"

#define PR_PROFILE 1
namespace Pearly {
    #ifdef PR_PROFILE
    static std::mutex mutex;

    static void EraseSubStr(std::string& mainStr, const std::string& toErase)
    {
        size_t pos = mainStr.find(toErase);
        if (pos != std::string::npos)
        {
            mainStr.erase(pos, toErase.size());
        }
    }

    struct ProfileResult
    {
        std::string Name;
        long long Start, End;
        uint32 ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
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
            m_CurrentSession = new InstrumentationSession{ name };
        }

        void EndSession()
        {
            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& result)
        {
            mutex.lock();
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.Name;
            EraseSubStr(name, "__cdecl ");
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
            mutex.unlock();
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
            m_StartTimepoint = std::chrono::steady_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!m_Stopped)
                Stop();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::steady_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

            m_Stopped = true;
        }
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
    #endif
}


#ifdef PR_PROFILE
    #define PR_PROFILE_BEGIN_SESSION(name, filepath) ::Pearly::Instrumentor::Get().BeginSession(name, filepath)
    #define PR_PROFILE_END_SESSION() ::Pearly::Instrumentor::Get().EndSession()
    #define PR_PROFILE_SCOPE(name)  ::Pearly::InstrumentationTimer timer##__LINE__(name)
    #define PR_PROFILE_FUNCTION() PR_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define PR_PROFILE_BEGIN_SESSION(name, filepath)
    #define PR_PROFILE_END_SESSION()
    #define PR_PROFILE_FUNCTION()
    #define PR_PROFILE_SCOPE(name)
#endif