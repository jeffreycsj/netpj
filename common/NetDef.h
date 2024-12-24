/**
 * @file NetDef.h
 * @author jeffreychen
 * @brief 网络通用定义
 * @version 0.1
 * @date 2024-10-14 11:02:33
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <math.h>

#include <cmath>
#include <cstring>
#include <functional>
#include <string>

enum NETWORK_TYPE
{
    NETWORK_TYPE_NONE         = 0,
    NETWORK_TYPE_TCP_ACCEPTOR = 1,
    NETWORK_TYPE_TCP          = 2,
    NETWORK_TYPE_UDP_ACCEPTOR = 3,
    NETWORK_TYPE_UDP          = 4,
    NETWORK_TYPE_KCP_ACCEPTOR = 5,
    NETWORK_TYPE_KCP          = 6,
};

enum NETWORK_CALLBACK
{
    NETWORK_CALLBACK_LISTENED              = 0,  // 监听完成
    NETWORK_CALLBACK_CONNECTED             = 1,  // 连接完成
    NETWORK_CALLBACK_CLOSED                = 2,  // 关闭
    NETWORK_CALLBACK_SENT                  = 3,  // 消息发送完毕
    NETWORK_CALLBACK_RECEIVED              = 4,  // 消息到达
};

/**
 * @brief 网络回调接口：handle, 回调类型(参考 NETWORK_CALLBACK)，数据，数据长度
 *
 */
using NetworkCallback = std::function<void(uint32_t, int32_t, const char*, int32_t)>;

// 以下的逻辑， 无需上一层知道
#pragma pack(push, 1)

// tcp 连接建立之前的头部数据
struct TCPPreHeadData
{
    uint32_t m_head{0xCA0DA011};  // 魔数头
    char m_openID[64] = {0};         // openid
    char m_isReconnect{0};                   // 是否重连
    uint32_t m_connectResult{0};        // 如果连接被拒，这里带上被拒原因
};

// tcp 连接上后，每次数据的头
struct TCPHeadData
{
    uint8_t m_msgType;      // 数据类型， 参考 NETWORK_PACKET_TYPE
    uint8_t PLACEHOLDER_1;  // 占位
    uint8_t PLACEHOLDER_2;  // 占位
    uint8_t PLACEHOLDER_3;  // 占位
};


// 自定义的 Network 层的 udp 包头，与上层业务逻辑无关
struct UDPHead
{
    uint8_t m_isReliable : 1;  // 是否可靠消息
    uint8_t m_msgType : 7;     // 消息类型，参考 NETWORK_PACKET_TYPE
};

using KCPHead = UDPHead;

struct HandshakeData
{
    uint32_t m_timestamp{0};        // 时间戳，由服务器返回的
    uint32_t m_connectID{0};        // 连接ID, 握手成功后，为当前连接分配的唯一ID
    char m_cookies[20]{0};          // 用来验证合法性的 cookie， 根据 客户端地址端口与当前时间戳 生成。为什么是20， 16字节地址信息+4字节的时间戳
    char m_openID[64]{0};           // openid
    char m_isReconnect{0};         // 是否重连
};

// udp, tcp， kcp 的数据都是这个结构
struct MsgDataHead
{
    uint32_t m_msgLength;
    char m_msgBody[0];
};

// 用来打包/解包用的辅助结构
struct TCPMsgData
{
    TCPHeadData m_headData;
    union
    {
        char m_msgData[0];
        MsgDataHead m_msgHead;  // 真正的数据
    };
};

// 用来打包/解包用的辅助结构
struct UDPMsgData
{
    UDPHead m_headData;
    union
    {
        char m_msgData[0];
        MsgDataHead m_msgHead;
    };
};

using KCPMsgData = UDPMsgData;

#pragma pack(pop)

// 握手 cookie 最长有效时间
const uint32_t MAX_HANDSHAKE_COOKIE_VALID_TIME = 10;

struct KCPKey
{
    char data[20];  // 16字节地址信息 + 4 字节端口
};

inline int operator<(const KCPKey& l, const KCPKey& r)
{
    return memcmp(l.data, r.data, sizeof(l.data)) < 0;
}

inline int operator==(const KCPKey& l, const KCPKey& r)
{
    return memcmp(l.data, r.data, sizeof(l.data)) == 0;
}

inline std::size_t hash_value(const KCPKey& key)
{
    unsigned char* bp = (unsigned char*)key.data;  // start of buffer
    unsigned char* be = bp + sizeof(key);          // beyond end of buffer
    uint64_t hval     = 0xcbf29ce484222325ULL;

    while (bp < be)
    {
        hval ^= (uint64_t)*bp++;
        hval += (hval << 1) + (hval << 4) + (hval << 5) + (hval << 7) + (hval << 8) + (hval << 40);
    }

    return hval;
}

enum NETWORK_PACKET_TYPE
{
    NETWORK_PACKET_TYPE_DATA                 = 0,   // 数据包
    NETWORK_PACKET_TYPE_HANDSHAKE_FIRST      = 1,   // 第一次握手包
    NETWORK_PACKET_TYPE_HANDSHAKE_FIRST_ACK  = 2,   // 第一次握手回包
    NETWORK_PACKET_TYPE_HANDSHAKE_SECOND     = 3,   // 第二次握手包
    NETWORK_PACKET_TYPE_HANDSHAKE_SECOND_ACK = 4,   // 第二次握手回包
    NETWORK_PACKET_TYPE_HEARTBEAT            = 5,   // 心跳包
    NETWORK_PACKET_TYPE_HEARTBEAT_ACK        = 6,   // 心跳回包
    NETWORK_PACKET_TYPE_RECONNECT            = 7,   // 重连包
    NETWORK_PACKET_TYPE_RECONNECT_ACK        = 8,   // 重连回包
    NETWORK_PACKET_TYPE_CLOSE                = 9,   // 客户端主动关闭连接
    NETWORK_PACKET_TYPE_CLOSE_ACK            = 10,  // 客户端主动关闭连接的回包
    NETWORK_PACKET_TYPE_FIN                  = 11,  // 服务器主动关闭连接
    NETWORK_PACKET_TYPE_FIN_ACK              = 12,  // 服务器主动关闭连接的回包
    NETWORK_PACKET_TYPE_RST                  = 13,  // 不可靠的，通知客户端重置网络
};


// 关闭连接数据
struct NetworkPacketTypeCloseData
{
    uint32_t m_reason;
};

// 请求 kcp 重连
struct NetworkPacketTypeReconnectData
{
    uint32_t m_connID;        // kcp 的 connid
};

struct NetworkPacketTypeReconnectACKData
{
    uint32_t m_connID;        // kcp 的 connid,如果为 0 ，表示重连失败
};

struct ExtraConnectionInfo
{
    std::string m_openID;  // 账号 openid
    bool m_isReconnect{false};    // 是否重连
};

// UDP的最大传输单元为何是548？
// 1.在链路层，由以太网的物理特性决定了数据帧的长度为(46＋18)－(1500＋18)，其中的18是数据帧的头和尾，也就是说数据帧的内容最大为1500(不包括帧头和帧尾)，即MTU(Maximum Transmission Unit)为1500； 　
// 2.在网络层，因为IP包的首部要占用20字节，所以这的MTU为1500－20＝1480；　
// 3.在传输层，对于UDP包的首部要占用8字节，所以这的MTU为1480－8＝1472； 　　
// 所以，在应用层，你的Data最大长度为1472。当我们的UDP包中的数据多于MTU(1472)时，发送方的IP层需要分片fragmentation进行传输，而在接收方IP层则需要进行数据报重组，由于UDP是不可靠的传输协议，如果分片丢失导致重组失败，将导致UDP数据包被丢弃。 　　
// 从上面的分析来看，在普通的局域网环境下，UDP的数据最大为1472字节最好(避免分片重组)。 　　
// 但在网络编程中，Internet中的路由器可能有设置成不同的值(小于默认值)，Internet上的标准MTU值为576，所以Internet的UDP编程时数据长度最好在576－20－8＝548字节以内
const int UDP_MTU = 548;

// 超时时间，秒
const static int NETWORK_TIMEOUT       = 6;
const static int KCP_HEARTBEAT_TIMEOUT = 30;  // kcp 心跳超时时间
const static int KCP_HANDSHAKE_RETRY_TIME = 1; //  防止握手丢包，重发包的间隔
const static int KCP_WND_SIZE = 256; // kcp 的发送和接收窗口