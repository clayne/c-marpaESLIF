static const char *MARPAESLIFLUA_MARPAESLIFJSON =
  "\n"
  "local marpaESLIFJSON = {}\n"
  "marpaESLIFJSON.new = function(marpaESLIFp, strict)\n"
  "   local marpaESLIFJSONEncoder = marpaESLIFp:marpaESLIFJSONEncoder_new(strict)\n"
  "   local marpaESLIFJSONDecoder = marpaESLIFp:marpaESLIFJSONDecoder_new(strict)\n"
  "\n"
  "   local self = {}\n"
  "\n"
  "   function self.encode(value)\n"
  "      return marpaESLIFJSONEncoder:encode(value)\n"
  "   end\n"
  "\n"
  "   function self.decode(value)\n"
  "      return marpaESLIFJSONDecoder:decode(value)\n"
  "   end\n"
  "\n"
  "   return self\n"
  "end\n"
  "\n"
  "return marpaESLIFJSON\n"
  "\n";