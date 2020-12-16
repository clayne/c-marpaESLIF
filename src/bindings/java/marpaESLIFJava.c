/* Java JNI bindings for marpaESLIF */

#include <jni.h>
#include <limits.h>
/* Should never happen */
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include <marpaESLIF.h>
#include <genericStack.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <float.h>
#include <genericLogger.h>

/* ---------------- */
/* Exported methods */
/* ---------------- */
JNIEXPORT jint         JNICALL JNI_OnLoad                                                      (JavaVM *vmp, void* reservedp);
JNIEXPORT void         JNICALL JNI_OnUnLoad                                                    (JavaVM *vmp, void* reservedp);

JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIF_jniNew                              (JNIEnv *envp, jobject eslifp, jint indice);
JNIEXPORT jstring      JNICALL Java_org_parser_marpa_ESLIF_jniJavaVersion                      (JNIEnv *envp, jclass classp);
JNIEXPORT jint         JNICALL Java_org_parser_marpa_ESLIF_jniJavaVersionMajor                 (JNIEnv *envp, jclass classp);
JNIEXPORT jint         JNICALL Java_org_parser_marpa_ESLIF_jniJavaVersionMinor                 (JNIEnv *envp, jclass classp);
JNIEXPORT jint         JNICALL Java_org_parser_marpa_ESLIF_jniJavaVersionPatch                 (JNIEnv *envp, jclass classp);
JNIEXPORT jstring      JNICALL Java_org_parser_marpa_ESLIF_jniVersion                          (JNIEnv *envp, jobject eslifp);
JNIEXPORT jint         JNICALL Java_org_parser_marpa_ESLIF_jniVersionMajor                     (JNIEnv *envp, jobject eslifp);
JNIEXPORT jint         JNICALL Java_org_parser_marpa_ESLIF_jniVersionMinor                     (JNIEnv *envp, jobject eslifp);
JNIEXPORT jint         JNICALL Java_org_parser_marpa_ESLIF_jniVersionPatch                     (JNIEnv *envp, jobject eslifp);
/* JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIF_jniFree                             (JNIEnv *envp, jobject eslifp); */
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFGrammar_jniNew                       (JNIEnv *envp, jobject eslifGrammarp, jbyteArray utf8byteArrayp);
JNIEXPORT jint         JNICALL Java_org_parser_marpa_ESLIFGrammar_jniNgrammar                  (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jint         JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentLevel              (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jstring      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentDescription        (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jstring      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniDescriptionByLevel        (JNIEnv *envp, jobject eslifGrammarp, jint level);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentRuleIds            (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentSymbolIds          (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleIdsByLevel            (JNIEnv *envp, jobject eslifGrammarp, jint level);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolIdsByLevel          (JNIEnv *envp, jobject eslifGrammarp, jint level);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplay               (JNIEnv *envp, jobject eslifGrammarp, jint rule);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolDisplay             (JNIEnv *envp, jobject eslifGrammarp, jint symbol);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleShow                  (JNIEnv *envp, jobject eslifGrammarp, jint rule);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplayByLevel        (JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolDisplayByLevel      (JNIEnv *envp, jobject eslifGrammarp, jint level, jint symbol);
JNIEXPORT jintArray    JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleShowByLevel           (JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule);
JNIEXPORT jstring      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniShow                      (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jstring      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniShowByLevel               (JNIEnv *envp, jobject eslifGrammarp, jint level);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFGrammar_jniParse                     (JNIEnv *envp, jobject eslifGrammarp, jobject eslifRecognizerInterfacep, jobject eslifValueInterfacep);
JNIEXPORT jobject      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniProperties                (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jobject      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniPropertiesByLevel         (JNIEnv *envp, jobject eslifGrammarp, jint level);
JNIEXPORT jobject      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleProperties            (JNIEnv *envp, jobject eslifGrammarp, jint rule);
JNIEXPORT jobject      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRulePropertiesByLevel     (JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule);
JNIEXPORT jobject      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolProperties          (JNIEnv *envp, jobject eslifGrammarp, jint symbol);
JNIEXPORT jobject      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolPropertiesByLevel   (JNIEnv *envp, jobject eslifGrammarp, jint level, jint symbol);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFGrammar_jniFree                      (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniNew                    (JNIEnv *envp, jobject eslifRecognizerp, jobject eslifGrammarp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniShare                  (JNIEnv *envp, jobject eslifRecognizerp, jobject eslifRecognizerSharedp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniUnshare                (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniScan                   (JNIEnv *envp, jobject eslifRecognizerp, jboolean initialEvents);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniResume                 (JNIEnv *envp, jobject eslifRecognizerp, jint deltaLength);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeAlternative      (JNIEnv *envp, jobject eslifRecognizerp, jstring namep, jobject objectp, jint grammarLengthi);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeComplete         (JNIEnv *envp, jobject eslifRecognizerp, jint lengthi);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeRead             (JNIEnv *envp, jobject eslifRecognizerp, jstring namep, jobject objectp, jint lengthi, jint grammarLengthi);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeTry              (JNIEnv *envp, jobject eslifRecognizerp, jstring namep);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniDiscardTry             (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jobjectArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeExpected         (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniIsEof                  (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniIsExhausted            (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniSetExhaustedFlag       (JNIEnv *envp, jobject eslifRecognizerp, jboolean flag);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniIsCanContinue          (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniRead                   (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jbyteArray   JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniInput                  (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jbyteArray   JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeLastPause        (JNIEnv *envp, jobject eslifRecognizerp, jstring lexemep);
JNIEXPORT jbyteArray   JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeLastTry          (JNIEnv *envp, jobject eslifRecognizerp, jstring lexemep);
JNIEXPORT jbyteArray   JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniDiscardLastTry         (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jbyteArray   JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniDiscardLast            (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jobjectArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniEvent                  (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jlong        JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLastCompletedOffset    (JNIEnv *envp, jobject eslifRecognizerp, jstring namep);
JNIEXPORT jlong        JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLastCompletedLength    (JNIEnv *envp, jobject eslifRecognizerp, jstring namep);
JNIEXPORT jlong        JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLine                   (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jlong        JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniColumn                 (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniEventOnOff             (JNIEnv *envp, jobject eslifRecognizerp, jstring symbolp, jobjectArray eventTypesp, jboolean onOff);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniHookDiscard            (JNIEnv *envp, jobject eslifRecognizerp, jboolean onOff);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniHookDiscardSwitch      (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniProgressLog            (JNIEnv *envp, jobject eslifRecognizerp, int starti, int endi, jobject levelp);
JNIEXPORT jbyteArray   JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniSymbolTry              (JNIEnv *envp, jobject eslifRecognizerp, jobject eslifSymbolp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniFree                   (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFValue_jniNew                         (JNIEnv *envp, jobject eslifValuep, jobject eslifRecognizerp);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFValue_jniValue                       (JNIEnv *envp, jobject eslifValuep);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFValue_jniFree                        (JNIEnv *envp, jobject eslifValuep);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFJSONEncoder_jniNew                   (JNIEnv *envp, jobject eslifJSONEncoderp, jboolean strict);
JNIEXPORT jstring      JNICALL Java_org_parser_marpa_ESLIFJSONEncoder_jniEncode                (JNIEnv *envp, jobject eslifJSONEncoderp, jobject o);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFJSONEncoder_jniFree                  (JNIEnv *envp, jobject eslifJSONEncoderp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFJSONDecoder_jniNew                   (JNIEnv *envp, jobject eslifJSONDecoderp, jboolean strict);
JNIEXPORT jobject      JNICALL Java_org_parser_marpa_ESLIFJSONDecoder_jniDecode                (JNIEnv *envp, jobject eslifJSONDecoderp, jobject eslifRecognizerInterfacep, jobject eslifDecodeOptionp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFJSONDecoder_jniFree                  (JNIEnv *envp, jobject eslifJSONDecoderp);

JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFSymbol_jniNew                        (JNIEnv *envp, jobject eslifSymbolp, jstring type, jbyteArray utf8byteArrayp, jstring modifiers);
JNIEXPORT jbyteArray   JNICALL Java_org_parser_marpa_ESLIFSymbol_jniTest                       (JNIEnv *envp, jobject eslifSymbolp, jbyteArray byteArrayp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFSymbol_jniFree                       (JNIEnv *envp, jobject eslifSymbolp);

/* ---------- */
/* Structures */
/* ---------- */
typedef struct genericLoggerContext {
  jint      indice;
} genericLoggerContext_t;

typedef struct marpaESLIFClassCache {
  char     *classs;
  jclass    classp;
  short     requiredb;
} marpaESLIFClassCache_t;

typedef struct marpaESLIFMethodCache {
  marpaESLIFClassCache_t *marpaESLIFClassCachep;
  char                   *methods;
  char                   *signatures;
  short                   staticb;
  jmethodID               methodp;
  short                   requiredb;
} marpaESLIFMethodCache_t;

typedef struct marpaESLIFFieldCache {
  marpaESLIFClassCache_t *marpaESLIFClassCachep;
  char                   *fields;
  char                   *signatures;
  short                   staticb;
  jfieldID                fieldp;
  short                   requiredb;
} marpaESLIFFieldCache_t;

typedef struct marpaESLIFRecognizerContext {
  jobject                  eslifRecognizerInterfacep;  /* Current recognizer interface instance - this can change at every call */
  jbyteArray               byteArrayp;
  jstring                  encodingp;
  genericStack_t           _lexemeStackp;
  genericStack_t          *lexemeStackp;
  JNIEnv                  *envp;
  marpaESLIFClassCache_t   classCache;                    /* Cache of current class */
  marpaESLIFMethodCache_t *methodCachep;                  /* Cache of method IDs */
  size_t                   methodCacheSizel;
  char                    *actions;                       /* shallow copy of last resolved name */
  jmethodID                methodp;                       /* Current resolved method ID */
  genericStack_t           _objectStack;
  genericStack_t          *objectStackp;
} marpaESLIFRecognizerContext_t;

typedef struct marpaESLIFValueContext {
  jobject                        eslifValueInterfacep;          /* Current eslifValueInterface instance */
  jobject                        eslifGrammarp;                 /* Current eslifGrammarp instance */
  marpaESLIFClassCache_t         classCache;                    /* Cache of current class */
  marpaESLIFMethodCache_t       *methodCachep;                  /* Cache of method IDs */
  size_t                         methodCacheSizel;
  jmethodID                      methodp;                       /* Current resolved method ID */
  char                          *actions;                       /* shallow copy of last resolved name */
  JNIEnv                        *envp;
  genericStack_t                 _objectStack;
  genericStack_t                *objectStackp;
} marpaESLIFValueContext_t;

typedef struct marpaESLIFJava_stringGeneratorContext { /* We use genericLogger to generate strings */
  char        *s;
  size_t       l;
  short        okb;
} marpaESLIFJava_stringGeneratorContext_t;

/* ------ */
/* Macros */
/* ------ */
static char _MARPAESLIF_JNI_CONTEXT;
#define MARPAESLIF_JNI_CONTEXT                    &_MARPAESLIF_JNI_CONTEXT
#define MARPAESLIF_JNI_VERSION                    JNI_VERSION_1_4 /* Because of NIO Support */
#define MARPAESLIF_FILENAMES                      "marpaESLIFJava.c"
#define MARPAESLIF_ESLIFEXCEPTION_CLASS           "org/parser/marpa/ESLIFException"
#define MARPAESLIF_ESLIF_CLASS                        "org/parser/marpa/ESLIF"
#define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS         "org/parser/marpa/ESLIFLoggerInterface"
#define MARPAESLIF_ESLIFGRAMMAR_CLASS                 "org/parser/marpa/ESLIFGrammar"
#define MARPAESLIF_ESLIFRECOGNIZER_CLASS              "org/parser/marpa/ESLIFRecognizer"
#define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS     "org/parser/marpa/ESLIFRecognizerInterface"
#define MARPAESLIF_ESLIFVALUE_CLASS                   "org/parser/marpa/ESLIFValue"
#define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS          "org/parser/marpa/ESLIFValueInterface"
#define MARPAESLIF_ESLIFEVENTTYPE_CLASS               "org/parser/marpa/ESLIFEventType"
#define MARPAESLIF_ESLIFEVENT_CLASS                   "org/parser/marpa/ESLIFEvent"
#define MARPAESLIF_ESLIFLOGGERLEVEL_CLASS             "org/parser/marpa/ESLIFLoggerLevel"
#define MARPAESLIF_ESLIFGRAMMARPROPERTIES_CLASS       "org/parser/marpa/ESLIFGrammarProperties"
#define MARPAESLIF_ESLIFGRAMMARRULEPROPERTIES_CLASS   "org/parser/marpa/ESLIFGrammarRuleProperties"
#define MARPAESLIF_ESLIFGRAMMARSYMBOLPROPERTIES_CLASS "org/parser/marpa/ESLIFGrammarSymbolProperties"
#define MARPAESLIF_ESLIFRULEPROPERTYBITSET_CLASS      "org/parser/marpa/ESLIFRulePropertyBitSet"
#define MARPAESLIF_ESLIFSYMBOLPROPERTYBITSET_CLASS    "org/parser/marpa/ESLIFSymbolPropertyBitSet"
#define MARPAESLIF_ESLIFSYMBOLTYPE_CLASS              "org/parser/marpa/ESLIFSymbolType"
#define JAVA_LANG_OBJECT_CLASS                        "java/lang/Object"
#define JAVA_LANG_CLASS_CLASS                         "java/lang/Class"
#define JAVA_LANG_STRING_CLASS                        "java/lang/String"
#define JAVA_LANG_CHARACTER_CLASS                     "java/lang/Character"
#define JAVA_LANG_SHORT_CLASS                         "java/lang/Short"
#define JAVA_LANG_INTEGER_CLASS                       "java/lang/Integer"
#define JAVA_LANG_LONG_CLASS                          "java/lang/Long"
#define JAVA_LANG_FLOAT_CLASS                         "java/lang/Float"
#define JAVA_LANG_DOUBLE_CLASS                        "java/lang/Double"
#define JAVA_LANG_BOOLEAN_CLASS                       "java/lang/Boolean"
#define JAVA_UTIL_HASHMAP_CLASS                       "java/util/HashMap"
#define JAVA_MATH_BIGDECIMAL_CLASS                    "java/math/BigDecimal"
#define JAVA_MATH_BIGINTEGER_CLASS                    "java/math/BigInteger"
#define JAVA_LANG_BYTE_CLASS                          "java/lang/Byte"
#define JAVA_LANG_REFLECT_ARRAY_CLASS                 "java/lang/reflect/Array"
#define JAVA_UTIL_ITERATOR_CLASS                      "java/util/Iterator"
#define JAVA_UTIL_SET_CLASS                           "java/util/Set"
#define JAVA_UTIL_MAP_CLASS                           "java/util/Map"
#define MARPAESLIF_ESLIFSYMBOLEVENTBITSET_CLASS       "org/parser/marpa/ESLIFSymbolEventBitSet"
#define MARPAESLIF_ESLIFJSONENCODER_CLASS             "org/parser/marpa/ESLIFJSONEncoder"
#define MARPAESLIF_ESLIFJSONDECODER_CLASS             "org/parser/marpa/ESLIFJSONDecoder"
#define MARPAESLIF_ESLIFJSONDECODEROPTION_CLASS       "org/parser/marpa/ESLIFJSONDecoderOption"
#define MARPAESLIF_ESLIFREGEXCALLOUT_CLASS            "org/parser/marpa/ESLIFRegexCallout"
#define MARPAESLIF_ESLIFSYMBOL_CLASS                  "org/parser/marpa/ESLIFSymbol"

#define MARPAESLIF_ESLIFVALUEINTERFACE_SYMBOLACTION_SIGNATURE "(Ljava/lang/Object;)Ljava/lang/Object;"
#define MARPAESLIF_ESLIFVALUEINTERFACE_RULEACTION_SIGNATURE   "([Ljava/lang/Object;)Ljava/lang/Object;"
#define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_IFACTION_SIGNATURE "([B)Z"
#define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_EVENTACTION_SIGNATURE "([Lorg/parser/marpa/ESLIFEvent;)Z"
#define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_REGEXACTION_SIGNATURE "(Lorg/parser/marpa/ESLIFRegexCallout;)I"

#ifdef HAVE_INTPTR_T
#define PTR_TO_JLONG(p) ((jlong) ((intptr_t) (p)))
#else
/* Likely to raise a warning */
#define PTR_TO_JLONG(p) ((jlong) (p))
#endif

/* -------------------------------- */
/* Globals and accessors as macros */
/* -------------------------------- */
static JavaVM  *marpaESLIF_vmp;
static const char *marpaESLIFJava_UTF8s = "UTF-8";
static jstring marpaESLIF_UTF8p;

static marpaESLIFClassCache_t marpaESLIFClassCacheArrayp[] = {
  #define MARPAESLIF_ESLIFEXCEPTION_CLASSCACHE           marpaESLIFClassCacheArrayp[0]
  #define MARPAESLIF_ESLIFEXCEPTION_CLASSP               marpaESLIFClassCacheArrayp[0].classp
  {       MARPAESLIF_ESLIFEXCEPTION_CLASS,               NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIF_CLASSCACHE                    marpaESLIFClassCacheArrayp[1]
  #define MARPAESLIF_ESLIF_CLASSP                        marpaESLIFClassCacheArrayp[1].classp
  {       MARPAESLIF_ESLIF_CLASS,                        NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE     marpaESLIFClassCacheArrayp[2]
  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSP         marpaESLIFClassCacheArrayp[2].classp
  {       MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS,         NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE             marpaESLIFClassCacheArrayp[3]
  #define MARPAESLIF_ESLIFGRAMMAR_CLASSP                 marpaESLIFClassCacheArrayp[3].classp
  {       MARPAESLIF_ESLIFGRAMMAR_CLASS,                 NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE          marpaESLIFClassCacheArrayp[4]
  #define MARPAESLIF_ESLIFRECOGNIZER_CLASSP              marpaESLIFClassCacheArrayp[4].classp
  {       MARPAESLIF_ESLIFRECOGNIZER_CLASS,              NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE marpaESLIFClassCacheArrayp[5]
  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSP     marpaESLIFClassCacheArrayp[5].classp
  {       MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS,     NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUE_CLASSCACHE               marpaESLIFClassCacheArrayp[6]
  #define MARPAESLIF_ESLIFVALUE_CLASSP                   marpaESLIFClassCacheArrayp[6].classp
  {       MARPAESLIF_ESLIFVALUE_CLASS,                   NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE      marpaESLIFClassCacheArrayp[7]
  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASSP          marpaESLIFClassCacheArrayp[7].classp
  {       MARPAESLIF_ESLIFVALUEINTERFACE_CLASS,          NULL, 1 /* requiredb */ },

  #define JAVA_LANG_OBJECT_CLASSCACHE                    marpaESLIFClassCacheArrayp[8]
  #define JAVA_LANG_OBJECT_CLASSP                        marpaESLIFClassCacheArrayp[8].classp
  {       JAVA_LANG_OBJECT_CLASS,                        NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CLASS_CLASSCACHE                     marpaESLIFClassCacheArrayp[9]
  #define JAVA_LANG_CLASS_CLASSP                         marpaESLIFClassCacheArrayp[9].classp
  {       JAVA_LANG_CLASS_CLASS,                         NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFEVENTTYPE_CLASSCACHE           marpaESLIFClassCacheArrayp[10]
  #define MARPAESLIF_ESLIFEVENTTYPE_CLASSP               marpaESLIFClassCacheArrayp[10].classp
  {       MARPAESLIF_ESLIFEVENTTYPE_CLASS,               NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFEVENT_CLASSCACHE               marpaESLIFClassCacheArrayp[11]
  #define MARPAESLIF_ESLIFEVENT_CLASSP                   marpaESLIFClassCacheArrayp[11].classp
  {       MARPAESLIF_ESLIFEVENT_CLASS,                   NULL, 1 /* requiredb */ },

  #define JAVA_LANG_STRING_CLASSCACHE                    marpaESLIFClassCacheArrayp[12]
  #define JAVA_LANG_STRING_CLASSP                        marpaESLIFClassCacheArrayp[12].classp
  {       JAVA_LANG_STRING_CLASS,                        NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERLEVEL_CLASSCACHE         marpaESLIFClassCacheArrayp[13]
  #define MARPAESLIF_ESLIFLOGGERLEVEL_CLASSP             marpaESLIFClassCacheArrayp[13].classp
  {       MARPAESLIF_ESLIFLOGGERLEVEL_CLASS,             NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMARPROPERTIES_CLASSCACHE   marpaESLIFClassCacheArrayp[14]
  #define MARPAESLIF_ESLIFGRAMMARPROPERTIES_CLASSP       marpaESLIFClassCacheArrayp[14].classp
  {       MARPAESLIF_ESLIFGRAMMARPROPERTIES_CLASS,       NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMARRULEPROPERTIES_CLASSCACHE marpaESLIFClassCacheArrayp[15]
  #define MARPAESLIF_ESLIFGRAMMARRULEPROPERTIES_CLASSP     marpaESLIFClassCacheArrayp[15].classp
  {       MARPAESLIF_ESLIFGRAMMARRULEPROPERTIES_CLASS,     NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMARSYMBOLPROPERTIES_CLASSCACHE marpaESLIFClassCacheArrayp[16]
  #define MARPAESLIF_ESLIFGRAMMARSYMBOLPROPERTIES_CLASSP     marpaESLIFClassCacheArrayp[16].classp
  {       MARPAESLIF_ESLIFGRAMMARSYMBOLPROPERTIES_CLASS,     NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRULEPROPERTYBITSET_CLASSCACHE  marpaESLIFClassCacheArrayp[17]
  #define MARPAESLIF_ESLIFRULEPROPERTYBITSET_CLASSP      marpaESLIFClassCacheArrayp[17].classp
  {       MARPAESLIF_ESLIFRULEPROPERTYBITSET_CLASS,      NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFSYMBOLPROPERTYBITSET_CLASSCACHE marpaESLIFClassCacheArrayp[18]
  #define MARPAESLIF_ESLIFSYMBOLPROPERTYBITSET_CLASSP     marpaESLIFClassCacheArrayp[18].classp
  {       MARPAESLIF_ESLIFSYMBOLPROPERTYBITSET_CLASS,     NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFSYMBOLTYPE_CLASSCACHE          marpaESLIFClassCacheArrayp[19]
  #define MARPAESLIF_ESLIFSYMBOLTYPE_CLASSP              marpaESLIFClassCacheArrayp[19].classp
  {       MARPAESLIF_ESLIFSYMBOLTYPE_CLASS,              NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CHARACTER_CLASSCACHE                 marpaESLIFClassCacheArrayp[20]
  #define JAVA_LANG_CHARACTER_CLASSP                     marpaESLIFClassCacheArrayp[20].classp
  {       JAVA_LANG_CHARACTER_CLASS,                     NULL, 1 /* requiredb */ },

  #define JAVA_LANG_SHORT_CLASSCACHE                     marpaESLIFClassCacheArrayp[21]
  #define JAVA_LANG_SHORT_CLASSP                         marpaESLIFClassCacheArrayp[21].classp
  {       JAVA_LANG_SHORT_CLASS,                         NULL, 1 /* requiredb */ },

  #define JAVA_LANG_INTEGER_CLASSCACHE                   marpaESLIFClassCacheArrayp[22]
  #define JAVA_LANG_INTEGER_CLASSP                       marpaESLIFClassCacheArrayp[22].classp
  {       JAVA_LANG_INTEGER_CLASS,                       NULL, 1 /* requiredb */ },

  #define JAVA_LANG_LONG_CLASSCACHE                      marpaESLIFClassCacheArrayp[23]
  #define JAVA_LANG_LONG_CLASSP                          marpaESLIFClassCacheArrayp[23].classp
  {       JAVA_LANG_LONG_CLASS,                          NULL, 1 /* requiredb */ },

  #define JAVA_LANG_FLOAT_CLASSCACHE                     marpaESLIFClassCacheArrayp[24]
  #define JAVA_LANG_FLOAT_CLASSP                         marpaESLIFClassCacheArrayp[24].classp
  {       JAVA_LANG_FLOAT_CLASS,                         NULL, 1 /* requiredb */ },

  #define JAVA_LANG_DOUBLE_CLASSCACHE                    marpaESLIFClassCacheArrayp[25]
  #define JAVA_LANG_DOUBLE_CLASSP                        marpaESLIFClassCacheArrayp[25].classp
  {       JAVA_LANG_DOUBLE_CLASS,                        NULL, 1 /* requiredb */ },

  #define JAVA_LANG_BOOLEAN_CLASSCACHE                   marpaESLIFClassCacheArrayp[26]
  #define JAVA_LANG_BOOLEAN_CLASSP                       marpaESLIFClassCacheArrayp[26].classp
  {       JAVA_LANG_BOOLEAN_CLASS,                       NULL, 1 /* requiredb */ },

  #define JAVA_UTIL_HASHMAP_CLASSCACHE                   marpaESLIFClassCacheArrayp[27]
  #define JAVA_UTIL_HASHMAP_CLASSP                       marpaESLIFClassCacheArrayp[27].classp
  {       JAVA_UTIL_HASHMAP_CLASS,                       NULL, 1 /* requiredb */ },

  #define JAVA_MATH_BIGDECIMAL_CLASSCACHE                marpaESLIFClassCacheArrayp[28]
  #define JAVA_MATH_BIGDECIMAL_CLASSP                    marpaESLIFClassCacheArrayp[28].classp
  {       JAVA_MATH_BIGDECIMAL_CLASS,                    NULL, 1 /* requiredb */ },

  #define JAVA_MATH_BIGINTEGER_CLASSCACHE                marpaESLIFClassCacheArrayp[29]
  #define JAVA_MATH_BIGINTEGER_CLASSP                    marpaESLIFClassCacheArrayp[29].classp
  {       JAVA_MATH_BIGINTEGER_CLASS,                    NULL, 1 /* requiredb */ },

  #define JAVA_LANG_BYTE_CLASSCACHE                      marpaESLIFClassCacheArrayp[30]
  #define JAVA_LANG_BYTE_CLASSP                          marpaESLIFClassCacheArrayp[30].classp
  {       JAVA_LANG_BYTE_CLASS,                          NULL, 1 /* requiredb */ },

  #define JAVA_LANG_REFLECT_ARRAY_CLASSCACHE             marpaESLIFClassCacheArrayp[31]
  #define JAVA_LANG_REFLECT_ARRAY_CLASSP                 marpaESLIFClassCacheArrayp[31].classp
  {       JAVA_LANG_REFLECT_ARRAY_CLASS,                 NULL, 1 /* requiredb */ },

  #define JAVA_UTIL_ITERATOR_CLASSCACHE                  marpaESLIFClassCacheArrayp[32]
  #define JAVA_UTIL_ITERATOR_CLASSP                      marpaESLIFClassCacheArrayp[32].classp
  {       JAVA_UTIL_ITERATOR_CLASS,                      NULL, 1 /* requiredb */ },

  #define JAVA_UTIL_SET_CLASSCACHE                       marpaESLIFClassCacheArrayp[33]
  #define JAVA_UTIL_SET_CLASSP                           marpaESLIFClassCacheArrayp[33].classp
  {       JAVA_UTIL_SET_CLASS,                           NULL, 1 /* requiredb */ },

  #define JAVA_UTIL_MAP_CLASSCACHE                       marpaESLIFClassCacheArrayp[34]
  #define JAVA_UTIL_MAP_CLASSP                           marpaESLIFClassCacheArrayp[34].classp
  {       JAVA_UTIL_MAP_CLASS,                           NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFSYMBOLEVENTBITSET_CLASSCACHE   marpaESLIFClassCacheArrayp[35]
  #define MARPAESLIF_ESLIFSYMBOLEVENTBITSET_CLASSP       marpaESLIFClassCacheArrayp[35].classp
  {       MARPAESLIF_ESLIFSYMBOLEVENTBITSET_CLASS,       NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFJSONENCODER_CLASSCACHE         marpaESLIFClassCacheArrayp[36]
  #define MARPAESLIF_ESLIFJSONENCODER_CLASSP             marpaESLIFClassCacheArrayp[36].classp
  {       MARPAESLIF_ESLIFJSONENCODER_CLASS,             NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFJSONDECODER_CLASSCACHE         marpaESLIFClassCacheArrayp[37]
  #define MARPAESLIF_ESLIFJSONDECODER_CLASSP             marpaESLIFClassCacheArrayp[37].classp
  {       MARPAESLIF_ESLIFJSONDECODER_CLASS,             NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFJSONDECODEROPTION_CLASSCACHE   marpaESLIFClassCacheArrayp[38]
  #define MARPAESLIF_ESLIFJSONDECODEROPTION_CLASSP       marpaESLIFClassCacheArrayp[38].classp
  {       MARPAESLIF_ESLIFJSONDECODEROPTION_CLASS,       NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFREGEXCALLOUT_CLASSCACHE        marpaESLIFClassCacheArrayp[39]
  #define MARPAESLIF_ESLIFREGEXCALLOUT_CLASSP            marpaESLIFClassCacheArrayp[39].classp
  {       MARPAESLIF_ESLIFREGEXCALLOUT_CLASS,            NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFSYMBOL_CLASSCACHE              marpaESLIFClassCacheArrayp[40]
  #define MARPAESLIF_ESLIFSYMBOL_CLASSP                  marpaESLIFClassCacheArrayp[40].classp
  {       MARPAESLIF_ESLIFSYMBOL_CLASS,                  NULL, 1 /* requiredb */ },

  { NULL }
};

static marpaESLIFMethodCache_t marpaESLIFMethodCacheArrayp[] = {
  #define MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP                        marpaESLIFMethodCacheArrayp[0].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "getLoggerInterface",                 "()Lorg/parser/marpa/ESLIFLoggerInterface;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIF_CLASS_getMarpaESLIFp_METHODP                             marpaESLIFMethodCacheArrayp[1].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "getMarpaESLIFp",                      "()Ljava/nio/ByteBuffer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIF_CLASS_setMarpaESLIFp_METHODP                             marpaESLIFMethodCacheArrayp[2].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "setMarpaESLIFp",                      "(Ljava/nio/ByteBuffer;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIF_CLASS_getGenericLoggerContextp_METHODP                   marpaESLIFMethodCacheArrayp[3].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "getGenericLoggerContextp",            "()Ljava/nio/ByteBuffer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIF_CLASS_setGenericLoggerContextp_METHODP                   marpaESLIFMethodCacheArrayp[4].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "setGenericLoggerContextp",            "(Ljava/nio/ByteBuffer;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIF_CLASS_getGenericLoggerp_METHODP                          marpaESLIFMethodCacheArrayp[5].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "getGenericLoggerp",                   "()Ljava/nio/ByteBuffer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIF_CLASS_setGenericLoggerp_METHODP                          marpaESLIFMethodCacheArrayp[6].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "setGenericLoggerp",                   "(Ljava/nio/ByteBuffer;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_trace_METHODP                       marpaESLIFMethodCacheArrayp[7].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "trace",                "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_debug_METHODP                       marpaESLIFMethodCacheArrayp[8].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "debug",                "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_info_METHODP                        marpaESLIFMethodCacheArrayp[9].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "info",                 "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_notice_METHODP                      marpaESLIFMethodCacheArrayp[10].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "notice",               "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_warning_METHODP                     marpaESLIFMethodCacheArrayp[11].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "warning",              "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_error_METHODP                       marpaESLIFMethodCacheArrayp[12].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "error",                "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_critical_METHODP                    marpaESLIFMethodCacheArrayp[13].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "critical",             "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_alert_METHODP                       marpaESLIFMethodCacheArrayp[14].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "alert",                "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_emergency_METHODP                   marpaESLIFMethodCacheArrayp[15].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "emergency",            "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP                 marpaESLIFMethodCacheArrayp[16].methodp
  {      &MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE, "getLoggerInterface",                 "()Lorg/parser/marpa/ESLIFLoggerInterface;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASS_getEslif_METHODP                            marpaESLIFMethodCacheArrayp[17].methodp
  {      &MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE, "getEslif",                           "()Lorg/parser/marpa/ESLIF;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASS_getMarpaESLIFGrammarp_METHODP               marpaESLIFMethodCacheArrayp[18].methodp
  {      &MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE, "getMarpaESLIFGrammarp",              "()Ljava/nio/ByteBuffer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASS_setMarpaESLIFGrammarp_METHODP               marpaESLIFMethodCacheArrayp[19].methodp
  {      &MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE, "setMarpaESLIFGrammarp",              "(Ljava/nio/ByteBuffer;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP              marpaESLIFMethodCacheArrayp[20].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getLoggerInterface",              "()Lorg/parser/marpa/ESLIFLoggerInterface;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifGrammar_METHODP                  marpaESLIFMethodCacheArrayp[21].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getEslifGrammar",                 "()Lorg/parser/marpa/ESLIFGrammar;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getMarpaESLIFRecognizerp_METHODP         marpaESLIFMethodCacheArrayp[22].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getMarpaESLIFRecognizerp",        "()Ljava/nio/ByteBuffer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_setMarpaESLIFRecognizerp_METHODP         marpaESLIFMethodCacheArrayp[23].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "setMarpaESLIFRecognizerp",        "(Ljava/nio/ByteBuffer;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifRecognizerInterface_METHODP      marpaESLIFMethodCacheArrayp[24].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getEslifRecognizerInterface",     "()Lorg/parser/marpa/ESLIFRecognizerInterface;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getMarpaESLIFRecognizerContextp_METHODP  marpaESLIFMethodCacheArrayp[25].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getMarpaESLIFRecognizerContextp", "()Ljava/nio/ByteBuffer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_setMarpaESLIFRecognizerContextp_METHODP  marpaESLIFMethodCacheArrayp[26].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "setMarpaESLIFRecognizerContextp", "(Ljava/nio/ByteBuffer;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_read_METHODP                    marpaESLIFMethodCacheArrayp[27].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "read",                   "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isEof_METHODP                   marpaESLIFMethodCacheArrayp[28].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isEof",                  "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isCharacterStream_METHODP       marpaESLIFMethodCacheArrayp[29].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isCharacterStream",      "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_encoding_METHODP                marpaESLIFMethodCacheArrayp[30].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "encoding",               "()Ljava/lang/String;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_data_METHODP                    marpaESLIFMethodCacheArrayp[31].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "data",                   "()[B", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithDisableThreshold_METHODP  marpaESLIFMethodCacheArrayp[32].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isWithDisableThreshold", "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithExhaustion_METHODP        marpaESLIFMethodCacheArrayp[33].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isWithExhaustion",       "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithNewline_METHODP           marpaESLIFMethodCacheArrayp[34].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isWithNewline",          "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithTrack_METHODP             marpaESLIFMethodCacheArrayp[35].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isWithTrack",            "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getLoggerInterfacep_METHODP                   marpaESLIFMethodCacheArrayp[36].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getLoggerInterface",                   "()Lorg/parser/marpa/ESLIFLoggerInterface;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getEslifRecognizer_METHODP                    marpaESLIFMethodCacheArrayp[37].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getEslifRecognizer",                   "()Lorg/parser/marpa/ESLIFRecognizer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getMarpaESLIFValuep_METHODP                   marpaESLIFMethodCacheArrayp[38].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getMarpaESLIFValuep",                  "()Ljava/nio/ByteBuffer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUE_CLASS_setMarpaESLIFValuep_METHODP                   marpaESLIFMethodCacheArrayp[39].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "setMarpaESLIFValuep",                  "(Ljava/nio/ByteBuffer;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getEslifValueInterface_METHODP                marpaESLIFMethodCacheArrayp[40].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getEslifValueInterface",               "()Lorg/parser/marpa/ESLIFValueInterface;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getMarpaESLIFValueContextp_METHODP            marpaESLIFMethodCacheArrayp[41].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getMarpaESLIFValueContextp",           "()Ljava/nio/ByteBuffer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUE_CLASS_setMarpaESLIFValueContextp_METHODP            marpaESLIFMethodCacheArrayp[42].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "setMarpaESLIFValueContextp",           "(Ljava/nio/ByteBuffer;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithHighRankOnly_METHODP           marpaESLIFMethodCacheArrayp[43].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "isWithHighRankOnly",          "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithOrderByRank_METHODP            marpaESLIFMethodCacheArrayp[44].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "isWithOrderByRank",           "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithAmbiguous_METHODP              marpaESLIFMethodCacheArrayp[45].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "isWithAmbiguous",             "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithNull_METHODP                   marpaESLIFMethodCacheArrayp[46].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "isWithNull",                  "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_maxParses_METHODP                    marpaESLIFMethodCacheArrayp[47].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "maxParses",                   "()I", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setResult_METHODP                    marpaESLIFMethodCacheArrayp[48].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "setResult",                   "(Ljava/lang/Object;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_getResult_METHODP                    marpaESLIFMethodCacheArrayp[49].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "getResult",                   "()Ljava/lang/Object;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setSymbolName_METHODP                marpaESLIFMethodCacheArrayp[50].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "setSymbolName",               "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setSymbolNumber_METHODP              marpaESLIFMethodCacheArrayp[51].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "setSymbolNumber",             "(I)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setRuleName_METHODP                  marpaESLIFMethodCacheArrayp[52].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "setRuleName",                 "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setRuleNumber_METHODP                marpaESLIFMethodCacheArrayp[53].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "setRuleNumber",               "(I)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setGrammar_METHODP                   marpaESLIFMethodCacheArrayp[54].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "setGrammar",                  "(Lorg/parser/marpa/ESLIFGrammar;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_OBJECT_CLASS_getClass_METHODP                                   marpaESLIFMethodCacheArrayp[55].methodp
  {      &JAVA_LANG_OBJECT_CLASSCACHE, "getClass",                                  "()Ljava/lang/Class;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CLASS_CLASS_getName_METHODP                                     marpaESLIFMethodCacheArrayp[56].methodp
  {      &JAVA_LANG_CLASS_CLASSCACHE, "getName",                                    "()Ljava/lang/String;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFEVENTTYPE_CLASS_get_METHODP                               marpaESLIFMethodCacheArrayp[57].methodp
  {      &MARPAESLIF_ESLIFEVENTTYPE_CLASSCACHE, "get",                              "(I)Lorg/parser/marpa/ESLIFEventType;", 1 /* static */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFEVENTTYPE_CLASS_getCode_METHODP                           marpaESLIFMethodCacheArrayp[58].methodp
  {      &MARPAESLIF_ESLIFEVENTTYPE_CLASSCACHE, "getCode",                          "()I", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFEVENT_CLASS_init_METHODP                                  marpaESLIFMethodCacheArrayp[59].methodp
  {      &MARPAESLIF_ESLIFEVENT_CLASSCACHE, "<init>",                               "(Lorg/parser/marpa/ESLIFEventType;Ljava/lang/String;Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFLOGGERLEVEL_CLASS_getCode_METHODP                         marpaESLIFMethodCacheArrayp[60].methodp
  {      &MARPAESLIF_ESLIFLOGGERLEVEL_CLASSCACHE, "getCode",                        "()I", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_STRING_CLASS_getBytes_String_METHODP                            marpaESLIFMethodCacheArrayp[61].methodp
  {      &JAVA_LANG_STRING_CLASSCACHE, "getBytes",                                  "(Ljava/lang/String;)[B", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMARPROPERTIES_CLASS_init_METHODP                      marpaESLIFMethodCacheArrayp[62].methodp
  {      &MARPAESLIF_ESLIFGRAMMARPROPERTIES_CLASSCACHE, "<init>",                   "(IILjava/lang/String;ZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;II[I[ILjava/lang/String;Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMARRULEPROPERTIES_CLASS_init_METHODP                  marpaESLIFMethodCacheArrayp[63].methodp
  {      &MARPAESLIF_ESLIFGRAMMARRULEPROPERTIES_CLASSCACHE, "<init>",               "(ILjava/lang/String;Ljava/lang/String;II[I[ZILjava/lang/String;Ljava/lang/String;ZIZZZIZIZ)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFGRAMMARSYMBOLPROPERTIES_CLASS_init_METHODP                marpaESLIFMethodCacheArrayp[64].methodp
  {      &MARPAESLIF_ESLIFGRAMMARSYMBOLPROPERTIES_CLASSCACHE, "<init>",             "(Lorg/parser/marpa/ESLIFSymbolType;ZZZZZILjava/lang/String;Ljava/lang/String;ZLjava/lang/String;ZLjava/lang/String;ZLjava/lang/String;ZLjava/lang/String;ZLjava/lang/String;ZIILjava/lang/String;IILjava/lang/String;Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFSYMBOLTYPE_CLASS_get_METHODP                              marpaESLIFMethodCacheArrayp[65].methodp
  {      &MARPAESLIF_ESLIFSYMBOLTYPE_CLASSCACHE, "get",                             "(I)Lorg/parser/marpa/ESLIFSymbolType;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIF_CLASS_getLoggerInterfaceByIndicep_METHODP                marpaESLIFMethodCacheArrayp[66].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "getLoggerInterfaceByIndice",          "(I)Lorg/parser/marpa/ESLIFLoggerInterface;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CHARACTER_CLASS_init_METHODP                                    marpaESLIFMethodCacheArrayp[67].methodp
  {      &JAVA_LANG_CHARACTER_CLASSCACHE, "<init>",                                 "(C)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CHARACTER_CLASS_valueOf_METHODP                                 marpaESLIFMethodCacheArrayp[68].methodp
  {      &JAVA_LANG_CHARACTER_CLASSCACHE, "valueOf",                                "(C)Ljava/lang/Character;", 1 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_LANG_SHORT_CLASS_init_METHODP                                        marpaESLIFMethodCacheArrayp[69].methodp
  {      &JAVA_LANG_SHORT_CLASSCACHE, "<init>",                                     "(S)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_SHORT_CLASS_valueOf_METHODP                                     marpaESLIFMethodCacheArrayp[70].methodp
  {      &JAVA_LANG_SHORT_CLASSCACHE, "valueOf",                                    "(S)Ljava/lang/Short;", 1 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_LANG_INTEGER_CLASS_init_METHODP                                      marpaESLIFMethodCacheArrayp[71].methodp
  {      &JAVA_LANG_INTEGER_CLASSCACHE, "<init>",                                   "(I)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_INTEGER_CLASS_valueOf_METHODP                                   marpaESLIFMethodCacheArrayp[72].methodp
  {      &JAVA_LANG_INTEGER_CLASSCACHE, "valueOf",                                  "(I)Ljava/lang/Integer;", 1 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_LANG_LONG_CLASS_init_METHODP                                         marpaESLIFMethodCacheArrayp[73].methodp
  {      &JAVA_LANG_LONG_CLASSCACHE, "<init>",                                      "(J)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_LONG_CLASS_valueOf_METHODP                                      marpaESLIFMethodCacheArrayp[74].methodp
  {      &JAVA_LANG_LONG_CLASSCACHE, "valueOf",                                     "(J)Ljava/lang/Long;", 1 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_LANG_FLOAT_CLASS_init_METHODP                                        marpaESLIFMethodCacheArrayp[75].methodp
  {      &JAVA_LANG_FLOAT_CLASSCACHE, "<init>",                                     "(F)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_FLOAT_CLASS_valueOf_METHODP                                     marpaESLIFMethodCacheArrayp[76].methodp
  {      &JAVA_LANG_FLOAT_CLASSCACHE, "valueOf",                                    "(F)Ljava/lang/Float;", 1 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_LANG_DOUBLE_CLASS_init_METHODP                                       marpaESLIFMethodCacheArrayp[77].methodp
  {      &JAVA_LANG_DOUBLE_CLASSCACHE, "<init>",                                    "(D)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_DOUBLE_CLASS_valueOf_METHODP                                    marpaESLIFMethodCacheArrayp[78].methodp
  {      &JAVA_LANG_DOUBLE_CLASSCACHE, "valueOf",                                   "(D)Ljava/lang/Double;", 1 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_LANG_BOOLEAN_CLASS_init_METHODP                                      marpaESLIFMethodCacheArrayp[79].methodp
  {      &JAVA_LANG_BOOLEAN_CLASSCACHE, "<init>",                                   "(Z)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_BOOLEAN_CLASS_valueOf_METHODP                                   marpaESLIFMethodCacheArrayp[80].methodp
  {      &JAVA_LANG_BOOLEAN_CLASSCACHE, "valueOf",                                  "(Z)Ljava/lang/Boolean;", 1 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_LANG_STRING_CLASS_init_byteArray_METHODP                             marpaESLIFMethodCacheArrayp[81].methodp
  {      &JAVA_LANG_STRING_CLASSCACHE, "<init>",                                    "([B)V", 0 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_LANG_STRING_CLASS_init_byteArray_String_METHODP                      marpaESLIFMethodCacheArrayp[82].methodp
  {      &JAVA_LANG_STRING_CLASSCACHE, "<init>",                                    "([BLjava/lang/String;)V", 0 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_UTIL_HASHMAP_CLASS_init_METHODP                                      marpaESLIFMethodCacheArrayp[83].methodp
  {      &JAVA_UTIL_HASHMAP_CLASSCACHE, "<init>",                                   "()V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_UTIL_HASHMAP_CLASS_put_METHODP                                       marpaESLIFMethodCacheArrayp[84].methodp
  {      &JAVA_UTIL_HASHMAP_CLASSCACHE, "put",                                      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_MATH_BIGDECIMAL_CLASS_String_init_METHODP                            marpaESLIFMethodCacheArrayp[85].methodp
  {      &JAVA_MATH_BIGDECIMAL_CLASSCACHE, "<init>",                                "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_DOUBLE_CLASS_doubleValue_METHODP                                marpaESLIFMethodCacheArrayp[86].methodp
  {      &JAVA_LANG_DOUBLE_CLASSCACHE, "doubleValue",                               "()D", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CHARACTER_CLASS_charValue_METHODP                               marpaESLIFMethodCacheArrayp[87].methodp
  {      &JAVA_LANG_CHARACTER_CLASSCACHE, "charValue",                              "()C", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_BYTE_CLASS_valueOf_METHODP                                      marpaESLIFMethodCacheArrayp[88].methodp
  {      &JAVA_LANG_BYTE_CLASSCACHE, "valueOf",                                     "(B)Ljava/lang/Byte;", 1 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_LANG_BYTE_CLASS_init_METHODP                                         marpaESLIFMethodCacheArrayp[89].methodp
  {      &JAVA_LANG_BYTE_CLASSCACHE, "<init>",                                      "(B)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_BOOLEAN_CLASS_booleanValue_METHODP                              marpaESLIFMethodCacheArrayp[90].methodp
  {      &JAVA_LANG_BOOLEAN_CLASSCACHE, "booleanValue",                             "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_BYTE_CLASS_byteValue_METHODP                                    marpaESLIFMethodCacheArrayp[91].methodp
  {      &JAVA_LANG_BYTE_CLASSCACHE, "byteValue",                                   "()B", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CHARACTER_CLASS_toString_METHODP                                marpaESLIFMethodCacheArrayp[92].methodp
  {      &JAVA_LANG_CHARACTER_CLASSCACHE, "toString",                               "()Ljava/lang/String;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_SHORT_CLASS_shortValue_METHODP                                  marpaESLIFMethodCacheArrayp[93].methodp
  {      &JAVA_LANG_SHORT_CLASSCACHE, "shortValue",                                 "()S", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CLASS_CLASS_equals_METHODP                                      marpaESLIFMethodCacheArrayp[94].methodp
  {      &JAVA_LANG_CLASS_CLASSCACHE, "equals",                                     "(Ljava/lang/Object;)Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_MATH_BIGINTEGER_CLASS_String_init_METHODP                            marpaESLIFMethodCacheArrayp[95].methodp
  {      &JAVA_MATH_BIGINTEGER_CLASSCACHE, "<init>",                                "(Ljava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_INTEGER_CLASS_intValue_METHODP                                  marpaESLIFMethodCacheArrayp[96].methodp
  {      &JAVA_LANG_INTEGER_CLASSCACHE, "intValue",                                 "()I", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_LONG_CLASS_longValue_METHODP                                    marpaESLIFMethodCacheArrayp[97].methodp
  {      &JAVA_LANG_LONG_CLASSCACHE, "longValue",                                   "()J", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_FLOAT_CLASS_floatValue_METHODP                                  marpaESLIFMethodCacheArrayp[98].methodp
  {      &JAVA_LANG_FLOAT_CLASSCACHE, "floatValue",                                 "()F", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CLASS_CLASS_isArray_METHODP                                     marpaESLIFMethodCacheArrayp[99].methodp
  {      &JAVA_LANG_CLASS_CLASSCACHE, "isArray",                                    "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CLASS_CLASS_getComponentType_METHODP                            marpaESLIFMethodCacheArrayp[100].methodp
  {      &JAVA_LANG_CLASS_CLASSCACHE, "getComponentType",                           "()Ljava/lang/Class;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CLASS_CLASS_isPrimitive_METHODP                                 marpaESLIFMethodCacheArrayp[101].methodp
  {      &JAVA_LANG_CLASS_CLASSCACHE, "isPrimitive",                                "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_STRING_CLASS_init_METHODP                                       marpaESLIFMethodCacheArrayp[102].methodp
  {      &JAVA_LANG_STRING_CLASSCACHE, "<init>",                                    "()V", 0 /* staticb */, NULL, 0 /* requiredb */ },

  #define JAVA_UTIL_SET_CLASS_iterator_METHODP                                      marpaESLIFMethodCacheArrayp[103].methodp
  {      &JAVA_UTIL_SET_CLASSCACHE, "iterator",                                     "()Ljava/util/Iterator;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_UTIL_ITERATOR_CLASS_hasNext_METHODP                                  marpaESLIFMethodCacheArrayp[104].methodp
  {      &JAVA_UTIL_ITERATOR_CLASSCACHE, "hasNext",                                 "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_UTIL_ITERATOR_CLASS_next_METHODP                                     marpaESLIFMethodCacheArrayp[105].methodp
  {      &JAVA_UTIL_ITERATOR_CLASSCACHE, "next",                                    "()Ljava/lang/Object;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFJSONDECODEROPTION_CLASS_isDisallowDupkeys_METHODP         marpaESLIFMethodCacheArrayp[106].methodp
  {      &MARPAESLIF_ESLIFJSONDECODEROPTION_CLASSCACHE, "isDisallowDupkeys",        "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFJSONDECODEROPTION_CLASS_getMaxDepth_METHODP               marpaESLIFMethodCacheArrayp[107].methodp
  {      &MARPAESLIF_ESLIFJSONDECODEROPTION_CLASSCACHE, "getMaxDepth",              "()J", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFJSONDECODEROPTION_CLASS_isNoReplacementCharacter_METHODP  marpaESLIFMethodCacheArrayp[108].methodp
  {      &MARPAESLIF_ESLIFJSONDECODEROPTION_CLASSCACHE, "isNoReplacementCharacter", "()Z", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFREGEXCALLOUT_CLASS_init_METHODP                           marpaESLIFMethodCacheArrayp[109].methodp
  {      &MARPAESLIF_ESLIFREGEXCALLOUT_CLASSCACHE, "<init>",                        "(Ljava/lang/Integer;Ljava/lang/String;[BLjava/lang/String;JJ[JLjava/lang/Long;JJLjava/lang/String;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFSYMBOL_CLASS_getLoggerInterfacep_METHODP                  marpaESLIFMethodCacheArrayp[110].methodp
  {      &MARPAESLIF_ESLIFSYMBOL_CLASSCACHE, "getLoggerInterface",                 "()Lorg/parser/marpa/ESLIFLoggerInterface;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFSYMBOL_CLASS_setMarpaESLIFSymbolp_METHODP                 marpaESLIFMethodCacheArrayp[111].methodp
  {      &MARPAESLIF_ESLIFSYMBOL_CLASSCACHE, "setMarpaESLIFSymbolp",                "(Ljava/nio/ByteBuffer;)V", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFSYMBOL_CLASS_getMarpaESLIFSymbolp_METHODP                 marpaESLIFMethodCacheArrayp[112].methodp
  {      &MARPAESLIF_ESLIFSYMBOL_CLASSCACHE, "getMarpaESLIFSymbolp",                "()Ljava/nio/ByteBuffer;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  #define MARPAESLIF_ESLIFSYMBOL_CLASS_getEslif_METHODP                             marpaESLIFMethodCacheArrayp[113].methodp
  {      &MARPAESLIF_ESLIFSYMBOL_CLASSCACHE, "getEslif",                            "()Lorg/parser/marpa/ESLIF;", 0 /* staticb */, NULL, 1 /* requiredb */ },

  { NULL }
};

static marpaESLIFFieldCache_t marpaESLIFFieldCacheArrayp[] = {
  #define JAVA_LANG_BOOLEAN_CLASS_TYPE_FIELDP                                      marpaESLIFFieldCacheArrayp[0].fieldp
  {      &JAVA_LANG_BOOLEAN_CLASSCACHE, "TYPE",                                    "Ljava/lang/Class;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_CHARACTER_CLASS_TYPE_FIELDP                                    marpaESLIFFieldCacheArrayp[1].fieldp
  {      &JAVA_LANG_CHARACTER_CLASSCACHE, "TYPE",                                  "Ljava/lang/Class;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_BYTE_CLASS_TYPE_FIELDP                                         marpaESLIFFieldCacheArrayp[2].fieldp
  {      &JAVA_LANG_BYTE_CLASSCACHE, "TYPE",                                       "Ljava/lang/Class;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_SHORT_CLASS_TYPE_FIELDP                                        marpaESLIFFieldCacheArrayp[3].fieldp
  {      &JAVA_LANG_SHORT_CLASSCACHE, "TYPE",                                      "Ljava/lang/Class;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_INTEGER_CLASS_TYPE_FIELDP                                      marpaESLIFFieldCacheArrayp[4].fieldp
  {      &JAVA_LANG_INTEGER_CLASSCACHE, "TYPE",                                    "Ljava/lang/Class;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_LONG_CLASS_TYPE_FIELDP                                         marpaESLIFFieldCacheArrayp[5].fieldp
  {      &JAVA_LANG_LONG_CLASSCACHE, "TYPE",                                       "Ljava/lang/Class;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_FLOAT_CLASS_TYPE_FIELDP                                        marpaESLIFFieldCacheArrayp[6].fieldp
  {      &JAVA_LANG_FLOAT_CLASSCACHE, "TYPE",                                      "Ljava/lang/Class;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_DOUBLE_CLASS_TYPE_FIELDP                                       marpaESLIFFieldCacheArrayp[7].fieldp
  {      &JAVA_LANG_DOUBLE_CLASSCACHE, "TYPE",                                     "Ljava/lang/Class;", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_FLOAT_CLASS_TYPE_POSITIVE_INFINITY                             marpaESLIFFieldCacheArrayp[8].fieldp
  {      &JAVA_LANG_FLOAT_CLASSCACHE, "POSITIVE_INFINITY",                         "F", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_FLOAT_CLASS_TYPE_NEGATIVE_INFINITY                             marpaESLIFFieldCacheArrayp[9].fieldp
  {      &JAVA_LANG_FLOAT_CLASSCACHE, "NEGATIVE_INFINITY",                         "F", 1 /* staticb */, NULL, 1 /* requiredb */ },

  #define JAVA_LANG_FLOAT_CLASS_TYPE_NAN                                           marpaESLIFFieldCacheArrayp[10].fieldp
  {      &JAVA_LANG_FLOAT_CLASSCACHE, "NaN",                                       "F", 1 /* staticb */, NULL, 1 /* requiredb */ },

  { NULL }
};

static jclass booleanTypeClassp = NULL;
static jclass characterTypeClassp = NULL;
static jclass byteTypeClassp = NULL;
static jclass shortTypeClassp = NULL;
static jclass integerTypeClassp = NULL;
static jclass longTypeClassp = NULL;
static jclass floatTypeClassp = NULL;
static jclass doubleTypeClassp = NULL;

static jfloat floatMaxValue;
static jfloat floatMinValue;
static jfloat floatPositiveInfinity;
static jfloat floatNegativeInfinity;
static jfloat floatNaN;

typedef struct fieldsValuesStorage {
  jclass    classp;
  jfieldID  fieldp;
  jclass    *primitiveClasspp;
} fieldsValuesStorage_t;

fieldsValuesStorage_t fieldsValues[] = {
  { NULL, NULL, &booleanTypeClassp },
  { NULL, NULL, &characterTypeClassp },
  { NULL, NULL, &byteTypeClassp },
  { NULL, NULL, &shortTypeClassp },
  { NULL, NULL, &integerTypeClassp },
  { NULL, NULL, &longTypeClassp },
  { NULL, NULL, &floatTypeClassp },
  { NULL, NULL, &doubleTypeClassp }
};

#define MARPAESLIFJAVA_PUSH_PTR(stackp, p) do {                         \
    GENERICSTACK_PUSH_PTR(stackp, p);                                   \
    if (GENERICSTACK_ERROR(stackp)) {                                   \
      RAISEEXCEPTIONF(envp, "%s push failure, %s", #stackp, strerror(errno)); \
    }                                                                   \
  } while (0)


#define MARPAESLIFJAVA_NEW_BYTE(envp, objectp, jbytevalue) do {         \
    if (JAVA_LANG_BYTE_CLASS_valueOf_METHODP != NULL) {                 \
      objectp = (*envp)->CallStaticObjectMethod(envp, JAVA_LANG_BYTE_CLASSP, JAVA_LANG_BYTE_CLASS_valueOf_METHODP, (jbyte) jbytevalue); \
    } else {                                                            \
      objectp = (*envp)->NewObject(envp, JAVA_LANG_BYTE_CLASSP, JAVA_LANG_BYTE_CLASS_init_METHODP, (jbyte) jbytevalue); \
    }                                                                   \
    if (objectp == NULL) {                                              \
      goto err;                                                         \
    }                                                                   \
  } while (0)

#define MARPAESLIFJAVA_NEW_SHORT(envp, objectp, jshortvalue) do {       \
    if (JAVA_LANG_SHORT_CLASS_valueOf_METHODP != NULL) {                \
      objectp = (*envp)->CallStaticObjectMethod(envp, JAVA_LANG_SHORT_CLASSP, JAVA_LANG_SHORT_CLASS_valueOf_METHODP, (jshort) jshortvalue); \
    } else {                                                            \
      objectp = (*envp)->NewObject(envp, JAVA_LANG_SHORT_CLASSP, JAVA_LANG_SHORT_CLASS_init_METHODP, (jshort) jshortvalue); \
    }                                                                   \
    if (objectp == NULL) {                                              \
      goto err;                                                         \
    }                                                                   \
  } while (0)

#define MARPAESLIFJAVA_NEW_INTEGER(envp, objectp, jintegervalue) do {   \
    if (JAVA_LANG_INTEGER_CLASS_valueOf_METHODP != NULL) {              \
      objectp = (*envp)->CallStaticObjectMethod(envp, JAVA_LANG_INTEGER_CLASSP, JAVA_LANG_INTEGER_CLASS_valueOf_METHODP, (jint) jintegervalue); \
    } else {                                                            \
      objectp = (*envp)->NewObject(envp, JAVA_LANG_INTEGER_CLASSP, JAVA_LANG_INTEGER_CLASS_init_METHODP, (jint) jintegervalue); \
    }                                                                   \
    if (objectp == NULL) {                                              \
      goto err;                                                         \
    }                                                                   \
  } while (0)

#define MARPAESLIFJAVA_NEW_LONG(envp, objectp, jlongvalue) do {         \
    if (JAVA_LANG_LONG_CLASS_valueOf_METHODP != NULL) {                 \
      objectp = (*envp)->CallStaticObjectMethod(envp, JAVA_LANG_LONG_CLASSP, JAVA_LANG_LONG_CLASS_valueOf_METHODP, (jlong) jlongvalue); \
    } else {                                                            \
      objectp = (*envp)->NewObject(envp, JAVA_LANG_LONG_CLASSP, JAVA_LANG_LONG_CLASS_init_METHODP, (jlong) jlongvalue); \
    }                                                                   \
    if (objectp == NULL) {                                              \
      goto err;                                                         \
    }                                                                   \
  } while (0)

#define MARPAESLIFJAVA_NEW_CHARACTER(envp, objectp, jcharvalue) do {    \
    if (JAVA_LANG_CHARACTER_CLASS_valueOf_METHODP != NULL) {            \
      objectp = (*envp)->CallStaticObjectMethod(envp, JAVA_LANG_CHARACTER_CLASSP, JAVA_LANG_CHARACTER_CLASS_valueOf_METHODP, (jchar) jcharvalue); \
    } else {                                                            \
      objectp = (*envp)->NewObject(envp, JAVA_LANG_CHARACTER_CLASSP, JAVA_LANG_CHARACTER_CLASS_init_METHODP, (jchar) jcharvalue); \
    }                                                                   \
    if (objectp == NULL) {                                              \
      goto err;                                                         \
    }                                                                   \
  } while (0)

#define MARPAESLIFJAVA_NEW_FLOAT(envp, objectp, jfloatvalue) do {       \
    if (JAVA_LANG_FLOAT_CLASS_valueOf_METHODP != NULL) {                \
      objectp = (*envp)->CallStaticObjectMethod(envp, JAVA_LANG_FLOAT_CLASSP, JAVA_LANG_FLOAT_CLASS_valueOf_METHODP, (jfloat) jfloatvalue); \
    } else {                                                            \
      objectp = (*envp)->NewObject(envp, JAVA_LANG_FLOAT_CLASSP, JAVA_LANG_FLOAT_CLASS_init_METHODP, (jfloat) jfloatvalue); \
    }                                                                   \
    if (objectp == NULL) {                                              \
      goto err;                                                         \
    }                                                                   \
  } while (0)


#define MARPAESLIFJAVA_NEW_DOUBLE(envp, objectp, jdoublevalue) do {     \
    if (JAVA_LANG_DOUBLE_CLASS_valueOf_METHODP != NULL) {               \
      objectp = (*envp)->CallStaticObjectMethod(envp, JAVA_LANG_DOUBLE_CLASSP, JAVA_LANG_DOUBLE_CLASS_valueOf_METHODP, (jdouble) jdoublevalue); \
    } else {                                                            \
      objectp = (*envp)->NewObject(envp, JAVA_LANG_DOUBLE_CLASSP, JAVA_LANG_DOUBLE_CLASS_init_METHODP, (jdouble) jdoublevalue); \
    }                                                                   \
    if (objectp == NULL) {                                              \
      goto err;                                                         \
    }                                                                   \
  } while (0)


#define MARPAESLIFJAVA_IMPORT_NUMBER_NOT_DECIMAL(envp, objectp, c_format, c_format_cast, c_value) do { \
    genericLogger_t              *_genericLoggerp          = NULL;      \
    jstring                       _numberp;                             \
    marpaESLIFJava_stringGeneratorContext_t  _stringGenerator = { NULL /* char *s */, 0 /* size_t l */, 0 /* short okb */}; \
    char                         *_p; /* To skip eventual spaces at the beginning of the string */ \
                                                                        \
    if (sizeof(c_format_cast) <= sizeof(jbyte)) {                       \
      /* fprintf(stderr, "==> %s: import Byte 0x%lx\n", funcs, (unsigned long) c_value); fflush(stdout); fflush(stderr); */  \
      MARPAESLIFJAVA_NEW_BYTE(envp, objectp, c_value);                  \
    } else if (sizeof(c_format_cast) <= sizeof(jshort)) {               \
      /* fprintf(stderr, "==> %s: import Short '%d'\n", funcs, (int) c_value); fflush(stdout); fflush(stderr); */  \
      MARPAESLIFJAVA_NEW_SHORT(envp, objectp, c_value);                 \
    } else if (sizeof(c_format_cast) <= sizeof(jint)) {                 \
      /* fprintf(stderr, "==> %s: import Integer '%d'\n", funcs, (int) c_value); fflush(stdout); fflush(stderr); */  \
      MARPAESLIFJAVA_NEW_INTEGER(envp, objectp, c_value);               \
    } else if (sizeof(c_format_cast) <= sizeof(jlong)) {                \
      /* fprintf(stderr, "==> %s: import Long '%ld'\n", funcs, (long) c_value); fflush(stdout); fflush(stderr); */  \
      MARPAESLIFJAVA_NEW_LONG(envp, objectp, c_value);                  \
    } else { /* Try with java.math.BigInteger */                        \
      /* We go through the string representation, hopefully containing all digits... */ \
      _genericLoggerp = GENERICLOGGER_CUSTOM(generateStringWithLoggerCallbackv, (void *) &_stringGenerator, GENERICLOGGER_LOGLEVEL_TRACE); \
      if (_genericLoggerp == NULL) {                                    \
        RAISEEXCEPTIONF(envp, "GENERICLOGGER_CUSTOM() returned NULL, %s", strerror(errno)); \
      }                                                                 \
      _stringGenerator.s   = NULL;                                      \
      _stringGenerator.l   = 0;                                         \
      _stringGenerator.okb = 0;                                         \
      GENERICLOGGER_TRACEF(_genericLoggerp, c_format, (c_format_cast) c_value); \
      if (! _stringGenerator.okb) {                                     \
        GENERICLOGGER_FREE(_genericLoggerp);                            \
        RAISEEXCEPTIONF(envp, "GENERICLOGGER_TRACEF with format string %s failed, %s", c_format, strerror(errno)); \
      }                                                                 \
      /* ASCII formatting, compatible with Java's modified UTF-8 */     \
      _p = _stringGenerator.s;                                          \
      while (*_p == ' ') {                                              \
        _p++;                                                           \
      }                                                                 \
      _numberp = (*envp)->NewStringUTF(envp, (const char *) _p); \
      if (_numberp == NULL) {                                           \
        /* We want OUR exception to be raised */                        \
        GENERICLOGGER_FREE(_genericLoggerp);                            \
        free(_stringGenerator.s);                                       \
        RAISEEXCEPTIONF(envp, "NewStringUTF(\"%s\") failure", _stringGenerator.s); \
      }                                                                 \
      /* fprintf(stderr, "==> %s: import BigInteger '%s'\n", funcs, (char *) _p); fflush(stdout); fflush(stderr); */  \
      objectp = (*envp)->NewObject(envp, JAVA_MATH_BIGINTEGER_CLASSP, JAVA_MATH_BIGINTEGER_CLASS_String_init_METHODP, _numberp); \
      if (objectp == NULL) {                                            \
        GENERICLOGGER_FREE(_genericLoggerp);                            \
        free(_stringGenerator.s);                                       \
        (*envp)->DeleteLocalRef(envp, _numberp);                        \
        RAISEEXCEPTION(envp, "NewObject failure");                      \
      }                                                                 \
      GENERICLOGGER_FREE(_genericLoggerp);                              \
      free(_stringGenerator.s);                                         \
      (*envp)->DeleteLocalRef(envp, _numberp);                          \
    }                                                                   \
  } while (0)

/* We have special things for +/-Infinity and NaN because Math.BigDecimal does NOT support */
/* these special values: only raw types do. We always project these special values to a jfloat */
/* that have the same representation in java than a jdouble when it is +/-Infinity or NaN */
#define MARPAESLIFJAVA_IMPORT_POSITIVE_INFINITY(marpaESLIFp, envp, objectp) do { \
    /* fprintf(stderr, "==> %s: import float positive infinity\n", funcs); fflush(stdout); fflush(stderr); */ \
    MARPAESLIFJAVA_NEW_FLOAT(envp, objectp, floatPositiveInfinity);     \
  } while (0);

#define MARPAESLIFJAVA_IMPORT_NEGATIVE_INFINITY(marpaESLIFp, envp, objectp) do { \
    /* fprintf(stderr, "==> %s: import float negative infinity\n", funcs); fflush(stdout); fflush(stderr); */ \
    MARPAESLIFJAVA_NEW_FLOAT(envp, objectp, floatNegativeInfinity);     \
  } while (0);

#define MARPAESLIFJAVA_IMPORT_NAN(marpaESLIFp, envp, objectp) do {      \
    /* fprintf(stderr, "==> %s: import float nan\n", funcs); fflush(stdout); fflush(stderr); */ \
    MARPAESLIFJAVA_NEW_FLOAT(envp, objectp, floatNaN);                  \
  } while (0);

#define MARPAESLIFJAVA_IMPORT_NUMBER_DECIMAL(marpaESLIFp, envp, objectp, c_format_cast, c_value, floattos) do { \
    jstring _numberp;                                                   \
    char *_s;                                                           \
                                                                        \
    if (sizeof(c_format_cast) <= sizeof(jfloat)) {                      \
      /* fprintf(stderr, "==> %s: import float '%f'\n", funcs, (double) c_value); fflush(stdout); fflush(stderr); */ \
      MARPAESLIFJAVA_NEW_FLOAT(envp, objectp, c_value);                 \
    } else if (sizeof(c_format_cast) <= sizeof(jdouble)) {              \
      /* fprintf(stderr, "==> %s: import double '%Lf'\n", funcs, (long double) c_value); fflush(stdout); fflush(stderr); */ \
      MARPAESLIFJAVA_NEW_DOUBLE(envp, objectp, c_value);                \
    } else { /* Try with java.math.BigDecimal */                        \
      /* We go through the string representation, hopefully containing all digits... */ \
      _s = floattos(marpaESLIFp, c_value);                              \
      if (_s == NULL) {                                                 \
        RAISEEXCEPTIONF(envp, "%s returned NULL", #floattos);           \
      }                                                                 \
      /* ASCII formatting, compatible with Java's modified UTF-8 */     \
      _numberp = (*envp)->NewStringUTF(envp, (const char *) _s);        \
      if (_numberp == NULL) {                                           \
        /* We want OUR exception to be raised */                        \
        free(_s);                                                       \
        RAISEEXCEPTIONF(envp, "NewStringUTF(\"%s\") failure", _stringGenerator.s); \
      }                                                                 \
      /* fprintf(stderr, "==> %s: import BigFloat '%s'\n", funcs, (char *) _s); fflush(stdout); fflush(stderr); */  \
      objectp = (*envp)->NewObject(envp, JAVA_MATH_BIGDECIMAL_CLASSP, JAVA_MATH_BIGDECIMAL_CLASS_String_init_METHODP, _numberp); \
      if (objectp == NULL) {                                            \
        free(_s);                                                       \
        (*envp)->DeleteLocalRef(envp, _numberp);                        \
        RAISEEXCEPTION(envp, "NewObject failure");                      \
      }                                                                 \
      free(_s);                                                         \
      (*envp)->DeleteLocalRef(envp, _numberp);                          \
    }                                                                   \
  } while (0)

#ifdef MARPAESLIF_HAVE_LONG_LONG

#  define MARPAESLIFJAVA_EXPORT_NUMBER_NOT_DECIMAL(envp, jni_type, jni_value, eslifb) do { \
    if (sizeof(jni_type) <= sizeof(char)) {                             \
      /* fprintf(stderr, "%s: export CHAR '%c'\n", funcs, (char) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_CHAR; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.c            = (char) jni_value;        \
      eslifb = 1;                                                       \
    } else if (sizeof(jni_type) <= sizeof(short)) {                     \
      /* fprintf(stderr, "%s: export SHORT '%d'\n", funcs, (int) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_SHORT; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.b            = (short) jni_value;       \
      eslifb = 1;                                                       \
    } else if (sizeof(jni_type) <= sizeof(int)) {                       \
      /* fprintf(stderr, "%s: export INT '%d'\n", funcs, (int) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_INT; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.i            = (int) jni_value;         \
      eslifb = 1;                                                       \
    } else if (sizeof(jni_type) <= sizeof(long)) {                      \
      /* fprintf(stderr, "%s: export LONG '%ld'\n", funcs, (long) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_LONG; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.l            = (long) jni_value;        \
      eslifb = 1;                                                       \
    } else if (sizeof(jni_type) <= sizeof(MARPAESLIF_LONG_LONG)) {      \
      /* fprintf(stderr, "%s: export LONG_LONG\n", funcs); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_LONG_LONG; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.ll           = (MARPAESLIF_LONG_LONG) jni_value; \
      eslifb = 1;                                                       \
    }                                                                   \
  } while (0)

#else /* MARPAESLIF_HAVE_LONG_LONG */

#  define MARPAESLIFJAVA_EXPORT_NUMBER_NOT_DECIMAL(envp, jni_type, jni_value, eslifb) do { \
    if (sizeof(jni_type) <= sizeof(char)) {                               \
      /* fprintf(stderr, "%s: export CHAR '%c'\n", funcs, (char) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_CHAR; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.c            = (char) jni_value;        \
      eslifb = 1;                                                       \
    } else if (sizeof(jni_type) <= sizeof(short)) {                     \
      /* fprintf(stderr, "%s: export SHORT '%d'\n", funcs, (int) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_SHORT; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.b            = (short) jni_value;       \
      eslifb = 1;                                                       \
    } else if (sizeof(jni_type) <= sizeof(int)) {                       \
      /* fprintf(stderr, "%s: export INT '%d'\n", funcs, (int) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_INT; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.i            = (int) jni_value;         \
      eslifb = 1;                                                       \
    } else if (sizeof(jni_type) <= sizeof(long)) {                      \
      /* fprintf(stderr, "%s: export LONG '%ld'\n", funcs, (long) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_LONG; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.l            = (long) jni_value;        \
      eslifb = 1;                                                       \
    }                                                                   \
  } while (0)

#endif /* MARPAESLIF_HAVE_LONG_LONG */

#define MARPAESLIFJAVA_EXPORT_NUMBER_DECIMAL(envp, jni_type, jni_value, eslifb) do { \
    if (sizeof(jni_type) <= sizeof(float)) {                            \
      /* fprintf(stderr, "%s: export FLOAT '%f'\n", funcs, (double) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_FLOAT; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.f            = (float) jni_value;       \
      eslifb = 1;                                                       \
    } else if (sizeof(jni_type) <= sizeof(double)) {                    \
      /* fprintf(stderr, "%s: export FLOAT '%f'\n", funcs, (double) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_DOUBLE; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.d            = (double) jni_value;      \
      eslifb = 1;                                                       \
    } else if (sizeof(jni_type) <= sizeof(long double)) {               \
      /* fprintf(stderr, "%s: export LONG_DOUBLE '%Lf'\n", funcs, (long double) jni_value); fflush(stdout); fflush(stderr); */ \
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_LONG_DOUBLE; \
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT; \
      marpaESLIFValueResultp->representationp = NULL;                   \
      marpaESLIFValueResultp->u.ld            = (long double) jni_value; \
      eslifb = 1;                                                       \
    }                                                                   \
  } while (0)

/* -------------- */
/* Static methods */
/* -------------- */
static void  generateStringWithLoggerCallbackv(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs);
static void  genericLoggerCallbackv(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs);
static void  readerCallbackDisposev(void *userDatavp, char *inputcp, size_t inputl, short eofb, short characterStreamb, char *encodings, size_t encodingl);
static short readerCallbackb(void *userDatavp, char **inputcpp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingsp, size_t *encodinglp, marpaESLIFReaderDispose_t *disposeCallbackpp);
static short ESLIF_contextb(JNIEnv *envp, jobject eslifp, jmethodID getLoggerInterfacep,
                            genericLogger_t **genericLoggerpp,
                            marpaESLIF_t **marpaESLIFpp);
static short ESLIFGrammar_contextb(JNIEnv *envp, jobject eslifGrammarp, marpaESLIF_t **marpaESLIFpp, marpaESLIFGrammar_t **marpaESLIFGrammarpp);
static short ESLIFSymbol_contextb(JNIEnv *envp, jobject eslifGrammarp, marpaESLIF_t **marpaESLIFpp, marpaESLIFSymbol_t **marpaESLIFSymbolpp);
static short ESLIFRecognizer_contextb(JNIEnv *envp, jobject eslifRecognizerp, marpaESLIFRecognizer_t **marpaESLIFRecognizerpp, marpaESLIFRecognizerContext_t **marpaESLIFRecognizerContextpp);
static short ESLIFValue_contextb(JNIEnv *envp, jobject eslifValuep, marpaESLIFValue_t **marpaESLIFValuepp, marpaESLIFValueContext_t **marpaESLIFValueContextpp);
static marpaESLIFValueRuleCallback_t   marpaESLIFValueRuleActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions);
static marpaESLIFValueSymbolCallback_t marpaESLIFValueSymbolActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions);
static marpaESLIFRecognizerIfCallback_t marpaESLIFRecognizerIfActionResolver(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *actions);
static marpaESLIFRecognizerEventCallback_t marpaESLIFRecognizerEventActionResolver(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *actions);
static marpaESLIFRecognizerRegexCallback_t marpaESLIFRecognizerRegexActionResolver(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *actions);
static short marpaESLIFValueContextInject(JNIEnv *envp, marpaESLIFValue_t *marpaESLIFValuep, jobject eslifValueInterfacep, marpaESLIFValueContext_t *marpaESLIFValueContextp);
static short marpaESLIFValueRuleCallback(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb);
static short marpaESLIFJava_valueSymbolCallbackb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, marpaESLIFValueResult_t *marpaESLIFValueResultp, int resulti);
static short marpaESLIFJava_recognizerIfCallbackb(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIFValueResult_t *marpaESLIFValueResultLexemep, marpaESLIFValueResultBool_t *marpaESLIFValueResultBoolp);
static short marpaESLIFJava_recognizerEventCallbackb(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIFEvent_t *eventArrayp, size_t eventArrayl, marpaESLIFValueResultBool_t *marpaESLIFValueResultBoolp);
static short marpaESLIFJava_recognizerRegexCallbackb(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIFValueResult_t *marpaESLIFCalloutBlockp, marpaESLIFValueResultInt_t *marpaESLIFValueResultOutp);
static void  marpaESLIFJava_genericFreeCallbackv(void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp);
static jmethodID marpaESLIFJava_valueActionResolveri(JNIEnv *envp, marpaESLIFValueContext_t *marpaESLIFValueContextp, char *methods, char *signatures);
static jmethodID marpaESLIFJava_recognizerActionResolveri(JNIEnv *envp, marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp, char *methods, char *signatures);
static void marpaESLIFJava_valueContextFreev(JNIEnv *envp, marpaESLIFValueContext_t *marpaESLIFValueContextp, short onStackb);
static void marpaESLIFJava_recognizerContextFreev(JNIEnv *envp, marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp, short onStackb);
static short marpaESLIFJava_valueContextInitb(JNIEnv *envp, jobject eslifValueInterfacep, jobject eslifGrammarp, marpaESLIFValueContext_t *marpaESLIFValueContextp);
static short marpaESLIFJava_recognizerContextInitb(JNIEnv *envp, jobject eslifRecognizerInterfacep, marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp, short haveLexemeStackb);
static void marpaESLIFJava_representationCallbackDisposev(void *userDatavp, char *inputcp, size_t inputl, char *encodingasciis);
static short marpaESLIFJava_representationCallbackb(void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp, char **inputcpp, size_t *inputlp, char **encodingasciisp, marpaESLIFRepresentationDispose_t *disposeCallbackpp);
static jobject marpaESLIFJava_grammarPropertiesp(JNIEnv *envp, marpaESLIFGrammarProperty_t *grammarPropertyp);
static jobject marpaESLIFJava_rulePropertiesp(JNIEnv *envp, marpaESLIFRuleProperty_t *rulePropertyp);
static jobject marpaESLIFJava_symbolPropertiesp(JNIEnv *envp, marpaESLIFSymbolProperty_t *symbolPropertyp);
static short marpaESLIFJava_valueGetObjectp(marpaESLIFValueContext_t *marpaESLIFValueContextp, marpaESLIFValue_t *marpaESLIFValuep, int stackindicei, marpaESLIFValueResult_t *marpaESLIFValueResultLexemep, jobject *objectpp);
static short marpaESLIFJava_recognizerGetObjectp(marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIFValueResult_t *marpaESLIFValueResultp, jobject *objectpp);
static short marpaESLIFJava_valueImportb(marpaESLIFValue_t *marpaESLIFValuep, void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp);
static short marpaESLIFJava_recognizerImportb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp);
static short marpaESLIFJava_importb(JNIEnv *envp, marpaESLIF_t *marpaESLIFp, genericStack_t *objectStackp, marpaESLIFValueResult_t *marpaESLIFValueResultp);
static jstring marpaESLIFJava_marpaESLIFStringToJavap(JNIEnv *envp, marpaESLIFString_t *marpaESLIFStringp);
static jstring marpaESLIFJava_marpaESLIFASCIIToJavap(JNIEnv *envp, char *asciis);
static jstring marpaESLIFJava_marpaESLIFActionToJavap(JNIEnv *envp, marpaESLIFAction_t *actionp);
static short marpaESLIFJava_stack_setb(JNIEnv *envp, marpaESLIFValue_t *marpaESLIFValuep, short resulti, jobject objectp, marpaESLIFValueResult_t *marpaESLIFValueResultOutputp);
static short marpaESLIFJava_JSONDecodeNumberActionb(void *userDatavp, char *strings, size_t stringl, marpaESLIFValueResult_t *marpaESLIFValueResultp);
static void  marpaESLIFJava_JSONDecodeFreeCallbackv(void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp);

/* --------------- */
/* Internal macros */
/* --------------- */

/* C.f. https://www.ibm.com/developerworks/aix/library/au-endianc/ */
static const int i_for_is_bigendian = 1;
short is_bigendian;
#define MARPAESLIF_IS_BIGENDIAN() ( (*(char*)&i_for_is_bigendian) == 0 )

/* Check if there is a pending exception - if yes, our raise will NOT raise a new one */
/* Since we asked for JNI_VERSION_1_4 per def ExceptionCheck() is available */
#define HAVEEXCEPTION(envp) (*envp)->ExceptionCheck(envp)

#define RAISEEXCEPTION(envp, msgs) do {                                 \
    if (! HAVEEXCEPTION(envp)) {                                        \
      if (MARPAESLIF_ESLIFEXCEPTION_CLASSP != NULL) {                   \
        genericLogger_t              *genericLoggerp = NULL;            \
        marpaESLIFJava_stringGeneratorContext_t  _stringGenerator;      \
                                                                        \
        _stringGenerator.s   = NULL;                                    \
        _stringGenerator.l   = 0;                                       \
        _stringGenerator.okb = 0;                                       \
                                                                        \
        genericLoggerp = GENERICLOGGER_CUSTOM(generateStringWithLoggerCallbackv, (void *) &_stringGenerator, GENERICLOGGER_LOGLEVEL_TRACE); \
        if (genericLoggerp != NULL) {                                   \
          GENERICLOGGER_TRACEF(genericLoggerp, "[In %s at %s:%d] %s", funcs, MARPAESLIF_FILENAMES, __LINE__, msgs); \
          if (_stringGenerator.okb) {                                   \
            (*envp)->ThrowNew(envp, MARPAESLIF_ESLIFEXCEPTION_CLASSP, _stringGenerator.s); \
            free(_stringGenerator.s);                                   \
          }                                                             \
          GENERICLOGGER_FREE(genericLoggerp);                           \
        }                                                               \
      }                                                                 \
    }                                                                   \
    goto err;                                                           \
} while (0)

#define RAISEEXCEPTIONF(envp, fmts, ...) do {                           \
    if (! HAVEEXCEPTION(envp)) {                                        \
      if (MARPAESLIF_ESLIFEXCEPTION_CLASSP != NULL) {                   \
        genericLogger_t              *genericLoggerp = NULL;            \
        marpaESLIFJava_stringGeneratorContext_t  _stringGenerator;      \
                                                                        \
        _stringGenerator.s   = NULL;                                    \
        _stringGenerator.l   = 0;                                       \
        _stringGenerator.okb = 0;                                       \
                                                                        \
        genericLoggerp = GENERICLOGGER_CUSTOM(generateStringWithLoggerCallbackv, (void *) &_stringGenerator, GENERICLOGGER_LOGLEVEL_TRACE); \
        if (genericLoggerp != NULL) {                                   \
          GENERICLOGGER_TRACEF(genericLoggerp, "[In %s at %s:%d] " fmts, funcs, MARPAESLIF_FILENAMES, __LINE__, __VA_ARGS__); \
          if (_stringGenerator.okb) {                                   \
            (*envp)->ThrowNew(envp, MARPAESLIF_ESLIFEXCEPTION_CLASSP, _stringGenerator.s); \
            free(_stringGenerator.s);                                   \
          }                                                             \
          GENERICLOGGER_FREE(genericLoggerp);                           \
        }                                                               \
      }                                                                 \
    }                                                                   \
    goto err;                                                           \
} while (0)

/* ---------------------------------------------------------------------------- */
/* Bytebuffer stuff - we always use a capaticy of 1 - we know what we are doing */
/* ---------------------------------------------------------------------------- */
#define BYTEBUFFER(what) byteBuffer##what##p
#define MARPAESLIF_PTR2BYTEBUFFER(what,ptr) do {                        \
    BYTEBUFFER(what) = (*envp)->NewDirectByteBuffer(envp, ptr, 1);      \
    if (BYTEBUFFER(what) == NULL) {                                     \
      RAISEEXCEPTION(envp, "NewDirectByteBuffer failure");              \
    }                                                                   \
  } while (0)

/*****************************************************************************/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *JavaVMp, void* reservedp)
/*****************************************************************************/
{
  static const char       *funcs = "JNI_OnLoad";
  jint                     rci;
  JNIEnv                  *envp                   = NULL;
  jclass                   classp                 = NULL;
  marpaESLIFClassCache_t  *marpaESLIFClassCachep  = marpaESLIFClassCacheArrayp;
  marpaESLIFMethodCache_t *marpaESLIFMethodCachep = marpaESLIFMethodCacheArrayp;
  marpaESLIFFieldCache_t  *marpaESLIFFieldCachep  = marpaESLIFFieldCacheArrayp;
  jstring                  stringp                = NULL;
  size_t                   i;

  /* ------------------------------------------------ */
  /* It is safe to store JavaVMp in a global variable */
  /* ------------------------------------------------ */
  marpaESLIF_vmp = JavaVMp;

  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    RAISEEXCEPTION(envp, "Failed to get environment");
  }

  /* Idem for marpaESLIF_UTF8p */
  stringp = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, "UTF-8");
  marpaESLIF_UTF8p = (jstring) (*envp)->NewGlobalRef(envp, (jobject) stringp);
  if (marpaESLIF_UTF8p == NULL) {
    /* We want OUR exception to be raised */
    RAISEEXCEPTION(envp, "NewGlobalRef failure");
  }

  /* --------------------------------------------------------------------------------------------------- */
  /* Get cache of classes - this is ok only if the result of FindClass is turned into a global reference */
  /* --------------------------------------------------------------------------------------------------- */
  while (marpaESLIFClassCachep->classs != NULL) {
    if (classp != NULL) {
      (*envp)->DeleteLocalRef(envp, classp);
    }
    classp = (*envp)->FindClass(envp, marpaESLIFClassCachep->classs);
    if (classp == NULL) {
      if (marpaESLIFClassCachep->requiredb) {
        /* We want OUR exception to be raised */
        (*envp)->ExceptionClear(envp);
        RAISEEXCEPTIONF(envp, "Failed to find class \"%s\"", marpaESLIFClassCachep->classs);
      } else {
        (*envp)->ExceptionClear(envp);
        /* Here, up to the program to know what it is doing */
      }
    } else {
      marpaESLIFClassCachep->classp = (*envp)->NewGlobalRef(envp, classp);
      if (marpaESLIFClassCachep->classp == NULL) {
        RAISEEXCEPTION(envp, "NewGlobalRef failure");
      }
      (*envp)->DeleteLocalRef(envp, classp);
      classp = NULL;
    }
    marpaESLIFClassCachep++;
  }

  /* -------------------- */
  /* Get cache of methods */
  /* -------------------- */
  while (marpaESLIFMethodCachep->marpaESLIFClassCachep != NULL) {
    /* If class does not exist and method is required this is an error */
    if (marpaESLIFMethodCachep->marpaESLIFClassCachep->classp == NULL) {
      if (marpaESLIFMethodCachep->requiredb) {
        RAISEEXCEPTIONF(envp,
                        "Failed to find %smethod \"%s\" with signature \"%s\" in class \"%s\" because the later was not found",
                        marpaESLIFMethodCachep->staticb ? "static " : "",
                        marpaESLIFMethodCachep->methods,
                        marpaESLIFMethodCachep->signatures,
                        marpaESLIFMethodCachep->marpaESLIFClassCachep->classs);
      }
    } else {
      if (marpaESLIFMethodCachep->staticb) {
        marpaESLIFMethodCachep->methodp = (*envp)->GetStaticMethodID(envp,
                                                                     marpaESLIFMethodCachep->marpaESLIFClassCachep->classp,
                                                                     marpaESLIFMethodCachep->methods,
                                                                     marpaESLIFMethodCachep->signatures);
      } else {
        marpaESLIFMethodCachep->methodp = (*envp)->GetMethodID(envp,
                                                               marpaESLIFMethodCachep->marpaESLIFClassCachep->classp,
                                                               marpaESLIFMethodCachep->methods,
                                                               marpaESLIFMethodCachep->signatures);
      }
      if (marpaESLIFMethodCachep->methodp == NULL) {
        if (marpaESLIFMethodCachep->requiredb) {
          /* We want OUR exception to be raised */
          (*envp)->ExceptionClear(envp);
          RAISEEXCEPTIONF(envp,
                          "Failed to find %smethod \"%s\" with signature \"%s\" in class \"%s\"",
                          marpaESLIFMethodCachep->staticb ? "static " : "",
                          marpaESLIFMethodCachep->methods,
                          marpaESLIFMethodCachep->signatures,
                          marpaESLIFMethodCachep->marpaESLIFClassCachep->classs);
        } else {
          /* Here, up to the program to know what it is doing */
          (*envp)->ExceptionClear(envp);
        }
      }
    }
    marpaESLIFMethodCachep++;
  }

  /* ------------------- */
  /* Get cache of fields */
  /* ------------------- */
  while (marpaESLIFFieldCachep->marpaESLIFClassCachep != NULL) {
    if (marpaESLIFFieldCachep->marpaESLIFClassCachep->classp == NULL) {
      /* We want OUR exception to be raised */
      (*envp)->ExceptionClear(envp);
      RAISEEXCEPTIONF(envp,
                      "Failed to find %sfield \"%s\" with signature \"%s\" in class \"%s\" because the later was not found",
                      marpaESLIFMethodCachep->staticb ? "static " : "",
                      marpaESLIFFieldCachep->fields,
                      marpaESLIFFieldCachep->signatures,
                      marpaESLIFFieldCachep->marpaESLIFClassCachep->classs);
    } else {
      if (marpaESLIFFieldCachep->staticb) {
        marpaESLIFFieldCachep->fieldp = (*envp)->GetStaticFieldID(envp,
                                                                  marpaESLIFFieldCachep->marpaESLIFClassCachep->classp,
                                                                  marpaESLIFFieldCachep->fields,
                                                                  marpaESLIFFieldCachep->signatures);
      } else {
        marpaESLIFFieldCachep->fieldp = (*envp)->GetFieldID(envp,
                                                            marpaESLIFFieldCachep->marpaESLIFClassCachep->classp,
                                                            marpaESLIFFieldCachep->fields,
                                                            marpaESLIFFieldCachep->signatures);
      }
      if (marpaESLIFFieldCachep->fieldp == NULL) {
        if (marpaESLIFFieldCachep->requiredb) {
          /* We want OUR exception to be raised */
          (*envp)->ExceptionClear(envp);
          RAISEEXCEPTIONF(envp,
                          "Failed to find %sfield \"%s\" with signature \"%s\" in class \"%s\"",
                          marpaESLIFMethodCachep->staticb ? "static " : "",
                          marpaESLIFFieldCachep->fields,
                          marpaESLIFFieldCachep->signatures,
                          marpaESLIFFieldCachep->marpaESLIFClassCachep->classs);
        } else {          
          /* Here, up to the program to know what it is doing */
          (*envp)->ExceptionClear(envp);
        }
      }
    }
    marpaESLIFFieldCachep++;
  }

  /* Get some static fields values */
  /* We have to fill the fieldsValues structure here for things that are not constant */
  fieldsValues[0].classp = JAVA_LANG_BOOLEAN_CLASSP;   fieldsValues[0].fieldp = JAVA_LANG_BOOLEAN_CLASS_TYPE_FIELDP;
  fieldsValues[1].classp = JAVA_LANG_CHARACTER_CLASSP; fieldsValues[1].fieldp = JAVA_LANG_CHARACTER_CLASS_TYPE_FIELDP;
  fieldsValues[2].classp = JAVA_LANG_BYTE_CLASSP;      fieldsValues[2].fieldp = JAVA_LANG_BYTE_CLASS_TYPE_FIELDP;
  fieldsValues[3].classp = JAVA_LANG_SHORT_CLASSP;     fieldsValues[3].fieldp = JAVA_LANG_SHORT_CLASS_TYPE_FIELDP;
  fieldsValues[4].classp = JAVA_LANG_INTEGER_CLASSP;   fieldsValues[4].fieldp = JAVA_LANG_INTEGER_CLASS_TYPE_FIELDP;
  fieldsValues[5].classp = JAVA_LANG_LONG_CLASSP;      fieldsValues[5].fieldp = JAVA_LANG_LONG_CLASS_TYPE_FIELDP;
  fieldsValues[6].classp = JAVA_LANG_FLOAT_CLASSP;     fieldsValues[6].fieldp = JAVA_LANG_FLOAT_CLASS_TYPE_FIELDP;
  fieldsValues[7].classp = JAVA_LANG_DOUBLE_CLASSP;    fieldsValues[7].fieldp = JAVA_LANG_DOUBLE_CLASS_TYPE_FIELDP;

  floatPositiveInfinity = (*envp)->GetStaticFloatField(envp, JAVA_LANG_FLOAT_CLASSP, JAVA_LANG_FLOAT_CLASS_TYPE_POSITIVE_INFINITY);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  floatNegativeInfinity = (*envp)->GetStaticFloatField(envp, JAVA_LANG_FLOAT_CLASSP, JAVA_LANG_FLOAT_CLASS_TYPE_NEGATIVE_INFINITY);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  floatNaN = (*envp)->GetStaticFloatField(envp, JAVA_LANG_FLOAT_CLASSP, JAVA_LANG_FLOAT_CLASS_TYPE_NAN);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  for (i = 0; i < sizeof(fieldsValues)/sizeof(fieldsValues[0]); i++) {
    classp = (*envp)->GetStaticObjectField(envp, fieldsValues[i].classp, fieldsValues[i].fieldp);
    if (classp == NULL) {
      RAISEEXCEPTION(envp, "GetStaticObjectField failure");
    }
    *(fieldsValues[i].primitiveClasspp) = (*envp)->NewGlobalRef(envp, classp);
    if (*(fieldsValues[i].primitiveClasspp) == NULL) {
      RAISEEXCEPTION(envp, "NewGlobalRef failure");
    }
    (*envp)->DeleteLocalRef(envp, classp);
  }

  /* Get endianness */
  is_bigendian = MARPAESLIF_IS_BIGENDIAN();

  rci = MARPAESLIF_JNI_VERSION;
  goto done;

 err:
  rci = JNI_ERR;

 done:
  if (envp != NULL) {
    if (classp != NULL) {
      (*envp)->DeleteLocalRef(envp, classp);
    }
    if (stringp != NULL) {
      (*envp)->DeleteLocalRef(envp, stringp);
    }
  }
  return rci;
}

/*****************************************************************************/
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM *vmp, void* reservedp)
/*****************************************************************************/
{
  static const char      *funcs = "JNI_OnUnLoad";
  JNIEnv                 *envp;
  marpaESLIFClassCache_t *marpaESLIFClassCachep  = marpaESLIFClassCacheArrayp;
  size_t                  i;

  /* ------------------------------------------------- */
  /* It was safe to store JavaVMp in a global variable */
  /* ------------------------------------------------- */
  if (((*vmp)->GetEnv(vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    RAISEEXCEPTION(envp, "Failed to get environment");
  }

  /* ---------------------------------------------- */
  /* Remove global references created by JNI_OnLoad */
  /* -----------------------------------------------*/
  while (marpaESLIFClassCachep->classs != NULL) {
    if (marpaESLIFClassCachep->classp != NULL) {
      (*envp)->DeleteGlobalRef(envp, marpaESLIFClassCachep->classp);
    }
    marpaESLIFClassCachep++;
  }
  if (marpaESLIF_UTF8p != NULL) {
    (*envp)->DeleteGlobalRef(envp, (jobject) marpaESLIF_UTF8p);
  }

  for (i = 0; i < sizeof(fieldsValues)/sizeof(fieldsValues[0]); i++) {
    if (*(fieldsValues[i].primitiveClasspp) != NULL) {
      (*envp)->DeleteGlobalRef(envp, *(fieldsValues[i].primitiveClasspp));
    }
  }

 err:
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIF_jniNew(JNIEnv *envp, jobject eslifp, jint indice)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIF_jniNew";
  genericLoggerContext_t *genericLoggerContextp = NULL;
  genericLogger_t        *genericLoggerp                  = NULL;
  marpaESLIF_t           *marpaESLIFp                     = NULL;
  marpaESLIFOption_t     marpaESLIFOption;
  jobject                BYTEBUFFER(genericLoggerContext);
  jobject                BYTEBUFFER(genericLogger);
  jobject                BYTEBUFFER(marpaESLIF);

  /* ------------------------------------- */
  /* Create and save genericLogger context */
  /* ------------------------------------- */
  genericLoggerContextp = (genericLoggerContext_t *) malloc(sizeof(genericLoggerContext_t));
  if (genericLoggerContextp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }

  genericLoggerContextp->indice              = indice;

  MARPAESLIF_PTR2BYTEBUFFER(genericLoggerContext, genericLoggerContextp);
 (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setGenericLoggerContextp_METHODP, BYTEBUFFER(genericLoggerContext));
 if (HAVEEXCEPTION(envp)) {
   goto err;
 }

 /* ------------------------------ */
 /* Create and save generic logger */
 /* ------------------------------ */
  genericLoggerp = genericLogger_newp(genericLoggerCallbackv, (void *) genericLoggerContextp, GENERICLOGGER_LOGLEVEL_TRACE);
  if (genericLoggerp == NULL) {
    RAISEEXCEPTIONF(envp, "genericLogger_newp failure, %s", strerror(errno));
  }
  MARPAESLIF_PTR2BYTEBUFFER(genericLogger, genericLoggerp);
  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setGenericLoggerp_METHODP, BYTEBUFFER(genericLogger));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  /* --------------------------- */
  /* Create and save marpaESLIFp */
  /* --------------------------- */
  marpaESLIFOption.genericLoggerp = genericLoggerp;
  marpaESLIFp = marpaESLIF_newp(&marpaESLIFOption);
  if (marpaESLIFp == NULL) {
    RAISEEXCEPTIONF(envp, "marpaESLIF_newp failure, %s", strerror(errno));
  }
  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIF, marpaESLIFp);
  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setMarpaESLIFp_METHODP, BYTEBUFFER(marpaESLIF));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  goto done;
  
 err:
  /* Java_org_parser_marpa_ESLIF_jniFree(envp, eslifp); */

 done:
  return;
}

/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIF_jniVersion(JNIEnv *envp, jobject eslifp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIF_jniVersion";
  marpaESLIF_t      *marpaESLIFp;
  char              *versions;

  if (ESLIF_contextb(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP,
                       NULL /* genericLoggerpp */,
                       &marpaESLIFp)) {
    if (! marpaESLIF_versionb(marpaESLIFp, &versions)) {
      RAISEEXCEPTIONF(envp, "marpaESLIF_versionb failure, %s", strerror(errno));
    }

    return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, versions);
  }

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jint JNICALL Java_org_parser_marpa_ESLIF_jniVersionMajor(JNIEnv *envp, jobject eslifp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIF_jniVersionMajor";
  marpaESLIF_t      *marpaESLIFp;
  int               majori;

  if (ESLIF_contextb(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP,
                       NULL /* genericLoggerpp */,
                       &marpaESLIFp)) {
    if (! marpaESLIF_versionMajorb(marpaESLIFp, &majori)) {
      RAISEEXCEPTIONF(envp, "marpaESLIF_versionMajorb failure, %s", strerror(errno));
    }

    return (jint) majori;
  }

 err:
  return 0;
}

/*****************************************************************************/
JNIEXPORT jint JNICALL Java_org_parser_marpa_ESLIF_jniVersionMinor(JNIEnv *envp, jobject eslifp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIF_jniVersionMinor";
  marpaESLIF_t      *marpaESLIFp;
  int               minori;

  if (ESLIF_contextb(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP,
                       NULL /* genericLoggerpp */,
                       &marpaESLIFp)) {
    if (! marpaESLIF_versionMinorb(marpaESLIFp, &minori)) {
      RAISEEXCEPTIONF(envp, "marpaESLIF_versionMinorb failure, %s", strerror(errno));
    }

    return (jint) minori;
  }

 err:
  return 0;
}

/*****************************************************************************/
JNIEXPORT jint JNICALL Java_org_parser_marpa_ESLIF_jniVersionPatch(JNIEnv *envp, jobject eslifp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIF_jniVersionPatch";
  marpaESLIF_t      *marpaESLIFp;
  int               patchi;

  if (ESLIF_contextb(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP,
                       NULL /* genericLoggerpp */,
                       &marpaESLIFp)) {
    if (! marpaESLIF_versionPatchb(marpaESLIFp, &patchi)) {
      RAISEEXCEPTIONF(envp, "marpaESLIF_versionPatchb failure, %s", strerror(errno));
    }

    return (jint) patchi;
  }

 err:
  return 0;
}

/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIF_jniJavaVersion(JNIEnv *envp, jclass classp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIF_jniJavaVersion";

  return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, MARPAESLIFJAVA_VERSION);
}

/*****************************************************************************/
JNIEXPORT jint JNICALL Java_org_parser_marpa_ESLIF_jniJavaVersionMajor(JNIEnv *envp, jclass classp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIF_jniJavaVersionMajor";

  return (jint) MARPAESLIFJAVA_VERSION_MAJOR;
}

/*****************************************************************************/
JNIEXPORT jint JNICALL Java_org_parser_marpa_ESLIF_jniJavaVersionMinor(JNIEnv *envp, jclass classp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIF_jniJavaVersionMinor";

  return (jint) MARPAESLIFJAVA_VERSION_MINOR;
}

/*****************************************************************************/
JNIEXPORT jint JNICALL Java_org_parser_marpa_ESLIF_jniJavaVersionPatch(JNIEnv *envp, jclass classp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIF_jniJavaVersionPatch";

  return (jint) MARPAESLIFJAVA_VERSION_PATCH;
}

/*
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIF_jniFree(JNIEnv *envp, jobject eslifp)
{
  genericLoggerContext_t *genericLoggerContextp;
  genericLogger_t        *genericLoggerp;
  marpaESLIF_t           *marpaESLIFp;

  if (! ESLIF_contextb(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP,
                       &genericLoggerp,
                       &genericLoggerContextp,
                       &marpaESLIFp)) {
    goto done;
  }

  marpaESLIF_freev(marpaESLIFp);
  genericLogger_freev(&genericLoggerp);
  if (genericLoggerContextp != NULL) {
    free(genericLoggerContextp);
  }

  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setGenericLoggerContextp_METHODP, NULL);
  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setGenericLoggerp_METHODP, NULL);
  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setMarpaESLIFp_METHODP, NULL);

 done:
  return;
}
*/

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFGrammar_jniNew(JNIEnv *envp, jobject eslifGrammarp, jbyteArray utf8byteArrayp)
/*****************************************************************************/
{
  static const char                *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniNew";
  jbyte                            *utf8bytep = NULL;
  marpaESLIFGrammar_t              *marpaESLIFGrammarp;
  marpaESLIF_t                     *marpaESLIFp;
  marpaESLIFGrammarOption_t         marpaESLIFGrammarOption;
  jobject                           BYTEBUFFER(marpaESLIFGrammar);
  jsize                             utf8lengthl;

  /* Always update genericLogger context */
  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, &marpaESLIFp, NULL /* marpaESLIFGrammarpp */)) {
    goto err;
  }

  /* Get information from the stack */
  if (utf8byteArrayp == NULL) {
    RAISEEXCEPTION(envp, "ByteArray argument is NULL");
  }
  utf8lengthl = (*envp)->GetArrayLength(envp, utf8byteArrayp);
  if (utf8lengthl <= 0) {
    RAISEEXCEPTION(envp, "ByteArray is empty");
  }
  utf8bytep = (jbyte *) malloc(utf8lengthl * sizeof(jbyte));
  if (utf8bytep == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  (*envp)->GetByteArrayRegion(envp, utf8byteArrayp, 0, utf8lengthl, utf8bytep);

  /* Create C object */
  marpaESLIFGrammarOption.bytep               = (void *) utf8bytep;
  marpaESLIFGrammarOption.bytel               = (size_t) utf8lengthl;
  marpaESLIFGrammarOption.encodings           = "UTF-8";
  marpaESLIFGrammarOption.encodingl           = strlen("UTF-8");

  marpaESLIFGrammarp = marpaESLIFGrammar_newp(marpaESLIFp, &marpaESLIFGrammarOption);
  if (marpaESLIFGrammarp == NULL) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_newp failure, %s", strerror(errno));
  }

  /* Store the object */
  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIFGrammar, marpaESLIFGrammarp);  
  (*envp)->CallVoidMethod(envp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_setMarpaESLIFGrammarp_METHODP, BYTEBUFFER(marpaESLIFGrammar));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  goto done;
  
 err:
  Java_org_parser_marpa_ESLIFGrammar_jniFree(envp, eslifGrammarp);

 done:
  if (utf8bytep != NULL) {
    free(utf8bytep);
  }
  return;
}

/*****************************************************************************/
JNIEXPORT jint JNICALL Java_org_parser_marpa_ESLIFGrammar_jniNgrammar(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniNGrammar";
  int                  ngrammari;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ngrammarib(marpaESLIFGrammarp, &ngrammari)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_ngrammarib failure");
  }

  goto done;

 err:
  ngrammari = -1;

 done:
  return (jint) ngrammari;
}

/*****************************************************************************/
JNIEXPORT jint JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentLevel(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniCurrentLevel";
  int                  leveli;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammar_currentb(marpaESLIFGrammarp, &leveli, NULL)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_grammar_currentb failure");
  }

  goto done;

 err:
  leveli = -1;

 done:
  return (jint) leveli;
}

/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentDescription(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniCurrentDescription";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  marpaESLIFString_t  *descp;
  jstring              stringp;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammar_currentb(marpaESLIFGrammarp, NULL, &descp)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_grammar_currentb failure");
  }

  stringp = marpaESLIFJava_marpaESLIFStringToJavap(envp, descp);
  goto done;

 err:
  stringp = NULL;

 done:
  return stringp;
}

/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIFGrammar_jniDescriptionByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniDescriptionByLevel";
  int                  leveli = (jint) level;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  marpaESLIFString_t  *descp;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammar_by_levelb(marpaESLIFGrammarp, leveli, NULL, NULL, &descp)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_grammar_by_levelb failure for level %d", leveli);
  }

  return marpaESLIFJava_marpaESLIFStringToJavap(envp, descp);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentRuleIds(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniCurrentRuleIds";
  jintArray            intArray = NULL;
  jint                *intp = NULL;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                 *ruleip;
  size_t               rulel;
  size_t               i;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_rulearray_currentb(marpaESLIFGrammarp, &ruleip, &rulel)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_rulearray_currentb failure");
  }
  if (rulel <= 0) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_rulearray_currentb returned no rule");
  }
  intp = (jint *) malloc(sizeof(jint) * rulel);
  if (intp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  for (i = 0; i < rulel; i++) {
    intp[i] = (jint) ruleip[i];
  }

  intArray = (*envp)->NewIntArray(envp, (jsize) rulel);
  if (intArray == NULL) {
    RAISEEXCEPTION(envp, "NewIntArray failure");
  }

  /* If it fails the exception will be seen by the Java layer */
  (*envp)->SetIntArrayRegion(envp, intArray, 0, (jsize) rulel, intp);
  goto done;

 err:
  if (intArray != NULL) {
    (*envp)->DeleteLocalRef(envp, intArray);
    intArray = NULL;
  }

 done:
  if (intp != NULL) {
    free(intp);
  }
  return intArray;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentSymbolIds(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniCurrentSymbolIds";
  jintArray            intArray = NULL;
  jint                *intp = NULL;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                 *symbolip;
  size_t               symboll;
  size_t               i;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_symbolarray_currentb(marpaESLIFGrammarp, &symbolip, &symboll)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_symbolarray_currentb failure");
  }
  if (symboll <= 0) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_symbolarray_currentb returned no symbol");
  }
  intp = (jint *) malloc(sizeof(jint) * symboll);
  if (intp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  for (i = 0; i < symboll; i++) {
    intp[i] = (jint) symbolip[i];
  }

  intArray = (*envp)->NewIntArray(envp, (jsize) symboll);
  if (intArray == NULL) {
    RAISEEXCEPTION(envp, "NewIntArray failure");
  }

  /* If it fails the exception will be seen by the Java layer */
  (*envp)->SetIntArrayRegion(envp, intArray, 0, (jsize) symboll, intp);
  goto done;

 err:
  if (intArray != NULL) {
    (*envp)->DeleteLocalRef(envp, intArray);
    intArray = NULL;
  }

 done:
  if (intp != NULL) {
    free(intp);
  }
  return intArray;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleIdsByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniRuleIdsByLevel";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  jintArray            intArray;
  jint                *intp = NULL;
  int                 *ruleip;
  size_t               rulel;
  size_t               i;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_rulearray_by_levelb(marpaESLIFGrammarp, &ruleip, &rulel, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_rulearray_by_levelb failure for level %d", leveli);
  }
  if (rulel <= 0) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_rulearray_currentb returned no rule for level %d", leveli);
  }
  intp = (jint *) malloc(sizeof(jint) * rulel);
  if (intp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  for (i = 0; i < rulel; i++) {
    intp[i] = (jint) ruleip[i];
  }

  intArray = (*envp)->NewIntArray(envp, (jsize) rulel);
  if (intArray == NULL) {
    RAISEEXCEPTION(envp, "NewIntArray failure");
  }

  /* If it fails the exception will be seen by the Java layer */
  (*envp)->SetIntArrayRegion(envp, intArray, 0, (jsize) rulel, intp);
  goto done;

 err:
  if (intArray != NULL) {
    (*envp)->DeleteLocalRef(envp, intArray);
    intArray = NULL;
  }

 done:
  if (intp != NULL) {
    free(intp);
  }
  return intArray;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolIdsByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniSymbolIdsByLevel";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  jintArray            intArray;
  jint                *intp = NULL;
  int                 *symbolip;
  size_t               symboll;
  size_t               i;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_symbolarray_by_levelb(marpaESLIFGrammarp, &symbolip, &symboll, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_symbolarray_by_levelb failure for level %d", leveli);
  }
  if (symboll <= 0) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_symbolarray_currentb returned no symbol for level %d", leveli);
  }
  intp = (jint *) malloc(sizeof(jint) * symboll);
  if (intp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  for (i = 0; i < symboll; i++) {
    intp[i] = (jint) symbolip[i];
  }

  intArray = (*envp)->NewIntArray(envp, (jsize) symboll);
  if (intArray == NULL) {
    RAISEEXCEPTION(envp, "NewIntArray failure");
  }

  /* If it fails the exception will be seen by the Java layer */
  (*envp)->SetIntArrayRegion(envp, intArray, 0, (jsize) symboll, intp);
  goto done;

 err:
  if (intArray != NULL) {
    (*envp)->DeleteLocalRef(envp, intArray);
    intArray = NULL;
  }

 done:
  if (intp != NULL) {
    free(intp);
  }
  return intArray;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplay(JNIEnv *envp, jobject eslifGrammarp, jint rule)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplay";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  rulei = (int) rule;
  char                *ruledisplays;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruledisplayform_currentb(marpaESLIFGrammarp, rulei, &ruledisplays)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_ruledisplayform_currentb failure for rule %d", rulei);
  }

  return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, ruledisplays);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolDisplay(JNIEnv *envp, jobject eslifGrammarp, jint symbol)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniSymbolDisplay";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  symboli = (int) symbol;
  char                *symboldisplays;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_symboldisplayform_currentb(marpaESLIFGrammarp, symboli, &symboldisplays)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_symboldisplayform_currentb failure for symbol %d", symboli);
  }

  return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, symboldisplays);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleShow(JNIEnv *envp, jobject eslifGrammarp, jint rule)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniRuleShow";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  rulei = (int) rule;
  char                *ruleshows;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruleshowform_currentb(marpaESLIFGrammarp, rulei, &ruleshows)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_ruleshowform_currentb failure for rule %d", rulei);
  }

  return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, ruleshows);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplayByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplayByLevel";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  int                  rulei = (int) rule;
  char                *ruledisplays;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruledisplayform_by_levelb(marpaESLIFGrammarp, rulei, &ruledisplays, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_ruledisplayform_currentb failure for rule %d and level %d", rulei, leveli);
  }

  return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, ruledisplays);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolDisplayByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level, jint symbol)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniSymbolDisplayByLevel";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  int                  symboli = (int) symbol;
  char                *symboldisplays;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_symboldisplayform_by_levelb(marpaESLIFGrammarp, symboli, &symboldisplays, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_symboldisplayform_currentb failure for symbol %d and level %d", symboli, leveli);
  }

  return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, symboldisplays);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleShowByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniRuleShowByLevel";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  int                  rulei = (int) rule;
  char                *ruleshows;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruleshowform_by_levelb(marpaESLIFGrammarp, rulei, &ruleshows, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_ruleshowform_by_levelb failure for rule %d and level %d", rulei, leveli);
  }

  return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, ruleshows);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIFGrammar_jniShow(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniShow";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  char                *shows;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammarshowform_currentb(marpaESLIFGrammarp, &shows)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_grammarshowform_currentb failure");
  }

  return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, shows);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIFGrammar_jniShowByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level)
/*****************************************************************************/
{
  static const char   *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniShowByLevel";
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  char                *shows;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammarshowform_by_levelb(marpaESLIFGrammarp, &shows, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_grammarshowform_by_levelb for level %d", leveli);
  }

  return marpaESLIFJava_marpaESLIFASCIIToJavap(envp, shows);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFGrammar_jniParse(JNIEnv *envp, jobject eslifGrammarp, jobject eslifRecognizerInterfacep, jobject eslifValueInterfacep)
/*****************************************************************************/
{
  static const char             *funcs                  = "Java_org_parser_marpa_ESLIFGrammar_jniParse";
  marpaESLIFValueResult_t       *marpaESLIFValueResultp = NULL;
  marpaESLIFGrammar_t           *marpaESLIFGrammarp;
  marpaESLIFRecognizerOption_t   marpaESLIFRecognizerOption;
  marpaESLIFValueOption_t        marpaESLIFValueOption;
  marpaESLIFRecognizerContext_t  marpaESLIFRecognizerContext;
  marpaESLIFValueContext_t       marpaESLIFValueContext;
  jobject                        objectp;
  jboolean                       rcb;

  if (! marpaESLIFJava_recognizerContextInitb(envp, eslifRecognizerInterfacep, &marpaESLIFRecognizerContext, 0 /* haveLexemeStackb */)) {
    goto err;
  }

  marpaESLIFRecognizerOption.userDatavp                = &marpaESLIFRecognizerContext;
  marpaESLIFRecognizerOption.readerCallbackp           = readerCallbackb;
  marpaESLIFRecognizerOption.disableThresholdb         = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithDisableThreshold_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.exhaustedb                = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithExhaustion_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.newlineb                  = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithNewline_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.trackb                    = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithTrack_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.bufsizl                   = 0; /* Recommended value */
  marpaESLIFRecognizerOption.buftriggerperci           = 50; /* Recommended value */
  marpaESLIFRecognizerOption.bufaddperci               = 50; /* Recommended value */
  marpaESLIFRecognizerOption.ifActionResolverp         = marpaESLIFRecognizerIfActionResolver;
  marpaESLIFRecognizerOption.eventActionResolverp      = marpaESLIFRecognizerEventActionResolver;
  marpaESLIFRecognizerOption.regexActionResolverp      = marpaESLIFRecognizerRegexActionResolver;
  marpaESLIFRecognizerOption.importerp                 = marpaESLIFJava_recognizerImportb;

  if (! marpaESLIFJava_valueContextInitb(envp, eslifValueInterfacep, eslifGrammarp, &marpaESLIFValueContext)) {
    goto err;
  }
  
  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  marpaESLIFValueOption.userDatavp                     = &marpaESLIFValueContext;
  marpaESLIFValueOption.ruleActionResolverp            = marpaESLIFValueRuleActionResolver;
  marpaESLIFValueOption.symbolActionResolverp          = marpaESLIFValueSymbolActionResolver;
  marpaESLIFValueOption.importerp                      = marpaESLIFJava_valueImportb;
  marpaESLIFValueOption.highRankOnlyb                  = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithHighRankOnly_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFValueOption.orderByRankb                   = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithOrderByRank_METHODP)  == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFValueOption.ambiguousb                     = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithAmbiguous_METHODP)    == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFValueOption.nullb                          = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithNull_METHODP)         == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFValueOption.maxParsesi                     = (*envp)->CallIntMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_maxParses_METHODP);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_parseb(marpaESLIFGrammarp, &marpaESLIFRecognizerOption, &marpaESLIFValueOption, NULL)) {
    goto err;
  }
  if (GENERICSTACK_USED(marpaESLIFValueContext.objectStackp) != 1) {
    RAISEEXCEPTIONF(envp, "Internal value stack is %d instead of 1", GENERICSTACK_USED(marpaESLIFValueContext.objectStackp));
  }
  objectp = (jobject) GENERICSTACK_POP_PTR(marpaESLIFValueContext.objectStackp);

  (*envp)->CallVoidMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setResult_METHODP, objectp);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  rcb = JNI_TRUE;
  goto done;

 err:
  rcb = JNI_FALSE;

 done:
  marpaESLIFJava_valueContextFreev(envp, &marpaESLIFValueContext, 1 /* onStackb */);
  marpaESLIFJava_recognizerContextFreev(envp, &marpaESLIFRecognizerContext, 1 /* onStackb */);
  return rcb;
}

/*****************************************************************************/
JNIEXPORT jobject JNICALL Java_org_parser_marpa_ESLIFGrammar_jniProperties(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  static const char           *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniProperties";
  marpaESLIFGrammarProperty_t  grammarProperty;
  marpaESLIFGrammar_t         *marpaESLIFGrammarp;
  jobject                      propertiesp = NULL;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammarproperty_currentb(marpaESLIFGrammarp, &grammarProperty)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_grammarproperty_currentb failure");
  }

  propertiesp = marpaESLIFJava_grammarPropertiesp(envp, &grammarProperty);
  goto done;

 err:
  propertiesp = NULL;

 done:
  return propertiesp;
}

/*****************************************************************************/
JNIEXPORT jobject JNICALL Java_org_parser_marpa_ESLIFGrammar_jniPropertiesByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level)
/*****************************************************************************/
{
  static const char           *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniProperties";
  marpaESLIFGrammarProperty_t  grammarProperty;
  marpaESLIFGrammar_t         *marpaESLIFGrammarp;
  jobject                      propertiesp = NULL;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammarproperty_by_levelb(marpaESLIFGrammarp, &grammarProperty, (int) level, NULL /* descp */)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_grammarproperty_by_levelb failure");
  }

  propertiesp = marpaESLIFJava_grammarPropertiesp(envp, &grammarProperty);
  goto done;

 err:
  propertiesp = NULL;

 done:
  return propertiesp;
}

/*****************************************************************************/
JNIEXPORT jobject JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleProperties(JNIEnv *envp, jobject eslifGrammarp, jint rule)
/*****************************************************************************/
{
  static const char        *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniRuleProperties";
  marpaESLIFRuleProperty_t  ruleProperty;
  marpaESLIFGrammar_t      *marpaESLIFGrammarp;
  jobject                   propertiesp = NULL;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruleproperty_currentb(marpaESLIFGrammarp, (int) rule, &ruleProperty)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_ruleproperty_currentb failure");
  }

  propertiesp = marpaESLIFJava_rulePropertiesp(envp, &ruleProperty);
  goto done;

 err:
  propertiesp = NULL;

 done:
  return propertiesp;
}

/*****************************************************************************/
JNIEXPORT jobject JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRulePropertiesByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule)
/*****************************************************************************/
{
  static const char        *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniRulePropertiesByLevel";
  marpaESLIFRuleProperty_t  ruleProperty;
  marpaESLIFGrammar_t      *marpaESLIFGrammarp;
  jobject                   propertiesp = NULL;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruleproperty_by_levelb(marpaESLIFGrammarp, (int) rule, &ruleProperty, (int) level, NULL /* descp */)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_ruleproperty_by_levelb failure");
  }

  propertiesp = marpaESLIFJava_rulePropertiesp(envp, &ruleProperty);
  goto done;

 err:
  propertiesp = NULL;

 done:
  return propertiesp;
}

/*****************************************************************************/
JNIEXPORT jobject JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolProperties(JNIEnv *envp, jobject eslifGrammarp, jint symbol)
/*****************************************************************************/
{
  static const char          *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniSymbolProperties";
  marpaESLIFSymbolProperty_t  symbolProperty;
  marpaESLIFGrammar_t        *marpaESLIFGrammarp;
  jobject                     propertiesp = NULL;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_symbolproperty_currentb(marpaESLIFGrammarp, (int) symbol, &symbolProperty)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_symbolproperty_currentb failure");
  }

  propertiesp = marpaESLIFJava_symbolPropertiesp(envp, &symbolProperty);
  goto done;

 err:
  propertiesp = NULL;

 done:
  return propertiesp;
}

/*****************************************************************************/
JNIEXPORT jobject JNICALL Java_org_parser_marpa_ESLIFGrammar_jniSymbolPropertiesByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level, jint symbol)
/*****************************************************************************/
{
  static const char          *funcs = "Java_org_parser_marpa_ESLIFGrammar_jniSymbolPropertiesByLevel";
  marpaESLIFSymbolProperty_t  symbolProperty;
  marpaESLIFGrammar_t        *marpaESLIFGrammarp;
  jobject                     propertiesp = NULL;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_symbolproperty_by_levelb(marpaESLIFGrammarp, (int) symbol, &symbolProperty, (int) level, NULL /* descp */)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_symbolproperty_by_levelb failure");
  }

  propertiesp = marpaESLIFJava_symbolPropertiesp(envp, &symbolProperty);
  goto done;

 err:
  propertiesp = NULL;

 done:
  return propertiesp;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFGrammar_jniFree(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFGrammarp;

  if (ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    marpaESLIFGrammar_freev(marpaESLIFGrammarp); /* This is NULL protected */
  }
}

/*****************************************************************************/
static void genericLoggerCallbackv(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs)
/*****************************************************************************/
{
  genericLoggerContext_t *genericLoggerContextp = (genericLoggerContext_t *) userDatavp;
  JNIEnv                 *envp;
  jobject                 loggerInterfacep;
  jmethodID               methodp;
  jstring                 stringp;

  if (genericLoggerContextp == NULL) {
    /* Impossible IMHO */
    return;
  }

  /* Logging callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    return;
  }

  loggerInterfacep = (*envp)->CallStaticObjectMethod(envp, MARPAESLIF_ESLIFEVENTTYPE_CLASSP, MARPAESLIF_ESLIF_CLASS_getLoggerInterfaceByIndicep_METHODP, genericLoggerContextp->indice);
  if (loggerInterfacep == NULL) {
    return;
  }

  switch (logLeveli) {
  case GENERICLOGGER_LOGLEVEL_TRACE:
    methodp = MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_trace_METHODP;
    break;
  case GENERICLOGGER_LOGLEVEL_DEBUG:
    methodp = MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_debug_METHODP;
    break;
  case GENERICLOGGER_LOGLEVEL_INFO:
    methodp = MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_info_METHODP;
    break;
  case GENERICLOGGER_LOGLEVEL_NOTICE:
    methodp = MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_notice_METHODP;
    break;
  case GENERICLOGGER_LOGLEVEL_WARNING:
    methodp = MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_warning_METHODP;
    break;
  case GENERICLOGGER_LOGLEVEL_ERROR:
    methodp = MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_error_METHODP;
    break;
  case GENERICLOGGER_LOGLEVEL_CRITICAL:
    methodp = MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_critical_METHODP;
    break;
  case GENERICLOGGER_LOGLEVEL_ALERT:
    methodp = MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_alert_METHODP;
    break;
  case GENERICLOGGER_LOGLEVEL_EMERGENCY:
    methodp = MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_emergency_METHODP;
    break;
  default:
    methodp = NULL;
    break;
  }

  if (methodp != NULL) {
    stringp = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, (char *) msgs);
    if (stringp != NULL) {
      (*envp)->CallVoidMethod(envp, loggerInterfacep, methodp, stringp);
      (*envp)->DeleteLocalRef(envp, stringp);
    }
  }
}

/*****************************************************************************/
static void generateStringWithLoggerCallbackv(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs)
/*****************************************************************************/
{
  marpaESLIFJava_stringGeneratorContext_t *contextp = (marpaESLIFJava_stringGeneratorContext_t *) userDatavp;
  char                                    *tmps;

  if (contextp->s == NULL) {
    /* First time */
    contextp->l = strlen(msgs) + 1;
    contextp->s = strdup(msgs);
    if (contextp->s == NULL) {
      contextp->l = 0;
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
    }
  }
}

/*****************************************************************************/
static short ESLIF_contextb(JNIEnv *envp, jobject eslifp, jmethodID getLoggerInterfacep, genericLogger_t **genericLoggerpp, marpaESLIF_t **marpaESLIFpp)
/*****************************************************************************/
{
  static const char      *funcs = "ESLIF_contextb";
  genericLoggerContext_t *genericLoggerContextp = NULL;
  marpaESLIF_t           *marpaESLIFp;
  genericLogger_t        *genericLoggerp;
  jobject                 BYTEBUFFER(genericLoggerContext);
  jobject                 BYTEBUFFER(marpaESLIF);
  jobject                 BYTEBUFFER(genericLogger);

  if (marpaESLIFpp != NULL) {
    /* If caller is giving marpaESLIFpp we expect bytebuffer and more to not be NULL */
    BYTEBUFFER(marpaESLIF) = (*envp)->CallObjectMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getMarpaESLIFp_METHODP);
    if (BYTEBUFFER(marpaESLIF) == NULL) {
      RAISEEXCEPTION(envp, "eslif bytebuffer is NULL");
    }
    marpaESLIFp = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(marpaESLIF));
    if (marpaESLIFp == NULL) {
      RAISEEXCEPTION(envp, "marpaESLIFp is NULL");
    }
    *marpaESLIFpp = marpaESLIFp;
  }

  if (genericLoggerpp != NULL) {
    /* Ditto for genericLogger context */
    BYTEBUFFER(genericLogger) = (*envp)->CallObjectMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getGenericLoggerp_METHODP);
    if (BYTEBUFFER(genericLogger) == NULL) {
      RAISEEXCEPTION(envp, "genericLogger bytebuffer is NULL");
    }
    genericLoggerp = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(genericLogger));
    if (genericLoggerp == NULL) {
      RAISEEXCEPTION(envp, "genericLoggerp is NULL");
    }
    *genericLoggerpp = genericLoggerp;
  }

  /* genericLogger context should always be updated */
  /* Ditto for genericLoggerContext */
  BYTEBUFFER(genericLoggerContext) = (*envp)->CallObjectMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getGenericLoggerContextp_METHODP);
  if (BYTEBUFFER(genericLoggerContext) == NULL) {
    RAISEEXCEPTION(envp, "genericLoggerContext bytebuffer is NULL");
  }
  genericLoggerContextp = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(genericLoggerContext));
  if (genericLoggerContextp == NULL) {
    RAISEEXCEPTION(envp, "genericLoggerContextp is NULL");
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short ESLIFGrammar_contextb(JNIEnv *envp, jobject eslifGrammarp, marpaESLIF_t **marpaESLIFpp, marpaESLIFGrammar_t **marpaESLIFGrammarpp)
/*****************************************************************************/
{
  static const char   *funcs = "ESLIFGrammar_contextb";
  jobject              BYTEBUFFER(marpaESLIFGrammar);
  jobject              eslifp;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
    
  if (marpaESLIFGrammarpp != NULL) {
   /* If caller is giving marpaESLIFGrammarpp we expect bytebuffer and more to not be NULL */
    BYTEBUFFER(marpaESLIFGrammar) = (*envp)->CallObjectMethod(envp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getMarpaESLIFGrammarp_METHODP);
    if (BYTEBUFFER(marpaESLIFGrammar) == NULL) {
      RAISEEXCEPTION(envp, "marpaESLIFGrammar bytebuffer is NULL");
    }
    marpaESLIFGrammarp = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(marpaESLIFGrammar));
    if (marpaESLIFGrammarp == NULL) {
      RAISEEXCEPTION(envp, "marpaESLIFGrammarp is NULL");
    }
    *marpaESLIFGrammarpp = marpaESLIFGrammarp;
  }

  eslifp = (*envp)->CallObjectMethod(envp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getEslif_METHODP);
  if (eslifp == NULL) {
    RAISEEXCEPTION(envp, "eslifp is NULL");
  }

  if (! ESLIF_contextb(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP, NULL, marpaESLIFpp)) {
    goto err;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short ESLIFSymbol_contextb(JNIEnv *envp, jobject eslifSymbolp, marpaESLIF_t **marpaESLIFpp, marpaESLIFSymbol_t **marpaESLIFSymbolpp)
/*****************************************************************************/
{
  static const char   *funcs = "ESLIFSymbol_contextb";
  jobject              BYTEBUFFER(marpaESLIFSymbol);
  jobject              eslifp;
  marpaESLIFSymbol_t *marpaESLIFSymbolp;
    
  if (marpaESLIFSymbolpp != NULL) {
   /* If caller is giving marpaESLIFSymbolpp we expect bytebuffer and more to not be NULL */
    BYTEBUFFER(marpaESLIFSymbol) = (*envp)->CallObjectMethod(envp, eslifSymbolp, MARPAESLIF_ESLIFSYMBOL_CLASS_getMarpaESLIFSymbolp_METHODP);
    if (BYTEBUFFER(marpaESLIFSymbol) == NULL) {
      RAISEEXCEPTION(envp, "marpaESLIFSymbol bytebuffer is NULL");
    }
    marpaESLIFSymbolp = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(marpaESLIFSymbol));
    if (marpaESLIFSymbolp == NULL) {
      RAISEEXCEPTION(envp, "marpaESLIFSymbolp is NULL");
    }
    *marpaESLIFSymbolpp = marpaESLIFSymbolp;
  }

  eslifp = (*envp)->CallObjectMethod(envp, eslifSymbolp, MARPAESLIF_ESLIFSYMBOL_CLASS_getEslif_METHODP);
  if (eslifp == NULL) {
    RAISEEXCEPTION(envp, "eslifp is NULL");
  }

  if (! ESLIF_contextb(envp, eslifp, MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP, NULL, marpaESLIFpp)) {
    goto err;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short ESLIFRecognizer_contextb(JNIEnv *envp, jobject eslifRecognizerp, marpaESLIFRecognizer_t **marpaESLIFRecognizerpp, marpaESLIFRecognizerContext_t **marpaESLIFRecognizerContextpp)
/*****************************************************************************/
{
  static const char            *funcs = "ESLIFRecognizer_contextb";
  jobject                       BYTEBUFFER(marpaESLIFRecognizer);
  jobject                       BYTEBUFFER(marpaESLIFRecognizerContext);
  marpaESLIFRecognizer_t       *marpaESLIFRecognizerp;
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp;
    
  if (marpaESLIFRecognizerpp != NULL) {
    BYTEBUFFER(marpaESLIFRecognizer) = (*envp)->CallObjectMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getMarpaESLIFRecognizerp_METHODP);
    if (BYTEBUFFER(marpaESLIFRecognizer) == NULL) {
      RAISEEXCEPTION(envp, "marpaESLIFRecognizer bytebuffer is NULL");
    }
    marpaESLIFRecognizerp = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(marpaESLIFRecognizer));
    if (marpaESLIFRecognizerp == NULL) {
      RAISEEXCEPTION(envp, "marpaESLIFRecognizerp is NULL");
    }
    *marpaESLIFRecognizerpp = marpaESLIFRecognizerp;
  }

  /* Recognizer context content is always updated */
  BYTEBUFFER(marpaESLIFRecognizerContext) = (*envp)->CallObjectMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getMarpaESLIFRecognizerContextp_METHODP);
  if (BYTEBUFFER(marpaESLIFRecognizerContext) == NULL) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizerContext bytebuffer is NULL");
  }
  marpaESLIFRecognizerContextp = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(marpaESLIFRecognizerContext));
  if (marpaESLIFRecognizerContextp == NULL) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizerContextp is NULL");
  }
  marpaESLIFRecognizerContextp->eslifRecognizerInterfacep = (*envp)->CallObjectMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifRecognizerInterface_METHODP);
  if (marpaESLIFRecognizerContextpp != NULL) {
    *marpaESLIFRecognizerContextpp = marpaESLIFRecognizerContextp;
  }
  
  return 1;

 err:
  return 0;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniNew(JNIEnv *envp, jobject eslifRecognizerp, jobject eslifGrammarp)
/*****************************************************************************/
{
  static const char                *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniNew";
  marpaESLIFRecognizer_t           *marpaESLIFRecognizerp  = NULL;
  marpaESLIFGrammar_t              *marpaESLIFGrammarp;
  marpaESLIFRecognizerOption_t      marpaESLIFRecognizerOption;
  marpaESLIFRecognizerContext_t     *marpaESLIFRecognizerContextp;
  jobject                           eslifRecognizerInterfacep;
  jobject                           BYTEBUFFER(marpaESLIFRecognizer);
  jobject                           BYTEBUFFER(marpaESLIFRecognizerContext);

  /* Always update genericLogger context */
  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, NULL /* marpaESLIFpp */, &marpaESLIFGrammarp)) {
    /* An exception was raised */
    goto err;
  }

  /* Get recognizer interface - class is protected to not accept the null argument */
  eslifRecognizerInterfacep = (*envp)->CallObjectMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifRecognizerInterface_METHODP);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  /* Recognizer context is a structure containing the current recognizer interface object instance - updated at every recognizer call */
  marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) malloc(sizeof(marpaESLIFRecognizerContext_t));
  if (marpaESLIFRecognizerContextp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }

  if (! marpaESLIFJava_recognizerContextInitb(envp, eslifRecognizerInterfacep, marpaESLIFRecognizerContextp, 1 /* haveLexemeStackb */)) {
    goto err;
  }

  /* Create C object */
  marpaESLIFRecognizerOption.userDatavp                = marpaESLIFRecognizerContextp;
  marpaESLIFRecognizerOption.readerCallbackp           = readerCallbackb;
  marpaESLIFRecognizerOption.disableThresholdb         = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithDisableThreshold_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.exhaustedb                = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithExhaustion_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.newlineb                  = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithNewline_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.trackb                    = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithTrack_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.bufsizl                   = 0; /* Recommended value */
  marpaESLIFRecognizerOption.buftriggerperci           = 50; /* Recommended value */
  marpaESLIFRecognizerOption.bufaddperci               = 50; /* Recommended value */
  marpaESLIFRecognizerOption.ifActionResolverp         = marpaESLIFRecognizerIfActionResolver;
  marpaESLIFRecognizerOption.eventActionResolverp      = marpaESLIFRecognizerEventActionResolver;
  marpaESLIFRecognizerOption.regexActionResolverp      = marpaESLIFRecognizerRegexActionResolver;
  marpaESLIFRecognizerOption.importerp                 = marpaESLIFJava_recognizerImportb;

  marpaESLIFRecognizerp = marpaESLIFRecognizer_newp(marpaESLIFGrammarp, &marpaESLIFRecognizerOption);
  if (marpaESLIFRecognizerp == NULL) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_newp failure");
  }

  /* Store the objects */
  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIFRecognizer, marpaESLIFRecognizerp);  
  (*envp)->CallVoidMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_setMarpaESLIFRecognizerp_METHODP, BYTEBUFFER(marpaESLIFRecognizer));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIFRecognizerContext, marpaESLIFRecognizerContextp);  
  (*envp)->CallVoidMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_setMarpaESLIFRecognizerContextp_METHODP, BYTEBUFFER(marpaESLIFRecognizerContext));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  goto done;
  
 err:
  Java_org_parser_marpa_ESLIFRecognizer_jniFree(envp, eslifRecognizerp);

 done:
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniShare(JNIEnv *envp, jobject eslifRecognizerp, jobject eslifRecognizerSharedp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniShare";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  marpaESLIFRecognizer_t *marpaESLIFRecognizerSharedp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! (*envp)->IsSameObject(envp, eslifRecognizerSharedp, NULL)) {
    if (! ESLIFRecognizer_contextb(envp, eslifRecognizerSharedp, &marpaESLIFRecognizerSharedp, NULL /* marpaESLIFRecognizerContextpp */)) {
      goto err;
    }
  } else {
    marpaESLIFRecognizerSharedp = NULL;
  }

  if (! marpaESLIFRecognizer_shareb(marpaESLIFRecognizerp, marpaESLIFRecognizerSharedp)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_shareb failure");
  }

 err: /* err and done share the same code */
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniUnshare(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniUnshare";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_shareb(marpaESLIFRecognizerp, NULL)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_shareb failure");
  }

 err: /* err and done share the same code */
  return;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniScan(JNIEnv *envp, jobject eslifRecognizerp, jboolean initialEvents)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniScan";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  short                   initialEventsb = (initialEvents == JNI_TRUE);
  short                   rcb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  rcb = marpaESLIFRecognizer_scanb(marpaESLIFRecognizerp, initialEventsb, NULL, NULL);

  return rcb ? JNI_TRUE : JNI_FALSE;

 err:
  return JNI_FALSE;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniResume(JNIEnv *envp, jobject eslifRecognizerp, jint deltaLength)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniResume";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  /* Let's be conservative */
  if (deltaLength < 0) {
    RAISEEXCEPTION(envp, "Resume delta length cannot be negative");
  }
  
  if (! marpaESLIFRecognizer_resumeb(marpaESLIFRecognizerp, (size_t) deltaLength, NULL, NULL)) {
    goto err;
  }

  return JNI_TRUE;

 err:
  return JNI_FALSE;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeAlternative(JNIEnv *envp, jobject eslifRecognizerp, jstring namep, jobject objectp, jint grammarLengthi)
/*****************************************************************************/
{
  static const char             *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLexemeAlternative";
  marpaESLIFRecognizer_t        *marpaESLIFRecognizerp;
  marpaESLIFAlternative_t        marpaESLIFAlternative;
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp;
  const char                    *names = NULL;
  jboolean                       isCopy;
  jboolean                       rcb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, &marpaESLIFRecognizerContextp)) {
    goto err;
  }

  /* Well, java have no notion of native unsigned value */
  if (grammarLengthi <= 0) {
    RAISEEXCEPTION(envp, "grammarLength cannot be <= 0");
  }
  
  if (namep != NULL) {
    names = (*envp)->GetStringUTFChars(envp, namep, &isCopy);
    if (names == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  marpaESLIFAlternative.lexemes               = (char *) names;
  marpaESLIFAlternative.grammarLengthl        = (size_t) grammarLengthi;
  if (! marpaESLIFJava_stack_setb(envp, NULL /* marpaESLIFValuep */, -1 /* resulti */, objectp, &(marpaESLIFAlternative.value))) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_lexeme_alternativeb(marpaESLIFRecognizerp, &marpaESLIFAlternative)) {
    goto err;
  }

  rcb = JNI_TRUE;
  goto done;

 err:
  rcb = JNI_FALSE;

 done:
  if (envp != NULL) {
    if ((namep != NULL) && (names != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, namep, names);
    }
  }

  return rcb;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeComplete(JNIEnv *envp, jobject eslifRecognizerp, jint lengthi)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLexemeComplete";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  /* Well, java have no notion of native unsigned value */
  if (lengthi < 0) {
    RAISEEXCEPTION(envp, "length cannot be < 0");
  }
  
  if (!  marpaESLIFRecognizer_lexeme_completeb(marpaESLIFRecognizerp, (size_t) lengthi)) {
    goto err;
  }

  return JNI_TRUE;

 err:
  return JNI_FALSE;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeRead(JNIEnv *envp, jobject eslifRecognizerp, jstring namep, jobject objectp, jint lengthi, jint grammarLengthi)
/*****************************************************************************/
{
  static const char             *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLexemeRead";
  marpaESLIFRecognizer_t        *marpaESLIFRecognizerp;
  marpaESLIFAlternative_t        marpaESLIFAlternative;
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp;
  const char                    *names = NULL;
  jboolean                       isCopy;
  jboolean                       rcb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, &marpaESLIFRecognizerContextp)) {
    goto err;
  }

  /* Well, java have no notion of native unsigned value */
  if (grammarLengthi <= 0) {
    RAISEEXCEPTION(envp, "grammarLength cannot be <= 0");
  }
  
  /* Well, java have no notion of native unsigned value */
  if (lengthi < 0) {
    RAISEEXCEPTION(envp, "length cannot be < 0");
  }
  
  if (namep != NULL) {
    names = (*envp)->GetStringUTFChars(envp, namep, &isCopy);
    if (names == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  marpaESLIFAlternative.lexemes               = (char *) names;
  marpaESLIFAlternative.grammarLengthl        = (size_t) grammarLengthi;
  if (! marpaESLIFJava_stack_setb(envp, NULL /* marpaESLIFValuep */, -1 /* resulti */, objectp, &(marpaESLIFAlternative.value))) {
    goto err;
  }

  if (! marpaESLIFRecognizer_lexeme_readb(marpaESLIFRecognizerp, &marpaESLIFAlternative, (size_t) lengthi)) {
    goto err;
  }

  rcb = JNI_TRUE;
  goto done;

 err:
  rcb = JNI_FALSE;

 done:
  if (envp != NULL) {
    if ((namep != NULL) && (names != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, namep, names);
    }
  }
  return rcb;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeTry(JNIEnv *envp, jobject eslifRecognizerp, jstring namep)
/*****************************************************************************/
{
  static const char             *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLexemeTry";
  marpaESLIFRecognizer_t        *marpaESLIFRecognizerp;
  const char                    *names = NULL;
  jboolean                       isCopy;
  short                          matchb;
  jboolean                       rcb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (namep != NULL) {
    names = (*envp)->GetStringUTFChars(envp, namep, &isCopy);
    if (names == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  if (! marpaESLIFRecognizer_lexeme_tryb(marpaESLIFRecognizerp, (char *) names, &matchb)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFRecognizer_lexeme_tryb failure", strerror(errno));
  }

  rcb = matchb ? JNI_TRUE : JNI_FALSE;
  goto done;

 err:
  rcb = JNI_FALSE;

 done:
  if (envp != NULL) {
    if ((namep != NULL) && (names != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, namep, names);
    }
  }

  return rcb;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniDiscardTry(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char             *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniDiscardTry";
  marpaESLIFRecognizer_t        *marpaESLIFRecognizerp;
  short                          matchb;
  jboolean                       rcb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_discard_tryb(marpaESLIFRecognizerp, &matchb)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_discard_tryb failure");
  }

  rcb = matchb ? JNI_TRUE : JNI_FALSE;
  goto done;

 err:
  rcb = JNI_FALSE;

 done:
  return rcb;
}

/*****************************************************************************/
JNIEXPORT jobjectArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeExpected(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLexemeExpected";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  size_t                  nLexeme;
  size_t                  i;
  char                  **lexemesArrayp;
  jobjectArray            objectArray = NULL;
  jstring                 string      = NULL;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_lexeme_expectedb(marpaESLIFRecognizerp, &nLexeme, &lexemesArrayp)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_lexeme_expectedb failure");
  }

  if (nLexeme > 0) {
    objectArray = (*envp)->NewObjectArray(envp, (jsize) nLexeme, JAVA_LANG_STRING_CLASSP, NULL /* initialElement */);
    if (objectArray == NULL) {
      RAISEEXCEPTION(envp, "NewObjectArray failure");
    }
    for (i = 0; i < nLexeme; i++) {
      string = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, lexemesArrayp[i]);
      (*envp)->SetObjectArrayElement(envp, objectArray, (jsize) i, string);
      if (HAVEEXCEPTION(envp)) {
        goto err;
      }
    }
  }

  goto done;

 err:
  if (envp != NULL) {
    if (objectArray != NULL) {
      (*envp)->DeleteLocalRef(envp, objectArray);
    }
    if (string != NULL) {
      (*envp)->DeleteLocalRef(envp, string);
    }
  }
  objectArray = NULL;

 done:
  return objectArray;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniIsEof(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniIsEof";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  short                   eofb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_isEofb(marpaESLIFRecognizerp, &eofb)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_isEofb failure");
  }

  return eofb ? JNI_TRUE : JNI_FALSE;

 err:
  return JNI_FALSE; /* Exception seen by the upper layer */
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniIsExhausted(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniIsExhausted";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  short                   exhaustedb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_isExhaustedb(marpaESLIFRecognizerp, &exhaustedb)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_isExhaustedb failure");
  }

  return exhaustedb ? JNI_TRUE : JNI_FALSE;

 err:
  return JNI_FALSE; /* Exception seen by the upper layer */
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniSetExhaustedFlag(JNIEnv *envp, jobject eslifRecognizerp, jboolean flag)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniSetExhaustedFlag";
  short                   exhaustedb = (flag == JNI_TRUE) ? 1 : 0;
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_set_exhausted_flagb(marpaESLIFRecognizerp, exhaustedb)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_set_exhausted_flagb failure");
  }

 err:
  return;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniIsCanContinue(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniIsCanContinue";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  short                   canContinueb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_isCanContinueb(marpaESLIFRecognizerp, &canContinueb)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_isCanContinueb failure");
  }

  return canContinueb ? JNI_TRUE : JNI_FALSE;

 err:
  return JNI_FALSE; /* Exception seen by the upper layer */
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniRead(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniRead";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_readb(marpaESLIFRecognizerp, NULL, NULL)) {
    goto err;
  }

  return JNI_TRUE;

 err:
  return JNI_FALSE;
}

/*****************************************************************************/
JNIEXPORT jbyteArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniInput(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniInput";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  jbyteArray              byteArrayp = NULL;
  char                   *inputs;
  size_t                  inputl;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_inputb(marpaESLIFRecognizerp, &inputs, &inputl)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFRecognizer_inputb failure, %s", strerror(errno));
  }

  if ((inputs != NULL) && (inputl > 0)) {
    byteArrayp = (*envp)->NewByteArray(envp, (jsize) inputl);
    if (byteArrayp == NULL) {
      goto err;
    }
    (*envp)->SetByteArrayRegion(envp, byteArrayp, (jsize) 0, (jsize) inputl, (jbyte *) inputs);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  goto done;

 err:
  if (envp != NULL) {
    if (byteArrayp != NULL) {
      (*envp)->DeleteLocalRef(envp, byteArrayp);
    }
  }
  byteArrayp = NULL;

 done:
  return byteArrayp;
}

/*****************************************************************************/
JNIEXPORT jbyteArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeLastPause(JNIEnv *envp, jobject eslifRecognizerp, jstring lexemep)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLexemeLastPause";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  jbyteArray              byteArrayp = NULL;
  const char             *lexemes = NULL;
  jboolean                isCopy;
  char                   *pauses;
  size_t                  pausel;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (lexemep != NULL) {
    lexemes = (*envp)->GetStringUTFChars(envp, lexemep, &isCopy);
    if (lexemes == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  if (!  marpaESLIFRecognizer_lexeme_last_pauseb(marpaESLIFRecognizerp, (char *) lexemes, &pauses, &pausel)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_lexeme_last_pauseb failure");
  }

  if ((pauses != NULL) && (pausel > 0)) {
    byteArrayp = (*envp)->NewByteArray(envp, (jsize) pausel);
    if (byteArrayp == NULL) {
      goto err;
    }
    (*envp)->SetByteArrayRegion(envp, byteArrayp, (jsize) 0, (jsize) pausel, (jbyte *) pauses);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  goto done;

 err:
  if (envp != NULL) {
    if (byteArrayp != NULL) {
      (*envp)->DeleteLocalRef(envp, byteArrayp);
    }
  }
  byteArrayp = NULL;

 done:
  if (envp != NULL) {
    if ((lexemep != NULL) && (lexemes != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, lexemep, lexemes);
    }
  }
  return byteArrayp;
}

/*****************************************************************************/
JNIEXPORT jbyteArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeLastTry(JNIEnv *envp, jobject eslifRecognizerp, jstring lexemep)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLexemeLastTry";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  jbyteArray              byteArrayp = NULL;
  const char             *lexemes = NULL;
  jboolean                isCopy;
  char                   *trys;
  size_t                  tryl;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (lexemep != NULL) {
    lexemes = (*envp)->GetStringUTFChars(envp, lexemep, &isCopy);
    if (lexemes == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  if (!  marpaESLIFRecognizer_lexeme_last_tryb(marpaESLIFRecognizerp, (char *) lexemes, &trys, &tryl)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_lexeme_last_tryb failure");
  }

  if ((trys != NULL) && (tryl > 0)) {
    byteArrayp = (*envp)->NewByteArray(envp, (jsize) tryl);
    if (byteArrayp == NULL) {
      goto err;
    }
    (*envp)->SetByteArrayRegion(envp, byteArrayp, (jsize) 0, (jsize) tryl, (jbyte *) trys);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  goto done;

 err:
  if (envp != NULL) {
    if (byteArrayp != NULL) {
      (*envp)->DeleteLocalRef(envp, byteArrayp);
    }
  }
  byteArrayp = NULL;

 done:
  if (envp != NULL) {
    if ((lexemep != NULL) && (lexemes != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, lexemep, lexemes);
    }
  }
  return byteArrayp;
}

/*****************************************************************************/
JNIEXPORT jbyteArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniDiscardLastTry(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniDiscardLastTry";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  jbyteArray              byteArrayp = NULL;
  char                   *trys;
  size_t                  tryl;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_discard_last_tryb(marpaESLIFRecognizerp, &trys, &tryl)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_discard_last_tryb failure");
  }

  if ((trys != NULL) && (tryl > 0)) {
    byteArrayp = (*envp)->NewByteArray(envp, (jsize) tryl);
    if (byteArrayp == NULL) {
      goto err;
    }
    (*envp)->SetByteArrayRegion(envp, byteArrayp, (jsize) 0, (jsize) tryl, (jbyte *) trys);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  goto done;

 err:
  if (envp != NULL) {
    if (byteArrayp != NULL) {
      (*envp)->DeleteLocalRef(envp, byteArrayp);
    }
  }
  byteArrayp = NULL;

 done:
  return byteArrayp;
}

/*****************************************************************************/
JNIEXPORT jbyteArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniDiscardLast(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniDiscardLast";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  jbyteArray              byteArrayp = NULL;
  char                   *lasts;
  size_t                  lastl;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_discard_lastb(marpaESLIFRecognizerp, &lasts, &lastl)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_discard_lastb failure");
  }

  if ((lasts != NULL) && (lastl > 0)) {
    byteArrayp = (*envp)->NewByteArray(envp, (jsize) lastl);
    if (byteArrayp == NULL) {
      goto err;
    }
    (*envp)->SetByteArrayRegion(envp, byteArrayp, (jsize) 0, (jsize) lastl, (jbyte *) lasts);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  goto done;

 err:
  if (envp != NULL) {
    if (byteArrayp != NULL) {
      (*envp)->DeleteLocalRef(envp, byteArrayp);
    }
  }
  byteArrayp = NULL;

 done:
  return byteArrayp;
}

/*****************************************************************************/
JNIEXPORT jlong JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLastCompletedOffset(JNIEnv *envp, jobject eslifRecognizerp, jstring namep)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLastCompletedOffset";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  const char             *names = NULL;
  jboolean                isCopy;
  char                   *offsetp = NULL;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (namep != NULL) {
    names = (*envp)->GetStringUTFChars(envp, namep, &isCopy);
    if (names == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  if (!  marpaESLIFRecognizer_last_completedb(marpaESLIFRecognizerp, (char *) names, &offsetp, NULL /* lengthlp */)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_last_completedb failure");
  }

  goto done;

 err:
  if (envp != NULL) {
    if ((namep != NULL) && (names != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, namep, names);
    }
  }

 done:
  return PTR_TO_JLONG(offsetp);
}

/*****************************************************************************/
JNIEXPORT jlong JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLastCompletedLength(JNIEnv *envp, jobject eslifRecognizerp, jstring namep)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLastCompletedLength";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  const char             *names = NULL;
  jboolean                isCopy;
  size_t                  lengthl;
  jlong                   rcl;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (namep != NULL) {
    names = (*envp)->GetStringUTFChars(envp, namep, &isCopy);
    if (names == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  if (!  marpaESLIFRecognizer_last_completedb(marpaESLIFRecognizerp, (char *) names, NULL /* offsetpp */, &lengthl)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_last_completedb failure");
  }

  rcl = (jlong) lengthl;
  goto done;

 err:
  rcl = -1;

 done:
  if (envp != NULL) {
    if ((namep != NULL) && (names != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, namep, names);
    }
  }

  return rcl;
}

/*****************************************************************************/
JNIEXPORT jlong JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLine(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniLine";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  size_t                  linel = 0;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_locationb(marpaESLIFRecognizerp, &linel, NULL /* columnlp */)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_locationb failure");
  }

 err: /* err and done share the same code */
  return (jlong) linel;
}

/*****************************************************************************/
JNIEXPORT jlong JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniColumn(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniColumn";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  size_t                  columnl = 0;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_locationb(marpaESLIFRecognizerp, NULL /* linelp */, &columnl)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_locationb failure");
  }

 err: /* err and done share the same code */
  return (jlong) columnl;
}

/*****************************************************************************/
JNIEXPORT jobjectArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniEvent(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniEvent";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  size_t                  eventArrayl;
  marpaESLIFEvent_t      *eventArrayp;
  jobjectArray            objectArray = NULL;
  jobject                 object      = NULL;
  jobject                 eventType   = NULL;
  jstring                 symbol      = NULL;
  jstring                 event       = NULL;
  size_t                  i;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_eventb(marpaESLIFRecognizerp, &eventArrayl, &eventArrayp)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFRecognizer_eventb failure, %s", strerror(errno));
  }

  objectArray = (*envp)->NewObjectArray(envp, (jsize) eventArrayl, MARPAESLIF_ESLIFEVENT_CLASSP, NULL /* initialElement */);
  if (objectArray == NULL) {
    RAISEEXCEPTION(envp, "NewObjectArray failure");
  }

  for (i = 0; i < eventArrayl; i++) {
    object    = NULL;
    eventType = NULL;
    symbol    = NULL;
    event     = NULL;

    eventType = (*envp)->CallStaticObjectMethod(envp, MARPAESLIF_ESLIFEVENTTYPE_CLASSP, MARPAESLIF_ESLIFEVENTTYPE_CLASS_get_METHODP, eventArrayp[i].type);
    if (eventType == NULL) {
      RAISEEXCEPTION(envp, "CallStaticObjectMethod failure");
    }

    symbol = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, eventArrayp[i].symbols);
    event = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, eventArrayp[i].events);

    object = (*envp)->NewObject(envp, MARPAESLIF_ESLIFEVENT_CLASSP, MARPAESLIF_ESLIFEVENT_CLASS_init_METHODP, eventType, symbol, event);
    if (object == NULL) {
      RAISEEXCEPTION(envp, "NewObject failure");
    }
    /* They are all in "object" now */
    eventType = NULL;
    symbol    = NULL;
    event     = NULL;

    (*envp)->SetObjectArrayElement(envp, objectArray, (jsize) i, object);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  goto done;

 err:
  if (envp != NULL) {
    if (objectArray != NULL) {
      (*envp)->DeleteLocalRef(envp, objectArray);
    }
    if (object != NULL) {
      (*envp)->DeleteLocalRef(envp, object);
    }
    if (eventType != NULL) {
      (*envp)->DeleteLocalRef(envp, eventType);
    }
    if (symbol != NULL) {
      (*envp)->DeleteLocalRef(envp, symbol);
    }
    if (event != NULL) {
      (*envp)->DeleteLocalRef(envp, event);
    }
  }
  objectArray = NULL;

 done:
  return objectArray;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniEventOnOff(JNIEnv *envp, jobject eslifRecognizerp, jstring symbolp, jobjectArray eventTypesp, jboolean onOff)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniEventOnOff";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  jboolean                isCopy;
  const char             *symbols    = (symbolp != NULL) ? (*envp)->GetStringUTFChars(envp, symbolp, &isCopy) : NULL;
  marpaESLIFEventType_t   eventSeti  = MARPAESLIF_EVENTTYPE_NONE;
  short                   onoffb     = (onOff == JNI_TRUE);
  jobject                 eventTypep = NULL;
  jsize                   eventTypesLengthl;
  jint                    codei;
  jsize                   i;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (eventTypesp != NULL) {
    eventTypesLengthl = (*envp)->GetArrayLength(envp, eventTypesp);
    if (HAVEEXCEPTION(envp)) {
      RAISEEXCEPTION(envp, "GetArrayLength failure");
    }
    for (i = 0; i < eventTypesLengthl; i++) {
      if (eventTypep != NULL) {
        (*envp)->DeleteLocalRef(envp, eventTypep);
      }
      eventTypep = (*envp)->GetObjectArrayElement(envp, eventTypesp, i);
      if (HAVEEXCEPTION(envp)) {
        RAISEEXCEPTION(envp, " GetObjectArrayElement failure");
      }
      if (eventTypep != NULL) {
        codei = (*envp)->CallIntMethod(envp, eventTypep, MARPAESLIF_ESLIFEVENTTYPE_CLASS_getCode_METHODP);
        if (HAVEEXCEPTION(envp)) {
          RAISEEXCEPTION(envp, "getCode failure");
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
          RAISEEXCEPTIONF(envp, "Unknown code %d", (int) codei);
          break;
        }
      }
    }
  }

  if (! marpaESLIFRecognizer_event_onoffb(marpaESLIFRecognizerp, (char *) symbols, eventSeti, onoffb)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFRecognizer_event_onoffb failure, %s", strerror(errno));
  }

 err: /* err and done share the same code */
  if (envp != NULL) {
    if ((symbolp != NULL) && (symbols != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, symbolp, symbols);
    }
    if (eventTypep != NULL) {
      (*envp)->DeleteLocalRef(envp, eventTypep);
    }
  }

  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniHookDiscard(JNIEnv *envp, jobject eslifRecognizerp, jboolean onOff)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniHookDiscard";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  short                   discardOnoffb = (onOff == JNI_TRUE);

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_hook_discardb(marpaESLIFRecognizerp, discardOnoffb)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFRecognizer_hook_discardb failure, %s", strerror(errno));
  }

 err: /* err and done share the same code */

  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniHookDiscardSwitch(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniHookDiscardSwitch";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_hook_discard_switchb(marpaESLIFRecognizerp)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFRecognizer_hook_discard_switchb failure, %s", strerror(errno));
  }

 err: /* err and done share the same code */

  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniProgressLog(JNIEnv *envp, jobject eslifRecognizerp, int starti, int endi, jobject levelp)
/*****************************************************************************/
{
  static const char      *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniProgressLog";
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  genericLoggerLevel_t    logleveli;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (levelp != NULL) {
    logleveli = (*envp)->CallIntMethod(envp, levelp, MARPAESLIF_ESLIFLOGGERLEVEL_CLASS_getCode_METHODP);
    if (HAVEEXCEPTION(envp)) {
      RAISEEXCEPTION(envp, "getCode failure");
    }
    switch (logleveli) {
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
      RAISEEXCEPTIONF(envp, "Unknown logger level %d", (int) logleveli);
      break;
    }
    if (! marpaESLIFRecognizer_progressLogb(marpaESLIFRecognizerp, (int) starti, (int) endi, logleveli)) {
      RAISEEXCEPTION(envp, "marpaESLIFRecognizer_progressLogb failure");
    }
  }

 err: /* err and done share the same code */
  return;
}

/*****************************************************************************/
JNIEXPORT jbyteArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniSymbolTry(JNIEnv *envp, jobject eslifRecognizerp, jobject eslifSymbolp)
/*****************************************************************************/
{
  static const char           *funcs = "Java_org_parser_marpa_ESLIFRecognizer_jniSymbolTry";
  marpaESLIFRecognizer_t      *marpaESLIFRecognizerp;
  marpaESLIFSymbol_t          *marpaESLIFSymbolp;
  jbyteArray                   byteArrayp = NULL;
  short                        matchb;
  marpaESLIFValueResultArray_t marpaESLIFValueResultArray;

  marpaESLIFValueResultArray.p        = NULL;
  marpaESLIFValueResultArray.sizel    = 0;
  marpaESLIFValueResultArray.shallowb = 0;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, NULL /* marpaESLIFRecognizerContextpp */)) {
    goto err;
  }

  if (! ESLIFSymbol_contextb(envp, eslifSymbolp, NULL /* marpaESLIFpp */, &marpaESLIFSymbolp)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_symbol_tryb(marpaESLIFRecognizerp, marpaESLIFSymbolp, &matchb, &marpaESLIFValueResultArray)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFRecognizer_symbol_tryb failure, %s", strerror(errno));
  }

  if (matchb) {
    byteArrayp = (*envp)->NewByteArray(envp, (jsize) marpaESLIFValueResultArray.sizel);
    if (byteArrayp == NULL) {
      goto err;
    }
    (*envp)->SetByteArrayRegion(envp, byteArrayp, (jsize) 0, (jsize) marpaESLIFValueResultArray.sizel, (jbyte *) marpaESLIFValueResultArray.p);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  goto done;

 err:
  if (envp != NULL) {
    if (byteArrayp != NULL) {
      (*envp)->DeleteLocalRef(envp, byteArrayp);
    }
  }
  byteArrayp = NULL;

 done:
  if ((! marpaESLIFValueResultArray.shallowb) && (marpaESLIFValueResultArray.p != NULL)) {
    free(marpaESLIFValueResultArray.p);
  }
  return byteArrayp;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniFree(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t        *marpaESLIFRecognizerp;
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp;

  if (ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, &marpaESLIFRecognizerContextp)) {
    marpaESLIFJava_recognizerContextFreev(envp, marpaESLIFRecognizerContextp, 0 /* onStackb */);
    marpaESLIFRecognizer_freev(marpaESLIFRecognizerp); /* This is NULL protected */
  }
}

/*****************************************************************************/
static short ESLIFValue_contextb(JNIEnv *envp, jobject eslifValuep, marpaESLIFValue_t **marpaESLIFValuepp, marpaESLIFValueContext_t **marpaESLIFValueContextpp)
/*****************************************************************************/
{
  static const char        *funcs = "ESLIFValue_contextb";
  jobject                   BYTEBUFFER(marpaESLIFValue);
  jobject                   BYTEBUFFER(marpaESLIFValueContext);
  marpaESLIFValue_t        *marpaESLIFValuep;
  marpaESLIFValueContext_t *marpaESLIFValueContextp;
    
  if (marpaESLIFValuepp != NULL) {
    BYTEBUFFER(marpaESLIFValue) = (*envp)->CallObjectMethod(envp, eslifValuep, MARPAESLIF_ESLIFVALUE_CLASS_getMarpaESLIFValuep_METHODP);
    if (BYTEBUFFER(marpaESLIFValue) == NULL) {
      RAISEEXCEPTION(envp, "marpaESLIFValue bytebuffer is NULL");
    }
    marpaESLIFValuep = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(marpaESLIFValue));
    if (marpaESLIFValuep == NULL) {
      RAISEEXCEPTION(envp, "marpaESLIFValuep is NULL");
    }
    *marpaESLIFValuepp = marpaESLIFValuep;
  }

  /* Value context content is always updated */
  BYTEBUFFER(marpaESLIFValueContext) = (*envp)->CallObjectMethod(envp, eslifValuep, MARPAESLIF_ESLIFVALUE_CLASS_getMarpaESLIFValueContextp_METHODP);
  if (BYTEBUFFER(marpaESLIFValueContext) == NULL) {
    RAISEEXCEPTION(envp, "marpaESLIFValueContext bytebuffer is NULL");
  }
  marpaESLIFValueContextp = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(marpaESLIFValueContext));
  if (marpaESLIFValueContextp == NULL) {
    RAISEEXCEPTION(envp, "marpaESLIFValueContextp is NULL");
  }
  marpaESLIFValueContextp->eslifValueInterfacep = (*envp)->CallObjectMethod(envp, eslifValuep, MARPAESLIF_ESLIFVALUE_CLASS_getEslifValueInterface_METHODP);
  if (marpaESLIFValueContextpp != NULL) {
    *marpaESLIFValueContextpp = marpaESLIFValueContextp;
  }
  if (marpaESLIFValueContextpp != NULL) {
    *marpaESLIFValueContextpp = marpaESLIFValueContextp;
  }
  
  return 1;

 err:
  return 0;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFValue_jniNew(JNIEnv *envp, jobject eslifValuep, jobject eslifRecognizerp)
/*****************************************************************************/
{
  static const char                *funcs = "Java_org_parser_marpa_ESLIFValue_jniNew";
  marpaESLIFValue_t                *marpaESLIFValuep  = NULL;
  marpaESLIFRecognizer_t           *marpaESLIFRecognizerp;
  marpaESLIFRecognizerContext_t    *marpaESLIFRecognizerContextp;
  marpaESLIFValueOption_t           marpaESLIFValueOption;
  marpaESLIFValueContext_t         *marpaESLIFValueContextp;
  jobject                           eslifValueInterfacep;
  jobject                           eslifGrammarp;
  jobject                           BYTEBUFFER(marpaESLIFValue);
  jobject                           BYTEBUFFER(marpaESLIFValueContext);

  /* Always update genericLogger context */
  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, &marpaESLIFRecognizerp, &marpaESLIFRecognizerContextp)) {
    /* An exception was raised */
    goto err;
  }

  /* Get grammar instance */
  eslifGrammarp = (*envp)->CallObjectMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifGrammar_METHODP);
  if (eslifGrammarp == NULL) {
    RAISEEXCEPTION(envp, "eslifGrammarp is NULL");
  }

 /* Get value interface - class is protected to not accept the null argument */
  eslifValueInterfacep = (*envp)->CallObjectMethod(envp, eslifValuep, MARPAESLIF_ESLIFVALUE_CLASS_getEslifValueInterface_METHODP);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  /* Value context is a structure containing the current value interface object instance - updated at every recognizer call */
  marpaESLIFValueContextp = (marpaESLIFValueContext_t *) malloc(sizeof(marpaESLIFValueContext_t));
  if (marpaESLIFValueContextp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  if (! marpaESLIFJava_valueContextInitb(envp, eslifValueInterfacep, eslifGrammarp, marpaESLIFValueContextp)) {
    goto err;
  }
  
  /* Create C object */
  marpaESLIFValueOption.userDatavp            = marpaESLIFValueContextp;
  marpaESLIFValueOption.ruleActionResolverp   = marpaESLIFValueRuleActionResolver;
  marpaESLIFValueOption.symbolActionResolverp = marpaESLIFValueSymbolActionResolver;
  marpaESLIFValueOption.importerp             = marpaESLIFJava_valueImportb;
  marpaESLIFValueOption.highRankOnlyb         = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithHighRankOnly_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFValueOption.orderByRankb          = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithOrderByRank_METHODP)  == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFValueOption.ambiguousb            = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithAmbiguous_METHODP)    == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFValueOption.nullb                 = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithNull_METHODP)         == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFValueOption.maxParsesi            = (*envp)->CallIntMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_maxParses_METHODP);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  marpaESLIFValuep = marpaESLIFValue_newp(marpaESLIFRecognizerp, &marpaESLIFValueOption);
  if (marpaESLIFValuep == NULL) {
    RAISEEXCEPTIONF(envp, "marpaESLIFValue_newp failure, %s", strerror(errno));
  }

  /* Store the object */
  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIFValue, marpaESLIFValuep);  
  (*envp)->CallVoidMethod(envp, eslifValuep, MARPAESLIF_ESLIFVALUE_CLASS_setMarpaESLIFValuep_METHODP, BYTEBUFFER(marpaESLIFValue));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIFValueContext, marpaESLIFValueContextp);  
  (*envp)->CallVoidMethod(envp, eslifValuep, MARPAESLIF_ESLIFVALUE_CLASS_setMarpaESLIFValueContextp_METHODP, BYTEBUFFER(marpaESLIFValueContext));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  goto done;
  
 err:
  Java_org_parser_marpa_ESLIFValue_jniFree(envp, eslifValuep);

 done:
  return;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFValue_jniValue(JNIEnv *envp, jobject eslifValuep)
/*****************************************************************************/
{
  static const char         *funcs = "Java_org_parser_marpa_ESLIFValue_jniValue";
  marpaESLIFValue_t         *marpaESLIFValuep;
  short                      valueb;
  jboolean                   rcb;
  marpaESLIFValueContext_t  *marpaESLIFValueContextp;
  jobject                    eslifValueInterfacep;
  jobject                    objectp;
  
  if (! ESLIFValue_contextb(envp, eslifValuep, &marpaESLIFValuep, &marpaESLIFValueContextp)) {
    goto err;
  }

  eslifValueInterfacep = marpaESLIFValueContextp->eslifValueInterfacep;

  valueb = marpaESLIFValue_valueb(marpaESLIFValuep);
  if (valueb < 0) {
    RAISEEXCEPTIONF(envp, "marpaESLIFValue_valueb failure, %s", strerror(errno));
  }

  if (valueb > 0) {
    if (GENERICSTACK_USED(marpaESLIFValueContextp->objectStackp) != 1) {
      RAISEEXCEPTIONF(envp, "Internal value stack is %d instead of 1", GENERICSTACK_USED(marpaESLIFValueContextp->objectStackp));
    }
    objectp = (jobject) GENERICSTACK_POP_PTR(marpaESLIFValueContextp->objectStackp);
    (*envp)->CallVoidMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setResult_METHODP, objectp);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
    rcb = JNI_TRUE;
  } else {
    rcb = JNI_FALSE;
  }

goto done;

 err:
  /* Well, caller will see an exception at this stage */
  rcb = JNI_FALSE;

 done:
  return rcb;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFValue_jniFree(JNIEnv *envp, jobject eslifValuep)
/*****************************************************************************/
{
  marpaESLIFValue_t        *marpaESLIFValuep;
  marpaESLIFValueContext_t *marpaESLIFValueContextp;

  if (ESLIFValue_contextb(envp, eslifValuep, &marpaESLIFValuep, &marpaESLIFValueContextp)) {
    marpaESLIFJava_valueContextFreev(envp, marpaESLIFValueContextp, 0 /* onStackb */);
    marpaESLIFValue_freev(marpaESLIFValuep); /* This is NULL protected */
  }
}

/*****************************************************************************/
static void readerCallbackDisposev(void *userDatavp, char *inputcp, size_t inputl, short eofb, short characterStreamb, char *encodings, size_t encodingl)
/*****************************************************************************/
{
  JNIEnv                        *envp;
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp;

  marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) userDatavp;

  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    return;
  }

  if (marpaESLIFRecognizerContextp != NULL) {
    if ((marpaESLIFRecognizerContextp->byteArrayp != NULL) && (inputcp != NULL)) {
      (*envp)->ReleaseByteArrayElements(envp, marpaESLIFRecognizerContextp->byteArrayp, (jbyte *) inputcp, JNI_ABORT);
    }
    marpaESLIFRecognizerContextp->byteArrayp = NULL;

    if ((marpaESLIFRecognizerContextp->encodingp != NULL) && (encodings != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, marpaESLIFRecognizerContextp->encodingp, (const char *) encodings);
    }
    marpaESLIFRecognizerContextp->encodingp = NULL;
  }
}

/*****************************************************************************/
static short readerCallbackb(void *userDatavp, char **inputcpp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingsp, size_t *encodinglp, marpaESLIFReaderDispose_t *disposeCallbackpp)
/*****************************************************************************/
{
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp;
  jobject                       eslifRecognizerInterfacep;
  JNIEnv                        *envp = NULL;
  jbyteArray                     byteArrayp = NULL;
  jbyte                         *datap = NULL;
  jstring                        encodingp = NULL;
  const char                    *encodings = NULL;
  jboolean                       isCopy;
  jboolean                       readb;
  short                          rcb;

  marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) userDatavp;
  eslifRecognizerInterfacep   = marpaESLIFRecognizerContextp->eslifRecognizerInterfacep;

  /* Reader callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Call the read interface */
  readb = (*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_read_METHODP);
  if (HAVEEXCEPTION(envp) || (readb != JNI_TRUE)) {
    goto err;
  }

  byteArrayp = (*envp)->CallObjectMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_data_METHODP);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  datap      = (byteArrayp != NULL) ? (*envp)->GetByteArrayElements(envp, byteArrayp, &isCopy) : NULL;
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  encodingp  = (*envp)->CallObjectMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_encoding_METHODP);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  encodings  = (encodingp != NULL) ? (*envp)->GetStringUTFChars(envp, encodingp, &isCopy) : NULL;  /* => The famous UTF-8 hardcoded below */
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  *inputcpp             = (char *) datap;
  *inputlp              = (size_t) (byteArrayp != NULL) ? (*envp)->GetArrayLength(envp, byteArrayp) : 0;
  *eofbp                = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isEof_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  *characterStreambp    = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isCharacterStream_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  *encodingsp           = (char *) encodings;
  *encodinglp           = (size_t) (encodings != NULL ? strlen(encodings) : 0);
  *disposeCallbackpp    = readerCallbackDisposev;

  marpaESLIFRecognizerContextp->byteArrayp = byteArrayp;
  marpaESLIFRecognizerContextp->encodingp  = encodingp;

  rcb = 1;
  goto done;

 err:
  if (envp != NULL) {
    if ((byteArrayp != NULL) && (datap != NULL)) {
      (*envp)->ReleaseByteArrayElements(envp, byteArrayp, datap, JNI_ABORT);
    }
    if ((encodingp != NULL) && (encodings != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, encodingp, encodings);
    }
  }

  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static marpaESLIFValueRuleCallback_t marpaESLIFValueRuleActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions)
/*****************************************************************************/
{
  JNIEnv                  *envp;
  marpaESLIFValueContext_t *marpaESLIFValueContextp = (marpaESLIFValueContext_t *) userDatavp;
  jmethodID                methodp;

  /* Resolver callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Make sure rule action method is resolved */
  methodp = marpaESLIFJava_valueActionResolveri(envp, marpaESLIFValueContextp, actions, MARPAESLIF_ESLIFVALUEINTERFACE_RULEACTION_SIGNATURE);
  if (methodp == NULL) {
    goto err;
  }

  marpaESLIFValueContextp->actions = actions;
  marpaESLIFValueContextp->methodp = methodp;

  return marpaESLIFValueRuleCallback;

 err:
  return NULL;
}

/*****************************************************************************/
static marpaESLIFValueSymbolCallback_t marpaESLIFValueSymbolActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions)
/*****************************************************************************/
{
  JNIEnv                  *envp;
  marpaESLIFValueContext_t *marpaESLIFValueContextp = (marpaESLIFValueContext_t *) userDatavp;
  jmethodID                methodp;

  /* Resolver callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Make sure symbol action method is resolved */
  methodp = marpaESLIFJava_valueActionResolveri(envp, marpaESLIFValueContextp, actions, MARPAESLIF_ESLIFVALUEINTERFACE_SYMBOLACTION_SIGNATURE);
  if (methodp == NULL) {
    goto err;
  }

  marpaESLIFValueContextp->actions = actions;
  marpaESLIFValueContextp->methodp = methodp;

  return marpaESLIFJava_valueSymbolCallbackb;

 err:
  return NULL;
}

/*****************************************************************************/
static marpaESLIFRecognizerIfCallback_t marpaESLIFRecognizerIfActionResolver(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *actions)
/*****************************************************************************/
{
  JNIEnv                        *envp;
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) userDatavp;
  jmethodID                      methodp;

  /* Resolver callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Make sure symbol action method is resolved */
  methodp = marpaESLIFJava_recognizerActionResolveri(envp, marpaESLIFRecognizerContextp, actions, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_IFACTION_SIGNATURE);
  if (methodp == NULL) {
    goto err;
  }

  marpaESLIFRecognizerContextp->actions = actions;
  marpaESLIFRecognizerContextp->methodp = methodp;

  return marpaESLIFJava_recognizerIfCallbackb;

 err:
  return NULL;
}

/*****************************************************************************/
static marpaESLIFRecognizerEventCallback_t marpaESLIFRecognizerEventActionResolver(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *actions)
/*****************************************************************************/
{
  JNIEnv                        *envp;
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) userDatavp;
  jmethodID                      methodp;

  /* Resolver callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Make sure event action method is resolved */
  methodp = marpaESLIFJava_recognizerActionResolveri(envp, marpaESLIFRecognizerContextp, actions, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_EVENTACTION_SIGNATURE);
  if (methodp == NULL) {
    goto err;
  }

  marpaESLIFRecognizerContextp->actions = actions;
  marpaESLIFRecognizerContextp->methodp = methodp;

  return marpaESLIFJava_recognizerEventCallbackb;

 err:
  return NULL;
}

/*****************************************************************************/
static marpaESLIFRecognizerRegexCallback_t marpaESLIFRecognizerRegexActionResolver(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, char *actions)
/*****************************************************************************/
{
  JNIEnv                        *envp;
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) userDatavp;
  jmethodID                      methodp;

  /* Resolver callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Make sure event action method is resolved */
  methodp = marpaESLIFJava_recognizerActionResolveri(envp, marpaESLIFRecognizerContextp, actions, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_REGEXACTION_SIGNATURE);
  if (methodp == NULL) {
    goto err;
  }

  marpaESLIFRecognizerContextp->actions = actions;
  marpaESLIFRecognizerContextp->methodp = methodp;

  return marpaESLIFJava_recognizerRegexCallbackb;

 err:
  return NULL;
}

/*****************************************************************************/
static short marpaESLIFValueContextInject(JNIEnv *envp, marpaESLIFValue_t *marpaESLIFValuep, jobject eslifValueInterfacep, marpaESLIFValueContext_t *marpaESLIFValueContextp)
/*****************************************************************************/
{
  static const char *funcs = "marpaESLIFValueContextInject";
  short              rcb;
  char              *symbols;
  int                symboli;
  char              *rules;
  int                rulei;
  jstring            symbolp = NULL;
  jstring            rulep = NULL;

  /* Get value context */
  if (! marpaESLIFValue_contextb(marpaESLIFValuep, &symbols, &symboli, &rules, &rulei)) {
    RAISEEXCEPTION(envp, "marpaESLIFValue_contextb");
  }

  /* Symbol name */
  if (symbols != NULL) {
    symbolp = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, symbols);
  }

  (*envp)->CallVoidMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setSymbolName_METHODP, symbolp /* Can be NULL */);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  /* Symbol number */
  (*envp)->CallVoidMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setSymbolNumber_METHODP, symboli);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  /* Rule name */
  if (rules != NULL) {
    rulep = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, rules);
  }

  (*envp)->CallVoidMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setRuleName_METHODP, rulep /* Can be NULL */);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  /* Rule number */
  (*envp)->CallVoidMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setRuleNumber_METHODP, rulei);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  /* Grammar instance - stored as a global reference in  marpaESLIFValueContextp */
  (*envp)->CallVoidMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setGrammar_METHODP, marpaESLIFValueContextp->eslifGrammarp);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  if (symbolp != NULL) {
    (*envp)->DeleteLocalRef(envp, symbolp);
  }
  if (rulep != NULL) {
    (*envp)->DeleteLocalRef(envp, rulep);
  }
  return rcb;
}

/*****************************************************************************/
static short marpaESLIFValueRuleCallback(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb)
/*****************************************************************************/
{
  static const char             *funcs                   = "marpaESLIFValueRuleCallback";
  marpaESLIFValueContext_t      *marpaESLIFValueContextp = (marpaESLIFValueContext_t *) userDatavp;
  jobjectArray                   list;
  JNIEnv                        *envp;
  jobject                        actionResultp;
  short                          rcb;
  int                            i;
  jint                           capacityi;
  jobject                        objectp;

  /* Rule callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Prototype of a rule action is: public Object action(List<Object> list) */
  if (! nullableb) {
    /* This is a "variable local references" method - make sure we will have enough room in local frame */
    capacityi = (argni - arg0i + 1) /* worst case where everything is a lexeme */ + 1 /* list itself */ + 1 /* action result */;
    if ((*envp)->EnsureLocalCapacity(envp, capacityi) != 0) {
      RAISEEXCEPTION(envp, "EnsureLocalCapacity failure");
    }
  
    /* Make the list */
    list = (*envp)->NewObjectArray(envp, argni - arg0i + 1, JAVA_LANG_OBJECT_CLASSP, NULL /* initialElement */);
    if (list == NULL) {
      RAISEEXCEPTION(envp, "NewObjectArray failure");
    }

    for (i = arg0i; i <= argni; i++) {
      if (! marpaESLIFJava_valueGetObjectp(marpaESLIFValueContextp, marpaESLIFValuep, i, NULL /* marpaESLIFValueResultLexeme */, &objectp)) {
	goto err;
      }
      (*envp)->SetObjectArrayElement(envp, list, i - arg0i, objectp);
    }
  }

  /* Inject the context */
  if (! marpaESLIFValueContextInject(envp, marpaESLIFValuep, marpaESLIFValueContextp->eslifValueInterfacep, marpaESLIFValueContextp)) {
    goto err;
  }
  /* Call the rule action */
  actionResultp = (*envp)->CallObjectMethod(envp, marpaESLIFValueContextp->eslifValueInterfacep, marpaESLIFValueContextp->methodp, list);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  if (! marpaESLIFJava_stack_setb(envp, marpaESLIFValuep, resulti, actionResultp, NULL)) {
    goto err;
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static short marpaESLIFJava_valueSymbolCallbackb(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, marpaESLIFValueResult_t *marpaESLIFValueResultp, int resulti)
/*****************************************************************************/
{
  /* Almost exactly like marpaESLIFValueRuleCallback except that we construct a list of one element containing a jbyteArray that we do ourself */
  static const char             *funcs                   = "marpaESLIFJava_valueSymbolCallbackb";
  marpaESLIFValueContext_t      *marpaESLIFValueContextp = (marpaESLIFValueContext_t *) userDatavp;
  JNIEnv                        *envp;
  jobject                        actionResultp;
  short                          rcb;
  jobject                        objectp;

  /* Symbol callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  if (! marpaESLIFJava_valueGetObjectp(marpaESLIFValueContextp, marpaESLIFValuep, -1 /* stackindicei */, marpaESLIFValueResultp, &objectp)) {
    goto err;
  }

  /* Inject the context */
  if (! marpaESLIFValueContextInject(envp, marpaESLIFValuep, marpaESLIFValueContextp->eslifValueInterfacep, marpaESLIFValueContextp)) {
    goto err;
  }

  /* Call the symbol action */
  actionResultp = (*envp)->CallObjectMethod(envp, marpaESLIFValueContextp->eslifValueInterfacep, marpaESLIFValueContextp->methodp, objectp);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  if (! marpaESLIFJava_stack_setb(envp, marpaESLIFValuep, resulti, actionResultp, NULL)) {
    goto err;
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static short marpaESLIFJava_recognizerIfCallbackb(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIFValueResult_t *marpaESLIFValueResultLexemep, marpaESLIFValueResultBool_t *marpaESLIFValueResultBoolp)
/*****************************************************************************/
{
  /* Almost exactly like marpaESLIFValueSymbolCallback except that we construct a list of one element containing a jbyteArray that we do ourself */
  static const char             *funcs                        = "marpaESLIFJava_recognizerIfCallbackb";
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) userDatavp;
  JNIEnv                        *envp;
  jboolean                       boolean;
  short                          rcb;
  jbyteArray                     byteArrayp;

  /* Symbol callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  byteArrayp = (*envp)->NewByteArray(envp, (jsize) marpaESLIFValueResultLexemep->u.a.sizel);
  if (byteArrayp == NULL) {
    goto err;
  }
  if (marpaESLIFValueResultLexemep->u.a.sizel > 0) {
    /* Don't know if it is legal to call SetByteArrayRegion with a zero size -; */
    (*envp)->SetByteArrayRegion(envp, byteArrayp, 0, (jsize) marpaESLIFValueResultLexemep->u.a.sizel, (jbyte *) marpaESLIFValueResultLexemep->u.a.p);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  /* Call the if action */
  boolean = (*envp)->CallBooleanMethod(envp, marpaESLIFRecognizerContextp->eslifRecognizerInterfacep, marpaESLIFRecognizerContextp->methodp, byteArrayp);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  *marpaESLIFValueResultBoolp = (boolean == JNI_FALSE) ? MARPAESLIFVALUERESULTBOOL_FALSE : MARPAESLIFVALUERESULTBOOL_TRUE;

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static short marpaESLIFJava_recognizerEventCallbackb(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIFEvent_t *eventArrayp, size_t eventArrayl, marpaESLIFValueResultBool_t *marpaESLIFValueResultBoolp)
/*****************************************************************************/
{
  /* Almost exactly like marpaESLIFJava_recognizerIfCallbackb */
  static const char             *funcs                        = "marpaESLIFJava_recognizerEventCallbackb";
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) userDatavp;
  jobjectArray                   objectArray                  = NULL;
  jobject                        object                       = NULL;
  jobject                        eventType                    = NULL;
  jstring                        symbol                       = NULL;
  jstring                        event                        = NULL;
  size_t                         i;
  JNIEnv                        *envp;
  jboolean                       boolean;
  short                          rcb;

  /* Event callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  objectArray = (*envp)->NewObjectArray(envp, (jsize) eventArrayl, MARPAESLIF_ESLIFEVENT_CLASSP, NULL /* initialElement */);
  if (objectArray == NULL) {
    RAISEEXCEPTION(envp, "NewObjectArray failure");
  }

  for (i = 0; i < eventArrayl; i++) {
    object    = NULL;
    eventType = NULL;
    symbol    = NULL;
    event     = NULL;

    eventType = (*envp)->CallStaticObjectMethod(envp, MARPAESLIF_ESLIFEVENTTYPE_CLASSP, MARPAESLIF_ESLIFEVENTTYPE_CLASS_get_METHODP, eventArrayp[i].type);
    if (eventType == NULL) {
      RAISEEXCEPTION(envp, "CallStaticObjectMethod failure");
    }

    symbol = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, eventArrayp[i].symbols);
    event = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, eventArrayp[i].events);

    object = (*envp)->NewObject(envp, MARPAESLIF_ESLIFEVENT_CLASSP, MARPAESLIF_ESLIFEVENT_CLASS_init_METHODP, eventType, symbol, event);
    if (object == NULL) {
      RAISEEXCEPTION(envp, "NewObject failure");
    }
    /* They are all in "object" now */
    eventType = NULL;
    symbol    = NULL;
    event     = NULL;

    (*envp)->SetObjectArrayElement(envp, objectArray, (jsize) i, object);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  /* Call the event action */
  boolean = (*envp)->CallBooleanMethod(envp, marpaESLIFRecognizerContextp->eslifRecognizerInterfacep, marpaESLIFRecognizerContextp->methodp, objectArray);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  *marpaESLIFValueResultBoolp = (boolean == JNI_FALSE) ? MARPAESLIFVALUERESULTBOOL_FALSE : MARPAESLIFVALUERESULTBOOL_TRUE;

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  if (envp != NULL) {
    if (objectArray != NULL) {
      (*envp)->DeleteLocalRef(envp, objectArray);
    }
    if (object != NULL) {
      (*envp)->DeleteLocalRef(envp, object);
    }
    if (eventType != NULL) {
      (*envp)->DeleteLocalRef(envp, eventType);
    }
    if (symbol != NULL) {
      (*envp)->DeleteLocalRef(envp, symbol);
    }
    if (event != NULL) {
      (*envp)->DeleteLocalRef(envp, event);
    }
  }

  return rcb;
}

/*****************************************************************************/
static short marpaESLIFJava_recognizerRegexCallbackb(void *userDatavp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIFValueResult_t *marpaESLIFCalloutBlockp, marpaESLIFValueResultInt_t *marpaESLIFValueResultOutp)
/*****************************************************************************/
{
  static const char                     *funcs                        = "marpaESLIFJava_recognizerRegexCallbackb";
  marpaESLIFRecognizerContext_t         *marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) userDatavp;
  jobject                                regexEslifCalloutp           = NULL;
  jobject                                calloutNumberp               = NULL;
  jstring                                calloutStringp               = NULL;
  jbyteArray                             subjectp                     = NULL;
  jstring                                patternp                     = NULL;
  jlong                                  captureTop;
  jlong                                  captureLast;
  jlongArray                             offsetVectorp                = NULL;
  jlong                                 *offsetVectorArrayp           = NULL;
  jobject                                markp                        = NULL;
  jlong                                  startMatch;
  jlong                                  currentPosition;
  jstring                                nextItemp                    = NULL;
  JNIEnv                                *envp;
  marpaESLIFString_t                     marpaESLIFString;
  jint                                   integer;
  short                                  rcb;
  size_t                                 l;

  /* Symbol callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* By definition, marpaESLIFCalloutBlockp is of type TABLE */
  if (marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_CALLOUT_NUMBER].value.type == MARPAESLIF_VALUE_TYPE_LONG) {
    MARPAESLIFJAVA_NEW_LONG(envp, calloutNumberp, marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_CALLOUT_NUMBER].value.u.l);
  }

  if (marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_CALLOUT_STRING].value.type == MARPAESLIF_VALUE_TYPE_STRING) {
    marpaESLIFString.bytep          = (char *) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_CALLOUT_STRING].value.u.s.p;
    marpaESLIFString.bytel          = marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_CALLOUT_STRING].value.u.s.sizel;
    marpaESLIFString.encodingasciis = marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_CALLOUT_STRING].value.u.s.encodingasciis;
    marpaESLIFString.asciis         = NULL;
    calloutStringp = marpaESLIFJava_marpaESLIFStringToJavap(envp, &marpaESLIFString);
  }

  subjectp = (*envp)->NewByteArray(envp, (jsize) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_SUBJECT].value.u.a.sizel);
  if (subjectp == NULL) {
    goto err;
  }
  if (marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_SUBJECT].value.u.a.sizel > 0) {
    (*envp)->SetByteArrayRegion(envp, subjectp, (jsize) 0, (jsize) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_SUBJECT].value.u.a.sizel, (jbyte *) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_SUBJECT].value.u.a.p);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  if (marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_PATTERN].value.type == MARPAESLIF_VALUE_TYPE_STRING) {
    marpaESLIFString.bytep          = (char *) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_PATTERN].value.u.s.p;
    marpaESLIFString.bytel          = marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_PATTERN].value.u.s.sizel;
    marpaESLIFString.encodingasciis = marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_PATTERN].value.u.s.encodingasciis;
    marpaESLIFString.asciis         = NULL;
    patternp = marpaESLIFJava_marpaESLIFStringToJavap(envp, &marpaESLIFString);
  }

  captureTop = (jlong) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_CAPTURE_TOP].value.u.l;
  captureLast = (jlong) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_CAPTURE_LAST].value.u.l;

  offsetVectorp = (*envp)->NewLongArray(envp, (jsize) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_OFFSET_VECTOR].value.u.r.sizel);
  if (offsetVectorp == NULL) {
    goto err;
  }
  if (marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_OFFSET_VECTOR].value.u.r.sizel > 0) {
    offsetVectorArrayp = (jlong *) malloc(sizeof(jlong) * marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_OFFSET_VECTOR].value.u.r.sizel);
    if (offsetVectorArrayp == NULL) {
      RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
    }
    for (l = 0; l < marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_OFFSET_VECTOR].value.u.r.sizel; l++) {
      offsetVectorArrayp[l] = (long) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_OFFSET_VECTOR].value.u.r.p[l].u.l;
    }
    (*envp)->SetLongArrayRegion(envp, offsetVectorp, 0, (jsize) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_OFFSET_VECTOR].value.u.r.sizel, offsetVectorArrayp);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }
  
  if (marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_MARK].value.type == MARPAESLIF_VALUE_TYPE_STRING) {
    marpaESLIFString.bytep          = (char *) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_MARK].value.u.s.p;
    marpaESLIFString.bytel          = marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_MARK].value.u.s.sizel;
    marpaESLIFString.encodingasciis = marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_MARK].value.u.s.encodingasciis;
    marpaESLIFString.asciis         = NULL;
    markp = marpaESLIFJava_marpaESLIFStringToJavap(envp, &marpaESLIFString);
  }

  startMatch = (jlong) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_START_MATCH].value.u.l;
  currentPosition = (jlong) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_CURRENT_POSITION].value.u.l;

  if (marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_NEXT_ITEM].value.type == MARPAESLIF_VALUE_TYPE_STRING) {
    marpaESLIFString.bytep          = (char *) marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_NEXT_ITEM].value.u.s.p;
    marpaESLIFString.bytel          = marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_NEXT_ITEM].value.u.s.sizel;
    marpaESLIFString.encodingasciis = marpaESLIFCalloutBlockp->u.t.p[MARPAESLIFCALLOUTBLOCK_NEXT_ITEM].value.u.s.encodingasciis;
    marpaESLIFString.asciis         = NULL;
    nextItemp = marpaESLIFJava_marpaESLIFStringToJavap(envp, &marpaESLIFString);
  }

  regexEslifCalloutp = (*envp)->NewObject(envp,
                                          MARPAESLIF_ESLIFREGEXCALLOUT_CLASSP,
                                          MARPAESLIF_ESLIFREGEXCALLOUT_CLASS_init_METHODP,
                                          calloutNumberp,
                                          calloutStringp,
                                          subjectp,
                                          patternp,
                                          captureTop,
                                          captureLast,
                                          offsetVectorp,
                                          markp,
                                          startMatch,
                                          currentPosition,
                                          nextItemp);
  if (regexEslifCalloutp == NULL) {
    goto err;
  }

  /* All objects are now in regexEslifCalloutp */
  calloutNumberp     = NULL;
  calloutStringp     = NULL;
  subjectp           = NULL;
  patternp           = NULL;
  offsetVectorp      = NULL;
  markp              = NULL;
  nextItemp          = NULL;

  /* Call the regex action */
  integer = (*envp)->CallIntMethod(envp, marpaESLIFRecognizerContextp->eslifRecognizerInterfacep, marpaESLIFRecognizerContextp->methodp, regexEslifCalloutp);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  *marpaESLIFValueResultOutp = (int) integer;

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  if (offsetVectorArrayp != NULL) {
    free(offsetVectorArrayp);
  }

  if (envp != NULL) {
    if (calloutNumberp != NULL) {
      (*envp)->DeleteLocalRef(envp, calloutNumberp);
    }
    if (calloutStringp != NULL) {
      (*envp)->DeleteLocalRef(envp, calloutStringp);
    }
    if (subjectp != NULL) {
      (*envp)->DeleteLocalRef(envp, subjectp);
    }
    if (patternp != NULL) {
      (*envp)->DeleteLocalRef(envp, patternp);
    }
    if (offsetVectorp != NULL) {
      (*envp)->DeleteLocalRef(envp, offsetVectorp);
    }
    if (markp != NULL) {
      (*envp)->DeleteLocalRef(envp, markp);
    }
    if (nextItemp != NULL) {
      (*envp)->DeleteLocalRef(envp, nextItemp);
    }
    if (regexEslifCalloutp != NULL) {
      (*envp)->DeleteLocalRef(envp, regexEslifCalloutp);
    }
  }

  return rcb;
}

/*****************************************************************************/
static void marpaESLIFJava_genericFreeCallbackv(void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp)
/*****************************************************************************/
{
  static const char *funcs = "marpaESLIFJava_genericFreeCallbackv";
  JNIEnv            *envp;

  /* Free callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    return;
  }
  switch (marpaESLIFValueResultp->type) {
  case MARPAESLIF_VALUE_TYPE_PTR:
    /* This is a global reference */
    if (marpaESLIFValueResultp->u.p.p != NULL) {
      (*envp)->DeleteGlobalRef(envp, (jobject) marpaESLIFValueResultp->u.p.p);
    }
    break;
  case MARPAESLIF_VALUE_TYPE_ARRAY:
    if (marpaESLIFValueResultp->u.a.p != NULL) {
      free(marpaESLIFValueResultp->u.a.p);
    }
    break;
  case MARPAESLIF_VALUE_TYPE_STRING:
    if (marpaESLIFValueResultp->u.s.p != NULL) {
      /* string may refer to the constant empty string */
      free(marpaESLIFValueResultp->u.s.p);
    }
    /* encoding may refer to the constant UTF8s */
    if ((marpaESLIFValueResultp->u.s.encodingasciis != NULL) && (marpaESLIFValueResultp->u.s.encodingasciis != marpaESLIFJava_UTF8s)) {
      free(marpaESLIFValueResultp->u.s.encodingasciis);
    }
    break;
  case MARPAESLIF_VALUE_TYPE_ROW:
    if (marpaESLIFValueResultp->u.r.p != NULL) {
      free(marpaESLIFValueResultp->u.r.p);
    }
    break;
  case MARPAESLIF_VALUE_TYPE_TABLE:
    if (marpaESLIFValueResultp->u.t.p != NULL) {
      free(marpaESLIFValueResultp->u.t.p);
    }
    break;
  default:
    break;
  }
}

/*****************************************************************************/
static jmethodID marpaESLIFJava_valueActionResolveri(JNIEnv *envp, marpaESLIFValueContext_t *marpaESLIFValueContextp, char *methods, char *signatures)
/*****************************************************************************/
{
  /* We do not used an internal hash, believing it is not worth the overhead */
  static const char       *funcs             = "marpaESLIFJava_valueActionResolveri";
  marpaESLIFMethodCache_t *methodCachep      = marpaESLIFValueContextp->methodCachep;
  size_t                   methodCacheSizel  = marpaESLIFValueContextp->methodCacheSizel;
  marpaESLIFMethodCache_t *localMethodCachep;
  jmethodID                methodp;
  size_t                   i;
  

  for (i = 0, localMethodCachep = methodCachep /* May be NULL */; i < methodCacheSizel; i++, localMethodCachep++) {
    if ((strcmp(localMethodCachep->methods, methods) == 0) && (strcmp(localMethodCachep->signatures, signatures) == 0)) {
      return localMethodCachep->methodp;
    }
  }

  /* If we are here, this mean that method with given signature was not resolved */
  if (methodCacheSizel <= 0) {
    methodCachep = (marpaESLIFMethodCache_t *) malloc(sizeof(marpaESLIFMethodCache_t));
    if (methodCachep == NULL) {
      RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
    }
  } else {
    methodCachep = (marpaESLIFMethodCache_t *) realloc(methodCachep, sizeof(marpaESLIFMethodCache_t) * (methodCacheSizel + 1));
    if (methodCachep == NULL) {
      RAISEEXCEPTIONF(envp, "realloc failure, %s", strerror(errno));
    }
  }

  marpaESLIFValueContextp->methodCachep     = methodCachep;
  marpaESLIFValueContextp->methodCacheSizel++;

  localMethodCachep                        = &(methodCachep[methodCacheSizel]);
  localMethodCachep->marpaESLIFClassCachep = &(marpaESLIFValueContextp->classCache);
  localMethodCachep->methods               = NULL;
  localMethodCachep->signatures            = NULL;
  localMethodCachep->methodp               = NULL;

  localMethodCachep->methods = strdup(methods);
  if (localMethodCachep->methods == NULL) {
    RAISEEXCEPTIONF(envp, "strdup failure, %s", strerror(errno));
  }

  localMethodCachep->signatures = strdup(signatures);
  if (localMethodCachep->signatures == NULL) {
    RAISEEXCEPTIONF(envp, "strdup failure, %s", strerror(errno));
  }

  methodp = (*envp)->GetMethodID(envp,
                                 marpaESLIFValueContextp->classCache.classp,
                                 methods,
                                 signatures);
  if (methodp == NULL) {
    /* We want OUR exception to be shown */
    (*envp)->ExceptionClear(envp);
    RAISEEXCEPTIONF(envp,
                    "Failed to find method \"%s\" with signature \"%s\" in class \"%s\"",
                    methods,
                    signatures,
                    marpaESLIFValueContextp->classCache.classs);
  }

  localMethodCachep->methodp = methodp;
  return methodp;

 err:
  return NULL;
}

/*****************************************************************************/
static jmethodID marpaESLIFJava_recognizerActionResolveri(JNIEnv *envp, marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp, char *methods, char *signatures)
/*****************************************************************************/
{
  /* We do not used an internal hash, believing it is not worth the overhead */
  static const char       *funcs             = "marpaESLIFJava_recognizerActionResolveri";
  marpaESLIFMethodCache_t *methodCachep      = marpaESLIFRecognizerContextp->methodCachep;
  size_t                   methodCacheSizel  = marpaESLIFRecognizerContextp->methodCacheSizel;
  marpaESLIFMethodCache_t *localMethodCachep;
  jmethodID                methodp;
  size_t                   i;

  for (i = 0, localMethodCachep = methodCachep /* May be NULL */; i < methodCacheSizel; i++, localMethodCachep++) {
    if ((strcmp(localMethodCachep->methods, methods) == 0) && (strcmp(localMethodCachep->signatures, signatures) == 0)) {
      return localMethodCachep->methodp;
    }
  }

  /* If we are here, this mean that method with given signature was not resolved */
  if (methodCacheSizel <= 0) {
    methodCachep = (marpaESLIFMethodCache_t *) malloc(sizeof(marpaESLIFMethodCache_t));
    if (methodCachep == NULL) {
      RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
    }
  } else {
    methodCachep = (marpaESLIFMethodCache_t *) realloc(methodCachep, sizeof(marpaESLIFMethodCache_t) * (methodCacheSizel + 1));
    if (methodCachep == NULL) {
      RAISEEXCEPTIONF(envp, "realloc failure, %s", strerror(errno));
    }
  }

  marpaESLIFRecognizerContextp->methodCachep     = methodCachep;
  marpaESLIFRecognizerContextp->methodCacheSizel++;

  localMethodCachep                        = &(methodCachep[methodCacheSizel]);
  localMethodCachep->marpaESLIFClassCachep = &(marpaESLIFRecognizerContextp->classCache);
  localMethodCachep->methods               = NULL;
  localMethodCachep->signatures            = NULL;
  localMethodCachep->methodp               = NULL;

  localMethodCachep->methods = strdup(methods);
  if (localMethodCachep->methods == NULL) {
    RAISEEXCEPTIONF(envp, "strdup failure, %s", strerror(errno));
  }

  localMethodCachep->signatures = strdup(signatures);
  if (localMethodCachep->signatures == NULL) {
    RAISEEXCEPTIONF(envp, "strdup failure, %s", strerror(errno));
  }

  methodp = (*envp)->GetMethodID(envp,
                                 marpaESLIFRecognizerContextp->classCache.classp,
                                 methods,
                                 signatures);
  if (methodp == NULL) {
    /* We want OUR exception to be shown */
    (*envp)->ExceptionClear(envp);
    RAISEEXCEPTIONF(envp,
                    "Failed to find method \"%s\" with signature \"%s\" in class \"%s\"",
                    methods,
                    signatures,
                    marpaESLIFRecognizerContextp->classCache.classs);
  }

  localMethodCachep->methodp = methodp;
  return methodp;

 err:
  return NULL;
}

/*****************************************************************************/
static void marpaESLIFJava_valueContextFreev(JNIEnv *envp, marpaESLIFValueContext_t *marpaESLIFValueContextp, short onStackb)
/*****************************************************************************/
{
  marpaESLIFMethodCache_t *methodCachep;
  size_t                   methodCacheSizel;
  marpaESLIFMethodCache_t *localMethodCachep;
  int                      i;
  jobject                  objectp;

  if (marpaESLIFValueContextp != NULL) {

    /* It is important to delete references in the reverse order of their creation */
    while (GENERICSTACK_USED(marpaESLIFValueContextp->objectStackp) > 0) {
      /* Last indice ... */
      i = GENERICSTACK_USED(marpaESLIFValueContextp->objectStackp) - 1;
      /* ... is cleared ... */
      if (GENERICSTACK_IS_PTR(marpaESLIFValueContextp->objectStackp, i)) {
        objectp = (jobject) GENERICSTACK_GET_PTR(marpaESLIFValueContextp->objectStackp, i);
        if (envp != NULL) {
          (*envp)->DeleteGlobalRef(envp, objectp);
        }
      }
      /* ... and becomes current used size */
      GENERICSTACK_USED(marpaESLIFValueContextp->objectStackp) = i;
    }
    GENERICSTACK_RESET(marpaESLIFValueContextp->objectStackp);

    if (marpaESLIFValueContextp->classCache.classp != NULL) {
      if (envp != NULL) {
        (*envp)->DeleteGlobalRef(envp, marpaESLIFValueContextp->classCache.classp);
      }
    }
    if (marpaESLIFValueContextp->classCache.classs != NULL) {
      free(marpaESLIFValueContextp->classCache.classs);
    }
    if (marpaESLIFValueContextp->methodCachep != NULL) {
      methodCachep      = marpaESLIFValueContextp->methodCachep;
      methodCacheSizel  = marpaESLIFValueContextp->methodCacheSizel;

      for (i = 0, localMethodCachep = methodCachep; i < (int) methodCacheSizel; i++, localMethodCachep++) {
        if (localMethodCachep->methods != NULL) {
          free(localMethodCachep->methods);
        }
        if (localMethodCachep->signatures != NULL) {
          free(localMethodCachep->signatures);
        }
      }
      free(marpaESLIFValueContextp->methodCachep);
    }
    if (marpaESLIFValueContextp->eslifGrammarp != NULL) {
      (*envp)->DeleteGlobalRef(envp, marpaESLIFValueContextp->eslifGrammarp);
    }
    if (! onStackb) {
      free(marpaESLIFValueContextp);
    }
  }
}

/*****************************************************************************/
static void marpaESLIFJava_recognizerContextFreev(JNIEnv *envp, marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp, short onStackb)
/*****************************************************************************/
{
  marpaESLIFMethodCache_t *methodCachep;
  size_t                   methodCacheSizel;
  marpaESLIFMethodCache_t *localMethodCachep;
  int                      i;
  jobject                  objectp;
  genericStack_t          *lexemeStackp;

  if (marpaESLIFRecognizerContextp != NULL) {

    /* It is important to delete references in the reverse order of their creation */
    while (GENERICSTACK_USED(marpaESLIFRecognizerContextp->objectStackp) > 0) {
      /* Last indice ... */
      i = GENERICSTACK_USED(marpaESLIFRecognizerContextp->objectStackp) - 1;
      /* ... is cleared ... */
      if (GENERICSTACK_IS_PTR(marpaESLIFRecognizerContextp->objectStackp, i)) {
        objectp = (jobject) GENERICSTACK_GET_PTR(marpaESLIFRecognizerContextp->objectStackp, i);
        if (envp != NULL) {
          (*envp)->DeleteGlobalRef(envp, objectp);
        }
      }
      /* ... and becomes current used size */
      GENERICSTACK_USED(marpaESLIFRecognizerContextp->objectStackp) = i;
    }
    GENERICSTACK_RESET(marpaESLIFRecognizerContextp->objectStackp);

    lexemeStackp = marpaESLIFRecognizerContextp->lexemeStackp;
    if (lexemeStackp != NULL) {
      /* It is important to delete global references in the reverse order of their creation */
      while (GENERICSTACK_USED(lexemeStackp) > 0) {
        i = GENERICSTACK_USED(lexemeStackp) - 1;
        if (GENERICSTACK_IS_PTR(lexemeStackp, i)) {
          objectp = (jobject) GENERICSTACK_POP_PTR(lexemeStackp);
          if (objectp != NULL) {
            (*envp)->DeleteGlobalRef(envp, objectp);
          }
        } else {
          GENERICSTACK_USED(lexemeStackp)--;
        }
      }
      GENERICSTACK_RESET(marpaESLIFRecognizerContextp->lexemeStackp);
    }
    if (marpaESLIFRecognizerContextp->classCache.classp != NULL) {
      if (envp != NULL) {
        (*envp)->DeleteGlobalRef(envp, marpaESLIFRecognizerContextp->classCache.classp);
      }
    }
    if (marpaESLIFRecognizerContextp->classCache.classs != NULL) {
      free(marpaESLIFRecognizerContextp->classCache.classs);
    }
    if (marpaESLIFRecognizerContextp->methodCachep != NULL) {
      methodCachep      = marpaESLIFRecognizerContextp->methodCachep;
      methodCacheSizel  = marpaESLIFRecognizerContextp->methodCacheSizel;

      for (i = 0, localMethodCachep = methodCachep; i < (int) methodCacheSizel; i++, localMethodCachep++) {
        if (localMethodCachep->methods != NULL) {
          free(localMethodCachep->methods);
        }
        if (localMethodCachep->signatures != NULL) {
          free(localMethodCachep->signatures);
        }
      }
      free(marpaESLIFRecognizerContextp->methodCachep);
    }
    if (! onStackb) {
      free(marpaESLIFRecognizerContextp);
    }
  }
}

/*****************************************************************************/
static short marpaESLIFJava_valueContextInitb(JNIEnv *envp, jobject eslifValueInterfacep, jobject eslifGrammarp, marpaESLIFValueContext_t *marpaESLIFValueContextp)
/*****************************************************************************/
{
  static const char *funcs   = "marpaESLIFJava_valueContextInitb";
  jclass             classp  = NULL;
  jstring            stringp = NULL;
  const char        *classs  = NULL;
  short              rcb;
  jboolean           isCopy;

  marpaESLIFValueContextp->eslifValueInterfacep          = eslifValueInterfacep;
  marpaESLIFValueContextp->eslifGrammarp                 = NULL;
  marpaESLIFValueContextp->classCache.classs             = NULL;
  marpaESLIFValueContextp->classCache.classp             = NULL;
  marpaESLIFValueContextp->methodCachep                  = NULL;
  marpaESLIFValueContextp->methodCacheSizel              = 0;
  marpaESLIFValueContextp->methodp                       = 0;
  marpaESLIFValueContextp->actions                       = NULL;
  marpaESLIFValueContextp->envp                          = envp;
  marpaESLIFValueContextp->objectStackp                  = &(marpaESLIFValueContextp->_objectStack);

  GENERICSTACK_INIT(marpaESLIFValueContextp->objectStackp);
  if (GENERICSTACK_ERROR(marpaESLIFValueContextp->objectStackp)) {
    marpaESLIFValueContextp->objectStackp = NULL;
    RAISEEXCEPTIONF(envp, "GENERICSTACK_INIT failure, %s", strerror(errno));
  }

  /* We want the grammar instance to stay alive until valuation is done */
  marpaESLIFValueContextp->eslifGrammarp = (*envp)->NewGlobalRef(envp, eslifGrammarp);
  if (marpaESLIFValueContextp->eslifGrammarp == NULL) {
    RAISEEXCEPTION(envp, "NewGlobalRef failure");
  }

  if (eslifValueInterfacep != NULL) {
    /* For run-time resolving of actions we need current jclass */
    classp = (*envp)->GetObjectClass(envp, eslifValueInterfacep);
    if (classp == NULL) {
      /* An exception was (must have been) raised */
      RAISEEXCEPTION(envp, "GetObjectClass failure");
    }

    stringp = (*envp)->CallObjectMethod(envp, classp, JAVA_LANG_CLASS_CLASS_getName_METHODP);
    if (stringp == NULL) {
      /* An exception was (must have been) raised */
      RAISEEXCEPTION(envp, "CallObjectMethod failure");
    }

    classs = (*envp)->GetStringUTFChars(envp, stringp, &isCopy);
    if (classs == NULL) {
      /* An exception was (must have been) raised */
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
    marpaESLIFValueContextp->classCache.classs = strdup(classs);
    if (marpaESLIFValueContextp->classCache.classs == NULL) {
      RAISEEXCEPTIONF(envp, "strdup failure, %s", strerror(errno));
    }

    marpaESLIFValueContextp->classCache.classp = (*envp)->NewGlobalRef(envp, classp);
    if (marpaESLIFValueContextp->classCache.classp == NULL) {
      /* An exception was (must have been) raised */
      RAISEEXCEPTION(envp, "NewGlobalRef failure");
    }
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  if (envp != NULL) {
    if (stringp != NULL) {
      if (classs != NULL) {
        (*envp)->ReleaseStringUTFChars(envp, stringp, classs);
      } else {
        (*envp)->DeleteLocalRef(envp, stringp);
      }
    }
    if (classp != NULL) {
      (*envp)->DeleteLocalRef(envp, classp);
    }
  }

  return rcb;
}

/*****************************************************************************/
static short marpaESLIFJava_recognizerContextInitb(JNIEnv *envp, jobject eslifRecognizerInterfacep, marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp, short haveLexemeStackb)
/*****************************************************************************/
{
  static const char *funcs   = "marpaESLIFJava_recognizerContextInitb";
  jclass             classp  = NULL;
  jstring            stringp = NULL;
  const char        *classs  = NULL;
  short              rcb;
  jboolean           isCopy;

  marpaESLIFRecognizerContextp->eslifRecognizerInterfacep = eslifRecognizerInterfacep;
  marpaESLIFRecognizerContextp->byteArrayp                = NULL;
  marpaESLIFRecognizerContextp->encodingp                 = NULL;
  marpaESLIFRecognizerContextp->lexemeStackp              = haveLexemeStackb ? &(marpaESLIFRecognizerContextp->_lexemeStackp) : NULL;
  marpaESLIFRecognizerContextp->envp                      = envp;
  marpaESLIFRecognizerContextp->classCache.classs         = NULL;
  marpaESLIFRecognizerContextp->classCache.classp         = NULL;
  marpaESLIFRecognizerContextp->methodCachep              = NULL;
  marpaESLIFRecognizerContextp->methodCacheSizel          = 0;
  marpaESLIFRecognizerContextp->actions                   = NULL;
  marpaESLIFRecognizerContextp->methodp                   = 0;
  marpaESLIFRecognizerContextp->objectStackp              = &(marpaESLIFRecognizerContextp->_objectStack);

  GENERICSTACK_INIT(marpaESLIFRecognizerContextp->objectStackp);
  if (GENERICSTACK_ERROR(marpaESLIFRecognizerContextp->objectStackp)) {
    marpaESLIFRecognizerContextp->objectStackp = NULL;
    RAISEEXCEPTIONF(envp, "GENERICSTACK_INIT failure, %s", strerror(errno));
  }

  if (haveLexemeStackb) {
    /* A call to jniParse do not need this stack */
    GENERICSTACK_INIT(marpaESLIFRecognizerContextp->lexemeStackp);
    if (GENERICSTACK_ERROR(marpaESLIFRecognizerContextp->lexemeStackp)) {
      marpaESLIFRecognizerContextp->lexemeStackp = NULL;
      RAISEEXCEPTIONF(envp, "GENERICSTACK_INIT failure, %s", strerror(errno));
    }
  }

  /* For run-time resolving of actions we need current jclass */
  classp = (*envp)->GetObjectClass(envp, eslifRecognizerInterfacep);
  if (classp == NULL) {
    /* An exception was (must have been) raised */
    RAISEEXCEPTION(envp, "GetObjectClass failure");
  }

  stringp = (*envp)->CallObjectMethod(envp, classp, JAVA_LANG_CLASS_CLASS_getName_METHODP);
  if (stringp == NULL) {
    /* An exception was (must have been) raised */
    RAISEEXCEPTION(envp, "CallObjectMethod failure");
  }

  classs = (*envp)->GetStringUTFChars(envp, stringp, &isCopy);
  if (classs == NULL) {
    /* An exception was (must have been) raised */
    RAISEEXCEPTION(envp, "GetStringUTFChars failure");
  }
  marpaESLIFRecognizerContextp->classCache.classs = strdup(classs);
  if (marpaESLIFRecognizerContextp->classCache.classs == NULL) {
    RAISEEXCEPTIONF(envp, "strdup failure, %s", strerror(errno));
  }

  marpaESLIFRecognizerContextp->classCache.classp = (*envp)->NewGlobalRef(envp, classp);
  if (marpaESLIFRecognizerContextp->classCache.classp == NULL) {
    /* An exception was (must have been) raised */
    RAISEEXCEPTION(envp, "NewGlobalRef failure");
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  if (envp != NULL) {
    if (stringp != NULL) {
      if (classs != NULL) {
        (*envp)->ReleaseStringUTFChars(envp, stringp, classs);
      } else {
        (*envp)->DeleteLocalRef(envp, stringp);
      }
    }
    if (classp != NULL) {
      (*envp)->DeleteLocalRef(envp, classp);
    }
  }

  return rcb;
}

/*****************************************************************************/
static void marpaESLIFJava_representationCallbackDisposev(void *userDatavp, char *inputcp, size_t inputl, char *encodingasciis)
/*****************************************************************************/
{
  if (inputcp != NULL) {
    free(inputcp);
  }
}

/*****************************************************************************/
static short marpaESLIFJava_representationCallbackb(void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp, char **inputcpp, size_t *inputlp, char **encodingasciisp, marpaESLIFRepresentationDispose_t *disposeCallbackpp)
/*****************************************************************************/
{
  static const char        *funcs = "marpaESLIFJava_representationCallbackb";
  JNIEnv                   *envp;
  marpaESLIFValueContext_t *marpaESLIFValueContextp = (marpaESLIFValueContext_t *) userDatavp;
  jstring                   stringp = NULL;
  jclass                    classp = NULL;
  jobject                   objectp;
  jmethodID                 methodp;
  jsize                     UTF16NbCodeUnit;
  size_t                    UTF8NbByte;
  jbyteArray                UTF8ByteArray = NULL;
  jbyte                    *UTF8Bytes = NULL;
  char                     *utf8s = NULL;
  size_t                    utf8l = 0;
  short                     rcb;
  jboolean                  isCopy;

  /* Representation callack is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* We always push a PTR */
  if (marpaESLIFValueResultp->type != MARPAESLIF_VALUE_TYPE_PTR) {
    RAISEEXCEPTIONF(envp, "User-defined value type is not MARPAESLIF_VALUE_TYPE_PTR but %d", marpaESLIFValueResultp->type);
  }
  objectp = (jobject) marpaESLIFValueResultp->u.p.p;

  /* We need to resolve the "toString" method - that every object have in Java */
  classp = (*envp)->GetObjectClass(envp, objectp);
  if (classp == NULL) {
    /* We want OUR exception to be raised */
    (*envp)->ExceptionClear(envp);
    RAISEEXCEPTION(envp, "Failed to find object class");
  }
  methodp = (*envp)->GetMethodID(envp, classp, "toString", "()Ljava/lang/String;");
  if (methodp == NULL) {
    /* We want OUR exception to be raised */
    (*envp)->ExceptionClear(envp);
    RAISEEXCEPTION(envp, "Failed to find toString method");
  }

  /* Call the toString method */
  stringp = (*envp)->CallObjectMethod(envp, objectp, methodp);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  /* It is not illegal to have a NULL stringp */
  if (stringp != NULL) {
    /* The UTF-16 string is always injected as an UTF-8 thingy as per the documentation */
    UTF16NbCodeUnit = (*envp)->GetStringLength(envp, stringp);
    /* If zero character, we do nothing - nothing will be appended by default */
    if (UTF16NbCodeUnit > 0) {
      /* Convert to UTF-8: a string in java is ALWAYS in native charset */
      UTF8ByteArray = (jbyteArray) (*envp)->CallObjectMethod(envp, stringp, JAVA_LANG_STRING_CLASS_getBytes_String_METHODP, marpaESLIF_UTF8p);
      if (UTF8ByteArray == NULL) {
        RAISEEXCEPTIONF(envp, "String.getBytes(..., \"%s\")", "UTF-8");
      }
      UTF8NbByte = (*envp)->GetArrayLength(envp, UTF8ByteArray);
      if (UTF8NbByte <= 0) {
        RAISEEXCEPTIONF(envp, "UTF-8 string length is %ld bytes long while UTF-16 string length is %ld code units ?", (unsigned long) UTF8NbByte, (unsigned long) UTF16NbCodeUnit);
      }
      UTF8Bytes = (*envp)->GetByteArrayElements(envp, UTF8ByteArray, &isCopy);

      utf8s = (char *) malloc((size_t) (UTF8NbByte + 1)); /* Hiden NUL byte */
      if (utf8s == NULL) {
        RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
      }
      memcpy(utf8s, UTF8Bytes, UTF8NbByte);
      utf8l  = (size_t) UTF8NbByte;
      utf8s[utf8l] = '\0';
    } else {
      utf8s = (char *) malloc(1); /* Hiden NUL byte */
      if (utf8s == NULL) {
        RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
      }
      utf8s[0] = '\0';
      utf8l  = 0;
    }
  } else {
    utf8s = (char *) malloc(1); /* Hiden NUL byte */
    if (utf8s == NULL) {
      RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
    }
    utf8s[0] = '\0';
    utf8l  = 0;
  }

  /* In any case, encoding must be set */
  *inputcpp          = utf8s;
  *inputlp           = utf8l;
  *encodingasciisp   = (char *) marpaESLIFJava_UTF8s;
  *disposeCallbackpp = marpaESLIFJava_representationCallbackDisposev;

  rcb = 1;
  goto done;
  
 err:
  if (utf8s != NULL) {
    free(utf8s);
  }
  rcb = 0;

 done:
  if (classp != NULL) {
    (*envp)->DeleteLocalRef(envp, classp);
  }
  if (UTF8ByteArray != NULL) {
    if (UTF8Bytes != NULL) {
      (*envp)->ReleaseByteArrayElements(envp, UTF8ByteArray, UTF8Bytes, JNI_ABORT);
    }
    (*envp)->DeleteLocalRef(envp, UTF8ByteArray);
  }

  return rcb;
}

/*****************************************************************************/
static jobject marpaESLIFJava_grammarPropertiesp(JNIEnv *envp, marpaESLIFGrammarProperty_t *grammarPropertyp)
/*****************************************************************************/
{
  static const char           *funcs = "marpaESLIFJava_grammarPropertiesp";
  jobject                      propertiesp = NULL;
  jint                         level;
  jint                         maxLevel;
  jstring                      description;
  jboolean                     latm;
  jstring                      defaultSymbolAction;
  jstring                      defaultRuleAction;
  jstring                      defaultEventAction;
  jstring                      defaultRegexAction;
  jint                         startId;
  jint                         discardId;
  jintArray                    symbolIds;
  jintArray                    ruleIds;
  jint                        *symbolIdsIntp = NULL;
  jint                        *ruleIdsIntp = NULL;
  jstring                      defaultEncoding;
  jstring                      fallbackEncoding;
  size_t                       i;

  level               = (jint) grammarPropertyp->leveli;
  maxLevel            = (jint) grammarPropertyp->maxLeveli;
  description         = marpaESLIFJava_marpaESLIFStringToJavap(envp, grammarPropertyp->descp);
  latm                = grammarPropertyp->latmb ? JNI_TRUE : JNI_FALSE;
  defaultSymbolAction = marpaESLIFJava_marpaESLIFActionToJavap(envp, grammarPropertyp->defaultSymbolActionp);
  defaultRuleAction   = marpaESLIFJava_marpaESLIFActionToJavap(envp, grammarPropertyp->defaultRuleActionp);
  defaultEventAction  = marpaESLIFJava_marpaESLIFActionToJavap(envp, grammarPropertyp->defaultEventActionp);
  defaultRegexAction  = marpaESLIFJava_marpaESLIFActionToJavap(envp, grammarPropertyp->defaultRegexActionp);
  startId             = (jint) grammarPropertyp->starti;
  discardId           = (jint) grammarPropertyp->discardi;
  defaultEncoding     = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, grammarPropertyp->defaultEncodings);
  fallbackEncoding    = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, grammarPropertyp->fallbackEncodings);

  if (grammarPropertyp->nsymboll <= 0) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_grammarproperty_currentb returned no symbol");
  }
  symbolIdsIntp = (jint *) malloc(sizeof(jint) * grammarPropertyp->nsymboll);
  if (symbolIdsIntp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  for (i = 0; i < grammarPropertyp->nsymboll; i++) {
    symbolIdsIntp[i] = (jint) grammarPropertyp->symbolip[i];
  }

  symbolIds = (*envp)->NewIntArray(envp, (jsize) grammarPropertyp->nsymboll);
  if (symbolIds == NULL) {
    RAISEEXCEPTION(envp, "NewIntArray failure");
  }

  (*envp)->SetIntArrayRegion(envp, symbolIds, 0, (jsize) grammarPropertyp->nsymboll, symbolIdsIntp);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  if (grammarPropertyp->nrulel <= 0) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_grammarproperty_currentb returned no rule");
  }
  ruleIdsIntp = (jint *) malloc(sizeof(jint) * grammarPropertyp->nrulel);
  if (ruleIdsIntp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  for (i = 0; i < grammarPropertyp->nrulel; i++) {
    ruleIdsIntp[i] = (jint) grammarPropertyp->ruleip[i];
  }

  ruleIds = (*envp)->NewIntArray(envp, (jsize) grammarPropertyp->nrulel);
  if (ruleIds == NULL) {
    RAISEEXCEPTION(envp, "NewIntArray failure");
  }

  (*envp)->SetIntArrayRegion(envp, ruleIds, 0, (jsize) grammarPropertyp->nrulel, ruleIdsIntp);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  propertiesp = (*envp)->NewObject(envp,
                                   MARPAESLIF_ESLIFGRAMMARPROPERTIES_CLASSP,
                                   MARPAESLIF_ESLIFGRAMMARPROPERTIES_CLASS_init_METHODP,
                                   level,
                                   maxLevel,
                                   description,
                                   latm,
                                   defaultSymbolAction,
                                   defaultRuleAction,
                                   defaultEventAction,
                                   defaultRegexAction,
                                   startId,
                                   discardId,
                                   symbolIds,
                                   ruleIds,
                                   defaultEncoding,
                                   fallbackEncoding
                                   );

 err: /* err and done share the same code */
  /* Java will immediately see the exception if there is one */
  if (symbolIdsIntp != NULL) {
    free(symbolIdsIntp);
  }
  if (ruleIdsIntp != NULL) {
    free(ruleIdsIntp);
  }
  return propertiesp;
}

/*****************************************************************************/
static jobject marpaESLIFJava_rulePropertiesp(JNIEnv *envp, marpaESLIFRuleProperty_t *rulePropertyp)
/*****************************************************************************/
{
  static const char           *funcs             = "marpaESLIFJava_rulePropertiesp";
  jobject                      propertiesp       = NULL;
  jintArray                    rhsIds            = NULL;
  jbooleanArray                skipIndices       = NULL;
  jint                        *rhsIdsIntp        = NULL;
  jboolean                    *skipIndicesShortp = NULL;
  jint                         id;
  jstring                      description;
  jstring                      show;
  jint                         lhsId;
  jint                         separatorId;
  jint                         exceptionId;
  jstring                      action;
  jstring                      discardEvent;
  jboolean                     discardEventInitialState;
  jint                         rank;
  jboolean                     nullRanksHigh;
  jboolean                     sequence;
  jboolean                     proper;
  jint                         minimum;
  jboolean                     internal;
  jint                         propertyBitSet;
  jboolean                     hideseparator;
  size_t                       i;

  id          = (jint) rulePropertyp->idi;
  description = marpaESLIFJava_marpaESLIFStringToJavap(envp, rulePropertyp->descp);
  show        = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, rulePropertyp->asciishows);
  lhsId       = (jint) rulePropertyp->lhsi;
  separatorId = (jint) rulePropertyp->separatori;

  if (rulePropertyp->nrhsl > 0) {
    rhsIdsIntp = (jint *) malloc(sizeof(jint) * rulePropertyp->nrhsl);
    if (rhsIdsIntp == NULL) {
      RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
    }
    for (i = 0; i < rulePropertyp->nrhsl; i++) {
      rhsIdsIntp[i] = (jint) rulePropertyp->rhsip[i];
    }
    rhsIds = (*envp)->NewIntArray(envp, (jsize) rulePropertyp->nrhsl);
    if (rhsIds == NULL) {
      RAISEEXCEPTION(envp, "NewIntArray failure");
    }
    (*envp)->SetIntArrayRegion(envp, rhsIds, 0, (jsize) rulePropertyp->nrhsl, rhsIdsIntp);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  if (rulePropertyp->skipbp != NULL) {
    skipIndicesShortp = (jboolean *) malloc(sizeof(jboolean) * rulePropertyp->nrhsl);
    if (skipIndicesShortp == NULL) {
      RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
    }
    for (i = 0; i < rulePropertyp->nrhsl; i++) {
      skipIndicesShortp[i] = rulePropertyp->skipbp[i] ? JNI_TRUE : JNI_FALSE;
    }
    skipIndices = (*envp)->NewBooleanArray(envp, (jsize) rulePropertyp->nrhsl);
    if (skipIndices == NULL) {
      RAISEEXCEPTION(envp, "NewBooleanArray failure");
    }
    (*envp)->SetBooleanArrayRegion(envp, skipIndices, 0, (jsize) rulePropertyp->nrhsl, skipIndicesShortp);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  exceptionId = (jint) rulePropertyp->exceptioni;
  action      = marpaESLIFJava_marpaESLIFActionToJavap(envp, rulePropertyp->actionp);

  if (rulePropertyp->discardEvents == NULL) {
    discardEvent = NULL;
  } else {
    discardEvent = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, rulePropertyp->discardEvents);
  }

  discardEventInitialState = rulePropertyp->discardEventb ? JNI_TRUE : JNI_FALSE;
  rank                     = (jint) rulePropertyp->ranki;
  nullRanksHigh            = rulePropertyp->nullRanksHighb ? JNI_TRUE : JNI_FALSE;
  sequence                 = rulePropertyp->sequenceb ? JNI_TRUE : JNI_FALSE;
  proper                   = rulePropertyp->properb ? JNI_TRUE : JNI_FALSE;
  minimum                  = (jint) rulePropertyp->minimumi;
  internal                 = rulePropertyp->internalb ? JNI_TRUE : JNI_FALSE;
  propertyBitSet           = (jint) rulePropertyp->propertyBitSet;
  hideseparator            = rulePropertyp->hideseparatorb ? JNI_TRUE : JNI_FALSE;

  propertiesp = (*envp)->NewObject(envp,
                                   MARPAESLIF_ESLIFGRAMMARRULEPROPERTIES_CLASSP,
                                   MARPAESLIF_ESLIFGRAMMARRULEPROPERTIES_CLASS_init_METHODP,
                                   id,
                                   description,
                                   show,
                                   lhsId,
                                   separatorId,
                                   rhsIds,
                                   skipIndices,
                                   exceptionId,
                                   action,
                                   discardEvent,
                                   discardEventInitialState,
                                   rank,
                                   nullRanksHigh,
                                   sequence,
                                   proper,
                                   minimum,
                                   internal,
                                   propertyBitSet,
                                   hideseparator
                                   );

 err: /* err and done share the same code */
  /* Java will immediately see the exception if there is one */
  if (rhsIdsIntp != NULL) {
    free(rhsIdsIntp);
  }
  if (skipIndicesShortp != NULL) {
    free(skipIndicesShortp);
  }
  return propertiesp;
}

/*****************************************************************************/
static jobject marpaESLIFJava_symbolPropertiesp(JNIEnv *envp, marpaESLIFSymbolProperty_t *symbolPropertyp)
/*****************************************************************************/
{
  static const char *funcs = "marpaESLIFJava_symbolPropertiesp";
  jobject            propertiesp = NULL;
  jobject            type;
  jboolean           start;
  jboolean           discard;
  jboolean           discardRhs;
  jboolean           lhs;
  jboolean           top;
  jint               id;
  jstring            description;
  jstring            eventBefore;
  jboolean           eventBeforeInitialState;
  jstring            eventAfter;
  jboolean           eventAfterInitialState;
  jstring            eventPredicted;
  jboolean           eventPredictedInitialState;
  jstring            eventNulled;
  jboolean           eventNulledInitialState;
  jstring            eventCompleted;
  jboolean           eventCompletedInitialState;
  jstring            discardEvent;
  jboolean           discardEventInitialState;
  jint               lookupResolvedLeveli;
  jint               priority;
  jstring            nullableAction;
  jint               propertyBitSet;
  jint               eventBitSet;
  jstring            symbolAction;
  jstring            ifAction;

  type = (*envp)->CallStaticObjectMethod(envp, MARPAESLIF_ESLIFSYMBOLTYPE_CLASSP, MARPAESLIF_ESLIFSYMBOLTYPE_CLASS_get_METHODP, symbolPropertyp->type);
  if (type == NULL) {
    RAISEEXCEPTION(envp, "CallStaticObjectMethod failure");
  }


  start                      = symbolPropertyp->startb ? JNI_TRUE : JNI_FALSE;
  discard                    = symbolPropertyp->discardb ? JNI_TRUE : JNI_FALSE;
  discardRhs                 = symbolPropertyp->discardRhsb ? JNI_TRUE : JNI_FALSE;
  lhs                        = symbolPropertyp->lhsb ? JNI_TRUE : JNI_FALSE;
  top                        = symbolPropertyp->topb ? JNI_TRUE : JNI_FALSE;
  id                         = (jint) symbolPropertyp->idi;
  description                = marpaESLIFJava_marpaESLIFStringToJavap(envp, symbolPropertyp->descp);
  eventBefore                = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, symbolPropertyp->eventBefores);
  eventBeforeInitialState    = symbolPropertyp->eventBeforeb ? JNI_TRUE : JNI_FALSE;
  eventAfter                 = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, symbolPropertyp->eventAfters);
  eventAfterInitialState     = symbolPropertyp->eventAfterb ? JNI_TRUE : JNI_FALSE;
  eventPredicted             = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, symbolPropertyp->eventPredicteds);
  eventPredictedInitialState = symbolPropertyp->eventPredictedb ? JNI_TRUE : JNI_FALSE;
  eventNulled                = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, symbolPropertyp->eventNulleds);
  eventNulledInitialState    = symbolPropertyp->eventNulledb ? JNI_TRUE : JNI_FALSE;
  eventCompleted             = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, symbolPropertyp->eventCompleteds);
  eventCompletedInitialState = symbolPropertyp->eventCompletedb ? JNI_TRUE : JNI_FALSE;
  discardEvent               = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, symbolPropertyp->discardEvents);
  discardEventInitialState   = symbolPropertyp->discardEventb ? JNI_TRUE : JNI_FALSE;
  lookupResolvedLeveli       = (jint) symbolPropertyp->lookupResolvedLeveli;
  priority                   = (jint) symbolPropertyp->priorityi;
  nullableAction             = marpaESLIFJava_marpaESLIFActionToJavap(envp, symbolPropertyp->nullableActionp);
  propertyBitSet             = (jint) symbolPropertyp->propertyBitSet;
  eventBitSet                = (jint) symbolPropertyp->eventBitSet;
  symbolAction               = marpaESLIFJava_marpaESLIFActionToJavap(envp, symbolPropertyp->symbolActionp);
  ifAction                   = marpaESLIFJava_marpaESLIFActionToJavap(envp, symbolPropertyp->ifActionp);

  propertiesp = (*envp)->NewObject(envp,
                                   MARPAESLIF_ESLIFGRAMMARSYMBOLPROPERTIES_CLASSP,
                                   MARPAESLIF_ESLIFGRAMMARSYMBOLPROPERTIES_CLASS_init_METHODP,
                                   type,
                                   start,
                                   discard,
                                   discardRhs,
                                   lhs,
                                   top,
                                   id,
                                   description,
                                   eventBefore,
                                   eventBeforeInitialState,
                                   eventAfter,
                                   eventAfterInitialState,
                                   eventPredicted,
                                   eventPredictedInitialState,
                                   eventNulled,
                                   eventNulledInitialState,
                                   eventCompleted,
                                   eventCompletedInitialState,
                                   discardEvent,
                                   discardEventInitialState,
                                   lookupResolvedLeveli,
                                   priority,
                                   nullableAction,
                                   propertyBitSet,
                                   eventBitSet,
                                   symbolAction,
                                   ifAction
                                   );

 err: /* err and done share the same code */
  return propertiesp;
}

/*****************************************************************************/
static short marpaESLIFJava_valueGetObjectp(marpaESLIFValueContext_t *marpaESLIFValueContextp, marpaESLIFValue_t *marpaESLIFValuep, int stackindicei, marpaESLIFValueResult_t *marpaESLIFValueResultLexemep, jobject *objectpp)
/*****************************************************************************/
{
  static const char       *funcs = "marpaESLIFJava_valueGetObjectp";
  marpaESLIFValueResult_t *marpaESLIFValueResultp;
  jobject                  objectp;
  short                    rcb;

  if (marpaESLIFValueResultLexemep != NULL) {
    marpaESLIFValueResultp = marpaESLIFValueResultLexemep;
  } else {
    marpaESLIFValueResultp = marpaESLIFValue_stack_getp(marpaESLIFValuep, stackindicei);
    if (marpaESLIFValueResultp == NULL) {
      RAISEEXCEPTIONF(marpaESLIFValueContextp->envp, "marpaESLIFValueResultp is NULL at stack indice %d", stackindicei);
    }
  }

  if (! marpaESLIFValue_importb(marpaESLIFValuep, marpaESLIFValueResultp)) {
    RAISEEXCEPTIONF(marpaESLIFValueContextp->envp, "marpaESLIFValue_importb failure, %s", strerror(errno));
  }

  if (GENERICSTACK_USED(marpaESLIFValueContextp->objectStackp) != 1) {
    RAISEEXCEPTIONF(marpaESLIFValueContextp->envp, "Internal value stack is %d instead of 1", GENERICSTACK_USED(marpaESLIFValueContextp->objectStackp));
  }

  objectp = (jobject) GENERICSTACK_POP_PTR(marpaESLIFValueContextp->objectStackp);
  if (objectpp != NULL) {
    *objectpp = objectp;
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static short marpaESLIFJava_recognizerGetObjectp(marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp, marpaESLIFRecognizer_t *marpaESLIFRecognizerp, marpaESLIFValueResult_t *marpaESLIFValueResultp, jobject *objectpp)
/*****************************************************************************/
{
  static const char       *funcs = "marpaESLIFJava_recognizerGetObjectp";
  jobject                  objectp;
  short                    rcb;

  if (! marpaESLIFRecognizer_importb(marpaESLIFRecognizerp, marpaESLIFValueResultp)) {
    RAISEEXCEPTIONF(marpaESLIFRecognizerContextp->envp, "marpaESLIFRecognizer_importb failure, %s", strerror(errno));
  }

  if (GENERICSTACK_USED(marpaESLIFRecognizerContextp->objectStackp) != 1) {
    RAISEEXCEPTIONF(marpaESLIFRecognizerContextp->envp, "Internal value stack is %d instead of 1", GENERICSTACK_USED(marpaESLIFRecognizerContextp->objectStackp));
  }

  objectp = (jobject) GENERICSTACK_POP_PTR(marpaESLIFRecognizerContextp->objectStackp);
  if (objectpp != NULL) {
    *objectpp = objectp;
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static short marpaESLIFJava_valueImportb(marpaESLIFValue_t *marpaESLIFValuep, void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp)
/*****************************************************************************/
{
  marpaESLIFValueContext_t     *marpaESLIFValueContextp = (marpaESLIFValueContext_t *) userDatavp;
  JNIEnv                       *envp                    = marpaESLIFValueContextp->envp;
  marpaESLIF_t                 *marpaESLIFp             = marpaESLIFGrammar_eslifp(marpaESLIFRecognizer_grammarp(marpaESLIFValue_recognizerp(marpaESLIFValuep)));

  return marpaESLIFJava_importb(envp, marpaESLIFp, marpaESLIFValueContextp->objectStackp, marpaESLIFValueResultp);
}

/*****************************************************************************/
static short marpaESLIFJava_recognizerImportb(marpaESLIFRecognizer_t *marpaESLIFRecognizerp, void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp)
/*****************************************************************************/
{
  marpaESLIFRecognizerContext_t *marpaESLIFRecognizerContextp = (marpaESLIFRecognizerContext_t *) userDatavp;
  JNIEnv                        *envp                         = marpaESLIFRecognizerContextp->envp;
  marpaESLIF_t                  *marpaESLIFp                  = marpaESLIFGrammar_eslifp(marpaESLIFRecognizer_grammarp(marpaESLIFRecognizerp));

  return marpaESLIFJava_importb(envp, marpaESLIFp, marpaESLIFRecognizerContextp->objectStackp, marpaESLIFValueResultp);
}

/*****************************************************************************/
static short marpaESLIFJava_importb(JNIEnv *envp, marpaESLIF_t *marpaESLIFp, genericStack_t *objectStackp, marpaESLIFValueResult_t *marpaESLIFValueResultp)
/*****************************************************************************/
{
  static const char *funcs      = "marpaESLIFJava_importb";
  jbyteArray         byteArrayp = NULL;
  jstring            encodingp  = NULL;
  short              rcb;
  jobject            objectp;
  jobjectArray       objectArrayp;
  jboolean           jb;
  size_t             i;
  jobject            objectHashMapp;
  jobject            keyp;
  jobject            valuep;

  /*
    marpaESLIF Type                    C type    C nb_bits      Java Type
    
    MARPAESLIF_VALUE_TYPE_UNDEF
    MARPAESLIF_VALUE_TYPE_CHAR         char      CHAR_BIT       java.lang.Byte or java.lang.Short or java.lang.Integer or java.lang.Long or java.math.BigInteger
    MARPAESLIF_VALUE_TYPE_SHORT        short     >= 16          java.lang.Byte or java.lang.Short or java.lang.Integer or java.lang.Long or java.math.BigInteger
    MARPAESLIF_VALUE_TYPE_INT          int       >= 16          java.lang.Byte or java.lang.Short or java.lang.Integer or java.lang.Long or java.math.BigInteger
    MARPAESLIF_VALUE_TYPE_LONG         long      >= 32          java.lang.Byte or java.lang.Short or java.lang.Integer or java.lang.Long or java.math.BigInteger
    MARPAESLIF_VALUE_TYPE_FLOAT        float     depends        java.lang.Float (includes +/-Inf and Nan) or java.lang.Double or java.math.BigDecimal
    MARPAESLIF_VALUE_TYPE_DOUBLE       double    depends        java.lang.Float or java.lang.Double or java.math.BigDecimal
    MARPAESLIF_VALUE_TYPE_PTR          char*                    java.lang.Object or java.lang.Long
    MARPAESLIF_VALUE_TYPE_ARRAY                                 byte[]
    MARPAESLIF_VALUE_TYPE_BOOL                                  java.lang.Bool
    MARPAESLIF_VALUE_TYPE_STRING                                java.lang.String
    MARPAESLIF_VALUE_TYPE_ROW                                   object[]
    MARPAESLIF_VALUE_TYPE_TABLE                                 java.util.HashMap
    MARPAESLIF_VALUE_TYPE_LONG_DOUBLE                           java.lang.Float or java.lang.Double or java.math.BigDecimal
    MARPAESLIF_VALUE_TYPE_LONG_LONG    long long >= 64          java.lang.Byte or java.lang.Short or java.lang.Integer or java.lang.Long or java.math.BigInteger
    
  */

  switch (marpaESLIFValueResultp->type) {
  case MARPAESLIF_VALUE_TYPE_UNDEF:
    /* fprintf(stderr, "==> %s: import null\n", funcs); fflush(stdout); fflush(stderr); */
    objectp = NULL;
    GENERICSTACK_PUSH_PTR(objectStackp, objectp);
    if (GENERICSTACK_ERROR(objectStackp)) {
      RAISEEXCEPTIONF(envp, "objectStackp push failure, %s", strerror(errno));
    }
    break;
  case MARPAESLIF_VALUE_TYPE_CHAR:
    /* fprintf(stderr, "==> %s: import CHAR 0x%lx\n", funcs, (unsigned long) marpaESLIFValueResultp->u.c); fflush(stdout); fflush(stderr); */
    MARPAESLIFJAVA_IMPORT_NUMBER_NOT_DECIMAL(envp, objectp, "%c", char, marpaESLIFValueResultp->u.c);
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
  case MARPAESLIF_VALUE_TYPE_SHORT:
    /* fprintf(stderr, "==> %s: import SHORT\n", funcs); fflush(stdout); fflush(stderr); */
    MARPAESLIFJAVA_IMPORT_NUMBER_NOT_DECIMAL(envp, objectp, "%d", int, marpaESLIFValueResultp->u.b);
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
  case MARPAESLIF_VALUE_TYPE_INT:
    /* fprintf(stderr, "==> %s: import INT\n", funcs); fflush(stdout); fflush(stderr); */
    MARPAESLIFJAVA_IMPORT_NUMBER_NOT_DECIMAL(envp, objectp, "%d", int, marpaESLIFValueResultp->u.i);
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
  case MARPAESLIF_VALUE_TYPE_LONG:
    /* fprintf(stderr, "==> %s: import LONG\n", funcs); fflush(stdout); fflush(stderr); */
    MARPAESLIFJAVA_IMPORT_NUMBER_NOT_DECIMAL(envp, objectp, "%ld", long, marpaESLIFValueResultp->u.l);
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
  case MARPAESLIF_VALUE_TYPE_FLOAT:
    /* If value is +/-Infinity or NaN this will be handle natively via jfloat */
    /* fprintf(stderr, "==> %s: MARPAESLIF_VALUE_TYPE_FLOAT: %f\n", funcs, (double) marpaESLIFValueResultp->u.f); fflush(stdout); fflush(stderr); */
    if (marpaESLIFValueResult_isinfb(marpaESLIFValueResultp)) {
      if (marpaESLIFValueResultp->u.f >= 0) {
        MARPAESLIFJAVA_IMPORT_POSITIVE_INFINITY(marpaESLIFp, envp, objectp);
      } else {
        MARPAESLIFJAVA_IMPORT_NEGATIVE_INFINITY(marpaESLIFp, envp, objectp);
      }
    } else if (marpaESLIFValueResult_isnanb(marpaESLIFValueResultp)) {
      MARPAESLIFJAVA_IMPORT_NAN(marpaESLIFp, envp, objectp);
    } else {
      MARPAESLIFJAVA_IMPORT_NUMBER_DECIMAL(marpaESLIFp, envp, objectp, double, marpaESLIFValueResultp->u.f, marpaESLIF_ftos);
    }
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
  case MARPAESLIF_VALUE_TYPE_DOUBLE:
    /* If value is +/-Infinity or NaN this will be handle natively via jdouble */
    /* fprintf(stderr, "==> %s: MARPAESLIF_VALUE_TYPE_DOUBLE: %f\n", funcs, (double) marpaESLIFValueResultp->u.d); fflush(stdout); fflush(stderr); */
    if (marpaESLIFValueResult_isinfb(marpaESLIFValueResultp)) {
      if (marpaESLIFValueResultp->u.d >= 0) {
        MARPAESLIFJAVA_IMPORT_POSITIVE_INFINITY(marpaESLIFp, envp, objectp);
      } else {
        MARPAESLIFJAVA_IMPORT_NEGATIVE_INFINITY(marpaESLIFp, envp, objectp);
      }
    } else if (marpaESLIFValueResult_isnanb(marpaESLIFValueResultp)) {
      MARPAESLIFJAVA_IMPORT_NAN(marpaESLIFp, envp, objectp);
    } else {
      MARPAESLIFJAVA_IMPORT_NUMBER_DECIMAL(marpaESLIFp, envp, objectp, double, marpaESLIFValueResultp->u.d, marpaESLIF_dtos);
    }
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
  case MARPAESLIF_VALUE_TYPE_PTR:
    /* fprintf(stderr, "==> %s: import PTR\n", funcs); fflush(stdout); fflush(stderr); */
    if (marpaESLIFValueResultp->contextp == MARPAESLIF_JNI_CONTEXT) {
      /* This is an object that we pushed */
      /* fprintf(stderr, "==> %s: import Object '%p'\n", funcs, (void *) marpaESLIFValueResultp->u.p.p); fflush(stdout); fflush(stderr); */
      objectp = (*envp)->NewLocalRef(envp, (jobject) marpaESLIFValueResultp->u.p.p);
    } else {
      /* This is a pointer coming from another source - we express that as a jlong */
      /* fprintf(stderr, "==> %s: import ptr as Long '%Ld'\n", funcs, (long) marpaESLIFValueResultp->u.p.p); fflush(stdout); fflush(stderr); */
      if (JAVA_LANG_LONG_CLASS_valueOf_METHODP != NULL) {
        objectp = (*envp)->CallStaticObjectMethod(envp, JAVA_LANG_LONG_CLASSP, JAVA_LANG_LONG_CLASS_valueOf_METHODP, PTR_TO_JLONG(marpaESLIFValueResultp->u.p.p));
      } else {
        objectp = (*envp)->NewObject(envp, JAVA_LANG_LONG_CLASSP, JAVA_LANG_LONG_CLASS_init_METHODP, PTR_TO_JLONG(marpaESLIFValueResultp->u.p.p));
      }
    }
    if (objectp == NULL) {
      /* An exception was raised by the JVM */
      goto err;
    }
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
  case MARPAESLIF_VALUE_TYPE_ARRAY:
    /* fprintf(stderr, "==> %s: import ARRAY\n", funcs); fflush(stdout); fflush(stderr); */
    byteArrayp = (*envp)->NewByteArray(envp, (jsize) marpaESLIFValueResultp->u.a.sizel);
    if (byteArrayp == NULL) {
      goto err;
    }
    if (marpaESLIFValueResultp->u.a.sizel > 0) {
      /* Don't know if it is legal to call SetByteArrayRegion with a zero size -; */
      (*envp)->SetByteArrayRegion(envp, byteArrayp, 0, (jsize) marpaESLIFValueResultp->u.a.sizel, (jbyte *) marpaESLIFValueResultp->u.a.p);
      if (HAVEEXCEPTION(envp)) {
        goto err;
      }
    }
    objectp = (jobject) byteArrayp;
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    byteArrayp = NULL; /* byteArrayp is in marpaESLIFValueContextp->objectp */
    break;
  case MARPAESLIF_VALUE_TYPE_BOOL:
    /* fprintf(stderr, "==> %s: import BOOL\n", funcs); fflush(stdout); fflush(stderr); */
    jb = (marpaESLIFValueResultp->u.y == MARPAESLIFVALUERESULTBOOL_FALSE) ? JNI_FALSE : JNI_TRUE;
    if (JAVA_LANG_BOOLEAN_CLASS_valueOf_METHODP != NULL) {
      objectp = (*envp)->CallStaticObjectMethod(envp, JAVA_LANG_BOOLEAN_CLASSP, JAVA_LANG_BOOLEAN_CLASS_valueOf_METHODP, jb);
    } else {
      objectp = (*envp)->NewObject(envp, JAVA_LANG_BOOLEAN_CLASSP, JAVA_LANG_BOOLEAN_CLASS_init_METHODP, jb);
    }
    if (objectp == NULL) {
      /* An exception was raised by the JVM */
      goto err;
    }
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
  case MARPAESLIF_VALUE_TYPE_STRING:
    /* fprintf(stderr, "==> %s: import STRING\n", funcs); fflush(stdout); fflush(stderr); */
    encodingp = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, marpaESLIFValueResultp->u.s.encodingasciis);
    if (encodingp == NULL) {
      /* We want OUR exception to be raised */
      RAISEEXCEPTION(envp, "marpaESLIFJava_marpaESLIFASCIIToJavap failure");
    }
    byteArrayp = (*envp)->NewByteArray(envp, (jsize) marpaESLIFValueResultp->u.s.sizel);
    if (byteArrayp == NULL) {
      goto err;
    }
    if ((marpaESLIFValueResultp->u.s.p != NULL) && (marpaESLIFValueResultp->u.s.sizel > 0)) {
      (*envp)->SetByteArrayRegion(envp, byteArrayp, (jsize) 0, (jsize) marpaESLIFValueResultp->u.s.sizel, (jbyte *) marpaESLIFValueResultp->u.s.p);
      if (HAVEEXCEPTION(envp)) {
        goto err;
      }
    }
    objectp = (*envp)->NewObject(envp, JAVA_LANG_STRING_CLASSP, JAVA_LANG_STRING_CLASS_init_byteArray_String_METHODP, byteArrayp, encodingp);
    if (objectp == NULL) {
      goto err;
    }
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
  case MARPAESLIF_VALUE_TYPE_ROW:
    /* fprintf(stderr, "==> %s: import ROW\n", funcs); fflush(stdout); fflush(stderr); */
    /* We received elements in order: first, second, etc..., we pushed that in valueStack, so pop will say last, beforelast, etc..., second, first */
    objectArrayp = (*envp)->NewObjectArray(envp, (jsize) marpaESLIFValueResultp->u.r.sizel, JAVA_LANG_OBJECT_CLASSP, NULL /* initialElement */);
    if (objectArrayp == NULL) {
      RAISEEXCEPTION(envp, "NewObjectArray failure");
    }
    for (i = 0; i < marpaESLIFValueResultp->u.r.sizel; i++) {
      objectp = (jobject) GENERICSTACK_POP_PTR(objectStackp);
      (*envp)->SetObjectArrayElement(envp, objectArrayp, (jsize) i, objectp);
      if (HAVEEXCEPTION(envp)) {
        goto err;
      }
    }
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectArrayp);
    break;
  case MARPAESLIF_VALUE_TYPE_TABLE:
    /* fprintf(stderr, "==> %s: import TABLE\n", funcs); fflush(stdout); fflush(stderr); */
    objectHashMapp = (*envp)->NewObject(envp, JAVA_UTIL_HASHMAP_CLASSP, JAVA_UTIL_HASHMAP_CLASS_init_METHODP);
    if (objectHashMapp == NULL) {
      RAISEEXCEPTION(envp, "NewObject failure");
    }
    /* We received elements in order: firstkey, firstvalue, secondkey, secondvalue, etc..., we pushed that in valueStack, so pop will say lastvalue, lastkey, ..., firstvalue, firstkey */
    for (i = 0; i < marpaESLIFValueResultp->u.t.sizel; i++) {
      valuep = (jobject) GENERICSTACK_POP_PTR(objectStackp);
      keyp = (jobject) GENERICSTACK_POP_PTR(objectStackp);
      (*envp)->CallObjectMethod(envp, objectHashMapp, JAVA_UTIL_HASHMAP_CLASS_put_METHODP, keyp, valuep);
      if (HAVEEXCEPTION(envp)) {
        goto err;
      }
    }
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectHashMapp);
    break;
  case MARPAESLIF_VALUE_TYPE_LONG_DOUBLE:
    if (marpaESLIFValueResult_isinfb(marpaESLIFValueResultp)) {
      if (marpaESLIFValueResultp->u.ld >= 0) {
        MARPAESLIFJAVA_IMPORT_POSITIVE_INFINITY(marpaESLIFp, envp, objectp);
      } else {
        MARPAESLIFJAVA_IMPORT_NEGATIVE_INFINITY(marpaESLIFp, envp, objectp);
      }
    } else if (marpaESLIFValueResult_isnanb(marpaESLIFValueResultp)) {
      MARPAESLIFJAVA_IMPORT_NAN(marpaESLIFp, envp, objectp);
    } else {
      MARPAESLIFJAVA_IMPORT_NUMBER_DECIMAL(marpaESLIFp, envp, objectp, long double, marpaESLIFValueResultp->u.ld, marpaESLIF_ldtos);
    }
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
#ifdef MARPAESLIF_HAVE_LONG_LONG
  case MARPAESLIF_VALUE_TYPE_LONG_LONG:
    /* fprintf(stderr, "==> %s: import LONG_LONG\n", funcs); fflush(stdout); fflush(stderr); */
    MARPAESLIFJAVA_IMPORT_NUMBER_NOT_DECIMAL(envp, objectp, MARPAESLIF_LONG_LONG_FMT, MARPAESLIF_LONG_LONG, marpaESLIFValueResultp->u.ll);
    MARPAESLIFJAVA_PUSH_PTR(objectStackp, objectp);
    break;
#endif
  default:
    break;
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  if (envp != NULL) {
    if (byteArrayp != NULL) {
      (*envp)->DeleteLocalRef(envp, byteArrayp);
    }
    if (encodingp != NULL) {
      (*envp)->DeleteLocalRef(envp, encodingp);
    }
  }
  return rcb;
}

/*****************************************************************************/
static jstring marpaESLIFJava_marpaESLIFStringToJavap(JNIEnv *envp, marpaESLIFString_t *marpaESLIFStringp)
/*****************************************************************************/
{
  static const char *funcs          = "marpaESLIFJava_marpaESLIFStringToJavap";
  jstring            stringp        = NULL;
  jstring            encodingasciip = NULL;
  jbyteArray         byteArrayp     = NULL;

  if (marpaESLIFStringp != NULL) {
    if (marpaESLIFStringp->bytep != NULL) {
      /* Note: ok if bytel is zero, then it is an empty string */
      byteArrayp = (*envp)->NewByteArray(envp, (jsize) marpaESLIFStringp->bytel);
      if (byteArrayp == NULL) {
        goto err;
      }
      if (marpaESLIFStringp->bytel > 0) {
        (*envp)->SetByteArrayRegion(envp, byteArrayp, (jsize) 0, (jsize) marpaESLIFStringp->bytel, (jbyte *) marpaESLIFStringp->bytep);
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }
      }
      if (marpaESLIFStringp->encodingasciis != NULL) {
        /* As per IANA this is in ASCII, thus compatible with Java's modified UTF-8 */
        encodingasciip = (*envp)->NewStringUTF(envp, (const char *) marpaESLIFStringp->encodingasciis);
        if (encodingasciip == NULL) {
          /* We want OUR exception to be raised */
          RAISEEXCEPTIONF(envp, "NewStringUTF(\"%s\") failure", marpaESLIFStringp->encodingasciis);
        }
        stringp = (*envp)->NewObject(envp, JAVA_LANG_STRING_CLASSP, JAVA_LANG_STRING_CLASS_init_byteArray_String_METHODP, byteArrayp, encodingasciip);
      } else {
        stringp = (*envp)->NewObject(envp, JAVA_LANG_STRING_CLASSP, JAVA_LANG_STRING_CLASS_init_byteArray_METHODP, byteArrayp);
      }
      if (stringp == NULL) {
        RAISEEXCEPTION(envp, "NewObject failure");
      }
    }
  }

  goto done;

 err:
  if (envp != NULL) {
    if (byteArrayp != NULL) {
      (*envp)->DeleteLocalRef(envp, byteArrayp);
    }
    if (encodingasciip != NULL) {
      (*envp)->DeleteLocalRef(envp, encodingasciip);
    }
    if (stringp != NULL) {
      (*envp)->DeleteLocalRef(envp, stringp);
    }
  }
  stringp = NULL;

 done:
  return stringp;
}

/*****************************************************************************/
static jstring marpaESLIFJava_marpaESLIFASCIIToJavap(JNIEnv *envp, char *asciis)
/*****************************************************************************/
{
  static const char *funcs = "marpaESLIFJava_marpaESLIFASCIIToJavap";
  jstring            stringp;

  if (asciis != NULL) {
    /* Per definition an ASCII string is compatible with modified UTF-8 */
    stringp = (*envp)->NewStringUTF(envp, asciis);
    if (stringp == NULL) {
      RAISEEXCEPTION(envp, "NewStringUTF(\"UTF-8\") failure");
    }
  } else {
    stringp = NULL;
  }

  goto done;

 err:
  stringp = NULL;

 done:
  return stringp;
}

/*****************************************************************************/
static jstring marpaESLIFJava_marpaESLIFActionToJavap(JNIEnv *envp, marpaESLIFAction_t *actionp)
/*****************************************************************************/
{
  static const char *funcs = "marpaESLIFJava_marpaESLIFActionToJavap";
  jstring            stringp;

  if (actionp == NULL) {
    stringp = NULL;
  } else {
    switch (actionp->type) {
    case MARPAESLIF_ACTION_TYPE_NAME:
      stringp = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, actionp->u.names);
      break;
    case MARPAESLIF_ACTION_TYPE_STRING:
      stringp = marpaESLIFJava_marpaESLIFStringToJavap(envp, actionp->u.stringp);
      break;
    case MARPAESLIF_ACTION_TYPE_LUA:
      stringp = marpaESLIFJava_marpaESLIFASCIIToJavap(envp, actionp->u.luas);
      break;
    default:
      RAISEEXCEPTIONF(envp, "Unsuported action type %d", actionp->type);
    }
  }

  goto done;

 err:
  stringp = NULL;

 done:
  return stringp;
}

/*****************************************************************************/
static short marpaESLIFJava_stack_setb(JNIEnv *envp, marpaESLIFValue_t *marpaESLIFValuep, short resulti, jobject objectp, marpaESLIFValueResult_t *marpaESLIFValueResultOutputp)
/*****************************************************************************/
/* Take care: IF resulti is >= 0, then marpaESLIFValuep must be != NULL      */
/*****************************************************************************/
{
  static const char       *funcs = "marpaESLIFJava_stack_setb";
  marpaESLIF_t            *marpaESLIFp = marpaESLIFGrammar_eslifp(marpaESLIFRecognizer_grammarp(marpaESLIFValue_recognizerp(marpaESLIFValuep)));
  genericStack_t           marpaESLIFValueResultStack;
  genericStack_t          *marpaESLIFValueResultStackp = NULL;
  short                    rcb;
  genericStack_t           objectStack;
  genericStack_t          *objectStackp = NULL;
  marpaESLIFValueResult_t  marpaESLIFValueResult;
  marpaESLIFValueResult_t  marpaESLIFValueResultTmp;
  marpaESLIFValueResult_t *marpaESLIFValueResultp;
  jclass                   classp;
  jclass                   componentclassp;
  jbyte                    java_byte;
  jshort                   java_short;
  jint                     java_int;
  jlong                    java_long;
  jboolean                 java_boolean;
  jfloat                   java_float;
  jdouble                  java_double;
  short                    eslifb;
  jobject                  globalObjectp;
  jbyte                   *bytep;
  jsize                    bytel;
  jsize                    arrayl;
  jsize                    i;
  jobject                  componentObjectp;
  jobject                  entrySetp;
  jobject                  iteratorp;
  jboolean                 hasNextb;
  jobject                  nextp;
  jobject                  keyp;
  jobject                  valuep;
  jint                     sizei;
  jmethodID                sizeMethodp;
  jmethodID                getMethodp;
  jmethodID                entrySetMethodp;
  jmethodID                getKeyMethodp;
  jmethodID                getValueMethodp;
  jclass                   nextclassp;
  marpaESLIFRepresentationDispose_t disposeCallbackp;

  /*
    Java Type                        marpaESLIFType

    N/A                              N/A
    java.lang.Byte                   CHAR or SHORT or INT or LONG or LONG_LONG or opaque
    java.lang.Short                  CHAR or SHORT or INT or LONG or LONG_LONG or opaque
    java.lang.Integer                CHAR or SHORT or INT or LONG or LONG_LONG or opaque
    java.lang.Long                   CHAR or SHORT or INT or LONG or LONG_LONG or opaque
    java.lang.Float                  FLOAT or DOUBLE or LONG_DOUBLE or opaque
    java.lang.Double                 FLOAT or DOUBLE or LONG_DOUBLE or opaque
    byte[]                           ARRAY
    java.lang.Boolean                BOOL
    java.lang.Character              STRING
    java.lang.String                 STRING
    object[]                         ROW
    java.util.HashMap                TABLE
    
  */

  /* We maintain in parallel a marpaESLIFValueResult and a jobject stacks */
  marpaESLIFValueResultStackp = &marpaESLIFValueResultStack;
  GENERICSTACK_INIT(marpaESLIFValueResultStackp);
  if (GENERICSTACK_ERROR(marpaESLIFValueResultStackp)) {
    marpaESLIFValueResultStackp = NULL;
    RAISEEXCEPTIONF(envp, "marpaESLIFValueResultStackp init failure, %s", strerror(errno));
  }

  objectStackp = &objectStack;
  GENERICSTACK_INIT(objectStackp);
  if (GENERICSTACK_ERROR(objectStackp)) {
    objectStackp = NULL;
    RAISEEXCEPTIONF(envp, "objectStackp init failure, %s", strerror(errno));
  }

  GENERICSTACK_PUSH_PTR(marpaESLIFValueResultStackp, &marpaESLIFValueResult);
  if (GENERICSTACK_ERROR(marpaESLIFValueResultStackp)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFValueResultStackp push failure, %s", strerror(errno));
  }

  GENERICSTACK_PUSH_PTR(objectStackp, objectp);
  if (GENERICSTACK_ERROR(objectStackp)) {
    RAISEEXCEPTIONF(envp, "objectStackp push failure, %s", strerror(errno));
  }

  while (GENERICSTACK_USED(marpaESLIFValueResultStackp) > 0) {
    marpaESLIFValueResultp = (marpaESLIFValueResult_t *) GENERICSTACK_POP_PTR(marpaESLIFValueResultStackp);
    objectp = (jobject) GENERICSTACK_POP_PTR(objectStackp);

    eslifb = 0;

    if (objectp == NULL) {
      /* fprintf(stderr, "==> %s: export UNDEF\n", funcs); fflush(stdout); fflush(stderr); */
      marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_UNDEF;
      marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT;
      marpaESLIFValueResultp->representationp = NULL;
      eslifb = 1;
    } else {
      /* Get object class */
      classp = (*envp)->GetObjectClass(envp, objectp);
      if (classp == NULL) {
        /* An exception was (must have been) raised */
        RAISEEXCEPTION(envp, "GetObjectClass failure");
      }
      /* Because of auto-boxing, direct type is always an Object (we require the Object type in the output of all actions) */
      if ((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_BYTE_CLASSP) == JNI_TRUE) {
        java_byte = (*envp)->CallByteMethod(envp, objectp, JAVA_LANG_BYTE_CLASS_byteValue_METHODP);
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }
        MARPAESLIFJAVA_EXPORT_NUMBER_NOT_DECIMAL(envp, jbyte, java_byte, eslifb);
      } else if ((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_SHORT_CLASSP) == JNI_TRUE) {
        java_short = (*envp)->CallShortMethod(envp, objectp, JAVA_LANG_SHORT_CLASS_shortValue_METHODP);
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }
        MARPAESLIFJAVA_EXPORT_NUMBER_NOT_DECIMAL(envp, jshort, java_short, eslifb);
      } else if ((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_INTEGER_CLASSP) == JNI_TRUE) {
        java_int = (*envp)->CallIntMethod(envp, objectp, JAVA_LANG_INTEGER_CLASS_intValue_METHODP);
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }
        MARPAESLIFJAVA_EXPORT_NUMBER_NOT_DECIMAL(envp, jint, java_int, eslifb);
      } else if ((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_LONG_CLASSP) == JNI_TRUE) {
        java_long = (*envp)->CallLongMethod(envp, objectp, JAVA_LANG_LONG_CLASS_longValue_METHODP);
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }
        MARPAESLIFJAVA_EXPORT_NUMBER_NOT_DECIMAL(envp, jlong, java_long, eslifb);
      } else if ((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_FLOAT_CLASSP) == JNI_TRUE) {
        java_float = (*envp)->CallFloatMethod(envp, objectp, JAVA_LANG_FLOAT_CLASS_floatValue_METHODP);
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }
        MARPAESLIFJAVA_EXPORT_NUMBER_DECIMAL(envp, jfloat, java_float, eslifb);
      } else if ((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_DOUBLE_CLASSP) == JNI_TRUE) {
        java_double = (*envp)->CallDoubleMethod(envp, objectp, JAVA_LANG_DOUBLE_CLASS_doubleValue_METHODP);
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }
        MARPAESLIFJAVA_EXPORT_NUMBER_DECIMAL(envp, jdouble, java_double, eslifb);
      } else if ((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_BOOLEAN_CLASSP) == JNI_TRUE) {
        java_boolean = (*envp)->CallBooleanMethod(envp, objectp, JAVA_LANG_BOOLEAN_CLASS_booleanValue_METHODP);
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }

        /* fprintf(stderr, "==> %s: export BOOL '%d'\n", funcs, (int) java_boolean); fflush(stdout); fflush(stderr); */
        marpaESLIFValueResultp->type            = MARPAESLIF_VALUE_TYPE_BOOL;
        marpaESLIFValueResultp->contextp        = MARPAESLIF_JNI_CONTEXT;
        marpaESLIFValueResultp->representationp = NULL;
        marpaESLIFValueResultp->u.y             = (java_boolean == JNI_FALSE) ? MARPAESLIFVALUERESULTBOOL_FALSE : MARPAESLIFVALUERESULTBOOL_TRUE;
        eslifb = 1;

      } else if (((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_CHARACTER_CLASSP) == JNI_TRUE) ||
                 ((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_STRING_CLASSP) == JNI_TRUE)) {
        /* Fake a PTR and make that an UTF-8 string */
        marpaESLIFValueResultTmp.type                 = MARPAESLIF_VALUE_TYPE_PTR;
        marpaESLIFValueResultTmp.contextp             = MARPAESLIF_JNI_CONTEXT;
        marpaESLIFValueResultTmp.u.p.p                = (void *) objectp;
        marpaESLIFValueResultTmp.u.p.freeUserDatavp   = NULL;
        marpaESLIFValueResultTmp.u.p.freeCallbackp    = NULL;
        marpaESLIFValueResultTmp.u.p.shallowb         = 1;
        if (! marpaESLIFJava_representationCallbackb(NULL /* marpaESLIFValueContextp */,
                                                     &marpaESLIFValueResultTmp,
                                                     (char **) &(marpaESLIFValueResultp->u.s.p),
                                                     &(marpaESLIFValueResultp->u.s.sizel),
                                                     &(marpaESLIFValueResultp->u.s.encodingasciis),
						     &disposeCallbackp)) {
          goto err;
        }
	/* Note that disposeCallbackp value is ignored because we push the result in marpaESLIFValueResultp that has */
	/* a freeCallbackp that is equivalent to our internal dispose callback for representation */

        /* fprintf(stderr, "==> %s: export STRING '%s' encoding '%s'\n", funcs, marpaESLIFValueResultp->u.s.p, marpaESLIFValueResultp->u.s.encodingasciis); fflush(stdout); fflush(stderr); */
        marpaESLIFValueResultp->type               = MARPAESLIF_VALUE_TYPE_STRING;
        marpaESLIFValueResultp->contextp           = MARPAESLIF_JNI_CONTEXT;
        marpaESLIFValueResultp->representationp    = NULL;
        marpaESLIFValueResultp->u.s.shallowb       = 0;
        marpaESLIFValueResultp->u.s.freeUserDatavp = NULL; /* We will resolve JNIEnv using jvm that is a safe static */
        marpaESLIFValueResultp->u.s.freeCallbackp  = marpaESLIFJava_genericFreeCallbackv;
        eslifb = 1;

      } else if ((*envp)->CallBooleanMethod(envp, classp, JAVA_LANG_CLASS_CLASS_isArray_METHODP) == JNI_TRUE) {
        /* Get component class: this CAN be a true primitive and not an object */
        componentclassp = (*envp)->CallObjectMethod(envp, classp, JAVA_LANG_CLASS_CLASS_getComponentType_METHODP);
        if (componentclassp == NULL) {
          /* An exception was (must have been) raised */
          RAISEEXCEPTION(envp, "getComponentType failure");
        }
        if (((*envp)->CallBooleanMethod(envp, componentclassp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_BYTE_CLASSP) == JNI_TRUE) ||
            ((*envp)->CallBooleanMethod(envp, componentclassp, JAVA_LANG_CLASS_CLASS_equals_METHODP, byteTypeClassp) == JNI_TRUE)) {
          /* This is a byte[] array */
          /* fprintf(stderr, "==> %s: export ARRAY\n", funcs); fflush(stdout); fflush(stderr); */
          marpaESLIFValueResultp->type               = MARPAESLIF_VALUE_TYPE_ARRAY;
          marpaESLIFValueResultp->contextp           = MARPAESLIF_JNI_CONTEXT;
          marpaESLIFValueResultp->representationp    = NULL;

          bytel = (*envp)->GetArrayLength(envp, objectp);

          marpaESLIFValueResultp->u.a.sizel    = (size_t) bytel;
          if (bytel > 0) {
            if (CHAR_BIT == 8) { /* a char is the same as jbyte */
              marpaESLIFValueResultp->u.a.p = (char *) malloc(bytel);
              if (marpaESLIFValueResultp->u.a.p == NULL) {
                RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
              }
              (*envp)->GetByteArrayRegion(envp, objectp, 0, bytel, marpaESLIFValueResultp->u.a.p);
            } else { /* a char is not 8 bits on this system */
              bytep = (jbyte *) malloc(bytel * sizeof(jbyte));
              if (bytep == NULL) {
                RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
              }
              (*envp)->GetByteArrayRegion(envp, objectp, 0, bytel, bytep);
              marpaESLIFValueResultp->u.a.p = (char *) malloc(bytel);
              for (i = 0; i < bytel; i++) {
                marpaESLIFValueResultp->u.a.p[i] = (char) bytep[i];
              }
              free(bytep);
            }
            marpaESLIFValueResultp->u.a.shallowb       = 0;
            marpaESLIFValueResultp->u.a.freeUserDatavp = NULL; /* We will resolve JNIEnv using jvm that is a safe static */
            marpaESLIFValueResultp->u.a.freeCallbackp  = marpaESLIFJava_genericFreeCallbackv;
          } else {
            marpaESLIFValueResultp->u.a.p = NULL;
            marpaESLIFValueResultp->u.a.shallowb       = 1;
            marpaESLIFValueResultp->u.a.freeUserDatavp = NULL; /* We will resolve JNIEnv using jvm that is a safe static */
            marpaESLIFValueResultp->u.a.freeCallbackp  = NULL;
          }
          eslifb = 1;
        } else { /* if ((*envp)->CallBooleanMethod(envp, componentclassp, JAVA_LANG_CLASS_CLASS_equals_METHODP, JAVA_LANG_OBJECT_CLASSP) == JNI_TRUE) { */
          /* fprintf(stderr, "==> %s: export ROW\n", funcs); fflush(stdout); fflush(stderr); */
          arrayl = (*envp)->GetArrayLength(envp, objectp);
          /* This is an object[] array */
          marpaESLIFValueResultp->type               = MARPAESLIF_VALUE_TYPE_ROW;
          marpaESLIFValueResultp->contextp           = MARPAESLIF_JNI_CONTEXT;
          marpaESLIFValueResultp->representationp    = NULL;
          marpaESLIFValueResultp->u.r.sizel          = (size_t) arrayl;
          marpaESLIFValueResultp->u.r.shallowb       = 0;
          marpaESLIFValueResultp->u.r.freeUserDatavp = NULL; /* We will resolve JNIEnv using jvm that is a safe static */
          marpaESLIFValueResultp->u.r.freeCallbackp  = marpaESLIFJava_genericFreeCallbackv;

          if (arrayl > 0) {
            marpaESLIFValueResultp->u.r.p = (marpaESLIFValueResult_t *) malloc(arrayl * sizeof(marpaESLIFValueResult_t));
            if (marpaESLIFValueResultp->u.r.p == NULL) {
              RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
            }
            for (i = 0; i < arrayl; i++) {
              componentObjectp = (*envp)->GetObjectArrayElement(envp, (jobjectArray) objectp, i);
              if (HAVEEXCEPTION(envp)) {
                RAISEEXCEPTION(envp, " GetObjectArrayElement failure");
              }
              GENERICSTACK_PUSH_PTR(objectStackp, componentObjectp);
              if (GENERICSTACK_ERROR(objectStackp)) {
                RAISEEXCEPTIONF(envp, "objectStackp push failure, %s", strerror(errno));
              }
              GENERICSTACK_PUSH_PTR(marpaESLIFValueResultStackp, (void *) &(marpaESLIFValueResultp->u.r.p[i]));
              if (GENERICSTACK_ERROR(marpaESLIFValueResultStackp)) {
                RAISEEXCEPTIONF(envp, "marpaESLIFValueResultStackp push failure, %s", strerror(errno));
              }
            }
          } else {
            marpaESLIFValueResultp->u.r.p = NULL;
          }
          eslifb = 1;
        }
      } else if ((*envp)->IsInstanceOf(envp, objectp, JAVA_UTIL_MAP_CLASSP) == JNI_TRUE) {

        /* fprintf(stderr, "==> %s: export TABLE\n", funcs); fflush(stdout); fflush(stderr); */

	sizeMethodp = (*envp)->GetMethodID(envp, classp, "size", "()I");
	if (sizeMethodp == NULL) {
          /* We want OUR exception to be raised */
          (*envp)->ExceptionClear(envp);
          RAISEEXCEPTION(envp, "Failed to find method \"size\" with signature \"()I\" in class being an instance of java/util/Map");
	}

	getMethodp = (*envp)->GetMethodID(envp, classp, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
	if (getMethodp == NULL) {
          /* We want OUR exception to be raised */
          (*envp)->ExceptionClear(envp);
          RAISEEXCEPTION(envp, "Failed to find method \"get\" with signature \"(Ljava/lang/Object;)Ljava/lang/Object;\" in class being an instance of java/util/Map");
	}

	entrySetMethodp = (*envp)->GetMethodID(envp, classp, "entrySet", "()Ljava/util/Set;");
	if (entrySetMethodp == NULL) {
          /* We want OUR exception to be raised */
          (*envp)->ExceptionClear(envp);
          RAISEEXCEPTION(envp, "Failed to find method \"entrySet\" with signature \"()Ljava/util/Set;\" in class being an instance of java/util/Map");
	}

        sizei = (*envp)->CallIntMethod(envp, objectp, sizeMethodp);
        /* fprintf(stderr, "==> %s: export TABLE: size=%ld\n", funcs, (unsigned long) sizei); fflush(stdout); fflush(stderr); */

        marpaESLIFValueResultp->type               = MARPAESLIF_VALUE_TYPE_TABLE;
        marpaESLIFValueResultp->contextp           = MARPAESLIF_JNI_CONTEXT;
        marpaESLIFValueResultp->representationp    = NULL;
        marpaESLIFValueResultp->u.t.sizel          = (size_t) sizei;
        marpaESLIFValueResultp->u.t.shallowb       = 0;
        marpaESLIFValueResultp->u.t.freeUserDatavp = NULL; /* We will resolve JNIEnv using jvm that is a safe static */
        marpaESLIFValueResultp->u.t.freeCallbackp  = marpaESLIFJava_genericFreeCallbackv;

        if (marpaESLIFValueResultp->u.t.sizel > 0) {
          marpaESLIFValueResultp->u.t.p = (marpaESLIFValueResultPair_t *) malloc(sizei * sizeof(marpaESLIFValueResultPair_t));
          if (marpaESLIFValueResultp->u.t.p == NULL) {
            RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
          }

          /* fprintf(stderr, "==> %s: export TABLE: entrySet\n", funcs); fflush(stdout); fflush(stderr); */
          entrySetp = (*envp)->CallObjectMethod(envp, objectp, entrySetMethodp);
          if (entrySetp == NULL) {
            /* An exception was (must have been) raised */
            RAISEEXCEPTION(envp, "entrySet failure");
          }

          /* fprintf(stderr, "==> %s: export TABLE: iterator\n", funcs); fflush(stdout); fflush(stderr); */
          iteratorp = (*envp)->CallObjectMethod(envp, entrySetp, JAVA_UTIL_SET_CLASS_iterator_METHODP);
          if (iteratorp == NULL) {
            /* An exception was (must have been) raised */
            RAISEEXCEPTION(envp, "iterator failure");
          }

          i = 0;
          while (1) {
            /* fprintf(stderr, "==> %s: export TABLE: iterator: hasNext\n", funcs); fflush(stdout); fflush(stderr); */
            hasNextb = (*envp)->CallBooleanMethod(envp, iteratorp, JAVA_UTIL_ITERATOR_CLASS_hasNext_METHODP);
            if (HAVEEXCEPTION(envp)) {
              goto err;
            }
            if (hasNextb != JNI_TRUE) {
              break;
            }

            /* fprintf(stderr, "==> %s: export TABLE: iterator: next\n", funcs); fflush(stdout); fflush(stderr); */
            nextp = (*envp)->CallObjectMethod(envp, iteratorp, JAVA_UTIL_ITERATOR_CLASS_next_METHODP);
            if (nextp == NULL) {
              goto err;
            }

	    nextclassp = (*envp)->GetObjectClass(envp, nextp);
	    if (nextclassp == NULL) {
	      /* An exception was (must have been) raised */
	      RAISEEXCEPTION(envp, "GetObjectClass failure");
	    }

	    getKeyMethodp = (*envp)->GetMethodID(envp, nextclassp, "getKey", "()Ljava/lang/Object;");
	    if (getKeyMethodp == NULL) {
	      /* We want OUR exception to be raised */
	      (*envp)->ExceptionClear(envp);
	      RAISEEXCEPTION(envp, "Failed to find method \"getKey\" with signature \"()Ljava/lang/Object;\"");
	    }

            /* fprintf(stderr, "==> %s: export TABLE: iterator: next: getKey\n", funcs); fflush(stdout); fflush(stderr); */
	    keyp = (*envp)->CallObjectMethod(envp, nextp, getKeyMethodp);
            if (HAVEEXCEPTION(envp)) {
              goto err;
            }
            GENERICSTACK_PUSH_PTR(objectStackp, keyp);
            if (GENERICSTACK_ERROR(objectStackp)) {
              RAISEEXCEPTIONF(envp, "objectStackp push failure, %s", strerror(errno));
            }
            GENERICSTACK_PUSH_PTR(marpaESLIFValueResultStackp, (void *) &(marpaESLIFValueResultp->u.t.p[i].key));
            if (GENERICSTACK_ERROR(marpaESLIFValueResultStackp)) {
              RAISEEXCEPTIONF(envp, "marpaESLIFValueResultStackp push failure, %s", strerror(errno));
            }

	    getValueMethodp = (*envp)->GetMethodID(envp, nextclassp, "getValue", "()Ljava/lang/Object;");
	    if (getValueMethodp == NULL) {
	      /* We want OUR exception to be raised */
	      (*envp)->ExceptionClear(envp);
	      RAISEEXCEPTION(envp, "Failed to find method \"getValue\" with signature \"()Ljava/lang/Object;\"");
	    }

            /* fprintf(stderr, "==> %s: export TABLE: iterator: next: getValue\n", funcs); fflush(stdout); fflush(stderr); */
	    valuep = (*envp)->CallObjectMethod(envp, nextp, getValueMethodp);
            if (HAVEEXCEPTION(envp)) {
              goto err;
            }
            GENERICSTACK_PUSH_PTR(objectStackp, valuep);
            if (GENERICSTACK_ERROR(objectStackp)) {
              RAISEEXCEPTIONF(envp, "objectStackp push failure, %s", strerror(errno));
            }
            GENERICSTACK_PUSH_PTR(marpaESLIFValueResultStackp, (void *) &(marpaESLIFValueResultp->u.t.p[i].value));
            if (GENERICSTACK_ERROR(marpaESLIFValueResultStackp)) {
              RAISEEXCEPTIONF(envp, "marpaESLIFValueResultStackp push failure, %s", strerror(errno));
            }

            i++;
          }
        } else {
          marpaESLIFValueResultp->u.t.p = NULL;
        }
	eslifb = 1;
      }
    }

    if (! eslifb) {
      /* fprintf(stderr, "==> %s: export PTR\n", funcs); fflush(stdout); fflush(stderr); */
      /* Make that a global reference and send a PTR */
      globalObjectp = (*envp)->NewGlobalRef(envp, objectp);
      if (globalObjectp == NULL) {
        goto err;
      }
      marpaESLIFValueResultp->type               = MARPAESLIF_VALUE_TYPE_PTR;
      marpaESLIFValueResultp->contextp           = MARPAESLIF_JNI_CONTEXT;
      marpaESLIFValueResultp->representationp    = marpaESLIFJava_representationCallbackb;
      marpaESLIFValueResultp->u.p.p              = (void *) globalObjectp;
      marpaESLIFValueResultp->u.p.shallowb       = 0;
      marpaESLIFValueResultp->u.p.freeUserDatavp = NULL; /* We will resolve JNIEnv using jvm that is a safe static */
      marpaESLIFValueResultp->u.p.freeCallbackp  = marpaESLIFJava_genericFreeCallbackv;
    }
  }

  if (resulti >= 0) {
    if (! marpaESLIFValue_stack_setb(marpaESLIFValuep, resulti, &marpaESLIFValueResult)) {
      RAISEEXCEPTIONF(envp, "marpaESLIFValue_stack_setb failure, %s", strerror(errno));
    }
  }
  if (marpaESLIFValueResultOutputp != NULL) {
    *marpaESLIFValueResultOutputp = marpaESLIFValueResult;
  }

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  GENERICSTACK_RESET(objectStackp);
  GENERICSTACK_RESET(marpaESLIFValueResultStackp);
  return rcb;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFJSONEncoder_jniNew(JNIEnv *envp, jobject eslifJSONEncoderp, jboolean strict)
/*****************************************************************************/
{
  static const char                *funcs = "Java_org_parser_marpa_ESLIFJSONEncoder_jniNew";
  marpaESLIFGrammar_t              *marpaESLIFJSONEncoderp;
  marpaESLIF_t                     *marpaESLIFp;
  jobject                           BYTEBUFFER(marpaESLIFJSONEncoder);

  /* Always update genericLogger context */
  if (! ESLIFGrammar_contextb(envp, eslifJSONEncoderp, &marpaESLIFp, NULL /* marpaESLIFGrammarpp */)) {
    goto err;
  }

  /* Create C object */
  marpaESLIFJSONEncoderp = marpaESLIFJSON_encode_newp(marpaESLIFp, (strict == JNI_TRUE) ? 1 : 0);
  if (marpaESLIFJSONEncoderp == NULL) {
    RAISEEXCEPTIONF(envp, "marpaESLIFJSON_encode_newp failure, %s", strerror(errno));
  }

  /* Store the object */
  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIFJSONEncoder, marpaESLIFJSONEncoderp);
  (*envp)->CallVoidMethod(envp, eslifJSONEncoderp, MARPAESLIF_ESLIFGRAMMAR_CLASS_setMarpaESLIFGrammarp_METHODP, BYTEBUFFER(marpaESLIFJSONEncoder));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  goto done;

 err:
  Java_org_parser_marpa_ESLIFJSONEncoder_jniFree(envp, eslifJSONEncoderp);

 done:
  return;
}

/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIFJSONEncoder_jniEncode(JNIEnv *envp, jobject eslifJSONEncoderp, jobject objectp)
/*****************************************************************************/
{
  static const char       *funcs = "Java_org_parser_marpa_ESLIFJSONEncoder_jniEncode";
  jstring                  rcp;
  marpaESLIFGrammar_t     *marpaESLIFJSONEncoderp;
  marpaESLIFValueResult_t  marpaESLIFValueResult;
  marpaESLIFValueOption_t  marpaESLIFValueOption;
  marpaESLIFValueContext_t marpaESLIFValueContext;

  if (! ESLIFGrammar_contextb(envp, eslifJSONEncoderp, NULL /* marpaESLIFpp */, &marpaESLIFJSONEncoderp)) {
    goto err;
  }

  /* Value interface is unmanaged: ESLIF does all the job */
  if (! marpaESLIFJava_valueContextInitb(envp, NULL /* eslifValueInterfacep */, eslifJSONEncoderp /* eslifGrammarp */, &marpaESLIFValueContext)) {
    goto err;
  }

  marpaESLIFValueOption.userDatavp = &marpaESLIFValueContext;
  marpaESLIFValueOption.importerp  = marpaESLIFJava_valueImportb;

  /* Create a marpaESLIFValueResult from objectp */
  if (! marpaESLIFJava_stack_setb(envp, NULL /* marpaESLIFValuep */, -1 /* resulti */, objectp, &marpaESLIFValueResult)) {
    goto err;
  }

  if (! marpaESLIFJSON_encodeb(marpaESLIFJSONEncoderp, &marpaESLIFValueResult, &marpaESLIFValueOption)) {
    marpaESLIFJava_valueContextFreev(envp, &marpaESLIFValueContext, 1 /* onStackb */);
    RAISEEXCEPTION(envp, "marpaESLIFJSON_encodeb failure");
  }
  if (GENERICSTACK_USED(marpaESLIFValueContext.objectStackp) != 1) {
    RAISEEXCEPTIONF(envp, "Internal value stack is %d instead of 1", GENERICSTACK_USED(marpaESLIFValueContext.objectStackp));
  }

  rcp = (jstring) GENERICSTACK_POP_PTR(marpaESLIFValueContext.objectStackp); /* It is a string by definition */
  goto done;

 err:
  rcp = NULL;

 done:
  return rcp;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFJSONEncoder_jniFree(JNIEnv *envp, jobject eslifJSONencoderp)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFJSONEncoderp;

  if (ESLIFGrammar_contextb(envp, eslifJSONencoderp, NULL /* marpaESLIFpp */, &marpaESLIFJSONEncoderp)) {
    marpaESLIFGrammar_freev(marpaESLIFJSONEncoderp); /* This is NULL protected */
  }
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFJSONDecoder_jniNew(JNIEnv *envp, jobject eslifJSONDecoderp, jboolean strict)
/*****************************************************************************/
{
  static const char                *funcs = "Java_org_parser_marpa_ESLIFJSONDecoder_jniNew";
  marpaESLIFGrammar_t              *marpaESLIFJSONDecoderp;
  marpaESLIF_t                     *marpaESLIFp;
  jobject                           BYTEBUFFER(marpaESLIFJSONDecoder);

  /* Always update genericLogger context */
  if (! ESLIFGrammar_contextb(envp, eslifJSONDecoderp, &marpaESLIFp, NULL /* marpaESLIFGrammarpp */)) {
    goto err;
  }

  /* Create C object */
  marpaESLIFJSONDecoderp = marpaESLIFJSON_decode_newp(marpaESLIFp, (strict == JNI_TRUE) ? 1 : 0);
  if (marpaESLIFJSONDecoderp == NULL) {
    RAISEEXCEPTIONF(envp, "marpaESLIFJSON_decode_newp failure, %s", strerror(errno));
  }

  /* Store the object */
  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIFJSONDecoder, marpaESLIFJSONDecoderp);
  (*envp)->CallVoidMethod(envp, eslifJSONDecoderp, MARPAESLIF_ESLIFGRAMMAR_CLASS_setMarpaESLIFGrammarp_METHODP, BYTEBUFFER(marpaESLIFJSONDecoder));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  goto done;

 err:
  Java_org_parser_marpa_ESLIFJSONDecoder_jniFree(envp, eslifJSONDecoderp);

 done:
  return;
}

/*****************************************************************************/
JNIEXPORT jobject JNICALL Java_org_parser_marpa_ESLIFJSONDecoder_jniDecode(JNIEnv *envp, jobject eslifJSONDecoderp, jobject eslifRecognizerInterfacep, jobject eslifDecodeOptionp)
/*****************************************************************************/
{
  static const char            *funcs = "Java_org_parser_marpa_ESLIFJSONDecoder_jniDecode";
  jobject                       rcp;
  marpaESLIFGrammar_t          *marpaESLIFJSONDecoderp;
  marpaESLIFValueOption_t       marpaESLIFValueOption;
  marpaESLIFJSONDecodeOption_t  marpaESLIFJSONDecodeOption;
  marpaESLIFRecognizerContext_t marpaESLIFRecognizerContext;
  marpaESLIFRecognizerOption_t  marpaESLIFRecognizerOption;
  marpaESLIFValueContext_t      marpaESLIFValueContext;

  if (! marpaESLIFJava_recognizerContextInitb(envp, eslifRecognizerInterfacep, &marpaESLIFRecognizerContext, 0 /* haveLexemeStackb */)) {
    goto err;
  }

  marpaESLIFJSONDecodeOption.disallowDupkeysb = ((*envp)->CallBooleanMethod(envp, eslifDecodeOptionp, MARPAESLIF_ESLIFJSONDECODEROPTION_CLASS_isDisallowDupkeys_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFJSONDecodeOption.maxDepthl = (size_t) (*envp)->CallLongMethod(envp, eslifDecodeOptionp, MARPAESLIF_ESLIFJSONDECODEROPTION_CLASS_getMaxDepth_METHODP);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFJSONDecodeOption.noReplacementCharacterb = ((*envp)->CallBooleanMethod(envp, eslifDecodeOptionp, MARPAESLIF_ESLIFJSONDECODEROPTION_CLASS_isNoReplacementCharacter_METHODP) == JNI_TRUE);
  marpaESLIFJSONDecodeOption.positiveInfinityActionp = NULL;         /* Because java always supports +Infinity in float */
  marpaESLIFJSONDecodeOption.negativeInfinityActionp = NULL;         /* Because java always supports -Infinity in float */
  marpaESLIFJSONDecodeOption.positiveNanActionp      = NULL;         /* Because java always supports NaN in float */
  marpaESLIFJSONDecodeOption.negativeNanActionp      = NULL;         /* Because java always supports NaN in float */
  marpaESLIFJSONDecodeOption.numberActionp           = marpaESLIFJava_JSONDecodeNumberActionb;

  marpaESLIFRecognizerOption.userDatavp                = &marpaESLIFRecognizerContext;
  marpaESLIFRecognizerOption.readerCallbackp           = readerCallbackb;
  marpaESLIFRecognizerOption.disableThresholdb         = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithDisableThreshold_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.exhaustedb                = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithExhaustion_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.newlineb                  = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithNewline_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.trackb                    = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithTrack_METHODP) == JNI_TRUE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  marpaESLIFRecognizerOption.bufsizl                   = 0; /* Recommended value */
  marpaESLIFRecognizerOption.buftriggerperci           = 50; /* Recommended value */
  marpaESLIFRecognizerOption.bufaddperci               = 50; /* Recommended value */
  marpaESLIFRecognizerOption.ifActionResolverp         = marpaESLIFRecognizerIfActionResolver;
  marpaESLIFRecognizerOption.eventActionResolverp      = marpaESLIFRecognizerEventActionResolver;
  marpaESLIFRecognizerOption.regexActionResolverp      = marpaESLIFRecognizerRegexActionResolver;
  marpaESLIFRecognizerOption.importerp                 = marpaESLIFJava_recognizerImportb;

  /* Value interface is unmanaged: ESLIF does all the job */
  if (! marpaESLIFJava_valueContextInitb(envp, NULL /* eslifValueInterfacep */, eslifJSONDecoderp /* eslifGrammarp */, &marpaESLIFValueContext)) {
    goto err;
  }

  marpaESLIFValueOption.userDatavp = &marpaESLIFValueContext;
  marpaESLIFValueOption.importerp  = marpaESLIFJava_valueImportb;

  if (! ESLIFGrammar_contextb(envp, eslifJSONDecoderp, NULL /* marpaESLIFpp */, &marpaESLIFJSONDecoderp)) {
    goto err;
  }

  if (! marpaESLIFJSON_decodeb(marpaESLIFJSONDecoderp, &marpaESLIFJSONDecodeOption, &marpaESLIFRecognizerOption, &marpaESLIFValueOption)) {
    RAISEEXCEPTION(envp, "marpaESLIFJSON_decodeb failure");
  }

  if (GENERICSTACK_USED(marpaESLIFValueContext.objectStackp) != 1) {
    RAISEEXCEPTIONF(envp, "Internal value stack is %d instead of 1", GENERICSTACK_USED(marpaESLIFValueContext.objectStackp));
  }

  rcp = (jstring) GENERICSTACK_POP_PTR(marpaESLIFValueContext.objectStackp); /* It is a string by definition */
  goto done;

 err:
  rcp = NULL;

 done:
  return rcp;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFJSONDecoder_jniFree(JNIEnv *envp, jobject eslifJSONdecoderp)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFJSONDecoderp;

  if (ESLIFGrammar_contextb(envp, eslifJSONdecoderp, NULL /* marpaESLIFpp */, &marpaESLIFJSONDecoderp)) {
    marpaESLIFGrammar_freev(marpaESLIFJSONDecoderp); /* This is NULL protected */
  }
}

/*****************************************************************************/
static short marpaESLIFJava_JSONDecodeNumberActionb(void *userDatavp, char *strings, size_t stringl, marpaESLIFValueResult_t *marpaESLIFValueResultp)
/*****************************************************************************/
{
  /* We always use Math::BigFloat->new(strings) */
  static const char        *funcs = "marpaESLIFJava_JSONDecodeNumberAction";
  marpaESLIFValueContext_t *marpaESLIFValueContextp = (marpaESLIFValueContext_t *) userDatavp;
  JNIEnv                   *envp                    = marpaESLIFValueContextp->envp;
  jstring                   numberp;
  jobject                   objectp;
  short                     rcb;

  numberp = (*envp)->NewStringUTF(envp, (const char *) strings);
  if (numberp == NULL) {
    /* We want OUR exception to be raised */
    RAISEEXCEPTIONF(envp, "NewStringUTF(\"%s\") failure", strings);
  }

  objectp = (*envp)->NewObject(envp, JAVA_MATH_BIGDECIMAL_CLASSP, JAVA_MATH_BIGDECIMAL_CLASS_String_init_METHODP, numberp);
  if (objectp == NULL) {
    (*envp)->DeleteLocalRef(envp, numberp);
    RAISEEXCEPTION(envp, "NewObject failure");
  }
  (*envp)->DeleteLocalRef(envp, numberp);

  /* Note that we do NOT need a global reference: this is a C callback within the same and single JNI call */

  marpaESLIFValueResultp->type               = MARPAESLIF_VALUE_TYPE_PTR;
  marpaESLIFValueResultp->contextp           = MARPAESLIF_JNI_CONTEXT;
  marpaESLIFValueResultp->representationp    = marpaESLIFJava_representationCallbackb;
  marpaESLIFValueResultp->u.p.p              = (void *) objectp;
  marpaESLIFValueResultp->u.p.shallowb       = 0;
  marpaESLIFValueResultp->u.p.freeUserDatavp = NULL;
  marpaESLIFValueResultp->u.p.freeCallbackp  = marpaESLIFJava_JSONDecodeFreeCallbackv;

  rcb = 1;
  goto done;

 err:
  rcb = 0;

 done:
  return rcb;
}

/*****************************************************************************/
static void marpaESLIFJava_JSONDecodeFreeCallbackv(void *userDatavp, marpaESLIFValueResult_t *marpaESLIFValueResultp)
/*****************************************************************************/
{
  /* Same as marpaESLIFJava_genericFreeCallbackv but we KNOW that we work on a local reference */
  /* because this is a C callback within one single JNI call. */
  static const char *funcs = "marpaESLIFJava_JSONDecodeFreeCallbackv";
  JNIEnv            *envp;

  /* Free callback is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    return;
  }
  switch (marpaESLIFValueResultp->type) {
  case MARPAESLIF_VALUE_TYPE_PTR:
    /* This is a local reference */
    if (marpaESLIFValueResultp->u.p.p != NULL) {
      (*envp)->DeleteLocalRef(envp, (jobject) marpaESLIFValueResultp->u.p.p);
    }
    break;
  default:
    break;
  }
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFSymbol_jniNew(JNIEnv *envp, jobject eslifSymbolp, jstring typep, jbyteArray utf8byteArrayp, jstring modifiersp)
/*****************************************************************************/
{
  static const char                *funcs = "Java_org_parser_marpa_ESLIFSymbol_jniNew";
  jbyte                            *utf8bytep = NULL;
  marpaESLIFSymbol_t               *marpaESLIFSymbolp;
  marpaESLIF_t                     *marpaESLIFp;
  marpaESLIFString_t                marpaESLIFString;
  jobject                           BYTEBUFFER(marpaESLIFSymbol);
  jsize                             utf8lengthl;
  const char                       *types;
  const char                       *modifiers = NULL;
  jboolean                          isCopy;

  /* Always update genericLogger context */
  if (! ESLIFSymbol_contextb(envp, eslifSymbolp, &marpaESLIFp, NULL /* marpaESLIFSymbolpp */)) {
    goto err;
  }

  /* Get information from the stack */
  if (utf8byteArrayp == NULL) {
    RAISEEXCEPTION(envp, "ByteArray argument is NULL");
  }
  utf8lengthl = (*envp)->GetArrayLength(envp, utf8byteArrayp);
  if (utf8lengthl <= 0) {
    RAISEEXCEPTION(envp, "ByteArray is empty");
  }
  utf8bytep = (jbyte *) malloc(utf8lengthl * sizeof(jbyte));
  if (utf8bytep == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  (*envp)->GetByteArrayRegion(envp, utf8byteArrayp, 0, utf8lengthl, utf8bytep);

  types = (*envp)->GetStringUTFChars(envp, typep, &isCopy);
  if (types == NULL) {
    RAISEEXCEPTION(envp, "GetStringUTFChars failure");
  }

  if (modifiersp != NULL) {
    modifiers = (*envp)->GetStringUTFChars(envp, modifiersp, &isCopy);
    if (modifiers == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  /* Create C object */
  marpaESLIFString.bytep          = utf8bytep;
  marpaESLIFString.bytel          = utf8lengthl;
  marpaESLIFString.encodingasciis = "UTF-8";
  marpaESLIFString.asciis         = NULL;

  marpaESLIFSymbolp = (strcmp(types, "regex") == 0) ? marpaESLIFSymbol_regex_newp(marpaESLIFp, &marpaESLIFString, (char *) modifiers) : marpaESLIFSymbol_string_newp(marpaESLIFp, &marpaESLIFString, (char *) modifiers);
  if (marpaESLIFSymbolp == NULL) {
    if (strcmp(types, "regex") == 0) {
      RAISEEXCEPTIONF(envp, "marpaESLIFSymbol_regex_newp failure, %s", strerror(errno));
    } else {
      RAISEEXCEPTIONF(envp, "marpaESLIFSymbol_string_newp failure, %s", strerror(errno));
    }
  }

  /* Store the object */
  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIFSymbol, marpaESLIFSymbolp);  
  (*envp)->CallVoidMethod(envp, eslifSymbolp, MARPAESLIF_ESLIFSYMBOL_CLASS_setMarpaESLIFSymbolp_METHODP, BYTEBUFFER(marpaESLIFSymbol));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  goto done;
  
 err:
  Java_org_parser_marpa_ESLIFSymbol_jniFree(envp, eslifSymbolp);

 done:
  if (utf8bytep != NULL) {
    free(utf8bytep);
  }
  if ((typep != NULL) && (types != NULL)) {
    (*envp)->ReleaseStringUTFChars(envp, typep, types);
  }
  if ((modifiersp != NULL) && (modifiers != NULL)) {
    (*envp)->ReleaseStringUTFChars(envp, modifiersp, modifiers);
  }
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFSymbol_jniFree(JNIEnv *envp, jobject eslifSymbolp)
/*****************************************************************************/
{
  marpaESLIFSymbol_t *marpaESLIFSymbolp;

  if (ESLIFSymbol_contextb(envp, eslifSymbolp, NULL /* marpaESLIFpp */, &marpaESLIFSymbolp)) {
    marpaESLIFSymbol_freev(marpaESLIFSymbolp); /* This is NULL protected */
  }
}

/*****************************************************************************/
JNIEXPORT jbyteArray JNICALL Java_org_parser_marpa_ESLIFSymbol_jniTest(JNIEnv *envp, jobject eslifSymbolp, jbyteArray jinputp)
/*****************************************************************************/
{
  static const char           *funcs = "Java_org_parser_marpa_ESLIFSymbol_jniTest";
  marpaESLIFSymbol_t          *marpaESLIFSymbolp;
  jbyteArray                   byteArrayp = NULL;
  jbyte                       *inputp = NULL;
  jsize                        inputl;
  short                        matchb;
  marpaESLIFValueResultArray_t marpaESLIFValueResultArray;

  marpaESLIFValueResultArray.p        = NULL;
  marpaESLIFValueResultArray.sizel    = 0;
  marpaESLIFValueResultArray.shallowb = 0;

  if (! ESLIFSymbol_contextb(envp, eslifSymbolp, NULL /* marpaESLIFpp */, &marpaESLIFSymbolp)) {
    goto err;
  }

  if (jinputp == NULL) {
    RAISEEXCEPTION(envp, "ByteArray argument is NULL");
  }
  inputl = (*envp)->GetArrayLength(envp, jinputp);
  if (inputl > 0) {
    inputp = (jbyte *) malloc(inputl * sizeof(jbyte));
    if (inputp == NULL) {
      RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
    }
    (*envp)->GetByteArrayRegion(envp, jinputp, 0, inputl, inputp);
  }

  if (! marpaESLIFSymbol_tryb(marpaESLIFSymbolp, (char *) inputp, (size_t) inputl, &matchb, &marpaESLIFValueResultArray)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFSymbol_tryb failure, %s", strerror(errno));
  }

  if (matchb) {
    byteArrayp = (*envp)->NewByteArray(envp, (jsize) marpaESLIFValueResultArray.sizel);
    if (byteArrayp == NULL) {
      goto err;
    }
    (*envp)->SetByteArrayRegion(envp, byteArrayp, (jsize) 0, (jsize) marpaESLIFValueResultArray.sizel, (jbyte *) marpaESLIFValueResultArray.p);
    if (HAVEEXCEPTION(envp)) {
      goto err;
    }
  }

  goto done;

 err:
  if (inputp != NULL) {
    free(inputp);
  }
  if (envp != NULL) {
    if (byteArrayp != NULL) {
      (*envp)->DeleteLocalRef(envp, byteArrayp);
    }
  }
  byteArrayp = NULL;

 done:
  if ((! marpaESLIFValueResultArray.shallowb) && (marpaESLIFValueResultArray.p != NULL)) {
    free(marpaESLIFValueResultArray.p);
  }
  return byteArrayp;
}

