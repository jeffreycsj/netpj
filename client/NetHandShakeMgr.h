/**
 * @file NetHandShakeMgr.h
 * @author jeffreychen
 * @brief 网络握手管理类
 * @version 0.1
 * @date 2024-10-14 16:25:09
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

class CNetHandShakeMgr {
public:
    static CNetHandShakeMgr& Inst() {
        static CNetHandShakeMgr s_inst;
        return s_inst;
    }
};