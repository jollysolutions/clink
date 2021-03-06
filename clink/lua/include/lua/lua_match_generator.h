// Copyright (c) 2015 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#pragma once

#include <lib/match_generator.h>

class lua_state;

//------------------------------------------------------------------------------
class lua_match_generator
    : public match_generator
{
public:
                    lua_match_generator(lua_state& state);
    virtual         ~lua_match_generator();

private:
    virtual bool    generate(const line_state& line, match_builder& builder) override;
    virtual void    get_word_break_info(const line_state& line, word_break_info& info) const override;
    void            print_error(const char* error) const;
    lua_state&      m_state;
};
