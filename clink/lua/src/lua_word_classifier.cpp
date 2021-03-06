// Copyright (c) 2020 Christopher Antos
// License: http://opensource.org/licenses/MIT

#include "pch.h"
#include "lua_word_classifier.h"
#include "lua_state.h"
#include "line_state_lua.h"

#include <lib/line_state.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

//------------------------------------------------------------------------------
lua_word_classifier::lua_word_classifier(lua_state& state)
: m_state(state)
{
}

//------------------------------------------------------------------------------
void lua_word_classifier::print_error(const char* error) const
{
    puts("");
    puts(error);
}

//------------------------------------------------------------------------------
void lua_word_classifier::classify(const line_state& line, word_classifications& classifications) const
{
    lua_State* state = m_state.get_state();

    // Call to Lua to generate matches.
    lua_getglobal(state, "clink");
    lua_pushliteral(state, "_parse_word_types");
    lua_rawget(state, -2);

    line_state_lua line_lua(line);
    line_lua.push(state);

    if (m_state.pcall(state, 1, 1) != 0)
    {
        if (const char* error = lua_tostring(state, -1))
            print_error(error);

        lua_settop(state, 0);
        return;
    }

    const char* ret = lua_tostring(state, -1);
    lua_settop(state, 0);

    for (unsigned int i = 0; i < strlen(ret); i++)
    {
        word_class* c = classifications.push_back();
        switch (ret[i])
        {
        default:    *c = word_class::other; break;
        case 'c':   *c = word_class::command; break;
        case 'd':   *c = word_class::doskey; break;
        case 'a':   *c = word_class::arg; break;
        case 'f':   *c = word_class::flag; break;
        case 'n':   *c = word_class::none; break;
        }
    }
}
