#pragma once

#include "_global.h"

namespace Exprfunc
{
    duint srcline(duint addr);
    duint srcdisp(duint addr);

    duint modparty(duint addr);

    duint disasmsel();
    duint dumpsel();
    duint stacksel();

    duint peb();
    duint teb();

    duint bswap(duint value);
}