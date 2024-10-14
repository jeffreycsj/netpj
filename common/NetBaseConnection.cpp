#include "NetBaseConnection.h"
#include "netinet/in.h"

CNetBaseConnection::CNetBaseConnection(uint32_t handle, const ExtraConnectionInfo &extraInfo)
:m_handle(handle), m_openID(extraInfo.m_openID), m_isReconnect(extraInfo.m_isReconnect)
{
}

void CNetBaseConnection::InitMsgDataHead(MsgDataHead* msgDataHead, const char* data, int32_t len)
{
    msgDataHead->m_msgLength = len;
    if(msgDataHead->m_msgBody != data)
    {
        memcpy(msgDataHead->m_msgBody, data, len);
    }

}

bool CNetBaseConnection::DecodeMsgDataHead(MsgDataHead *msgDataHead, int32_t totalLen)
{
    int32_t len = ntohl(msgDataHead->m_msgLength);
    if(len > totalLen)
    {
        return false;
    }
    msgDataHead->m_msgLength = len;
    return true;
}
