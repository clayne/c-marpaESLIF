#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <genericLogger.h>
#include <marpaESLIF.h>

static short inputReaderb(void *userDatavp, char **inputsp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingOfEncodingsp, char **encodingsp, size_t *encodinglp);

typedef struct marpaESLIFTester_context {
  genericLogger_t *genericLoggerp;
  char            *inputs;
  size_t           inputl;
} marpaESLIFTester_context_t;

const static char *dsl = "\n"
":default ::= action => ::shift\n"
":start       ::= json\n"
"json         ::= object\n"
"               | array\n"
"object       ::= '{' members '}'       action => do_object\n"
"# comma is provided as a char class here, to ensure that char classes\n"
"# as separators are in the test suite.\n"
"members      ::= pair*                 action => do_array separator => [,]\n"
"pair         ::= string ':' value      action => do_array\n"
"value        ::= string\n"
"               | object\n"
"               | number\n"
"               | array\n"
"               | 'true'                action => do_true\n"
"               | 'false'               action => do_true\n"
"               | 'null'                action => ::undef\n"
"array        ::= '[' ']'               action => do_empty_array\n"
"               | '[' elements ']'\n"
"# comma is provided as a char class here, to ensure that char classes\n"
"# as separators are in the test suite.\n"
"elements     ::= value+                action => do_array separator => [,]\n"
"number         ~ int\n"
"               | int frac\n"
"               | int exp\n"
"               | int frac exp\n"
"int            ~ digits\n"
"               | '-' digits\n"
"digits         ~ [\\d]+\n"
"frac           ~ '.' digits\n"
"exp            ~ e digits\n"
"e              ~ 'e'\n"
"               | 'e+'\n"
"               | 'e-'\n"
"               | 'E'\n"
"               | 'E+'\n"
"               | 'E-'\n"
"string       ::= lstring\n"
":lexeme ::= lstring pause => after event => lstring$\n"
"lstring        ~ quote in_string quote\n"
"quote          ~ [\"]\n"
"in_string      ~ in_string_char*\n"
"in_string_char  ~ [^\"] | '\\\\\"'\n"
":discard       ::= whitespace\n"
"whitespace     ~ [\\s]+\n"
  ;

int main() {
  marpaESLIF_t                *marpaESLIFp        = NULL;
  marpaESLIFGrammar_t         *marpaESLIFGrammarp = NULL;
  marpaESLIFOption_t           marpaESLIFOption;
  marpaESLIFGrammarOption_t    marpaESLIFGrammarOption;
  int                          exiti;
  int                          ngrammari;
  char                        *grammarshows;
  int                          grammari;
  genericLogger_t             *genericLoggerp;
  marpaESLIFTester_context_t   marpaESLIFTester_context;
  marpaESLIFRecognizerOption_t marpaESLIFRecognizerOption;
  marpaESLIFRecognizer_t      *marpaESLIFRecognizerp = NULL;
  short                        continueb;
  short                        exhaustedb;
  int                          i;
  char                        *pauses;
  size_t                       pausel;
  const static char           *inputs[] = {
    "{\"test\":\"1\"}",
    "{\"test\":[1,2,3]}",
    "{\"test\":true}",
    "{\"test\":false}",
    "{\"test\":null}",
    "{\"test\":null, \"test2\":\"hello world\"}",
    "{\"test\":\"1.25\"}",
    "{\"test\":\"1.25e4\"}",
    "[]",
    "[\n"
    "  {\n"
    "     \"precision\": \"zip\",\n"
    "     \"Latitude\":  37.7668,\n"
    "     \"Longitude\": -122.3959,\n"
    "     \"Address\":   \"\",\n"
    "     \"City\":      \"SAN FRANCISCO\",\n"
    "     \"State\":     \"CA\",\n"
    "     \"Zip\":       \"94107\",\n"
    "     \"Country\":   \"US\"\n"
    "  },\n"
    "  {\n"
    "     \"precision\": \"zip\",\n"
    "     \"Latitude\":  37.371991,\n"
    "     \"Longitude\": -122.026020,\n"
    "     \"Address\":   \"\",\n"
    "     \"City\":      \"SUNNYVALE\",\n"
    "     \"State\":     \"CA\",\n"
    "     \"Zip\":       \"94085\",\n"
    "     \"Country\":   \"US\"\n"
    "  }\n"
    "]",
    "{\n"
    "  \"Image\": {\n"
    "    \"Width\":  800,\n"
    "    \"Height\": 600,\n"
    "    \"Title\":  \"View from 15th Floor\",\n"
    "    \"Thumbnail\": {\n"
    "        \"Url\":    \"http://www.example.com/image/481989943\",\n"
    "        \"Height\": 125,\n"
    "        \"Width\":  \"100\"\n"
    "    },\n"
    "    \"IDs\": [116, 943, 234, 38793]\n"
    "  }\n"
    "}",
    "{\n"
    "  \"source\" : \"<a href=\\\"http://janetter.net/\\\" rel=\\\"nofollow\\\">Janetter</a>\",\n"
    "  \"entities\" : {\n"
    "      \"user_mentions\" : [ {\n"
    "              \"name\" : \"James Governor\",\n"
    "              \"screen_name\" : \"moankchips\",\n"
    "              \"indices\" : [ 0, 10 ],\n"
    "              \"id_str\" : \"61233\",\n"
    "              \"id\" : 61233\n"
    "          } ],\n"
    "      \"media\" : [ ],\n"
    "      \"hashtags\" : [ ],\n"
    "     \"urls\" : [ ]\n"
    "  },\n"
    "  \"in_reply_to_status_id_str\" : \"281400879465238529\",\n"
    "  \"geo\" : {\n"
    "  },\n"
    "  \"id_str\" : \"281405942321532929\",\n"
    "  \"in_reply_to_user_id\" : 61233,\n"
    "  \"text\" : \"@monkchips Ouch. Some regrets are harsher than others.\",\n"
    "  \"id\" : 281405942321532929,\n"
    "  \"in_reply_to_status_id\" : 281400879465238529,\n"
    "  \"created_at\" : \"Wed Dec 19 14:29:39 +0000 2012\",\n"
    "  \"in_reply_to_screen_name\" : \"monkchips\",\n"
    "  \"in_reply_to_user_id_str\" : \"61233\",\n"
    "  \"user\" : {\n"
    "      \"name\" : \"Sarah Bourne\",\n"
    "      \"screen_name\" : \"sarahebourne\",\n"
    "      \"protected\" : false,\n"
    "      \"id_str\" : \"16010789\",\n"
    "      \"profile_image_url_https\" : \"https://si0.twimg.com/profile_images/638441870/Snapshot-of-sb_normal.jpg\",\n"
    "      \"id\" : 16010789,\n"
    "     \"verified\" : false\n"
    "  }\n"
    "}"
  };

  genericLoggerp = GENERICLOGGER_NEW(GENERICLOGGER_LOGLEVEL_DEBUG);
  if (genericLoggerp == NULL) {
    perror("GENERICLOGGER_NEW");
    goto err;
  }

  marpaESLIFOption.genericLoggerp = genericLoggerp;
  marpaESLIFp = marpaESLIF_newp(&marpaESLIFOption);
  if (marpaESLIFp == NULL) {
    goto err;
  }

  marpaESLIFGrammarOption.bytep               = (void *) dsl;
  marpaESLIFGrammarOption.bytel               = strlen(dsl);
  marpaESLIFGrammarOption.encodings           = NULL;
  marpaESLIFGrammarOption.encodingl           = 0;
  marpaESLIFGrammarOption.encodingOfEncodings = NULL;

  marpaESLIFGrammarp = marpaESLIFGrammar_newp(marpaESLIFp, &marpaESLIFGrammarOption);
  if (marpaESLIFGrammarp == NULL) {
    goto err;
  }

  /* Dump grammar */
  if (marpaESLIFGrammar_ngrammarib(marpaESLIFGrammarp, &ngrammari)) {
    for (grammari = 0; grammari < ngrammari; grammari++) {
      if (marpaESLIFGrammar_grammarshowform_by_grammarb(marpaESLIFGrammarp, &grammarshows, grammari, NULL)) {
        GENERICLOGGER_INFO (marpaESLIFOption.genericLoggerp, "-------------------------");
        GENERICLOGGER_INFOF(marpaESLIFOption.genericLoggerp, "TEST grammar at level %d:", grammari);
        GENERICLOGGER_INFOF(marpaESLIFOption.genericLoggerp, "-------------------------\n\n%s", grammarshows);
      }
    }
  }

  for (i = 0; i < (sizeof(inputs)/sizeof(inputs[0])); i++) {
    marpaESLIFTester_context.genericLoggerp = genericLoggerp;
    marpaESLIFTester_context.inputs         = (char *) inputs[i];
    marpaESLIFTester_context.inputl         = strlen(inputs[i]);

    marpaESLIFRecognizerOption.userDatavp                = &marpaESLIFTester_context;
    marpaESLIFRecognizerOption.marpaESLIFReaderCallbackp = inputReaderb;
    marpaESLIFRecognizerOption.disableThresholdb         = 0;
    marpaESLIFRecognizerOption.exhaustedb                = 0;
    marpaESLIFRecognizerOption.newlineb                  = 1;
    marpaESLIFRecognizerOption.bufsizl                   = 0;
    marpaESLIFRecognizerOption.buftriggerperci           = 50;
    marpaESLIFRecognizerOption.bufaddperci               = 50;

    marpaESLIFRecognizerp = marpaESLIFRecognizer_newp(marpaESLIFGrammarp, &marpaESLIFRecognizerOption);
    if (marpaESLIFRecognizerp == NULL) {
      goto err;
    }

    GENERICLOGGER_INFO(genericLoggerp, "Scanning JSON");
    GENERICLOGGER_INFO(genericLoggerp, "-------------");
    GENERICLOGGER_INFOF(genericLoggerp, "%s", inputs[i]);
    GENERICLOGGER_INFO(genericLoggerp, "-------------");
    /* Scan the input */
    /* genericLogger_logLevel_seti(genericLoggerp, GENERICLOGGER_LOGLEVEL_TRACE); */
    if (! marpaESLIFRecognizer_scanb(marpaESLIFRecognizerp, 1 /* initialEventsb */, &continueb, &exhaustedb)) {
      goto err;
    }

    while (continueb) {
      /* We have a single event, no need to ask what it is */
      marpaESLIFRecognizer_pausev(marpaESLIFRecognizerp, &pauses, &pausel, NULL /* eofbp */);
      GENERICLOGGER_INFOF(genericLoggerp, "Got lstring: %s; length=%ld", pauses, pausel);

      /* Resume */
      if (! marpaESLIFRecognizer_resumeb(marpaESLIFRecognizerp, &continueb, &exhaustedb)) {
        goto err;
      }
    }
  }

  exiti = 0;
  goto done;

 err:
  exiti = 1;

 done:
  marpaESLIFRecognizer_freev(marpaESLIFRecognizerp);
  marpaESLIFGrammar_freev(marpaESLIFGrammarp);
  marpaESLIF_freev(marpaESLIFp);

  GENERICLOGGER_FREE(genericLoggerp);
  return exiti;
}

/*****************************************************************************/
static short inputReaderb(void *userDatavp, char **inputsp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingOfEncodingsp, char **encodingsp, size_t *encodinglp)
/*****************************************************************************/
{
  marpaESLIFTester_context_t *marpaESLIFTester_contextp = (marpaESLIFTester_context_t *) userDatavp;

  *inputsp              = marpaESLIFTester_contextp->inputs;
  *inputlp              = marpaESLIFTester_contextp->inputl;
  *eofbp                = 1;
  *characterStreambp    = 1; /* We say this is a stream of characters */
  *encodingOfEncodingsp = NULL;
  *encodingsp           = NULL;
  *encodinglp           = 0;

  return 1;
}
