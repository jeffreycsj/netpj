/**
 * @file NetBufferMgr.h
 * @author jeffreychen
 * @brief 网络buffer管理
 * @version 0.1
 * @date 2024-8-19 11:50:05
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <stdint.h>
#include <vector>
#include <memory>
#include "NetBuffer.h"

class NetBufferMgr {
public:
    NetBufferMgr(){}
    ~NetBufferMgr(){}

    static NetBufferMgr& Inst()
    {
        static NetBufferMgr inst;
        return inst;
    }

    /**
     * @brief 预分配buffer
    */
    void PreAllocateBuffer(int32_t num, int32_t cap);

    /**
     * @brief 获取可用的buffer
    */
    std::shared_ptr<NetBuffer> AvailableBuffer();

    /**
     * @brief 回收buffer
    */
    void ReclaimBuffer(NetBuffer* buffer);
private:
    std::vector<NetBuffer*> m_freeBuffers;
    std::vector<NetBuffer*> m_busyBuffers;
};