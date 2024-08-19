/**
 * @file NetBuffer.h
 * @author jeffreychen (jeffreychen@chaocanshu.ai)
 * @brief 网络buffer
 * @version 0.1
 * @date 2024-8-19 10:42:26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#include <stdint.h>
#include <vector>

class NetBuffer {
public:
    NetBuffer(int32_t cap);
    ~NetBuffer();

    /**
     * @brief 拿到数据基址
    */
    char* GetData();

    /**
     * @brief 扩容
    */
    void Scale(int32_t cap);

    /**
     * @brief 清空数据
    */
    void ClearData();

    /**
     * 获得buffer容量
    */
    int32_t GetCap();

    /**
     * 获得实际数据长度
    */
    int32_t GetLen();

    /**
     * 设置长度，ps：如果有发生扩容行为，原来指针会失效
    */
    void SetLen(int32_t len);
private:
    std::vector<char> m_buffer;
    int32_t m_len;
    int32_t m_startIdx;
};