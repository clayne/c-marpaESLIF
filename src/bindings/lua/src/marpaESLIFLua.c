/* Only 5.2 and upwards is supported */

#include "lua.h"        /* As per CMake doc */
#include "lauxlib.h"    /* As per CMake doc */
#include "lualib.h"     /* As per CMake doc */
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <marpaESLIFLua.h>
#include <marpaESLIF.h>
#include <genericStack.h>

/* To ease me debugging -; */
static void marpaESLIFLua_stackdumpv(lua_State* L, int forcelookupi);

/* Global table for the multiton pattern */
#define MARPAESLIFMULTITONSTABLE "__marpaESLIFLuaMultitonsTable"
/* Every key   is a marpaESLIFLuaContext light userdata */
/* Every value is a reference to the logger (reference to nil if there is none) */

/* marpaESLIFLua correctly unreferences everything from the valuation, but it you */
/* prefer it not to use the global registry, then uncomment the next line -; */
/* #define MARPAESLIFLUA_USE_INTERNALREGISTRYINDEX */

#ifdef MARPAESLIFLUA_USE_INTERNALREGISTRYINDEX
/* Global table for our references */
#define MARPAESLIFLUAREGISTRYINDEX "__marpaESLIFLuaRegistryindex"
#endif

#ifndef MARPAESLIFLUA_CONTEXT
static char _MARPAESLIFLUA_CONTEXT;
#define MARPAESLIFLUA_CONTEXT &_MARPAESLIFLUA_CONTEXT
#endif /* MARPAESLIFLUA_CONTEXT */

/* ESLIF proxy context */
/* This one is special because it is returned by marpaESLIFLua_marpaESLIF_new(?From)?i and         */
/* marpaESLIFLua_marpaESLIF_newFromUnmanagedi.                                                     */
/* In the first case, it is guaranteed that we own it and it is also stored in the multiton table. */
/* In the second case, we never own it and it is never stored in the multiton table.               */
/* We know the destroy workflows because there are two different entry points:                     */
/* - marpaESLIFLua_marpaESLIFMultitonsTable_freei                                                  */
/* - marpaESLIFLua_marpaESLIF_freei                                                                */
typedef struct marpaESLIFLuaContext {
  lua_State    *L;           /* Lua state */
  marpaESLIF_t *marpaESLIFp;
  short         multitonb;
} marpaESLIFLuaContext_t;

/* Logger proxy context */
typedef struct marpaESLIFLuaGenericLoggerContext {
  lua_State *L; /* Lua state */
  int logger_r; /* Lua logger reference */
} marpaESLIFLuaGenericLoggerContext_t;

/* Grammar proxy context */
typedef struct marpaESLIFLuaGrammarContext {
  lua_State              *L;                      /* Lua state */
  int                     eslif_r;                /* Lua eslif reference */
  marpaESLIFGrammar_t    *marpaESLIFGrammarp;
  short                   managedb;               /* True if we own marpaESLIFGrammarp */
} marpaESLIFLuaGrammarContext_t;

/* Recognizer proxy context */
typedef struct marpaESLIFLuaRecognizerContext {
  lua_State              *L;                      /* Lua state */
  int                     grammar_r;              /* Lua grammar reference */
  int                     recognizerInterface_r;  /* Lua recognizer interface reference */
  int                     recognizer_orig_r;      /* Lua original recognizer reference in case of newFrom() */
  char                   *previousInputs;         /* Because we want to maintain lifetime of inputs lua string outside of stack */
  char                   *previousEncodings;      /* Because we want to maintain lifetime of encodings lua string outside of stack */
  genericStack_t         *lexemeStackp;
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  short                   managedb;               /* True if we own marpaESLIFRecognizerp */
} marpaESLIFLuaRecognizerContext_t;

/* Value proxy context */
typedef struct marpaESLIFLuaValueContext {
  lua_State         *L;                     /* Lua state */
  int                valueInterface_r;      /* Lua value reference */
  int                recognizerInterface_r; /* Lua recognizer reference - can be LUA_NOREF */
  int                grammar_r;             /* Lua grammar reference - can be LUA_NOREF */
  char              *actions;               /* Shallow copy of last resolved name */
  void              *previous_strings;      /* Latest stringification result */
  marpaESLIFValue_t *marpaESLIFValuep;
  short              managedb;              /* True if we own marpaESLIFValuep */
  char              *symbols;
  int                symboli;
  char              *rules;
  int                rulei;
  int                result_r;              /* Reference to last result */
} marpaESLIFLuaValueContext_t;

static short                           marpaESLIFLua_paramIsLoggerInterfaceOrNilb(lua_State *L, int stacki);
static short                           marpaESLIFLua_paramIsRecognizerInterfacev(lua_State *L, int stacki);
static short                           marpaESLIFLua_paramIsValueInterfacev(lua_State *L, int stacki);
static short                           marpaESLIFLua_contextInitb(lua_State *L, marpaESLIFLuaContext_t *marpaESLIFLuaContextp, short unmanagedb);
static void                            marpaESLIFLua_contextFreev(marpaESLIFLuaContext_t *marpaESLIFLuaContextp, short multitonDestroyModeb);
static short                           marpaESLIFLua_grammarContextInitb(lua_State *L, int eslifStacki, marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp, short unmanagedb);
static short                           marpaESLIFLua_recognizerContextInitb(lua_State *L, int grammarStacki, int recognizerInterfaceStacki, int recognizerOrigStacki, marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp, short unmanagedb);
static short                           marpaESLIFLua_valueContextInitb(lua_State *L, int grammarStacki, int recognizerStacki, int valueInterfaceStacki, marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp, short unmanagedb, short grammarStackiCanBeZerob);
static void                            marpaESLIFLua_grammarContextFreev(marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp, short onStackb);
static void                            marpaESLIFLua_recognizerContextCleanupv(marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp);
static void                            marpaESLIFLua_recognizerContextFreev(marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp, short onStackb);
static void                            marpaESLIFLua_valueContextCleanupv(marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp);
static void                            marpaESLIFLua_valueContextFreev(marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp, short onStackb);
static void                            marpaESLIFLua_genericLoggerCallbackv(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs);
static int                             marpaESLIFLua_installi(lua_State *L);
static int                             marpaESLIFLua_versioni(lua_State *L);
static int                             marpaESLIFLua_versionMajori(lua_State *L);
static int                             marpaESLIFLua_versionMinori(lua_State *L);
static int                             marpaESLIFLua_versionPatchi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIF_newi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIF_freei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIF_versioni(lua_State *L);
static int                             marpaESLIFLua_marpaESLIF_versionMajori(lua_State *L);
static int                             marpaESLIFLua_marpaESLIF_versionMinori(lua_State *L);
static int                             marpaESLIFLua_marpaESLIF_versionPatchi(lua_State *L);
#ifdef MARPAESLIFLUA_EMBEDDED
static int                             marpaESLIFLua_marpaESLIF_newFromUnmanagedi(lua_State *L, marpaESLIF_t *marpaESLIFUnmanagedp);
#endif
static int                             marpaESLIFLua_marpaESLIFMultitonsTable_freei(lua_State *L);
#ifdef MARPAESLIFLUA_USE_INTERNALREGISTRYINDEX
static int                             marpaESLIFLua_marpaESLIFRegistryindex_freei(lua_State *L);
#endif
static int                             marpaESLIFLua_marpaESLIFGrammar_newi(lua_State *L);
#ifdef MARPAESLIFLUA_EMBEDDED
static int                             marpaESLIFLua_marpaESLIFGrammar_newFromUnmanagedi(lua_State *L, marpaESLIFGrammar_t *marpaESLIFGrammarUnmanagedp);
#endif
static int                             marpaESLIFLua_marpaESLIFGrammar_freei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_ngrammari(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_currentLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_currentDescriptioni(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_descriptionByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_currentRuleIdsi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_ruleIdsByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_currentSymbolIdsi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_symbolIdsByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_currentPropertiesi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_propertiesByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_currentRulePropertiesi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_rulePropertiesByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_currentSymbolPropertiesi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_symbolPropertiesByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_ruleDisplayi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_symbolDisplayi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_ruleShowi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_ruleDisplayByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_symbolDisplayByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_ruleShowByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_showi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_showByLeveli(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFGrammar_parsei(lua_State *L);
static short                           marpaESLIFLua_readerCallbackb(void *userDatavp, char **inputcpp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingsp, size_t *encodinglp);
static marpaESLIFValueRuleCallback_t   marpaESLIFLua_valueRuleActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions);
static marpaESLIFValueSymbolCallback_t marpaESLIFLua_valueSymbolActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions);
static marpaESLIFValueFreeCallback_t   marpaESLIFLua_valueFreeActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions);
static short                           marpaESLIFLua_valueRuleCallbackb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb);
static short                           marpaESLIFLua_valueSymbolCallbackb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *bytep, size_t bytel, int resulti);
static void                            marpaESLIFLua_valueFreeCallbackv(void *userDatavp, void *contextp, marpaESLIFValueType_t type, void *p, size_t sizel);
static short                           marpaESLIFLua_transformUndefb(void *userDatavp, void *contextp);
static short                           marpaESLIFLua_transformCharb(void *userDatavp, void *contextp, char c);
static short                           marpaESLIFLua_transformShortb(void *userDatavp, void *contextp, short b);
static short                           marpaESLIFLua_transformIntb(void *userDatavp, void *contextp, int i);
static short                           marpaESLIFLua_transformLongb(void *userDatavp, void *contextp, long l);
static short                           marpaESLIFLua_transformFloatb(void *userDatavp, void *contextp, float f);
static short                           marpaESLIFLua_transformDoubleb(void *userDatavp, void *contextp, double d);
static short                           marpaESLIFLua_transformPtrb(void *userDatavp, void *contextp, void *p);
static short                           marpaESLIFLua_transformArrayb(void *userDatavp, void *contextp, void *p, size_t sizel);
static short                           marpaESLIFLua_transformBoolb(void *userDatavp, void *contextp, short b);
static short                           marpaESLIFLua_pushValueb(marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp, marpaESLIFValue_t *marpaESLIFValuep, int stackindicei, char *bytep, size_t bytel);
static short                           marpaESLIFLua_representationb(void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp, char **inputcpp, size_t *inputlp);
static void                            marpaESLIFLua_iterate_and_print(lua_State *L, int index);
static int                             marpaESLIFLua_marpaESLIFRecognizer_newi(lua_State *L);
#ifdef MARPAESLIFLUA_EMBEDDED
static int                             marpaESLIFLua_marpaESLIFRecognizer_newFromUnmanagedi(lua_State *L, marpaESLIFRecognizer_t *marpaESLIFRecognizerUnmanagedp);
#endif
static int                             marpaESLIFLua_marpaESLIFRecognizer_freei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_newFromi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_set_exhausted_flagi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_sharei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_isCanContinuei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_isExhaustedi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_scani(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_resumei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_eventsi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_eventOnOffi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lexemeAlternativei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lexemeCompletei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lexemeReadi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lexemeTryi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_discardTryi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lexemeExpectedi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lexemeLastPausei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lexemeLastTryi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_discardLastTryi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_isEofi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_readi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_inputi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_progressLogi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lastCompletedOffseti(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lastCompletedLengthi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_lastCompletedLocationi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_linei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_columni(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_locationi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFRecognizer_hookDiscardi(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFValue_newi(lua_State *L);
#ifdef MARPAESLIFLUA_EMBEDDED
static int                             marpaESLIFLua_marpaESLIFValue_newFromUnmanagedi(lua_State *L, marpaESLIFValue_t *marpaESLIFValueUnmanagedp);
#endif
static int                             marpaESLIFLua_marpaESLIFValue_freei(lua_State *L);
static int                             marpaESLIFLua_marpaESLIFValue_valuei(lua_State *L);

/* Transformers */
static marpaESLIFValueResultTransform_t marpaESLIFLuaValueResultTransformDefault = {
  marpaESLIFLua_transformUndefb,
  marpaESLIFLua_transformCharb,
  marpaESLIFLua_transformShortb,
  marpaESLIFLua_transformIntb,
  marpaESLIFLua_transformLongb,
  marpaESLIFLua_transformFloatb,
  marpaESLIFLua_transformDoubleb,
  marpaESLIFLua_transformPtrb,
  marpaESLIFLua_transformArrayb,
  marpaESLIFLua_transformBoolb
};

#define MARPAESLIFLUA_NOOP

static short marpaESLIFLua_lua_pushinteger(lua_State *L, lua_Integer n);
static short marpaESLIFLua_lua_setglobal (lua_State *L, const char *name);
static short marpaESLIFLua_lua_getglobal (int *luaip, lua_State *L, const char *name);
static short marpaESLIFLua_lua_type(int *luaip, lua_State *L, int index);
static short marpaESLIFLua_lua_pop(lua_State *L, int n);
static short marpaESLIFLua_lua_newtable(lua_State *L);
static short marpaESLIFLua_lua_pushcfunction(lua_State *L, lua_CFunction f);
static short marpaESLIFLua_lua_setfield(lua_State *L, int index, const char *k);
static short marpaESLIFLua_lua_setmetatable(lua_State *L, int index);
static short marpaESLIFLua_lua_insert(lua_State *L, int index);
static short marpaESLIFLua_lua_rawgeti(int *luaip, lua_State *L, int index, lua_Integer n);
static short marpaESLIFLua_lua_remove(lua_State *L, int index);
static short marpaESLIFLua_lua_createtable(lua_State *L, int narr, int nrec);
static short marpaESLIFLua_lua_rawseti(lua_State *L, int index, lua_Integer i);
static short marpaESLIFLua_lua_pushstring(const char **luasp, lua_State *L, const char *s);
static short marpaESLIFLua_lua_pushlstring(const char **luasp, lua_State *L, const char *s, size_t len);
static short marpaESLIFLua_lua_pushnil(lua_State *L);
static short marpaESLIFLua_lua_getfield(int *luaip, lua_State *L, int index, const char *k);
static short marpaESLIFLua_lua_call(lua_State *L, int nargs, int nresults);
static short marpaESLIFLua_lua_settop(lua_State *L, int index);
static short marpaESLIFLua_lua_copy(lua_State *L, int fromidx, int toidx);
static short marpaESLIFLua_lua_rawsetp(lua_State *L, int index, const void *p);
static short marpaESLIFLua_lua_pushboolean(lua_State *L, int b);
static short marpaESLIFLua_lua_pushnumber(lua_State *L, lua_Number n);
static short marpaESLIFLua_lua_pushlightuserdata(lua_State *L, void *p);
static short marpaESLIFLua_lua_pushvalue(lua_State *L, int index);
static short marpaESLIFLua_luaL_ref(int *rcip, lua_State *L, int t);
static short marpaESLIFLua_luaL_unref(lua_State *L, int t, int ref);
#ifndef marpaESLIFLua_luaL_error
#define marpaESLIFLua_luaL_error(L, fmt, ...) luaL_error(L, fmt)
#endif
#ifndef marpaESLIFLua_luaL_errorf
#define marpaESLIFLua_luaL_errorf(L, fmt, ...) luaL_error(L, fmt, __VA_ARGS__)
#endif
static short marpaESLIFLua_luaL_requiref(lua_State *L, const char *modname, lua_CFunction openf, int glb);
/* Grrr lua defines that with a macro */
#ifndef marpaESLIFLua_luaL_newlib
#define marpaESLIFLua_luaL_newlib(L, l) (luaL_newlib(L, l), 1)
#endif

#define MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, i) do {                  \
    if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) i)) goto err;  \
    if (! marpaESLIFLua_lua_setglobal(L, #i)) goto err;                 \
  } while (0)

#define MARPAESLIFLUA_GETORCREATEGLOBAL(L, name, gcp) do {              \
    int _typei;                                                         \
    if (! marpaESLIFLua_lua_getglobal(NULL, L, name)) goto err;         \
    if (! marpaESLIFLua_lua_type(&_typei, L, -1)) goto err;             \
    if (_typei != LUA_TTABLE) {                                         \
      if (! marpaESLIFLua_lua_pop(L, 1)) goto err;                      \
      if (! marpaESLIFLua_lua_newtable(L)) goto err;                    \
      if (gcp != NULL) {                                                \
        if (! marpaESLIFLua_lua_newtable(L)) goto err;                  \
        if (! marpaESLIFLua_lua_pushcfunction(L, gcp)) goto err;        \
        if (! marpaESLIFLua_lua_setfield(L, -2, "__gc")) goto err;      \
        if (! marpaESLIFLua_lua_setmetatable(L, -2)) goto err;          \
        if (! marpaESLIFLua_lua_setglobal(L, name)) goto err;           \
        if (! marpaESLIFLua_lua_getglobal(NULL, L, name)) goto err;     \
      }                                                                 \
    }                                                                   \
} while (0)

#ifdef MARPAESLIFLUA_USE_INTERNALREGISTRYINDEX
#define MARPAESLIFLUA_REF(L, refi) do {                                 \
    MARPAESLIFLUA_GETORCREATEGLOBAL(L, MARPAESLIFLUAREGISTRYINDEX, marpaESLIFLua_marpaESLIFRegistryindex_freei); \
    if (! marpaESLIFLua_lua_insert(L, -2)) goto err;                    \
    if (! marpaESLIFLua_luaL_ref(&(refi), L, -2)) goto err;             \
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;                        \
  } while (0);

#define MARPAESLIFLUA_UNREF(L, refi) do {                               \
    MARPAESLIFLUA_GETORCREATEGLOBAL(L, MARPAESLIFLUAREGISTRYINDEX, marpaESLIFLua_marpaESLIFRegistryindex_freei); \
    if (! marpaESLIFLua_luaL_unref(L, -1, refi)) goto err;              \
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;                        \
  } while (0);

#define MARPAESLIFLUA_DEREF(L, refi) do {                               \
    MARPAESLIFLUA_GETORCREATEGLOBAL(L, MARPAESLIFLUAREGISTRYINDEX, marpaESLIFLua_marpaESLIFRegistryindex_freei); \
    if (! marpaESLIFLua_lua_rawgeti(NULL, L, -1, refi)) goto err;       \
    if (! marpaESLIFLua_lua_remove(L, -2)) goto err;                    \
  } while (0);

#else /* MARPAESLIFLUA_USE_INTERNALREGISTRYINDEX */

#define MARPAESLIFLUA_REF(L, refi) do {                                 \
    if (! marpaESLIFLua_luaL_ref(&(refi), L, LUA_REGISTRYINDEX)) goto err; \
  } while (0);

#define MARPAESLIFLUA_UNREF(L, refi) do {                               \
    if (! marpaESLIFLua_luaL_unref(L, LUA_REGISTRYINDEX, refi)) goto err; \
  } while (0);

#define MARPAESLIFLUA_DEREF(L, refi) do {                               \
    if (! marpaESLIFLua_lua_rawgeti(NULL, L, LUA_REGISTRYINDEX, refi)) goto err; \
  } while (0);

#endif /* MARPAESLIFLUA_USE_INTERNALREGISTRYINDEX */

/* For every MARPAESLIFLUA_STORE_xxx macro, destination table is assumed to be at the top of the stack */
#define MARPAESLIFLUA_STORE_BY_KEY(L, key, valueproducer) do {          \
    valueproducer                                                       \
    if (! marpaESLIFLua_lua_setfield(L, -2, key)) goto err;             \
  } while (0)

#define MARPAESLIFLUA_STORE_FUNCTION(L, key, functionp)                 \
  MARPAESLIFLUA_STORE_BY_KEY(L, key, if (functionp == NULL) { if (! marpaESLIFLua_lua_pushnil(L)) goto err; } else { if (! marpaESLIFLua_lua_pushcfunction(L, functionp)) goto err; })

#define MARPAESLIFLUA_STORE_USERDATA(L, key, p)                         \
  MARPAESLIFLUA_STORE_BY_KEY(L, key, if (p == NULL)         { if (! marpaESLIFLua_lua_pushnil(L)) goto err; } else { if (! marpaESLIFLua_lua_pushlightuserdata(L, p)) goto err; })

#define MARPAESLIFLUA_STORE_STRING(L, key, stringp)                     \
  MARPAESLIFLUA_STORE_BY_KEY(L, key, if (stringp == NULL)   { if (! marpaESLIFLua_lua_pushnil(L)) goto err; } else { if (! marpaESLIFLua_lua_pushlstring(NULL, L, (const char *) stringp->bytep, stringp->bytel)) goto err; })

#define MARPAESLIFLUA_STORE_ASCIISTRING(L, key, asciis)                 \
  MARPAESLIFLUA_STORE_BY_KEY(L, key, if (asciis == NULL)    { if (! marpaESLIFLua_lua_pushnil(L)) goto err; } else { if (! marpaESLIFLua_lua_pushstring(NULL, L, asciis)) goto err; })

#define MARPAESLIFLUA_STORE_INTEGER(L, key, i)                          \
  MARPAESLIFLUA_STORE_BY_KEY(L, key, if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) i)) goto err;)

#define MARPAESLIFLUA_PUSH_INTEGER_ARRAY(L, integerl, integerp) do {    \
    size_t _iteratorl;                                                  \
                                                                        \
    if (! marpaESLIFLua_lua_createtable(L, (int) integerl, 0)) goto err; \
    for (_iteratorl = 0; _iteratorl < integerl; _iteratorl++) {         \
      if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) integerp[_iteratorl])) goto err; \
      if (! marpaESLIFLua_lua_rawseti(L, -2, (lua_Integer) _iteratorl)) goto err; \
    }                                                                   \
  } while (0)

#define MARPAESLIFLUA_PUSH_ASCIISTRING_ARRAY(L, stringl, stringp) do {  \
    size_t _iteratorl;                                                  \
                                                                        \
    if (! marpaESLIFLua_lua_createtable(L, (int) stringl, 0)) goto err; \
    for (_iteratorl = 0; _iteratorl < stringl; _iteratorl++) {          \
      if (! marpaESLIFLua_lua_pushstring(NULL, L, stringp[_iteratorl])) goto err; \
      if (! marpaESLIFLua_lua_rawseti(L, -2, (lua_Integer) _iteratorl)) goto err; \
    }                                                                   \
  } while (0)

#define MARPAESLIFLUA_STORE_INTEGER_ARRAY(L, key, integerl, integerp)   \
  MARPAESLIFLUA_STORE_BY_KEY(L, key, MARPAESLIFLUA_PUSH_INTEGER_ARRAY(L, integerl, integerp);)

#define MARPAESLIFLUA_STORE_BOOLEAN(L, key, b)                          \
  MARPAESLIFLUA_STORE_BY_KEY(L, key, if (! marpaESLIFLua_lua_pushboolean(L, (int) b)) goto err;)

#define MARPAESLIFLUA_STORE_ACTION(L, key, actionp)                     \
  MARPAESLIFLUA_STORE_BY_KEY(L, key,                                    \
                             if (actionp != NULL) {                     \
                               switch (actionp->type) {                 \
                               case MARPAESLIF_ACTION_TYPE_NAME:        \
                                 if (! marpaESLIFLua_lua_pushstring(NULL, L, actionp->u.names)) goto err; \
                                 break;                                 \
                               case MARPAESLIF_ACTION_TYPE_STRING:      \
                                 if (! marpaESLIFLua_lua_pushlstring(NULL, L, (const char *) actionp->u.stringp->bytep, actionp->u.stringp->bytel)) goto err; \
                                 break;                                 \
                               default:                                 \
                                 marpaESLIFLua_luaL_errorf(L, "Unsupported action type %d", actionp->type); \
                                 goto err;                              \
                               }                                        \
                             } else {                                   \
                               if (! marpaESLIFLua_lua_pushnil(L)) goto err;                          \
                             }                                          \
                             )

/* This is vicious, but here it is: we assume that EVERY callback function refers to an object */
/* i.e. a function that has "self" as the first parameter. So nargs is always nargs+1 in reality */
/* This is the reason for lua_insert(L, -2) instead of lua_remove(L, -2) */
/* Nevertheless this is not true when we are embedded: then it is assumed that the function */
/* reside in the main namespace and have no object. We distinguish the two cases with the interface_r value: */
#define MARPAESLIFLUA_CALLBACK(L, interface_r, funcs, nargs, parameters) do { \
    int _typei;                                                         \
    if (interface_r != LUA_NOREF) {					\
      MARPAESLIFLUA_DEREF(L, interface_r);                              \
      if (! marpaESLIFLua_lua_getfield(NULL, L, -1, funcs)) goto err;   \
      if (! marpaESLIFLua_lua_type(&_typei, L, -1)) goto err;           \
      if (_typei != LUA_TFUNCTION) {                                    \
        marpaESLIFLua_luaL_errorf(L, "No such function %s", funcs);     \
        goto err;                                                       \
      }                                                                 \
      if (! marpaESLIFLua_lua_insert(L, -2)) goto err;                  \
      parameters                                                        \
      if (! marpaESLIFLua_lua_call(L, nargs + 1, LUA_MULTRET)) goto err; \
    } else {								\
      if (! marpaESLIFLua_lua_getglobal(NULL, L, funcs)) goto err;      \
      if (! marpaESLIFLua_lua_type(&_typei, L, -1)) goto err;           \
      if (_typei != LUA_TFUNCTION) {                                    \
        marpaESLIFLua_luaL_errorf(L, "No such function %s", funcs);     \
        goto err;                                                       \
      }                                                                 \
      parameters                                                        \
      if (! marpaESLIFLua_lua_call(L, nargs, LUA_MULTRET)) goto err;    \
    }                                                                   \
} while (0)
    
#define MARPAESLIFLUA_CALLBACKV(L, interface_r, funcs, nargs, parameters) do { \
  int _topi;                                                            \
                                                                        \
  _topi = lua_gettop(L);                                                \
  MARPAESLIFLUA_CALLBACK(L, interface_r, funcs, nargs, parameters);     \
  if (! marpaESLIFLua_lua_settop(L, _topi)) goto err;                                                 \
} while (0)
    
#define MARPAESLIFLUA_CALLBACKB(L, interface_r, funcs, nargs, parameters, bp) do { \
    int _topi;                                                          \
    int _newtopi;                                                       \
    int _typei;                                                         \
                                                                        \
    _topi = lua_gettop(L);                                              \
    MARPAESLIFLUA_CALLBACK(L, interface_r, funcs, nargs, parameters);   \
    _newtopi = lua_gettop(L);                                           \
    if (_newtopi != (_topi + 1)) {                                      \
      marpaESLIFLua_luaL_errorf(L, "Function %s must return exactly one value", funcs); \
      goto err;                                                         \
    }                                                                   \
    if (! marpaESLIFLua_lua_type(&_typei, L, -1)) goto err;             \
    if (_typei != LUA_TBOOLEAN) {                                       \
      marpaESLIFLua_luaL_errorf(L, "Function %s must return a boolean value, got %s", funcs, lua_typename(L, _typei)); \
      goto err;                                                         \
    }                                                                   \
    if (bp != NULL) {                                                   \
      if (lua_toboolean(L, -1) != 0) {                                  \
        *bp = 1;                                                        \
      } else {                                                          \
        *bp = 0;                                                        \
      }                                                                 \
    }                                                                   \
    if (! marpaESLIFLua_lua_settop(L, _topi)) goto err;                                               \
} while (0)

/* Value is at the top of the stack and is popped by this macro */
/* We store at "indice" a pointer to an integer that contains a reference to the value */
#define MARPAESLIFLUA_SET_VALUE(marpaESLIFLuaValueContextp, marpaESLIFValuep, indicei, stringificationp) do { \
    marpaESLIFValueResult_t _marpaESLIFValueResult;                     \
    int *_p;                                                            \
                                                                        \
    _p = (int *) malloc(sizeof(int));					\
    if (_p == NULL) {                                                   \
      marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno)); \
      goto err;                                                         \
    }                                                                   \
                                                                        \
    if (! marpaESLIFLua_lua_pushnil(marpaESLIFLuaValueContextp->L)) goto err; \
    if (! marpaESLIFLua_lua_copy(marpaESLIFLuaValueContextp->L, -2, -1)) goto err; \
    MARPAESLIFLUA_REF(marpaESLIFLuaValueContextp->L, *_p);              \
                                                                        \
    _marpaESLIFValueResult.contextp        = MARPAESLIFLUA_CONTEXT;     \
    _marpaESLIFValueResult.representationp = stringificationp;          \
    _marpaESLIFValueResult.type            = MARPAESLIF_VALUE_TYPE_PTR; \
    _marpaESLIFValueResult.u.p.p           = _p;                        \
    _marpaESLIFValueResult.u.p.shallowb    = 0;                         \
                                                                        \
    if (! marpaESLIFValue_stack_setb(marpaESLIFValuep, indicei, &_marpaESLIFValueResult)) { \
      marpaESLIFLua_luaL_errorf(L, "marpaESLIFValue_stack_setb failure, %s", strerror(errno)); \
      goto err;                                                         \
    }                                                                   \
                                                                        \
  } while (0)

#define MARPAESLIFLUA_CALLBACKI(L, interface_r, funcs, nargs, parameters, ip) do { \
    int _topi;                                                          \
    int _newtopi;                                                       \
    int _isnum;                                                         \
    int _typei;                                                         \
    lua_Integer _i;                                                     \
                                                                        \
    _topi = lua_gettop(L);                                              \
    MARPAESLIFLUA_CALLBACK(L, interface_r, funcs, nargs, parameters);   \
    _newtopi = lua_gettop(L);                                           \
    if (_newtopi != (_topi + 1)) {                                      \
      marpaESLIFLua_luaL_errorf(L, "Function %s must return exactly one value", funcs); \
      goto err;                                                         \
    }                                                                   \
    if (! marpaESLIFLua_lua_type(&_typei, L, -1)) goto err;             \
    if (_typei != LUA_TNUMBER) {                                        \
      marpaESLIFLua_luaL_errorf(L, "Function %s must return a number value, got %s", funcs, lua_typename(L, _typei)); \
      goto err;                                                         \
    }                                                                   \
    if (ip != NULL) {                                                   \
      _i = lua_tointegerx(L, -1, &_isnum);                              \
      if (! _isnum) {                                                   \
        marpaESLIFLua_luaL_error(L, "Convertion to a number failed");   \
        goto err;                                                       \
      }                                                                 \
      *ip = (int) _i;                                                   \
    }                                                                   \
    if (! marpaESLIFLua_lua_settop(L, _topi)) goto err;                                               \
} while (0)

/* Take care: if *sp is != NULL outside of the scope, caller's responsibility to free it */
#define MARPAESLIFLUA_CALLBACKS(L, interface_r, funcs, nargs, parameters, sp, lp) do { \
    int _topi;                                                          \
    int _newtopi;                                                       \
    char *_s;                                                           \
    size_t _l;                                                          \
    int _typei;                                                         \
                                                                        \
    _topi = lua_gettop(L);                                              \
    MARPAESLIFLUA_CALLBACK(L, interface_r, funcs, nargs, parameters);   \
    _newtopi = lua_gettop(L);                                           \
    if (_newtopi != (_topi + 1)) {                                      \
      marpaESLIFLua_luaL_errorf(L, "Function %s must return exactly one value", funcs); \
      goto err;                                                         \
    }                                                                   \
    if (! marpaESLIFLua_lua_type(&_typei, L, -1)) goto err;             \
    switch (_typei) {                                                   \
    case LUA_TNIL:                                                      \
      *sp = NULL;                                                       \
      *lp = 0;                                                          \
      break;                                                            \
    case LUA_TSTRING:                                                   \
      _s = (char *) lua_tolstring(L, -1, &_l);                          \
      if (_s != NULL) {                                                 \
        *sp = malloc(_l);                                               \
        if (*sp == NULL) {                                              \
          marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));  \
          goto err;                                                     \
        }                                                               \
        memcpy(*sp, _s, _l);                                            \
        *lp = _l;                                                       \
      } else {                                                          \
        *sp = NULL;                                                     \
        *lp = 0;                                                        \
      }                                                                 \
      break;                                                            \
    default:                                                            \
      marpaESLIFLua_luaL_errorf(L, "Function %s must return a string value or nil, got %s", funcs, lua_typename(L, _typei)); \
      goto err;                                                         \
    }                                                                   \
    if (! marpaESLIFLua_lua_settop(L, _topi)) goto err;                                               \
} while (0)

/* -------------------- */
/* Push of ESLIF object */
/* -------------------- */
/* We VOLUNTARILY never associate a __gc to an ESLIF object because */
/* it is stored in MARPAESLIFMULTITONSTABLE */
#define MARPAESLIFLUA_PUSH_MARPAESLIF_OBJECT(L, marpaESLIFLuaContextp) do { \
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                      \
    MARPAESLIFLUA_STORE_USERDATA(L, "marpaESLIFLuaContextp", marpaESLIFLuaContextp); \
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                      \
    MARPAESLIFLUA_STORE_ASCIISTRING(L, "__mode", "v");                  \
    MARPAESLIFLUA_STORE_FUNCTION(L, "__gc",                  marpaESLIFLua_marpaESLIF_freei); \
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                      \
    MARPAESLIFLUA_STORE_FUNCTION(L, "version",               marpaESLIFLua_marpaESLIF_versioni); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "versionMajor",          marpaESLIFLua_marpaESLIF_versionMajori); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "versionMinor",          marpaESLIFLua_marpaESLIF_versionMinori); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "versionPatch",          marpaESLIFLua_marpaESLIF_versionPatchi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "marpaESLIFGrammar_new", marpaESLIFLua_marpaESLIFGrammar_newi); \
    if (! marpaESLIFLua_lua_setfield(L, -2, "__index")) goto err;       \
    if (! marpaESLIFLua_lua_setmetatable(L, -2)) goto err;              \
  } while (0)

/* ---------------------------- */
/* Push of ESLIF grammar object */
/* ---------------------------- */
#define MARPAESLIFLUA_PUSH_MARPAESLIFGRAMMAR_OBJECT(L, marpaESLIFLuaGrammarContextp) do { \
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                      \
    MARPAESLIFLUA_STORE_USERDATA(L, "marpaESLIFLuaGrammarContextp", marpaESLIFLuaGrammarContextp); \
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                      \
    MARPAESLIFLUA_STORE_ASCIISTRING(L, "__mode", "v");                  \
    MARPAESLIFLUA_STORE_FUNCTION(L, "__gc",                         marpaESLIFLua_marpaESLIFGrammar_freei); \
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                      \
    MARPAESLIFLUA_STORE_FUNCTION(L, "ngrammar",                     marpaESLIFLua_marpaESLIFGrammar_ngrammari); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "currentLevel",                 marpaESLIFLua_marpaESLIFGrammar_currentLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "currentDescription",           marpaESLIFLua_marpaESLIFGrammar_currentDescriptioni); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "descriptionByLevel",           marpaESLIFLua_marpaESLIFGrammar_descriptionByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "currentRuleIds",               marpaESLIFLua_marpaESLIFGrammar_currentRuleIdsi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "ruleIdsByLevel",               marpaESLIFLua_marpaESLIFGrammar_ruleIdsByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "currentSymbolIds",             marpaESLIFLua_marpaESLIFGrammar_currentSymbolIdsi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "symbolIdsByLevel",             marpaESLIFLua_marpaESLIFGrammar_symbolIdsByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "currentProperties",            marpaESLIFLua_marpaESLIFGrammar_currentPropertiesi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "propertiesByLevel",            marpaESLIFLua_marpaESLIFGrammar_propertiesByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "currentRuleProperties",        marpaESLIFLua_marpaESLIFGrammar_currentRulePropertiesi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "rulePropertiesByLevel",        marpaESLIFLua_marpaESLIFGrammar_rulePropertiesByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "currentSymbolProperties",      marpaESLIFLua_marpaESLIFGrammar_currentSymbolPropertiesi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "symbolPropertiesByLevel",      marpaESLIFLua_marpaESLIFGrammar_symbolPropertiesByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "ruleDisplay",                  marpaESLIFLua_marpaESLIFGrammar_ruleDisplayi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "symbolDisplay",                marpaESLIFLua_marpaESLIFGrammar_symbolDisplayi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "ruleShow",                     marpaESLIFLua_marpaESLIFGrammar_ruleShowi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "ruleDisplayByLevel",           marpaESLIFLua_marpaESLIFGrammar_ruleDisplayByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "symbolDisplayByLevel",         marpaESLIFLua_marpaESLIFGrammar_symbolDisplayByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "ruleShowByLevel",              marpaESLIFLua_marpaESLIFGrammar_ruleShowByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "show",                         marpaESLIFLua_marpaESLIFGrammar_showi); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "showByLevel",                  marpaESLIFLua_marpaESLIFGrammar_showByLeveli); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "parse",                        marpaESLIFLua_marpaESLIFGrammar_parsei); \
    MARPAESLIFLUA_STORE_FUNCTION(L, "marpaESLIFRecognizer_new",     marpaESLIFLua_marpaESLIFRecognizer_newi); \
    if (! marpaESLIFLua_lua_setfield(L, -2, "__index")) goto err;       \
    if (! marpaESLIFLua_lua_setmetatable(L, -2)) goto err;              \
  } while (0)

/* ------------------------------- */
/* Push of ESLIF recognizer object */
/* ------------------------------- */
#define MARPAESLIFLUA_PUSH_MARPAESLIFRECOGNIZER_OBJECT(L, marpaESLIFLuaRecognizerContextp) do { \
  if (! marpaESLIFLua_lua_newtable(L)) goto err;                        \
  MARPAESLIFLUA_STORE_USERDATA(L, "marpaESLIFLuaRecognizerContextp", marpaESLIFLuaRecognizerContextp); \
  if (! marpaESLIFLua_lua_newtable(L)) goto err;                        \
  MARPAESLIFLUA_STORE_ASCIISTRING(L, "__mode", "v");                    \
  MARPAESLIFLUA_STORE_FUNCTION(L, "__gc",                            marpaESLIFLua_marpaESLIFRecognizer_freei); \
  if (! marpaESLIFLua_lua_newtable(L)) goto err;                        \
  MARPAESLIFLUA_STORE_FUNCTION(L, "newFrom",                         marpaESLIFLua_marpaESLIFRecognizer_newFromi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "set_exhausted_flag",              marpaESLIFLua_marpaESLIFRecognizer_set_exhausted_flagi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "share",                           marpaESLIFLua_marpaESLIFRecognizer_sharei); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "isCanContinue",                   marpaESLIFLua_marpaESLIFRecognizer_isCanContinuei); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "isExhausted",                     marpaESLIFLua_marpaESLIFRecognizer_isExhaustedi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "scan",                            marpaESLIFLua_marpaESLIFRecognizer_scani); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "resume",                          marpaESLIFLua_marpaESLIFRecognizer_resumei); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "events",                          marpaESLIFLua_marpaESLIFRecognizer_eventsi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "eventOnOff",                      marpaESLIFLua_marpaESLIFRecognizer_eventOnOffi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lexemeAlternative",               marpaESLIFLua_marpaESLIFRecognizer_lexemeAlternativei); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lexemeComplete",                  marpaESLIFLua_marpaESLIFRecognizer_lexemeCompletei); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lexemeRead",                      marpaESLIFLua_marpaESLIFRecognizer_lexemeReadi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lexemeTry",                       marpaESLIFLua_marpaESLIFRecognizer_lexemeTryi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "discardTry",                      marpaESLIFLua_marpaESLIFRecognizer_discardTryi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lexemeExpected",                  marpaESLIFLua_marpaESLIFRecognizer_lexemeExpectedi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lexemeLastPause",                 marpaESLIFLua_marpaESLIFRecognizer_lexemeLastPausei); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lexemeLastTry",                   marpaESLIFLua_marpaESLIFRecognizer_lexemeLastTryi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "discardLastTry",                  marpaESLIFLua_marpaESLIFRecognizer_discardLastTryi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "isEof",                           marpaESLIFLua_marpaESLIFRecognizer_isEofi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "read",                            marpaESLIFLua_marpaESLIFRecognizer_readi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "input",                           marpaESLIFLua_marpaESLIFRecognizer_inputi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "progressLog",                     marpaESLIFLua_marpaESLIFRecognizer_progressLogi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lastCompletedOffset",             marpaESLIFLua_marpaESLIFRecognizer_lastCompletedOffseti); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lastCompletedLength",             marpaESLIFLua_marpaESLIFRecognizer_lastCompletedLengthi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "lastCompletedLocation",           marpaESLIFLua_marpaESLIFRecognizer_lastCompletedLocationi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "line",                            marpaESLIFLua_marpaESLIFRecognizer_linei); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "column",                          marpaESLIFLua_marpaESLIFRecognizer_columni); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "location",                        marpaESLIFLua_marpaESLIFRecognizer_locationi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "hookDiscard",                     marpaESLIFLua_marpaESLIFRecognizer_hookDiscardi); \
  MARPAESLIFLUA_STORE_FUNCTION(L, "marpaESLIFValue_new",             marpaESLIFLua_marpaESLIFValue_newi); \
  if (! marpaESLIFLua_lua_setfield(L, -2, "__index")) goto err;         \
  if (! marpaESLIFLua_lua_setmetatable(L, -2)) goto err;                \
  } while (0)

/* ---------------------------- */
/* Push of ESLIF value object */
/* ---------------------------- */
#define MARPAESLIFLUA_PUSH_MARPAESLIFVALUE_OBJECT(L, marpaESLIFLuaValueContextp) do { \
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                      \
    MARPAESLIFLUA_STORE_USERDATA(L, "marpaESLIFLuaValueContextp", marpaESLIFLuaValueContextp); \
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                      \
    MARPAESLIFLUA_STORE_ASCIISTRING(L, "__mode", "v");                  \
    MARPAESLIFLUA_STORE_FUNCTION(L, "__gc",                       marpaESLIFLua_marpaESLIFValue_freei); \
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                      \
    MARPAESLIFLUA_STORE_FUNCTION(L, "value",                      marpaESLIFLua_marpaESLIFValue_valuei); \
    if (! marpaESLIFLua_lua_setfield(L, -2, "__index")) goto err;       \
    if (! marpaESLIFLua_lua_setmetatable(L, -2)) goto err;              \
  } while (0)

#ifdef MARPAESLIFLUA_EMBEDDED
/* luaopen_marpaESLIFLua is to be called explicitely by the program that embeds marpaESLIFLua */
static
#endif
/****************************************************************************/
int luaopen_marpaESLIFLua(lua_State* L)
/****************************************************************************/
{
  static const char *funcs = "luaopen_marpaESLIFLua";

  if (! marpaESLIFLua_luaL_requiref(L, "marpaESLIFLua", marpaESLIFLua_installi, 1 /* global */)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int marpaESLIFLua_installi(lua_State *L)
/****************************************************************************/
{
  static const char     *funcs                        = "marpaESLIFLua_installi";
  static const luaL_Reg  marpaESLIFLua_installTable[] = {
    {"version",                     marpaESLIFLua_versioni},
    {"versionMajor",                marpaESLIFLua_versionMajori},
    {"versionMinor",                marpaESLIFLua_versionMinori},
    {"versionPatch",                marpaESLIFLua_versionPatchi},
    {"marpaESLIF_new",              marpaESLIFLua_marpaESLIF_newi},
    {NULL, NULL}
  };
  int                    rci;

  if (! marpaESLIFLua_luaL_newlib(L, marpaESLIFLua_installTable)) goto err;

  /* Create constants */
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_EVENTTYPE_NONE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_EVENTTYPE_COMPLETED);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_EVENTTYPE_NULLED);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_EVENTTYPE_PREDICTED);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_EVENTTYPE_BEFORE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_EVENTTYPE_AFTER);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_EVENTTYPE_EXHAUSTED);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_EVENTTYPE_DISCARD);
  
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_UNDEF);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_CHAR);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_SHORT);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_INT);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_LONG);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_FLOAT);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_DOUBLE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_PTR);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_ARRAY);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_VALUE_TYPE_BOOL);
  
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_RULE_IS_ACCESSIBLE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_RULE_IS_NULLABLE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_RULE_IS_NULLING);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_RULE_IS_LOOP);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_RULE_IS_PRODUCTIVE);
  
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_SYMBOL_IS_ACCESSIBLE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_SYMBOL_IS_NULLABLE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_SYMBOL_IS_NULLING);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_SYMBOL_IS_PRODUCTIVE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_SYMBOL_IS_START);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_SYMBOL_IS_TERMINAL);
  
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_SYMBOLTYPE_TERMINAL);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, MARPAESLIF_SYMBOLTYPE_META);

  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, GENERICLOGGER_LOGLEVEL_TRACE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, GENERICLOGGER_LOGLEVEL_DEBUG);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, GENERICLOGGER_LOGLEVEL_INFO);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, GENERICLOGGER_LOGLEVEL_NOTICE);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, GENERICLOGGER_LOGLEVEL_WARNING);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, GENERICLOGGER_LOGLEVEL_ERROR);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, GENERICLOGGER_LOGLEVEL_CRITICAL);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, GENERICLOGGER_LOGLEVEL_ALERT);
  MARPAESLIFLUA_CREATEINTEGERCONSTANT(L, GENERICLOGGER_LOGLEVEL_EMERGENCY);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int marpaESLIFLua_versioni(lua_State *L)
/****************************************************************************/
{
  static const char      *funcs = "marpaESLIFLua_versioni";

  if (lua_gettop(L) != 0) {
    marpaESLIFLua_luaL_error(L, "Usage: version()");
    goto err;
  }

  if (! marpaESLIFLua_lua_pushstring(NULL, L, MARPAESLIFLUA_VERSION)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int marpaESLIFLua_versionMajori(lua_State *L)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_versionMajori";
  int                rci;

  if (lua_gettop(L) != 0) {
    marpaESLIFLua_luaL_error(L, "Usage: versionMajor()");
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) MARPAESLIFLUA_VERSION_MAJOR)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int marpaESLIFLua_versionMinori(lua_State *L)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_versionMinori";
  int                rci;

  if (lua_gettop(L) != 0) {
    marpaESLIFLua_luaL_error(L, "Usage: versionMinor()");
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) MARPAESLIFLUA_VERSION_MINOR)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int marpaESLIFLua_versionPatchi(lua_State *L)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_versionPatchi";
  int                rci;

  if (lua_gettop(L) != 0) {
    marpaESLIFLua_luaL_error(L, "Usage: versionPatch()");
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) MARPAESLIFLUA_VERSION_PATCH)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int marpaESLIFLua_marpaESLIF_newi(lua_State *L)
/****************************************************************************/
{
  static const char                   *funcs                 = "marpaESLIFLua_marpaESLIF_newi";
  marpaESLIFLuaContext_t              *marpaESLIFLuaContextp = NULL;
  short                                loggerb;
  marpaESLIFLuaGenericLoggerContext_t *marpaESLIFLuaGenericLoggerContextp;
  genericLogger_t                     *genericLoggerp;
  marpaESLIFOption_t                   marpaESLIFOption;
  int                                  logger_r;
  int                                  rci;

  switch (lua_gettop(L)) {
  case 0:
    loggerb = 0;
    break;
  case 1:
    loggerb = marpaESLIFLua_paramIsLoggerInterfaceOrNilb(L, 1);
    break;
  default:
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIF_new([logger]");
    goto err;
  }

  MARPAESLIFLUA_GETORCREATEGLOBAL(L, MARPAESLIFMULTITONSTABLE, marpaESLIFLua_marpaESLIFMultitonsTable_freei); /* stack: logger?, MARPAESLIFMULTITONSTABLE */

  if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                                                   /* stack: logger?, MARPAESLIFMULTITONSTABLE, nil */
  while (lua_next(L, -2) != 0) {                                                                    /* stack: logger?, MARPAESLIFMULTITONSTABLE, marpaESLIFLuaContextp, r */
    logger_r = (int) lua_tointeger(L, -1);
    if (logger_r == LUA_NOREF) {
      if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                                               /* stack: logger?, MARPAESLIFMULTITONSTABLE, marpaESLIFLuaContextp, r, nil */
    } else {
      MARPAESLIFLUA_DEREF(L, logger_r);                                                             /* stack: logger?, MARPAESLIFMULTITONSTABLE, marpaESLIFLuaContextp, r, loggerp_from_registry */
    }
    /* Look if MARPAESLIFMULTITONSTABLE already contains a reference to logger */
    if (((! loggerb) && lua_isnil(L, -1))
        ||
        ((  loggerb) && lua_compare(L, 1, -1, LUA_OPEQ))) {
      marpaESLIFLuaContextp = (marpaESLIFLuaContext_t *) lua_touserdata(L, -3);
      if (! marpaESLIFLua_lua_pop(L, 3)) goto err;                                                  /* stack: logger?, MARPAESLIFMULTITONSTABLE */
      break;
    }
    if (! marpaESLIFLua_lua_pop(L, 2)) goto err;                                                    /* stack: logger?, MARPAESLIFMULTITONSTABLE, marpaESLIFLuaContextp */
  }
  
  if (marpaESLIFLuaContextp == NULL) {
    if (loggerb) {
      marpaESLIFLuaGenericLoggerContextp = (marpaESLIFLuaGenericLoggerContext_t *) malloc(sizeof(marpaESLIFLuaGenericLoggerContext_t));
      if (marpaESLIFLuaGenericLoggerContextp == NULL) {
        marpaESLIFLua_luaL_errorf(L, "malloc failure, %s\n", strerror(errno));
        goto err;
      }

      marpaESLIFLuaGenericLoggerContextp->L = NULL;
      marpaESLIFLuaGenericLoggerContextp->logger_r = LUA_NOREF;

      /* Get logger reference */
      if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                                               /* stack: logger, MARPAESLIFMULTITONSTABLE, nil */
      if (! marpaESLIFLua_lua_copy(L, 1, -1)) goto err;                                                                           /* stack: logger, MARPAESLIFMULTITONSTABLE, logger */
      MARPAESLIFLUA_REF(L, logger_r);                                                               /* stack: logger, MARPAESLIFMULTITONSTABLE */

      /* Fill genericLogger context */
      marpaESLIFLuaGenericLoggerContextp->L = L;
      marpaESLIFLuaGenericLoggerContextp->logger_r = logger_r;
      genericLoggerp = genericLogger_newp(marpaESLIFLua_genericLoggerCallbackv, marpaESLIFLuaGenericLoggerContextp, GENERICLOGGER_LOGLEVEL_TRACE);
      if (genericLoggerp == NULL) {
        MARPAESLIFLUA_UNREF(L, logger_r);
        free(marpaESLIFLuaGenericLoggerContextp);
        marpaESLIFLua_luaL_errorf(L, "genericLogger_newp failure, %s\n", strerror(errno));
        goto err;
      }
    } else {
      logger_r = LUA_NOREF;
      genericLoggerp = NULL;
    }

    marpaESLIFLuaContextp = malloc(sizeof(marpaESLIFLuaContext_t));
    if (marpaESLIFLuaContextp == NULL) {
      MARPAESLIFLUA_UNREF(L, logger_r); /* No effect if it is LUA_NOREF */
      free(marpaESLIFLuaGenericLoggerContextp);
      marpaESLIFLua_luaL_errorf(L, "malloc failure, %s\n", strerror(errno));
      goto err;
    }
    if (! marpaESLIFLua_contextInitb(L, marpaESLIFLuaContextp, 0 /* unmanagedb */)) goto err;

    marpaESLIFOption.genericLoggerp    = genericLoggerp;
    marpaESLIFLuaContextp->marpaESLIFp = marpaESLIF_newp(&marpaESLIFOption);
    if (marpaESLIFLuaContextp->marpaESLIFp == NULL) {
      MARPAESLIFLUA_UNREF(L, logger_r); /* No effect if it is LUA_NOREF */
      free(marpaESLIFLuaContextp);
      free(marpaESLIFLuaGenericLoggerContextp);
      marpaESLIFLua_luaL_errorf(L, "marpaESLIF_newp failure, %s\n", strerror(errno));
      goto err;
    }

    /* Link marpaESLIFp to logger_r */
    if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) logger_r)) goto err;                       /* stack: logger?, MARPAESLIFMULTITONSTABLE, logger_r */
    if (! marpaESLIFLua_lua_rawsetp(L, -2, (const void *) marpaESLIFLuaContextp)) goto err;         /* stack: logger?, MARPAESLIFMULTITONSTABLE */

    /* Remember it is in the multiton table */
    marpaESLIFLuaContextp->multitonb = 1;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  /* Push marpaESLIF object */
  MARPAESLIFLUA_PUSH_MARPAESLIF_OBJECT(L, marpaESLIFLuaContextp);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int marpaESLIFLua_marpaESLIF_freei(lua_State *L)
/****************************************************************************/
{
  static const char      *funcs = "marpaESLIFLua_marpaESLIF_freei";
  marpaESLIFLuaContext_t *marpaESLIFLuaContextp;

  if (! marpaESLIFLua_lua_getfield(NULL, L, -1, "marpaESLIFLuaContextp")) goto err; /* stack: {...}, marpaESLIFLuaContextp */
  marpaESLIFLuaContextp = (marpaESLIFLuaContext_t *) lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;  /* stack: {...} */

  marpaESLIFLua_contextFreev(marpaESLIFLuaContextp, 0 /* multitonDestroyModeb */);

  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;        /* stack: */

  return 0;

 err:
  return 0;
}

/****************************************************************************/
static int marpaESLIFLua_marpaESLIF_versioni(lua_State *L)
/****************************************************************************/
{
  static const char      *funcs = "marpaESLIFLua_marpaESLIF_versioni";
  marpaESLIFLuaContext_t *marpaESLIFLuaContextp;
  char                   *versions;
  int                     typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: version(marpaESLIFp)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL, L, 1, "marpaESLIFLuaContextp")) goto err;   /* stack: marpaESLIFTable, marpaESLIFLuaContextp */
  marpaESLIFLuaContextp = lua_touserdata(L, -1); /* stack: marpaESLIFTable, marpaESLIFLuaContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;   /* stack: */

  if (! marpaESLIF_versionb(marpaESLIFLuaContextp->marpaESLIFp, &versions)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIF_versionb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushstring(NULL, L, versions)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int marpaESLIFLua_marpaESLIF_versionMajori(lua_State *L)
/****************************************************************************/
{
  static const char      *funcs = "marpaESLIFLua_marpaESLIF_versionMajori";
  marpaESLIFLuaContext_t *marpaESLIFLuaContextp;
  int                     majori;
  int                     rci;
  int                     typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: version(marpaESLIFp)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL, L, 1, "marpaESLIFLuaContextp")) goto err;   /* stack: marpaESLIFTable, marpaESLIFLuaContextp */
  marpaESLIFLuaContextp = lua_touserdata(L, -1); /* stack: marpaESLIFTable, marpaESLIFLuaContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;   /* stack: */

  if (! marpaESLIF_versionMajorb(marpaESLIFLuaContextp->marpaESLIFp, &majori)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIF_versionMajorb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) majori)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int marpaESLIFLua_marpaESLIF_versionMinori(lua_State *L)
/****************************************************************************/
{
  static const char      *funcs = "marpaESLIFLua_marpaESLIF_versionMinori";
  marpaESLIFLuaContext_t *marpaESLIFLuaContextp;
  int                     minori;
  int                     rci;
  int                     typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: version(marpaESLIFp)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL, L, 1, "marpaESLIFLuaContextp")) goto err;   /* stack: marpaESLIFTable, marpaESLIFLuaContextp */
  marpaESLIFLuaContextp = lua_touserdata(L, -1); /* stack: marpaESLIFTable, marpaESLIFLuaContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;   /* stack: */

  if (! marpaESLIF_versionMinorb(marpaESLIFLuaContextp->marpaESLIFp, &minori)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIF_versionMinorb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) minori)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int marpaESLIFLua_marpaESLIF_versionPatchi(lua_State *L)
/****************************************************************************/
{
  static const char      *funcs = "marpaESLIFLua_marpaESLIF_versionPatchi";
  marpaESLIFLuaContext_t *marpaESLIFLuaContextp;
  int                     patchi;
  int                     rci;
  int                     typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: version(marpaESLIFp)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaContextp")) goto err;   /* stack: marpaESLIFTable, marpaESLIFLuaContextp */
  marpaESLIFLuaContextp = lua_touserdata(L, -1); /* stack: marpaESLIFTable, marpaESLIFLuaContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;   /* stack: */

  if (! marpaESLIF_versionPatchb(marpaESLIFLuaContextp->marpaESLIFp, &patchi)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIF_versionPatchb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) patchi)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

#ifdef MARPAESLIFLUA_EMBEDDED
/****************************************************************************/
static int  marpaESLIFLua_marpaESLIF_newFromUnmanagedi(lua_State *L, marpaESLIF_t *marpaESLIFUnmanagedp)
/****************************************************************************/
{
  static const char      *funcs                  = "marpaESLIFLua_marpaESLIF_newFromUnmanagedi";
  marpaESLIFLuaContext_t  *marpaESLIFLuaContextp = NULL;

  marpaESLIFLuaContextp = malloc(sizeof(marpaESLIFLuaContext_t));
  if (marpaESLIFLuaContextp == NULL) {
    marpaESLIFLua_luaL_errorf(L, "malloc failure, %s\n", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_contextInitb(L, marpaESLIFLuaContextp, 1 /* unmanagedb */)) goto err;

  marpaESLIFLuaContextp->marpaESLIFp = marpaESLIFUnmanagedp;

  /* Push marpaESLIF object */
  MARPAESLIFLUA_PUSH_MARPAESLIF_OBJECT(L, marpaESLIFLuaContextp);

  return 1;
 err:
  return 0;
}
#endif /* MARPAESLIFLUA_EMBEDDED */

/****************************************************************************/
static short marpaESLIFLua_paramIsLoggerInterfaceOrNilb(lua_State *L, int stacki)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_paramIsLoggerInterfaceOrNilb";
  static const char *loggerFunctions[] = {
    "trace",
    "debug",
    "info",
    "notice",
    "warning",
    "error",
    "critical",
    "emergency"
  };
  int                i;
  short              loggerb;
  int                typei;

  if (! marpaESLIFLua_lua_type(&typei, L, stacki)) goto err;
  if (typei == LUA_TNIL) {
    loggerb = 0;
  } else {
    /* Verify that the logger can do all wanted methods */
    if (typei != LUA_TTABLE) {
      marpaESLIFLua_luaL_error(L, "logger interface must be a table");
      goto err;
    }
    for (i = 0; i < sizeof(loggerFunctions)/sizeof(loggerFunctions[0]); i++) {
      if (! marpaESLIFLua_lua_getfield(NULL,L, stacki, loggerFunctions[i])) goto err;                             /* stack: stack1, ..., stacki, field */
      if (! marpaESLIFLua_lua_type(&typei, L, -1)) goto err;
      if (typei != LUA_TFUNCTION) {
        if (! marpaESLIFLua_lua_pop(L, 1)) goto err;                           /* stack: stack1, ..., stacki */
        marpaESLIFLua_luaL_errorf(L, "logger table must have a field named '%s' that is a function", loggerFunctions[i]);
        goto err;
      } else {
        if (! marpaESLIFLua_lua_pop(L, 1)) goto err;                           /* stack: stack1, ..., stacki */
      }
    }
    loggerb = 1;
  }

  return loggerb;

 err:
  return 0;
}

/****************************************************************************/
static short marpaESLIFLua_paramIsRecognizerInterfacev(lua_State *L, int stacki)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_paramIsRecognizerInterfacev";
  static const char *recognizerFunctions[] = {
    "read",
    "isEof",
    "isCharacterStream",
    "encoding",
    "data",
    "isWithDisableThreshold",
    "isWithExhaustion",
    "isWithNewline",
    "isWithTrack"
  };
  int                i;
  int                typei;

  /* Verify that the recognizer can do all wanted methods */
  if (! marpaESLIFLua_lua_type(&typei, L, stacki)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "recognizer interface must be a table");
    goto err;
  }
  for (i = 0; i < sizeof(recognizerFunctions)/sizeof(recognizerFunctions[0]); i++) {
    if (! marpaESLIFLua_lua_getfield(NULL,L, stacki, recognizerFunctions[i])) goto err;                             /* stack: stack1, ..., stacki, field */
    if (! marpaESLIFLua_lua_type(&typei, L, -1)) goto err;
    if (typei != LUA_TFUNCTION) {
      if (! marpaESLIFLua_lua_pop(L, 1)) goto err;                               /* stack: stack1, ..., stacki */
      marpaESLIFLua_luaL_errorf(L, "recognizer table must have a field named '%s' that is a function", recognizerFunctions[i]);
      goto err;
    } else {
      if (! marpaESLIFLua_lua_pop(L, 1)) goto err;                               /* stack: stack1, ..., stacki */
    }
  }

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static short marpaESLIFLua_paramIsValueInterfacev(lua_State *L, int stacki)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_paramIsValueInterfacev";
  static const char *valueFunctions[] = {
    "isWithHighRankOnly",
    "isWithOrderByRank",
    "isWithAmbiguous",
    "isWithNull",
    "maxParses",
    "setResult",
    "getResult"
  };
  int                i;
  int                typei;

  /* Verify that the recognizer can do all wanted methods */
  if (! marpaESLIFLua_lua_type(&typei, L, stacki)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "value interface must be a table");
    goto err;
  }
  for (i = 0; i < sizeof(valueFunctions)/sizeof(valueFunctions[0]); i++) {
    if (! marpaESLIFLua_lua_getfield(NULL,L, stacki, valueFunctions[i])) goto err;                             /* stack: stack1, ..., stacki, field */
    if (! marpaESLIFLua_lua_type(&typei, L, -1)) goto err;
    if (typei != LUA_TFUNCTION) {
      if (! marpaESLIFLua_lua_pop(L, 1)) goto err;                          /* stack: stack1, ..., stacki */
      marpaESLIFLua_luaL_errorf(L, "value table must have a field named '%s' that is a function", valueFunctions[i]);
      goto err;
    } else {
      if (! marpaESLIFLua_lua_pop(L, 1)) goto err;                          /* stack: stack1, ..., stacki */
    }
  }

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static short marpaESLIFLua_contextInitb(lua_State *L, marpaESLIFLuaContext_t *marpaESLIFLuaContextp, short unmanagedb /* not used */)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_contextInitb";

  marpaESLIFLuaContextp->L           = L;
  marpaESLIFLuaContextp->marpaESLIFp = NULL;
  marpaESLIFLuaContextp->multitonb   = 0;

  return 1;
}

/****************************************************************************/
static void marpaESLIFLua_contextFreev(marpaESLIFLuaContext_t *marpaESLIFLuaContextp, short multitonDestroyModeb)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_contextFreev";

  if (marpaESLIFLuaContextp != NULL) {
    if (multitonDestroyModeb) {
      /* We own everything - no need for checks */
      if (marpaESLIFLuaContextp->marpaESLIFp != NULL) {
	marpaESLIF_freev(marpaESLIFLuaContextp->marpaESLIFp);
      }
      free(marpaESLIFLuaContextp);
    } else {
      /* We have to free the structure and only it when this is an unmanaged marpaESLIFp */
      if (! marpaESLIFLuaContextp->multitonb) {
        free(marpaESLIFLuaContextp);
      }
    }
  }
}

/****************************************************************************/
static short marpaESLIFLua_grammarContextInitb(lua_State *L, int eslifStacki, marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp, short unmanagedb)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_grammarContextInitb";

  marpaESLIFLuaGrammarContextp->L = L;
  /* Get eslif reference - required */
  if (eslifStacki != 0) {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                   /* stack: xxx, nil */
    if (! marpaESLIFLua_lua_copy(L, eslifStacki, -1)) goto err;                                     /* stack: xxx, eslif */
    MARPAESLIFLUA_REF(L, marpaESLIFLuaGrammarContextp->eslif_r);      /* stack: xxx */
  } else {
    if (unmanagedb) {
      marpaESLIFLuaGrammarContextp->eslif_r = LUA_NOREF;
    } else {
      marpaESLIFLua_luaL_error(L, "eslifStacki must be != 0");
      goto err;
    }
  }

  marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp = NULL;
  marpaESLIFLuaGrammarContextp->managedb           = 0;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static short  marpaESLIFLua_recognizerContextInitb(lua_State *L, int grammarStacki, int recognizerInterfaceStacki, int recognizerOrigStacki, marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp, short unmanagedb)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_recognizerContextInitb";

  marpaESLIFLuaRecognizerContextp->L = L;
  /* Get grammar reference - required */
  if (grammarStacki != 0) {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                   /* stack: xxx, nil */
    if (! marpaESLIFLua_lua_copy(L, grammarStacki, -1)) goto err;                                   /* stack: xxx, grammarTable */
    MARPAESLIFLUA_REF(L, marpaESLIFLuaRecognizerContextp->grammar_r); /* stack: xxx */
  } else {
    if (unmanagedb) {
      marpaESLIFLuaRecognizerContextp->grammar_r = LUA_NOREF;
    } else {
      marpaESLIFLua_luaL_error(L, "grammarStacki must be != 0");
      goto err;
    }
  }
  /* Get recognizer reference - optional */
  if (recognizerInterfaceStacki != 0) {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                    /* stack: xxx, nil */
    if (! marpaESLIFLua_lua_copy(L, recognizerInterfaceStacki, -1)) goto err;                        /* stack: xxx, recognizerInterface */
    MARPAESLIFLUA_REF(L, marpaESLIFLuaRecognizerContextp->recognizerInterface_r);            /* stack: xxx */
  } else {
    marpaESLIFLuaRecognizerContextp->recognizerInterface_r = LUA_NOREF;
  }
  /* Get original recognizer reference (in case of newFrom()) - optional */
  if (recognizerOrigStacki != 0) {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                           /* stack: xxx, nil */
    if (! marpaESLIFLua_lua_copy(L, recognizerOrigStacki, -1)) goto err;                                    /* stack: xxx, recognizerOrigInterface */
    MARPAESLIFLUA_REF(L, marpaESLIFLuaRecognizerContextp->recognizer_orig_r); /* stack: xxx */
  } else {
    marpaESLIFLuaRecognizerContextp->recognizer_orig_r = LUA_NOREF;
  }
  marpaESLIFLuaRecognizerContextp->previousInputs        = NULL;
  marpaESLIFLuaRecognizerContextp->previousEncodings     = NULL;
  marpaESLIFLuaRecognizerContextp->lexemeStackp          = NULL;
  marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp = NULL;
  marpaESLIFLuaRecognizerContextp->managedb              = 0;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static void marpaESLIFLua_grammarContextFreev(marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp, short onStackb)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_grammarContextFreev";
  lua_State         *L;

  if (marpaESLIFLuaGrammarContextp != NULL) {
    L = marpaESLIFLuaGrammarContextp->L;

    if (marpaESLIFLuaGrammarContextp->eslif_r != LUA_NOREF) {
      MARPAESLIFLUA_UNREF(L, marpaESLIFLuaGrammarContextp->eslif_r);
    }

    if (marpaESLIFLuaGrammarContextp->managedb) {
      if (marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp != NULL) {
	marpaESLIFGrammar_freev(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp);
	marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp = NULL;
      }
      marpaESLIFLuaGrammarContextp->managedb = 0;
    } else {
      marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp = NULL;
    }

    if (! onStackb) {
      free(marpaESLIFLuaGrammarContextp);
    }
  }

 err: /* Because of MARPAESLIFLUA_UNREF */
  return;
}

/****************************************************************************/
static void  marpaESLIFLua_recognizerContextCleanupv(marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp)
/****************************************************************************/
{
  if (marpaESLIFLuaRecognizerContextp != NULL) {
    if (marpaESLIFLuaRecognizerContextp->previousInputs != NULL) {
      free(marpaESLIFLuaRecognizerContextp->previousInputs);
      marpaESLIFLuaRecognizerContextp->previousInputs = NULL;
    }
    if (marpaESLIFLuaRecognizerContextp->previousEncodings != NULL) {
      free(marpaESLIFLuaRecognizerContextp->previousEncodings);
      marpaESLIFLuaRecognizerContextp->previousEncodings = NULL;
    }
  }
}

/****************************************************************************/
static void  marpaESLIFLua_recognizerContextFreev(marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp, short onStackb)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_recognizerContextFreev";
  lua_State         *L;
  genericStack_t    *lexemeStackp;
  int                i;
  int                refi;

  if (marpaESLIFLuaRecognizerContextp != NULL) {
    L = marpaESLIFLuaRecognizerContextp->L;

    marpaESLIFLua_recognizerContextCleanupv(marpaESLIFLuaRecognizerContextp);

    if (marpaESLIFLuaRecognizerContextp->grammar_r != LUA_NOREF) {
      MARPAESLIFLUA_UNREF(L, marpaESLIFLuaRecognizerContextp->grammar_r);
    }

    if (marpaESLIFLuaRecognizerContextp->recognizerInterface_r != LUA_NOREF) {
      MARPAESLIFLUA_UNREF(L, marpaESLIFLuaRecognizerContextp->recognizerInterface_r);
    }

    if (marpaESLIFLuaRecognizerContextp->recognizer_orig_r != LUA_NOREF) {
      MARPAESLIFLUA_UNREF(L, marpaESLIFLuaRecognizerContextp->recognizer_orig_r);
    }

    lexemeStackp = marpaESLIFLuaRecognizerContextp->lexemeStackp;
    if (lexemeStackp != NULL) {
      /* It is important to delete references in the reverse order of their creation */
      while (GENERICSTACK_USED(lexemeStackp) > 0) {
        /* Last indice ... */
        i = GENERICSTACK_USED(lexemeStackp) - 1;
        /* ... is cleared ... */
        if (GENERICSTACK_IS_INT(lexemeStackp, i)) {
          refi = GENERICSTACK_GET_INT(lexemeStackp, i);
          MARPAESLIFLUA_UNREF(L, refi);
        }
        /* ... and becomes current used size */
        GENERICSTACK_USED(lexemeStackp) = i;
      }
      GENERICSTACK_FREE(marpaESLIFLuaRecognizerContextp->lexemeStackp);
    }
    if (marpaESLIFLuaRecognizerContextp->managedb) {
      if (marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp != NULL) {
	marpaESLIFRecognizer_freev(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp);
	marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp = NULL;
      }
      marpaESLIFLuaRecognizerContextp->managedb = 0;
    } else {
      marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp = NULL;
    }

    if (! onStackb) {
      free(marpaESLIFLuaRecognizerContextp);
    }
  }

 err: /* Because of MARPAESLIFLUA_UNREF */
  return;
}

/****************************************************************************/
static void  marpaESLIFLua_valueContextCleanupv(marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp)
/****************************************************************************/
{
  if (marpaESLIFLuaValueContextp != NULL) {
    if (marpaESLIFLuaValueContextp->previous_strings != NULL) {
      free(marpaESLIFLuaValueContextp->previous_strings);
      marpaESLIFLuaValueContextp->previous_strings = NULL;
    }
  }
}

/****************************************************************************/
static void  marpaESLIFLua_valueContextFreev(marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp, short onStackb)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_valueContextFreev";
  lua_State         *L;

  if (marpaESLIFLuaValueContextp != NULL) {
    L = marpaESLIFLuaValueContextp->L;

    marpaESLIFLua_valueContextCleanupv(marpaESLIFLuaValueContextp);

    /* Decrement dependencies */
    if (marpaESLIFLuaValueContextp->valueInterface_r != LUA_NOREF) {
      MARPAESLIFLUA_UNREF(L, marpaESLIFLuaValueContextp->valueInterface_r);
    }
    if (marpaESLIFLuaValueContextp->recognizerInterface_r != LUA_NOREF) {
      MARPAESLIFLUA_UNREF(L, marpaESLIFLuaValueContextp->recognizerInterface_r);
    }
    if (marpaESLIFLuaValueContextp->grammar_r != LUA_NOREF) {
      MARPAESLIFLUA_UNREF(L, marpaESLIFLuaValueContextp->grammar_r);
    }

    if (marpaESLIFLuaValueContextp->managedb) {
      if (marpaESLIFLuaValueContextp->marpaESLIFValuep != NULL) {
        marpaESLIFValue_freev(marpaESLIFLuaValueContextp->marpaESLIFValuep);
        marpaESLIFLuaValueContextp->marpaESLIFValuep = NULL;
      }
      marpaESLIFLuaValueContextp->managedb = 0;
    } else {
      marpaESLIFLuaValueContextp->marpaESLIFValuep = NULL;
    }

    if (! onStackb) {
      free(marpaESLIFLuaValueContextp);
    }
  }

 err: /* Because of MARPAESLIFLUA_UNREF */
  return;
}

/****************************************************************************/
static short  marpaESLIFLua_valueContextInitb(lua_State *L, int grammarStacki, int recognizerStacki, int valueInterfaceStacki, marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp, short unmanagedb, short grammarStackiCanBeZerob)
/****************************************************************************/
{
  static const char *funcs = "marpaESLIFLua_valueContextInitb";

  marpaESLIFLuaValueContextp->L                = L;
  /* Get value reference - required */
  if (valueInterfaceStacki != 0) {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                 /* stack: xxx, nil */
    if (! marpaESLIFLua_lua_copy(L, valueInterfaceStacki, -1)) goto err;                          /* stack: xxx, valueInterface */
    MARPAESLIFLUA_REF(L, marpaESLIFLuaValueContextp->valueInterface_r);                   /* stack: xxx */
  } else {
    if (unmanagedb) {
      marpaESLIFLuaValueContextp->valueInterface_r = LUA_NOREF;
    } else {
      marpaESLIFLua_luaL_error(L, "valueInterfaceStacki must be != 0");
      goto err;
    }
  }
  /* Get recognizer reference - optional */
  if (recognizerStacki != 0) {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                 /* stack: xxx, nil */
    if (! marpaESLIFLua_lua_copy(L, recognizerStacki, -1)) goto err;                              /* stack: xxx, recognizer */
    MARPAESLIFLUA_REF(L, marpaESLIFLuaValueContextp->recognizerInterface_r);                      /* stack: xxx */
  } else {
    /* Allowed to be unset when we come from parse */
    marpaESLIFLuaValueContextp->recognizerInterface_r = LUA_NOREF;
  }
  /* Get grammar reference - optional */
  if (grammarStacki != 0) {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                 /* stack: xxx, nil */
    if (! marpaESLIFLua_lua_copy(L, grammarStacki, -1)) goto err;                                 /* stack: xxx, grammar */
    MARPAESLIFLUA_REF(L, marpaESLIFLuaValueContextp->grammar_r);                                  /* stack: xxx */
  } else {
    if (unmanagedb || grammarStackiCanBeZerob) { /* When we come for grammar parse(), it is legal to have grammarStacki == 0 */
      marpaESLIFLuaValueContextp->grammar_r = LUA_NOREF;
    } else {
      marpaESLIFLua_luaL_error(L, "grammarStacki must be != 0");
      goto err;
    }
  }
  marpaESLIFLuaValueContextp->actions          = NULL;
  marpaESLIFLuaValueContextp->previous_strings = NULL;
  marpaESLIFLuaValueContextp->marpaESLIFValuep = NULL;
  marpaESLIFLuaValueContextp->managedb         = 0;
  marpaESLIFLuaValueContextp->symbols          = NULL;
  marpaESLIFLuaValueContextp->symboli          = -1;
  marpaESLIFLuaValueContextp->rules            = NULL;
  marpaESLIFLuaValueContextp->rulei            = -1;
  marpaESLIFLuaValueContextp->result_r         = LUA_NOREF;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static void marpaESLIFLua_genericLoggerCallbackv(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs)
/****************************************************************************/
{
  static const char                   *funcs = "marpaESLIFLua_genericLoggerCallbackv";
  marpaESLIFLuaGenericLoggerContext_t *marpaESLIFLuaGenericLoggerContextp = (marpaESLIFLuaGenericLoggerContext_t *) userDatavp;
  int                                  logger_r             = marpaESLIFLuaGenericLoggerContextp->logger_r;
  lua_State                           *L                    = marpaESLIFLuaGenericLoggerContextp->L;
  const char                          *loggerfuncs;

  switch (logLeveli) {
  case GENERICLOGGER_LOGLEVEL_TRACE:
    loggerfuncs = "trace";
    break;
  case GENERICLOGGER_LOGLEVEL_DEBUG:
    loggerfuncs = "debug";
    break;
  case GENERICLOGGER_LOGLEVEL_INFO:
    loggerfuncs = "info";
    break;
  case GENERICLOGGER_LOGLEVEL_NOTICE:
    loggerfuncs = "notice";
    break;
  case GENERICLOGGER_LOGLEVEL_WARNING:
    loggerfuncs = "warning";
    break;
  case GENERICLOGGER_LOGLEVEL_ERROR:
    loggerfuncs = "error";
    break;
  case GENERICLOGGER_LOGLEVEL_CRITICAL:
    loggerfuncs = "critical";
    break;
  case GENERICLOGGER_LOGLEVEL_ALERT:
    loggerfuncs = "alert";
    break;
  case GENERICLOGGER_LOGLEVEL_EMERGENCY:
    loggerfuncs = "emergency";
    break;
  default:
    loggerfuncs = NULL;
    break;
  }

  if (loggerfuncs != NULL) {
    MARPAESLIFLUA_CALLBACKV(L, logger_r, loggerfuncs, 1 /* nargs */, if (! marpaESLIFLua_lua_pushstring(NULL, L, msgs)) goto err;);
  }

 err:
  return;
}

/****************************************************************************/
static int marpaESLIFLua_marpaESLIFMultitonsTable_freei(lua_State *L)
/****************************************************************************/
{
  static const char                   *funcs                              = "marpaESLIFLua_marpaESLIFMultitonsTable_freei";
  marpaESLIFLuaContext_t              *marpaESLIFLuaContextp              = NULL;
  marpaESLIFLuaGenericLoggerContext_t *marpaESLIFLuaGenericLoggerContextp = NULL;
  lua_Integer                         logger_r              = LUA_NOREF;
  genericLogger_t                     *genericLoggerp        = NULL;
  marpaESLIFOption_t                  *marpaESLIFOptionp;

  /* Loop on MARPAESLIFMULTITONSTABLE */
  if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                     /* stack: MARPAESLIFMULTITONSTABLE, nil */
  while (lua_next(L, -2) != 0) {                                      /* stack: MARPAESLIFMULTITONSTABLE, marpaESLIFLuaContextp, r */
    marpaESLIFLuaContextp = lua_touserdata(L, -2);
    logger_r = lua_tointeger(L, -1);

    if (logger_r != LUA_NOREF) {

      marpaESLIFOptionp = marpaESLIF_optionp(marpaESLIFLuaContextp->marpaESLIFp);
      if (marpaESLIFOptionp != NULL) {
        genericLoggerp = marpaESLIFOptionp->genericLoggerp;
        if (genericLoggerp != NULL) {
          marpaESLIFLuaGenericLoggerContextp = (marpaESLIFLuaGenericLoggerContext_t *) genericLogger_userDatavp_getp(genericLoggerp);
	  if (marpaESLIFLuaGenericLoggerContextp != NULL) {
	    MARPAESLIFLUA_UNREF(L, marpaESLIFLuaGenericLoggerContextp->logger_r); /* By construction marpaESLIFLuaGenericLoggerContextp->logger_r == logger_r */
	    free(marpaESLIFLuaGenericLoggerContextp);
	  }
	  genericLogger_freev(&genericLoggerp);
        }
      }
    }

    marpaESLIFLua_contextFreev(marpaESLIFLuaContextp, 1 /* multitonDestroyModeb */);

    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;        /* stack: MARPAESLIFMULTITONSTABLE, marpaESLIFLuaContextp */
  }

  return 0;

 err:
  return 0;
}

#ifdef MARPAESLIFLUA_USE_INTERNALREGISTRYINDEX
/****************************************************************************/
static int marpaESLIFLua_marpaESLIFRegistryindex_freei(lua_State *L)
/****************************************************************************/
{
  static const char      *funcs = "marpaESLIFLua_marpaESLIFRegistryindex_freei";

  return 0;
}
#endif /* MARPAESLIFLUA_USE_INTERNALREGISTRYINDEX */

/****************************************************************************/
static int marpaESLIFLua_marpaESLIFGrammar_newi(lua_State *L)
/****************************************************************************/
{
  static const char              *funcs = "marpaESLIFLua_marpaESLIFGrammar_newi";
  marpaESLIFLuaContext_t         *marpaESLIFLuaContextp;
  int                            ngrammari;
  int                            i;
  marpaESLIFAction_t             defaultFreeAction;
  marpaESLIFGrammarDefaults_t    marpaESLIFGrammarDefaults;
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  marpaESLIFGrammarOption_t      marpaESLIFGrammarOption = {
    NULL, /* bytep */
    0,    /* bytel */
    NULL, /* encodings */
    0     /* encodingl */
  };
  int                            typei;
 
  switch (lua_gettop(L)) {
  case 3:
    marpaESLIFGrammarOption.encodings = (char *) luaL_checklstring(L, 3, &(marpaESLIFGrammarOption.encodingl));
    /* Intentionnaly no break */
  case 2:
    marpaESLIFGrammarOption.bytep = (char *) luaL_checklstring(L, 2, &(marpaESLIFGrammarOption.bytel));

    if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
    if (typei != LUA_TTABLE) {
      marpaESLIFLua_luaL_error(L, "marpaESLIFp must be a table");
      goto err;
    }

    if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaContextp")) goto err;   /* stack: ..., marpaESLIFLuaContextp */
    marpaESLIFLuaContextp = lua_touserdata(L, -1); /* stack: ..., marpaESLIFLuaContextp */
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;   /* stack: ... */
    break;
  default:
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_new(marpaESLIFp, string[, encoding])");
    goto err;
  }

  marpaESLIFLuaGrammarContextp = malloc(sizeof(marpaESLIFLuaGrammarContext_t));
  if (marpaESLIFLuaGrammarContextp == NULL) {
    marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
    goto err;
  }
  
  if (! marpaESLIFLua_grammarContextInitb(L, 1 /* eslifStacki */, marpaESLIFLuaGrammarContextp, 0 /* unmanagedb */)) goto err;

  marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp = marpaESLIFGrammar_newp(marpaESLIFLuaContextp->marpaESLIFp, &marpaESLIFGrammarOption);
  if (marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp == NULL) {
    int save_errno = errno;
    marpaESLIFLua_grammarContextFreev(marpaESLIFLuaGrammarContextp, 0 /* onStackb */);
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_newp failure, %s", strerror(save_errno));
    goto err;
  }
  marpaESLIFLuaGrammarContextp->managedb = 1;

  /* We want to take control over the free default action, and put something that is illegal via normal parse */
  if (! marpaESLIFGrammar_ngrammarib(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &ngrammari)) {
    int save_errno = errno;
    marpaESLIFLua_grammarContextFreev(marpaESLIFLuaGrammarContextp, 0 /* onStackb */);
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_ngrammarib failure, %s", strerror(save_errno));
    goto err;
  }
  for (i = 0; i < ngrammari; i++) {
    if (! marpaESLIFGrammar_defaults_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &marpaESLIFGrammarDefaults, i, NULL /* descp */)) {
      int save_errno = errno;
      marpaESLIFLua_grammarContextFreev(marpaESLIFLuaGrammarContextp, 0 /* onStackb */);
      marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_defaults_by_levelb failure, %s", strerror(save_errno));
      goto err;
    }
    defaultFreeAction.type    = MARPAESLIF_ACTION_TYPE_NAME;
    defaultFreeAction.u.names = ":defaultFreeActions";
    marpaESLIFGrammarDefaults.defaultFreeActionp = &defaultFreeAction;
    if (! marpaESLIFGrammar_defaults_by_level_setb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &marpaESLIFGrammarDefaults, i, NULL /* descp */)) {
      int save_errno = errno;
      marpaESLIFLua_grammarContextFreev(marpaESLIFLuaGrammarContextp, 0 /* onStackb */);
      marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_defaults_by_level_setb failure, %s", strerror(save_errno));
      goto err;
    }
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  MARPAESLIFLUA_PUSH_MARPAESLIFGRAMMAR_OBJECT(L, marpaESLIFLuaGrammarContextp);

  return 1;

 err:
  return 0;
}

#ifdef MARPAESLIFLUA_EMBEDDED
/****************************************************************************/
static int marpaESLIFLua_marpaESLIFGrammar_newFromUnmanagedi(lua_State *L, marpaESLIFGrammar_t *marpaESLIFGrammarUnmanagedp)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_newFromUnmanagedi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;

  marpaESLIFLuaGrammarContextp = malloc(sizeof(marpaESLIFLuaGrammarContext_t));
  if (marpaESLIFLuaGrammarContextp == NULL) {
    marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_grammarContextInitb(L, 0 /* eslifStacki */, marpaESLIFLuaGrammarContextp, 1 /* unmanagedb */)) goto err;
  marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp = marpaESLIFGrammarUnmanagedp;
  marpaESLIFLuaGrammarContextp->managedb           = 0;

  MARPAESLIFLUA_PUSH_MARPAESLIFGRAMMAR_OBJECT(L, marpaESLIFLuaGrammarContextp);

  return 1;

 err:
  return 0;
}
#endif /* MARPAESLIFLUA_EMBEDDED */

/****************************************************************************/
static int marpaESLIFLua_marpaESLIFGrammar_freei(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_freei";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;

  if (! marpaESLIFLua_lua_getfield(NULL,L, -1, "marpaESLIFLuaGrammarContextp")) goto err; /* stack: {...}, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = (marpaESLIFLuaGrammarContext_t *) lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;         /* stack: {...} */

  marpaESLIFLua_grammarContextFreev(marpaESLIFLuaGrammarContextp, 0 /* onStackb */);

  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;         /* stack: */

  return 0;

 err:
  return 0;
}

/****************************************************************************/
static void marpaESLIFLua_stackdumpv(lua_State* L, int forcelookupi)
/****************************************************************************/
/* Reference: https://groups.google.com/forum/#!topic/lua5/gc3Ghjo6ipg      */
/****************************************************************************/
{
  int topi   = lua_gettop(L);
  int starti = forcelookupi ? forcelookupi : 1;
  int endi   = forcelookupi ? forcelookupi : topi;
  int i;

  if (! forcelookupi) {
    printf("total in stack %d\n", topi);
  }
  
  for (i = starti; i <= endi; i++) {
    int t;

    if (! marpaESLIFLua_lua_type(&t, L, i)) goto err;
    switch (t) {
    case LUA_TNIL:
      printf("  [%d] nil\n", i);
      break;
    case LUA_TNUMBER:
      printf("  [%d] number: %g\n", i, lua_tonumber(L, i));
      break;
    case LUA_TBOOLEAN:
      printf("  [%d] boolean %s\n", i, lua_toboolean(L, i) ? "true" : "false");
      break;
    case LUA_TUSERDATA:
      printf("  [%d] userdata: %p\n", i, lua_touserdata(L, i));
      break;
    case LUA_TLIGHTUSERDATA:
      printf("  [%d] light userdata: %p\n", i, lua_touserdata(L, i));
      break;
      /*
    case LUA_TSTRING:
      printf("  [%d] string: '%s'\n", i, lua_tostring(L, i));
      break;
      */
    default:  /* other values */
      printf("  [%d] %s\n", i, lua_typename(L, t));
      break;
    }
  }
  fflush(stdout);

 err:
  return;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_ngrammari(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_ngrammari";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  int                            ngrammari;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_ngrammar(marpaESLIFGrammarp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_ngrammarib(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &ngrammari)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_ngrammarib failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) ngrammari)) goto err;   /* stack: ngrammari */

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_currentLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_currentLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  int                            leveli;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_currentLevel(marpaESLIFGrammarp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_grammar_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &leveli, NULL)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_grammar_currentb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) leveli)) goto err;   /* stack: ngrammari */

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_currentDescriptioni(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_currentDescriptioni";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  marpaESLIFString_t            *descp;
  int                            typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_currentDescription(marpaESLIFGrammarp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_grammar_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, NULL, &descp)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_grammar_currentb failure, %s", strerror(errno));
    goto err;
  }
  if (! marpaESLIFLua_lua_pushlstring(NULL, L, (const char *) descp->bytep, descp->bytel)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_descriptionByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_descriptionByLeveli";
  lua_Integer                    leveli;
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  marpaESLIFString_t            *descp;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_descriptionByLevel(marpaESLIFGrammarp, leveli)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_grammar_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) leveli, NULL, NULL, &descp)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_grammar_by_levelb failure, %s", strerror(errno));
    goto err;
  }
  if (! marpaESLIFLua_lua_pushlstring(NULL, L, (const char *) descp->bytep, descp->bytel)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_currentRuleIdsi(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_currentRuleIdsi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  int                           *ruleip;
  size_t                         rulel;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_currentRuleIds(marpaESLIFGrammarp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_rulearray_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &ruleip, &rulel)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_rulearray_currentb failure, %s", strerror(errno));
    goto err;
  }
  if (rulel <= 0) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammar_rulearray_currentb returned no rule");
    goto err;
  }

  MARPAESLIFLUA_PUSH_INTEGER_ARRAY(L, rulel, ruleip);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_ruleIdsByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_ruleIdsByLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    leveli;
  int                           *ruleip;
  size_t                         rulel;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_ruleIdsByLevel(marpaESLIFGrammarp, leveli)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_rulearray_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &ruleip, &rulel, (int) leveli, NULL)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_rulearray_by_levelb failure, %s", strerror(errno));
    goto err;
  }
  if (rulel <= 0) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammar_rulearray_by_levelb returned no rule");
    goto err;
  }

  MARPAESLIFLUA_PUSH_INTEGER_ARRAY(L, rulel, ruleip);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_currentSymbolIdsi(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_currentSymbolIdsi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  int                           *symbolip;
  size_t                         symboll;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_currentSymbolIds(marpaESLIFGrammarp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_symbolarray_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &symbolip, &symboll)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_symbolarray_currentb failure, %s", strerror(errno));
    goto err;
  }
  if (symboll <= 0) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammar_symbolarray_currentb returned no symbol");
    goto err;
  }

  MARPAESLIFLUA_PUSH_INTEGER_ARRAY(L, symboll, symbolip);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_symbolIdsByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_symbolIdsByLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    leveli;
  int                           *symbolip;
  size_t                         symboll;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_symbolIdsByLevel(marpaESLIFGrammarp, leveli)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_symbolarray_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &symbolip, &symboll, (int) leveli, NULL)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_symbolarray_by_levelb failure, %s", strerror(errno));
    goto err;
  }
  if (symboll <= 0) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammar_symbolarray_by_levelb returned no symbol");
    goto err;
  }

  MARPAESLIFLUA_PUSH_INTEGER_ARRAY(L, symboll, symbolip);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_currentPropertiesi(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_currentPropertiesi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  marpaESLIFGrammarProperty_t    grammarProperty;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_currentProperties(marpaESLIFGrammarp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_grammarproperty_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &grammarProperty)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_grammarproperty_currentb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_createtable(L, 11, 0)) goto err;                                                 /* stack; {} */
  MARPAESLIFLUA_STORE_INTEGER      (L, "level",               grammarProperty.leveli);
  MARPAESLIFLUA_STORE_INTEGER      (L, "maxlevel",            grammarProperty.maxLeveli);
  MARPAESLIFLUA_STORE_STRING       (L, "description",         grammarProperty.descp);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "latm",                grammarProperty.latmb);
  MARPAESLIFLUA_STORE_ACTION       (L, "defaultSymbolAction", grammarProperty.defaultSymbolActionp);
  MARPAESLIFLUA_STORE_ACTION       (L, "defaultRuleAction",   grammarProperty.defaultRuleActionp);
  MARPAESLIFLUA_STORE_ACTION       (L, "defaultFreeAction",   grammarProperty.defaultFreeActionp);
  MARPAESLIFLUA_STORE_INTEGER      (L, "startId",             grammarProperty.starti);
  MARPAESLIFLUA_STORE_INTEGER      (L, "discardId",           grammarProperty.discardi);
  MARPAESLIFLUA_STORE_INTEGER_ARRAY(L, "symbolIds",           grammarProperty.nsymboll, grammarProperty.symbolip);
  MARPAESLIFLUA_STORE_INTEGER_ARRAY(L, "ruleIds",             grammarProperty.nrulel, grammarProperty.ruleip);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_propertiesByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_propertiesByLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    leveli;
  marpaESLIFGrammarProperty_t    grammarProperty;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_symbolIdsByLevel(marpaESLIFGrammarp, leveli)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_grammarproperty_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &grammarProperty, (int) leveli, NULL /* descp */)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_grammarproperty_by_levelb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_createtable(L, 11, 0)) goto err;                                                 /* stack; {} */
  MARPAESLIFLUA_STORE_INTEGER      (L, "level",               grammarProperty.leveli);
  MARPAESLIFLUA_STORE_INTEGER      (L, "maxlevel",            grammarProperty.maxLeveli);
  MARPAESLIFLUA_STORE_STRING       (L, "description",         grammarProperty.descp);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "latm",                grammarProperty.latmb);
  MARPAESLIFLUA_STORE_ACTION       (L, "defaultSymbolAction", grammarProperty.defaultSymbolActionp);
  MARPAESLIFLUA_STORE_ACTION       (L, "defaultRuleAction",   grammarProperty.defaultRuleActionp);
  MARPAESLIFLUA_STORE_ACTION       (L, "defaultFreeAction",   grammarProperty.defaultFreeActionp);
  MARPAESLIFLUA_STORE_INTEGER      (L, "startId",             grammarProperty.starti);
  MARPAESLIFLUA_STORE_INTEGER      (L, "discardId",           grammarProperty.discardi);
  MARPAESLIFLUA_STORE_INTEGER_ARRAY(L, "symbolIds",           grammarProperty.nsymboll, grammarProperty.symbolip);
  MARPAESLIFLUA_STORE_INTEGER_ARRAY(L, "ruleIds",             grammarProperty.nrulel, grammarProperty.ruleip);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_currentRulePropertiesi(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_currentRulePropertiesi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    rulei;
  marpaESLIFRuleProperty_t       ruleProperty;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_currentRuleProperties(marpaESLIFGrammarp, rulei)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, rulei, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, rulei, marpaESLIFLuaGrammarContextp */
  rulei = luaL_checkinteger(L, 2);
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_ruleproperty_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) rulei, &ruleProperty)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_ruleproperty_currentb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_createtable(L, 18, 0)) goto err;                                                 /* stack; {} */
  MARPAESLIFLUA_STORE_INTEGER      (L, "id",                       ruleProperty.idi);
  MARPAESLIFLUA_STORE_STRING       (L, "description",              ruleProperty.descp);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "show",                     ruleProperty.asciishows);
  MARPAESLIFLUA_STORE_INTEGER      (L, "lhsId",                    ruleProperty.lhsi);
  MARPAESLIFLUA_STORE_INTEGER      (L, "separatorId",              ruleProperty.separatori);
  MARPAESLIFLUA_STORE_INTEGER_ARRAY(L, "rhsIds",                   ruleProperty.nrhsl, ruleProperty.rhsip);
  MARPAESLIFLUA_STORE_INTEGER      (L, "exceptionId",              ruleProperty.exceptioni);
  MARPAESLIFLUA_STORE_ACTION       (L, "action",                   ruleProperty.actionp);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "discardEvent",             ruleProperty.discardEvents);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "discardEventInitialState", ruleProperty.discardEventb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "rank",                     ruleProperty.ranki);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "nullRanksHigh",            ruleProperty.nullRanksHighb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "sequence",                 ruleProperty.sequenceb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "proper",                   ruleProperty.properb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "minimum",                  ruleProperty.minimumi);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "internal",                 ruleProperty.internalb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "propertyBitSet",           ruleProperty.propertyBitSet);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "hideseparator",            ruleProperty.hideseparatorb);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_rulePropertiesByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_rulePropertiesByLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    leveli;
  lua_Integer                    rulei;
  marpaESLIFRuleProperty_t       ruleProperty;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 3) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_rulePropertiesByLevel(marpaESLIFGrammarp, leveli, rulei)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, rulei, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, rulei, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);
  rulei = luaL_checkinteger(L, 3);
  if (! marpaESLIFLua_lua_pop(L, 4)) goto err;          /* stack: */

  if (! marpaESLIFGrammar_ruleproperty_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) rulei, &ruleProperty, (int) leveli, NULL /* descp */)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_ruleproperty_by_levelb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_createtable(L, 18, 0)) goto err;                                                 /* stack; {} */
  MARPAESLIFLUA_STORE_INTEGER      (L, "id",                       ruleProperty.idi);
  MARPAESLIFLUA_STORE_STRING       (L, "description",              ruleProperty.descp);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "show",                     ruleProperty.asciishows);
  MARPAESLIFLUA_STORE_INTEGER      (L, "lhsId",                    ruleProperty.lhsi);
  MARPAESLIFLUA_STORE_INTEGER      (L, "separatorId",              ruleProperty.separatori);
  MARPAESLIFLUA_STORE_INTEGER_ARRAY(L, "rhsIds",                   ruleProperty.nrhsl, ruleProperty.rhsip);
  MARPAESLIFLUA_STORE_INTEGER      (L, "exceptionId",              ruleProperty.exceptioni);
  MARPAESLIFLUA_STORE_ACTION       (L, "action",                   ruleProperty.actionp);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "discardEvent",             ruleProperty.discardEvents);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "discardEventInitialState", ruleProperty.discardEventb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "rank",                     ruleProperty.ranki);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "nullRanksHigh",            ruleProperty.nullRanksHighb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "sequence",                 ruleProperty.sequenceb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "proper",                   ruleProperty.properb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "minimum",                  ruleProperty.minimumi);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "internal",                 ruleProperty.internalb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "propertyBitSet",           ruleProperty.propertyBitSet);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "hideseparator",            ruleProperty.hideseparatorb);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_currentSymbolPropertiesi(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_currentSymbolPropertiesi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    symboli;
  marpaESLIFSymbolProperty_t     symbolProperty;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_currentSymbolProperties(marpaESLIFGrammarp, symboli)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  symboli = luaL_checkinteger(L, 2);
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;

  if (! marpaESLIFGrammar_symbolproperty_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) symboli, &symbolProperty)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_symbolproperty_currentb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_createtable(L, 24, 0)) goto err;                                                 /* stack; {} */
  MARPAESLIFLUA_STORE_INTEGER      (L, "type",                       symbolProperty.type);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "start",                      symbolProperty.startb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "discard",                    symbolProperty.discardb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "discardRhs",                 symbolProperty.discardRhsb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "lhs",                        symbolProperty.lhsb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "top",                        symbolProperty.topb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "id",                         symbolProperty.idi);
  MARPAESLIFLUA_STORE_STRING       (L, "description",                symbolProperty.descp);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventBefore",                symbolProperty.eventBefores);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventBeforeInitialState",    symbolProperty.eventBeforeb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventAfter",                 symbolProperty.eventAfters);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventAfterInitialState",     symbolProperty.eventAfterb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventPredicted",             symbolProperty.eventPredicteds);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventPredictedInitialState", symbolProperty.eventPredictedb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventNulled",                symbolProperty.eventNulleds);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventNulledInitialState",    symbolProperty.eventNulledb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventCompleted",             symbolProperty.eventCompleteds);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventCompletedInitialState", symbolProperty.eventCompletedb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "discardEvent",               symbolProperty.discardEvents);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "discardEventInitialState",   symbolProperty.discardEventb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "lookupResolvedLeveli",       symbolProperty.lookupResolvedLeveli);
  MARPAESLIFLUA_STORE_INTEGER      (L, "priority",                   symbolProperty.priorityi);
  MARPAESLIFLUA_STORE_ACTION       (L, "nullableAction",             symbolProperty.nullableActionp);
  MARPAESLIFLUA_STORE_INTEGER      (L, "propertyBitSet",             symbolProperty.propertyBitSet);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_symbolPropertiesByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_symbolPropertiesByLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    leveli;
  lua_Integer                    symboli;
  marpaESLIFSymbolProperty_t     symbolProperty;
  int                            rci;
  int                            typei;

  if (lua_gettop(L) != 3) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_symbolPropertiesByLevel(marpaESLIFGrammarp, leveli, symboli)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, symboli, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, symboli, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);
  symboli = luaL_checkinteger(L, 3);
  if (! marpaESLIFLua_lua_pop(L, 4)) goto err;

  if (! marpaESLIFGrammar_symbolproperty_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) symboli, &symbolProperty, (int) leveli, NULL /* descp */)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_symbolproperty_by_levelb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_createtable(L, 24, 0)) goto err;                                                 /* stack; {} */
  MARPAESLIFLUA_STORE_INTEGER      (L, "type",                       symbolProperty.type);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "start",                      symbolProperty.startb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "discard",                    symbolProperty.discardb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "discardRhs",                 symbolProperty.discardRhsb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "lhs",                        symbolProperty.lhsb);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "top",                        symbolProperty.topb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "id",                         symbolProperty.idi);
  MARPAESLIFLUA_STORE_STRING       (L, "description",                symbolProperty.descp);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventBefore",                symbolProperty.eventBefores);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventBeforeInitialState",    symbolProperty.eventBeforeb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventAfter",                 symbolProperty.eventAfters);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventAfterInitialState",     symbolProperty.eventAfterb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventPredicted",             symbolProperty.eventPredicteds);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventPredictedInitialState", symbolProperty.eventPredictedb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventNulled",                symbolProperty.eventNulleds);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventNulledInitialState",    symbolProperty.eventNulledb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "eventCompleted",             symbolProperty.eventCompleteds);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "eventCompletedInitialState", symbolProperty.eventCompletedb);
  MARPAESLIFLUA_STORE_ASCIISTRING  (L, "discardEvent",               symbolProperty.discardEvents);
  MARPAESLIFLUA_STORE_BOOLEAN      (L, "discardEventInitialState",   symbolProperty.discardEventb);
  MARPAESLIFLUA_STORE_INTEGER      (L, "lookupResolvedLeveli",       symbolProperty.lookupResolvedLeveli);
  MARPAESLIFLUA_STORE_INTEGER      (L, "priority",                   symbolProperty.priorityi);
  MARPAESLIFLUA_STORE_ACTION       (L, "nullableAction",             symbolProperty.nullableActionp);
  MARPAESLIFLUA_STORE_INTEGER      (L, "propertyBitSet",             symbolProperty.propertyBitSet);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_ruleDisplayi(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_ruleDisplayi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    rulei;
  char                          *ruleDisplays;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_ruleDisplay(marpaESLIFGrammarp, rulei)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, rulei, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, rulei, marpaESLIFLuaGrammarContextp */
  rulei = luaL_checkinteger(L, 2);
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;

  if (! marpaESLIFGrammar_ruledisplayform_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) rulei, &ruleDisplays)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_ruledisplayform_currentb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushstring(NULL, L, (const char *) ruleDisplays)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_symbolDisplayi(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_symbolDisplayi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    symboli;
  char                          *symbolDisplays;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_symbolDisplay(marpaESLIFGrammarp, symboli)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, symboli, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, symboli, marpaESLIFLuaGrammarContextp */
  symboli = luaL_checkinteger(L, 2);
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;

  if (! marpaESLIFGrammar_symboldisplayform_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) symboli, &symbolDisplays)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_symboldisplayform_currentb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushstring(NULL, L, (const char *) symbolDisplays)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_ruleShowi(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_ruleShowi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    rulei;
  char                          *ruleShows;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_ruleShow(marpaESLIFGrammarp, rulei)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, rulei, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, rulei, marpaESLIFLuaGrammarContextp */
  rulei = luaL_checkinteger(L, 2);
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;

  if (! marpaESLIFGrammar_ruleshowform_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) rulei, &ruleShows)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_ruleshowform_currentb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushstring(NULL, L, (const char *) ruleShows)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_ruleDisplayByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_ruleDisplayByLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    leveli;
  lua_Integer                    rulei;
  char                          *ruleDisplays;
  int                            typei;

  if (lua_gettop(L) != 3) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_ruleShow(marpaESLIFGrammarp, rulei)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, rulei, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, rulei, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);
  rulei = luaL_checkinteger(L, 3);
  if (! marpaESLIFLua_lua_pop(L, 4)) goto err;

  if (! marpaESLIFGrammar_ruledisplayform_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) rulei, &ruleDisplays, (int) leveli, NULL)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_ruledisplayform_by_levelb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushstring(NULL, L, (const char *) ruleDisplays)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_symbolDisplayByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_symbolDisplayByLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    leveli;
  lua_Integer                    symboli;
  char                          *symbolDisplays;
  int                            typei;

  if (lua_gettop(L) != 3) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_symbolShow(marpaESLIFGrammarp, symboli)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, symboli, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, symboli, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);
  symboli = luaL_checkinteger(L, 3);
  if (! marpaESLIFLua_lua_pop(L, 4)) goto err;

  if (! marpaESLIFGrammar_symboldisplayform_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) symboli, &symbolDisplays, (int) leveli, NULL)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_symboldisplayform_by_levelb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushstring(NULL, L, (const char *) symbolDisplays)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_ruleShowByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_ruleShowByLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    leveli;
  lua_Integer                    rulei;
  char                          *ruleShows;
  int                            typei;

  if (lua_gettop(L) != 3) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_ruleShow(marpaESLIFGrammarp, rulei)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, rulei, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, rulei, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);
  rulei = luaL_checkinteger(L, 3);
  if (! marpaESLIFLua_lua_pop(L, 4)) goto err;

  if (! marpaESLIFGrammar_ruleshowform_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, (int) rulei, &ruleShows, (int) leveli, NULL)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_ruleshowform_by_levelb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushstring(NULL, L, (const char *) ruleShows)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_showi(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_showi";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  char                          *shows;
  int                            typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_show(marpaESLIFGrammarp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 2)) goto err;

  if (! marpaESLIFGrammar_grammarshowform_currentb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &shows)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_grammarshowform_currentb failure, %s", strerror(errno));
    goto err;
  }
  if (! marpaESLIFLua_lua_pushstring(NULL, L, (const char *) shows)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_showByLeveli(lua_State *L)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFGrammar_showByLeveli";
  marpaESLIFLuaGrammarContext_t *marpaESLIFLuaGrammarContextp;
  lua_Integer                    leveli;
  char                          *shows;
  int                            typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_showByLevel(marpaESLIFGrammarp, leveli)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFLuaGrammarContextp */
  leveli = luaL_checkinteger(L, 2);                     /* stack: marpaESLIFGrammarTable, leveli, marpaESLIFGrammarp */
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;

  if (! marpaESLIFGrammar_grammarshowform_by_levelb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &shows, (int) leveli, NULL)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFGrammar_grammarshowform_by_levelb failure, %s", strerror(errno));
    goto err;
  }
  if (! marpaESLIFLua_lua_pushstring(NULL, L, (const char *) shows)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
static int  marpaESLIFLua_marpaESLIFGrammar_parsei(lua_State *L)
/****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFGrammar_parsei";
  marpaESLIFLuaGrammarContext_t    *marpaESLIFLuaGrammarContextp;
  marpaESLIFLuaRecognizerContext_t  marpaESLIFLuaRecognizerContext;
  marpaESLIFLuaValueContext_t       marpaESLIFLuaValueContext;
  marpaESLIFRecognizerOption_t      marpaESLIFRecognizerOption;
  marpaESLIFValueOption_t           marpaESLIFValueOption;
  int                               rci;
  int                               resultStacki;
  int                               typei;

  if (lua_gettop(L) != 3) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFGrammar_parse(marpaESLIFGrammarp, recognizerInterface, valueInterface)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, recognizerInterface, valueInterface, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, recognizerInterface, valueInterface, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  marpaESLIFLua_paramIsRecognizerInterfacev(L, 2);
  marpaESLIFLua_paramIsValueInterfacev(L, 3);

  if (! marpaESLIFLua_recognizerContextInitb(L, 1 /* grammarStacki */, 2 /* recognizerInterfaceStacki */, 0 /* recognizerOrigStacki */, &marpaESLIFLuaRecognizerContext, 0 /* unmanagedb */)) goto err;
  if (! marpaESLIFLua_valueContextInitb(L, 1 /* grammarStacki */, 0 /* recognizerStacki */, 3 /* valueInterfaceStacki */, &marpaESLIFLuaValueContext, 0 /* unmanagedb */, 0 /* grammarStackiCanBeZerob */)) goto err;
  
  if (! marpaESLIFLua_lua_pop(L, 3)) goto err;

  marpaESLIFRecognizerOption.userDatavp        = &marpaESLIFLuaRecognizerContext;
  marpaESLIFRecognizerOption.readerCallbackp   = marpaESLIFLua_readerCallbackb;
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaRecognizerContext.recognizerInterface_r, "isWithDisableThreshold", 0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFRecognizerOption.disableThresholdb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaRecognizerContext.recognizerInterface_r, "isWithExhaustion",       0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFRecognizerOption.exhaustedb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaRecognizerContext.recognizerInterface_r, "isWithNewline",          0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFRecognizerOption.newlineb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaRecognizerContext.recognizerInterface_r, "isWithTrack",            0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFRecognizerOption.trackb));
  marpaESLIFRecognizerOption.bufsizl           = 0; /* Recommended value */
  marpaESLIFRecognizerOption.buftriggerperci   = 50; /* Recommended value */
  marpaESLIFRecognizerOption.bufaddperci       = 50; /* Recommended value */

  marpaESLIFValueOption.userDatavp             = &marpaESLIFLuaValueContext;
  marpaESLIFValueOption.ruleActionResolverp    = marpaESLIFLua_valueRuleActionResolver;
  marpaESLIFValueOption.symbolActionResolverp  = marpaESLIFLua_valueSymbolActionResolver;
  marpaESLIFValueOption.freeActionResolverp    = marpaESLIFLua_valueFreeActionResolver;
  marpaESLIFValueOption.transformerp           = &marpaESLIFLuaValueResultTransformDefault;
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaValueContext.valueInterface_r, "isWithHighRankOnly", 0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.highRankOnlyb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaValueContext.valueInterface_r, "isWithOrderByRank",  0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.orderByRankb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaValueContext.valueInterface_r, "isWithAmbiguous",    0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.ambiguousb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaValueContext.valueInterface_r, "isWithNull",         0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.nullb));
  MARPAESLIFLUA_CALLBACKI(L, marpaESLIFLuaValueContext.valueInterface_r, "maxParses",          0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.maxParsesi));

  if ((rci = marpaESLIFGrammar_parseb(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &marpaESLIFRecognizerOption, &marpaESLIFValueOption, NULL)) != 0) {
    resultStacki = lua_gettop(L);
    /* marpaESLIFGrammar_parseb called the transformers that pushed the final value to the stack */
    MARPAESLIFLUA_CALLBACKV(L, marpaESLIFLuaValueContext.valueInterface_r, "setResult", 1 /* nargs */, if (! marpaESLIFLua_lua_pushnil(L)) goto err; if (! marpaESLIFLua_lua_copy(L, resultStacki, -1)) goto err;);
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;
  }

  marpaESLIFLua_valueContextFreev(&marpaESLIFLuaValueContext, 1 /* onStackb */);
  marpaESLIFLua_recognizerContextFreev(&marpaESLIFLuaRecognizerContext, 1 /* onStackb */);

  if (! marpaESLIFLua_lua_pushboolean(L, rci)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_readerCallbackb(void *userDatavp, char **inputcpp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingsp, size_t *encodinglp)
/*****************************************************************************/
{
  static const char                *funcs                           = "marpaESLIFLua_readerCallbackb";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp = (marpaESLIFLuaRecognizerContext_t *) userDatavp;
  lua_State                        *L                               = marpaESLIFLuaRecognizerContextp->L;
  int                               recognizerInterface_r           = marpaESLIFLuaRecognizerContextp->recognizerInterface_r;
  char                             *inputs                          = NULL;
  char                             *encodings                       = NULL;
  size_t                            inputl;
  size_t                            encodingl;
  short                             callbackb;

  marpaESLIFLua_recognizerContextCleanupv(marpaESLIFLuaRecognizerContextp);

  /* Call the read interface */
  MARPAESLIFLUA_CALLBACKB(L, recognizerInterface_r, "read", 0 /* nargs */, MARPAESLIFLUA_NOOP, &callbackb);
  if (! callbackb) {
    marpaESLIFLua_luaL_errorf(L, "Recognizer read method failure, %s", strerror(errno));
    goto err;
  }

  /* Call the data interface */
  MARPAESLIFLUA_CALLBACKS(L, recognizerInterface_r, "data", 0 /* nargs */, MARPAESLIFLUA_NOOP, &inputs, &inputl);

  /* Call the encoding interface */
  MARPAESLIFLUA_CALLBACKS(L, recognizerInterface_r, "encoding", 0 /* nargs */, MARPAESLIFLUA_NOOP, &encodings, &encodingl);

  *inputcpp             = inputs;
  *inputlp              = (size_t) inputl;
  MARPAESLIFLUA_CALLBACKB(L, recognizerInterface_r, "isEof",             0 /* nargs */, MARPAESLIFLUA_NOOP, eofbp);
  MARPAESLIFLUA_CALLBACKB(L, recognizerInterface_r, "isCharacterStream", 0 /* nargs */, MARPAESLIFLUA_NOOP, characterStreambp);
  *encodingsp           = encodings;
  *encodinglp           = encodingl;

  marpaESLIFLuaRecognizerContextp->previousInputs    = (char *) inputs;
  marpaESLIFLuaRecognizerContextp->previousEncodings = (char *) encodings;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static marpaESLIFValueRuleCallback_t marpaESLIFLua_valueRuleActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_valueRuleActionResolver";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  /* Just remember the action name - lua will croak if calling this method fails */
  marpaESLIFLuaValueContextp->actions = actions;

  return marpaESLIFLua_valueRuleCallbackb;
}

/*****************************************************************************/
static marpaESLIFValueSymbolCallback_t marpaESLIFLua_valueSymbolActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_valueSymbolActionResolver";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  /* Just remember the action name - lua will croak if calling this method fails */
  marpaESLIFLuaValueContextp->actions = actions;

  return marpaESLIFLua_valueSymbolCallbackb;
}

/*****************************************************************************/
static marpaESLIFValueFreeCallback_t marpaESLIFLua_valueFreeActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_valueFreeActionResolver";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  /* It HAS to be ":defaultFreeActions" */
  if (strcmp(actions, ":defaultFreeActions") != 0) {
    return NULL;
  }

  /* Just remember the action name - lua will croak if calling this method fails */
  marpaESLIFLuaValueContextp->actions = actions;

  return marpaESLIFLua_valueFreeCallbackv;
}

/*****************************************************************************/
static short marpaESLIFLua_valueRuleCallbackb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_valueRuleCallbackb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;
  int                          topi;
  int                          newtopi;
  int                          i;

  /* Get value context */
  if (! marpaESLIFValue_contextb(marpaESLIFValuep, &(marpaESLIFLuaValueContextp->symbols), &(marpaESLIFLuaValueContextp->symboli), &(marpaESLIFLuaValueContextp->rules), &(marpaESLIFLuaValueContextp->rulei))) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFValue_contextb failure, %s", strerror(errno));
    goto err;
  }

  topi = lua_gettop(L);
  MARPAESLIFLUA_CALLBACK(L, marpaESLIFLuaValueContextp->valueInterface_r, marpaESLIFLuaValueContextp->actions, nullableb ? 0 : (argni - arg0i + 1) /* nargs */, 
                         if (! nullableb) {
                           for (i = arg0i; i <= argni; i++) {
                             if (! marpaESLIFLua_pushValueb(marpaESLIFLuaValueContextp, marpaESLIFValuep, i, NULL /* bytep */, 0 /* bytel */)) goto err;
                           }
                         }
                         );
  newtopi = lua_gettop(L);
  if (newtopi == topi) {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;
  } else {
    if (newtopi != (topi + 1)) {
      marpaESLIFLua_luaL_errorf(L, "Function %s must return exactly one or zero value", marpaESLIFLuaValueContextp->actions);
      goto err;
    }
  }

  MARPAESLIFLUA_SET_VALUE(marpaESLIFLuaValueContextp, marpaESLIFValuep, resulti, marpaESLIFLua_representationb);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_valueSymbolCallbackb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *bytep, size_t bytel, int resulti)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_valueSymbolCallbackb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;
  int                          topi;
  int                          newtopi;

  /* Get value context */
  if (! marpaESLIFValue_contextb(marpaESLIFValuep, &(marpaESLIFLuaValueContextp->symbols), &(marpaESLIFLuaValueContextp->symboli), &(marpaESLIFLuaValueContextp->rules), &(marpaESLIFLuaValueContextp->rulei))) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFValue_contextb failure, %s", strerror(errno));
    goto err;
  }

  topi = lua_gettop(L);
  MARPAESLIFLUA_CALLBACK(L, marpaESLIFLuaValueContextp->valueInterface_r, marpaESLIFLuaValueContextp->actions, 1 /* nargs */, if (! marpaESLIFLua_pushValueb(marpaESLIFLuaValueContextp, marpaESLIFValuep, -1 /* stackindicei */, bytep, bytel)) goto err;);
  newtopi = lua_gettop(L);
  if (newtopi == topi) {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;
  } else {
    if (newtopi != (topi + 1)) {
      marpaESLIFLua_luaL_errorf(L, "Function %s must return exactly one or zero value", marpaESLIFLuaValueContextp->actions);
      goto err;
    }
  }

  MARPAESLIFLUA_SET_VALUE(marpaESLIFLuaValueContextp, marpaESLIFValuep, resulti, marpaESLIFLua_representationb);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static void marpaESLIFLua_valueFreeCallbackv(void *userDatavp, void *contextp, marpaESLIFValueType_t type, void *p, size_t sizel)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_valueFreeCallbackv";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  /* we always and only push a PTR, so no need to check the context */
  MARPAESLIFLUA_UNREF(L, * (int *) p);
  free(p);

 err:
  return;
}

/*****************************************************************************/
static short marpaESLIFLua_transformUndefb(void *userDatavp, void *contextp)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformUndefb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  if (! marpaESLIFLua_lua_pushnil(marpaESLIFLuaValueContextp->L)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_transformCharb(void *userDatavp, void *contextp, char c)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformCharb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  if (! marpaESLIFLua_lua_pushlstring(NULL, marpaESLIFLuaValueContextp->L, &c, 1)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_transformShortb(void *userDatavp, void *contextp, short b)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformShortb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  return marpaESLIFLua_lua_pushinteger(marpaESLIFLuaValueContextp->L, (lua_Integer) b);
}

/*****************************************************************************/
static short marpaESLIFLua_transformIntb(void *userDatavp, void *contextp, int i)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformIntb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  return marpaESLIFLua_lua_pushinteger(marpaESLIFLuaValueContextp->L, (lua_Integer) i);
}

/*****************************************************************************/
static short marpaESLIFLua_transformLongb(void *userDatavp, void *contextp, long l)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformLongb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  return marpaESLIFLua_lua_pushinteger(marpaESLIFLuaValueContextp->L, (lua_Integer) l);
}

/*****************************************************************************/
static short marpaESLIFLua_transformFloatb(void *userDatavp, void *contextp, float f)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformFloatb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  if (! marpaESLIFLua_lua_pushnumber(marpaESLIFLuaValueContextp->L, (lua_Number) f)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_transformDoubleb(void *userDatavp, void *contextp, double d)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformDoubleb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  if (! marpaESLIFLua_lua_pushnumber(marpaESLIFLuaValueContextp->L, (lua_Number) d)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_transformPtrb(void *userDatavp, void *contextp, void *p)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformPtrb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  if (contextp == MARPAESLIFLUA_CONTEXT) {
    /* This is a pointer to an integer value that is a global reference to the real value */
    MARPAESLIFLUA_DEREF(L, * (int *) p);
  } else {
    if (! marpaESLIFLua_lua_pushlightuserdata(marpaESLIFLuaValueContextp->L, p)) goto err;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_transformArrayb(void *userDatavp, void *contextp, void *p, size_t sizel)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformArrayb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  if (! marpaESLIFLua_lua_pushlstring(NULL, marpaESLIFLuaValueContextp->L, p, sizel)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_transformBoolb(void *userDatavp, void *contextp, short y)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_transformBoolb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;

  if (! marpaESLIFLua_lua_pushboolean(marpaESLIFLuaValueContextp->L, (int) y)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_pushValueb(marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp, marpaESLIFValue_t *marpaESLIFValuep, int stackindicei, char *bytep, size_t bytel)
/*****************************************************************************/
{
  static const char       *funcs = "marpaESLIFLua_pushValueb";
  lua_State               *L     = marpaESLIFLuaValueContextp->L;
  marpaESLIFValueResult_t *marpaESLIFValueResultp;
  marpaESLIFValueResult_t  marpaESLIFValueResult;

  if (bytep != NULL) {
    /* Fake a marpaESLIFValueResult */
    marpaESLIFValueResult.type         = MARPAESLIF_VALUE_TYPE_ARRAY;
    marpaESLIFValueResult.contextp     = NULL;
    marpaESLIFValueResult.u.a.p        = bytep;
    marpaESLIFValueResult.u.a.shallowb = 1;
    marpaESLIFValueResult.u.a.sizel    = bytel;
    marpaESLIFValueResultp             = &marpaESLIFValueResult;
  } else {
    marpaESLIFValueResultp = marpaESLIFValue_stack_getp(marpaESLIFValuep, stackindicei);
    if (marpaESLIFValueResultp == NULL) {
      marpaESLIFLua_luaL_errorf(L, "marpaESLIFValueResultp is NULL at stack indice %d", stackindicei);
      goto err;
    }
  }

  if (! marpaESLIFValue_transformb(marpaESLIFValuep, marpaESLIFValueResultp, NULL /* marpaESLIFValueResultResolvedp */)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFValue_transformb failure, %s", strerror(errno));
    goto err;
  }

  /* Dereference eventual last result and keep a reference to the new one */
  MARPAESLIFLUA_UNREF(L, marpaESLIFLuaValueContextp->result_r);
  if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                                                      /* stack: value, nil */
  if (! marpaESLIFLua_lua_copy(L, -2, -1)) goto err;                                                                 /* stack: value, value */
  MARPAESLIFLUA_REF(L, marpaESLIFLuaValueContextp->result_r);                                       /* stack: value */

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short marpaESLIFLua_representationb(void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp, char **inputcpp, size_t *inputlp)
/*****************************************************************************/
{
  static const char           *funcs                      = "marpaESLIFLua_representationb";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) userDatavp;
  lua_State                   *L                          = marpaESLIFLuaValueContextp->L;
  const char                  *s;
  size_t                       l;

  marpaESLIFLua_valueContextCleanupv(marpaESLIFLuaValueContextp);

  /* We always push a PTR */
  if (marpaESLIFValueResultp->type != MARPAESLIF_VALUE_TYPE_PTR) {
    marpaESLIFLua_luaL_errorf(L, "User-defined value type is not MARPAESLIF_VALUE_TYPE_PTR but %d", marpaESLIFValueResultp->type);
    goto err;
  }
  /* Our context is always MARPAESLIFLUA_CONTEXT */
  if (marpaESLIFValueResultp->contextp != MARPAESLIFLUA_CONTEXT) {
    marpaESLIFLua_luaL_errorf(L, "User-defined value context is not ESLIF_PERL_CONTEXT but %d", marpaESLIFValueResultp->contextp);
    goto err;
  }

  MARPAESLIFLUA_DEREF(L, * (int *) marpaESLIFValueResultp->u.p.p);
  s = lua_tolstring(L, -1, &l);
  if ((s != NULL) && (l > 0)) {
    /* No guarantee this will survive the lua call, so we keep an explicitly copy */
    /* until marpaESLIF also takes a copy. */
    *inputcpp = (char *) malloc(l);
    if (*inputcpp == NULL) {
      marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
      goto err;
    }
    memcpy(*inputcpp, s, l);
    *inputlp = l;
    marpaESLIFLuaValueContextp->previous_strings = (void *) *inputcpp;
  }
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Always return a true value, else ::concat will abort */
  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static void marpaESLIFLua_iterate_and_print(lua_State *L, int index)
/*****************************************************************************/
/* C.f. https://stackoverflow.com/questions/6137684/iterate-through-lua-table */
/*****************************************************************************/
{
  const char *key;
  const char *value;

  /* Push another reference to the table on top of the stack (so we know */
  /* where it is, and this function can work for negative, positive and */
  /* pseudo indices */
  if (! marpaESLIFLua_lua_pushvalue(L, index)) goto err;
  /* stack now contains: -1 => table */
  if (! marpaESLIFLua_lua_pushnil(L)) goto err;
  /* stack now contains: -1 => nil; -2 => table */
  while (lua_next(L, -2)) {
    /* stack now contains: -1 => value; -2 => key; -3 => table */
    /* copy the key so that lua_tostring does not modify the original */
    if (! marpaESLIFLua_lua_pushvalue(L, -2)) goto err;
    /* stack now contains: -1 => key; -2 => value; -3 => key; -4 => table */
    key = lua_tostring(L, -1);
    value = lua_tostring(L, -2);
    printf("%s => %s\n", key, value);
    /* pop value + copy of key, leaving original key */
    if (! marpaESLIFLua_lua_pop(L, 2)) goto err;
    /* stack now contains: -1 => key; -2 => table */
  }
  /* stack now contains: -1 => table (when lua_next returns 0 it pops the key */
  /* but does not push anything.) */
  /* Pop table */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;
  /* Stack is now the same as it was on entry to this function */

 err:
  return;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_newi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_newi";
  marpaESLIFLuaGrammarContext_t    *marpaESLIFLuaGrammarContextp;
  marpaESLIFRecognizerOption_t      marpaESLIFRecognizerOption;
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  int                               typei;
 
  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_new(marpaESLIFGrammarp, recognizerInterface)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaGrammarContextp")) goto err;   /* stack: marpaESLIFGrammarTable, recognizerInterface, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1); /* stack: marpaESLIFGrammarTable, recognizerInterface, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  marpaESLIFLua_paramIsRecognizerInterfacev(L, 2);
  
  marpaESLIFLuaRecognizerContextp = (marpaESLIFLuaRecognizerContext_t *) malloc(sizeof(marpaESLIFLuaRecognizerContext_t));
  if (marpaESLIFLuaRecognizerContextp == NULL) {
    marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_recognizerContextInitb(L, 1 /* grammarStacki */, 2 /* recognizerInterfaceStacki */, 0 /* recognizerOrigStacki */, marpaESLIFLuaRecognizerContextp, 0 /* unmanagedb */)) goto err;

  /* We need a lexeme stack in this mode (in contrary to the parse() method that never calls back) */
  GENERICSTACK_NEW(marpaESLIFLuaRecognizerContextp->lexemeStackp);
  if (marpaESLIFLuaRecognizerContextp->lexemeStackp == NULL) {
    int save_errno = errno;
    marpaESLIFLua_recognizerContextFreev(marpaESLIFLuaRecognizerContextp, 0 /* onStackb */);
    marpaESLIFLua_luaL_errorf(L, "GENERICSTACK_NEW failure, %s", strerror(save_errno));
    goto err;
  }

  marpaESLIFRecognizerOption.userDatavp        = marpaESLIFLuaRecognizerContextp;
  marpaESLIFRecognizerOption.readerCallbackp   = marpaESLIFLua_readerCallbackb;
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaRecognizerContextp->recognizerInterface_r, "isWithDisableThreshold", 0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFRecognizerOption.disableThresholdb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaRecognizerContextp->recognizerInterface_r, "isWithExhaustion",       0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFRecognizerOption.exhaustedb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaRecognizerContextp->recognizerInterface_r, "isWithNewline",          0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFRecognizerOption.newlineb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaRecognizerContextp->recognizerInterface_r, "isWithTrack",            0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFRecognizerOption.trackb));
  marpaESLIFRecognizerOption.bufsizl           = 0; /* Recommended value */
  marpaESLIFRecognizerOption.buftriggerperci   = 50; /* Recommended value */
  marpaESLIFRecognizerOption.bufaddperci       = 50; /* Recommended value */

  marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp = marpaESLIFRecognizer_newp(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, &marpaESLIFRecognizerOption);
  marpaESLIFLuaRecognizerContextp->managedb = 1;

  if (marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp == NULL) {
    int save_errno = errno;
    marpaESLIFLua_recognizerContextFreev(marpaESLIFLuaRecognizerContextp, 0 /* onStackb */);
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_newp failure, %s", strerror(save_errno));
    goto err;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  MARPAESLIFLUA_PUSH_MARPAESLIFRECOGNIZER_OBJECT(L, marpaESLIFLuaRecognizerContextp);

  return 1;

 err:
  return 0;
}

#ifdef MARPAESLIFLUA_EMBEDDED
/****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_newFromUnmanagedi(lua_State *L, marpaESLIFRecognizer_t *marpaESLIFRecognizerUnmanagedp)
/****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_newFromUnmanagedi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;

  marpaESLIFLuaRecognizerContextp = (marpaESLIFLuaRecognizerContext_t *) malloc(sizeof(marpaESLIFLuaRecognizerContext_t));
  if (marpaESLIFLuaRecognizerContextp == NULL) {
    marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_recognizerContextInitb(L, 0 /* grammarStacki */, 0 /* recognizerInterfaceStacki */, 0 /* recognizerOrigStacki */, marpaESLIFLuaRecognizerContextp, 1 /* unmanagedb */)) goto err;
  marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp = marpaESLIFRecognizerUnmanagedp;
  marpaESLIFLuaRecognizerContextp->managedb              = 0;

  MARPAESLIFLUA_PUSH_MARPAESLIFRECOGNIZER_OBJECT(L, marpaESLIFLuaRecognizerContextp);

  return 1;

 err:
  return 0;
}
#endif /* MARPAESLIFLUA_EMBEDDED */

/****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_freei(lua_State *L)
/****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_freei";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;

  if (! marpaESLIFLua_lua_getfield(NULL,L, -1, "marpaESLIFLuaRecognizerContextp")) goto err; /* stack: {...}, marpaESLIFLuaRecognizerContextp */
  marpaESLIFLuaRecognizerContextp = (marpaESLIFLuaRecognizerContext_t *) lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  marpaESLIFLua_recognizerContextFreev(marpaESLIFLuaRecognizerContextp, 0 /* onStackb */);

  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  return 0;
 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_newFromi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_newFromi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextFromp;
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  marpaESLIFLuaGrammarContext_t    *marpaESLIFLuaGrammarContextp;
  int                               typei;
 
  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_newFrom(marpaESLIFRecognizerp, marpaESLIFGrammarp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFGrammarp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;       /* stack: marpaESLIFRecognizerTable, marpaESLIFGrammarTable, marpaESLIFLuaRecognizerContextFromp */
  marpaESLIFLuaRecognizerContextFromp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, marpaESLIFGrammarTable, marpaESLIFLuaRecognizerContextFromp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_getfield(NULL,L, 2, "marpaESLIFLuaGrammarContextp")) goto err;          /* stack: marpaESLIFRecognizerTable, marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  marpaESLIFLuaGrammarContextp = lua_touserdata(L, -1);        /* stack: marpaESLIFRecognizerTable, marpaESLIFGrammarTable, marpaESLIFLuaGrammarContextp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  marpaESLIFLuaRecognizerContextp = (marpaESLIFLuaRecognizerContext_t *) malloc(sizeof(marpaESLIFLuaRecognizerContext_t));
  if (marpaESLIFLuaRecognizerContextp == NULL) {
    marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_recognizerContextInitb(L, 2 /* grammarStacki */, 0 /* recognizerInterfaceStacki */, 1 /* recognizerOrigStacki */, marpaESLIFLuaRecognizerContextp, 0 /* unmanagedb */)) goto err;

  /* We need a lexeme stack in this mode (in contrary to the parse() method that never calls back) */
  GENERICSTACK_NEW(marpaESLIFLuaRecognizerContextp->lexemeStackp);
  if (marpaESLIFLuaRecognizerContextp->lexemeStackp == NULL) {
    int save_errno = errno;
    marpaESLIFLua_recognizerContextFreev(marpaESLIFLuaRecognizerContextp, 0 /* onStackb */);
    marpaESLIFLua_luaL_errorf(L, "GENERICSTACK_NEW failure, %s", strerror(save_errno));
    goto err;
  }

  marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp = marpaESLIFRecognizer_newFromp(marpaESLIFLuaGrammarContextp->marpaESLIFGrammarp, marpaESLIFLuaRecognizerContextFromp->marpaESLIFRecognizerp);
  marpaESLIFLuaRecognizerContextp->managedb              = 1;

  if (marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp == NULL) {
    int save_errno = errno;
    marpaESLIFLua_recognizerContextFreev(marpaESLIFLuaRecognizerContextp, 0 /* onStackb */);
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_newFromp failure, %s", strerror(save_errno));
    goto err;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  MARPAESLIFLUA_PUSH_MARPAESLIFRECOGNIZER_OBJECT(L, marpaESLIFLuaRecognizerContextp);

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_set_exhausted_flagi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_set_exhausted_flagi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  int                               typei;
 
  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_set_exhausted_flag(marpaESLIFRecognizerp, flag)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TBOOLEAN) {
    marpaESLIFLua_luaL_error(L, "flag must be a boolean");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;   /* stack: marpaESLIFRecognizerTable, marpaESLIFGrammarTable, marpaESLIFLuaRecognizerContextFromp */
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, marpaESLIFGrammarTable, marpaESLIFLuaRecognizerContextFromp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFRecognizer_set_exhausted_flagb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, lua_toboolean(L, 2) ? 1 : 0)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_set_exhausted_flagb failure, %s", strerror(errno));
    goto err;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  return 0;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_sharei(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_sharei";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextSharedp;
  int                               typei;
 
  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_sharei(marpaESLIFRecognizerp, marpaESLIFRecognizerSharedp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerSharedp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;   /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, marpaESLIFLuaRecognizerContextFromp */
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, marpaESLIFLuaRecognizerContextFromp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_getfield(NULL,L, 2, "marpaESLIFLuaRecognizerContextp")) goto err;         /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, marpaESLIFLuaRecognizerContextFromp */
  marpaESLIFLuaRecognizerContextSharedp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, marpaESLIFLuaRecognizerContextFromp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /*
   * The eventual previous reference on another shared recognizer has its refcount decreased.
   */
  if (marpaESLIFLuaRecognizerContextp->recognizer_orig_r != LUA_NOREF) {
    MARPAESLIFLUA_UNREF(L, marpaESLIFLuaRecognizerContextp->recognizer_orig_r);
  }

  if (! marpaESLIFLua_lua_pushnil(L)) goto err;                                              /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, nil */
  if (! marpaESLIFLua_lua_copy(L, 2, -1)) goto err;                                          /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, marpaESLIFRecognizerSharedTable */
  MARPAESLIFLUA_REF(L, marpaESLIFLuaRecognizerContextp->recognizer_orig_r); /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable */

  if (! marpaESLIFRecognizer_shareb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, marpaESLIFLuaRecognizerContextSharedp->marpaESLIFRecognizerp)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_shareb failure, %s", strerror(errno));
    goto err;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  return 0;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_isCanContinuei(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_isCanContinuei";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  short                             isCanContinueb;
  int                               typei;
 
  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_isCanContinue(marpaESLIFRecognizerp, marpaESLIFRecognizerSharedp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;   /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, marpaESLIFLuaRecognizerContextFromp */
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, marpaESLIFLuaRecognizerContextFromp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_isCanContinueb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &isCanContinueb)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_isCanContinueb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushboolean(L, isCanContinueb)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_isExhaustedi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_isExhaustedi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  short                             exhaustedb;
  int                               typei;
 
  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_isexhausted(marpaESLIFRecognizerp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;   /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, marpaESLIFLuaRecognizerContextFromp */
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, marpaESLIFRecognizerSharedTable, marpaESLIFLuaRecognizerContextFromp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_isExhaustedb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &exhaustedb)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_isExhaustedb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushboolean(L, exhaustedb)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_scani(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_scani";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  short                             initialEventsb = 0;
  int                               typei;
 
  switch (lua_gettop(L)) {
  case 2:
    if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
    if (typei != LUA_TBOOLEAN) {
      marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_scan(marpaESLIFRecognizerp, initialEvents)");
      goto err;
    }
    initialEventsb = lua_toboolean(L, 2) ? 1 : 0;
    /* Intentionnaly no break here */
  case 1:
    if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
    if (typei != LUA_TTABLE) {
      marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
      goto err;
    }
    if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;   /* stack: marpaESLIFRecognizerTable, initialEventsb?, marpaESLIFLuaRecognizerContextFromp */
    marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, initialEventsb?, marpaESLIFLuaRecognizerContextFromp */
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;
    break;
  default:
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_scan(marpaESLIFRecognizerp[, initialEvents])");
    goto err;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFLua_lua_pushboolean(L, marpaESLIFRecognizer_scanb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, initialEventsb, NULL /* continuebp */, NULL /* exhaustedbp */))) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_resumei(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_resumei";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  int                               isNumi;
  int                               deltaLengthi = 0;
  int                               typei;
 
  switch (lua_gettop(L)) {
  case 2:
    if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
    if (typei != LUA_TNUMBER) {
      marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_resume(marpaESLIFRecognizerp, deltaLength) (got typei=%d != %d)");
      goto err;
    }
    deltaLengthi = (int) lua_tointegerx(L, 2, &isNumi);
    if (! isNumi) {
      marpaESLIFLua_luaL_error(L, "Failed to convert deltaLength argument to a number");
      goto err;
    }
    if (deltaLengthi < 0) {
      marpaESLIFLua_luaL_error(L, "deltaLength argument cannot be negative");
      goto err;
    }
    /* Intentionnaly no break here */
  case 1:
    if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
    if (typei != LUA_TTABLE) {
      marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
      goto err;
    }
    if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;   /* stack: marpaESLIFRecognizerTable, deltaLength?, marpaESLIFLuaRecognizerContextFromp */
    marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, deltaLength?, marpaESLIFLuaRecognizerContextFromp */
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;
    break;
  default:
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_resume(marpaESLIFRecognizerp[, deltaLength])");
    goto err;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFLua_lua_pushboolean(L, marpaESLIFRecognizer_resumeb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, (size_t) deltaLengthi, NULL /* continuebp */, NULL /* exhaustedbp */))) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_eventsi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_eventsi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  size_t                            i;
  size_t                            eventArrayl;
  marpaESLIFEvent_t                *eventArrayp;
  int                               rci;
  int                               typei;
 
  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_events(marpaESLIFRecognizerp)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;   /* stack: marpaESLIFRecognizerTable, initialEventsb?, marpaESLIFLuaRecognizerContextFromp */
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, initialEventsb?, marpaESLIFLuaRecognizerContextFromp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_eventb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &eventArrayl, &eventArrayp)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_eventb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_newtable(L)) goto err;                          /* stack: {} */
  for (i = 0; i < eventArrayl; i++) {
    if (! marpaESLIFLua_lua_newtable(L)) goto err;                        /* stack: {}, {} */
    MARPAESLIFLUA_STORE_INTEGER(L, "type", eventArrayp[i].type);          /* stack: {}, {"type" => type} */
    MARPAESLIFLUA_STORE_ASCIISTRING(L, "symbol", eventArrayp[i].symbols); /* stack: {}, {"type" => type, "symbol" => symbol} */
    MARPAESLIFLUA_STORE_ASCIISTRING(L, "event", eventArrayp[i].events);   /* stack: {}, {"type" => type, "symbol" => symbol, "event" => event} */
    if (! marpaESLIFLua_lua_rawseti(L, 1, (int) i)) goto err;             /* stack: {i => {"type" => type, "symbol" => symbol, "event" => event}} */
  }

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_eventOnOffi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs     = "marpaESLIFLua_marpaESLIFRecognizer_eventOnOffi";
  marpaESLIFEventType_t             eventSeti = MARPAESLIF_EVENTTYPE_NONE;
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  const char                       *symbols;
  int                               isNumi;
  int                               codei;
  int                               typei;

  if (lua_gettop(L) != 4) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_eventOnOff(marpaESLIFRecognizerp, symbol, eventTypes, onOff)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;   /* stack: marpaESLIFRecognizerTable, symbol, eventTypes, onOff, marpaESLIFLuaRecognizerContextFromp */
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1); /* stack: marpaESLIFRecognizerTable, symbol, eventTypes, onOff, marpaESLIFLuaRecognizerContextFromp */
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TSTRING) {
    marpaESLIFLua_luaL_error(L, "symbol must be a string");
    goto err;
  }
  symbols = lua_tostring(L, 2);

  if (! marpaESLIFLua_lua_type(&typei, L, 3)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "eventTypes must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_pushnil(L)) goto err;
  while (lua_next(L, 3) != 0) {
    codei = (int) lua_tonumberx(L, -1, &isNumi);
    if (! isNumi) {
      marpaESLIFLua_luaL_error(L, "Failed to convert event type to a number");
      goto err;
    }
    switch (codei) {
    case MARPAESLIF_EVENTTYPE_NONE:
      break;
    case MARPAESLIF_EVENTTYPE_COMPLETED:
    case MARPAESLIF_EVENTTYPE_NULLED:
    case MARPAESLIF_EVENTTYPE_PREDICTED:
    case MARPAESLIF_EVENTTYPE_BEFORE:
    case MARPAESLIF_EVENTTYPE_AFTER:
    case MARPAESLIF_EVENTTYPE_EXHAUSTED:
    case MARPAESLIF_EVENTTYPE_DISCARD:
      eventSeti |= codei;
      break;
    default:
      marpaESLIFLua_luaL_errorf(L, "Unknown code %d", (int) codei);
      goto err;
    }
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 4)) goto err;
  if (typei != LUA_TBOOLEAN) {
    marpaESLIFLua_luaL_error(L, "onOff must be a boolean");
    goto err;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_event_onoffb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, (char *) symbols, eventSeti, lua_toboolean(L, 4) ? 1 : 0)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_event_onoffb failure, %s", strerror(errno));
    goto err;
  }

  return 0;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lexemeAlternativei(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs          = "marpaESLIFLua_marpaESLIFRecognizer_lexemeAlternativei";
  size_t                            grammarLengthl = 1;
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  const char                       *names;
  int                               isNumi;
  int                               refi;
  marpaESLIFAlternative_t           marpaESLIFAlternative;
  int                              *p;
  int                               typei;

  switch (lua_gettop(L)) {
  case 4:
    if (! marpaESLIFLua_lua_type(&typei, L, 4)) goto err;
    if (typei != LUA_TNUMBER) {
      marpaESLIFLua_luaL_error(L, "grammarLength must be a number");
      goto err;
    }
    grammarLengthl = (size_t) lua_tonumberx(L, 4, &isNumi);
    if (! isNumi) {
      marpaESLIFLua_luaL_error(L, "Failed to convert grammarLengths to a number");
      goto err;
    }
    /* Intentionnaly no break */
  case 3:
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
    if (typei != LUA_TTABLE) {
      marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
      goto err;
    }
    if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
    marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

    if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
    if (typei != LUA_TSTRING) {
      marpaESLIFLua_luaL_error(L, "name must be a string");
      goto err;
    }
    names = lua_tostring(L, 2);

    /* We make a reference to the value and remember that in an (int *) pointer */
    /* in order to re-use the marpaESLIFLua_representationb method */
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;
    if (! marpaESLIFLua_lua_copy(L, 3, -1)) goto err;
    MARPAESLIFLUA_REF(L, refi);
    
    p = (int *) malloc(sizeof(int));
    if (p == NULL) {
      marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
      goto err;
    }
    *p = refi;

    /* And remember that reference */
    GENERICSTACK_PUSH_INT(marpaESLIFLuaRecognizerContextp->lexemeStackp, refi);
    if (GENERICSTACK_ERROR(marpaESLIFLuaRecognizerContextp->lexemeStackp)) {
      marpaESLIFLua_luaL_errorf(L, "lexemeStackp push failure, %s", strerror(errno));
      goto err;
    }
    break;
  default:
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lexemeAlternative(marpaESLIFRecognizerp, name, value[, grammarLength])");
    goto err;
  }

  marpaESLIFAlternative.lexemes               = (char *) names;
  marpaESLIFAlternative.value.contextp        = MARPAESLIFLUA_CONTEXT;
  marpaESLIFAlternative.value.representationp = marpaESLIFLua_representationb;
  marpaESLIFAlternative.value.type            = MARPAESLIF_VALUE_TYPE_PTR;
  marpaESLIFAlternative.value.u.p.p           = p;
  marpaESLIFAlternative.value.u.p.shallowb    = 0; /* C.f. marpaESLIF_valueFreeCallbackv */
  marpaESLIFAlternative.grammarLengthl        = grammarLengthl;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFLua_lua_pushboolean(L, marpaESLIFRecognizer_lexeme_alternativeb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &marpaESLIFAlternative))) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lexemeCompletei(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_lexemeCompletei";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  size_t                            lengthl;
  int                               isNumi;
  int                               typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lexemeComplete(marpaESLIFRecognizerp, length)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TNUMBER) {
    marpaESLIFLua_luaL_error(L, "length must be a number");
    goto err;
  }
  lengthl = (size_t) lua_tonumberx(L, 2, &isNumi);
  if (! isNumi) {
    marpaESLIFLua_luaL_error(L, "Failed to convert length to a number");
    goto err;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFLua_lua_pushboolean(L, marpaESLIFRecognizer_lexeme_completeb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, lengthl))) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lexemeReadi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs          = "marpaESLIFLua_marpaESLIFRecognizer_lexemeReadi";
  size_t                            grammarLengthl = 1;
  size_t                            lengthl;
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  const char                       *names;
  int                               isNumi;
  int                               refi;
  marpaESLIFAlternative_t           marpaESLIFAlternative;
  int                              *p;
  int                               typei;

  switch (lua_gettop(L)) {
  case 5:
  if (! marpaESLIFLua_lua_type(&typei, L, 5)) goto err;
    if (typei != LUA_TNUMBER) {
      marpaESLIFLua_luaL_error(L, "grammarLength must be a number");
      goto err;
    }
    grammarLengthl = (size_t) lua_tonumberx(L, 5, &isNumi);
    if (! isNumi) {
      marpaESLIFLua_luaL_error(L, "Failed to convert grammarLength to a number");
      goto err;
    }
    /* Intentionnaly no break */
  case 4:
    if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
    if (typei != LUA_TTABLE) {
      marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
      goto err;
    }
    if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
    marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

    if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
    if (typei != LUA_TSTRING) {
      marpaESLIFLua_luaL_error(L, "name must be a string");
      goto err;
    }
    names = lua_tostring(L, 2);

    /* We make a reference to the value and remember that in an (int *) pointer */
    /* in order to re-use the marpaESLIFLua_representationb method */
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;
    if (! marpaESLIFLua_lua_copy(L, 3, -1)) goto err;
    MARPAESLIFLUA_REF(L, refi);
    
    p = (int *) malloc(sizeof(int));
    if (p == NULL) {
      marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
      goto err;
    }
    *p = refi;

    /* And remember that reference */
    GENERICSTACK_PUSH_INT(marpaESLIFLuaRecognizerContextp->lexemeStackp, refi);
    if (GENERICSTACK_ERROR(marpaESLIFLuaRecognizerContextp->lexemeStackp)) {
      marpaESLIFLua_luaL_errorf(L, "lexemeStackp push failure, %s", strerror(errno));
      goto err;
    }

    if (! marpaESLIFLua_lua_type(&typei, L, 4)) goto err;
    if (typei != LUA_TNUMBER) {
      marpaESLIFLua_luaL_error(L, "length must be a number");
      goto err;
    }
    lengthl = (size_t) lua_tonumberx(L, 4, &isNumi);
    if (! isNumi) {
      marpaESLIFLua_luaL_error(L, "Failed to convert length to a number");
      goto err;
    }
    break;
  default:
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lexemeRead(marpaESLIFRecognizerp, name, value, length[, grammarLength])");
    goto err;
  }

  marpaESLIFAlternative.lexemes               = (char *) names;
  marpaESLIFAlternative.value.contextp        = MARPAESLIFLUA_CONTEXT;
  marpaESLIFAlternative.value.representationp = marpaESLIFLua_representationb;
  marpaESLIFAlternative.value.type            = MARPAESLIF_VALUE_TYPE_PTR;
  marpaESLIFAlternative.value.u.p.p           = p;
  marpaESLIFAlternative.value.u.p.shallowb    = 0; /* C.f. marpaESLIFLua_valueFreeCallbackv */
  marpaESLIFAlternative.grammarLengthl        = grammarLengthl;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFLua_lua_pushboolean(L, marpaESLIFRecognizer_lexeme_readb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &marpaESLIFAlternative, lengthl))) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lexemeTryi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_lexemeTryi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  const char                       *names;
  short                            rcb;
  int                              typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lexemeTry(marpaESLIFRecognizerp, name)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TSTRING) {
    marpaESLIFLua_luaL_error(L, "name must be a string");
    goto err;
  }
  names = lua_tostring(L, 2);

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_lexeme_tryb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, (char *) names, &rcb)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_lexeme_tryb failure, %s", strerror(errno));
    goto err;
  }
  
  if (! marpaESLIFLua_lua_pushboolean(L, rcb)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_discardTryi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_discardTryi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  short                             rcb;
  int                               typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_discardTry(marpaESLIFRecognizerp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_discard_tryb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &rcb)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_discard_tryb failure, %s", strerror(errno));
    goto err;
  }
  
  if (! marpaESLIFLua_lua_pushboolean(L, rcb)) goto err;

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lexemeExpectedi(lua_State *L)
/*****************************************************************************/
{
  static const char                 *funcs = "marpaESLIFLua_marpaESLIFRecognizer_lexemeExpectedi";
  marpaESLIFLuaRecognizerContext_t  *marpaESLIFLuaRecognizerContextp;
  size_t                             nLexeme;
  char                             **lexemesArrayp;
  int                                typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lexemeExpected(marpaESLIFRecognizerp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_lexeme_expectedb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &nLexeme, &lexemesArrayp)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_lexeme_expectedb failure, %s", strerror(errno));
    goto err;
  }
  
  MARPAESLIFLUA_PUSH_ASCIISTRING_ARRAY(L, nLexeme, lexemesArrayp);

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lexemeLastPausei(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_lexemeLastPausei";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  const char                       *names;
  char                             *pauses;
  size_t                            pausel;
  int                               typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lexemeLastPause(marpaESLIFRecognizerp, name)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TSTRING) {
    marpaESLIFLua_luaL_error(L, "name must be a string");
    goto err;
  }
  names = lua_tostring(L, 2);

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_lexeme_last_pauseb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, (char *) names, &pauses, &pausel)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_lexeme_last_pauseb failure, %s", strerror(errno));
    goto err;
  }
  
  if ((pauses != NULL) && (pausel > 0)) {
    if (! marpaESLIFLua_lua_pushlstring(NULL, L, (const char *) pauses, pausel)) goto err;
  } else {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lexemeLastTryi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_lexemeLastTryi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  const char                       *names;
  char                             *trys;
  size_t                            tryl;
  int                               typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lexemeLastTry(marpaESLIFRecognizerp, name)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TSTRING) {
    marpaESLIFLua_luaL_error(L, "name must be a string");
    goto err;
  }
  names = lua_tostring(L, 2);

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_lexeme_last_tryb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, (char *) names, &trys, &tryl)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_lexeme_last_tryb failure, %s", strerror(errno));
    goto err;
  }
  
  if ((trys != NULL) && (tryl > 0)) {
    if (! marpaESLIFLua_lua_pushlstring(NULL, L, (const char *) trys, tryl)) goto err;
  } else {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_discardLastTryi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_discardLastTryi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  char                             *trys;
  size_t                            tryl;
  int                               typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_discardLastTry(marpaESLIFRecognizerp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_discard_last_tryb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &trys, &tryl)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_discard_last_tryb failure, %s", strerror(errno));
    goto err;
  }
  
  if ((trys != NULL) && (tryl > 0)) {
    if (! marpaESLIFLua_lua_pushlstring(NULL, L, (const char *) trys, tryl)) goto err;
  } else {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_isEofi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_isEofi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  short                             eofb;
  int                               typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_isEof(marpaESLIFRecognizerp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_isEofb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &eofb)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_isEofb failure, %s", strerror(errno));
    goto err;
  }
  
  if (! marpaESLIFLua_lua_pushboolean(L, eofb ? 1 : 0)) goto err;
  
  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_readi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_readi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  int                               typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_read(marpaESLIFRecognizerp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFLua_lua_pushboolean(L, marpaESLIFRecognizer_readb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, NULL, NULL) ? 1 : 0)) goto err;
  
  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_inputi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_inputi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  char                             *inputs;
  size_t                            inputl;
  int                               typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_read(marpaESLIFRecognizerp)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_inputb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &inputs, &inputl)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_inputb failure, %s", strerror(errno));
    goto err;
  }

  if ((inputs != NULL) && (inputl > 0)) {
    if (! marpaESLIFLua_lua_pushlstring(NULL, L, (const char *) inputs, inputl)) goto err;
  } else {
    if (! marpaESLIFLua_lua_pushnil(L)) goto err;
  }
  
  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_progressLogi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_progressLogi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  int                               starti;
  int                               endi;
  int                               leveli;
  int                               isNumi;
  int                               typei;

  if (lua_gettop(L) != 4) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_read(marpaESLIFRecognizerp, start, end, level)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TNUMBER) {
    marpaESLIFLua_luaL_error(L, "start must be a number");
    goto err;
  }
  starti = (int) lua_tointegerx(L, 2, &isNumi);
  if (! isNumi) {
    marpaESLIFLua_luaL_error(L, "Failed to convert start to a number");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 3)) goto err;
  if (typei != LUA_TNUMBER) {
    marpaESLIFLua_luaL_error(L, "end must be a number");
    goto err;
  }
  endi = (int) lua_tointegerx(L, 2, &isNumi);
  if (! isNumi) {
    marpaESLIFLua_luaL_error(L, "Failed to convert end to a number");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 4)) goto err;
  if (typei != LUA_TNUMBER) {
    marpaESLIFLua_luaL_error(L, "level must be a number");
    goto err;
  }
  leveli = (int) lua_tointegerx(L, 4, &isNumi);
  if (! isNumi) {
    marpaESLIFLua_luaL_error(L, "Failed to convert level to a number");
    goto err;
  }

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  switch (leveli) {
  case GENERICLOGGER_LOGLEVEL_TRACE:
  case GENERICLOGGER_LOGLEVEL_DEBUG:
  case GENERICLOGGER_LOGLEVEL_INFO:
  case GENERICLOGGER_LOGLEVEL_NOTICE:
  case GENERICLOGGER_LOGLEVEL_WARNING:
  case GENERICLOGGER_LOGLEVEL_ERROR:
  case GENERICLOGGER_LOGLEVEL_CRITICAL:
  case GENERICLOGGER_LOGLEVEL_ALERT:
  case GENERICLOGGER_LOGLEVEL_EMERGENCY:
    break;
  default:
    marpaESLIFLua_luaL_errorf(L, "Unknown logger level %d", leveli);
    goto err;
    break;
  }

  if (! marpaESLIFRecognizer_progressLogb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, starti, endi, leveli)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_progressLogb failure, %s", strerror(errno));
    goto err;
  }
  
  return 0;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lastCompletedOffseti(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_lastCompletedOffseti";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  const char                       *names;
  char                             *offsetp;
  size_t                            offsetl;
  int                               rci;
  int                               typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lastCompletedOffset(marpaESLIFRecognizerp, name)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TSTRING) {
    marpaESLIFLua_luaL_error(L, "name must be a string");
    goto err;
  }
  names = lua_tostring(L, 2);

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_last_completedb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, (char *) names, &offsetp, NULL /* lengthlp */)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_last_completedb failure, %s", strerror(errno));
    goto err;
  }

  offsetl = (size_t) offsetp;

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) offsetl)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lastCompletedLengthi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_lastCompletedLenghti";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  const char                       *names;
  size_t                            lengthl;
  int                               rci;
  int                               typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lastCompletedLength(marpaESLIFRecognizerp, name)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TSTRING) {
    marpaESLIFLua_luaL_error(L, "name must be a string");
    goto err;
  }
  names = lua_tostring(L, 2);

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_last_completedb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, (char *) names, NULL /* offsetpp */, &lengthl)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_last_completedb failure, %s", strerror(errno));
    goto err;
  }
  
  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) lengthl)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_lastCompletedLocationi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_lastCompletedLocationi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  const char                       *names;
  char                             *offsetp;
  size_t                            lengthl;
  size_t                            offsetl;
  int                               rci;
  int                               typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_lastCompletedLocation(marpaESLIFRecognizerp, name)");
    goto err;
  }
  
  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TSTRING) {
    marpaESLIFLua_luaL_error(L, "name must be a string");
    goto err;
  }
  names = lua_tostring(L, 2);

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_last_completedb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, (char *) names, &offsetp, &lengthl)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_last_completedb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_newtable(L)) goto err;
  offsetl = (size_t) offsetp;
  MARPAESLIFLUA_STORE_INTEGER(L, "offset", (lua_Integer) offsetl);
  MARPAESLIFLUA_STORE_INTEGER(L, "length", (lua_Integer) lengthl);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_linei(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_linei";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  size_t                            linel;
  int                               rci;
  int                               typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_line(marpaESLIFRecognizerp)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_locationb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &linel, NULL /* columnlp */)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_locationb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) linel)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_columni(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_columni";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  size_t                            columnl;
  int                               rci;
  int                               typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_column(marpaESLIFRecognizerp)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_locationb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, NULL /* linelp */, &columnl)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_locationb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_pushinteger(L, (lua_Integer) columnl)) goto err;

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_locationi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_locationi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  size_t                            linel;
  size_t                            columnl;
  int                               rci;
  int                               typei;

  if (lua_gettop(L) != 1) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_location(marpaESLIFRecognizerp)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_locationb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &linel, &columnl)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_locationb failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_lua_createtable(L, 2, 0)) goto err;                                                 /* stack; {} */
  MARPAESLIFLUA_STORE_INTEGER(L, "line",   linel);
  MARPAESLIFLUA_STORE_INTEGER(L, "column", columnl);

  rci = 1;
  goto done;

 err:
  rci = 0;

 done:
  return rci;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFRecognizer_hookDiscardi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFRecognizer_hookDiscardi";
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  short                             discardOnOffb;
  int                               typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFRecognizer_hookDiscard(marpaESLIFRecognizerp, discardOnOff)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  if (! marpaESLIFLua_lua_type(&typei, L, 2)) goto err;
  if (typei != LUA_TBOOLEAN) {
    marpaESLIFLua_luaL_error(L, "discardOnOff must be a boolean");
    goto err;
  }
  discardOnOffb = lua_toboolean(L, 2) ? 1 : 0;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  if (! marpaESLIFRecognizer_hook_discardb(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, discardOnOffb)) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFRecognizer_hook_discardb failure, %s", strerror(errno));
    goto err;
  }

  return 0;

 err:
  return 0;
}

/*****************************************************************************/
static int marpaESLIFLua_marpaESLIFValue_newi(lua_State *L)
/*****************************************************************************/
{
  static const char                *funcs = "marpaESLIFLua_marpaESLIFValue_newi";
  marpaESLIFValueOption_t           marpaESLIFValueOption;
  marpaESLIFLuaRecognizerContext_t *marpaESLIFLuaRecognizerContextp;
  marpaESLIFLuaValueContext_t      *marpaESLIFLuaValueContextp;
  int                               typei;

  if (lua_gettop(L) != 2) {
    marpaESLIFLua_luaL_error(L, "Usage: marpaESLIFValue_new(marpaESLIFRecognizerp, valueInterface)");
    goto err;
  }

  if (! marpaESLIFLua_lua_type(&typei, L, 1)) goto err;
  if (typei != LUA_TTABLE) {
    marpaESLIFLua_luaL_error(L, "marpaESLIFRecognizerp must be a table");
    goto err;
  }
  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaRecognizerContextp")) goto err;
  marpaESLIFLuaRecognizerContextp = lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  marpaESLIFLua_paramIsValueInterfacev(L, 2);

  marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) malloc(sizeof(marpaESLIFLuaValueContext_t));
  if (marpaESLIFLuaValueContextp == NULL) {
    marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_valueContextInitb(L, 0 /* grammarStacki */, 1 /* recognizerStacki */, 2 /* valueInterfaceStacki */, marpaESLIFLuaValueContextp, 0 /* unmanagedb */, 1 /* grammarStackiCanBeZerob */)) goto err;

  marpaESLIFValueOption.userDatavp             = marpaESLIFLuaValueContextp;
  marpaESLIFValueOption.ruleActionResolverp    = marpaESLIFLua_valueRuleActionResolver;
  marpaESLIFValueOption.symbolActionResolverp  = marpaESLIFLua_valueSymbolActionResolver;
  marpaESLIFValueOption.freeActionResolverp    = marpaESLIFLua_valueFreeActionResolver;
  marpaESLIFValueOption.transformerp           = &marpaESLIFLuaValueResultTransformDefault;
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaValueContextp->valueInterface_r, "isWithHighRankOnly", 0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.highRankOnlyb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaValueContextp->valueInterface_r, "isWithOrderByRank",  0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.orderByRankb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaValueContextp->valueInterface_r, "isWithAmbiguous",    0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.ambiguousb));
  MARPAESLIFLUA_CALLBACKB(L, marpaESLIFLuaValueContextp->valueInterface_r, "isWithNull",         0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.nullb));
  MARPAESLIFLUA_CALLBACKI(L, marpaESLIFLuaValueContextp->valueInterface_r, "maxParses",          0 /* nargs */, MARPAESLIFLUA_NOOP, &(marpaESLIFValueOption.maxParsesi));

  marpaESLIFLuaValueContextp->marpaESLIFValuep = marpaESLIFValue_newp(marpaESLIFLuaRecognizerContextp->marpaESLIFRecognizerp, &marpaESLIFValueOption);
  if (marpaESLIFLuaValueContextp->marpaESLIFValuep == NULL) {
    int save_errno = errno;
    marpaESLIFLua_valueContextFreev(marpaESLIFLuaValueContextp, 0 /* onStackb */);
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFValue_newp failure, %s", strerror(save_errno));
    goto err;
  }

  marpaESLIFLuaValueContextp->managedb = 1;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  MARPAESLIFLUA_PUSH_MARPAESLIFVALUE_OBJECT(L, marpaESLIFLuaValueContextp);

  return 1;

 err:
  return 0;
}

#ifdef MARPAESLIFLUA_EMBEDDED
/****************************************************************************/
static int marpaESLIFLua_marpaESLIFValue_newFromUnmanagedi(lua_State *L, marpaESLIFValue_t *marpaESLIFValueUnmanagedp)
/****************************************************************************/
{
  static const char             *funcs = "marpaESLIFLua_marpaESLIFValue_newFromUnmanagedi";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp;

  marpaESLIFLuaValueContextp = malloc(sizeof(marpaESLIFLuaValueContext_t));
  if (marpaESLIFLuaValueContextp == NULL) {
    marpaESLIFLua_luaL_errorf(L, "malloc failure, %s", strerror(errno));
    goto err;
  }

  if (! marpaESLIFLua_valueContextInitb(L, 0 /* grammarStacki */, 0 /* recognizerStacki */, 0 /* valueInterfaceStacki */, marpaESLIFLuaValueContextp, 1 /* unmanagedb */, 1 /* grammarStackiCanBeZerob */)) goto err;
  marpaESLIFLuaValueContextp->marpaESLIFValuep = marpaESLIFValueUnmanagedp;
  marpaESLIFLuaValueContextp->managedb           = 0;

  MARPAESLIFLUA_PUSH_MARPAESLIFVALUE_OBJECT(L, marpaESLIFLuaValueContextp);

  return 1;

 err:
  return 0;
}
#endif /* MARPAESLIFLUA_EMBEDDED */

/****************************************************************************/
static int marpaESLIFLua_marpaESLIFValue_freei(lua_State *L)
/****************************************************************************/
{
  static const char           *funcs = "marpaESLIFLua_marpaESLIFValue_freei";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp;

  if (! marpaESLIFLua_lua_getfield(NULL,L, -1, "marpaESLIFLuaValueContextp")) goto err; /* stack: {...}, marpaESLIFLuaValueContextp */
  marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  marpaESLIFLua_valueContextFreev(marpaESLIFLuaValueContextp, 0 /* onStackb */);

  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  return 0;

 err:
  return 0;
}

/****************************************************************************/
static int marpaESLIFLua_marpaESLIFValue_valuei(lua_State *L)
/****************************************************************************/
{
  static const char           *funcs = "marpaESLIFLua_marpaESLIFValue_valuei";
  marpaESLIFLuaValueContext_t *marpaESLIFLuaValueContextp;
  short                        valueb;
  int                          rci;
  int                          resultStacki;

  if (! marpaESLIFLua_lua_getfield(NULL,L, 1, "marpaESLIFLuaValueContextp")) goto err; /* stack: {...}, marpaESLIFLuaValueContextp */
  marpaESLIFLuaValueContextp = (marpaESLIFLuaValueContext_t *) lua_touserdata(L, -1);
  if (! marpaESLIFLua_lua_pop(L, 1)) goto err;

  /* Clear the stack */
  if (! marpaESLIFLua_lua_settop(L, 0)) goto err;

  valueb = marpaESLIFValue_valueb(marpaESLIFLuaValueContextp->marpaESLIFValuep);
  if (valueb < 0) {
    marpaESLIFLua_luaL_errorf(L, "marpaESLIFValue_valueb failure, %s", strerror(errno));
    goto err;
  }

  /* marpaESLIFLua_stackdumpv(L, 0); */
  if (valueb > 0) {
    resultStacki = lua_gettop(L);
    /* marpaESLIFValue_valueb called the transformers that pushed the final value to the stack */
    MARPAESLIFLUA_CALLBACKV(L, marpaESLIFLuaValueContextp->valueInterface_r, "setResult", 1 /* nargs */, if (! marpaESLIFLua_lua_pushnil(L)) goto err; if (! marpaESLIFLua_lua_copy(L, resultStacki, -1)) goto err;);
    if (! marpaESLIFLua_lua_pop(L, 1)) goto err;
    rci = 1;
  } else {
    rci = 0;
  }

  if (! marpaESLIFLua_lua_pushboolean(L, rci)) goto err;

  return 1;

 err:
  return 0;
}

/****************************************************************************/
/* When MARPAESLIFLUA_EMBEDDED the file that includes this source must      */
/* provide the following implementations.                                   */
/****************************************************************************/
#ifndef MARPAESLIFLUA_EMBEDDED

/****************************************************************************/
static short marpaESLIFLua_lua_pushinteger(lua_State *L, lua_Integer n)
/****************************************************************************/
{
  lua_pushinteger(L, n); /* Native lua call */
  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_setglobal (lua_State *L, const char *name)
/****************************************************************************/
{
  lua_setglobal(L, name); /* Native lua call */
  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_getglobal (int *luaip, lua_State *L, const char *name)
/****************************************************************************/
{
  int luai;

  luai = lua_getglobal(L, name); /* Native lua call */
  if (luaip != NULL) *luaip = luai;

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_type(int *luaip, lua_State *L, int index)
/****************************************************************************/
{
  int luai;

  luai = lua_type(L, index); /* Native lua call */
  if (luaip != NULL) *luaip = luai;

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_pop(lua_State *L, int n)
/****************************************************************************/
{
  lua_pop(L, n); /* Native lua call */
  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_newtable(lua_State *L)
/****************************************************************************/
{
  lua_newtable(L); /* Native lua call */
  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_pushcfunction (lua_State *L, lua_CFunction f)
/****************************************************************************/
{
  lua_pushcfunction(L, f); /* Native lua call */
  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_setfield(lua_State *L, int index, const char *k)
/****************************************************************************/
{
  lua_setfield(L, index, k); /* Native lua call */
  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_setmetatable (lua_State *L, int index)
/****************************************************************************/
{

  lua_setmetatable(L, index); /* Native lua call */
  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_insert(lua_State *L, int index)
/****************************************************************************/
{
  lua_insert(L, index); /* Native lua call */
  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_rawgeti(int *luaip, lua_State *L, int index, lua_Integer n)
/****************************************************************************/
{
  int luai;

  luai = lua_rawgeti(L, index, n); /* Native lua call */
  if (luaip != NULL) *luaip = luai;

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_remove(lua_State *L, int index)
/****************************************************************************/
{
  lua_remove(L, index); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_createtable(lua_State *L, int narr, int nrec)
/****************************************************************************/
{
  lua_createtable(L, narr, nrec); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_rawseti(lua_State *L, int index, lua_Integer i)
/****************************************************************************/
{
  lua_rawseti(L, index, i); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_pushstring(const char **luasp, lua_State *L, const char *s)
/****************************************************************************/
{
  const char *luas;

  luas = lua_pushstring(L, s); /* Native lua call */
  if (luasp != NULL) *luasp = luas;

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_pushlstring(const char **luasp, lua_State *L, const char *s, size_t len)
/****************************************************************************/
{
  const char *luas;

  luas = lua_pushlstring(L, s, len); /* Native lua call */
  if (luasp != NULL) *luasp = luas;

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_pushnil(lua_State *L)
/****************************************************************************/
{
  lua_pushnil(L); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_getfield(int *luaip, lua_State *L, int index, const char *k)
/****************************************************************************/
{
  int luai;

  luai = lua_getfield(L, index, k); /* Native lua call */
  if (luaip != NULL) *luaip = luai;

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_call(lua_State *L, int nargs, int nresults)
/****************************************************************************/
{
  lua_call(L, nargs, nresults); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_settop(lua_State *L, int index)
/****************************************************************************/
{
  lua_settop(L, index); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_copy(lua_State *L, int fromidx, int toidx)
/****************************************************************************/
{
  lua_copy(L, fromidx, toidx); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_rawsetp(lua_State *L, int index, const void *p)
/****************************************************************************/
{
  lua_rawsetp(L, index, p); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_pushboolean(lua_State *L, int b)
/****************************************************************************/
{
  lua_pushboolean(L, b); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_pushnumber(lua_State *L, lua_Number n)
/****************************************************************************/
{
  lua_pushnumber(L, n); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_pushlightuserdata(lua_State *L, void *p)
/****************************************************************************/
{
  lua_pushlightuserdata(L, p); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_lua_pushvalue(lua_State *L, int index)
/****************************************************************************/
{
  lua_pushvalue(L, index); /* Native lua call */

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_luaL_ref(int *rcip, lua_State *L, int t)
/****************************************************************************/
{
  int rci;

  rci = luaL_ref(L, t); /* Native lua call */
  if (rcip != NULL) *rcip = rci;

  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_luaL_unref(lua_State *L, int t, int ref)
/****************************************************************************/
{
  luaL_unref(L, t, ref); /* Native lua call */
  return 1;
}

/****************************************************************************/
static short marpaESLIFLua_luaL_requiref(lua_State *L, const char *modname, lua_CFunction openf, int glb)
/****************************************************************************/
{
  luaL_requiref(L, modname, openf, glb); /* Native lua call */
  return 1;
}

#endif /* MARPAESLIFLUA_EMBEDDED */