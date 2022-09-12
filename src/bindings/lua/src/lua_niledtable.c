static const char *MARPAESLIFLUA_NILEDTABLE =
  "\n"
  "-- NiledTable.lua\n"
  "local M = {}\n"
  "\n"
  "-- weak table for representing proxied storage tables.\n"
  "local data = setmetatable({}, {__mode = 'k'})\n"
  "local countnildata = setmetatable({}, {__mode = 'k'})\n"
  "\n"
  "-- nil placeholder.\n"
  "-- Note: this value is not exposed outside this module, so\n"
  "-- there's typically no possibility that a user could attempt\n"
  "-- to store a 'nil placeholder' in a table, leading to the\n"
  "-- same problem as storing nils in tables.\n"
  "local NIL = {\n"
  "   __tostring = function() return tostring(nil) end,\n"
  "   __type = function() return type(nil) end\n"
  "}\n"
  "setmetatable(NIL, NIL)\n"
  "\n"
  "-- metatable for NiledTable's.\n"
  "local mt = {}\n"
  "function mt.__index(t,k)\n"
  "  -- print('__index('..tostring(t)..', '..tostring(k)..')')\n"
  "  local d = data[t]\n"
  "  local v = d and d[k]\n"
  "  if v == NIL then v = nil end\n"
  "  return v\n"
  "end\n"
  "-- This function is a hook to access directly inner data: if a niled table has no niled data, then __nileddata returns the raw table\n"
  "-- it allows marpaESLIFLua to call directly lua_next instead of a doing a lua_call at every iteration\n"
  "function mt.__nileddata(t)\n"
  "  -- print('__nileddata('..tostring(t)..')')\n"
  "  if not countnildata[t] then                         -- No such table or no NIL\n"
  "    local d = data[t]\n"
  "    if d == nil then\n"
  "      -- print('==> Case 1 : return {}')\n"
  "      return {}\n"
  "    else\n"
  "      -- print('==> Case 2 : return '..tostring(d))\n"
  "      return d\n"
  "    end\n"
  "    -- return data[t] or {}\n"
  "  else\n"
  "    -- print('==> Case 3 : return nil')\n"
  "    return nil\n"
  "  end\n"
  "end\n"
  "function mt.__newindex(t,k,v)\n"
  "  -- print('__newindex('..tostring(t)..', '..tostring(k)..', '..tostring(v)..')')\n"
  "  if v == nil then\n"
  "    local n = countnildata[t]\n"
  "    if not n then\n"
  "      countnildata[t] = 0\n"
  "    else\n"
  "      countnildata[t] = countnildata[t] + 1\n"
  "    end\n"
  "    v = NIL\n"
  "  end\n"
  "  local d = data[t]\n"
  "  if not d then\n"
  "    d = {}\n"
  "    data[t] = d\n"
  "  end\n"
  "  d[k] = v\n"
  "  -- print('__newindex('..tostring(t)..', '..tostring(k)..', '..tostring(v)..') : countnildata('..tostring(t)..') = '..tostring(countnildata[t]))\n"
  "end\n"
  "function mt.__len(t)  -- note: ignored by Lua but used by exlen below\n"
  "  -- print('__len('..tostring(t)..')')\n"
  "  local d = data[t]\n"
  "  return d and #d or 0\n"
  "end\n"
  "\n"
  "-- constructor\n"
  "setmetatable(M, {__call = function(class, t)\n"
  "  return setmetatable(t, mt)\n"
  "end})\n"
  "\n"
  "function M.exists(t, k)\n"
  "  -- print('exists('..tostring(t)..', '..tostring(k)..')')\n"
  "  local d = data[t]\n"
  "  return (d and d[k]) ~= nil\n"
  "end\n"
  "local exists = M.exists\n"
  "\n"
  "function M.exlen(t)\n"
  "  -- print('exlen('..tostring(t)..')')\n"
  "  local mt = getmetatable(t)\n"
  "  local len = mt.__len\n"
  "  return len and len(t) or #t\n"
  "end\n"
  "\n"
  "local function exipairs_iter(t, i)\n"
  "  i = i + 1\n"
  "  if exists(t, i) then\n"
  "    local v = t[i]\n"
  "    return i, v\n"
  "  end\n"
  "end\n"
  "\n"
  "-- ipairs replacement that handles nil values in tables.\n"
  "function M.exipairs(t, i)\n"
  "  -- print('exipairs('..tostring(t)..', '..tostring(i)..')')\n"
  "  return exipairs_iter, t, 0\n"
  "end\n"
  "\n"
  "-- next replacement that handles nil values in tables\n"
  "function M.exnext(t, k)\n"
  "  -- print('exnext('..tostring(t)..', '..tostring(k)..')')\n"
  "  local d = data[t]\n"
  "  if not d then return end\n"
  "  k, v = next(d, k)\n"
  "  -- print('... => '..tostring(k)..', '..tostring(v))\n"
  "  return k, v\n"
  "end\n"
  "local exnext = M.exnext\n"
  "\n"
  "-- pairs replacement that handles nil values in tables.\n"
  "function M.expairs(t, i)\n"
  "  -- print('expairs('..tostring(t)..', '..tostring(i)..')')\n"
  "  return exnext, t, nil\n"
  "end\n"
  "\n"
  "-- Remove key in table.  This is used since there is no\n"
  "-- value v such that t[k] = v will remove k from the table.\n"
  "function M.delete(t, k)\n"
  "  -- print('delete('..tostring(t)..', '..tostring(k)..')')\n"
  "  local d = data[t]\n"
  "  if d then\n"
  "    d[k] = nil\n"
  "    if d[k] == NIL then\n"
  "      local n = countnildata[t]\n"
  "      if n then\n"
  "        countnildata[t] = countnildata[t] - 1\n"
  "      end\n"
  "    end\n"
  "  end\n"
  "end\n"
  "\n"
  "-- array constructor replacement.  used since {...} discards nils.\n"
  "function M.niledarray(...)\n"
  "  -- print('niledarray(...)')\n"
  "  local n = select('#', ...)\n"
  "  local d = {...}\n"
  "  local _mt = { __index     = mt.__index,\n"
  "                __countnil  = mt.__countnil,\n"
  "                __newindex  = mt.__newindex, \n"
  "                __nileddata = mt.__nileddata, \n"
  "                __len       = mt.__len, \n"
  "                __pairs     = mt.__pairs, \n"
  "                __ipairs    = mt.__ipairs, \n"
  "                __next      = mt.__next, \n"
  "                __exists    = mt.__exists, \n"
  "                __delete    = mt.__delete,\n"
  "                canarray    = true }\n"
  "  local t = setmetatable({}, _mt)\n"
  "  countnildata[t] = 0\n"
  "  for i = 1,n do\n"
  "    if d[i] == nil then\n"
  "      d[i] = NIL -- This is a direct data assignment\n"
  "      countnildata[t] = countnildata[t] + 1\n"
  "    end\n"
  "  end\n"
  "  data[t] = d\n"
  "  return t\n"
  "end\n"
  "\n"
  "-- table constructor replacement.  used since {...} discards nils.\n"
  "function M.niledtablekv(...)\n"
  "  -- print('niledtablekv(...)')\n"
  "  -- possibly more optimally implemented in C.\n"
  "  local n = select('#', ...)\n"
  "  local tmp = {...} -- it would be nice to avoid this\n"
  "  local _mt = { __index     = mt.__index,\n"
  "                __countnil  = mt.__countnil,\n"
  "                __newindex  = mt.__newindex, \n"
  "                __nileddata = mt.__nileddata, \n"
  "                __len       = mt.__len, \n"
  "                __pairs     = mt.__pairs, \n"
  "                __ipairs    = mt.__ipairs, \n"
  "                __next      = mt.__next, \n"
  "                __exists    = mt.__exists, \n"
  "                __delete    = mt.__delete,\n"
  "                canarray    = false }\n"
  "  local t = setmetatable({}, _mt)\n"
  "  countnildata[t] = 0\n"
  "  for i = 1,n,2 do\n"
  "    t[tmp[i]] = tmp[i+1] -- This will call __newindex\n"
  "   end\n"
  "  return t\n"
  "end\n"
  "\n"
  "mt.__pairs  = M.expairs\n"
  "mt.__ipairs = M.exipairs\n"
  "mt.__next   = M.exnext\n"
  "mt.__exists = M.exists\n"
  "mt.__delete = M.delete\n"
  "\n"
  "return M\n"
  "\n";
