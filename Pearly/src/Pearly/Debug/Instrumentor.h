#pragma once
#include "prpch.h"
#include "Pearly/Core/Core.h"

#define PR_PROFILE 1
namespace Pearly {
    #ifdef PR_PROFILE

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
        std::thread::id ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        std::mutex m_Mutex;
        InstrumentationSession* m_CurrentSession = nullptr;
        std::ofstream m_OutputStream;
        int m_ProfileCount = 0;
    public:
        Instrumentor() {}

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession) {
                PR_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
                return;
            }
            m_OutputStream.open(filepath);
            if (m_OutputStream.is_open()) {
                m_CurrentSession = new InstrumentationSession({ name });
                WriteHeader();
            }
            else {
                PR_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
            }
        }

        void EndSession()
        {
            std::lock_guard lock(m_Mutex);
            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& result)
        {
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.Name;
            EraseSubStr(name, "__cdecl ");
            std::replace(name.begin(), name.end(), '"', '\'');
            std::stringstream json;

            json << "{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.End - result.Start) << ',';
            json << "\"name\":\"" << name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.ThreadID << ",";
            json << "\"ts\":" << result.Start;
            json << "}";

            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession) {
                m_OutputStream << json.str();
                m_OutputStream.flush();
            }
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

            Instrumentor::Get().WriteProfile({ m_Name, start, end, std::this_thread::get_id() });

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
    // Resolve which function signature macro will be used. Note that this only
    // is resolved when the (pre)compiler starts, so the syntax highlighting
    // could mark the wrong one in your editor!
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
        #define PR_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
        #define PR_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__FUNCSIG__)
        #define PR_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
        #define PR_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
        #define PR_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
        #define PR_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
        #define PR_FUNC_SIG __func__
    #else
        #define PR_FUNC_SIG "PR_FUNC_SIG unknown!"
    #endif
    


    #define PR_PROFILE_BEGIN_SESSION(name, filepath) ::Pearly::Instrumentor::Get().BeginSession(name, filepath)
    #define PR_PROFILE_END_SESSION() ::Pearly::Instrumentor::Get().EndSession()
    #define PR_PROFILE_SCOPE(name)  ::Pearly::InstrumentationTimer timer##__LINE__(name)
    #define PR_PROFILE_FUNCTION() PR_PROFILE_SCOPE(PR_FUNC_SIG)
#else
    #define PR_PROFILE_BEGIN_SESSION(name, filepath)
    #define PR_PROFILE_END_SESSION()
    #define PR_PROFILE_FUNCTION()
    #define PR_PROFILE_SCOPE(name)
#endif