#pragma once

namespace Pearly {

    class Timestep
    {
    public:
        Timestep(float time = 0.0f)
            : m_Time(time)
        {}

        inline float GetSeconds() const { return m_Time; }
        inline float GetMiliseconds() const { return m_Time * 1000; }

        template<typename T>
        Timestep operator+ (T number) const { return Timestep(m_Time + number); }
        template<typename T>
        Timestep operator- (T number) const { return Timestep(m_Time - number); }
        template<typename T>
        Timestep operator* (T number) const { return Timestep(m_Time * number); }
        template<typename T>
        Timestep operator/ (T number) const { return Timestep(m_Time / number); }

        operator float() const { return m_Time; }

        explicit operator short () const { return (short)m_Time; }
        explicit operator int () const { return (int)m_Time; }
        explicit operator long () const { return (long)m_Time; }
        explicit operator double () const { return (double)m_Time; }

    private:
        float m_Time;
    };


    inline std::ostream& operator<<(std::ostream& os, Timestep e)
    {
        return os << e.GetSeconds() << "s";
    }
}