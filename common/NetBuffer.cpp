#include "NetBuffer.h"
#include <cstring>

NetBuffer::NetBuffer(int32_t cap): m_len(0), m_startIdx(0), m_idx(-1)
{
    m_buffer.resize(cap);
}

NetBuffer::~NetBuffer()
{
}

char *NetBuffer::GetData()
{
    return &*(m_buffer.begin() + m_startIdx);
}

void NetBuffer::Scale(int32_t cap)
{
    if(m_buffer.capacity() >= cap)
    {
        return;
    }

    m_buffer.resize(1024 * (cap / 1024 + 1), 0);
}

void NetBuffer::ClearData()
{
    m_startIdx = 0;
    m_len = 0;
    memset(m_buffer.data(), 0, m_buffer.size());
}

int32_t NetBuffer::GetCap()
{
    return m_buffer.capacity();
}

int32_t NetBuffer::GetLen()
{
    return m_len - m_startIdx;
}

void NetBuffer::SetLen(int32_t len)
{
    if(len > m_buffer.capacity())
    {
        Scale(len);
    }
    m_len = len;
}
