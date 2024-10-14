/**
 * @file NetLog.h
 * @author jeffreychen
 * @brief 网络日志
 * @version 0.1
 * @date 2024-10-14 11:02:33
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <functional>
class CNetLog {
public:
    static CNetLog& Inst() {
        static CNetLog s_inst;
        return s_inst;
    }

    void LogMessage(const char* format, ...);

    void SetCallBack(std::function<void(const char*)> callback);
private:
    std::function<void(const char* )> m_callback;
};