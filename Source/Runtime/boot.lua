local lpeg = require("lpeg")

if (lpeg.P("hello"):match("hello") == 6) then
  print("Welcome to Quip!")
end
