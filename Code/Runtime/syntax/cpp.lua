local L = require("lpeg")
local S = require("syntax")

local preprocessor_directive = S.token("Preprocessor", L.P("#") * (L.P("ifdef") + L.P("ifndef") + L.P("if") + L.P("else") + L.P("endif") + L.P("include") + L.P("define") + L.P("undef")))
local item = preprocessor_directive + (L.P(1) / S.ignore)
local primary = L.Ct(item^1)

-- The first and only argument to the function is the text to be matched.
local line = ...
local result = primary:match(line)

-- A Quip syntax file returns a table of all the captured tokens.
return result
