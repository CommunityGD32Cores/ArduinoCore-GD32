#include <fatal.h>
#include <stdarg.h>
#include <stdio.h>
#include <variant.h>

/* standard setting: swallow fatal erros for firmware size reasons */
#ifndef FATAL_LOGS_TO_SERIAL
#define FATAL_LOGS_TO_SERIAL 0
#endif

void fatal(const char *format, ...)
{
#if FATAL_LOGS_TO_SERIAL != 0
    va_list arglist;
    va_start(arglist, format);
    vprintf(format, arglist);
    va_end(arglist);
#endif
}