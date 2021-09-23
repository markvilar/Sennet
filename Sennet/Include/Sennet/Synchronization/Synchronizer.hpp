#pragma once

#include <chrono>
#include <fstream>
#include <mutex>

#include "Sennet/Synchronization/Timestamp.hpp"

namespace Sennet
{

struct SynchronizationSession
{
    std::string Name;
};

class Synchronizer
{
public:
    Synchronizer(const Synchronizer&) = delete;
    Synchronizer(Synchronizer&&) = delete;

    void BeginSession(const std::string& name,
        const std::string& filepath = "synchronization.json")
    {
        std::lock_guard lock(m_Mutex);
        if (m_CurrentSession)
        {
            if (Log::GetCoreLogger())
            {
                SENNET_CORE_ERROR("Synchronizer::BeginSession('{0}') when session \
					'{1}' already open.",
                    name,
                    m_CurrentSession->Name);
            }
            InternalEndSession();
        }
        m_OutputStream.open(filepath);

        if (m_OutputStream.is_open())
        {
            m_CurrentSession = new SynchronizationSession({name});
            WriteHeader();
        }
        else
        {
            if (Log::GetCoreLogger())
            {
                SENNET_CORE_ERROR("Synchronizer could not open synchronization \
					file '{0}'",
                    filepath);
            }
        }
    }

    void EndSession()
    {
        std::lock_guard lock(m_Mutex);
        InternalEndSession();
    }

    void WriteEntry(const std::string localName,
        const Timestamp& local,
        const std::string remoteName,
        const Timestamp& remote)
    {
        std::stringstream json;
        json << std::setprecision(6) << std::fixed;
        json << ",{";
        json << "\"local\":\"" << localName << "\",";
        json << "\"localtime\":" << local.GetMilliseconds() << ",";
        json << "\"remote\":\"" << remoteName << "\",";
        json << "\"remotetime\":" << remote.GetMilliseconds();
        json << "}";

        std::lock_guard lock(m_Mutex);
        if (m_CurrentSession)
        {
            m_OutputStream << json.str();
            m_OutputStream.flush();
        }
    }

    static Synchronizer& Get()
    {
        static Synchronizer synchronizer;
        return synchronizer;
    }

private:
    Synchronizer() : m_CurrentSession(nullptr) {}

    ~Synchronizer() { EndSession(); }

    void WriteHeader()
    {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
        m_OutputStream.flush();
    }

    void WriteFooter()
    {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    void InternalEndSession()
    {
        if (m_CurrentSession)
        {
            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
        }
    }

private:
    std::mutex m_Mutex;
    SynchronizationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
};

} // namespace Sennet
