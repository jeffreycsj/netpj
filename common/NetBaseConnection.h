/**
 * @file NetBaseConnection.h
 * @author jeffreychen
 * @brief 网络连接抽象
 * @version 0.1
 * @date 2024-10-14 16:29:36
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "NetDef.h"
#include <cstdint>

class CNetBaseConnection {
public:
    CNetBaseConnection(uint32_t handle, const ExtraConnectionInfo& extraInfo);

    uint32_t Handle() { return m_handle; }

    const std::string& OpenID() { return m_openID; }

    bool IsReconnect() { return m_isReconnect; }

    virtual const std::string& IP() = 0;

    void InitMsgDataHead(MsgDataHead* msgDataHead, const char* data, int32_t len);

    bool DecodeMsgDataHead(MsgDataHead* msgDataHead, int32_t totalLen);
private:
    uint32_t m_handle;
    std::string m_openID;
    bool m_isReconnect;
};