#ifndef MARPAESLIF_JSONTESTERSTRICTEXTERNAL_H
#define MARPAESLIF_JSONTESTERSTRICTEXTERNAL_H

static const char *grammars =
  "# --------------------------------------------------\n"
  "# Meta settings\n"
  "# --------------------------------------------------\n"
  ":desc              ::= 'Strict JSON Grammar'\n"
  ":default           ::= action => ::shift fallback-encoding => UTF-8 discard-is-fallback => 1\n"
  "\n"
  "# ---------------------------------\n"
  "# Discard unsignificant whitespaces\n"
  "# ---------------------------------\n"
  ":discard           ::= /[\\x{9}\\x{A}\\x{D}\\x{20}]+/\n"
  "\n"
  "#\n"
  "# ---------------\n"
  "# Terminal events\n"
  "# ---------------\n"
  ":symbol ::= '\"' pause => after event => :discard[switch]\n"
  "\n"
  "# ----------\n"
  "# JSON value\n"
  "# ----------\n"
  "value               ::= object\n"
  "                      | array\n"
  "                      | string\n"
  "                      | constant\n"
  "                      | number\n"
  "\n"
  "# -----------\n"
  "# JSON object\n"
  "# -----------\n"
  "object              ::= (-'{'-) members (-'}'-)\n"
  "members             ::= member*                      action         => ::lua->members\n"
  "                                                     separator      => ','\n"
  "                                                     proper         => 1\n"
  "                                                     hide-separator => 1\n"
  "member              ::= string (-':'-) value         action         => ::row\n"
  "\n"
  "# ----------\n"
  "# JSON Array\n"
  "# ----------\n"
  "array               ::= (-'['-) elements (-']'-)\n"
  "elements            ::= value*                       action         => ::row\n"
  "                                                     separator      => ','\n"
  "                                                     proper         => 1\n"
  "                                                     hide-separator => 1\n"
  "\n"
  "# -----------\n"
  "# JSON String\n"
  "# -----------\n"
  "string              ::= (-'\"'-) string_parts (-'\"'-)\n"
  "string_parts        ::= string_part*                 action         => ::concat\n"
  "string_part         ::= string_ascii_part\n"
  "                      | string_escape_part           \n"
  "                      | string_unicode_part          \n"
  "string_ascii_part   ::= /[^\"\\\\\\x00-\\x1F]+/u\n"
  "string_escape_part  ::= /(?:\\\\[\"\\\\\\/bfnrt])+/u action         => ::lua->string_escape_part\n"
  "string_unicode_part ::= /(?:\\\\u[[:xdigit:]]{4})+/u action         => ::lua->string_unicode_part\n"
  "\n"
  "\n"
  "# -------------\n"
  "# JSON constant\n"
  "# -------------\n"
  "constant ::= 'true'                                  action         => ::true\n"
  "           | 'false'                                 action         => ::false\n"
  "           | 'null'                                  action         => ::undef\n"
  "                     \n"
  "\n"
  "# -----------\n"
  "# JSON number\n"
  "# -----------\n"
  "number ::= /-?(?:0|[1-9][0-9]*)(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?/ action => ::luac->function(input) return tonumber(input) end\n"
  "\n"
  "<luascript>\n"
  "function members(...)\n"
  "  local output = niledtablekv()\n"
  "  if arg ~= nil then\n"
  "    for n=1,select('#',...) do\n"
  "      local member = select(n, ...)\n"
  "      output[member[1]] = member[2]\n"
  "    end\n"
  "  end\n"
  "  return output\n"
  "end\n"
  "\n"
  "function string_escape_part(input)\n"
  "  local c = input:sub(2, 2) -- get second character\n"
  "  if     c == '\"'  then return c\n"
  "  elseif c == '\\\\' then return c\n"
  "  elseif c == '/'  then return c\n"
  "  elseif c == 'b'  then return '\\b'\n"
  "  elseif c == 'f'  then return '\\f'\n"
  "  elseif c == 'n'  then return '\\n'\n"
  "  elseif c == 'r'  then return '\\r'\n"
  "  elseif c == 't'  then return '\\t'\n"
  "  else   error('Unsupported escaped character '..c)\n"
  "  end\n"
  "end\n"
  "\n"
  "function string_unicode_part(input)\n"
  "  local stringtable = {}\n"
  "  local uint32 = {}\n"
  "  local p = 1\n"
  "  local pmax = input:len()\n"
  "  while p <= pmax do\n"
  "    local u = input:sub(p+2, p + 5)\n"
  "    uint32[#uint32+1] = tonumber('0x'..u)\n"
  "    p = p + 6\n"
  "  end\n"
  "  local j = 2\n"
  "  for i = 1, #uint32 do\n"
  "    local u = uint32[i]\n"
  "    if ((j <= #uint32) and (u >= 0xD800) and (u <= 0xDBFF) and (uint32[j] >= 0xDC00) and (uint32[j] <= 0xDFFF)) then\n"
  "      -- Surrogate UTF-16 pair\n"
  "      u = 0x10000 + ((u & 0x3FF) << 10) + (uint32[j] & 0x3FF)\n"
  "      i = i + 1\n"
  "      j = j + 1\n"
  "    end\n"
  "    if ((u >= 0xD800) and (u <= 0xDFFF)) then\n"
  "      u = 0xFFFD -- Replacement character\n"
  "    end\n"
  "    c = utf8.char(u)\n"
  "    stringtable[#stringtable + 1] = c\n"
  "  end\n"
  "  return table.concat(stringtable)\n"
  "end\n"
  "</luascript>\n"
  ;

#endif /* MARPAESLIF_JSONTESTERSTRICTEXTERNAL_H */
