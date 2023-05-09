#pragma once
#include "../debug.h"

enum LoggerTarget {
    NONE,
    TERMINAL,
    SERIAL,
    BOTH
};

class Logger {
    public:
    char* tag;
    char* nspace;
    LoggerTarget target = LoggerTarget::BOTH;
    
    Logger(char* tag, char* nspace) {
        this->tag = tag;
        this->nspace = nspace;
    }

    /// @brief Log in the logger
    void info(char* str, ...) {
        va_list args;
        va_start(args, str);

        __internal_log("] $c7[INF]$cF ", str, args);
    }
    
    /// @brief Log in the logger
    void warn(char* str, ...) {
        va_list args;
        va_start(args, str);

        __internal_log("] $cY[WRN]$cF ", str, args);
    }
    
    /// @brief Log in the logger
    void error(char* str, ...) {
        va_list args;
        va_start(args, str);

        __internal_log("] $cR[ERR]$cF ", str, args);
    }

    private:
    void __internal_log(char* tpStr, char* str, va_list args) {
        FastStringBuilder* builder = new FastStringBuilder(7);
        builder->append("$c8[");
        builder->append(nspace);
        builder->append("/");
        builder->append(tag);
        builder->append(tpStr);
        builder->append(str);
        builder->append("\n");

        switch(target) {
            case LoggerTarget::BOTH:
                vakprintf_both(builder->buildAndDestroy(), args);
                break;
            case LoggerTarget::SERIAL:
                vakprintf_serial(builder->buildAndDestroy(), args);
                break;
            case LoggerTarget::TERMINAL:
                vakprintf(builder->buildAndDestroy(), args);
                break;
            case LoggerTarget::NONE:
                delete builder;
                break;
        }
    }
};