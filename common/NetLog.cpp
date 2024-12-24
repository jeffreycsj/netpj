#include "NetLog.h"
#include <stdarg.h>
#include <iostream>

void CNetLog::LogMessage(const char *format, ...)
{
    va_list vaArgs;
    va_start(vaArgs, format);

    va_list vaCopy;
    va_copy(vaCopy, vaArgs);

    int len = vsnprintf(NULL, 0, format, vaCopy);
    va_end(vaCopy);

    char* buffer = new char[len + 1];
    vsnprintf(buffer, len + 1, format, vaArgs);

    va_end(vaArgs);

    if (m_callback) {
        m_callback(buffer);
    } else {
        std::cerr << buffer << std::endl;
    }

    delete[] buffer;
}

void CNetLog::SetCallBack(std::function<void(const char *)> callback)
{
    m_callback = callback;
}
