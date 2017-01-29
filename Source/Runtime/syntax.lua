-- Provides an API for describing high-level syntax highlighting primitives using LPeg.

local L = require("lpeg")

S = {}

function S.token(attribute, pattern)
  return L.Cc(attribute) * L.Cp() * pattern * L.Cp()
end

function S.ignore(pattern)
end

return S
