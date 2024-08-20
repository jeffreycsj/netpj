#include "NetBufferMgr.h"

void NetBufferMgr::PreAllocateBuffer(int32_t num, int32_t cap)
{
    if(num > 0)
    {
        m_freeBuffers.reserve(num);
    }

    for (int32_t i = 0; i < num; i++)
    {
        m_freeBuffers.push_back(new NetBuffer(cap));
    }
}

std::shared_ptr<NetBuffer> NetBufferMgr::AvailableBuffer()
{
    if (m_freeBuffers.empty())
    {
        m_freeBuffers.push_back(new NetBuffer());
    }
    NetBuffer* buffer = nullptr;
    buffer = m_freeBuffers.back();
    m_freeBuffers.pop_back();
    m_busyBuffers.push_back(buffer);
    buffer->m_idx = m_busyBuffers.size() - 1;
    return std::make_shared<NetBuffer>(buffer, [](NetBuffer* p) { NetBufferMgr::Inst().ReclaimBuffer(p); });;
}

void NetBufferMgr::ReclaimBuffer(NetBuffer *buffer)
{
    if (buffer->m_idx < 0)
    {
        return;
    }
    m_busyBuffers[buffer->m_idx] = m_busyBuffers.back();
    m_busyBuffers[buffer->m_idx]->m_idx = buffer->m_idx;
    m_busyBuffers[m_busyBuffers.size() - 1] = buffer;
    m_busyBuffers.pop_back();
    buffer->m_idx = -1;
    buffer->ClearData();
    m_freeBuffers.push_back(buffer);
}
