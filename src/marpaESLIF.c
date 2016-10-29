#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <genericLogger.h>
#include <marpaESLIF.h>
#include <marpaWrapper.h>
#include <genericStack.h>
#include <tconv.h>
#include <ctype.h>
#include "config.h"
#include "marpaESLIF/internal/structures.h"
#include "marpaESLIF/internal/logging.h"

#ifndef MARPAESLIF_INITIAL_REPLACEMENT_LENGTH
#define MARPAESLIF_INITIAL_REPLACEMENT_LENGTH 8096  /* Subjective number */
#endif

/* This is very internal: I use the genericLogger to generate strings */
typedef struct _marpaESLIF_stringGenerator {
  marpaESLIF_t *marpaESLIFp;
  char         *s;
  size_t        l;
  short         okb;
} _marpaESLIF_stringGenerator_t;

const static char *GENERICSTACKITEMTYPE_NA_STRING      = "NA";
const static char *GENERICSTACKITEMTYPE_CHAR_STRING    = "CHAR";
const static char *GENERICSTACKITEMTYPE_SHORT_STRING   = "SHORT";
const static char *GENERICSTACKITEMTYPE_INT_STRING     = "INT";
const static char *GENERICSTACKITEMTYPE_LONG_STRING    = "LONG";
const static char *GENERICSTACKITEMTYPE_FLOAT_STRING   = "FLOAT";
const static char *GENERICSTACKITEMTYPE_DOUBLE_STRING  = "DOUBLE";
const static char *GENERICSTACKITEMTYPE_PTR_STRING     = "PTR";
const static char *GENERICSTACKITEMTYPE_ARRAY_STRING   = "ARRAY";
const static char *GENERICSTACKITEMTYPE_UNKNOWN_STRING = "UNKNOWN";

const marpaESLIF_uint32_t pcre2_option_binary_default  = PCRE2_NOTEMPTY;
const marpaESLIF_uint32_t pcre2_option_char_default    = PCRE2_NOTEMPTY|PCRE2_NO_UTF_CHECK;
const marpaESLIF_uint32_t pcre2_option_partial_default = PCRE2_NOTEMPTY|PCRE2_PARTIAL_HARD;  /* No PCRE2_NO_UTF_CHECK c.f. regex_match to know why */

/* Please note that EVERY _marpaESLIFRecognizer_xxx() method is logging at start and at return */

static inline marpaESLIF_string_t   *_marpaESLIF_string_newp(marpaESLIF_t *marpaESLIFp, char *encodings, char *bytep, size_t bytel, short asciib);
static inline marpaESLIF_string_t   *_marpaESLIF_string_shallowp(marpaESLIF_t *marpaESLIFp, marpaESLIF_string_t *stringp);
static inline marpaESLIF_string_t   *_marpaESLIF_string_clonep(marpaESLIF_t *marpaESLIFp, marpaESLIF_string_t *stringp);
static inline void                   _marpaESLIF_string_freev(marpaESLIF_string_t *stringp);
static inline short                  _marpaESLIF_string_eqb(marpaESLIF_string_t *string1p, marpaESLIF_string_t *string2p);
static inline marpaESLIF_terminal_t *_marpaESLIF_terminal_newp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, int eventSeti, char *descEncodings, char *descs, size_t descl, marpaESLIF_terminal_type_t type, marpaESLIF_uint32_t opti, char *utf8s, size_t utf8l, char *testFullMatchs, char *testPartialMatchs, marpaESLIF_action_type_t actionType, genericStack_t *actionStackp, char *actionEncodings, char *actions, size_t actionl);
static inline void                   _marpaESLIF_terminal_freev(marpaESLIF_terminal_t *terminalp);

static inline marpaESLIF_meta_t     *_marpaESLIF_meta_newp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, int eventSeti, char *descs, size_t descl);
static inline void                   _marpaESLIF_meta_freev(marpaESLIF_meta_t *metap);

static inline marpaESLIF_grammar_t  *_marpaESLIF_grammar_newp(marpaESLIF_t *marpaESLIFp, marpaWrapperGrammarOption_t *marpaWrapperGrammarOptionp, int leveli, char *descs, size_t descl);
static inline void                   _marpaESLIF_grammar_freev(marpaESLIF_grammar_t *grammarp);

static inline void                   _marpaESLIF_ruleStack_freev(genericStack_t *ruleStackp);
static inline void                   _marpaESLIFrecognizer_lexemeStack_freev(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp);
static inline void                   _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp);

static inline void                   _marpaESLIF_grammarStack_freev(genericStack_t *grammarStackp);
static inline void                   _marpaESLIF_grammarStack_resetv(genericStack_t *grammarStackp);

static inline short                  _marpaESLIFRecognizer_lexemeStack_i_resetb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i);
static inline short                  _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, size_t *sizelp);
static inline short                  _marpaESLIFRecognizer_lexemeStack_i_p_and_sizeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, char **pp, size_t *sizelp);
static inline short                  _marpaESLIFRecognizer_lexemeStack_i_setarraypb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, GENERICSTACKITEMTYPE2TYPE_ARRAYP arrayp);
static inline short                  _marpaESLIFRecognizer_lexemeStack_i_setb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, void *p, size_t sizel);
static inline short                  _marpaESLIFRecognizer_lexemeStack_i_moveb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackDstp, int dsti, genericStack_t *lexemeStackSrcp, int srci);
static inline short                  _marpaESLIFRecognizer_lexemeStack_i_dupb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackDstp, int dsti, genericStack_t *lexemeStackSrcp, int srci);
static inline short                  _marpaESLIFRecognizer_lexemeStack_i_p(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, char **pp);
static inline const char            *_marpaESLIF_genericStack_i_types(genericStack_t *stackp, int i);

static inline marpaESLIF_rule_t     *_marpaESLIF_rule_newp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, char *descs, size_t descl, int lhsi, size_t nrhsl, int *rhsip, short *maskbp, size_t nexceptionl, int *exceptionip, int ranki, short nullRanksHighb, short sequenceb, int minimumi, int separatori, short properb);
static inline void                   _marpaESLIF_rule_freev(marpaESLIF_rule_t *rulep);

static inline marpaESLIF_symbol_t   *_marpaESLIF_symbol_newp(marpaESLIF_t *marpaESLIFp);
static inline void                   _marpaESLIF_symbol_freev(marpaESLIF_symbol_t *symbolp);

static inline void                   _marpaESLIF_symbolStack_freev(genericStack_t *symbolStackp);

static inline marpaESLIF_grammar_t  *_marpaESLIF_bootstrap_grammar_L0p(marpaESLIF_t *marpaESLIFp);
static inline marpaESLIF_grammar_t  *_marpaESLIF_bootstrap_grammar_G1p(marpaESLIF_t *marpaESLIFp);
static inline marpaESLIF_grammar_t  *_marpaESLIF_bootstrap_grammarp(marpaESLIF_t *marpaESLIFp,
                                                                    int leveli,
                                                                    char  *descs,
                                                                    size_t descl,
                                                                    short warningIsErrorb,
                                                                    short warningIsIgnoredb,
                                                                    short autorankb,
                                                                    int bootstrap_grammar_terminali, bootstrap_grammar_terminal_t *bootstrap_grammar_terminalp,
                                                                    int bootstrap_grammar_metai, bootstrap_grammar_meta_t *bootstrap_grammar_metap,
                                                                    int bootstrap_grammar_rulei, bootstrap_grammar_rule_t *bootstrap_grammar_rulep);
static inline short                  _marpaESLIFGrammar_validateb(marpaESLIFGrammar_t *marpaESLIFGrammar);

static inline short                  _marpaESLIFRecognizer_regex_matcherb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIF_terminal_t *terminalp, char *inputs, size_t inputl, short eofb, marpaESLIF_matcher_value_t *rcip, genericStack_t *outputStackp);
static inline short                  _marpaESLIFRecognizer_meta_matcherb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, int grammarLeveli, marpaWrapperGrammar_t *marpaWrapperGrammarp, marpaESLIF_terminal_t *terminalp, marpaESLIF_meta_t *metap, marpaESLIF_matcher_value_t *rcip, genericStack_t *outputStackp, short *exhaustedbp);
static inline short                  _marpaESLIFRecognizer_symbol_matcherb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIF_symbol_t *symbolp, marpaESLIF_matcher_value_t *rcip, genericStack_t *outputStackp, short *exhaustedbp);

const static  char                  *_marpaESLIF_utf82printableascii_defaultp = "<!NOT TRANSLATED!>";
#ifndef MARPAESLIF_NTRACE
static        void                   _marpaESLIF_tconvTraceCallback(void *userDatavp, const char *msgs);
#endif

static inline char                  *_marpaESLIF_charconvp(marpaESLIF_t *marpaESLIFp, char *toEncodings, char *fromEncodings, char *srcs, size_t srcl, size_t *dstlp, char **fromEncodingsp);

static inline char                  *_marpaESLIF_utf82printableascii_newp(marpaESLIF_t *marpaESLIFp, char *descs, size_t descl);
static inline void                   _marpaESLIF_utf82printableascii_freev(char *utf82printableasciip);
static        short                  _marpaESLIFReader_grammarReader(void *userDatavp, char **inputsp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingsp);
static inline short                  _marpaESLIFRecognizer_completeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp);
static inline short                  _marpaESLIFRecognizer_resumeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, short ignorePreviousEventsb, short *continuebp, short *exhaustedbp);
static inline marpaESLIF_symbol_t   *_marpaESLIFRecognizer_symbol_findp(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *descs, size_t descl);
static inline marpaESLIF_grammar_t  *_marpaESLIFGrammar_grammar_findp(marpaESLIFGrammar_t *marpaESLIFGrammarp, char *descs, size_t descl, int leveli);
static inline marpaESLIF_rule_t     *_marpaESLIF_rule_findp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, char *descs, size_t descl, int rulei);
static inline marpaESLIF_symbol_t   *_marpaESLIF_symbol_findp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, char *descs, size_t descl, int symboli);
static inline short                  _marpaESLIFRecognizer_alternativeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIF_symbol_t *symbolp, int valuei);
static inline void                   _marpaESLIF_freeCallbackv(void *userDatavp, void *p);
static inline void                   _marpaESLIFRecognizer_reset_eventsb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp);
static inline short                  _marpaESLIFRecognizer_grammar_eventsb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, short *completedbp, short *nulledbp, short *predictedbp, short *exhaustedbp);
static inline marpaESLIFRecognizer_t *_marpaESLIFRecognizer_newp(marpaESLIFGrammar_t *marpaESLIFGrammarp, marpaESLIFRecognizerOption_t *marpaESLIFRecognizerOptionp, short discardb, marpaESLIFRecognizer_t *marpaESLIFRecognizerParentp, short fakeb);
static inline short                  _marpaESLIFGrammar_parseb(marpaESLIFGrammar_t *marpaESLIFGrammarp, marpaESLIFRecognizerOption_t *marpaESLIFRecognizerOptionp, marpaESLIFValueOption_t *marpaESLIFValueOptionp, short discardb, marpaESLIFRecognizer_t *marpaESLIFRecognizerParentp, short *exhaustedbp, short internalb);
static        void                   _marpaESLIF_generateStringWithLoggerCallback(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs);
static inline void                   _marpaESLIFRecognizer_hexdumpv(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *headers, char *asciidescs, const char *p, size_t lengthl, short traceb);
static inline short                  _marpaESLIFRecognizer_readb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp);

static        short                  _marpaESLIFValueRuleCallbackWrapper(void *userDatavp, int rulei, int arg0i, int argni, int resulti);
static        short                  _marpaESLIFValueSymbolCallbackWrapper(void *userDatavp, int symboli, int argi, int resulti);
static        short                  _marpaESLIFValueNullingCallbackWrapper(void *userDatavp, int symboli, int resulti);

static        short                  _marpaESLIFValueRuleCallbackInternalWrapper(void *userDatavp, int rulei, int arg0i, int argni, int resulti);
static        short                  _marpaESLIFValueSymbolCallbackInternalWrapper(void *userDatavp, int symboli, int argi, int resulti);
static        short                  _marpaESLIFValueNullingCallbackInternalWrapper(void *userDatavp, int symboli, int resulti);

static        short                  _marpaESLIFValueRuleCallbackInternalLexeme(void *userDatavp, int rulei, int arg0i, int argni, int resulti);
static        short                  _marpaESLIFValueSymbolCallbackInternalLexeme(void *userDatavp, genericStack_t *lexemeStackp, int symboli, int argi, int resulti);
static        short                  _marpaESLIFValueNullingCallbackInternalLexeme(void *userDatavp, int symboli, int resulti);

static        short                  _marpaESLIFValueRuleCallbackInternalGrammar(void *userDatavp, int rulei, int arg0i, int argni, int resulti);
static        short                  _marpaESLIFValueSymbolCallbackInternalGrammar(void *userDatavp, genericStack_t *lexemeStackp, int symboli, int argi, int resulti);
static        short                  _marpaESLIFValueNullingCallbackInternalGrammar(void *userDatavp, int symboli, int resulti);

static inline void                   _marpaESLIFGrammar_freev(marpaESLIFGrammar_t *marpaESLIFGrammarp, short onStackb);
static        char                  *_marpaESLIFGrammar_symbolDescriptionCallback(void *userDatavp, int symboli);
static        short                  _marpaESLIFGrammar_symbolOptionSetterOnlyStartCompletionEvent(void *userDatavp, int symboli, marpaWrapperGrammarSymbolOption_t *marpaWrapperGrammarSymbolOptionp);
static inline void                   _marpaESLIF_rule_createshowv(marpaESLIF_t *marpaESLIFp, marpaESLIF_rule_t *rulep, char *asciishows, size_t *asciishowlp);
static inline int                    _marpaESLIF_utf82ordi(PCRE2_SPTR8 utf8bytes, marpaESLIF_uint32_t *uint32p);
static inline short                  _marpaESLIFRecognizer_matchPostProcessingb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, size_t matchl);
static inline marpaESLIF_action_t   *_marpaESLIF_action_newp(marpaESLIF_t *marpaESLIFp, marpaESLIF_action_type_t type, genericStack_t *actionStackp, char *encodings, char *bytep, size_t bytel);
static inline void                   _marpaESLIF_action_freev(marpaESLIF_action_t *actionp);
static inline short                  _marpaESLIFValue_valueb(marpaESLIFValue_t *marpaESLIFValuep, short internalb);

/*****************************************************************************/
static inline marpaESLIF_string_t *_marpaESLIF_string_newp(marpaESLIF_t *marpaESLIFp, char *encodings, char *bytep, size_t bytel, short asciib)
/*****************************************************************************/
{
  const static char   *funcs = "_marpaESLIF_string_newp";
  marpaESLIF_string_t *stringp = NULL;
  char                *dstbytep;
  char                *dstasciis;

  if ((bytep == NULL) || (bytel <= 0)) {
    errno = EINVAL;
    MARPAESLIF_ERROR(marpaESLIFp, "Invalid input");
    goto err;
  }

  stringp = (marpaESLIF_string_t *) malloc(sizeof(marpaESLIF_string_t));
  if (stringp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }
  stringp->bytep     = NULL;
  stringp->bytel     = bytel;
  stringp->encodings = NULL;
  stringp->asciis    = NULL;
  stringp->shallowb  = 0;
  stringp->stringp   = 0;

  stringp->bytep = dstbytep = (char *) malloc(bytel);
  if (dstbytep == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }
  memcpy(dstbytep, bytep, bytel);

  if (encodings != NULL) {
    stringp->encodings = strdup(encodings);
    if (stringp->encodings == NULL) {
      MARPAESLIF_ERRORF(marpaESLIFp, "strdup failure, %s", strerror(errno));
      goto err;
    }
  }

  if (asciib) {
    stringp->asciis = dstasciis = _marpaESLIF_charconvp(marpaESLIFp, "ASCII//TRANSLIT//IGNORE", encodings, bytep, bytel, NULL, &(stringp->encodings));
    if (dstasciis == NULL) {
      goto err;
    }
  }

  goto done;

 err:
  _marpaESLIF_string_freev(stringp);
  stringp = NULL;

 done:
  return stringp;
}

/*****************************************************************************/
static inline marpaESLIF_string_t *_marpaESLIF_string_shallowp(marpaESLIF_t *marpaESLIFp, marpaESLIF_string_t *stringp)
/*****************************************************************************/
{
  marpaESLIF_string_t *rcp = NULL;
  
  if (stringp == NULL) {
    goto err;
  }

  rcp = (marpaESLIF_string_t *) malloc(sizeof(marpaESLIF_string_t));
  if (rcp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }
  memcpy(rcp, stringp, sizeof(marpaESLIF_string_t));
  rcp->shallowb = 1;

  goto done;

 err:
  _marpaESLIF_string_freev(rcp);
  rcp = NULL;

 done:
  return rcp;
}

/*****************************************************************************/
static inline marpaESLIF_string_t *_marpaESLIF_string_clonep(marpaESLIF_t *marpaESLIFp, marpaESLIF_string_t *stringp)
/*****************************************************************************/
{
  marpaESLIF_string_t *rcp = NULL;
  char                *bytep;
  size_t               bytel;
  char                *asciis;
  char                *encodings;
  
  if (stringp == NULL) {
    goto err;
  }

  rcp = (marpaESLIF_string_t *) malloc(sizeof(marpaESLIF_string_t));
  if (rcp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }
  rcp->bytep     = NULL;
  rcp->bytel     = 0;
  rcp->encodings = NULL;
  rcp->asciis    = NULL;
  rcp->shallowb  = 0;
  rcp->stringp   = 0;

  bytep = rcp->bytep = (char *) malloc((rcp->bytel = bytel = stringp->bytel));
  if (bytep == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }
  memcpy(bytep, stringp->bytep, bytel);

  if (stringp->asciis != NULL) {
    rcp->asciis = asciis = strdup(stringp->asciis);
    if (asciis == NULL) {
      goto err;
    }
  }

  if (stringp->encodings != NULL) {
    rcp->encodings = encodings = strdup(stringp->encodings);
    if (encodings == NULL) {
      goto err;
    }
  }

  goto done;

 err:
  _marpaESLIF_string_freev(rcp);
  rcp = NULL;

 done:
  return rcp;
}

/*****************************************************************************/
static inline void _marpaESLIF_string_freev(marpaESLIF_string_t *stringp)
/*****************************************************************************/
{
  if (stringp != NULL) {
    if (! stringp->shallowb) {
      if (stringp->bytep != NULL) {
        free(stringp->bytep);
      }
      if (stringp->encodings != NULL) {
        free(stringp->encodings);
      }
      if (stringp->asciis != NULL) {
        free(stringp->asciis);
      }
    }
    free(stringp);
  }
}

/*****************************************************************************/
static inline short _marpaESLIF_string_eqb(marpaESLIF_string_t *string1p, marpaESLIF_string_t *string2p)
/*****************************************************************************/
{
  char  *byte1p;
  char  *byte2p;
  size_t bytel;
  
  if ((string1p == NULL) || (string2p == NULL)) {
    return 0;
  }
  if (((byte1p = string1p->bytep) == NULL) || ((byte2p = string2p->bytep) == NULL)) {
    return 0;
  }
  if ((bytel = string1p->bytel) != string2p->bytel) {
    return 0;
  }
  return (memcmp(byte1p, byte2p, bytel) == 0) ? 1 : 0;
}

/*****************************************************************************/
static inline marpaESLIF_terminal_t *_marpaESLIF_terminal_newp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, int eventSeti, char *descEncodings, char *descs, size_t descl, marpaESLIF_terminal_type_t type, marpaESLIF_uint32_t opti, char *utf8s, size_t utf8l, char *testFullMatchs, char *testPartialMatchs, marpaESLIF_action_type_t actionType, genericStack_t *actionStackp, char *actionEncodings, char *actions, size_t actionl)
/*****************************************************************************/
/* This method is bootstraped at marpaESLIFp creation itself to have the internal regexps, with grammarp being NULL... */
/*****************************************************************************/
{
  const static char                *funcs = "_marpaESLIF_terminal_newp";
  genericStack_t                    localOutputStack;
  genericStack_t                   *localOutputStackp     = NULL;
  char                             *strings               = NULL;
  marpaESLIFRecognizer_t           *marpaESLIFRecognizerp = NULL;
  marpaESLIF_terminal_t            *terminalp;
  marpaWrapperGrammarSymbolOption_t marpaWrapperGrammarSymbolOption;
  marpaESLIF_uint32_t               pcre2Optioni;
  int                               pcre2Errornumberi;
  PCRE2_SIZE                        pcre2ErrorOffsetl;
  PCRE2_UCHAR                       pcre2ErrorBuffer[256];
  int                               i;
  marpaESLIFGrammar_t               marpaESLIFGrammar;
  char                             *inputs;
  size_t                            inputl;
  marpaESLIF_matcher_value_t        rci;
  marpaESLIF_uint32_t               codepointi;
  short                             utfflagb;
  size_t                            stringl;
  char                             *tmps;
  size_t                            hexdigitl;
  int                               utf82ordi;
  size_t                            matchedl;
  char                             *matchedp;

  /* Please note the "fakeb" parameter below */

  terminalp = (marpaESLIF_terminal_t *) malloc(sizeof(marpaESLIF_terminal_t));
  if (terminalp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  terminalp->idi                 = -1;
  terminalp->descp               = NULL;
  terminalp->regex.patternp      = NULL;
  terminalp->regex.match_datap   = NULL;
#ifdef PCRE2_CONFIG_JIT
  terminalp->regex.jitCompleteb  = 0;
  terminalp->regex.jitPartialb   = 0;
#endif

  /* ----------- Terminal Identifier ------------ */
  if (grammarp != NULL) { /* Here is the bootstrap dependency with grammarp == NULL */
    marpaWrapperGrammarSymbolOption.terminalb = 1;
    marpaWrapperGrammarSymbolOption.startb    = 0;
    marpaWrapperGrammarSymbolOption.eventSeti = eventSeti;
    terminalp->idi = marpaWrapperGrammar_newSymboli(grammarp->marpaWrapperGrammarStartp, &marpaWrapperGrammarSymbolOption);
    if (terminalp->idi < 0) {
      goto err;
    }
  }

  /* ----------- Terminal Description ------------ */
  terminalp->descp = _marpaESLIF_string_newp(marpaESLIFp, descEncodings, descs, descl, 1);
  if (terminalp->descp == NULL) {
    goto err;
  }

  /* ----------- Terminal Implementation ------------ */
  switch (type) {

  case MARPAESLIF_TERMINAL_TYPE_STRING:
    /* We convert a string terminal into a regexp */
    /* By construction we are coming from the parsing of a grammar, that previously translated the whole */
    /* grammar into an UTF-8 string. We use PCRE2 to extract all code points, and create a new string that */
    /* is a concatenation of \x{} thingies. By doing so, btw, we are able to know if we need PCRE2_UTF flag. */

    marpaESLIFGrammar.marpaESLIFp = marpaESLIFp;
    inputs = utf8s;
    inputl = utf8l;
    localOutputStackp = &localOutputStack;

    /* Fake a recognizer. EOF flag will be set automatically in fake mode */
    marpaESLIFRecognizerp = _marpaESLIFRecognizer_newp(&marpaESLIFGrammar, NULL /* marpaESLIFRecognizerOptionp */, 0 /* discardb */, NULL /* marpaESLIFRecognizerParentp */, 1 /* fakeb */);
    if (marpaESLIFRecognizerp == NULL) {
      goto err;
    }

    GENERICSTACK_INIT(localOutputStackp);
    if (GENERICSTACK_ERROR(localOutputStackp)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "localOutputStackp initialization failure, %s", strerror(errno));
      localOutputStackp = NULL;
      goto err;
    }

    while (inputl > 0) {
      if (! _marpaESLIFRecognizer_regex_matcherb(marpaESLIFRecognizerp, marpaESLIFp->anycharp, inputs, inputl, 1 /* eofb */, &rci, localOutputStackp)) {
        goto err;
      }
      if (rci != MARPAESLIF_MATCH_OK) {
        MARPAESLIF_ERROR(marpaESLIFp, "Failed to detect all characters of terminal string");
        goto err;
      }
      if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, localOutputStackp, 0, &matchedl)) {
        goto err;
      }
      if (matchedl <= 0) {
        MARPAESLIF_ERROR(marpaESLIFp, "anychar regex matched zero byte");
        goto err;
      }
      inputs += matchedl;
      inputl -= matchedl;
    }
    /* All matches are in localOutputStackp, in order. Take all unicode code points to generate a regex out of this string. */
    utfflagb = 0;
    stringl = 0;
    for (i = 0; i < GENERICSTACK_USED(localOutputStackp); i++) {
      if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, localOutputStackp, 0, &matchedl)) {
        goto err;
      }
      if (! _marpaESLIFRecognizer_lexemeStack_i_p(marpaESLIFRecognizerp, localOutputStackp, i, &matchedp)) {
        goto err;
      }
      /* Get the code point from the UTF-8 representation */
      utf82ordi = _marpaESLIF_utf82ordi((PCRE2_SPTR8) matchedp, &codepointi);
      if (utf82ordi <= 0) {
        MARPAESLIF_ERRORF(marpaESLIFp, "Malformed UTF-8 character at offset %d", -utf82ordi);
        goto err;
      } else if (utf82ordi != (int) matchedl) {
        MARPAESLIF_ERRORF(marpaESLIFp, "Not all bytes consumed: %d instead of %ld", utf82ordi, (unsigned long) matchedl);
        goto err;
      }
      /* Determine the number of hex digits to fully represent the code point, remembering if we need PCRE2_UTF flag */
      hexdigitl = 4; /* \x{} */
      if ((codepointi & 0xFF000000) != 0x00000000) {
        hexdigitl += 8;
        utfflagb = 1;
      } else if ((codepointi & 0x00FF0000) != 0x00000000) {
        hexdigitl += 6;
        utfflagb = 1;
      } else if ((codepointi & 0x0000FF00) != 0x00000000) {
        hexdigitl += 4;
        utfflagb = 1;
      } else {
        hexdigitl += 2;
      }
      /* Append the ASCII representation */
      stringl += hexdigitl;
      if (strings == NULL) {
        strings = (char *) malloc(stringl + 1);
        if (strings == NULL) {
          MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
          goto err;
        }
        strings[0] = '\0'; /* Start with an empty string */
      } else {
        tmps = (char *) realloc(strings, stringl + 1);
        if (tmps == NULL) {
          MARPAESLIF_ERRORF(marpaESLIFp, "realloc failure, %s", strerror(errno));
          goto err;
        }
        strings = tmps;
      }
      strings[stringl] = '\0'; /* Make sure the string always end with NUL */
      hexdigitl -= 4; /* \x{} */
      sprintf(strings + strlen(strings), "\\x{%0*x}", (int) hexdigitl, codepointi);
    }
    /* Done - now we can generate a regexp out of that UTF-8 compatible string */
    MARPAESLIF_TRACEF(marpaESLIFp, funcs, "%s: content string converted to regex %s (UTF=%d)", terminalp->descp->asciis, strings, utfflagb);
    utf8s = strings;
    utf8l = stringl;
    /* opti for string is compatible with opti for regex - just that the lexer accept less options - in particular the UTF flag */
    if (utfflagb) {
      opti |= MARPAESLIF_REGEX_OPTION_UTF;
    }
    marpaESLIFRecognizer_freev(marpaESLIFRecognizerp);
    marpaESLIFRecognizerp = NULL;

    /* ********************************************************************************************************** */
    /*                                   THERE IS NO BREAK INTENTIONALY HERE                                      */
    /* ********************************************************************************************************** */
    /* break; */

  case MARPAESLIF_TERMINAL_TYPE_REGEX:
    
    if ((utf8s == NULL) || (utf8l <= 0)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "%s: Invalid terminal origin", terminalp->descp->asciis);
      goto err;
    }
    pcre2Optioni = PCRE2_ANCHORED;      /* By default patterns are always anchored and only that */
    for (i = 0; i < _MARPAESLIF_REGEX_OPTION_ID_MAX; i++) {
      if ((opti & marpaESLIF_regex_option_map[i].opti) == marpaESLIF_regex_option_map[i].opti) {
	/* It is important to process pcre2OptionNoti first */
	if (marpaESLIF_regex_option_map[i].pcre2OptionNoti != 0) {
          MARPAESLIF_TRACEF(marpaESLIFp, funcs, "%s: regex modifier %s: removing %s", terminalp->descp->asciis, marpaESLIF_regex_option_map[i].modifiers, marpaESLIF_regex_option_map[i].pcre2OptionNots);
	  pcre2Optioni &= ~marpaESLIF_regex_option_map[i].pcre2OptionNoti;
	}
	if (marpaESLIF_regex_option_map[i].pcre2Optioni != 0) {
          MARPAESLIF_TRACEF(marpaESLIFp, funcs, "%s: regex modifier %s: adding %s", terminalp->descp->asciis, marpaESLIF_regex_option_map[i].modifiers, marpaESLIF_regex_option_map[i].pcre2Options);
	  pcre2Optioni |= marpaESLIF_regex_option_map[i].pcre2Optioni;
	}
      }
    }
    terminalp->regex.patternp = pcre2_compile(
                                              (PCRE2_SPTR) utf8s,      /* An UTF-8 pattern */
                                              (PCRE2_SIZE) utf8l,      /* In code units (!= code points) - in UTF-8 a code unit is a byte */
                                              pcre2Optioni,
                                              &pcre2Errornumberi, /* for error number */
                                              &pcre2ErrorOffsetl, /* for error offset */
                                              NULL);        /* use default compile context */
    if (terminalp->regex.patternp == NULL) {
      pcre2_get_error_message(pcre2Errornumberi, pcre2ErrorBuffer, sizeof(pcre2ErrorBuffer));
      MARPAESLIF_ERRORF(marpaESLIFp, "%s: pcre2_compile failure at offset %ld: %s", terminalp->descp->asciis, (unsigned long) pcre2ErrorOffsetl, pcre2ErrorBuffer);
      goto err;
    }
    terminalp->regex.match_datap = pcre2_match_data_create(1 /* We are interested in the string that matched the full pattern */,
                                                             NULL /* Default memory allocation */);
    if (terminalp->regex.match_datap == NULL) {
      MARPAESLIF_ERRORF(marpaESLIFp, "%s: pcre2_match_data_create_from_pattern failure, %s", terminalp->descp->asciis, strerror(errno));
      goto err;
    }
    /* Determine if we can do JIT */
#ifdef PCRE2_CONFIG_JIT
    if ((pcre2_config(PCRE2_CONFIG_JIT, &pcre2Optioni) >= 0) && (pcre2Optioni == 1)) {
#ifdef PCRE2_JIT_COMPLETE
      terminalp->regex.jitCompleteb = (pcre2_jit_compile(terminalp->regex.patternp, PCRE2_JIT_COMPLETE) == 0) ? 1 : 0;
#else
      terminalp->regex.jitCompleteb = 0;
#endif
#ifdef PCRE2_JIT_PARTIAL_HARD
      terminalp->regex.jitPartialb = (pcre2_jit_compile(terminalp->regex.patternp, PCRE2_JIT_PARTIAL_HARD) == 0) ? 1 : 0;
#else
      terminalp->regex.jitPartialb = 0;
#endif /*  PCRE2_CONFIG_JIT */
    } else {
      terminalp->regex.jitCompleteb = 0;
      terminalp->regex.jitPartialb = 0;
    }
#endif /*  PCRE2_CONFIG_JIT */
    /* And in the UTF mode is on */
    pcre2Errornumberi = pcre2_pattern_info(terminalp->regex.patternp, PCRE2_INFO_ALLOPTIONS, &pcre2Optioni);
    if (pcre2Errornumberi != 0) {
      pcre2_get_error_message(pcre2Errornumberi, pcre2ErrorBuffer, sizeof(pcre2ErrorBuffer));
      MARPAESLIF_ERRORF(marpaESLIFp, "%s: pcre2_pattern_info failure: %s", terminalp->descp->asciis, pcre2ErrorBuffer);
      goto err;
    }
    terminalp->regex.utfb = ((pcre2Optioni & PCRE2_UTF) == PCRE2_UTF);
    MARPAESLIF_TRACEF(marpaESLIFp, funcs, "%s: UTF mode is %s", terminalp->descp->asciis, terminalp->regex.utfb ? "on" : "off");
    break;

  default:
    MARPAESLIF_ERRORF(marpaESLIFp, "%s: Unsupported terminal type %d", terminalp->descp->asciis, type);
    goto err;
    break;
  }

#ifndef MARPAESLIF_NTRACE
  {
    marpaESLIFGrammar_t marpaESLIFGrammar;

    marpaESLIFGrammar.marpaESLIFp   = marpaESLIFp;
    marpaESLIFGrammar.grammarStackp = NULL;
    marpaESLIFGrammar.grammarp      = grammarp;

    /* Fake a recognizer. EOF flag will be set automatically in fake mode */
    marpaESLIFRecognizerp = _marpaESLIFRecognizer_newp(&marpaESLIFGrammar, NULL /* marpaESLIFRecognizerOptionp */, 0 /* discardb */, NULL /* marpaESLIFRecognizerParentp */, 1 /* fakeb */);
    if (marpaESLIFRecognizerp == NULL) {
      goto err;
    }

    /* Look to the implementations of terminal matchers: they NEVER use leveli, nor marpaWrapperGrammarp, nor grammarStackp -; */
    /* Also, note that we always end up with a regex. */
    
    if (testFullMatchs != NULL) {

      if (! _marpaESLIFRecognizer_regex_matcherb(marpaESLIFRecognizerp, terminalp, testFullMatchs, strlen(testFullMatchs), 1, &rci, NULL)) {
        MARPAESLIF_ERRORF(marpaESLIFp, "%s: testing full match: matcher general failure", terminalp->descp->asciis);
        goto err;
      }
      if (rci != MARPAESLIF_MATCH_OK) {
        MARPAESLIF_ERRORF(marpaESLIFp, "%s: testing full match: matcher returned rci = %d", terminalp->descp->asciis, rci);
        goto err;
      }
      /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "%s: testing full match is successful on %s", terminalp->descp->asciis, testFullMatchs); */
    }

    if (testPartialMatchs != NULL) {

      if (! _marpaESLIFRecognizer_regex_matcherb(marpaESLIFRecognizerp, terminalp, testPartialMatchs, strlen(testPartialMatchs), 0, &rci, NULL)) {
        MARPAESLIF_ERRORF(marpaESLIFp, "%s: testing partial match: matcher general failure", terminalp->descp->asciis);
        goto err;
      }
      if (rci != MARPAESLIF_MATCH_AGAIN) {
        MARPAESLIF_ERRORF(marpaESLIFp, "%s: testing partial match: matcher returned rci = %d", terminalp->descp->asciis, rci);
        goto err;
      }
      /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "%s: testing partial match is successful on %s when not at EOF", terminalp->descp->asciis, testPartialMatchs); */
    }
  }

#endif

  goto done;
  
 err:
  _marpaESLIF_terminal_freev(terminalp);
  terminalp = NULL;

 done:
  if (strings != NULL) {
    free(strings);
  }
  _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizerp, localOutputStackp);
  marpaESLIFRecognizer_freev(marpaESLIFRecognizerp);
  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", terminalp); */
  return terminalp;
}

/*****************************************************************************/
static inline marpaESLIF_meta_t *_marpaESLIF_meta_newp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, int eventSeti, char *descs, size_t descl)
/*****************************************************************************/
{
  const static char                *funcs = "_marpaESLIF_meta_newp";
  marpaESLIF_meta_t                *metap;
  marpaWrapperGrammarSymbolOption_t marpaWrapperGrammarSymbolOption;

  /* MARPAESLIF_TRACE(marpaESLIFp, funcs, "Building meta"); */

  metap = (marpaESLIF_meta_t *) malloc(sizeof(marpaESLIF_meta_t));
  if (metap == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  metap->idi                       = -1;
  metap->descp                     = NULL;
  metap->marpaWrapperGrammarClonep = NULL; /* Eventually changed when validating the grammar */

  marpaWrapperGrammarSymbolOption.terminalb = 0;
  marpaWrapperGrammarSymbolOption.startb    = 0;
  marpaWrapperGrammarSymbolOption.eventSeti = eventSeti;

  /* ----------- Meta Identifier ------------ */
  metap->idi = marpaWrapperGrammar_newSymboli(grammarp->marpaWrapperGrammarStartp, &marpaWrapperGrammarSymbolOption);
  if (metap->idi < 0) {
    goto err;
  }

  /* -------- Meta Description -------- */
  metap->descp = _marpaESLIF_string_newp(marpaESLIFp, NULL, descs, descl, 1);
  if (metap->descp == NULL) {
    goto err;
  }

  goto done;

 err:
  _marpaESLIF_meta_freev(metap);
  metap = NULL;

 done:
  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", metap); */
  return metap;
}

/*****************************************************************************/
static inline void _marpaESLIF_meta_freev(marpaESLIF_meta_t *metap)
/*****************************************************************************/
{
  if (metap != NULL) {
    _marpaESLIF_string_freev(metap->descp);
    if (metap->marpaWrapperGrammarClonep != NULL) {
      marpaWrapperGrammar_freev(metap->marpaWrapperGrammarClonep);
    }
    /* All the rest are shallow pointers */
    free(metap);
  }
}

/*****************************************************************************/
static inline marpaESLIF_grammar_t *_marpaESLIF_bootstrap_grammar_L0p(marpaESLIF_t *marpaESLIFp)
/*****************************************************************************/
{
  return _marpaESLIF_bootstrap_grammarp(marpaESLIFp,
					1, /* L0 in Marpa::R2 terminology is level No 1 for us */
                                        "L0",
                                        strlen("L0"),
					0, /* warningIsErrorb */
					1, /* warningIsIgnoredb */
					0, /* autorankb */
					sizeof(bootstrap_grammar_L0_terminals) / sizeof(bootstrap_grammar_L0_terminals[0]),
					bootstrap_grammar_L0_terminals,
					sizeof(bootstrap_grammar_L0_metas) / sizeof(bootstrap_grammar_L0_metas[0]),
					bootstrap_grammar_L0_metas,
					sizeof(bootstrap_grammar_L0_rules) / sizeof(bootstrap_grammar_L0_rules[0]),
					bootstrap_grammar_L0_rules);
}

/*****************************************************************************/
static inline marpaESLIF_grammar_t *_marpaESLIF_bootstrap_grammar_G1p(marpaESLIF_t *marpaESLIFp)
/*****************************************************************************/
{
  return _marpaESLIF_bootstrap_grammarp(marpaESLIFp,
					0, /* G1 in Marpa::R2 terminology is level No 0 for us */
                                        "G1",
                                        strlen("G1"),
					0, /* warningIsErrorb */
					1, /* warningIsIgnoredb */
					0, /* autorankb */
					sizeof(bootstrap_grammar_G1_terminals) / sizeof(bootstrap_grammar_G1_terminals[0]),
					bootstrap_grammar_G1_terminals,
					sizeof(bootstrap_grammar_G1_metas) / sizeof(bootstrap_grammar_G1_metas[0]),
					bootstrap_grammar_G1_metas,
					sizeof(bootstrap_grammar_G1_rules) / sizeof(bootstrap_grammar_G1_rules[0]),
					bootstrap_grammar_G1_rules);
}

/*****************************************************************************/
static inline marpaESLIF_grammar_t *_marpaESLIF_bootstrap_grammarp(marpaESLIF_t *marpaESLIFp,
								   int leveli,
                                                                   char  *descs,
                                                                   size_t descl,
								   short warningIsErrorb,
								   short warningIsIgnoredb,
								   short autorankb,
								   int bootstrap_grammar_terminali, bootstrap_grammar_terminal_t *bootstrap_grammar_terminalp,
								   int bootstrap_grammar_metai, bootstrap_grammar_meta_t *bootstrap_grammar_metap,
								   int bootstrap_grammar_rulei, bootstrap_grammar_rule_t *bootstrap_grammar_rulep)
/*****************************************************************************/
{
  const static char          *funcs        = "_marpaESLIF_bootstrap_grammarp";
  marpaESLIF_symbol_t        *symbolp      = NULL;
  marpaESLIF_rule_t          *rulep        = NULL;
  marpaESLIF_terminal_t      *terminalp    = NULL;
  marpaESLIF_meta_t          *metap        = NULL;
  marpaESLIF_grammar_t       *grammarp;
  marpaWrapperGrammarOption_t marpaWrapperGrammarOption;
  int                         i;

  MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Bootstrapping grammar at level %d", (int) leveli);

  marpaWrapperGrammarOption.genericLoggerp    = marpaESLIFp->marpaESLIFOption.genericLoggerp;
  marpaWrapperGrammarOption.warningIsErrorb   = warningIsErrorb;
  marpaWrapperGrammarOption.warningIsIgnoredb = warningIsIgnoredb;
  marpaWrapperGrammarOption.autorankb         = autorankb;
  marpaWrapperGrammarOption.exhaustionEventb  = 1;
  
  grammarp = _marpaESLIF_grammar_newp(marpaESLIFp, &marpaWrapperGrammarOption, leveli, descs, descl);
  if (grammarp == NULL) {
    goto err;
  }

  /* First the terminals */
  for (i = 0; i < bootstrap_grammar_terminali; i++) {
    symbolp = _marpaESLIF_symbol_newp(marpaESLIFp);
    if (symbolp == NULL) {
      goto err;
    }

    terminalp = _marpaESLIF_terminal_newp(marpaESLIFp,
					  grammarp,
					  MARPAWRAPPERGRAMMAR_EVENTTYPE_NONE,
                                          NULL, /* descEncodings */
					  bootstrap_grammar_terminalp[i].descs,
					  strlen(bootstrap_grammar_terminalp[i].descs),
					  bootstrap_grammar_terminalp[i].terminalType,
					  bootstrap_grammar_terminalp[i].optioni,
					  bootstrap_grammar_terminalp[i].utf8s,
					  (bootstrap_grammar_terminalp[i].utf8s != NULL) ? strlen(bootstrap_grammar_terminalp[i].utf8s) : 0,
					  bootstrap_grammar_terminalp[i].testFullMatchs,
					  bootstrap_grammar_terminalp[i].testPartialMatchs,
                                          MARPAESLIF_ACTION_TYPE_NAME,
                                          NULL, /* actionStackp */
                                          NULL, /* actionEncodings */
					  bootstrap_grammar_terminalp[i].actions,
					  (bootstrap_grammar_terminalp[i].actions != NULL) ? strlen(bootstrap_grammar_terminalp[i].actions) : 0
					  );
    if (terminalp == NULL) {
      goto err;
    }
    /* When bootstrapping the grammar, we expect terminal IDs to be exactly the value of the enum */
    if (terminalp->idi != bootstrap_grammar_terminalp[i].idi) {
      MARPAESLIF_ERRORF(marpaESLIFp, "Got symbol ID %d from Marpa while we were expecting %d", terminalp->idi, bootstrap_grammar_terminalp[i].idi);
      goto err;
    }

    symbolp->type        = MARPAESLIF_SYMBOL_TYPE_TERMINAL;
    symbolp->u.terminalp = terminalp;
    symbolp->idi         = terminalp->idi;
    symbolp->descp       = terminalp->descp;
    /* Terminal is now in symbol */
    terminalp = NULL;

    GENERICSTACK_SET_PTR(grammarp->symbolStackp, symbolp, symbolp->u.terminalp->idi);
    if (GENERICSTACK_ERROR(grammarp->symbolStackp)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "symbolStackp push failure, %s", strerror(errno));
      goto err;
    }
    /* Push is ok: symbolp is in grammarp->symbolStackp */
    symbolp = NULL;
  }

  /* Then the non-terminals */
  for (i = 0; i < bootstrap_grammar_metai; i++) {
    symbolp = _marpaESLIF_symbol_newp(marpaESLIFp);
    if (symbolp == NULL) {
      goto err;
    }
    metap = _marpaESLIF_meta_newp(marpaESLIFp,
				  grammarp,
				  MARPAWRAPPERGRAMMAR_EVENTTYPE_NONE,
				  bootstrap_grammar_metap[i].descs,
				  strlen(bootstrap_grammar_metap[i].descs)
				  );
    if (metap == NULL) {
      goto err;
    }
    /* When bootstrapping the grammar, we expect meta IDs to be exactly the value of the enum */
    if (metap->idi != bootstrap_grammar_metap[i].idi) {
      MARPAESLIF_ERRORF(marpaESLIFp, "Got symbol ID %d from Marpa while we were expecting %d", metap->idi, bootstrap_grammar_metap[i].idi);
      goto err;
    }

    symbolp->type       = MARPAESLIF_SYMBOL_TYPE_META;
    symbolp->startb     = bootstrap_grammar_metap[i].startb;
    symbolp->discardb   = bootstrap_grammar_metap[i].discardb;
    symbolp->u.metap    = metap;
    symbolp->idi        = metap->idi;
    symbolp->descp      = metap->descp;
    /* Terminal is now in symbol */
    metap = NULL;

    GENERICSTACK_SET_PTR(grammarp->symbolStackp, symbolp, symbolp->u.metap->idi);
    if (GENERICSTACK_ERROR(grammarp->symbolStackp)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "symbolStackp set failure, %s", strerror(errno));
      goto err;
    }
    /* Push is ok: symbolp is in grammarp->symbolStackp */
    symbolp = NULL;
  }

  /* Then the rules */
  for (i = 0; i < bootstrap_grammar_rulei; i++) {
    rulep = _marpaESLIF_rule_newp(marpaESLIFp,
				  grammarp,
				  bootstrap_grammar_rulep[i].descs,
				  strlen(bootstrap_grammar_rulep[i].descs),
				  bootstrap_grammar_rulep[i].lhsi,
				  bootstrap_grammar_rulep[i].nrhsl,
				  bootstrap_grammar_rulep[i].rhsip,
				  bootstrap_grammar_rulep[i].maskbp,
				  0, /* nexceptionl */
				  NULL, /* exceptionip */
				  0, /* ranki */
				  0, /* nullRanksHighb */
				  (bootstrap_grammar_rulep[i].type == MARPAESLIF_RULE_TYPE_ALTERNATIVE) ? 0 : 1, /* sequenceb */
				  bootstrap_grammar_rulep[i].minimumi,
				  bootstrap_grammar_rulep[i].separatori,
				  bootstrap_grammar_rulep[i].properb
				  );
    if (rulep == NULL) {
      goto err;
    }
    GENERICSTACK_SET_PTR(grammarp->ruleStackp, rulep, rulep->idi);
    if (GENERICSTACK_ERROR(grammarp->ruleStackp)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "ruleStackp set failure, %s", strerror(errno));
      goto err;
    }
    /* Push is ok: rulep is in grammarp->ruleStackp */
    rulep = NULL;
  }

  goto done;
  
 err:
  _marpaESLIF_terminal_freev(terminalp);
  _marpaESLIF_meta_freev(metap);
  _marpaESLIF_rule_freev(rulep);
  _marpaESLIF_symbol_freev(symbolp);
  _marpaESLIF_grammar_freev(grammarp);
  grammarp = NULL;

 done:
  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", grammarp); */
  return grammarp;
}

/*****************************************************************************/
static inline short _marpaESLIFGrammar_validateb(marpaESLIFGrammar_t *marpaESLIFGrammarp)
/*****************************************************************************/
{
  const static char     *funcs                     = "_marpaESLIFGrammar_validateb";
  marpaESLIF_t          *marpaESLIFp               = marpaESLIFGrammarp->marpaESLIFp;
  genericStack_t        *grammarStackp             = marpaESLIFGrammarp->grammarStackp;
  marpaWrapperGrammar_t *marpaWrapperGrammarClonep = NULL;
  marpaESLIF_meta_t     *metap;
  genericStack_t        *symbolStackp;
  genericStack_t        *subSymbolStackp;
  genericStack_t        *ruleStackp;
  int                    grammari;
  int                    subGrammari;
  marpaESLIF_symbol_t   *symbolp;
  marpaESLIF_symbol_t   *tmpSymbolp;
  marpaESLIF_symbol_t   *subSymbolp;
  int                    symboli;
  int                    subSymboli;
  marpaESLIF_rule_t     *rulep;
  int                    rulei;
  marpaESLIF_grammar_t  *grammarp;
  marpaESLIF_grammar_t  *subGrammarp;
  marpaESLIF_grammar_t  *tmpGrammarp;
  short                  lhsb;
  marpaESLIF_symbol_t   *lhsp;
  int                    lookupLevelDeltai;
  marpaESLIF_string_t   *lookupGrammarStringp;
  marpaESLIF_symbol_t   *startp;
  marpaESLIF_symbol_t   *discardp;
  short                  rcb;
  size_t                 asciishowl;
  marpaESLIF_cloneContext_t marpaESLIF_cloneContext = {
    marpaESLIFp,
    NULL /* grammarp */
  };
  marpaWrapperGrammarCloneOption_t marpaWrapperGrammarCloneOption = {
    (void *) &marpaESLIF_cloneContext,
    NULL,
    NULL /* ruleOptionSetterp */
  };

  /* MARPAESLIF_TRACE(marpaESLIFp, funcs, "Validating ESLIF grammar"); */

  /* The rules are:

   1. There must be a grammar at level 0
   2. Grammar at any level must precompute at its start symbol and its eventual discard symbol
     a. Only one symbol can have the start flag
     b. Only one symbol can have the discard flag
   3. At any grammar level n, if a symbol never appear as an LHS of a rule, then
      it must be an LHS of grammar at level leveli, which must de-factor must also exist.
   4. At any grammar level n, if there is a :discard it must be precompute at this symbol

      It is not illegal to have sparse items in grammarStackp.

      The end of this routine is filling grammar information.
  */

  /*
   1. There must be a grammar at level 0
  */
  if (! GENERICSTACK_IS_PTR(grammarStackp, 0)) {
    MARPAESLIF_ERROR(marpaESLIFp, "No top-level grammar");
    goto err;
  }
  grammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(grammarStackp, 0);
  /*
   2. Grammar at level 0 must precompute at its start symbol, grammar at level n at its eventual discard symbol
     a. Only one symbol can have the start flag
     b. Only one symbol can have the discard flag
  */
  for (grammari = 0; grammari < GENERICSTACK_USED(grammarStackp); grammari++) {
    if (! GENERICSTACK_IS_PTR(grammarStackp, grammari)) {
      /* Sparse item in grammarStackp -; */
      continue;
    }
    grammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(grammarStackp, grammari);
    symbolStackp = grammarp->symbolStackp;
    ruleStackp = grammarp->ruleStackp;

    /* :start meta symbol check */
    startp = NULL;
    for (symboli = 0; symboli < GENERICSTACK_USED(symbolStackp); symboli++) {
      if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
        /* Should never happen, but who knows */
        continue;
      }
      symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);
      if (symbolp->startb) {
        if (startp == NULL) {
          startp = symbolp;
        } else {
          MARPAESLIF_TRACEF(marpaESLIFp, funcs, "More than one :start symbol at grammar level %d (%s): symbols No %d (%s) and %d (%s)", grammari, grammarp->descp->asciis, startp->idi, startp->descp->asciis, symbolp->idi, symbolp->descp->asciis);
          goto err;
        }
      }
    }
    if (startp == NULL) {
      /* Use the first rule */
      rulep = NULL;
      for (rulei = 0; rulei < GENERICSTACK_USED(ruleStackp); rulei++) {
        if (! GENERICSTACK_IS_PTR(ruleStackp, rulei)) {
          /* Should never happen, but who knows */
          continue;
        }
        rulep = (marpaESLIF_rule_t *) GENERICSTACK_GET_PTR(ruleStackp, rulei);
        break;
      }
      if (rulep == NULL) {
        MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Precomputing grammar level %d (%s) is impossible: no rule", grammari, grammarp->descp->asciis);
        goto err;
      }
      MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Precomputing grammar level %d (%s) at its default start symbol No %d (%s)", grammari, grammarp->descp->asciis, rulep->lhsp->idi, rulep->lhsp->descp->asciis);
      if (! marpaWrapperGrammar_precompute_startb(grammarp->marpaWrapperGrammarStartp, rulep->lhsp->idi)) {
        goto err;
      }
      grammarp->starti = rulep->lhsp->idi;
    } else {
      MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Precomputing grammar level %d (%s) at start symbol No %d (%s)", grammari, grammarp->descp->asciis, startp->idi, startp->descp->asciis);
      if (! marpaWrapperGrammar_precompute_startb(grammarp->marpaWrapperGrammarStartp, startp->idi)) {
        goto err;
      }
      grammarp->starti = startp->idi;
    }

    /* :discard meta symbol check */
    discardp = NULL;
    for (symboli = 0; symboli < GENERICSTACK_USED(symbolStackp); symboli++) {
      if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
        /* Should never happen, but who knows */
        continue;
      }
      symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);
      if (symbolp->discardb) {
        if (discardp == NULL) {
          discardp = symbolp;
        } else {
          MARPAESLIF_TRACEF(marpaESLIFp, funcs, "More than one :discard symbol at grammar level %d (%s): symbols No %d (%s) and %d (%s)", grammari, grammarp->descp->asciis, discardp->idi, discardp->descp->asciis, symbolp->idi, symbolp->descp->asciis);
          goto err;
        }
      }
    }
    if (discardp != NULL) {
      MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Precomputing grammar level %d (%s) at discard symbol No %d (%s)", grammari, grammarp->descp->asciis, discardp->idi, discardp->descp->asciis);
      /* Please note that we disable ALL events in any discarded precomputed grammar except completion on start symbol */
      marpaESLIF_cloneContext.grammarp = grammarp;
      marpaWrapperGrammarCloneOption.symbolOptionSetterp = _marpaESLIFGrammar_symbolOptionSetterOnlyStartCompletionEvent;
      marpaWrapperGrammarClonep = marpaWrapperGrammar_clonep(grammarp->marpaWrapperGrammarStartp, &marpaWrapperGrammarCloneOption);
      if (marpaWrapperGrammarClonep == NULL) {
        MARPAESLIF_ERRORF(marpaESLIFp, "Precomputing grammar level %d (%s) at discard symbol No %d (%s): cloning failure", grammari, grammarp->descp->asciis, discardp->idi, discardp->descp->asciis);
        goto err;
      }
      if (! marpaWrapperGrammar_precompute_startb(marpaWrapperGrammarClonep, discardp->idi)) {
        MARPAESLIF_ERRORF(marpaESLIFp, "Precomputing grammar level %d (%s) at discard symbol No %d (%s): precompute failure", grammari, grammarp->descp->asciis, discardp->idi, discardp->descp->asciis);
        goto err;
      }
      grammarp->marpaWrapperGrammarDiscardp = marpaWrapperGrammarClonep;
      marpaWrapperGrammarClonep = NULL;
    }
  }
  
  /*
    3. In any rule of any grammar, an RHS can be at any level as well. Default being the current one.
    When the RHS level is the current level, if this RHS never appear as an LHS of another rule at the
    same level, then it must be an LHS of grammar at a resolved level, which must de-factor must also exist.
    
    Therefore every grammar is first scanned to detect all symbols that are truely LHS's at this level.
    Then every RHS of every rule is verified: it is must an LHS at its specified grammar level. When found,
    This resolved grammar is precomputed at this found LHS and the result is attached to the symbol of the
    parent grammar.
  */
  for (grammari = 0; grammari < GENERICSTACK_USED(grammarStackp); grammari++) {
    if (! GENERICSTACK_IS_PTR(grammarStackp, grammari)) {
      /* Sparse item in grammarStackp -; */
      continue;
    }
    grammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(grammarStackp, grammari);
    MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Looking at symbols in grammar level %d (%s)", grammari, grammarp->descp->asciis);

    /* Loop on symbols */
    symbolStackp = grammarp->symbolStackp;
    ruleStackp = grammarp->ruleStackp;
    for (symboli = 0; symboli < GENERICSTACK_USED(symbolStackp); symboli++) {
      if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
        /* Should never happen, but who knows */
        continue;
      }
      symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);
      /* Only meta symbols should be looked at: if not an LHS then it is a dependency on a LHS of another grammar */
      if (symbolp->type != MARPAESLIF_SYMBOL_TYPE_META) {
        continue;
      }

      lhsb = 0;
      for (rulei = 0; rulei < GENERICSTACK_USED(ruleStackp); rulei++) {
        if (! GENERICSTACK_IS_PTR(ruleStackp, rulei)) {
          /* Should never happen, but who knows */
          continue;
        }
        rulep = (marpaESLIF_rule_t *) GENERICSTACK_GET_PTR(ruleStackp, rulei);
        lhsp = rulep->lhsp;
        if (lhsp->lhsb) {
          /* Symbol already marked */
        }
        if (_marpaESLIF_string_eqb(lhsp->descp, symbolp->descp)) {
          /* Found */
          MARPAESLIF_TRACEF(marpaESLIFp, funcs, "... Grammar level %d (%s): symbol %d (%s) marked as LHS", grammari, grammarp->descp->asciis, lhsp->idi, lhsp->descp->asciis);
          lhsb = 1;
          break;
        }
      }
      symbolp->lhsb = lhsb;
    }
  }

  /* From grammar point of view, an expected symbol will always be either symbols explicitely created as terminals,
     either symbols not being an LHS. Per definition symbols created as terminals cannot be LHS symbols: precomputing
     the grammar will automatically fail. This is made sure by always precomputing at least grammar at level 0, and
     by precomputing any needed grammar at any other level with an alternative starting symbol.
  */
  for (grammari = 0; grammari < GENERICSTACK_USED(grammarStackp); grammari++) {

    if (! GENERICSTACK_IS_PTR(grammarStackp, grammari)) {
      continue;
    }
    grammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(grammarStackp, grammari);
    MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Looking at rules in grammar level %d (%s)", grammari, grammarp->descp->asciis);

    symbolStackp = grammarp->symbolStackp;
    for (symboli = 0; symboli <= GENERICSTACK_USED(symbolStackp); symboli++) {
      if (symboli < GENERICSTACK_USED(symbolStackp)) {
        if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
          /* Should never happen, but who knows */
          continue;
        }
        symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);
      } else {
        /* Faked additional entry */
        if (rulep->separatorp == NULL) {
          break;
        } else {
          symbolp = rulep->separatorp;
        }
      }
      /* Only non LHS meta symbols should be looked at */
      if ((symbolp->type != MARPAESLIF_SYMBOL_TYPE_META) || symbolp->lhsb) {
        continue;
      }
      metap = symbolp->u.metap;
      /* Since we loop on symbols of every rule, it can very well happen that we hit */
      /* the same meta symbol more than once.                                        */
      if (metap->marpaWrapperGrammarClonep != NULL) {
        MARPAESLIF_TRACEF(marpaESLIFp,  funcs, "... Grammar level %d (%s): symbol %d (%s) already processed", grammari, grammarp->descp->asciis, symbolp->idi, symbolp->descp->asciis);
        continue;
      }

      /* Level of RHS is either via a grammar description, either via a number */
      /* They are exclusive (c.f. the grammar).                                */

      lookupLevelDeltai    = symbolp->lookupLevelDeltai;
      lookupGrammarStringp = symbolp->lookupGrammarStringp;
      subGrammarp = NULL;
      /* We always look to the eventual string first */
      if (lookupGrammarStringp != NULL) {
        /* Look for such a grammar description */
        for (subGrammari = 0; subGrammari < GENERICSTACK_USED(grammarStackp); subGrammari++) {
          if (! GENERICSTACK_IS_PTR(grammarStackp, subGrammari)) {
            continue;
          }
          tmpGrammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(grammarStackp, grammari);
          if (_marpaESLIF_string_eqb(tmpGrammarp->descp, lookupGrammarStringp)) {
            break;
          }
        }
      } else {
        /* RHS level is relative - if RHS level is 0 the we fall back to current grammar */
        subGrammari = grammari + lookupLevelDeltai;
        if ((subGrammari >= 0) && GENERICSTACK_IS_PTR(grammarStackp, subGrammari)) {
          subGrammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(grammarStackp, subGrammari);
        }
      }
      /* Impossible */
      if (subGrammarp == NULL) {
        MARPAESLIF_ERRORF(marpaESLIFp, "Looking at rules in grammar level %d (%s): symbol %d (%s) is referencing a non-existing grammar ", grammari, grammarp->descp->asciis, symbolp->idi, symbolp->descp->asciis);
        goto err;
      }
      /* Make sure this RHS is an LHS in sub grammar, ignoring the case where sub grammar would be current grammar */
      subSymbolStackp = subGrammarp->symbolStackp;
      subSymbolp = NULL;
      for (subSymboli = 0; subSymboli < GENERICSTACK_USED(subSymbolStackp); subSymboli++) {
        if (! GENERICSTACK_IS_PTR(subSymbolStackp, subSymboli)) {
          /* Should never happen, but who knows */
          continue;
        }
        tmpSymbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(subSymbolStackp, subSymboli);
        if (_marpaESLIF_string_eqb(tmpSymbolp->descp, symbolp->descp)) {
          subSymbolp = tmpSymbolp;
          break;
        }
      }
      if (subSymbolp == NULL) {
        MARPAESLIF_ERRORF(marpaESLIFp, "Looking at rules in grammar level %d (%s): symbol %d (%s) is referencing a non-existing symbol at grammar level %d (%s)", grammari, grammarp->descp->asciis, symbolp->idi, symbolp->descp->asciis, subGrammarp->leveli, subGrammarp->descp->asciis);
        goto err;
      }
      if (! subSymbolp->lhsb) {
        /* When sub grammar is current grammar, this mean that we require that this RHS is also an LHS - which is correct because we restricted symbol loop on meta symbols */
        MARPAESLIF_ERRORF(marpaESLIFp, "Looking at rules in grammar level %d (%s): symbol %d (%s) is referencing existing symbol No %d (%s) at grammar level %d (%s) but it is not an LHS", grammari, grammarp->descp->asciis, symbolp->idi, symbolp->descp->asciis, subSymbolp->idi, subSymbolp->descp->asciis, subGrammarp->leveli, subGrammarp->descp->asciis);
        goto err;
      }
      /* Very good - attach a precomputed grammar to this symbol, unless this is current grammar! */
      if (subGrammarp->leveli == grammarp->leveli) {
        continue;
      }
      marpaESLIF_cloneContext.grammarp = grammarp;
      marpaWrapperGrammarClonep = marpaWrapperGrammar_clonep(subGrammarp->marpaWrapperGrammarStartp, &marpaWrapperGrammarCloneOption);
      if (marpaWrapperGrammarClonep == NULL) {
        goto err;
      }
      if (! marpaWrapperGrammar_precompute_startb(marpaWrapperGrammarClonep, subSymbolp->idi)) {
        goto err;
      }
      /* Commit resolved level in symbol */
      symbolp->resolvedLeveli = subGrammarp->leveli;
      metap->marpaWrapperGrammarClonep = marpaWrapperGrammarClonep;
      marpaWrapperGrammarClonep = NULL;
      MARPAESLIF_TRACEF(marpaESLIFp,  funcs, "... Grammar level %d (%s): symbol %d (%s) have grammar resolved level set to %d", grammari, grammarp->descp->asciis, symbolp->idi, symbolp->descp->asciis, symbolp->resolvedLeveli);
    }
  }

  /* Fill grammars information */
  /* - rule IDs, rule show (ASCII) */
  for (grammari = 0; grammari < GENERICSTACK_USED(grammarStackp); grammari++) {
    if (! GENERICSTACK_IS_PTR(grammarStackp, grammari)) {
      continue;
    }
    grammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(grammarStackp, grammari);
    MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Filling rule IDs array in grammar level %d (%s)", grammari, grammarp->descp->asciis);

    ruleStackp = grammarp->ruleStackp;
    grammarp->rulel = 0;
    for (rulei = 0; rulei < GENERICSTACK_USED(ruleStackp); rulei++) {
      if (! GENERICSTACK_IS_PTR(ruleStackp, rulei)) {
        continue;
      }
      grammarp->rulel++;
    }
    if (grammarp->rulel > 0) {
      grammarp->ruleip = (int *) malloc(grammarp->rulel * sizeof(int));
      if (grammarp->ruleip == NULL) {
        MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
        goto err;
      }
      grammarp->rulel = 0;
      for (rulei = 0; rulei < GENERICSTACK_USED(ruleStackp); rulei++) {
        if (! GENERICSTACK_IS_PTR(ruleStackp, rulei)) {
          continue;
        }
        rulep = (marpaESLIF_rule_t *) GENERICSTACK_GET_PTR(ruleStackp, rulei);
        grammarp->ruleip[grammarp->rulel++] = rulep->idi;
        _marpaESLIF_rule_createshowv(marpaESLIFp, rulep, NULL, &asciishowl);
        rulep->asciishows = (char *) malloc(asciishowl);
        if (rulep->asciishows == NULL) {
          MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
          goto err;
        }
        _marpaESLIF_rule_createshowv(marpaESLIFp, rulep, rulep->asciishows, NULL);
      }
    }
  }

  rcb = 1;
  goto done;
  
 err:
  rcb = 0;

 done:
  if (marpaWrapperGrammarClonep != NULL) {
    marpaWrapperGrammar_freev(marpaWrapperGrammarClonep);
  }

  MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %d", (int) rcb);
  return rcb;
}

/*****************************************************************************/
static inline marpaESLIF_grammar_t *_marpaESLIF_grammar_newp(marpaESLIF_t *marpaESLIFp, marpaWrapperGrammarOption_t *marpaWrapperGrammarOptionp, int leveli, char *descs, size_t descl)
/*****************************************************************************/
{
  const static char             *funcs          = "_marpaESLIF_grammar_newp";
  genericLogger_t               *genericLoggerp = NULL;
  marpaESLIF_grammar_t          *grammarp       = NULL;
  _marpaESLIF_stringGenerator_t  _marpaESLIF_stringGenerator;

  /* MARPAESLIF_TRACE(marpaESLIFp, funcs, "Building ESLIF grammar"); */

  grammarp = (marpaESLIF_grammar_t *) malloc(sizeof(marpaESLIF_grammar_t));
  if (grammarp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  grammarp->marpaESLIFp                 = marpaESLIFp;
  grammarp->leveli                      = leveli;
  grammarp->descp                       = NULL;
  grammarp->marpaWrapperGrammarStartp   = NULL;
  grammarp->marpaWrapperGrammarDiscardp = NULL;
  grammarp->discardSymbolp              = NULL;
  grammarp->symbolStackp                = NULL;
  grammarp->ruleStackp                  = NULL;
  grammarp->actionp                     = NULL;
  grammarp->starti                      = 0;    /* Filled during grammar validation */
  grammarp->ruleip                      = NULL; /* Filled by grammar validation */
  grammarp->rulel                       = 0;    /* Filled by grammar validation */

  grammarp->marpaWrapperGrammarStartp = marpaWrapperGrammar_newp(marpaWrapperGrammarOptionp);
  if (grammarp->marpaWrapperGrammarStartp == NULL) {
    goto err;
  }

  /* ----------- Grammar description ------------- */
  if (descs == NULL) {
    /* Generate a default description */
    _marpaESLIF_stringGenerator.marpaESLIFp = marpaESLIFp;
    _marpaESLIF_stringGenerator.s           = NULL;
    _marpaESLIF_stringGenerator.l           = 0;
    _marpaESLIF_stringGenerator.okb         = 0;

    genericLoggerp = GENERICLOGGER_CUSTOM(_marpaESLIF_generateStringWithLoggerCallback, (void *) &_marpaESLIF_stringGenerator, GENERICLOGGER_LOGLEVEL_TRACE);
    if (genericLoggerp == NULL) {
      goto err;
    }
    GENERICLOGGER_TRACEF(genericLoggerp, "Grammar level %d (autogenerated description)", leveli);
    if (! _marpaESLIF_stringGenerator.okb) {
      goto err;
    }
    grammarp->descp = _marpaESLIF_string_newp(marpaESLIFp, NULL, _marpaESLIF_stringGenerator.s, strlen(_marpaESLIF_stringGenerator.s), 1);
    free(_marpaESLIF_stringGenerator.s);
  } else {
    grammarp->descp = _marpaESLIF_string_newp(marpaESLIFp, NULL, descs, descl, 1);
  }
  if (grammarp->descp == NULL) {
    goto err;
  }
  GENERICSTACK_NEW(grammarp->symbolStackp);
  if (GENERICSTACK_ERROR(grammarp->symbolStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "symbolStackp initialization failure, %s", strerror(errno));
    goto err;
  }
  GENERICSTACK_NEW(grammarp->ruleStackp);
  if (GENERICSTACK_ERROR(grammarp->ruleStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "ruleStackp initialization failure, %s", strerror(errno));
    goto err;
  }

  goto done;

 err:
  _marpaESLIF_grammar_freev(grammarp);
  grammarp = NULL;

 done:
  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", grammarp); */
  GENERICLOGGER_FREE(genericLoggerp);
  return grammarp;
}

/*****************************************************************************/
static inline void _marpaESLIF_grammar_freev(marpaESLIF_grammar_t *grammarp)
/*****************************************************************************/
{
  if (grammarp != NULL) {
    _marpaESLIF_string_freev(grammarp->descp);
    if (grammarp->marpaWrapperGrammarStartp != NULL) {
      marpaWrapperGrammar_freev(grammarp->marpaWrapperGrammarStartp);
    }			       
    if (grammarp->marpaWrapperGrammarDiscardp != NULL) {
      marpaWrapperGrammar_freev(grammarp->marpaWrapperGrammarDiscardp);
    }			       
    _marpaESLIF_symbolStack_freev(grammarp->symbolStackp);
    _marpaESLIF_ruleStack_freev(grammarp->ruleStackp);
    if (grammarp->ruleip != NULL) {
      free(grammarp->ruleip);
    }
    /* TO DO : actionp */
    free(grammarp);
  }
}

/*****************************************************************************/
static inline void _marpaESLIF_ruleStack_freev(genericStack_t *ruleStackp)
/*****************************************************************************/
{
  if (ruleStackp != NULL) {
    while (GENERICSTACK_USED(ruleStackp) > 0) {
      if (GENERICSTACK_IS_PTR(ruleStackp, GENERICSTACK_USED(ruleStackp) - 1)) {
	marpaESLIF_rule_t *rulep = (marpaESLIF_rule_t *) GENERICSTACK_POP_PTR(ruleStackp);
	_marpaESLIF_rule_freev(rulep);
      } else {
	GENERICSTACK_USED(ruleStackp)--;
      }
    }
    GENERICSTACK_FREE(ruleStackp);
  }
}

/*****************************************************************************/
static inline void _marpaESLIFrecognizer_lexemeStack_freev(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp)
/*****************************************************************************/
{
  const static char *funcs = "_marpaESLIFrecognizer_lexemeStack_freev";

  if (lexemeStackp != NULL) {
    marpaESLIFRecognizerp->callstackCounteri++;
    MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

    _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizerp, lexemeStackp);
    GENERICSTACK_FREE(lexemeStackp);

    MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "return");
    marpaESLIFRecognizerp->callstackCounteri--;
  }

}

/*****************************************************************************/
static inline void _marpaESLIF_grammarStack_freev(genericStack_t *grammarStackp)
/*****************************************************************************/
{
  if (grammarStackp != NULL) {
    _marpaESLIF_grammarStack_resetv(grammarStackp);
    GENERICSTACK_FREE(grammarStackp);
  }
}

/*****************************************************************************/
static inline void _marpaESLIF_grammarStack_resetv(genericStack_t *grammarStackp)
/*****************************************************************************/
{
  if (grammarStackp != NULL) {
    /* TO DO */
  }
}

/*****************************************************************************/
static inline void _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp)
/*****************************************************************************/
{
  const static char *funcs = "_marpaESLIFrecognizer_lexemeStack_resetv";
  int                i;
  int                usedi;

  if (lexemeStackp != NULL) {

    marpaESLIFRecognizerp->callstackCounteri++;
    MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

    usedi = (int) GENERICSTACK_USED(lexemeStackp);
    for (i = usedi - 1; i >= 0; i--) {
      /* if it failed, we will have a memory leak - error will be logged */
      /* In practice it never fails, because _marpaESLIFRecognizer_lexemeStack_i_resetb() */
      /* frees() only something that it recognizes, and afterwards set it to NA - and the */
      /* to NA cannot fail, because this place place in stack memory is already there. */
      /* Anyway, suppose that it would fail, I repeat, at most there is a memory leak */
      _marpaESLIFRecognizer_lexemeStack_i_resetb(marpaESLIFRecognizerp, lexemeStackp, i);
      GENERICSTACK_USED(lexemeStackp)--;
    }

    MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "return");
    marpaESLIFRecognizerp->callstackCounteri--;
  }

}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_lexemeStack_i_resetb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i)
/*****************************************************************************/
{
  const static char *funcs = "_marpaESLIFRecognizer_lexemeStack_i_resetb";
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  if (lexemeStackp != NULL) {
    if (GENERICSTACK_IS_ARRAY(lexemeStackp, i)) {
      GENERICSTACKITEMTYPE2TYPE_ARRAY array = GENERICSTACK_GET_ARRAY(lexemeStackp, i);
      if (GENERICSTACK_ARRAY_PTR(array) != NULL) {
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Freeing %p->[%d] = {%p, %d}", lexemeStackp, i, GENERICSTACK_ARRAY_PTR(array), GENERICSTACK_ARRAY_LENGTH(array));
        free(GENERICSTACK_ARRAY_PTR(array));
      }
      MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Resetting %p->[%d]", lexemeStackp, i);
      GENERICSTACK_SET_NA(lexemeStackp, i);
      if (GENERICSTACK_ERROR(lexemeStackp)) {
        MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "lexemeStackp %p->[%d] set failure, %s", lexemeStackp, i, strerror(errno));
        goto err;
      }
    }
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_lexemeStack_i_setarraypb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, GENERICSTACKITEMTYPE2TYPE_ARRAYP arrayp)
/*****************************************************************************/
{
  const static char *funcs = "_marpaESLIFRecognizer_lexemeStack_i_setarraypb";
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  if (! _marpaESLIFRecognizer_lexemeStack_i_resetb(marpaESLIFRecognizerp, lexemeStackp, i)) {
    goto err;
  }
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Setting %p->[%d] = {%p, %d}", lexemeStackp, i, GENERICSTACK_ARRAYP_PTR(arrayp), GENERICSTACK_ARRAYP_LENGTH(arrayp));
  GENERICSTACK_SET_ARRAYP(lexemeStackp, arrayp, i);
  if (GENERICSTACK_ERROR(lexemeStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "lexemeStackp %p->[%d] set failure, %s", lexemeStackp, i, strerror(errno));
    goto err;
  }

  rcb = 1;
  goto done;
  
 err:
  rcb = 0;

 done:
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_lexemeStack_i_setb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, void *p, size_t sizel)
/*****************************************************************************/
{
  const static char              *funcs = "_marpaESLIFRecognizer_lexemeStack_i_setb";
  GENERICSTACKITEMTYPE2TYPE_ARRAY array;
  int                             rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  GENERICSTACK_ARRAY_PTR(array)    = p;
  GENERICSTACK_ARRAY_LENGTH(array) = sizel;
  rcb = _marpaESLIFRecognizer_lexemeStack_i_setarraypb(marpaESLIFRecognizerp, lexemeStackp, i, &array);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_lexemeStack_i_moveb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackDstp, int dsti, genericStack_t *lexemeStackSrcp, int srci)
/*****************************************************************************/
{
  const static char              *funcs = "_marpaESLIFRecognizer_lexemeStack_i_moveb";
  GENERICSTACKITEMTYPE2TYPE_ARRAY array;
  short                           rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  GENERICSTACK_ARRAY_PTR(array)    = NULL;
  GENERICSTACK_ARRAY_LENGTH(array) = 0;
  
  if (! GENERICSTACK_IS_ARRAY(lexemeStackSrcp, srci)) {
    MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "Bad type %s in lexemeStackSrcp %p at indice %d", _marpaESLIF_genericStack_i_types(lexemeStackSrcp, srci), lexemeStackSrcp, srci);
    goto err;
  }
  array = GENERICSTACK_GET_ARRAY(lexemeStackSrcp, srci);
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Got %p->[%d] = {%p, %d}", lexemeStackSrcp, srci, GENERICSTACK_ARRAY_PTR(array), GENERICSTACK_ARRAY_LENGTH(array));
  GENERICSTACK_SET_NA(lexemeStackSrcp, srci);
  if (GENERICSTACK_ERROR(lexemeStackSrcp)) {
    MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "lexemeStackSrcp set failure, %s", strerror(errno));
    goto err;
  }
  if (! _marpaESLIFRecognizer_lexemeStack_i_setarraypb(marpaESLIFRecognizerp, lexemeStackDstp, dsti, &array)) {
    goto err;
  }

  rcb = 1;
  goto done;
  
 err:
  if (GENERICSTACK_ARRAY_PTR(array) != NULL) {
    free(GENERICSTACK_ARRAY_PTR(array));
  }
  rcb = 0;

 done:
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_lexemeStack_i_dupb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackDstp, int dsti, genericStack_t *lexemeStackSrcp, int srci)
/*****************************************************************************/
{
  const static char              *funcs = "_marpaESLIFRecognizer_lexemeStack_i_dupb";
  GENERICSTACKITEMTYPE2TYPE_ARRAY srcArray;
  GENERICSTACKITEMTYPE2TYPE_ARRAY dstArray;
  short                           rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  GENERICSTACK_ARRAY_PTR(srcArray)    = NULL;
  GENERICSTACK_ARRAY_LENGTH(srcArray) = 0;

  GENERICSTACK_ARRAY_PTR(dstArray)    = NULL;
  GENERICSTACK_ARRAY_LENGTH(dstArray) = 0;
  
  if (! GENERICSTACK_IS_ARRAY(lexemeStackSrcp, srci)) {
    MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "Bad type %s in lexemeStackSrcp %p at indice %d", _marpaESLIF_genericStack_i_types(lexemeStackSrcp, srci), lexemeStackSrcp, srci);
    goto err;
  }

  /* Prepare the clone, prevent free of origin if any failure */
  srcArray = GENERICSTACK_GET_ARRAY(lexemeStackSrcp, srci);
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Got %p->[%d] = {%p, %d}", lexemeStackSrcp, srci, GENERICSTACK_ARRAY_PTR(srcArray), GENERICSTACK_ARRAY_LENGTH(srcArray));
  if (GENERICSTACK_ARRAY_PTR(srcArray) != NULL) {
    if (GENERICSTACK_ARRAY_LENGTH(srcArray) <= 0) {
      MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "Got %p->[%d] = {%p, %d}", lexemeStackSrcp, srci, GENERICSTACK_ARRAY_PTR(srcArray), GENERICSTACK_ARRAY_LENGTH(srcArray));
      goto err;
    }
    GENERICSTACK_ARRAY_PTR(dstArray) = malloc(GENERICSTACK_ARRAY_LENGTH(srcArray));
    if (GENERICSTACK_ARRAY_PTR(dstArray) == NULL) {
      MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "malloc failure, %s", strerror(errno));
      goto err;
    }
    memcpy(GENERICSTACK_ARRAY_PTR(dstArray), GENERICSTACK_ARRAY_PTR(srcArray), GENERICSTACK_ARRAY_LENGTH(srcArray));
    GENERICSTACK_ARRAY_LENGTH(dstArray) = GENERICSTACK_ARRAY_LENGTH(srcArray);
    MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Dupped %p->[%d] = {%p, %d} to {%p, %d}", lexemeStackSrcp, srci, GENERICSTACK_ARRAY_PTR(srcArray), GENERICSTACK_ARRAY_LENGTH(srcArray), GENERICSTACK_ARRAY_PTR(dstArray), GENERICSTACK_ARRAY_LENGTH(dstArray));
  }
  if (! _marpaESLIFRecognizer_lexemeStack_i_setarraypb(marpaESLIFRecognizerp, lexemeStackDstp, dsti, &dstArray)) {
    goto err;
  }
  /* Never free dst: copy is successful */
  /* GENERICSTACK_ARRAY_PTR(dstArray) = NULL; /* /* Commented because the goto will make sure we never do the free... */

  rcb = 1;
  goto done;
  
 err:
  if (GENERICSTACK_ARRAY_PTR(dstArray) != NULL) {
    free(GENERICSTACK_ARRAY_PTR(dstArray));
  }
  rcb = 0;

 done:
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline marpaESLIF_rule_t *_marpaESLIF_rule_newp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, char *descs, size_t descl, int lhsi, size_t nrhsl, int *rhsip, short *maskbp, size_t nexceptionl, int *exceptionip, int ranki, short nullRanksHighb, short sequenceb, int minimumi, int separatori, short properb)
/*****************************************************************************/
{
  const static char               *funcs        = "_marpaESLIF_rule_newp";
  genericStack_t                  *symbolStackp = grammarp->symbolStackp;
  short                            symbolFoundb = 0;
  short                            separatorFoundb;
  marpaESLIF_symbol_t             *symbolp;
  marpaESLIF_rule_t               *rulep;
  marpaWrapperGrammarRuleOption_t  marpaWrapperGrammarRuleOption;
  size_t                           i;
  short                            maskb;
  int                              symboli;

  /* MARPAESLIF_TRACE(marpaESLIFp, funcs, "Building rule"); */

  rulep = (marpaESLIF_rule_t *) malloc(sizeof(marpaESLIF_rule_t));
  if (rulep == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  rulep->idi             = -1;
  rulep->descp           = NULL;
  rulep->asciishows      = NULL; /* Filled by grammar validation */
  rulep->lhsp            = NULL;
  rulep->separatorp      = NULL;
  rulep->rhsStackp       = NULL;
  rulep->maskStackp      = NULL;
  rulep->exceptionStackp = NULL;
  rulep->actionp         = NULL;
  rulep->ranki           = ranki;
  rulep->nullRanksHighb  = nullRanksHighb;
  rulep->sequenceb       = sequenceb;
  rulep->properb         = properb;
  rulep->minimumi        = minimumi;

  /* -------- Rule Description -------- */
  rulep->descp = _marpaESLIF_string_newp(marpaESLIFp, NULL, descs, descl, 1);
  if (rulep->descp == NULL) {
    goto err;
  }

  /* Look to the symbol itself, and remember it is an LHS - this is used when validating the grammar */
  for (symboli = 0; symboli < GENERICSTACK_USED(symbolStackp); symboli++) {
    if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
      /* Should never happen, but who knows */
      continue;
    }
    symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);
    if (symbolp->idi == lhsi) {
      symbolFoundb = 1;
      break;
    }
  }
  if (! symbolFoundb) {
    MARPAESLIF_ERRORF(marpaESLIFp, "At grammar level %d, rule %s: LHS symbol No %d does not exist", grammarp->leveli, rulep->descp->asciis, lhsi);
    goto err;
  }
  symbolp->lhsb = 1;
  rulep->lhsp = symbolp;

  /* Idem for the separator */
  if (sequenceb && (separatori >= 0)) {
    separatorFoundb = 0;
    for (symboli = 0; symboli < GENERICSTACK_USED(symbolStackp); symboli++) {
      if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
        /* Should never happen, but who knows */
        continue;
      }
      symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);
      if (symbolp->idi == separatori) {
        separatorFoundb = 1;
        break;
      }
    }
    if (! separatorFoundb) {
      MARPAESLIF_ERRORF(marpaESLIFp, "At grammar level %d, rule %s: LHS separator No %d does not exist", grammarp->leveli, rulep->descp->asciis, separatori);
      goto err;
    }
    rulep->separatorp = symbolp;
  }

  GENERICSTACK_NEW(rulep->rhsStackp);
  if (GENERICSTACK_ERROR(rulep->rhsStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "rhsStackp initialization failure, %s", strerror(errno));
    goto err;
  }

  GENERICSTACK_NEW(rulep->maskStackp);
  if (GENERICSTACK_ERROR(rulep->maskStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "maskStackp initialization failure, %s", strerror(errno));
    goto err;
  }

  GENERICSTACK_NEW(rulep->exceptionStackp);
  if (GENERICSTACK_ERROR(rulep->exceptionStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "exceptionStackp initialization failure, %s", strerror(errno));
    goto err;
  }

  /* Fill rhs symbol stack */
  for (i = 0; i < nrhsl; i++) {
    if (! GENERICSTACK_IS_PTR(grammarp->symbolStackp, rhsip[i])) {
      MARPAESLIF_ERRORF(marpaESLIFp, "At grammar level %d, rule %s: No such RHS symbol No %d", grammarp->leveli, rulep->descp->asciis, rhsip[i]);
      goto err;
    }
    GENERICSTACK_PUSH_PTR(rulep->rhsStackp, GENERICSTACK_GET_PTR(grammarp->symbolStackp, rhsip[i]));
    if (GENERICSTACK_ERROR(rulep->rhsStackp)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "rhsStackp push failure, %s", strerror(errno));
      goto err;
    }
    /* It is ASSUMED that the caller allocated as many items as there are RHSs */
    maskb = (maskbp != NULL) ? maskbp[i] : 0;
    GENERICSTACK_PUSH_SHORT(rulep->maskStackp, maskb);
    if (GENERICSTACK_ERROR(rulep->maskStackp)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "maskStackp push failure, %s", strerror(errno));
      goto err;
    }
  }
  
  /* Fill exception symbol stack */
  for (i = 0; i < nexceptionl; i++) {
    if (! GENERICSTACK_IS_PTR(grammarp->symbolStackp, exceptionip[i])) {
      MARPAESLIF_ERRORF(marpaESLIFp, "At grammar level %d, rule %s: No such RHS exception symbol No %d", grammarp->leveli, rulep->descp->asciis, exceptionip[i]);
      goto err;
    }
    GENERICSTACK_PUSH_PTR(rulep->exceptionStackp, GENERICSTACK_GET_PTR(grammarp->symbolStackp, exceptionip[i]));
    if (GENERICSTACK_ERROR(rulep->exceptionStackp)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "exceptionStackp push failure, %s", strerror(errno));
      goto err;
    }
  }
  
  marpaWrapperGrammarRuleOption.ranki            = ranki;
  marpaWrapperGrammarRuleOption.nullRanksHighb   = nullRanksHighb;
  marpaWrapperGrammarRuleOption.sequenceb        = sequenceb;
  marpaWrapperGrammarRuleOption.separatorSymboli = separatori;
  marpaWrapperGrammarRuleOption.properb          = properb;
  marpaWrapperGrammarRuleOption.minimumi         = minimumi;

  /* ----------- Meta Identifier ------------ */
  rulep->idi = marpaWrapperGrammar_newRulei(grammarp->marpaWrapperGrammarStartp, &marpaWrapperGrammarRuleOption, lhsi, nrhsl, rhsip);
  if (rulep->idi < 0) {
    goto err;
  }

  goto done;

 err:
  _marpaESLIF_rule_freev(rulep);
  rulep = NULL;

 done:
  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", rulep); */
  return rulep;
}

/*****************************************************************************/
static inline void _marpaESLIF_rule_freev(marpaESLIF_rule_t *rulep)
/*****************************************************************************/
{
  if (rulep != NULL) {
    _marpaESLIF_string_freev(rulep->descp);
    if (rulep->asciishows != NULL) {
      free(rulep->asciishows);
    }
    /* In the rule structure, lhsp, rhsStackp and exceptionStackp contain shallow pointers */
    /* Only the stack themselves should be freed. */
    /*
    _marpaESLIF_symbol_freev(rulep->lhsp);
    _marpaESLIF_symbolStack_freev(rulep->rhsStackp);
    _marpaESLIF_symbolStack_freev(marpaESLIFp, exceptionStackp);
    */
    GENERICSTACK_FREE(rulep->maskStackp);
    GENERICSTACK_FREE(rulep->rhsStackp);
    GENERICSTACK_FREE(rulep->exceptionStackp);
    /* TO DO : actionp */
    free(rulep);
  }
}

/*****************************************************************************/
static inline marpaESLIF_symbol_t *_marpaESLIF_symbol_newp(marpaESLIF_t *marpaESLIFp)
/*****************************************************************************/
{
  const static char   *funcs = "_marpaESLIF_symbol_newp";
  marpaESLIF_symbol_t *symbolp;

  /* MARPAESLIF_TRACE(marpaESLIFp, funcs, "Building symbol"); */

  symbolp = (marpaESLIF_symbol_t *) malloc(sizeof(marpaESLIF_symbol_t));
  if (symbolp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto done;
  }

  symbolp->type   = MARPAESLIF_SYMBOL_TYPE_NA;
  /* Union itself is undetermined at this stage */
  symbolp->startb               = 0;
  symbolp->discardb             = 0;
  symbolp->lhsb                 = 0;
  symbolp->idi                  = -1;
  symbolp->descp                = NULL;
  symbolp->pauseb               = 0;
  symbolp->pauseIsOnb           = 0;
  symbolp->pausep               = NULL;
  symbolp->asciipauses          = NULL;
  symbolp->eventp               = NULL;
  symbolp->asciievents          = NULL;
  symbolp->lookupLevelDeltai    = 1;   /* Default lookup is the next grammar level */
  symbolp->lookupGrammarStringp = NULL;
  symbolp->resolvedLeveli       = -1; /* This will be overwriten by _marpaESLIFGrammar_validateb() and used only when symbol is a lexeme from another grammar */
  symbolp->priorityi            = 0; /* Default priority is 0 */
  symbolp->actionp              = NULL;

 done:
  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", symbolp); */
  return symbolp;
}

/*****************************************************************************/
static inline void _marpaESLIF_symbol_freev(marpaESLIF_symbol_t *symbolp)
/*****************************************************************************/
{
  if (symbolp != NULL) {
    /* All pointers are the top level of this structure are shallow pointers */
    switch (symbolp->type) {
    case MARPAESLIF_SYMBOL_TYPE_TERMINAL:
      _marpaESLIF_terminal_freev(symbolp->u.terminalp);
      break;
    case MARPAESLIF_SYMBOL_TYPE_META:
      _marpaESLIF_meta_freev(symbolp->u.metap);
      break;
    default:
      break;
    }
    _marpaESLIF_string_freev(symbolp->pausep);
    _marpaESLIF_utf82printableascii_freev(symbolp->asciipauses);
    _marpaESLIF_string_freev(symbolp->eventp);
    _marpaESLIF_utf82printableascii_freev(symbolp->asciievents);
    /* TO DO : actionp */
    free(symbolp);
  }
}

/*****************************************************************************/
static inline void _marpaESLIF_symbolStack_freev(genericStack_t *symbolStackp)
/*****************************************************************************/
{
  if (symbolStackp != NULL) {
    while (GENERICSTACK_USED(symbolStackp) > 0) {
      if (GENERICSTACK_IS_PTR(symbolStackp, GENERICSTACK_USED(symbolStackp) - 1)) {
	marpaESLIF_symbol_t *symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_POP_PTR(symbolStackp);
	_marpaESLIF_symbol_freev(symbolp);
      } else {
	GENERICSTACK_USED(symbolStackp)--;
      }
    }
    GENERICSTACK_FREE(symbolStackp);
  }
}

/*****************************************************************************/
static inline void _marpaESLIF_terminal_freev(marpaESLIF_terminal_t *terminalp)
/*****************************************************************************/
{
  if (terminalp != NULL) {
    _marpaESLIF_string_freev(terminalp->descp);
    if (terminalp->regex.match_datap != NULL) {
      pcre2_match_data_free(terminalp->regex.match_datap);
    }
    if (terminalp->regex.patternp != NULL) {
      pcre2_code_free(terminalp->regex.patternp);
    }
    free(terminalp);
  }
}

/*****************************************************************************/
marpaESLIF_t *marpaESLIF_newp(marpaESLIFOption_t *marpaESLIFOptionp)
/*****************************************************************************/
{
  const static char     *funcs       = "marpaESLIF_newp";
  marpaESLIF_grammar_t  *grammarp    = NULL;
  marpaESLIF_t          *marpaESLIFp = NULL;
  genericLogger_t       *genericLoggerp;

  if (marpaESLIFOptionp == NULL) {
    marpaESLIFOptionp = &marpaESLIFOption_default;
  }

  genericLoggerp = marpaESLIFOptionp->genericLoggerp;

#ifndef MARPAESLIF_NTRACE
  if (genericLoggerp != NULL) {
    GENERICLOGGER_TRACEF(genericLoggerp, "[%s] Building ESLIF", funcs);
  }
#endif

  marpaESLIFp = (marpaESLIF_t *) malloc(sizeof(marpaESLIF_t));
  if (marpaESLIFp == NULL) {
    if (marpaESLIFOptionp->genericLoggerp != NULL) {
      GENERICLOGGER_ERRORF(marpaESLIFOptionp->genericLoggerp, "malloc failure, %s", strerror(errno));
      goto err;
    }
  }

  marpaESLIFp->marpaESLIFOption   = *marpaESLIFOptionp;
  marpaESLIFp->marpaESLIFGrammarp = NULL;
  marpaESLIFp->anycharp           = NULL;
  marpaESLIFp->utf8bomp           = NULL;
  marpaESLIFp->newlinep           = NULL;

  /* Create internal anychar regex */
  marpaESLIFp->anycharp = _marpaESLIF_terminal_newp(marpaESLIFp,
                                                    NULL, /* grammarp */
                                                    MARPAESLIF_EVENTTYPE_NONE, /* eventSeti */
                                                    NULL, /* descEncodings */
                                                    INTERNAL_ANYCHAR_PATTERN, /* descs */
                                                    strlen(INTERNAL_ANYCHAR_PATTERN), /* descl */
                                                    MARPAESLIF_TERMINAL_TYPE_REGEX, /* type */
                                                    MARPAESLIF_REGEX_OPTION_DOTALL|MARPAESLIF_REGEX_OPTION_UTF, /* opti */
                                                    INTERNAL_ANYCHAR_PATTERN, /* utf8s */
                                                    strlen(INTERNAL_ANYCHAR_PATTERN), /* utf8l */
                                                    NULL, /* testFullMatchs */
                                                    NULL, /* testPartialMatchs */
                                                    MARPAESLIF_ACTION_TYPE_INTERNAL_LEXEME, /* actionType */
                                                    NULL, /* actionStackp */
                                                    NULL, /* actionEncodings */
                                                    NULL, /* actions */
                                                    0 /* actionl */
                                                    );
  if (marpaESLIFp->anycharp == NULL) {
    goto err;
  }

  /* Create internal utf8bom regex */
  marpaESLIFp->utf8bomp = _marpaESLIF_terminal_newp(marpaESLIFp,
                                                    NULL /* grammarp */,
                                                    MARPAESLIF_EVENTTYPE_NONE, /* eventSeti */
                                                    NULL, /* descEncodings */
                                                    INTERNAL_UTF8BOM_PATTERN, /* descs */
                                                    strlen(INTERNAL_UTF8BOM_PATTERN), /* descl */
                                                    MARPAESLIF_TERMINAL_TYPE_REGEX, /* type */
                                                    MARPAESLIF_REGEX_OPTION_UTF, /* opti */
                                                    INTERNAL_UTF8BOM_PATTERN, /* utf8s */
                                                    strlen(INTERNAL_UTF8BOM_PATTERN), /* utf8l */
                                                    NULL, /* testFullMatchs */
                                                    NULL, /* testPartialMatchs */
                                                    MARPAESLIF_ACTION_TYPE_INTERNAL_LEXEME, /* actionType */
                                                    NULL, /* actionStackp */
                                                    NULL, /* actionEncodings */
                                                    NULL, /* actions */
                                                    0 /* actionl */
                                                    );
  if (marpaESLIFp->utf8bomp == NULL) {
    goto err;
  }

  /* Create internal newline regex */
  /* Please note that the newline regexp does NOT require UTF-8 correctness -; */
  marpaESLIFp->newlinep = _marpaESLIF_terminal_newp(marpaESLIFp,
                                                    NULL /* grammarp */,
                                                    MARPAESLIF_EVENTTYPE_NONE, /* eventSeti */
                                                    NULL, /* descEncodings */
                                                    INTERNAL_NEWLINE_PATTERN /* descs */,
                                                    strlen(INTERNAL_NEWLINE_PATTERN) /* descl */,
                                                    MARPAESLIF_TERMINAL_TYPE_REGEX, /* type */
                                                    MARPAESLIF_REGEX_OPTION_NA, /* opti */
                                                    INTERNAL_NEWLINE_PATTERN, /* utf8s */
                                                    strlen(INTERNAL_NEWLINE_PATTERN), /* utf8l */
                                                    NULL, /* testFullMatchs */
                                                    NULL, /* testPartialMatchs */
                                                    MARPAESLIF_ACTION_TYPE_INTERNAL_LEXEME, /* actionType */
                                                    NULL, /* actionStackp */
                                                    NULL, /* actionEncodings */
                                                    NULL, /* actions */
                                                    0 /* actionl */
                                                    );
  if (marpaESLIFp->newlinep == NULL) {
    goto err;
  }
  
  /* Create internal ESLIF grammar - it is important to set the option first */
  marpaESLIFp->marpaESLIFGrammarp = (marpaESLIFGrammar_t *) malloc(sizeof(marpaESLIFGrammar_t));

  if (marpaESLIFp->marpaESLIFGrammarp == NULL) {
    GENERICLOGGER_ERRORF(marpaESLIFOptionp->genericLoggerp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  marpaESLIFp->marpaESLIFGrammarp->marpaESLIFp   = marpaESLIFp;
  marpaESLIFp->marpaESLIFGrammarp->grammarStackp = NULL;
  marpaESLIFp->marpaESLIFGrammarp->grammarp      = NULL;

  GENERICSTACK_NEW(marpaESLIFp->marpaESLIFGrammarp->grammarStackp);
  if (GENERICSTACK_ERROR(marpaESLIFp->marpaESLIFGrammarp->grammarStackp)) {
    GENERICLOGGER_ERRORF(marpaESLIFOptionp->genericLoggerp, "marpaESLIFp->marpaESLIFGrammarp->grammarStackp initialization failure, %s", strerror(errno));
    goto err;
  }

  /* G1 */
  grammarp = _marpaESLIF_bootstrap_grammar_G1p(marpaESLIFp);
  if (grammarp == NULL) {
    goto err;
  }
  GENERICSTACK_SET_PTR(marpaESLIFp->marpaESLIFGrammarp->grammarStackp, grammarp, grammarp->leveli);
  if (GENERICSTACK_ERROR(marpaESLIFp->marpaESLIFGrammarp->grammarStackp)) {
    GENERICLOGGER_ERRORF(marpaESLIFOptionp->genericLoggerp, "marpaESLIFp->marpaESLIFGrammarp->grammarStackp set failure, %s", strerror(errno));
    goto err;
  }
  grammarp = NULL;

  /* L0 */
  grammarp = _marpaESLIF_bootstrap_grammar_L0p(marpaESLIFp);
  if (grammarp == NULL) {
    goto err;
  }
  GENERICSTACK_SET_PTR(marpaESLIFp->marpaESLIFGrammarp->grammarStackp, grammarp, grammarp->leveli);
  if (GENERICSTACK_ERROR(marpaESLIFp->marpaESLIFGrammarp->grammarStackp)) {
    GENERICLOGGER_ERRORF(marpaESLIFOptionp->genericLoggerp, "marpaESLIFp->marpaESLIFGrammarp->grammarStackp set failure, %s", strerror(errno));
    goto err;
  }
  grammarp = NULL;

  /* Validate the bootstrap grammar */
  if (! _marpaESLIFGrammar_validateb(marpaESLIFp->marpaESLIFGrammarp)) {
    goto err;
  }

  /* Commit G1 grammar as the current grammar */
  if (! GENERICSTACK_IS_PTR(marpaESLIFp->marpaESLIFGrammarp->grammarStackp, 0)) {
    GENERICLOGGER_ERROR(marpaESLIFOptionp->genericLoggerp, "No top level grammar after bootstrap");
    goto err;
  }
  marpaESLIFp->marpaESLIFGrammarp->grammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(marpaESLIFp->marpaESLIFGrammarp->grammarStackp, 0);
  goto done;
  
 err:
  marpaESLIF_freev(marpaESLIFp);
  marpaESLIFp = NULL;

 done:
  _marpaESLIF_grammar_freev(grammarp);
#ifndef MARPAESLIF_NTRACE
  if (genericLoggerp != NULL) {
    GENERICLOGGER_TRACEF(genericLoggerp, "[%s] return %p", funcs, marpaESLIFp);
  }
#endif
	
  return marpaESLIFp;
}


/*****************************************************************************/
void marpaESLIF_freev(marpaESLIF_t *marpaESLIFp)
/*****************************************************************************/
{
  if (marpaESLIFp != NULL) {
    marpaESLIFGrammar_freev(marpaESLIFp->marpaESLIFGrammarp);
    _marpaESLIF_terminal_freev(marpaESLIFp->anycharp);
    _marpaESLIF_terminal_freev(marpaESLIFp->utf8bomp);
    _marpaESLIF_terminal_freev(marpaESLIFp->newlinep);
    free(marpaESLIFp);
  }
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_regex_matcherb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIF_terminal_t *terminalp, char *inputs, size_t inputl, short eofb, marpaESLIF_matcher_value_t *rcip, genericStack_t *outputStackp)
/*****************************************************************************/
{
  const static char          *funcs = "_marpaESLIFRecognizer_regex_matcherb";
  marpaESLIF_t               *marpaESLIFp = marpaESLIFRecognizerp->marpaESLIFp;
  char                       *tmps  = NULL;
  marpaESLIF_matcher_value_t  rci;
  marpaESLIF_regex_t          marpaESLIF_regex;
  int                         pcre2Errornumberi;
  PCRE2_UCHAR                 pcre2ErrorBuffer[256];
  PCRE2_SIZE                 *pcre2_ovectorp;
  size_t                      matchedLengthl;
  marpaESLIF_uint32_t         pcre2_option;
  short                       binmodeb;
  short                       rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  /*********************************************************************************/
  /* A matcher tries to match a terminal v.s. input that is eventually incomplete. */
  /* It return 1 on success, 0 on failure, -1 if more data is needed.              */
  /*********************************************************************************/

  if (inputl > 0) {

    marpaESLIF_regex = terminalp->regex;

    /* If the regexp is working in UTF mode then we check that character conversion   */
    /* was done. This is how we are sure that calling regexp with PCRE2_NO_UTF_CHECK  */
    /* is ok: we have done ourself the UTF-8 validation on the subject.               */
    /* This is a boost in performance also when we are using the built-in PCRE2: the  */
    /* later is never compiled with JIT support, which mean that UTF checking is done */
    /* by default, unless PCRE2_NO_UTF_CHECK is set.                                  */
    if (marpaESLIF_regex.utfb) {                     /* UTF-8 correctness is required */
      if (! *(marpaESLIFRecognizerp->utfbp)) {
        pcre2_option = pcre2_option_binary_default;  /* We have done no conversion : PCRE2 will check */
        binmodeb = 1;
      } else {
        pcre2_option = pcre2_option_char_default;    /* We made sure this is ok */
        binmodeb = 0;
      }
    } else {
      pcre2_option = pcre2_option_binary_default;    /* Not needed */
      binmodeb = 1;
    }

    /* --------------------------------------------------------- */
    /* EOF mode:                                                 */
    /* return full match status: OK or FAILURE.                  */
    /* --------------------------------------------------------- */
    /* NOT EOF mode:                                             */
    /* If the full match is successful:                          */
    /* - if it reaches the end of the buffer, return EGAIN.      */
    /* - if it does not reach the end of the buffer, return OK.  */
    /* Else if the partial match is successul:                   */
    /* - return EGAIN.                                           */
    /* Else                                                      */
    /* - return FAILURE.                                         */
    /*                                                           */
    /* In conclusion we always start with the full match.        */
    /* --------------------------------------------------------- */
#ifdef PCRE2_CONFIG_JIT
    if (marpaESLIF_regex.jitCompleteb) {
      pcre2Errornumberi = pcre2_jit_match(marpaESLIF_regex.patternp,    /* code */
                                          (PCRE2_SPTR) inputs,          /* subject */
                                          (PCRE2_SIZE) inputl,          /* length */
                                          (PCRE2_SIZE) 0,               /* startoffset */
                                          pcre2_option,                 /* options */
                                          marpaESLIF_regex.match_datap, /* match data */
                                          NULL                          /* match context - used default */
                                          );
      if (pcre2Errornumberi == PCRE2_ERROR_JIT_STACKLIMIT) {
        /* Back luck, out of stack for JIT */
        pcre2_get_error_message(pcre2Errornumberi, pcre2ErrorBuffer, sizeof(pcre2ErrorBuffer));
        goto eof_nojitcomplete;
      }
    } else {
    eof_nojitcomplete:
#endif
      pcre2Errornumberi = pcre2_match(marpaESLIF_regex.patternp,    /* code */
                                      (PCRE2_SPTR) inputs,          /* subject */
                                      (PCRE2_SIZE) inputl,          /* length */
                                      (PCRE2_SIZE) 0,               /* startoffset */
                                      pcre2_option,                 /* options */
                                      marpaESLIF_regex.match_datap, /* match data */
                                      NULL                          /* match context - used default */
                                      );
#ifdef PCRE2_CONFIG_JIT
    }
#endif

    /* In any case - set UTF buffer correctness if needed and if possible */
    if (binmodeb && marpaESLIF_regex.utfb) {
      if ((pcre2Errornumberi >= 0) || (pcre2Errornumberi == PCRE2_ERROR_NOMATCH)) {
        /* Either regex is successful, either it failed with the accepted failure code PCRE2_ERROR_NOMATCH */
        MARPAESLIF_TRACEF(marpaESLIFp, funcs, "%s: UTF-8 correctess successful and remembered", terminalp->descp->asciis);
        *(marpaESLIFRecognizerp->utfbp) = 1;
      }
    }

    if (eofb) {
      if (pcre2Errornumberi < 0) {
        /* Only PCRE2_ERROR_NOMATCH is an acceptable error. */
        if (pcre2Errornumberi != PCRE2_ERROR_NOMATCH) {
          pcre2_get_error_message(pcre2Errornumberi, pcre2ErrorBuffer, sizeof(pcre2ErrorBuffer));
          MARPAESLIF_WARNF(marpaESLIFp, "%s: Uncaught pcre2 match failure: %s", terminalp->descp->asciis, pcre2ErrorBuffer);
        }
        rci = MARPAESLIF_MATCH_FAILURE;
      } else {
        /* Check the length of matched data */
        if (pcre2_get_ovector_count(marpaESLIF_regex.match_datap) <= 0) {
          MARPAESLIF_ERRORF(marpaESLIFp, "%s: pcre2_get_ovector_count returned no number of pairs of values", terminalp->descp->asciis);
          goto err;
        }
        pcre2_ovectorp = pcre2_get_ovector_pointer(marpaESLIF_regex.match_datap);
        if (pcre2_ovectorp == NULL) {
          MARPAESLIF_ERRORF(marpaESLIFp, "%s: pcre2_get_ovector_pointer returned NULL", terminalp->descp->asciis);
          goto err;
        }
        /* We said PCRE2_NOTEMPTY so this cannot be empty */
        matchedLengthl = pcre2_ovectorp[1] - pcre2_ovectorp[0];
        if (matchedLengthl <= 0) {
          MARPAESLIF_ERRORF(marpaESLIFp, "%s: Empty match when it is configured as not possible", terminalp->descp->asciis);
          goto err;
        }
        /* Very good -; */
        rci = MARPAESLIF_MATCH_OK;
      }
    } else {
      if (pcre2Errornumberi >= 0) {
        /* Full match is successful. */
        /* Check the length of matched data */
        if (pcre2_get_ovector_count(marpaESLIF_regex.match_datap) <= 0) {
          MARPAESLIF_ERRORF(marpaESLIFp, "%s: pcre2_get_ovector_count returned no number of pairs of values", terminalp->descp->asciis);
          goto err;
        }
        pcre2_ovectorp = pcre2_get_ovector_pointer(marpaESLIF_regex.match_datap);
        if (pcre2_ovectorp == NULL) {
          MARPAESLIF_ERRORF(marpaESLIFp, "%s: pcre2_get_ovector_pointer returned NULL", terminalp->descp->asciis);
          goto err;
        }
        /* We said PCRE2_NOTEMPTY so this cannot be empty */
        matchedLengthl = pcre2_ovectorp[1] - pcre2_ovectorp[0];
        if (matchedLengthl <= 0) {
          MARPAESLIF_ERRORF(marpaESLIFp, "%s: Empty match when it is configured as not possible", terminalp->descp->asciis);
          goto err;
        }
        if (matchedLengthl >= inputl) {
          /* But end of the buffer is reached, and we are not at the eof! We have to ask for more bytes. */
          rci = MARPAESLIF_MATCH_AGAIN;
        } else {
          /* And end of the buffer is not reached */
          rci = MARPAESLIF_MATCH_OK;
        }
      } else {
        /* Do a partial match. This section cannot return MARPAESLIF_MATCH_OK. */
        /* Please note that we NEVER ask for UTF-8 correctness here, because previous section */
        /* made sure it has always been done. */
#ifdef PCRE2_CONFIG_JIT
        if (marpaESLIF_regex.jitPartialb) {
          pcre2Errornumberi = pcre2_jit_match(marpaESLIF_regex.patternp,    /* code */
                                              (PCRE2_SPTR) inputs,          /* subject */
                                              (PCRE2_SIZE) inputl,          /* length */
                                              (PCRE2_SIZE) 0,               /* startoffset */
                                              pcre2_option_partial_default, /* options - this one is supported in JIT mode */
                                              marpaESLIF_regex.match_datap, /* match data */
                                              NULL                          /* match context - used default */
                                              );
          if (pcre2Errornumberi == PCRE2_ERROR_JIT_STACKLIMIT) {
            /* Back luck, out of stack for JIT */
            pcre2_get_error_message(pcre2Errornumberi, pcre2ErrorBuffer, sizeof(pcre2ErrorBuffer));
            goto eof_nojitpartial;
          }
        } else {
        eof_nojitpartial:
#endif
          pcre2Errornumberi = pcre2_match(marpaESLIF_regex.patternp,    /* code */
                                          (PCRE2_SPTR) inputs,          /* subject */
                                          (PCRE2_SIZE) inputl,          /* length */
                                          (PCRE2_SIZE) 0,               /* startoffset */
                                          pcre2_option_partial_default, /* options - this one is supported in JIT mode */
                                          marpaESLIF_regex.match_datap, /* match data */
                                          NULL                          /* match context - used default */
                                          );
#ifdef PCRE2_CONFIG_JIT
        }
#endif
        /* Only PCRE2_ERROR_PARTIAL is an acceptable error */
        if (pcre2Errornumberi == PCRE2_ERROR_PARTIAL) {
          /* Partial match is successful */
          rci = MARPAESLIF_MATCH_AGAIN;
        } else {
          /* Partial match is not successful */
          rci = MARPAESLIF_MATCH_FAILURE;
        }
      }
    }
  } else {
    rci = eofb ? MARPAESLIF_MATCH_FAILURE : MARPAESLIF_MATCH_AGAIN;
  }

  if (rcip != NULL) {
    *rcip = rci;
  }

  if (rci == MARPAESLIF_MATCH_OK) {
    if (outputStackp != NULL) {
      tmps = (char *) malloc(matchedLengthl);
      if (tmps == NULL) {
        MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
        goto err;
      }
      memcpy(tmps, inputs, matchedLengthl);
      if (! _marpaESLIFRecognizer_lexemeStack_i_setb(marpaESLIFRecognizerp, outputStackp, GENERICSTACK_USED(outputStackp), (void *) tmps, matchedLengthl)) {
        goto err;
      }
      tmps = NULL;
    }
  }

  rcb = 1;
  goto done;

 err:
  /* Only here it is possible that tmps is != NULL */
  if (tmps != NULL) {
    free(tmps);
  }
  rcb = 0;

 done:
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_meta_matcherb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, int grammarLeveli, marpaWrapperGrammar_t *marpaWrapperGrammarp, marpaESLIF_terminal_t *terminalp, marpaESLIF_meta_t *metap, marpaESLIF_matcher_value_t *rcip, genericStack_t *outputStackp, short *exhaustedbp)
/*****************************************************************************/
{
  /* All in all, this routine is the core of this module, and the cause of recursion -; */
  const static char            *funcs              = "_marpaESLIFRecognizer_meta_matcherb";
  marpaESLIF_t                 *marpaESLIFp        = marpaESLIFRecognizerp->marpaESLIFp;
  marpaESLIFGrammar_t          *marpaESLIFGrammarp = marpaESLIFRecognizerp->marpaESLIFGrammarp;
  genericStack_t               *localOutputStackp  = NULL;
  short                         rcb;
  marpaESLIFGrammar_t           marpaESLIFGrammar; /* Fake marpaESLIFGrammar with the grammar sent in the stack */
  marpaESLIF_grammar_t          grammar;
  marpaESLIF_grammar_t         *grammarp;
  marpaESLIFRecognizerOption_t  marpaESLIFRecognizerOption; /* This is an internal recognizer */
  marpaESLIFValueOption_t       marpaESLIFValueOption = marpaESLIFValueOption_default_template;
  marpaESLIF_valueContext_t     valueContext;
  void                         *p;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");
  
  /* A meta matcher is always using ANOTHER grammar at level grammarLeveli (validator guaranteed that is exists) that is sent on the stack. */
  /* Though the precomputed grammar is known to the symbol that called us, also sent on the stack. */
  if (! GENERICSTACK_IS_PTR(marpaESLIFGrammarp->grammarStackp, grammarLeveli)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "At grammar No %d (%s), meta symbol No %d (%s) resolve to a grammar level %d that do not exist", marpaESLIFGrammarp->grammarp->leveli, marpaESLIFGrammarp->grammarp->descp->asciis, metap->idi, metap->descp->asciis, grammarLeveli);
    goto err;
  }
  grammarp                           = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(marpaESLIFGrammarp->grammarStackp, grammarLeveli);
  grammar                            = *grammarp;
  grammar.marpaWrapperGrammarStartp  = marpaWrapperGrammarp;
  marpaESLIFGrammar                  = *marpaESLIFGrammarp;
  marpaESLIFGrammar.grammarp         = &grammar;

  marpaESLIFRecognizerOption                   = marpaESLIFRecognizerp->marpaESLIFRecognizerOption;
  marpaESLIFRecognizerOption.disableThresholdb = 1;
  marpaESLIFRecognizerOption.exhaustedb        = 1;
  marpaESLIFRecognizerOption.latmb             = 1;

  valueContext.marpaESLIFGrammarp = &marpaESLIFGrammar;
  localOutputStackp = &(valueContext.outputStack);
  GENERICSTACK_INIT(localOutputStackp);
  if (GENERICSTACK_ERROR(localOutputStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "localOutputStackp initialization failure, %s", strerror(errno));
    localOutputStackp = NULL;
    goto err;
  }

  marpaESLIFValueOption.userDatavp       = (void *) &valueContext; /* Take care: internal mode i.e. callbacks are called with marpaESLIFValuep */
  marpaESLIFValueOption.ruleCallbackp    = _marpaESLIFValueRuleCallbackInternalLexeme;
  marpaESLIFValueOption.symbolCallbackp  = _marpaESLIFValueSymbolCallbackInternalLexeme;
  marpaESLIFValueOption.nullingCallbackp = _marpaESLIFValueNullingCallbackInternalLexeme;

  if (! _marpaESLIFGrammar_parseb(&marpaESLIFGrammar, &marpaESLIFRecognizerOption, &marpaESLIFValueOption, 0 /* discardb */, marpaESLIFRecognizerp, exhaustedbp, 1 /* internalb */)) {
    goto err;
  }

  /* We know the result is at indice 0 */
  if (! _marpaESLIFRecognizer_lexemeStack_i_moveb(marpaESLIFRecognizerp, outputStackp, GENERICSTACK_USED(outputStackp), localOutputStackp, 0)) {
    goto err;
  }
  
  *rcip = MARPAESLIF_MATCH_OK;
  rcb = 1;
  goto done;
  
 err:
  rcb = 0;

 done:
  _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizerp, localOutputStackp);
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_symbol_matcherb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIF_symbol_t *symbolp, marpaESLIF_matcher_value_t *rcip, genericStack_t *outputStackp, short *exhaustedbp)
/*****************************************************************************/
{
  const static char *funcs       = "_marpaESLIFRecognizer_symbol_matcherb";
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  switch (symbolp->type) {
  case MARPAESLIF_SYMBOL_TYPE_TERMINAL:
    /* A terminal matcher NEVER updates the stream : inputs, inputl and eof can be passed as is. */
    rcb = _marpaESLIFRecognizer_regex_matcherb(marpaESLIFRecognizerp,
                                               symbolp->u.terminalp,
                                               marpaESLIFRecognizerp->inputs,
                                               marpaESLIFRecognizerp->inputl,
                                               *(marpaESLIFRecognizerp->eofbp),
                                               rcip,
                                               outputStackp);
      break;
    case MARPAESLIF_SYMBOL_TYPE_META:
      /* A terminal matcher MAY recursively call other recognizers, reading new data, etc... : this will update current recognizer inputs, inputl and eof. */
      /* The result will be a parse tree value, at indice 0 of outputStackp */
      rcb = _marpaESLIFRecognizer_meta_matcherb(marpaESLIFRecognizerp,
                                                symbolp->resolvedLeveli, /* Computed by grammar validator */
                                                symbolp->u.metap->marpaWrapperGrammarClonep,
                                                NULL /* terminalp */,
                                                symbolp->u.metap,
                                                rcip,
                                                outputStackp,
                                                exhaustedbp);
      break;
    default:
      MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "Unknown symbol type %d", symbolp->type);
      goto err;
    }

#ifndef MARPAESLIF_NTRACE
  if (rcb) {
    if (*rcip == MARPAESLIF_MATCH_OK) {
      size_t sizel;
      char  *p;

      if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, outputStackp, 0, &sizel)) {
        goto err;
      }
      if (! _marpaESLIFRecognizer_lexemeStack_i_p(marpaESLIFRecognizerp, outputStackp, 0, &p)) {
        goto err;
      }

      if (sizel > 0) {
        _marpaESLIFRecognizer_hexdumpv(marpaESLIFRecognizerp, "Match for ", symbolp->descp->asciis, p, sizel, 1 /* traceb */);
      } else {
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Match for %s is the empty string", symbolp->descp->asciis);
      }
    }
  }
#endif

  goto done;

 err:
  rcb = 0;

 done:
  /*
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  */
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

#ifndef MARPAESLIF_NTRACE
/*****************************************************************************/
static void _marpaESLIF_tconvTraceCallback(void *userDatavp, const char *msgs)
/*****************************************************************************/
{
  const static char *funcs  = "_marpaESLIF_tconvTraceCallback";
  marpaESLIF_t *marpaESLIFp = (marpaESLIF_t *) userDatavp;

  MARPAESLIF_TRACEF(marpaESLIFp, funcs, "%s", msgs);
}
#endif

/*****************************************************************************/
static inline char *_marpaESLIF_utf82printableascii_newp(marpaESLIF_t *marpaESLIFp, char *descs, size_t descl)
/*****************************************************************************/
{
  const static char      *funcs  = "_marpaESLIF_utf82printableascii_newp";
  size_t                  asciil;
  char                   *p;
  char                   *asciis;
  unsigned char           c;

  asciis = _marpaESLIF_charconvp(marpaESLIFp, "ASCII//TRANSLIT//IGNORE", "UTF-8", descs, descl, &asciil, NULL);
  if (asciis == NULL) {
    asciis = (char *) _marpaESLIF_utf82printableascii_defaultp;
    asciil = strlen(asciis);
  } else {
    /* We are doing this only on descriptions - which are always small amount of bytes  */
    /* (will the user ever write a description taking megabytes !?). Therefore if it ok */
    /* to remove by hand bom and realloc if necessary.                                  */

    /* Remove by hand any ASCII character not truely printable.      */
    /* Only the historical ASCII table [0-127] is a portable thingy. */
    p = asciis;
    while ((c = (unsigned char) *p) != '\0') {
      if ((c >= 128) || (! isprint(c & 0xFF))) {
        *p = ' ';
      }
      p++;
    }
  }

  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return \"%s\"", asciis); */
  return asciis;
}

/*****************************************************************************/
static inline void _marpaESLIF_utf82printableascii_freev(char *asciis)
/*****************************************************************************/
{
  if ((asciis != NULL) && (asciis != _marpaESLIF_utf82printableascii_defaultp)) {
    free(asciis);
  }
}

/*****************************************************************************/
static inline char *_marpaESLIF_charconvp(marpaESLIF_t *marpaESLIFp, char *toEncodings, char *fromEncodings, char *srcs, size_t srcl, size_t *dstlp, char **fromEncodingsp)
/*****************************************************************************/
{
  const static char *funcs       = "_marpaESLIF_utf8_newp";
  tconv_t            tconvp      = NULL;
  char              *inbuforigp  = srcs;
  size_t             inleftorigl = srcl;
  char              *outbuforigp = NULL;
  size_t             outbuforigl = 0;
  tconv_option_t     tconvOption = { NULL /* charsetp */, NULL /* convertp */, NULL /* traceCallbackp */, NULL /* traceUserDatavp */ };
  char              *rcp;
  char              *inbufp;
  size_t             inleftl;
  char              *outbufp;
  size_t             outleftl;
  size_t             nconvl;

  /* ------- Our input is always a well formed UTF-8 */
#ifndef MARPAESLIF_NTRACE
  tconvOption.traceCallbackp  = _marpaESLIF_tconvTraceCallback;
  tconvOption.traceUserDatavp = marpaESLIFp;
#endif
  tconvp = tconv_open_ext(toEncodings, fromEncodings, &tconvOption);
  if (tconvp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "tconv_open failure, %s", strerror(errno));
    goto err;
  }
#ifndef MARPAESLIF_NTRACE
  tconv_trace_on(tconvp);
#endif

  /* We start with an output buffer of the same size of input buffer.                  */
  /* Whatever the destination encoding, we always reserve one byte more to place a NUL */
  /* just in case. This NUL is absolutetly harmless but is usefull if one want to look */
  /* at the variables via a debugger -;.                                               */
  /* It is more than useful when the destination encoding is ASCII: string will be NUL */
  /* terminated by default.                                                            */
  outbuforigp = (char *) malloc(srcl + 1);
  if (outbuforigp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }
  outbuforigp[srcl] = '\0';
  outbuforigl = srcl;

  /* We want to translate descriptions in trace or error cases - these are short things, and */
  /* it does not really harm if we redo the whole translation stuff in case of E2BIG:        */
  /* - in trace mode it is documented that performance is awful                              */
  /* - in error mode this call will happen once                                              */

  inbufp   = inbuforigp;
  inleftl  = inleftorigl;
  outbufp  = outbuforigp;
  outleftl = outbuforigl;
  
  while (1) {

    nconvl = tconv(tconvp, (inbufp != NULL) ? &inbufp : NULL, &inleftl, &outbufp, &outleftl);

    if (nconvl == (size_t) -1) {
      char  *tmp;
      size_t deltal = outbuforigl;
      
      /* The only acceptable error is E2BIG */
      if (errno != E2BIG) {
	MARPAESLIF_ERRORF(marpaESLIFp, "tconv failure, %s", strerror(errno));
	goto err;
      }
      /* Try to alloc more */
      outbuforigl *= 2;
      /* Will this ever happen ? */
      if (outbuforigl < srcl) {
	MARPAESLIF_ERROR(marpaESLIFp, "size_t flip");
	goto err;
      }
      /* Note the "+ 1" */
      tmp = realloc(outbuforigp, outbuforigl + 1);
      if (tmp == NULL) {
	MARPAESLIF_ERRORF(marpaESLIFp, "realloc failure, %s", strerror(errno));
	goto err;
      }
      outbuforigp[outbuforigl] = '\0';
      outbuforigp = tmp;
      outleftl   += deltal;
    }

    if (inbufp == NULL) {
      /* This was the last round */
      break;
    }

    if (inleftl <= 0) {
      /* Next round is the last one */
      inbufp = NULL;
    }
  }

  rcp = outbuforigp;
  if (dstlp != NULL) {
    *dstlp = outbuforigl;
  }
  if (fromEncodingsp != NULL) {
    if (fromEncodings != NULL) {
      *fromEncodingsp = strdup(fromEncodings);
      if (*fromEncodingsp == NULL) {
	MARPAESLIF_ERRORF(marpaESLIFp, "strdup failure, %s", strerror(errno));
	goto err;
      }
    } else {
      /* Get the guess from tconv */
      *fromEncodingsp = tconv_fromcode(tconvp);
      if (*fromEncodingsp == NULL) {
        /* Should never happen */
	MARPAESLIF_ERROR(marpaESLIFp, "tconv returned a NULL origin encoding");
        errno = EINVAL;
	goto err;
      }
      MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Encoding guessed to %s", *fromEncodingsp);
      /* We do not mind if we loose the original - it is inside tconv that will be freed */
      *fromEncodingsp = strdup(*fromEncodingsp);
      if (*fromEncodingsp == NULL) {
	MARPAESLIF_ERRORF(marpaESLIFp, "strdup failure, %s", strerror(errno));
	goto err;
      }
    }
  }
  goto done;

 err:
  rcp = NULL;

 done:
  if (tconvp != NULL) {
    if (tconv_close(tconvp) != 0) {
      MARPAESLIF_ERRORF(marpaESLIFp, "tconv_close failure, %s", strerror(errno));
    }
  }

  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", rcp); */
  return rcp;
}

/*****************************************************************************/
marpaESLIFGrammar_t *marpaESLIFGrammar_newp(marpaESLIF_t *marpaESLIFp, marpaESLIFGrammarOption_t *marpaESLIFGrammarOptionp)
/*****************************************************************************/
{
  const static char                *funcs              = "marpaESLIFGrammar_newp";
  genericStack_t                   *localOutputStackp  = NULL;
  marpaESLIFGrammar_t              *marpaESLIFGrammarp = NULL;
  marpaESLIF_readerContext_t        marpaESLIF_readerContext;
  marpaESLIFRecognizerOption_t      marpaESLIFRecognizerOption;
  marpaESLIFValueOption_t           marpaESLIFValueOption = marpaESLIFValueOption_default_template;
  marpaESLIF_valueContext_t         valueContext;

  /* MARPAESLIF_TRACE(marpaESLIFp, funcs, "Building Grammar"); */

  if (marpaESLIFGrammarOptionp == NULL) {
    marpaESLIFGrammarOptionp = &marpaESLIFGrammarOption_default;
  }

  if (marpaESLIFGrammarOptionp->grammars == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, funcs, "Null source grammar pointer");
    goto err;
  }

  marpaESLIFGrammarp = (marpaESLIFGrammar_t *) malloc(sizeof(marpaESLIFGrammar_t));
  if (marpaESLIFGrammarp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  marpaESLIFGrammarp->marpaESLIFp   = marpaESLIFp;
  marpaESLIFGrammarp->grammarStackp = NULL;
  marpaESLIFGrammarp->grammarp      = NULL;

  GENERICSTACK_NEW(marpaESLIFGrammarp->grammarStackp);
  if (GENERICSTACK_ERROR(marpaESLIFGrammarp->grammarStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "marpaESLIFGrammarp->grammarStackp initialization failure, %s", strerror(errno));
    goto err;
  }

  /* Our internal grammar reader callback */
  marpaESLIF_readerContext.marpaESLIFp = marpaESLIFp;
  marpaESLIF_readerContext.marpaESLIFGrammarOptionp = marpaESLIFGrammarOptionp;

  marpaESLIFRecognizerOption.userDatavp                  = (void *) &marpaESLIF_readerContext;
  marpaESLIFRecognizerOption.marpaESLIFReaderCallbackp   = _marpaESLIFReader_grammarReader;
  marpaESLIFRecognizerOption.disableThresholdb           = 1; /* No threshold warning when parsing a grammar */
  marpaESLIFRecognizerOption.exhaustedb                  = 0; /* Exhaustion is not allowed */
  marpaESLIFRecognizerOption.latmb                       = 1; /* Our internal grammar is working in latm mode */
  marpaESLIFRecognizerOption.rejectionb                  = 1; /* Our internal grammar have no rejection */
  marpaESLIFRecognizerOption.newlineb                    = 1; /* Grammars are short - we can count line/columns numbers */

  valueContext.marpaESLIFGrammarp = marpaESLIFGrammarp;
  localOutputStackp = &(valueContext.outputStack);
  GENERICSTACK_INIT(localOutputStackp);
  if (GENERICSTACK_ERROR(localOutputStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "localOutputStackp initialization failure, %s", strerror(errno));
    localOutputStackp = NULL;
    goto err;
  }

  marpaESLIFValueOption.userDatavp       = (void *) &valueContext; /* Take care: internal mode i.e. callbacks are called with marpaESLIFValuep */
  marpaESLIFValueOption.ruleCallbackp    = _marpaESLIFValueRuleCallbackInternalGrammar;
  marpaESLIFValueOption.symbolCallbackp  = _marpaESLIFValueSymbolCallbackInternalGrammar;
  marpaESLIFValueOption.nullingCallbackp = _marpaESLIFValueNullingCallbackInternalGrammar;

  /* Parser will automatically create marpaESLIFValuep */
  if (! _marpaESLIFGrammar_parseb(marpaESLIFp->marpaESLIFGrammarp, &marpaESLIFRecognizerOption, &marpaESLIFValueOption, 0 /* discardb */, NULL /* marpaESLIFRecognizerParentp */, NULL /* exhaustedbp */, 1 /* internalb */)) {
    goto err;
  }

  /* Per def result is in marpaESLIFGrammar.outputStackp at indice 0, itself beeing a grammar instance -; */
  if (! GENERICSTACK_IS_PTR(localOutputStackp, 0)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "Not a pointer in localOutputStackp at indice 0 " MARPAESLIF_LOC_FMT, MARPAESLIF_LOC_VAR);
    goto err;
  }

  marpaESLIFGrammarp = GENERICSTACK_GET_PTR(localOutputStackp, 0);
  /* This case is vicious: we want to free everything but the first item on the outputstack... */
  GENERICSTACK_SET_NA(localOutputStackp, 0);
  if (GENERICSTACK_ERROR(localOutputStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "localOutputStackp set failure, %s", strerror(errno));
    goto err;
  }
  goto done;

 err:
  marpaESLIFGrammar_freev(marpaESLIFGrammarp);
  marpaESLIFGrammarp = NULL;

 done:
  _marpaESLIF_grammarStack_resetv(localOutputStackp);
  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", marpaESLIFGrammarp); */
  return marpaESLIFGrammarp;
}

/*****************************************************************************/
short marpaESLIFGrammar_startib(marpaESLIFGrammar_t *marpaESLIFGrammarp, int *startip)
/*****************************************************************************/
{
  return marpaESLIFGrammar_starti_by_grammarb(marpaESLIFGrammarp, startip, NULL, 0, 0);
}

/*****************************************************************************/
short marpaESLIFGrammar_starti_by_grammarb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int *startip, char *grammardescs, size_t grammardescl, int grammarleveli)
/*****************************************************************************/
{
  marpaESLIF_grammar_t *grammarp;
  short                 rcb;

  grammarp = _marpaESLIFGrammar_grammar_findp(marpaESLIFGrammarp, grammardescs, grammardescl, grammarleveli);
  if (grammarp == NULL) {
    goto err;
  }

  if (startip != NULL) {
    *startip = grammarp->starti;
  }
  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
short marpaESLIFGrammar_rulesb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int **ruleipp, size_t *rulelp)
/*****************************************************************************/
{
  return marpaESLIFGrammar_rules_by_grammarb(marpaESLIFGrammarp, ruleipp, rulelp, NULL, 0, 0);
}

/*****************************************************************************/
short marpaESLIFGrammar_rules_by_grammarb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int **ruleipp, size_t *rulelp, char *grammardescs, size_t grammardescl, int grammarleveli)
/*****************************************************************************/
{
  marpaESLIF_grammar_t *grammarp;
  short                 rcb;

  grammarp = _marpaESLIFGrammar_grammar_findp(marpaESLIFGrammarp, grammardescs, grammardescl, grammarleveli);
  if (grammarp == NULL) {
    goto err;
  }

  if (ruleipp != NULL) {
    *ruleipp = grammarp->ruleip;
  }
  if (rulelp != NULL) {
    *rulelp = grammarp->rulel;
  }
  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
short marpaESLIFGrammar_rulenameb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int rulei, char **ruledecsp, size_t *ruledesclp)
/*****************************************************************************/
{
  return marpaESLIFGrammar_rulename_by_grammarb(marpaESLIFGrammarp, rulei, ruledecsp, ruledesclp, NULL, 0, 0);
}

/*****************************************************************************/
short marpaESLIFGrammar_rulename_by_grammarb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int rulei, char **ruledecsp, size_t *ruledesclp, char *grammardescs, size_t grammardescl, int grammarleveli)
/*****************************************************************************/
{
  marpaESLIF_grammar_t *grammarp;
  marpaESLIF_rule_t    *rulep;
  short                 rcb;
  marpaESLIF_string_t  *stringp;

  grammarp = _marpaESLIFGrammar_grammar_findp(marpaESLIFGrammarp, grammardescs, grammardescl, grammarleveli);
  if (grammarp == NULL) {
    goto err;
  }
  rulep = _marpaESLIF_rule_findp(marpaESLIFGrammarp->marpaESLIFp, grammarp, NULL, 0, rulei);
  if (rulep == NULL) {
    goto err;
  }
  stringp = rulep->descp;
 
  if (ruledecsp != NULL) {
    *ruledecsp = stringp->bytep;
  }
  if (ruledesclp != NULL) {
    *ruledesclp = stringp->bytel;
  }
  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
short marpaESLIFGrammar_ruledisplayformb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int rulei, char **ruledisplaysp)
/*****************************************************************************/
{
  return marpaESLIFGrammar_ruledisplayform_by_grammarb(marpaESLIFGrammarp, rulei, ruledisplaysp, NULL, 0, 0);
}

/*****************************************************************************/
short marpaESLIFGrammar_ruledisplayform_by_grammarb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int rulei, char **ruledisplaysp, char *grammardescs, size_t grammardescl, int grammarleveli)
/*****************************************************************************/
{
  marpaESLIF_grammar_t *grammarp;
  marpaESLIF_rule_t    *rulep;
  short                 rcb;

  grammarp = _marpaESLIFGrammar_grammar_findp(marpaESLIFGrammarp, grammardescs, grammardescl, grammarleveli);
  if (grammarp == NULL) {
    goto err;
  }
  rulep = _marpaESLIF_rule_findp(marpaESLIFGrammarp->marpaESLIFp, grammarp, NULL, 0, rulei);
  if (rulep == NULL) {
    goto err;
  }
 
  if (ruledisplaysp != NULL) {
    *ruledisplaysp = rulep->descp->asciis;
  }
  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
short marpaESLIFGrammar_ruleshowformb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int rulei, char **ruleshowsp)
/*****************************************************************************/
{
  return marpaESLIFGrammar_ruleshowform_by_grammarb(marpaESLIFGrammarp, rulei, ruleshowsp, NULL, 0, 0);
}

/*****************************************************************************/
short marpaESLIFGrammar_ruleshowform_by_grammarb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int rulei, char **ruleshowsp, char *grammardescs, size_t grammardescl, int grammarleveli)
/*****************************************************************************/
{
  marpaESLIF_grammar_t *grammarp;
  marpaESLIF_rule_t    *rulep;
  short                 rcb;

  grammarp = _marpaESLIFGrammar_grammar_findp(marpaESLIFGrammarp, grammardescs, grammardescl, grammarleveli);
  if (grammarp == NULL) {
    goto err;
  }
  rulep = _marpaESLIF_rule_findp(marpaESLIFGrammarp->marpaESLIFp, grammarp, NULL, 0, rulei);
  if (rulep == NULL) {
    goto err;
  }
 
  if (ruleshowsp != NULL) {
    *ruleshowsp = rulep->asciishows;
  }
  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
short marpaESLIFGrammar_symbolnameb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int symboli, char **symboldecsp, size_t *symboldesclp)
/*****************************************************************************/
{
  return marpaESLIFGrammar_symbolname_by_grammarb(marpaESLIFGrammarp, symboli, symboldecsp, symboldesclp, NULL, 0, 0);
}

/*****************************************************************************/
short marpaESLIFGrammar_symbolname_by_grammarb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int symboli, char **symboldecsp, size_t *symboldesclp, char *grammardescs, size_t grammardescl, int grammarleveli)
/*****************************************************************************/
{
  marpaESLIF_grammar_t *grammarp;
  marpaESLIF_symbol_t  *symbolp;
  short                 rcb;
  marpaESLIF_string_t  *stringp;

  grammarp = _marpaESLIFGrammar_grammar_findp(marpaESLIFGrammarp, grammardescs, grammardescl, grammarleveli);
  if (grammarp == NULL) {
    goto err;
  }
  symbolp = _marpaESLIF_symbol_findp(marpaESLIFGrammarp->marpaESLIFp, grammarp, NULL, 0, symboli);
  if (symbolp == NULL) {
    goto err;
  }
  stringp = symbolp->descp;
  
  if (symboldecsp != NULL) {
    *symboldecsp = stringp->bytep;
  }
  if (symboldesclp != NULL) {
    *symboldesclp = stringp->bytel;
  }
  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
short marpaESLIFGrammar_symboldisplayformb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int symboli, char **symboldisplaysp)
/*****************************************************************************/
{
  return marpaESLIFGrammar_symboldisplayform_by_grammarb(marpaESLIFGrammarp, symboli, symboldisplaysp, NULL, 0, 0);
}

/*****************************************************************************/
short marpaESLIFGrammar_symboldisplayform_by_grammarb(marpaESLIFGrammar_t *marpaESLIFGrammarp, int symboli, char **symboldisplaysp, char *grammardescs, size_t grammardescl, int grammarleveli)
/*****************************************************************************/
{
  marpaESLIF_grammar_t *grammarp;
  marpaESLIF_symbol_t  *symbolp;
  short                 rcb;

  grammarp = _marpaESLIFGrammar_grammar_findp(marpaESLIFGrammarp, grammardescs, grammardescl, grammarleveli);
  if (grammarp == NULL) {
    goto err;
  }
  symbolp = _marpaESLIF_symbol_findp(marpaESLIFGrammarp->marpaESLIFp, grammarp, NULL, 0, symboli);
  if (symbolp == NULL) {
    goto err;
  }
 
  if (symboldisplaysp != NULL) {
    *symboldisplaysp = symbolp->descp->asciis;
  }
  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
marpaESLIFRecognizer_t *marpaESLIFRecognizer_newp(marpaESLIFGrammar_t *marpaESLIFGrammarp, marpaESLIFRecognizerOption_t *marpaESLIFRecognizerOptionp)
/*****************************************************************************/
{
  return _marpaESLIFRecognizer_newp(marpaESLIFGrammarp, marpaESLIFRecognizerOptionp, 0 /* discardb */, NULL /* marpaESLIFRecognizerParentp */, 0 /* fakeb */);
}

/*****************************************************************************/
short marpaESLIFRecognizer_scanb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, short initialEventsb, short *continuebp, short *exhaustedbp)
/*****************************************************************************/
{
  const static char *funcs = "marpaESLIFRecognizer_scanb";
  marpaESLIF_t      *marpaESLIFp = marpaESLIFRecognizerp->marpaESLIFp;
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  if (marpaESLIFRecognizerp->scanb) {
    MARPAESLIF_ERROR(marpaESLIFp, "Scan can be done one once only");
    goto err;
  }

  marpaESLIFRecognizerp->scanb = 1;
  rcb = _marpaESLIFRecognizer_resumeb(marpaESLIFRecognizerp, initialEventsb, continuebp, exhaustedbp);
  goto done;

 err:
  rcb = 0;

 done:
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
short marpaESLIFRecognizer_resumeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, short *continuebp, short *exhaustedbp)
/*****************************************************************************/
{
  const static char *funcs = "marpaESLIFRecognizer_resumeb";
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  rcb = _marpaESLIFRecognizer_resumeb(marpaESLIFRecognizerp, 0 /* initialEventsb */, continuebp, exhaustedbp);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_resumeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, short initialEventsb, short *continuebp, short *exhaustedbp)
/*****************************************************************************/
{
  const static char               *funcs                             = "_marpaESLIFRecognizer_resumeb";
  marpaESLIF_t                    *marpaESLIFp                       = marpaESLIFRecognizerp->marpaESLIFp;
  genericStack_t                  *lexemeInputStackp                 = marpaESLIFRecognizerp->lexemeInputStackp;
  marpaESLIFGrammar_t             *marpaESLIFGrammarp                = marpaESLIFRecognizerp->marpaESLIFGrammarp;
  marpaESLIF_grammar_t            *grammarp                          = marpaESLIFGrammarp->grammarp;
  genericStack_t                  *symbolStackp                      = grammarp->symbolStackp;
  short                            latmb                             = marpaESLIFRecognizerp->marpaESLIFRecognizerOption.latmb;
  short                            exhaustedb                        = 0;
  short                            completedb                        = 0;
  short                            collectb                          = 0;
  marpaESLIFValueOption_t          marpaESLIFValueOptionDiscard      = marpaESLIFValueOption_default_template;
  marpaESLIFRecognizerOption_t     marpaESLIFRecognizerOptionDiscard = marpaESLIFRecognizerp->marpaESLIFRecognizerOption; /* Things overwriten, see below */
  genericStack_t                  *localOutputStackp                 = NULL;
  marpaESLIF_valueContext_t        discardValueContext               = { marpaESLIFGrammarp }; /* Next member is value's output stack, c.f. discard section */
  short                            continueb;
  size_t                           maxMatchedl;
  int                              maxPriorityi;
  short                            maxPriorityInitializedb = 0;
  genericStack_t                   matchedStack;
  genericStack_t                  *matchedStackp = &matchedStack;
  genericStack_t                   alternativeStack;
  genericStack_t                  *alternativeStackp = &alternativeStack;
  int                              alternativeOki;
  size_t                           nSymboll;
  int                             *symbolArrayp;
  size_t                           symboll;
  int                              symboli;
  marpaESLIF_symbol_t             *symbolp;
  marpaESLIF_matcher_value_t       rci;
  size_t                           matchedl;
  short                            rcb;
  size_t                           sizel;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

#undef MARPAESLIFRECOGNIZER_COLLECT_EVENTS
#define MARPAESLIFRECOGNIZER_COLLECT_EVENTS(forceb, exhaustedbp)  do {  \
    if (forceb || ! collectb) {                                         \
      _marpaESLIFRecognizer_reset_eventsb(marpaESLIFRecognizerp);       \
      if (! _marpaESLIFRecognizer_grammar_eventsb(marpaESLIFRecognizerp, NULL, NULL, NULL, exhaustedbp)) { \
        goto err;                                                       \
      }                                                                 \
      collectb = 1;                                                     \
    }                                                                   \
  } while (0)

  /* Checks */
  if (! marpaESLIFRecognizerp->scanb) {
    MARPAESLIF_ERROR(marpaESLIFp, "Scan must be called first");
    goto err;
  }

  /* Initializations */
  GENERICSTACK_INIT(alternativeStackp);
  if (GENERICSTACK_ERROR(alternativeStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "alternativeStackp init failure, %s", strerror(errno));
    alternativeStackp = NULL;
    goto err;
  }
  GENERICSTACK_INIT(matchedStackp);
  if (GENERICSTACK_ERROR(matchedStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "matchedStackp init failure, %s", strerror(errno));
    matchedStackp = NULL;
    goto err;
  }

  marpaESLIFRecognizerp->resumeCounteri++; /* Increment internal counter for tracing */
  marpaESLIFRecognizerOptionDiscard.disableThresholdb = 1; /* If discard, prepare the option to disable threshold */
  marpaESLIFRecognizerOptionDiscard.exhaustedb        = 1; /* ... and have the exhausted event */
  marpaESLIFRecognizerOptionDiscard.newlineb          = 0; /* ... and not count line/column numbers */

  if (initialEventsb) {
    MARPAESLIFRECOGNIZER_COLLECT_EVENTS(0, &exhaustedb);
    if (marpaESLIFRecognizerp->eventArrayl > 0) {
      continueb = ! exhaustedb;
      rcb = 1;
      goto done;
    }
  }
  
  /* Ask for expected lexemes */
  if (! marpaESLIFRecognizer_expectedb(marpaESLIFRecognizerp, &nSymboll, &symbolArrayp)) {
    goto err;
  }

  if (nSymboll <= 0) {
    /* No symbol expected: this is an error unless:
       - discard mode and completion is reached, or
       - exhaustion
    */
    MARPAESLIFRECOGNIZER_COLLECT_EVENTS(0, &exhaustedb);
    if ((marpaESLIFRecognizerp->discardb && completedb) || exhaustedb) {
      continueb = 0;
      rcb = 1;
      goto done;
    }
    goto err;
  }

  /* Try to match */
  alternativeOki = 0;
  maxMatchedl = 0;
  retry:
  for (symboll = 0; symboll < nSymboll; symboll++) {
    symboli = symbolArrayp[symboll];
    if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "No such symbol ID %d", symboli);
      goto err;
    }
    symbolp = GENERICSTACK_GET_PTR(symbolStackp, symboli);
    MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Trying to match %s", symbolp->descp->asciis);
  match_again:
    _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizerp, matchedStackp);
    if (! _marpaESLIFRecognizer_symbol_matcherb(marpaESLIFRecognizerp, symbolp, &rci, matchedStackp, &exhaustedb)) {
      if (! _marpaESLIFRecognizer_lexemeStack_i_setb(marpaESLIFRecognizerp, alternativeStackp, (int) symboll, NULL, 0)) {
        goto err;
      } else {
        continue;
      }
    }
    switch (rci) {
    case MARPAESLIF_MATCH_AGAIN:
      /* We have to load more unless already at EOF */
      if (! *(marpaESLIFRecognizerp->eofbp)) {
        if (! _marpaESLIFRecognizer_readb(marpaESLIFRecognizerp)) {
          goto err;
        } else {
          goto match_again;
        }
      } else {
        if (! _marpaESLIFRecognizer_lexemeStack_i_setb(marpaESLIFRecognizerp, alternativeStackp, (int) symboll, NULL, 0)) {
          goto err;
        }
      }
      break;
    case MARPAESLIF_MATCH_FAILURE:
      if (! _marpaESLIFRecognizer_lexemeStack_i_setb(marpaESLIFRecognizerp, alternativeStackp, (int) symboll, NULL, 0)) {
        goto err;
      }
      break;
    case MARPAESLIF_MATCH_OK:
      if (! _marpaESLIFRecognizer_lexemeStack_i_moveb(marpaESLIFRecognizerp, alternativeStackp, (int) symboll, matchedStackp, 0)) {
        goto err;
      }
      if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, alternativeStackp, (int) symboll, &matchedl)) {
        goto err;
      }
      /* Remember max matched length */
      if (matchedl > maxMatchedl) {
        maxMatchedl = matchedl;
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Setted maxMatchedl to %ld", (unsigned long) maxMatchedl);
      }
      /* Count */
      ++alternativeOki;
      break;
    default:
      MARPAESLIF_ERRORF(marpaESLIFp, "Unsupported matcher return code %d", rci);
      goto err;
    }
  }
  
  if (alternativeOki <= 0) {
    /* If we already doing discard and discard failed, this is end of the story */
    if (marpaESLIFRecognizerp->discardb) {
      goto err;
    }
    if (grammarp->marpaWrapperGrammarDiscardp != NULL) {
      /* localOutputStackp is initialized only if needed */
      if (localOutputStackp == NULL) {
        localOutputStackp = &(discardValueContext.outputStack);
        GENERICSTACK_INIT(localOutputStackp);
        if (GENERICSTACK_ERROR(localOutputStackp)) {
          MARPAESLIF_ERRORF(marpaESLIFp, "localOutputStackp initialization faillure, %s", strerror(errno));
          localOutputStackp = NULL;
          goto err;
        }
      } else {
        _marpaESLIF_grammarStack_resetv(localOutputStackp);
      }
      marpaESLIFValueOptionDiscard.userDatavp       = (void *) &discardValueContext; /* Take care: internal mode i.e. callbacks are called with marpaESLIFValuep */
      marpaESLIFValueOptionDiscard.ruleCallbackp    = _marpaESLIFValueRuleCallbackInternalLexeme;
      marpaESLIFValueOptionDiscard.symbolCallbackp  = _marpaESLIFValueSymbolCallbackInternalLexeme;
      marpaESLIFValueOptionDiscard.nullingCallbackp = _marpaESLIFValueNullingCallbackInternalLexeme;
      if (_marpaESLIFGrammar_parseb(marpaESLIFGrammarp, &marpaESLIFRecognizerOptionDiscard, &marpaESLIFValueOptionDiscard, 1 /* discardb */, marpaESLIFRecognizerp /* marpaESLIFRecognizerParentp */, NULL /* exhaustedbp */, 1 /* internalb */)) {
        /* Discard symbol got matched. Get the length. */
        if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, localOutputStackp, 0, &matchedl)) {
          goto err;
        }
        /* In the exceptional case where discard would be a nulling, matchedl can be 0. */
        if (matchedl <= 0) {
          MARPAESLIF_ERROR(marpaESLIFp, "Discard rule that was matched is a nulling and this is not allowed");
          goto err;
        }
        /* New line processing, etc... */
        if (! _marpaESLIFRecognizer_matchPostProcessingb(marpaESLIFRecognizerp, matchedl)) {
          goto err;
        }
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Advancing stream internal position by %ld bytes", (unsigned long) matchedl);
        marpaESLIFRecognizerp->inputs += matchedl;
        marpaESLIFRecognizerp->inputl -= matchedl;
        /* Well, we KNOW that no lexeme matched. This mean that alternative stack contains only N/A's: no need to free anything */
        GENERICSTACK_USED(alternativeStackp) = 0;
        goto retry;
      }
    }

    /* Discard failure - this is an error unless lexemes were read and:
       - exhaustion is on, or
       - eof flags is true and all the data is consumed */
    if (marpaESLIFRecognizerp->haveLexemeb && (marpaESLIFRecognizerp->marpaESLIFRecognizerOption.exhaustedb || (*(marpaESLIFRecognizerp->eofbp) && (marpaESLIFRecognizerp->inputl <= 0)))) {
      continueb = 0;
      rcb = 1;
      goto done;
    } else {
      rcb = 0;
      goto err;
    }
  }

  /* Generate the eventual pre-lexeme events */
  /* Switch to user space now */

  /* Prepare filterings */
  maxPriorityInitializedb = 0;
  for (symboll = 0; symboll < nSymboll; symboll++) {
    symboli = symbolArrayp[symboll];
    if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "No such symbol ID %d", symboli);
      goto err;
    }
    symbolp = GENERICSTACK_GET_PTR(symbolStackp, symboli);

    /* Get lexeme length */
    if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, alternativeStackp, (int) symboll, &sizel)) {
      goto err;
    }
    if (sizel <= 0) {
      /* This symbol did not match */
      continue;
    }

    if (latmb) {
      /* In latm mode, keep only the longests alternatives */
      if (sizel < maxMatchedl) {
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp,
                                    funcs,
                                    "Alternative %s is skipped (length %ld < max length %ld)",
                                    symbolp->descp->asciis,
                                    (unsigned long) sizel,
                                    (unsigned long) maxMatchedl);
        continue;
      }      
    }

    if ((! maxPriorityInitializedb) || (symbolp->priorityi > maxPriorityi)) {
      maxPriorityi = symbolp->priorityi;
    }
  }
  
  /* Push alternatives - there are as many entries in alternativeStackp than the number of expected symbols */
  /* We have done it like this to be able to recover symbolp easily. */
  for (symboll = 0; symboll < nSymboll; symboll++) {
    symboli = symbolArrayp[symboll];
    if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
      MARPAESLIF_ERRORF(marpaESLIFp, "No such symbol ID %d", symboli);
      goto err;
    }
    symbolp = GENERICSTACK_GET_PTR(symbolStackp, symboli);

    /* Get lexeme length */
    if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, alternativeStackp, (int) symboll, &sizel)) {
      goto err;
    }
    if (sizel <= 0) {
      /* This symbol did not match */
      continue;
    }

    if (latmb) {
      /* If latm mode is true, keep only the longests alternatives */
      /* Already traced few lines higher */
      if (sizel < maxMatchedl) {
        continue;
      }      
    }

    if (symbolp->priorityi < maxPriorityi) {
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp,
                                    funcs,
                                    "Alternative %s is out-prioritized (priority %d < max priority %d)",
                                    symbolp->descp->asciis,
                                    symbolp->priorityi,
                                    maxPriorityi);
        continue;
    }

    /* Commit in the lexeme input stack */
    if (! _marpaESLIFRecognizer_lexemeStack_i_moveb(marpaESLIFRecognizerp, lexemeInputStackp, GENERICSTACK_USED(lexemeInputStackp), alternativeStackp, (int) symboll)) {
      goto err;
    }

    /* And to Marpa */
    if (! _marpaESLIFRecognizer_alternativeb(marpaESLIFRecognizerp,
                                             symbolp,
                                             GENERICSTACK_USED(lexemeInputStackp) - 1)) {
      goto err;
    }
  }

  /* Commit */
  if (! _marpaESLIFRecognizer_completeb(marpaESLIFRecognizerp)) {
#ifndef MARPAESLIF_NTRACE
    marpaESLIFRecognizer_progressLogb(marpaESLIFRecognizerp,
                                      0,
                                      -1,
                                      GENERICLOGGER_LOGLEVEL_TRACE,
                                      marpaESLIFGrammarp,
                                      _marpaESLIFGrammar_symbolDescriptionCallback);
#endif
    goto err;
  }

  /* Remember this recognizer have at least one lexeme */
  marpaESLIFRecognizerp->haveLexemeb = 1;
  
  /* New line processing, etc... */
  if (! _marpaESLIFRecognizer_matchPostProcessingb(marpaESLIFRecognizerp, maxMatchedl)) {
    goto err;
  }
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Advancing stream internal position by %ld bytes", (unsigned long) maxMatchedl);
  marpaESLIFRecognizerp->inputs += maxMatchedl;
  marpaESLIFRecognizerp->inputl -= maxMatchedl;

  /* Collect events */
  MARPAESLIFRECOGNIZER_COLLECT_EVENTS(1, &exhaustedb);

  rcb = 1;
  continueb = ! exhaustedb;
  goto done;

 err:
  rcb = 0;

 done:
  _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizerp, alternativeStackp);
  _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizerp, matchedStackp);
  _marpaESLIF_grammarStack_resetv(localOutputStackp);
  if (rcb) {
    if (exhaustedbp != NULL) {
      *exhaustedbp = exhaustedb;
    }
    if (continuebp != NULL) {
      *continuebp = continueb;
    }
    MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d (*continuebp=%d, *exhaustedbp=%d)", (int) rcb, (int) continueb, (int) exhaustedb);
  } else {
    MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "return 0");
  }

  /* At level 0, this is the final value - we generate error information if there is input */
  if (! rcb) {
    if ((marpaESLIFRecognizerp->leveli == 0) && (! marpaESLIFRecognizerp->discardb)) {
      MARPAESLIF_ERROR(marpaESLIFRecognizerp->marpaESLIFp, "--------------------------------------------");
      MARPAESLIF_ERROR(marpaESLIFRecognizerp->marpaESLIFp, "Recognizer failure. Current state:");
      marpaESLIFRecognizer_progressLogb(marpaESLIFRecognizerp,
                                        -1,
                                        -1,
                                        GENERICLOGGER_LOGLEVEL_ERROR,
                                        marpaESLIFRecognizerp->marpaESLIFGrammarp,
                                        _marpaESLIFGrammar_symbolDescriptionCallback);
      MARPAESLIF_ERROR(marpaESLIFRecognizerp->marpaESLIFp, "--------------------------------------------");
      if (nSymboll <= 0) {
        MARPAESLIF_ERROR(marpaESLIFRecognizerp->marpaESLIFp, "There was no expected terminal");
      } else {
        for (symboll = 0; symboll < nSymboll; symboll++) {
          symboli = symbolArrayp[symboll];
          if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
            continue;
          }
          symbolp = GENERICSTACK_GET_PTR(symbolStackp, symboli);
          MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "Expected terminal: %s", symbolp->descp->asciis);
        }
      }
      /* If there is some information before, show it */
      if ((marpaESLIFRecognizerp->inputs != NULL) && ((*marpaESLIFRecognizerp->buffersp) != NULL) && (marpaESLIFRecognizerp->inputs > *(marpaESLIFRecognizerp->buffersp))) {
        char  *dumps;
        size_t dumpl;

        if ((marpaESLIFRecognizerp->inputs - *(marpaESLIFRecognizerp->buffersp)) > 128) {
          dumps = marpaESLIFRecognizerp->inputs - 128;
          dumpl = 128;
        } else {
          dumps = *(marpaESLIFRecognizerp->buffersp);
          dumpl = marpaESLIFRecognizerp->inputs - *(marpaESLIFRecognizerp->buffersp);
        }
        _marpaESLIFRecognizer_hexdumpv(marpaESLIFRecognizerp,
                                       "",
                                       *(marpaESLIFRecognizerp->utfbp) ? "UTF-8 converted data before the failure" : "Raw data before the failure",
                                       dumps,
                                       dumpl,
                                       0 /* traceb */);
      }
      if (marpaESLIFRecognizerp->marpaESLIFRecognizerOption.newlineb) {
        if (marpaESLIFRecognizerp->columnl > 0) {
          /* Column is known (in terms of character count) */
          MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "RECOGNIZER FAILURE AFTER LINE No %ld COLUMN No %ld, HERE:", (unsigned long) marpaESLIFRecognizerp->linel, (unsigned long) marpaESLIFRecognizerp->columnl);
        } else {
          /* Column is not known */
          MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "RECOGNIZER FAILURE AFTER LINE No %ld, HERE:", (unsigned long) marpaESLIFRecognizerp->linel);
        }
      } else {
        MARPAESLIF_ERROR(marpaESLIFRecognizerp->marpaESLIFp, "RECOGNIZER FAILURE HERE:");
      }
      /* If there is some information after, show it */
      if ((marpaESLIFRecognizerp->inputs != NULL) && (marpaESLIFRecognizerp->inputl > 0)) {
        char  *dumps;
        size_t dumpl;

        dumps = marpaESLIFRecognizerp->inputs;
        dumpl = marpaESLIFRecognizerp->inputl > 128 ? 128 : marpaESLIFRecognizerp->inputl;
        _marpaESLIFRecognizer_hexdumpv(marpaESLIFRecognizerp,
                                       "",
                                       *(marpaESLIFRecognizerp->utfbp) ? "UTF-8 converted data after the failure" : "Raw data after the failure",
                                       dumps,
                                       dumpl,
                                       0 /* traceb */);
      }
    }
  }

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
short marpaESLIFRecognizer_alternativeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *symbolnamecp, size_t symbolnamel, int valuei)
/*****************************************************************************/
{
  const static char    *funcs              = "marpaESLIFRecognizer_alternativeb";
  marpaESLIF_symbol_t  *symbolp;
  short                 rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  symbolp = _marpaESLIFRecognizer_symbol_findp(marpaESLIFRecognizerp, symbolnamecp, symbolnamel);
  if (symbolp == NULL) {
    return 0;
  }

  rcb = _marpaESLIFRecognizer_alternativeb(marpaESLIFRecognizerp, symbolp, valuei);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_alternativeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIF_symbol_t *symbolp, int valuei)
/*****************************************************************************/
{
  const static char *funcs = "_marpaESLIFRecognizer_alternativeb";
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "Committing alternative %s at input stack %d", symbolp->descp->asciis, valuei);
  if (valuei == 743) {
    int i = 0;
    i += 1;
  }

  rcb = marpaWrapperRecognizer_alternativeb(marpaESLIFRecognizerp->marpaWrapperRecognizerp, symbolp->idi, valuei, 1);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
short marpaESLIFRecognizer_completeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp)
/*****************************************************************************/
{
  const static char *funcs = "marpaESLIFRecognizer_completeb";
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "Completing alternatives");
  rcb = _marpaESLIFRecognizer_completeb(marpaESLIFRecognizerp);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_completeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp)
/*****************************************************************************/
{
  const static char *funcs = "_marpaESLIFRecognizer_completeb";
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  rcb = marpaWrapperRecognizer_completeb(marpaESLIFRecognizerp->marpaWrapperRecognizerp);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
short marpaESLIFRecognizer_event_onoffb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, int symboli, marpaESLIFEventType_t eventSeti, int onoffb)
/*****************************************************************************/
{
  const static char *funcs = "marpaESLIFRecognizer_event_onoffb";
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  /* Of course, our marpaESLIFEventType_t is strictly equivalent to marpaWrapperGrammarEventType_t -; */
  rcb = marpaWrapperRecognizer_event_onoffb(marpaESLIFRecognizerp->marpaWrapperRecognizerp, symboli, eventSeti, onoffb);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
short marpaESLIFRecognizer_expectedb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, size_t *nSymbollp, int **symbolArraypp)
/*****************************************************************************/
{
  const static char *funcs = "marpaESLIFRecognizer_expectedb";
  short              rcb;
  
  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  rcb = marpaWrapperRecognizer_expectedb(marpaESLIFRecognizerp->marpaWrapperRecognizerp, nSymbollp, symbolArraypp);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
void marpaESLIFGrammar_freev(marpaESLIFGrammar_t *marpaESLIFGrammarp)
/*****************************************************************************/
{
  _marpaESLIFGrammar_freev(marpaESLIFGrammarp, 0 /* onStackb */);
}

/*****************************************************************************/
void marpaESLIFRecognizer_freev(marpaESLIFRecognizer_t *marpaESLIFRecognizerp)
/*****************************************************************************/
{
  if (marpaESLIFRecognizerp != NULL) {
    marpaESLIFRecognizer_t *marpaESLIFRecognizerParentp = marpaESLIFRecognizerp->parentRecognizerp;
    
    _marpaESLIFrecognizer_lexemeStack_freev(marpaESLIFRecognizerp, marpaESLIFRecognizerp->lexemeInputStackp);
    if (marpaESLIFRecognizerp->marpaWrapperRecognizerp != NULL) {
      marpaWrapperRecognizer_freev(marpaESLIFRecognizerp->marpaWrapperRecognizerp);
    }
    if (marpaESLIFRecognizerp->eventArrayp != NULL) {
      free(marpaESLIFRecognizerp->eventArrayp);
    }
    if (marpaESLIFRecognizerParentp == NULL) {
      /* The "buffer" area is managed by the parent recognizer */
      if (marpaESLIFRecognizerp->_buffers != NULL) {
        free(marpaESLIFRecognizerp->_buffers);
      }
    } else {
      /* Parent's "current" position have to be updated */
      marpaESLIFRecognizerParentp->inputs = *(marpaESLIFRecognizerp->buffersp) + marpaESLIFRecognizerp->parentDeltal;
      marpaESLIFRecognizerParentp->inputl = *(marpaESLIFRecognizerp->bufferlp) - marpaESLIFRecognizerp->parentDeltal;
    }
    free(marpaESLIFRecognizerp);
  }
}

/*****************************************************************************/
short marpaESLIFGrammar_parseb(marpaESLIFGrammar_t *marpaESLIFGrammarp, marpaESLIFRecognizerOption_t *marpaESLIFRecognizerOptionp, marpaESLIFValueOption_t *marpaESLIFValueOptionp, short *exhaustedbp)
/*****************************************************************************/
{
  return marpaESLIFGrammar_parse_by_grammarb(marpaESLIFGrammarp, marpaESLIFRecognizerOptionp, marpaESLIFValueOptionp, exhaustedbp, NULL /* grammardescs */, 0 /* grammardescl */, 0 /* grammarleveli */);
}

/*****************************************************************************/
short marpaESLIFGrammar_parse_by_grammarb(marpaESLIFGrammar_t *marpaESLIFGrammarp, marpaESLIFRecognizerOption_t *marpaESLIFRecognizerOptionp, marpaESLIFValueOption_t *marpaESLIFValueOptionp, short *exhaustedbp, char *grammardescs, size_t grammardescl, int grammarleveli)
/*****************************************************************************/
{
  const static char          *funcs        = "marpaESLIFGrammar_parse_by_grammarb";
  marpaESLIF_grammar_t       *grammarp;
  short                       rcb;
  marpaESLIFGrammar_t         marpaESLIFGrammar;

  grammarp = _marpaESLIFGrammar_grammar_findp(marpaESLIFGrammarp, grammardescs, grammardescl, grammarleveli);
  if (grammarp == NULL) {
    goto err;
  }

  /* Use a local marpaESLIFGrammar and change current gramamar */
  marpaESLIFGrammar          = *marpaESLIFGrammarp;
  marpaESLIFGrammar.grammarp = grammarp;
  rcb = _marpaESLIFGrammar_parseb(&marpaESLIFGrammar, marpaESLIFRecognizerOptionp, marpaESLIFValueOptionp, 0 /* discardb */, NULL /* marpaESLIFRecognizerParentp */, exhaustedbp, 0 /* internalb */);
  goto done;

 err:
  rcb = 0;

 done:
  MARPAESLIF_TRACEF(marpaESLIFGrammarp->marpaESLIFp, funcs, "return %d", (int) rcb);
  return 1;
}

/*****************************************************************************/
static short _marpaESLIFReader_grammarReader(void *userDatavp, char **inputsp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingsp)
/*****************************************************************************/
{
  const static char          *funcs                     = "marpaESLIFReader_grammarReader";
  marpaESLIF_readerContext_t *marpaESLIF_readerContextp = (marpaESLIF_readerContext_t *) userDatavp;
  marpaESLIF_t               *marpaESLIFp               = marpaESLIF_readerContextp->marpaESLIFp;

  *inputsp           = marpaESLIF_readerContextp->marpaESLIFGrammarOptionp->grammars;
  *inputlp           = marpaESLIF_readerContextp->marpaESLIFGrammarOptionp->grammarl;
  *encodingsp        = marpaESLIF_readerContextp->marpaESLIFGrammarOptionp->encodings;
  *eofbp             = 1;
  *characterStreambp = 1; /* We say this is a stream of characters */
  *encodingsp        = NULL;     /* We let ESLIF determine the encoding */

  MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return 1 (*inputsp=%p, *inputlp=%ld, *eofbp=%d, *characterStreambp=%d)", *inputsp, (unsigned long) *inputlp, (int) *eofbp, (int) *characterStreambp);
  return 1;
}

/*****************************************************************************/
static inline marpaESLIF_symbol_t *_marpaESLIFRecognizer_symbol_findp(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *descs, size_t descl)
/*****************************************************************************/
{
  const static char   *funcs = "_marpaESLIFRecognizer_symbol_findp";
  marpaESLIF_symbol_t *symbolp;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  symbolp = _marpaESLIF_symbol_findp(marpaESLIFRecognizerp->marpaESLIFp, marpaESLIFRecognizerp->marpaESLIFGrammarp->grammarp, descs, descl, -1);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %p", symbolp);
  marpaESLIFRecognizerp->callstackCounteri--;
  return symbolp;
}

/*****************************************************************************/
static inline marpaESLIF_grammar_t *_marpaESLIFGrammar_grammar_findp(marpaESLIFGrammar_t *marpaESLIFGrammarp, char *descs, size_t descl, int leveli)
/*****************************************************************************/
{
  const static char    *funcs         = "_marpaESLIFGrammar_grammar_findp";
  genericStack_t       *grammarStackp = marpaESLIFGrammarp->grammarStackp;
  marpaESLIF_grammar_t *rcp           = NULL;
  marpaESLIF_string_t   desc          = { descs /* bytep */, descl /* bytel */ };
  marpaESLIF_grammar_t *grammarp;
  int                   grammari;

  if (descs != NULL) {
    /* Give precedence to grammar by name */
    for (grammari = 0; grammari < GENERICSTACK_USED(grammarStackp); grammari++) {
      if (! GENERICSTACK_IS_PTR(grammarStackp, grammari)) {
        /* Should never happen */
        continue;
      }
      grammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(grammarStackp, grammari);
      if (_marpaESLIF_string_eqb(grammarp->descp, &desc)) {
        rcp = grammarp;
        break;
      }
    }
  } else {
    if (GENERICSTACK_IS_PTR(grammarStackp, leveli)) {
      grammarp = (marpaESLIF_grammar_t *) GENERICSTACK_GET_PTR(grammarStackp, leveli);
    }
  }

  if (rcp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFGrammarp->marpaESLIFp, "No such grammar");
  }

  MARPAESLIF_TRACEF(marpaESLIFGrammarp->marpaESLIFp, funcs, "return %p", rcp);
  return rcp;
}
 
/*****************************************************************************/
static inline marpaESLIF_rule_t *_marpaESLIF_rule_findp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, char *descs, size_t descl, int rulei)
/*****************************************************************************/
{
  const static char    *funcs        = "_marpaESLIF_rule_findp";
  genericStack_t       *ruleStackp   = grammarp->ruleStackp;
  marpaESLIF_string_t   desc          = { descs /* bytep */, descl /* bytel */ };
  marpaESLIF_rule_t    *rcp          = NULL;
  marpaESLIF_rule_t    *rulep;
  int                   i;

  if (descs != NULL) {
    /* Give precedence to rule by name */
    for (i = 0; i < GENERICSTACK_USED(ruleStackp); i++) {
      if (! GENERICSTACK_IS_PTR(ruleStackp, i)) {
        /* Should never happen */
        continue;
      }
      rulep = (marpaESLIF_rule_t *) GENERICSTACK_GET_PTR(ruleStackp, i);
      if (_marpaESLIF_string_eqb(rulep->descp, &desc)) {
        rcp = rulep;
        break;
      }
    }
  } else {
    if (GENERICSTACK_IS_PTR(ruleStackp, rulei)) {
      rcp = (marpaESLIF_rule_t *) GENERICSTACK_GET_PTR(ruleStackp, rulei);
    }
  }

  if (rcp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFp, "No such rule");
  }

  MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", rcp);
  return rcp;
}

/*****************************************************************************/
static inline marpaESLIF_symbol_t *_marpaESLIF_symbol_findp(marpaESLIF_t *marpaESLIFp, marpaESLIF_grammar_t *grammarp, char *descs, size_t descl, int symboli)
/*****************************************************************************/
{
  const static char    *funcs        = "_marpaESLIF_symbol_findp";
  genericStack_t       *symbolStackp = grammarp->symbolStackp;
  marpaESLIF_string_t   desc         = { descs /* bytep */, descl /* bytel */ };
  marpaESLIF_symbol_t  *rcp          = NULL;
  marpaESLIF_symbol_t  *symbolp;
  int                   i;

  if (descs != NULL) {
    /* Give precedence to symbol by name */
    for (i = 0; i < GENERICSTACK_USED(symbolStackp); i++) {
      if (! GENERICSTACK_IS_PTR(symbolStackp, i)) {
        /* Should never happen */
        continue;
      }
      symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, i);
      if (_marpaESLIF_string_eqb(symbolp->descp, &desc)) {
        rcp = symbolp;
        break;
      }
    }
  } else {
    if (GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
      rcp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);
    }
  }

  if (rcp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFp, "No such symbol");
  }

  MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", rcp);
  return rcp;
}

/*****************************************************************************/
void marpaESLIFRecognizer_eventb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, size_t *eventArraylp, marpaESLIFEvent_t **eventArraypp)
/*****************************************************************************/
{
  const static char *funcs = "marpaESLIFRecognizer_eventb";

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  if (eventArraylp != NULL) {
    *eventArraylp = marpaESLIFRecognizerp->eventArrayl;
  }
  if (eventArraypp != NULL) {
    *eventArraypp = marpaESLIFRecognizerp->eventArrayp;
  }

  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "return");
  marpaESLIFRecognizerp->callstackCounteri--;
}

/*****************************************************************************/
static inline void _marpaESLIF_freeCallbackv(void *userDatavp, void *p)
/*****************************************************************************/
{
  if (p != NULL) {
    free(p);
  }
}

/*****************************************************************************/
static inline void _marpaESLIFRecognizer_reset_eventsb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp)
/*****************************************************************************/
{
  const static char *funcs = "_marpaESLIFRecognizer_reset_eventsb";

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  if (marpaESLIFRecognizerp->eventArrayp != NULL) {
    free(marpaESLIFRecognizerp->eventArrayp);
    marpaESLIFRecognizerp->eventArrayp = NULL;
  }
  marpaESLIFRecognizerp->eventArrayl = 0;

  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "return");
  marpaESLIFRecognizerp->callstackCounteri--;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_grammar_eventsb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, short *completedbp, short *nulledbp, short *predictedbp, short *exhaustedbp)
/*****************************************************************************/
{
  const static char          *funcs              = "_marpaESLIFRecognizer_grammar_eventsb";
  marpaESLIF_t               *marpaESLIFp        = marpaESLIFRecognizerp->marpaESLIFp;
  marpaESLIFGrammar_t        *marpaESLIFGrammarp = marpaESLIFRecognizerp->marpaESLIFGrammarp;
  marpaESLIF_grammar_t       *grammarp           = marpaESLIFGrammarp->grammarp;
  genericStack_t             *symbolStackp       = grammarp->symbolStackp;
  short                       completedb         = 0;
  short                       nulledb            = 0;
  short                       predictedb         = 0;
  short                       exhaustedb         = 0;
  size_t                      okeventl           = 0;
  marpaESLIF_symbol_t        *symbolp;
  int                         symboli;
  size_t                      grammarEventl;
  marpaWrapperGrammarEvent_t *grammarEventp;
  short                       rcb;
  marpaESLIFString_t         *stringp;
  size_t                      i;
  marpaESLIFEventType_t       type;
  marpaESLIFEvent_t           *eventArrayp;      /* For eventual realloc */

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  /* Collect grammar native events and push them in the events stack */
  if (! marpaWrapperGrammar_eventb(marpaESLIFRecognizerp->discardb ? grammarp->marpaWrapperGrammarDiscardp : grammarp->marpaWrapperGrammarStartp, &grammarEventl, &grammarEventp, 0)) {
    goto err;
  }
  if (grammarEventl > 0) {
    marpaESLIFRecognizerp->eventArrayp = malloc(grammarEventl * sizeof(marpaESLIFEvent_t));
    if (marpaESLIFRecognizerp->eventArrayp == NULL) {
      MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
      goto err;
    }
    marpaESLIFRecognizerp->eventArrayl = 0;

    for (i = 0; i < grammarEventl; i++) {
      symboli = grammarEventp[i].symboli;
      type    = MARPAESLIF_EVENTTYPE_NONE;
      stringp = NULL;
      if (symboli >= 0) {
        /* Look for the symbol */
        if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
          MARPAESLIF_ERRORF(marpaESLIFp, "No such symbol ID %d", symboli);
          goto err;
        }
        symbolp = GENERICSTACK_GET_PTR(symbolStackp, symboli);
      } else {
        symbolp = NULL;
      }
      
      /* Our grammar made sure there can by only one named event per symbol */
      /* In addition, marpaWrapper guarantee there is a symbol associated to */
      /* completion, nulled or prediction events */
      switch (grammarEventp[i].eventType) {
      case MARPAWRAPPERGRAMMAR_EVENT_COMPLETED:
        type        = MARPAESLIF_EVENTTYPE_COMPLETED;
        if (symbolp != NULL) {
          stringp = symbolp->eventp;
        }
        completedb  = 1;
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%s: completion event", symbolp->descp->asciis);
        break;
      case MARPAWRAPPERGRAMMAR_EVENT_NULLED:
        type        = MARPAESLIF_EVENTTYPE_NULLED;
        if (symbolp != NULL) {
          stringp = symbolp->eventp;
        }
        nulledb     = 1;
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%s: nulling event", symbolp->descp->asciis);
      case MARPAWRAPPERGRAMMAR_EVENT_EXPECTED:
        type        = MARPAESLIF_EVENTTYPE_PREDICTED;
        if (symbolp != NULL) {
          stringp = symbolp->eventp;
        }
        predictedb  = 1;
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%s: prediction event", symbolp->descp->asciis);
        break;
      case MARPAWRAPPERGRAMMAR_EVENT_EXHAUSTED:
        /* This is ok at EOF or if the recognizer is ok with exhaustion */
        if ((! *(marpaESLIFRecognizerp->eofbp)) && (! marpaESLIFRecognizerp->marpaESLIFRecognizerOption.exhaustedb)) {
          MARPAESLIF_ERROR(marpaESLIFp, "Grammar is exhausted but lexeme remains");
          goto err;
        }
        type        = MARPAESLIF_EVENTTYPE_EXHAUSTED;
        exhaustedb  = 1;
        MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "Exhausted event");
        /* symboli will be -1 as per marpaWrapper spec */
        break;
      default:
        MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%s: unsupported event type %d", symbolp->descp->asciis, grammarEventp[i].eventType);
        break;
      }

      if (type != MARPAESLIF_EVENTTYPE_EXHAUSTED) {
        marpaESLIFRecognizerp->eventArrayp[okeventl].type    = type;
        marpaESLIFRecognizerp->eventArrayp[okeventl].stringp = stringp;
        marpaESLIFRecognizerp->eventArrayl = ++okeventl;
      }
    }
  }

  if (! exhaustedb) {
    /* The event on exhaustion only occurs if needed to provide a reason to return. */
    /* If not sent by the grammar, we check explicitely ourself.                    */
    if (! marpaWrapperRecognizer_exhaustedb(marpaESLIFRecognizerp->marpaWrapperRecognizerp, &exhaustedb)) {
      goto err;
    }
    if (exhaustedb) {
      /* This is ok at EOF or if the recognizer is ok with exhaustion */
      if ((! *(marpaESLIFRecognizerp->eofbp)) && (! marpaESLIFRecognizerp->marpaESLIFRecognizerOption.exhaustedb)) {
        MARPAESLIF_ERROR(marpaESLIFp, "Grammar is exhausted but lexeme remains");
        goto err;
      }
      /* Extend of create the array */
      if (okeventl <= 0) {
        marpaESLIFRecognizerp->eventArrayp = malloc(sizeof(marpaESLIFEvent_t));
        if (marpaESLIFRecognizerp->eventArrayp == NULL) {
          MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
          goto err;
        }
        marpaESLIFRecognizerp->eventArrayp[0].type    = MARPAESLIF_EVENTTYPE_EXHAUSTED;
        marpaESLIFRecognizerp->eventArrayp[0].stringp = NULL;
        marpaESLIFRecognizerp->eventArrayl = 1;
      } else {
        eventArrayp = realloc(marpaESLIFRecognizerp->eventArrayp, (okeventl + 1) + sizeof(marpaESLIFEvent_t));
        if (eventArrayp == NULL) {
          MARPAESLIF_ERRORF(marpaESLIFp, "realloc failure, %s", strerror(errno));
          goto err;
        }
        marpaESLIFRecognizerp->eventArrayp = eventArrayp;
        marpaESLIFRecognizerp->eventArrayp[okeventl].type    = MARPAESLIF_EVENTTYPE_EXHAUSTED;
        marpaESLIFRecognizerp->eventArrayp[okeventl].stringp = NULL;
        marpaESLIFRecognizerp->eventArrayl++;
      }
        MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "Exhausted event");
    }
  }

  rcb = 1;
  goto done;
  
 err:
  rcb = 0;

 done:
  if (completedbp != NULL) {
    *completedbp = completedb;
  }
  if (nulledbp != NULL) {
    *nulledbp = nulledb;
  }
  if (predictedbp != NULL) {
    *predictedbp = predictedb;
  }
  if (exhaustedbp != NULL) {
    *exhaustedbp = exhaustedb;
  }
  /* MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d (*exhaustedbp=%d)", (int) rcb, (int) exhaustedb); */
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline marpaESLIFRecognizer_t *_marpaESLIFRecognizer_newp(marpaESLIFGrammar_t *marpaESLIFGrammarp, marpaESLIFRecognizerOption_t *marpaESLIFRecognizerOptionp, short discardb, marpaESLIFRecognizer_t *marpaESLIFRecognizerParentp, short fakeb)
/*****************************************************************************/
{
  const static char             *funcs                 = "_marpaESLIFRecognizer_newp";
  marpaESLIF_t                  *marpaESLIFp           = marpaESLIFGrammarp->marpaESLIFp;
  marpaESLIFRecognizer_t        *marpaESLIFRecognizerp = NULL;
  marpaWrapperRecognizerOption_t marpaWrapperRecognizerOption;

#ifndef MARPAESLIF_NTRACE
  /*
  if (marpaESLIFRecognizerParentp != NULL) {
    MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerParentp, funcs, "Building %s Recognizer", discardb ? ":discard" : ":start");
  } else {
    MARPAESLIF_TRACEF(marpaESLIFp, funcs, "Building %s Recognizer", discardb ? ":discard" : ":start");
  }
  */
#endif
  
  if (marpaESLIFRecognizerOptionp == NULL) {
    marpaESLIFRecognizerOptionp = &marpaESLIFRecognizerOption_default;
  }

  if (marpaESLIFGrammarp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFp, "Null grammar pointer");
    goto err;
  }

  /* We request a stream reader callback unless eof flag is set by an eventual parent recognizer or we are in fake mode */
  if (marpaESLIFRecognizerOptionp->marpaESLIFReaderCallbackp == NULL) {
    if (! (((marpaESLIFRecognizerParentp != NULL) && *(marpaESLIFRecognizerParentp->eofbp))
           ||
           fakeb)) {
      MARPAESLIF_ERROR(marpaESLIFp, "Null reader callback");
      goto err;
    }
  }
  
  marpaESLIFRecognizerp = (marpaESLIFRecognizer_t *) malloc(sizeof(marpaESLIFRecognizer_t));
  if (marpaESLIFRecognizerp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  marpaESLIFRecognizerp->marpaESLIFp                = marpaESLIFp;
  marpaESLIFRecognizerp->marpaESLIFGrammarp         = marpaESLIFGrammarp;
  marpaESLIFRecognizerp->marpaESLIFRecognizerOption = *marpaESLIFRecognizerOptionp;
  marpaESLIFRecognizerp->marpaWrapperRecognizerp    = NULL;
  marpaESLIFRecognizerp->lexemeInputStackp          = NULL;
  marpaESLIFRecognizerp->eventArrayp                = NULL;
  marpaESLIFRecognizerp->eventArrayl                = 0;
  marpaESLIFRecognizerp->parentRecognizerp          = marpaESLIFRecognizerParentp;
  marpaESLIFRecognizerp->resumeCounteri             = 0;
  marpaESLIFRecognizerp->callstackCounteri          = 0;
  marpaESLIFRecognizerp->_buffers                   = NULL;
  marpaESLIFRecognizerp->_bufferl                   = 0;
  marpaESLIFRecognizerp->_eofb                      = fakeb;  /* In fake mode, always make sure there is no reader needed */
  marpaESLIFRecognizerp->_utfb                      = 0;
  /* If this is a parent recognizer get its stream information */
  if (marpaESLIFRecognizerParentp != NULL) {
    marpaESLIFRecognizerp->leveli                     = marpaESLIFRecognizerParentp->leveli + 1;
    marpaESLIFRecognizerp->buffersp                   = marpaESLIFRecognizerParentp->buffersp;
    marpaESLIFRecognizerp->bufferlp                   = marpaESLIFRecognizerParentp->bufferlp;
    marpaESLIFRecognizerp->eofbp                      = marpaESLIFRecognizerParentp->eofbp;
    marpaESLIFRecognizerp->utfbp                      = marpaESLIFRecognizerParentp->utfbp;
    marpaESLIFRecognizerp->parentDeltal               = marpaESLIFRecognizerParentp->inputs - *(marpaESLIFRecognizerParentp->buffersp);
    /* New recognizer is starting at the parent's inputs pointer */
    marpaESLIFRecognizerp->inputs                     = marpaESLIFRecognizerParentp->inputs;
    marpaESLIFRecognizerp->inputl                     = marpaESLIFRecognizerParentp->inputl;
  } else {
    marpaESLIFRecognizerp->leveli                     = 0;
    marpaESLIFRecognizerp->buffersp                   = &(marpaESLIFRecognizerp->_buffers);
    marpaESLIFRecognizerp->bufferlp                   = &(marpaESLIFRecognizerp->_bufferl);
    marpaESLIFRecognizerp->eofbp                      = &(marpaESLIFRecognizerp->_eofb);
    marpaESLIFRecognizerp->utfbp                      = &(marpaESLIFRecognizerp->_utfb);
    marpaESLIFRecognizerp->parentDeltal               = 0;
    /* New recognizer is starting nowhere for the moment - it will ask for more data, c.f. recognizer's read() */
    marpaESLIFRecognizerp->inputs                     = NULL;
    marpaESLIFRecognizerp->inputl                     = 0;
  }
  marpaESLIFRecognizerp->scanb                      = 0;
  marpaESLIFRecognizerp->discardb                   = discardb;
  marpaESLIFRecognizerp->haveLexemeb                = 0;
  marpaESLIFRecognizerp->linel                      = 1;
  marpaESLIFRecognizerp->columnl                    = 0;

  marpaWrapperRecognizerOption.genericLoggerp       = marpaESLIFp->marpaESLIFOption.genericLoggerp;
  marpaWrapperRecognizerOption.disableThresholdb    = marpaESLIFRecognizerOptionp->disableThresholdb;

  if (! fakeb) {
    marpaESLIFRecognizerp->marpaWrapperRecognizerp    = marpaWrapperRecognizer_newp(discardb ?
                                                                                    marpaESLIFGrammarp->grammarp->marpaWrapperGrammarDiscardp
                                                                                    :
                                                                                    marpaESLIFGrammarp->grammarp->marpaWrapperGrammarStartp
                                                                                    , &marpaWrapperRecognizerOption);
    if (marpaESLIFRecognizerp->marpaWrapperRecognizerp == NULL) {
      goto err;
    }
  } else {
    marpaESLIFRecognizerp->marpaWrapperRecognizerp = NULL;
  }

  GENERICSTACK_NEW(marpaESLIFRecognizerp->lexemeInputStackp);
  if (marpaESLIFRecognizerp->lexemeInputStackp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "lexemeInputStackp initialization failure, %s", strerror(errno));
    goto err;
  }
  
  /* Marpa does not like the indice 0 */
  GENERICSTACK_PUSH_NA(marpaESLIFRecognizerp->lexemeInputStackp);
  if (GENERICSTACK_ERROR(marpaESLIFRecognizerp->lexemeInputStackp)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "lexemeInputStackp push failure, %s", strerror(errno));
    goto err;
  }

  goto done;

 err:
  marpaESLIFRecognizer_freev(marpaESLIFRecognizerp);
  marpaESLIFRecognizerp = NULL;

 done:
#ifndef MARPAESLIF_NTRACE
  /*
  if (marpaESLIFRecognizerParentp != NULL) {
    MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerParentp, funcs, "return %p", marpaESLIFRecognizerp);
  } else {
    MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", marpaESLIFRecognizerp);
  }
  */
#endif
  return marpaESLIFRecognizerp;
}

/*****************************************************************************/
static inline short _marpaESLIFGrammar_parseb(marpaESLIFGrammar_t *marpaESLIFGrammarp, marpaESLIFRecognizerOption_t *marpaESLIFRecognizerOptionp, marpaESLIFValueOption_t *marpaESLIFValueOptionp, short discardb, marpaESLIFRecognizer_t *marpaESLIFRecognizerParentp, short *exhaustedbp, short internalb)
/*****************************************************************************/
{
  const static char      *funcs                 = "_marpaESLIFGrammar_parseb";
  marpaESLIF_t           *marpaESLIFp           = marpaESLIFGrammarp->marpaESLIFp;
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp = NULL;
  marpaESLIFValueOption_t marpaESLIFValueOption = (marpaESLIFValueOptionp != NULL) ? *marpaESLIFValueOptionp : marpaESLIFValueOption_default_template;
  marpaESLIFValue_t      *marpaESLIFValuep      = NULL;
  short                   exhaustedb;
  short                   continueb;
  short                   rcb;

  marpaESLIFRecognizerp = _marpaESLIFRecognizer_newp(marpaESLIFGrammarp, marpaESLIFRecognizerOptionp, discardb, marpaESLIFRecognizerParentp, 0 /* fakeb */);
  if (marpaESLIFRecognizerp == NULL) {
    goto err;
  }
  if (! marpaESLIFRecognizer_scanb(marpaESLIFRecognizerp, 1 /* initialEventsb */, &continueb, &exhaustedb)) {
    goto err;
  }
  while (continueb) {
    if (! marpaESLIFRecognizer_resumeb(marpaESLIFRecognizerp, &continueb, &exhaustedb)) {
      goto err;
    }
  }

  /* Force unambiguity */
  marpaESLIFValueOption.ambiguousb = 0;
  marpaESLIFValuep = marpaESLIFValue_newp(marpaESLIFRecognizerp, &marpaESLIFValueOption);
  if (marpaESLIFValuep == NULL) {
    goto err;
  }
  /* No loop because we ask for a non-ambigous parse tree value */
  if (_marpaESLIFValue_valueb(marpaESLIFValuep, internalb) <= 0) {
    goto err;
  }

  rcb = 1;
  if (exhaustedbp != NULL) {
    *exhaustedbp = exhaustedb;
  }
  goto done;
  
 err:
  rcb = 0;

 done:
  marpaESLIFValue_freev(marpaESLIFValuep);
#ifndef MARPAESLIF_NTRACE
  if (marpaESLIFRecognizerp != NULL) {
    MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  } else {
    MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %d", (int) rcb);
  }
#endif
  marpaESLIFRecognizer_freev(marpaESLIFRecognizerp);
  return rcb;
}

/*****************************************************************************/
static void _marpaESLIF_generateStringWithLoggerCallback(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs)
/*****************************************************************************/
{
  _marpaESLIF_stringGenerator_t *contextp = (_marpaESLIF_stringGenerator_t *) userDatavp;
  char                         *tmps;

  if (contextp->s == NULL) {
    /* First time */
    contextp->l = strlen(msgs) + 1;
    contextp->s = strdup(msgs);
    if (contextp->s == NULL) {
      contextp->l = 0;
      MARPAESLIF_ERRORF(contextp->marpaESLIFp, "strdup failure, %s", strerror(errno));
      /* contextp->okb = 0; */ /* We initialized it with a false value -; */
    } else {
      contextp->okb = 1;
    }
  } else if (contextp->okb) {
    /* Only if previous round was ok */
    contextp->l = strlen(contextp->s) + strlen(msgs) + 1;
    tmps = (char *) realloc(contextp->s, contextp->l);
    if (tmps != NULL) {
      strcat(tmps, msgs);
      contextp->s = tmps;
    } else {
      contextp->okb = 0;
      contextp->l = 0;
      free(contextp->s);
      contextp->s = NULL;
      MARPAESLIF_ERRORF(contextp->marpaESLIFp, "realloc failure, %s", strerror(errno));
    }
  }
}

/* C.f. http://grapsus.net/blog/post/Hexadecimal-dump-in-C */
#ifndef MARPAESLIF_HEXDUMP_COLS
#define MARPAESLIF_HEXDUMP_COLS 16
#endif
/*****************************************************************************/
static inline void _marpaESLIFRecognizer_hexdumpv(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *headers, char *asciidescs, const char *p, size_t lengthl, short traceb)
/*****************************************************************************/
{
  const static char             *funcs       = "_marpaESLIFRecognizer_hexdumpv";
  marpaESLIF_t                  *marpaESLIFp = marpaESLIFRecognizerp->marpaESLIFp;
  _marpaESLIF_stringGenerator_t _marpaESLIF_stringGenerator;
  genericLogger_t               *genericLoggerp;
  size_t  i;
  size_t  j;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  _marpaESLIF_stringGenerator.marpaESLIFp = marpaESLIFp;
  _marpaESLIF_stringGenerator.s           = NULL;
  _marpaESLIF_stringGenerator.l           = 0;
  _marpaESLIF_stringGenerator.okb         = 0;

  genericLoggerp = GENERICLOGGER_CUSTOM(_marpaESLIF_generateStringWithLoggerCallback, (void *) &_marpaESLIF_stringGenerator, GENERICLOGGER_LOGLEVEL_TRACE);
  if (genericLoggerp == NULL) {
    goto done;
  }

  if (traceb) {
    MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "--------------------------------------------");
    MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%s%s (%ld bytes)", headers, asciidescs, (unsigned long) lengthl);
  } else {
    MARPAESLIF_ERROR(marpaESLIFp, "--------------------------------------------");
    MARPAESLIF_ERRORF(marpaESLIFp, "%s%s (%ld bytes)", headers, asciidescs, (unsigned long) lengthl);
  }
  for (i = 0; i < lengthl + ((lengthl % MARPAESLIF_HEXDUMP_COLS) ? (MARPAESLIF_HEXDUMP_COLS - lengthl % MARPAESLIF_HEXDUMP_COLS) : 0); i++) {
    /* print offset */
    if (i % MARPAESLIF_HEXDUMP_COLS == 0) {
      GENERICLOGGER_TRACEF(genericLoggerp, "0x%06x: ", i);
    }
    /* print hex data */
    if (i < lengthl) {
      GENERICLOGGER_TRACEF(genericLoggerp, "%02x ", 0xFF & ((char*)p)[i]);
    } else { /* end of block, just aligning for ASCII dump */
      GENERICLOGGER_TRACE(genericLoggerp, "   ");
    }
    /* print ASCII dump */
    if (i % MARPAESLIF_HEXDUMP_COLS == (MARPAESLIF_HEXDUMP_COLS - 1)) {
      for (j = i - (MARPAESLIF_HEXDUMP_COLS - 1); j <= i; j++) {
        if (j >= lengthl) { /* end of block, not really printing */
          GENERICLOGGER_TRACE(genericLoggerp, " ");
        }
        else if (isprint(0xFF & ((char*)p)[j])) { /* printable char */
          GENERICLOGGER_TRACEF(genericLoggerp, "%c", 0xFF & ((char*)p)[j]);
        }
        else { /* other char */
          GENERICLOGGER_TRACE(genericLoggerp, ".");
        }
      }
      if (_marpaESLIF_stringGenerator.okb) {
        if (traceb) {
          MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, _marpaESLIF_stringGenerator.s);
        } else {
          MARPAESLIF_ERROR(marpaESLIFp, _marpaESLIF_stringGenerator.s);
        }
        free(_marpaESLIF_stringGenerator.s);
        _marpaESLIF_stringGenerator.s = NULL;
        _marpaESLIF_stringGenerator.okb = 0;
      }
    }
  }

  if (traceb) {
    MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "--------------------------------------------");
  } else {
    MARPAESLIF_ERROR(marpaESLIFRecognizerp->marpaESLIFp, "--------------------------------------------");
  }
  GENERICLOGGER_FREE(genericLoggerp);

 done:
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "return");
  marpaESLIFRecognizerp->callstackCounteri--;
}

/*****************************************************************************/
marpaESLIFValue_t *marpaESLIFValue_newp(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIFValueOption_t *marpaESLIFValueOptionp)
/*****************************************************************************/
{
  const static char        *funcs             = "marpaESLIFValue_newp";
  marpaESLIF_t             *marpaESLIFp       = marpaESLIFRecognizerp->marpaESLIFp;
  marpaESLIFValue_t        *marpaESLIFValuep  = NULL;
  marpaWrapperValue_t      *marpaWrapperValuep = NULL;
  marpaWrapperValueOption_t marpaWrapperValueOption;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  /* MARPAESLIF_TRACE(marpaESLIFp, funcs, "Building Value"); */

  if (marpaESLIFValueOptionp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFp, "Value option structure must not be NULL");
    goto err;
  }

  marpaESLIFValuep = (marpaESLIFValue_t *) malloc(sizeof(marpaESLIFValue_t));
  if (marpaESLIFValuep == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  marpaESLIFValuep->marpaESLIFp            = marpaESLIFp;
  marpaESLIFValuep->marpaESLIFRecognizerp  = marpaESLIFRecognizerp;
  marpaESLIFValuep->marpaESLIFValueOption  = *marpaESLIFValueOptionp;
  marpaESLIFValuep->marpaWrapperValuep     = NULL;

  marpaWrapperValueOption.genericLoggerp = marpaESLIFp->marpaESLIFOption.genericLoggerp;
  marpaWrapperValueOption.highRankOnlyb  = marpaESLIFValueOptionp->highRankOnlyb;
  marpaWrapperValueOption.orderByRankb   = marpaESLIFValueOptionp->orderByRankb;
  marpaWrapperValueOption.ambiguousb     = marpaESLIFValueOptionp->ambiguousb;
  marpaWrapperValueOption.nullb          = marpaESLIFValueOptionp->nullb;
  marpaWrapperValueOption.maxParsesi     = marpaESLIFValueOptionp->maxParsesi;
  marpaWrapperValuep = marpaWrapperValue_newp(marpaESLIFRecognizerp->marpaWrapperRecognizerp, &marpaWrapperValueOption);
  if (marpaWrapperValuep == NULL) {
    goto err;
  }

  marpaESLIFValuep->marpaWrapperValuep = marpaWrapperValuep;
  goto done;

 err:
  marpaESLIFValue_freev(marpaESLIFValuep);
  marpaESLIFValuep = NULL;

 done:
  /* MARPAESLIF_TRACEF(marpaESLIFp, funcs, "return %p", marpaESLIFValuep); */
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %p", marpaESLIFValuep);
  marpaESLIFRecognizerp->callstackCounteri--;
  return marpaESLIFValuep;
}

/*****************************************************************************/
static inline short _marpaESLIFValue_valueb(marpaESLIFValue_t *marpaESLIFValuep, short internalb)
/*****************************************************************************/
{
  short rcb;

  if (internalb) {
    /* In the internal mode, we use internal wrappers that are forcing the context to be marpaESLIFValuep */
    rcb = marpaWrapperValue_valueb(marpaESLIFValuep->marpaWrapperValuep,
                                   marpaESLIFValuep,
                                   _marpaESLIFValueRuleCallbackInternalWrapper,
                                   _marpaESLIFValueSymbolCallbackInternalWrapper,
                                   _marpaESLIFValueNullingCallbackInternalWrapper);
  } else {
    rcb = marpaWrapperValue_valueb(marpaESLIFValuep->marpaWrapperValuep,
                                   marpaESLIFValuep,
                                   _marpaESLIFValueRuleCallbackWrapper,
                                   _marpaESLIFValueSymbolCallbackWrapper,
                                   _marpaESLIFValueNullingCallbackWrapper);
  }

return (rcb <= 0) ? 0 : 1;
}

/*****************************************************************************/
short marpaESLIFValue_valueb(marpaESLIFValue_t *marpaESLIFValuep)
/*****************************************************************************/
{
  return _marpaESLIFValue_valueb(marpaESLIFValuep, 0);
}

/*****************************************************************************/
void marpaESLIFValue_freev(marpaESLIFValue_t *marpaESLIFValuep)
/*****************************************************************************/
{
  if (marpaESLIFValuep != NULL) {
    marpaWrapperValue_t *marpaWrapperValuep = marpaESLIFValuep->marpaWrapperValuep;
    if (marpaWrapperValuep != NULL) {
      marpaWrapperValue_freev(marpaWrapperValuep);
    }
    free(marpaESLIFValuep);
  }
}

/*****************************************************************************/
static short _marpaESLIFValueRuleCallbackWrapper(void *userDatavp, int rulei, int arg0i, int argni, int resulti)
/*****************************************************************************/
{
  const static char              *funcs                 = "_marpaESLIFValueRuleCallbackWrapper";
  marpaESLIFValue_t              *marpaESLIFValuep      = (marpaESLIFValue_t *) userDatavp;
  marpaESLIFValueOption_t         marpaESLIFValueOption = marpaESLIFValuep->marpaESLIFValueOption;
  marpaESLIFValueRuleCallback_t   ruleCallbackp         = marpaESLIFValueOption.ruleCallbackp;
  
  /* Look if there is a rule callback */
  if (ruleCallbackp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFValuep->marpaESLIFp, "No rule value callback");
    return 0;
  }

  return ruleCallbackp(marpaESLIFValueOption.userDatavp, rulei, arg0i, argni, resulti);
}

/*****************************************************************************/
static short _marpaESLIFValueSymbolCallbackWrapper(void *userDatavp, int symboli, int argi, int resulti)
/*****************************************************************************/
{
  const static char               *funcs                 = "_marpaESLIFValueSymbolCallbackWrapper";
  marpaESLIFValue_t               *marpaESLIFValuep      = (marpaESLIFValue_t *) userDatavp;
  marpaESLIFValueOption_t          marpaESLIFValueOption = marpaESLIFValuep->marpaESLIFValueOption;
  marpaESLIFValueSymbolCallback_t  symbolCallbackp         = marpaESLIFValueOption.symbolCallbackp;
  
  /* Look if there is a rule callback */
  if (symbolCallbackp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFValuep->marpaESLIFp, "No symbol value callback");
    return 0;
  }

  return symbolCallbackp(marpaESLIFValueOption.userDatavp, marpaESLIFValuep->marpaESLIFRecognizerp->lexemeInputStackp, symboli, argi, resulti);
}

/*****************************************************************************/
static short _marpaESLIFValueNullingCallbackWrapper(void *userDatavp, int symboli, int resulti)
/*****************************************************************************/
{
  const static char               *funcs                 = "_marpaESLIFValueNullingCallbackWrapper";
  marpaESLIFValue_t               *marpaESLIFValuep      = (marpaESLIFValue_t *) userDatavp;
  marpaESLIFValueOption_t          marpaESLIFValueOption = marpaESLIFValuep->marpaESLIFValueOption;
  marpaESLIFValueNullingCallback_t nullingCallbackp      = marpaESLIFValueOption.nullingCallbackp;
  
  /* Look if there is a rule callback */
  if (nullingCallbackp == NULL) {
    MARPAESLIF_ERROR(marpaESLIFValuep->marpaESLIFp, "No nulling value callback");
    return 0;
  }

  return nullingCallbackp(marpaESLIFValueOption.userDatavp, symboli, resulti);
}

/*****************************************************************************/
static short _marpaESLIFValueRuleCallbackInternalWrapper(void *userDatavp, int rulei, int arg0i, int argni, int resulti)
/*****************************************************************************/
{
  marpaESLIFValue_t *marpaESLIFValuep = (marpaESLIFValue_t *) userDatavp;
  
  /* In the internal mode : we know what we are doing, i.e. ruleCallbackp is always set */
  /* and we force the context to be current marpaESLIFValuep.                           */
  return marpaESLIFValuep->marpaESLIFValueOption.ruleCallbackp(marpaESLIFValuep, rulei, arg0i, argni, resulti);
}

/*****************************************************************************/
static short _marpaESLIFValueSymbolCallbackInternalWrapper(void *userDatavp, int symboli, int argi, int resulti)
/*****************************************************************************/
{
  marpaESLIFValue_t *marpaESLIFValuep = (marpaESLIFValue_t *) userDatavp;
  
  /* In the internal mode : we know what we are doing, i.e. ruleCallbackp is always set */
  /* and we force the context to be current marpaESLIFValuep.                           */
  return marpaESLIFValuep->marpaESLIFValueOption.symbolCallbackp(marpaESLIFValuep, marpaESLIFValuep->marpaESLIFRecognizerp->lexemeInputStackp, symboli, argi, resulti);
}

/*****************************************************************************/
static short _marpaESLIFValueNullingCallbackInternalWrapper(void *userDatavp, int symboli, int resulti)
/*****************************************************************************/
{
  marpaESLIFValue_t *marpaESLIFValuep = (marpaESLIFValue_t *) userDatavp;
  
  /* In the internal mode : we know what we are doing, i.e. ruleCallbackp is always set */
  /* and we force the context to be current marpaESLIFValuep.                           */
  return marpaESLIFValuep->marpaESLIFValueOption.nullingCallbackp(marpaESLIFValuep, symboli, resulti);
}

/*****************************************************************************/
static short _marpaESLIFValueRuleCallbackInternalLexeme(void *userDatavp, int rulei, int arg0i, int argni, int resulti)
/*****************************************************************************/
{
  const static char         *funcs                 = "_marpaESLIFValueRuleCallbackInternalLexeme";
  marpaESLIFValue_t         *marpaESLIFValuep      = (marpaESLIFValue_t *) userDatavp;
  marpaESLIFRecognizer_t    *marpaESLIFRecognizerp = marpaESLIFValuep->marpaESLIFRecognizerp;
  marpaESLIF_valueContext_t *valueContextp         = (marpaESLIF_valueContext_t *) marpaESLIFValuep->marpaESLIFValueOption.userDatavp;
  marpaESLIFGrammar_t       *marpaESLIFGrammarp    = valueContextp->marpaESLIFGrammarp;
  marpaESLIF_t              *marpaESLIFp           = marpaESLIFGrammarp->marpaESLIFp;
#ifndef MARPAESLIF_NTRACE
  marpaESLIF_grammar_t      *grammarp              = marpaESLIFGrammarp->grammarp;
  genericStack_t            *ruleStackp            = grammarp->ruleStackp;
  marpaESLIF_rule_t         *rulep;
#endif
  genericStack_t            *outputStackp          = &(valueContextp->outputStack);
  size_t                     totalSizel            = 0;
  size_t                     previousTotalSizel    = 0;
  char                      *p                     = NULL;
  size_t                     sizel                 = 0;
  char                      *q;
  char                      *lexemep;
  size_t                     lexemel;
  int                        i;
  short                      rcb;

#ifndef MARPAESLIF_NTRACE
  if (! GENERICSTACK_IS_PTR(ruleStackp, rulei)) {
    MARPAESLIF_ERRORF(marpaESLIFGrammarp->marpaESLIFp, "No such rule No %d", rulei);
    goto err;
  }
  rulep = (marpaESLIF_rule_t *) GENERICSTACK_GET_PTR(ruleStackp, rulei);
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%p->[%d..%d] -> %p->[%d] %s", outputStackp, arg0i, argni, outputStackp, resulti, rulep->asciishows);
#endif

  /* Collect final size */
  for (i = arg0i; i <= argni; i++) {
    if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, outputStackp, i, &sizel)) {
      goto err;
    }
    /* Who knows */
    previousTotalSizel = totalSizel;
    totalSizel += sizel;
    if (totalSizel < previousTotalSizel) {
      MARPAESLIF_ERROR(marpaESLIFGrammarp->marpaESLIFp, "size_t turnaround");
      goto err;
    }
  }

  if (totalSizel > 0) {
    p = malloc(totalSizel);
    if (p == NULL) {
      MARPAESLIF_ERRORF(marpaESLIFGrammarp->marpaESLIFp, "malloc failure, %s", strerror(errno));
      goto err;
    }
    q = p;
    for (i = arg0i; i <= argni; i++) {
      if (! _marpaESLIFRecognizer_lexemeStack_i_p_and_sizeb(marpaESLIFRecognizerp, outputStackp, i, &lexemep, &lexemel)) {
        goto err;
      }
      /* The condition sizel > 0 and p == NULL never happen: this is our stack we control it */
      if (lexemep != NULL) {
        memcpy(q, lexemep, lexemel);
        q += lexemel;
      }
    }
  }

  if (! _marpaESLIFRecognizer_lexemeStack_i_setb(marpaESLIFRecognizerp, outputStackp, resulti, p, totalSizel)) {
    goto err;
  }

#ifndef MARPAESLIF_NTRACE
  _marpaESLIFRecognizer_hexdumpv(marpaESLIFRecognizerp, "Value result ", rulep->descp->asciis, p, totalSizel, 1 /* traceb */);
#endif

  q = NULL;
  rcb = 1;
  goto done;

 err:
  if (q != NULL) {
    free(q);
  }
  rcb = 0;

 done:
#ifndef MARPAESLIF_NTRACE
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%p->[%d..%d] -> %p->[%d] %s: %s", outputStackp, arg0i, argni, outputStackp, resulti, rulep->asciishows, rcb ? "OK" : "KO");
#endif
  return rcb;
}

/*****************************************************************************/
static short _marpaESLIFValueSymbolCallbackInternalLexeme(void *userDatavp, genericStack_t *lexemeStackp, int symboli, int argi, int resulti)
/*****************************************************************************/
{
  const static char         *funcs                 = "_marpaESLIFValueSymbolCallbackInternalLexeme";
  marpaESLIFValue_t         *marpaESLIFValuep      = (marpaESLIFValue_t *) userDatavp;
  marpaESLIFRecognizer_t    *marpaESLIFRecognizerp = marpaESLIFValuep->marpaESLIFRecognizerp;
  marpaESLIF_valueContext_t *valueContextp         = (marpaESLIF_valueContext_t *) marpaESLIFValuep->marpaESLIFValueOption.userDatavp;
  genericStack_t            *outputStackp          = &(valueContextp->outputStack);
  marpaESLIFGrammar_t       *marpaESLIFGrammarp    = valueContextp->marpaESLIFGrammarp;
  marpaESLIF_t              *marpaESLIFp           = marpaESLIFGrammarp->marpaESLIFp;
#ifndef MARPAESLIF_NTRACE
  marpaESLIF_grammar_t      *grammarp              = marpaESLIFGrammarp->grammarp;
  genericStack_t            *symbolStackp          = grammarp->symbolStackp;
  marpaESLIF_symbol_t       *symbolp;
#endif
  short                      rcb;

#ifndef MARPAESLIF_NTRACE
  if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "No such symbol No %d", symboli);
    goto err;
  }
  symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%p->[%d] -> %p->[%d] %s", lexemeStackp, argi, outputStackp, resulti, symbolp->descp->asciis);
#endif

  if (! _marpaESLIFRecognizer_lexemeStack_i_dupb(marpaESLIFRecognizerp, outputStackp, resulti, lexemeStackp, argi)) {
    goto err;
  }
  
  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
#ifndef MARPAESLIF_NTRACE
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%p->[%d] -> %p->[%d] %s: %s", lexemeStackp, argi, outputStackp, resulti, symbolp->descp->asciis, rcb ? "OK" : "KO");
#endif
  return rcb;
}

/*****************************************************************************/
static short _marpaESLIFValueNullingCallbackInternalLexeme(void *userDatavp, int symboli, int resulti)
/*****************************************************************************/
{
  const static char         *funcs                 = "_marpaESLIFValueNullingCallbackInternalLexeme";
  marpaESLIFValue_t         *marpaESLIFValuep      = (marpaESLIFValue_t *) userDatavp;
  marpaESLIFRecognizer_t    *marpaESLIFRecognizerp = marpaESLIFValuep->marpaESLIFRecognizerp;
  marpaESLIF_valueContext_t *valueContextp         = (marpaESLIF_valueContext_t *) marpaESLIFValuep->marpaESLIFValueOption.userDatavp;
  genericStack_t            *outputStackp          = &(valueContextp->outputStack);
  marpaESLIFGrammar_t       *marpaESLIFGrammarp    = valueContextp->marpaESLIFGrammarp;
  marpaESLIF_t              *marpaESLIFp           = marpaESLIFGrammarp->marpaESLIFp;
#ifndef MARPAESLIF_NTRACE
  marpaESLIF_grammar_t      *grammarp              = marpaESLIFGrammarp->grammarp;
  genericStack_t            *symbolStackp          = grammarp->symbolStackp;
  marpaESLIF_symbol_t       *symbolp;
#endif
  short                      rcb;

#ifndef MARPAESLIF_NTRACE
  if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
    MARPAESLIF_ERRORF(marpaESLIFp, "No such symbol No %d", symboli);
    goto err;
  }
  symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%p->[%d] %s", outputStackp, resulti, symbolp->descp->asciis);
#endif

  if (! _marpaESLIFRecognizer_lexemeStack_i_setb(marpaESLIFRecognizerp, outputStackp, resulti, NULL, 0)) {
    goto err;
  }
  
  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
#ifndef MARPAESLIF_NTRACE
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "%p->[%d] %s: %s", outputStackp, resulti, symbolp->descp->asciis, rcb ? "OK" : "KO");
#endif
  return rcb;
}

/*****************************************************************************/
static short _marpaESLIFValueRuleCallbackInternalGrammar(void *userDatavp, int rulei, int arg0i, int argni, int resulti)
/*****************************************************************************/
{
  const static char               *funcs                 = "_marpaESLIFValueRuleCallbackInternalGrammar";
  marpaESLIF_valueContext_t       *valueContextp         = (marpaESLIF_valueContext_t *) userDatavp;
  marpaESLIFGrammar_t             *marpaESLIFGrammarp    = valueContextp->marpaESLIFGrammarp;
  genericStack_t                  *outputStackp          = &(valueContextp->outputStack);

  /* TO DO */
  return 0;
}

/*****************************************************************************/
static short _marpaESLIFValueSymbolCallbackInternalGrammar(void *userDatavp, genericStack_t *lexemeStackp, int symboli, int argi, int resulti)
/*****************************************************************************/
{
  const static char               *funcs                 = "_marpaESLIFValueSymboCallbackGrammar";
  marpaESLIF_valueContext_t       *valueContextp         = (marpaESLIF_valueContext_t *) userDatavp;
  marpaESLIFGrammar_t             *marpaESLIFGrammarp    = valueContextp->marpaESLIFGrammarp;
  genericStack_t                  *outputStackp          = &(valueContextp->outputStack);

  /* TO DO */
  return 0;
}

/*****************************************************************************/
static short _marpaESLIFValueNullingCallbackInternalGrammar(void *userDatavp, int symboli, int resulti)
/*****************************************************************************/
{
  const static char               *funcs                 = "_marpaESLIFValueNullingCallbackInternalGrammar";
  marpaESLIF_valueContext_t       *valueContextp         = (marpaESLIF_valueContext_t *) userDatavp;
  marpaESLIFGrammar_t             *marpaESLIFGrammarp    = valueContextp->marpaESLIFGrammarp;
  genericStack_t                  *outputStackp          = &(valueContextp->outputStack);

  /* TO DO */
  return 0;
}

/*****************************************************************************/
marpaESLIFGrammar_t *marpaESLIFValue_grammarp(marpaESLIFValue_t *marpaESLIFValuep)
/*****************************************************************************/
{
  return marpaESLIFValuep->marpaESLIFRecognizerp->marpaESLIFGrammarp;
}

/*****************************************************************************/
static short _marpaESLIFActionCallback(void *userDatavp, char *names, size_t namel, char *inputs, size_t inputl, int arg0i, int argni, int resulti)
/*****************************************************************************/
{
  const static char *funcs            = "_marpaESLIFActionCallback";
  marpaESLIFValue_t *marpaESLIFValuep = (marpaESLIFValue_t *) userDatavp;

  /* This is our stuff, we know what we are doing */
}

/*****************************************************************************/
static inline void _marpaESLIFGrammar_freev(marpaESLIFGrammar_t *marpaESLIFGrammarp, short onStackb)
/*****************************************************************************/
{
  if (marpaESLIFGrammarp != NULL) {
    if (marpaESLIFGrammarp->grammarStackp != NULL) {
      while (GENERICSTACK_USED(marpaESLIFGrammarp->grammarStackp) > 0) {
	if (GENERICSTACK_IS_PTR(marpaESLIFGrammarp->grammarStackp, GENERICSTACK_USED(marpaESLIFGrammarp->grammarStackp) - 1)) {
	  marpaESLIF_grammar_t *grammarp = (marpaESLIF_grammar_t *) GENERICSTACK_POP_PTR(marpaESLIFGrammarp->grammarStackp);
	  _marpaESLIF_grammar_freev(grammarp);
	} else {
	  GENERICSTACK_USED(marpaESLIFGrammarp->grammarStackp)--;
	}
      }
      GENERICSTACK_FREE(marpaESLIFGrammarp->grammarStackp);
    }
    if (! onStackb) {
      free(marpaESLIFGrammarp);
    }
  }
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_lexemeStack_i_p_and_sizeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, char **pp, size_t *sizelp)
/*****************************************************************************/
{
  const static char                *funcs = "_marpaESLIFRecognizer_lexemeStack_i_p_and_sizeb";
  GENERICSTACKITEMTYPE2TYPE_ARRAYP  arrayp;
  char                             *p;
  size_t                            sizel;
  short                             rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  if (GENERICSTACK_IS_ARRAY(lexemeStackp, i)) {
    arrayp = GENERICSTACK_GET_ARRAYP(lexemeStackp, i);
    p = GENERICSTACK_ARRAYP_PTR(arrayp);
    sizel = GENERICSTACK_ARRAYP_LENGTH(arrayp);
  } else {
    MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "Bad type %s in lexeme stack at indice %d", _marpaESLIF_genericStack_i_types(lexemeStackp, i), i);
    goto err;
  }

  rcb = 1;
  *pp = p;
  *sizelp = sizel;
  goto done;

 err:
  rcb = 0;

 done:
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, size_t *sizelp)
/*****************************************************************************/
{
  const static char                *funcs = "_marpaESLIFRecognizer_lexemeStack_i_sizeb";
  GENERICSTACKITEMTYPE2TYPE_ARRAYP  arrayp;
  size_t                            sizel;
  short                             rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  if (GENERICSTACK_IS_ARRAY(lexemeStackp, i)) {
    arrayp = GENERICSTACK_GET_ARRAYP(lexemeStackp, i);
    sizel = GENERICSTACK_ARRAYP_LENGTH(arrayp);
  } else {
    MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "Bad type %s in lexeme stack at indice %d", _marpaESLIF_genericStack_i_types(lexemeStackp, i), i);
    goto err;
  }

  rcb = 1;
  *sizelp = sizel;
  goto done;

 err:
  rcb = 0;

 done:
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_lexemeStack_i_p(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, genericStack_t *lexemeStackp, int i, char **pp)
/*****************************************************************************/
{
  const static char                *funcs = "_marpaESLIFRecognizer_lexemeStack_i_p";
  GENERICSTACKITEMTYPE2TYPE_ARRAYP  arrayp;
  short                             rcb;
  char                             *p;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  if (GENERICSTACK_IS_ARRAY(lexemeStackp, i)) {
    arrayp = GENERICSTACK_GET_ARRAYP(lexemeStackp, i);
    p = GENERICSTACK_ARRAYP_PTR(arrayp);
  } else {
    MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "Bad type %s in lexeme stack at indice %d", _marpaESLIF_genericStack_i_types(lexemeStackp, i), i);
    goto err;
  }

  rcb = 1;
  *pp = p;
  goto done;

 err:
  rcb = 0;

 done:
  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline const char *_marpaESLIF_genericStack_i_types(genericStack_t *stackp, int i)
/*****************************************************************************/
{
  const char *s;

  switch (GENERICSTACKITEMTYPE(stackp, i)) {
  case GENERICSTACKITEMTYPE_NA:
    s = GENERICSTACKITEMTYPE_NA_STRING;
    break;
  case GENERICSTACKITEMTYPE_CHAR:
    s = GENERICSTACKITEMTYPE_CHAR_STRING;
    break;
  case GENERICSTACKITEMTYPE_SHORT:
    s = GENERICSTACKITEMTYPE_SHORT_STRING;
    break;
  case GENERICSTACKITEMTYPE_INT:
    s = GENERICSTACKITEMTYPE_INT_STRING;
    break;
  case GENERICSTACKITEMTYPE_LONG:
    s = GENERICSTACKITEMTYPE_LONG_STRING;
    break;
  case GENERICSTACKITEMTYPE_FLOAT:
    s = GENERICSTACKITEMTYPE_FLOAT_STRING;
    break;
  case GENERICSTACKITEMTYPE_DOUBLE:
    s = GENERICSTACKITEMTYPE_DOUBLE_STRING;
    break;
  case GENERICSTACKITEMTYPE_PTR:
    s = GENERICSTACKITEMTYPE_PTR_STRING;
    break;
  case GENERICSTACKITEMTYPE_ARRAY:
    s = GENERICSTACKITEMTYPE_ARRAY_STRING;
    break;
  default:
    s = GENERICSTACKITEMTYPE_UNKNOWN_STRING;
    break;
  }

  return s;
}

/*****************************************************************************/
static char *_marpaESLIFGrammar_symbolDescriptionCallback(void *userDatavp, int symboli)
/*****************************************************************************/
{
  const static char    *funcs              = "_marpaESLIFGrammar_symbolDescriptionCallback";
  marpaESLIFGrammar_t  *marpaESLIFGrammarp = (marpaESLIFGrammar_t *) userDatavp;
  marpaESLIF_grammar_t *grammarp           = marpaESLIFGrammarp->grammarp;
  genericStack_t       *symbolStackp       = grammarp->symbolStackp;
  marpaESLIF_symbol_t  *symbolp;

  if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
    return NULL;
  }
  symbolp = GENERICSTACK_GET_PTR(symbolStackp, symboli);

  return symbolp->descp->asciis;
}

/*****************************************************************************/
 static short _marpaESLIFGrammar_symbolOptionSetterOnlyStartCompletionEvent(void *userDatavp, int symboli, marpaWrapperGrammarSymbolOption_t *marpaWrapperGrammarSymbolOptionp)
/*****************************************************************************/
{
  const static char         *funcs                    = "_marpaESLIFGrammar_symbolOptionSetterOnlyStartCompletionEvent";
  marpaESLIF_cloneContext_t *marpaESLIF_cloneContextp = (marpaESLIF_cloneContext_t *) userDatavp;
  marpaESLIF_grammar_t      *grammarp                 = marpaESLIF_cloneContextp->grammarp;
  genericStack_t            *symbolStackp             = grammarp->symbolStackp;
  marpaESLIF_symbol_t       *symbolp;
  short                     rcb;

  if (! GENERICSTACK_IS_PTR(symbolStackp, symboli)) {
    MARPAESLIF_ERRORF(marpaESLIF_cloneContextp->marpaESLIFp, "Cannot find symbol No %d", symboli);
    goto err;
  }
  symbolp = (marpaESLIF_symbol_t *) GENERICSTACK_GET_PTR(symbolStackp, symboli);

  /* Consistenty check */
  if (symbolp->idi != symboli) {
    MARPAESLIF_ERRORF(marpaESLIF_cloneContextp->marpaESLIFp, "Clone symbol callback for symbol No %d while we have %d !?", symboli, symbolp->idi);
    goto err;
  }

  if (symbolp->discardb) {
    if (marpaWrapperGrammarSymbolOptionp->eventSeti != MARPAWRAPPERGRAMMAR_EVENTTYPE_COMPLETION) {
      MARPAESLIF_TRACEF(marpaESLIF_cloneContextp->marpaESLIFp, funcs, "Setting event set for symbol No %d (%s) at grammar level %d (%s) on completion only", symbolp->idi, symbolp->descp->asciis, grammarp->leveli, grammarp->descp->asciis);
      marpaWrapperGrammarSymbolOptionp->eventSeti = MARPAWRAPPERGRAMMAR_EVENTTYPE_COMPLETION;
    }
  } else {
    if (marpaWrapperGrammarSymbolOptionp->eventSeti != MARPAWRAPPERGRAMMAR_EVENTTYPE_NONE) {
      MARPAESLIF_TRACEF(marpaESLIF_cloneContextp->marpaESLIFp, funcs, "Resetting event set for symbol No %d (%s) at grammar level %d (%s)", symbolp->idi, symbolp->descp->asciis, grammarp->leveli, grammarp->descp->asciis);
      marpaWrapperGrammarSymbolOptionp->eventSeti = MARPAWRAPPERGRAMMAR_EVENTTYPE_NONE;
    }
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_readb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp)
/*****************************************************************************/
/*
  In the recognizer:
  - buffers is containing unparsed data, and its size can vary at any time. ALWAYS SHARED with all parents.
  - inputs is always a pointer within buffers.                              ALWAYS SPECIFIC to current recognizer.
  - eofb is when EOF is hit.                                                ALWAYS SHARED with all parents.

  Handled in regex match:
  - encodings is eventual encoding information as per the user              ALWAYS SHARED with all parents.
  - utf8s is the UTF-8 conversion of buffer. Handled in regex match.        ALWAYS SHARED with all parents.
  
  Remember the semantics: from our point of view, reader is reading NEW data. We always append.
*/
{
  const static char            *funcs                      = "_marpaESLIFRecognizer_readb";
  marpaESLIFRecognizerOption_t  marpaESLIFRecognizerOption = marpaESLIFRecognizerp->marpaESLIFRecognizerOption;
  char                         *inputs                     = NULL;
  char                         *encodings                  = NULL;
  size_t                        inputl                     = 0;
  short                         eofb                       = 0;
  short                         characterStreamb           = 0;
  char                         *utf8s                      = NULL;
  size_t                        utf8l;
  short                         rcb;
  marpaESLIF_matcher_value_t    rci;
  genericStack_t                outputStack;
  genericStack_t               *outputStackp = NULL;
  char                         *buffers;
  size_t                        bufferl;
  size_t                        sizel;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  if (! marpaESLIFRecognizerOption.marpaESLIFReaderCallbackp(marpaESLIFRecognizerOption.userDatavp, &inputs, &inputl, &eofb, &characterStreamb, &encodings)) {
    MARPAESLIF_ERROR(marpaESLIFRecognizerp->marpaESLIFp, "reader failure");
    goto err;
  }

  if ((inputs != NULL) && (inputl > 0)) {
    /* Some new data is coming - remember delta before doing anything */
    size_t deltal = marpaESLIFRecognizerp->inputs - *(marpaESLIFRecognizerp->buffersp);

    if (characterStreamb) {
      /* User says this is a stream of characters */
      /* Input is systematically converted into UTF-8. If user said "UTF-8" it is equivalent to */
      /* an UTF-8 validation. The user MUST send a buffer information that contain full characters.          */
      utf8s = _marpaESLIF_charconvp(marpaESLIFRecognizerp->marpaESLIFp, "UTF-8", encodings, inputs, inputl, &utf8l, NULL /* fromEncodingsp */);
      if (utf8s == NULL) {
        goto err;
      }
      inputs = utf8s;
      inputl = utf8l;

      /* Remember NOW it has been converted. This has an impact on the regex call below */
      *(marpaESLIFRecognizerp->utfbp) = 1;

      /* Remove eventually the BOM */
      outputStackp = &outputStack;
      GENERICSTACK_INIT(outputStackp);
      if (GENERICSTACK_ERROR(outputStackp)) {
        outputStackp = NULL;
        goto err;
      }
      if (! _marpaESLIFRecognizer_regex_matcherb(marpaESLIFRecognizerp, marpaESLIFRecognizerp->marpaESLIFp->utf8bomp, inputs, inputl, 1 /* eofb */, &rci, outputStackp)) {
        goto err;
      }
      if (rci == MARPAESLIF_MATCH_OK) {
        if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, outputStackp, 0, &sizel)) {
          goto err;
        }
        inputs += sizel;
        inputl -= sizel;
        if (inputl <= 0) {
          /* Only the BOM !? */
          goto empty_without_bom;
        }
      }
    } else {
      *(marpaESLIFRecognizerp->utfbp) = 0;
    }

    buffers = *(marpaESLIFRecognizerp->buffersp);
    bufferl = *(marpaESLIFRecognizerp->bufferlp);

    if (marpaESLIFRecognizerp->parentRecognizerp == NULL) {
      /* We can crunch data at any time. */
      /* Unless we are a child reognizer: our parent is stucked on its {inputs,inputl} tuple. */
      /* We decide abritrarily than when inputl is < half of buffer size, the later can be reduced */
      size_t bufferl2 = (size_t) (bufferl / (size_t) 2);

      if (marpaESLIFRecognizerp->inputl < bufferl2) {
        size_t newbufferl = bufferl - marpaESLIFRecognizerp->inputl;
        char  *tmps;

        /* This mean that buffers is not NULL and its size is at least two bytes btw */
        memmove(buffers, buffers + marpaESLIFRecognizerp->inputl, newbufferl);
        /* Try to realloc */
        tmps = realloc(buffers, newbufferl);
        if (tmps == NULL) {
          /* We could have continue, this is not truely fatal - but we are in a bad shape anyway -; */
          MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "realloc failure, %s", strerror(errno));
          goto err;
        }
        *(marpaESLIFRecognizerp->buffersp) = tmps;
        *(marpaESLIFRecognizerp->bufferlp) = newbufferl;
        marpaESLIFRecognizerp->inputs  = *(marpaESLIFRecognizerp->buffersp);
        /* inputl does not change yet - we just crunched data */
        buffers = tmps;
        bufferl = newbufferl;
      }
    }

    /* Append data, as we do in the remembering mode */
    if (buffers == NULL) {
      /* First time we put in the buffer */
      buffers = (char *) malloc(inputl);
      if (buffers == NULL) {
        MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "malloc failure, %s", strerror(errno));
        goto err;
      }
    } else {
      char *tmps = realloc(buffers, bufferl + inputl);
      if (tmps == NULL) {
        MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "realloc failure, %s", strerror(errno));
        goto err;
      }
      buffers = tmps;
    }
    memcpy(buffers + bufferl, inputs, inputl);
    bufferl += inputl;

    (*marpaESLIFRecognizerp->buffersp) = buffers;
    (*marpaESLIFRecognizerp->bufferlp) = bufferl;

    /* Update current pointers */
    marpaESLIFRecognizerp->inputs = (*marpaESLIFRecognizerp->buffersp) + deltal;
    marpaESLIFRecognizerp->inputl = (*marpaESLIFRecognizerp->bufferlp) - deltal;

    /* Please see the free method for the impact on parent's current pointer in input   */
    /* This need to be done once only, at return, this is why it is done at free level. */
    /* Note that when we create a grand child we strip off ALL events, so the user can */
    /* never got control back until we are finished. I.e. until all the free methods of */
    /* all the children are executed -; */
  }

 empty_without_bom:
  rcb = 1;
  (*marpaESLIFRecognizerp->eofbp) = eofb;
  goto done;

 err:
  rcb = 0;

 done:
  if (outputStackp != NULL) {
    GENERICSTACK_RESET(outputStackp);
  }
  if (utf8s != NULL) {
    free(utf8s);
  }

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline void _marpaESLIF_rule_createshowv(marpaESLIF_t *marpaESLIFp, marpaESLIF_rule_t *rulep, char *asciishows, size_t *asciishowlp)
/*****************************************************************************/
{
  genericStack_t      *rhsStackp       = rulep->rhsStackp;
  genericStack_t      *maskStackp      = rulep->maskStackp;
  genericStack_t      *exceptionStackp = rulep->exceptionStackp;
  marpaESLIF_symbol_t *symbolp;
  short                maskb;
  size_t               asciishowl;
  int                  exceptioni;
  size_t               exceptionl;
  int                  rhsi;
  char                 tmps[1024];

  /* Calculate the size needed to show the rule in ASCII form */
  asciishowl = strlen(rulep->lhsp->descp->asciis); /* "LHS" */
  if (asciishows != NULL) {
    strcpy(asciishows, rulep->lhsp->descp->asciis);
  }
  asciishowl += 4;                              /* " ::=" */
  if (asciishows != NULL) {
    strcat(asciishows, " ::=");
  }
  for (rhsi = 0; rhsi < GENERICSTACK_USED(rhsStackp); rhsi++) {
    if (! GENERICSTACK_IS_PTR(rhsStackp, rhsi)) {
      continue;
    }
    if (! GENERICSTACK_IS_SHORT(maskStackp, rhsi)) {
      continue;
    }
    symbolp = GENERICSTACK_GET_PTR(rhsStackp, rhsi);
    maskb   = GENERICSTACK_GET_SHORT(maskStackp, rhsi);

    asciishowl++;                              /* space */
    if (asciishows != NULL) {
      strcat(asciishows, " ");
    }
    if (maskb) {
      asciishowl++;                         /* Surrounded by ( */
      if (asciishows != NULL) {
        strcat(asciishows, "(");
      }
    }
    asciishowl += strlen(symbolp->descp->asciis); /* RHS description (ASCII) */
    if (asciishows != NULL) {
      strcat(asciishows, symbolp->descp->asciis);
    }
    if (maskb) {
      asciishowl++;                         /* Surrounded by ) */
      if (asciishows != NULL) {
        strcat(asciishows, ")");
      }
    }
  }
  exceptionl = 0;
  for (exceptioni = 0; exceptioni < GENERICSTACK_USED(exceptionStackp); exceptioni++) {
    if (! GENERICSTACK_IS_PTR(exceptionStackp, exceptioni)) {
      continue;
    }
    exceptionl++;
  }
  if (rulep->sequenceb) {
    asciishowl++;                                 /* '*' or '+' */
    if (asciishows != NULL) {
      strcat(asciishows, (rulep->minimumi == 0) ? "*" : "+");
    }
    if (rulep->separatorp != NULL) {
      asciishowl++;                                        /* space */
      if (asciishows != NULL) {
        strcat(asciishows, " ");
      }
      asciishowl += strlen("separator => ");               /* "separator =>" */
      if (asciishows != NULL) {
        strcat(asciishows, "separator => ");
      }
      asciishowl += strlen(rulep->separatorp->descp->asciis); /* Separator description (ASCII) */
      if (asciishows != NULL) {
        strcat(asciishows, rulep->separatorp->descp->asciis);
      }
    }
    if (rulep->properb) {
      asciishowl++;                                 /* space */
      if (asciishows != NULL) {
        strcat(asciishows, " ");
      }
      asciishowl += strlen("proper => 1");          /* "proper => 1" */
      if (asciishows != NULL) {
        strcat(asciishows, "proper => 1");
      }
    }
  }
  if (exceptionl > 0) {
    asciishowl++;                            /* space */
    if (asciishows != NULL) {
      strcat(asciishows, " ");
    }
    asciishowl++;                            /* Surround exception by ( */
    if (asciishows != NULL) {
      strcat(asciishows, "(");
    }
    exceptionl = 0;
    for (exceptioni = 0; exceptioni < GENERICSTACK_USED(exceptionStackp); exceptioni++) {
      if (! GENERICSTACK_IS_PTR(exceptionStackp, exceptioni)) {
        continue;
      }
      symbolp = GENERICSTACK_GET_PTR(exceptionStackp, exceptioni);
      if (exceptionl > 0) {
        asciishowl++;                            /* space */
        if (asciishows != NULL) {
          strcat(asciishows, " ");
        }
      }
      asciishowl += strlen(symbolp->descp->asciis); /* Exception description (ASCII) */
      if (asciishows != NULL) {
        strcat(asciishows, symbolp->descp->asciis);
      }
    }
    asciishowl++;                             /* Surround exception by ) */
    if (asciishows != NULL) {
      strcat(asciishows, ")");
    }
  }
  if (rulep->ranki != 0) {
    asciishowl++;                            /* space */
    if (asciishows != NULL) {
      strcat(asciishows, " ");
    }
    asciishowl += strlen("rank => ");        /* "rank => " */
    if (asciishows != NULL) {
      strcat(asciishows, "rank => ");
    }
    /* Will an "int" ever have more than 1023 digits ? */
    sprintf(tmps, "%d", rulep->ranki);
    asciishowl += strlen(tmps);
    if (asciishows != NULL) {
      strcat(asciishows, tmps);
    }
  }
  if (rulep->nullRanksHighb) {
    asciishowl++;                                 /* space */
    if (asciishows != NULL) {
      strcat(asciishows, " ");
    }
    asciishowl += strlen("null-ranking => high"); /* "null-ranking => high */
    if (asciishows != NULL) {
      strcat(asciishows, "null-ranking => high");
    }
  }
  asciishowl++; /* NUL byte */

  if (asciishowlp != NULL) {
    *asciishowlp = asciishowl;
  }
}

/*****************************************************************************/
static inline int _marpaESLIF_utf82ordi(PCRE2_SPTR8 utf8bytes, marpaESLIF_uint32_t *uint32p)
/*****************************************************************************/
/* This is a copy of utf2ord from pcre2test.c
-----------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the University of Cambridge nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------
/* This function reads one or more bytes that represent a UTF-8 character,
and returns the codepoint of that character. Note that the function supports
the original UTF-8 definition of RFC 2279, allowing for values in the range 0
to 0x7fffffff, up to 6 bytes long. This makes it possible to generate
codepoints greater than 0x10ffff which are useful for testing PCRE2's error
checking, and also for generating 32-bit non-UTF data values above the UTF
limit.

Argument:
  utf8bytes   a pointer to the byte vector
  vptr        a pointer to an int to receive the value

Returns:      >  0 => the number of bytes consumed
              -6 to 0 => malformed UTF-8 character at offset = (-return)
*/
/*****************************************************************************/
{
  marpaESLIF_uint32_t c = *utf8bytes++;
  marpaESLIF_uint32_t d = c;
  int i, j, s;
  const int utf8_table1[] = { 0x7f, 0x7ff, 0xffff, 0x1fffff, 0x3ffffff, 0x7fffffff};
  const int utf8_table3[] = { 0xff, 0x1f, 0x0f, 0x07, 0x03, 0x01};
  const int utf8_table1_size = sizeof(utf8_table1) / sizeof(int);

  for (i = -1; i < 6; i++) {               /* i is number of additional bytes */
    if ((d & 0x80) == 0) break;
    d <<= 1;
  }

  if (i == -1) {
    /* ascii character */
    *uint32p = c;
    return 1;
  }
  if (i == 0 || i == 6) {
    return 0;
  } /* invalid UTF-8 */

  /* i now has a value in the range 1-5 */

  s = 6*i;
  d = (c & utf8_table3[i]) << s;

  for (j = 0; j < i; j++) {
    c = *utf8bytes++;
    if ((c & 0xc0) != 0x80) {
      return -(j+1);
    }
    s -= 6;
    d |= (c & 0x3f) << s;
  }

  /* Check that encoding was the correct unique one */

  for (j = 0; j < utf8_table1_size; j++) {
    if (d <= (uint32_t)utf8_table1[j]) {
      break;
    }
  }
  if (j != i) {
    return -(i+1);
  }

  /* Valid value */

  *uint32p = d;
  return i+1;
}

/*****************************************************************************/
static inline short _marpaESLIFRecognizer_matchPostProcessingb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, size_t matchl)
/*****************************************************************************/
{
  const static char          *funcs = "_marpaESLIFRecognizer_matchPostProcessingb";
  marpaESLIF_terminal_t      *newlinep;
  marpaESLIF_terminal_t      *anycharp;
  char                       *linep;
  size_t                      linel;
  size_t                      matchedl;
  genericStack_t              lexemeStack;
  genericStack_t             *lexemeStackp = NULL;
  marpaESLIF_matcher_value_t  rci;
  short                       rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  /* If newline counting is on, so do we - only at first level */
  if (marpaESLIFRecognizerp->marpaESLIFRecognizerOption.newlineb && (marpaESLIFRecognizerp->leveli == 0)) {
    newlinep = marpaESLIFRecognizerp->marpaESLIFp->newlinep;
    anycharp = marpaESLIFRecognizerp->marpaESLIFp->anycharp;
    linep = marpaESLIFRecognizerp->inputs;
    linel = matchl;
    lexemeStackp = &lexemeStack;

    GENERICSTACK_INIT(lexemeStackp);
    if (GENERICSTACK_ERROR(lexemeStackp)) {
      MARPAESLIF_ERRORF(marpaESLIFRecognizerp->marpaESLIFp, "lexemeStackp intialization error, %s", strerror(errno));
      lexemeStackp = NULL;
      goto err;
    }

    /* Check newline */
    while (1) {
      _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizerp, lexemeStackp);
      /* We count newlines only when a discard or a complete has happened. So by definition */
      /* character sequences are complete. This is why we fake EOF to true. */
      if (! _marpaESLIFRecognizer_regex_matcherb(marpaESLIFRecognizerp,
                                                 newlinep,
                                                 linep,
                                                 linel,
                                                 1, /* eofb */
                                                 &rci,
                                                 lexemeStackp)) {
        goto err;
      }
      if (rci != MARPAESLIF_MATCH_OK) {
        break;
      }
      if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, lexemeStackp, 0, &matchedl)) {
        goto err;
      }
      if (matchedl <= 0) {
        MARPAESLIF_ERROR(marpaESLIFRecognizerp->marpaESLIFp, "newline regex matched zero byte");
        goto err;
      }
      linep += matchedl;
      linel -= matchedl;
      /* A new line, reset column count */
      marpaESLIFRecognizerp->linel++;
      marpaESLIFRecognizerp->columnl = 0;
    }

    if (linel > 0) {
      /* Count characters */
      while (1) {
        /* We can re-use lexemeStack -; */
        _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizerp, lexemeStackp);
        /* We count newlines only when a discard or a complete has happened. So by definition */
        /* character sequences are complete. This is why we fake EOF to true. */
        if (! _marpaESLIFRecognizer_regex_matcherb(marpaESLIFRecognizerp,
                                                   anycharp,
                                                   linep,
                                                   linel,
                                                   1, /* eofb */
                                                   &rci,
                                                   lexemeStackp)) {
          goto err;
        }
        if (rci != MARPAESLIF_MATCH_OK) {
          break;
        }
        if (! _marpaESLIFRecognizer_lexemeStack_i_sizeb(marpaESLIFRecognizerp, lexemeStackp, 0, &matchedl)) {
          goto err;
        }
        if (matchedl <= 0) {
          MARPAESLIF_ERROR(marpaESLIFRecognizerp->marpaESLIFp, "anychar regex matched zero byte");
          goto err;
        }
        linep += matchedl;
        linel -= matchedl;
        /* A new character */
        marpaESLIFRecognizerp->columnl++;
      }
    }
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  _marpaESLIFrecognizer_lexemeStack_resetv(marpaESLIFRecognizerp, lexemeStackp);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
static inline marpaESLIF_action_t *_marpaESLIF_action_newp(marpaESLIF_t *marpaESLIFp, marpaESLIF_action_type_t type, genericStack_t *actionStackp, char *encodings, char *bytep, size_t bytel)
/*****************************************************************************/
{
  marpaESLIF_action_t *actionp;

  actionp = (marpaESLIF_action_t *) malloc(sizeof(marpaESLIF_action_t));
  if (actionp == NULL) {
    MARPAESLIF_ERRORF(marpaESLIFp, "malloc failure, %s", strerror(errno));
    goto err;
  }

  actionp->type = MARPAESLIF_ACTION_TYPE_NA;
  /* Rest is undef at this stage */

  switch (type) {
  case MARPAESLIF_ACTION_TYPE_NA:         /* None */
  case MARPAESLIF_ACTION_TYPE_ARRAY:      /* ::array */
  case MARPAESLIF_ACTION_TYPE_FIRST:      /* ::first */
  case MARPAESLIF_ACTION_TYPE_UNDEF:      /* ::undef */
    break;
  case MARPAESLIF_ACTION_TYPE_COMPOSITE:  /* [] */
    break;
  case MARPAESLIF_ACTION_TYPE_NAME:       /* c.f. action callback */
    break;
  case MARPAESLIF_ACTION_TYPE_INTERNAL_LEXEME: /* Internal action that is always produce a "lexeme", i.e. an genericStack's array item */
    break;
  default:
    MARPAESLIF_ERRORF(marpaESLIFp, "Unsupported action type %d", type);
    goto err;    
  }
  
  goto done;
  
 err:
  _marpaESLIF_action_freev(actionp);
  actionp = NULL;

 done:
  return actionp;
}

/*****************************************************************************/
static inline void _marpaESLIF_action_freev(marpaESLIF_action_t *actionp)
/*****************************************************************************/
{
  if (actionp != NULL) {
    switch (actionp->type) {
    case MARPAESLIF_ACTION_TYPE_COMPOSITE:
      break;
    case MARPAESLIF_ACTION_TYPE_NAME:
      _marpaESLIF_string_freev(actionp->u.stringp);
      break;
    default:
      break;
    }
    free(actionp);
  }
}

/*****************************************************************************/
short marpaESLIFRecognizer_progressLogb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, int starti, int endi, genericLoggerLevel_t logleveli, void *userDatavp, marpaESLISymbolDescriptionCallback_t symbolDescriptionCallbackp)
/*****************************************************************************/
{
  const static char *funcs = "marpaESLIFRecognizer_progressLogb";
  short              rcb;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  rcb = marpaWrapperRecognizer_progressLogb(marpaESLIFRecognizerp->marpaWrapperRecognizerp,
                                            starti,
                                            endi,
                                            logleveli,
                                            userDatavp,
                                            symbolDescriptionCallbackp);

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %d", (int) rcb);
  marpaESLIFRecognizerp->callstackCounteri--;
  return rcb;
}

/*****************************************************************************/
marpaESLIFRecognizer_t *marpaESLIFValue_recognizerp(marpaESLIFValue_t *marpaESLIFValuep)
/*****************************************************************************/
{
  if (marpaESLIFValuep == NULL) {
    errno = EINVAL;
    return NULL;
  }

  return marpaESLIFValuep->marpaESLIFRecognizerp;
}

/*****************************************************************************/
marpaESLIFGrammar_t *marpaESLIFRecognizer_grammarp(marpaESLIFRecognizer_t *marpaESLIFRecognizerp)
/*****************************************************************************/
{
  const static char   *funcs = "marpaESLIFRecognizer_grammarp";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;

  marpaESLIFRecognizerp->callstackCounteri++;
  MARPAESLIFRECOGNIZER_TRACE(marpaESLIFRecognizerp, funcs, "start");

  marpaESLIFGrammarp = marpaESLIFRecognizerp->marpaESLIFGrammarp;

  MARPAESLIFRECOGNIZER_TRACEF(marpaESLIFRecognizerp, funcs, "return %p", marpaESLIFGrammarp);
  marpaESLIFRecognizerp->callstackCounteri--;
  return marpaESLIFGrammarp;
}

