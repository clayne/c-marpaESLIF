/* Java JNI bindings for marpaESLIF                          */
#include <jni.h>
#include <marpaESLIF.h>
#include <genericStack.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <genericLogger.h>

/* ---------------- */
/* Exported methods */
/* ---------------- */
JNIEXPORT jint    JNICALL JNI_OnLoad(JavaVM *vmp, void* reservedp);
JNIEXPORT void    JNICALL JNI_OnUnLoad(JavaVM *vmp, void* reservedp);

JNIEXPORT void    JNICALL Java_org_parser_marpa_ESLIF_jniNew    (JNIEnv *envp, jobject eslifp);
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIF_jniVersion(JNIEnv *envp, jobject eslifp);
JNIEXPORT void    JNICALL Java_org_parser_marpa_ESLIF_jniFree   (JNIEnv *envp, jobject eslifp);

JNIEXPORT void      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniNew               (JNIEnv *envp, jobject eslifGrammarp, jbyteArray utf8byteArrayp);
JNIEXPORT jint      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniNgrammar          (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jint      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentLevel      (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jstring   JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentDescription(JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jstring   JNICALL Java_org_parser_marpa_ESLIFGrammar_jniDescriptionByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level);
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentRuleIds    (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleIdsByLevel    (JNIEnv *envp, jobject eslifGrammarp, jint level);
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplay       (JNIEnv *envp, jobject eslifGrammarp, jint rule);
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleShow          (JNIEnv *envp, jobject eslifGrammarp, jint rule);
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplayByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule);
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleShowByLevel   (JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule);
JNIEXPORT jstring   JNICALL Java_org_parser_marpa_ESLIFGrammar_jniShow              (JNIEnv *envp, jobject eslifGrammarp);
JNIEXPORT jstring   JNICALL Java_org_parser_marpa_ESLIFGrammar_jniShowByLevel       (JNIEnv *envp, jobject eslifGrammarp, jint level);
JNIEXPORT jboolean  JNICALL Java_org_parser_marpa_ESLIFGrammar_jniParse             (JNIEnv *envp, jobject eslifGrammarp, jobject eslifRecognizerInterfacep, jobject eslifValueInterfacep);
JNIEXPORT void      JNICALL Java_org_parser_marpa_ESLIFGrammar_jniFree              (JNIEnv *envp, jobject eslifGrammarp);

JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniNew                    (JNIEnv *envp, jobject eslifRecognizerp, jobject eslifGrammarp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniScan                   (JNIEnv *envp, jobject eslifRecognizerp, jboolean initialEvents);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniResume                 (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeAlternativeLength(JNIEnv *envp, jobject eslifRecognizerp, jint lengthi);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeAlternative      (JNIEnv *envp, jobject eslifRecognizerp, jstring namep);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeComplete         (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeRead             (JNIEnv *envp, jobject eslifRecognizerp, jstring namep, jint lengthi);
JNIEXPORT jobjectArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeExpected         (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniEof                    (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jboolean     JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniIsEof                  (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniRead                   (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT jobjectArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniEvent                  (JNIEnv *envp, jobject eslifRecognizerp);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniEventOnOff             (JNIEnv *envp, jobject eslifRecognizerp, jstring symbolp, jobjectArray eventTypesp, jboolean onOff);
JNIEXPORT void         JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniFree                   (JNIEnv *envp, jobject eslifRecognizerp);

JNIEXPORT void      JNICALL Java_org_parser_marpa_ESLIFValue_jniNew                 (JNIEnv *envp, jobject eslifValuep, jobject eslifRecognizerp);
JNIEXPORT void      JNICALL Java_org_parser_marpa_ESLIFValue_jniFree                (JNIEnv *envp, jobject eslifValuep);

/* ---------- */
/* Structures */
/* ---------- */
typedef struct genericLoggerContext {
  jobject   objectp;              /* Current object - this can change at every call */
  jmethodID getLoggerInterfacep;
} genericLoggerContext_t;

typedef struct recognizerInterfaceContext {
  jobject      eslifRecognizerInterfacep;  /* Current recognizer interface instance - this can change at every call */
  jbyteArray   previousByteArrayp;         /* to prevent exhaustion of local references */
  jbyte       *previousDatap;
  jstring      previousEncodingp;
  const char  *previousUTFCharp;
} recognizerInterfaceContext_t;

typedef struct marpaESLIFClassCache {
  char     *classs;
  jclass    classp;
} marpaESLIFClassCache_t;

typedef struct marpaESLIFMethodCache {
  marpaESLIFClassCache_t *marpaESLIFClassCachep;
  char                   *methods;
  char                   *signatures;
  short                   staticb;
  jmethodID               methodp;
} marpaESLIFMethodCache_t;

typedef struct marpaESLIFFieldCache {
  marpaESLIFClassCache_t *marpaESLIFClassCachep;
  char                   *fields;
  char                   *signatures;
  short                   staticb;
  jfieldID                fieldp;
} marpaESLIFFieldCache_t;

typedef struct valueInterfaceContext {
  jobject                  objectp;
  marpaESLIFClassCache_t   classCache;           /* Cache of current class */
  marpaESLIFMethodCache_t *methodCachep;         /* Cache of method IDs */
  size_t                   methodCacheSizel;
  jmethodID                methodp;              /* Current resolved method ID */
  char                    *actions;              /* shallow copy of last resolved name */
  genericStack_t          *stackp;               /* Stack of objects */
} valueInterfaceContext_t;

typedef struct marpaESLIF_stringGenerator { /* We use genericLogger to generate strings */
  char        *s;
  size_t       l;
  short        okb;
} marpaESLIF_stringGenerator_t;

/* ------ */
/* Macros */
/* ------ */
#define MARPAESLIF_JNI_VERSION                    JNI_VERSION_1_4 /* Because of NIO Support */
#define MARPAESLIF_FILENAMES                      "marpaESLIFJava.c"
#define MARPAESLIF_ESLIFEXCEPTION_CLASS           "org/parser/marpa/ESLIFException"
#define MARPAESLIF_ESLIF_CLASS                    "org/parser/marpa/ESLIF"
#define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS     "org/parser/marpa/ESLIFLoggerInterface"
#define MARPAESLIF_ESLIFGRAMMAR_CLASS             "org/parser/marpa/ESLIFGrammar"
#define MARPAESLIF_ESLIFRECOGNIZER_CLASS          "org/parser/marpa/ESLIFRecognizer"
#define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS "org/parser/marpa/ESLIFRecognizerInterface"
#define MARPAESLIF_ESLIFVALUE_CLASS               "org/parser/marpa/ESLIFValue"
#define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS      "org/parser/marpa/ESLIFValueInterface"
#define MARPAESLIF_OBJECT_CLASS                   "java/lang/Object"
#define MARPAESLIF_CLASS_CLASS                    "java/lang/Class"
#define MARPAESLIF_ESLIFEVENTTYPE_CLASS           "org/parser/marpa/ESLIFEventType"
#define MARPAESLIF_ESLIFEVENT_CLASS               "org/parser/marpa/ESLIFEvent"
#define MARPAESLIF_STRING_CLASS                   "java/lang/String"

#define MARPAESLIF_ESLIFVALUEINTERFACE_SYMBOLACTION_SIGNATURE "(Ljava/nio/ByteBuffer;)Ljava/lang/Object;"
#define MARPAESLIF_ESLIFVALUEINTERFACE_RULEACTION_SIGNATURE   "([Ljava/lang/Object;)Ljava/lang/Object;"

/* -------------------------------- */
/* Globals and accessors as macros */
/* -------------------------------- */
static JavaVM *marpaESLIF_vmp;

static marpaESLIFClassCache_t marpaESLIFClassCacheArrayp[] = {
  #define MARPAESLIF_ESLIFEXCEPTION_CLASSCACHE           marpaESLIFClassCacheArrayp[0]
  #define MARPAESLIF_ESLIFEXCEPTION_CLASSP               marpaESLIFClassCacheArrayp[0].classp
  {       MARPAESLIF_ESLIFEXCEPTION_CLASS,               NULL },

  #define MARPAESLIF_ESLIF_CLASSCACHE                    marpaESLIFClassCacheArrayp[1]
  #define MARPAESLIF_ESLIF_CLASSP                        marpaESLIFClassCacheArrayp[1].classp
  {       MARPAESLIF_ESLIF_CLASS,                        NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE     marpaESLIFClassCacheArrayp[2]
  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSP         marpaESLIFClassCacheArrayp[2].classp
  {       MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS,         NULL },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE             marpaESLIFClassCacheArrayp[3]
  #define MARPAESLIF_ESLIFGRAMMAR_CLASSP                 marpaESLIFClassCacheArrayp[3].classp
  {       MARPAESLIF_ESLIFGRAMMAR_CLASS,                 NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE          marpaESLIFClassCacheArrayp[4]
  #define MARPAESLIF_ESLIFRECOGNIZER_CLASSP              marpaESLIFClassCacheArrayp[4].classp
  {       MARPAESLIF_ESLIFRECOGNIZER_CLASS,              NULL },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE marpaESLIFClassCacheArrayp[5]
  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSP     marpaESLIFClassCacheArrayp[5].classp
  {       MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS,     NULL },

  #define MARPAESLIF_ESLIFVALUE_CLASSCACHE               marpaESLIFClassCacheArrayp[6]
  #define MARPAESLIF_ESLIFVALUE_CLASSP                   marpaESLIFClassCacheArrayp[6].classp
  {       MARPAESLIF_ESLIFVALUE_CLASS,                   NULL },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE      marpaESLIFClassCacheArrayp[7]
  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASSP          marpaESLIFClassCacheArrayp[7].classp
  {       MARPAESLIF_ESLIFVALUEINTERFACE_CLASS,          NULL },

  #define MARPAESLIF_OBJECT_CLASSCACHE                   marpaESLIFClassCacheArrayp[8]
  #define MARPAESLIF_OBJECT_CLASSP                       marpaESLIFClassCacheArrayp[8].classp
  {       MARPAESLIF_OBJECT_CLASS,                       NULL },

  #define MARPAESLIF_CLASS_CLASSCACHE                    marpaESLIFClassCacheArrayp[9]
  #define MARPAESLIF_CLASS_CLASSP                        marpaESLIFClassCacheArrayp[9].classp
  {       MARPAESLIF_CLASS_CLASS,                        NULL },

  #define MARPAESLIF_ESLIFEVENTTYPE_CLASSCACHE           marpaESLIFClassCacheArrayp[10]
  #define MARPAESLIF_ESLIFEVENTTYPE_CLASSP               marpaESLIFClassCacheArrayp[10].classp
  {       MARPAESLIF_ESLIFEVENTTYPE_CLASS,               NULL },

  #define MARPAESLIF_ESLIFEVENT_CLASSCACHE               marpaESLIFClassCacheArrayp[11]
  #define MARPAESLIF_ESLIFEVENT_CLASSP                   marpaESLIFClassCacheArrayp[11].classp
  {       MARPAESLIF_ESLIFEVENT_CLASS,                   NULL },

  #define MARPAESLIF_STRING_CLASSCACHE                   marpaESLIFClassCacheArrayp[12]
  #define MARPAESLIF_STRING_CLASSP                       marpaESLIFClassCacheArrayp[12].classp
  {       MARPAESLIF_STRING_CLASS,                       NULL },

  { NULL }
};

static marpaESLIFMethodCache_t marpaESLIFMethodCacheArrayp[] = {
  #define MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP                        marpaESLIFMethodCacheArrayp[0].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "getLoggerInterface",                 "()Lorg/parser/marpa/ESLIFLoggerInterface;", 0, NULL },

  #define MARPAESLIF_ESLIF_CLASS_getMarpaESLIFp_METHODP                             marpaESLIFMethodCacheArrayp[1].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "getMarpaESLIFp",                      "()Ljava/nio/ByteBuffer;", 0, NULL },

  #define MARPAESLIF_ESLIF_CLASS_setMarpaESLIFp_METHODP                             marpaESLIFMethodCacheArrayp[2].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "setMarpaESLIFp",                      "(Ljava/nio/ByteBuffer;)V", 0, NULL },

  #define MARPAESLIF_ESLIF_CLASS_getGenericLoggerContextp_METHODP                   marpaESLIFMethodCacheArrayp[3].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "getGenericLoggerContextp",            "()Ljava/nio/ByteBuffer;", 0, NULL },

  #define MARPAESLIF_ESLIF_CLASS_setGenericLoggerContextp_METHODP                   marpaESLIFMethodCacheArrayp[4].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "setGenericLoggerContextp",            "(Ljava/nio/ByteBuffer;)V", 0, NULL },

  #define MARPAESLIF_ESLIF_CLASS_getGenericLoggerp_METHODP                          marpaESLIFMethodCacheArrayp[5].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "getGenericLoggerp",                   "()Ljava/nio/ByteBuffer;", 0, NULL },

  #define MARPAESLIF_ESLIF_CLASS_setGenericLoggerp_METHODP                          marpaESLIFMethodCacheArrayp[6].methodp
  {      &MARPAESLIF_ESLIF_CLASSCACHE,       "setGenericLoggerp",                   "(Ljava/nio/ByteBuffer;)V", 0, NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_trace_METHODP                       marpaESLIFMethodCacheArrayp[7].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "trace",                "(Ljava/lang/String;)V", 0, NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_debug_METHODP                       marpaESLIFMethodCacheArrayp[8].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "debug",                "(Ljava/lang/String;)V", 0, NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_info_METHODP                        marpaESLIFMethodCacheArrayp[9].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "info",                 "(Ljava/lang/String;)V", 0, NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_notice_METHODP                      marpaESLIFMethodCacheArrayp[10].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "notice",               "(Ljava/lang/String;)V", 0, NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_warning_METHODP                     marpaESLIFMethodCacheArrayp[11].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "warning",              "(Ljava/lang/String;)V", 0, NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_error_METHODP                       marpaESLIFMethodCacheArrayp[12].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "error",                "(Ljava/lang/String;)V", 0, NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_critical_METHODP                    marpaESLIFMethodCacheArrayp[13].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "critical",             "(Ljava/lang/String;)V", 0, NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_alert_METHODP                       marpaESLIFMethodCacheArrayp[14].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "alert",                "(Ljava/lang/String;)V", 0, NULL },

  #define MARPAESLIF_ESLIFLOGGERINTERFACE_CLASS_emergency_METHODP                   marpaESLIFMethodCacheArrayp[15].methodp
  {      &MARPAESLIF_ESLIFLOGGERINTERFACE_CLASSCACHE,       "emergency",            "(Ljava/lang/String;)V", 0, NULL },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP                 marpaESLIFMethodCacheArrayp[16].methodp
  {      &MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE, "getLoggerInterface",                 "()Lorg/parser/marpa/ESLIFLoggerInterface;", 0, NULL },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASS_getEslif_METHODP                            marpaESLIFMethodCacheArrayp[17].methodp
  {      &MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE, "getEslif",                           "()Lorg/parser/marpa/ESLIF;", 0, NULL },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASS_getMarpaESLIFGrammarp_METHODP               marpaESLIFMethodCacheArrayp[18].methodp
  {      &MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE, "getMarpaESLIFGrammarp",              "()Ljava/nio/ByteBuffer;", 0, NULL },

  #define MARPAESLIF_ESLIFGRAMMAR_CLASS_setMarpaESLIFGrammarp_METHODP               marpaESLIFMethodCacheArrayp[19].methodp
  {      &MARPAESLIF_ESLIFGRAMMAR_CLASSCACHE, "setMarpaESLIFGrammarp",              "(Ljava/nio/ByteBuffer;)V", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP              marpaESLIFMethodCacheArrayp[20].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getLoggerInterface",              "()Lorg/parser/marpa/ESLIFLoggerInterface;", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifGrammar_METHODP                  marpaESLIFMethodCacheArrayp[21].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getEslifGrammar",                 "()Lorg/parser/marpa/ESLIFGrammar;", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getMarpaESLIFRecognizerp_METHODP         marpaESLIFMethodCacheArrayp[22].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getMarpaESLIFRecognizerp",        "()Ljava/nio/ByteBuffer;", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_setMarpaESLIFRecognizerp_METHODP         marpaESLIFMethodCacheArrayp[23].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "setMarpaESLIFRecognizerp",        "(Ljava/nio/ByteBuffer;)V", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifRecognizerInterface_METHODP      marpaESLIFMethodCacheArrayp[24].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getEslifRecognizerInterface",     "()Lorg/parser/marpa/ESLIFRecognizerInterface;", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_getMarpaESLIFRecognizerContextp_METHODP  marpaESLIFMethodCacheArrayp[25].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "getMarpaESLIFRecognizerContextp", "()Ljava/nio/ByteBuffer;", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_setMarpaESLIFRecognizerContextp_METHODP  marpaESLIFMethodCacheArrayp[26].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "setMarpaESLIFRecognizerContextp", "(Ljava/nio/ByteBuffer;)V", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_setCanContinue_METHODP                   marpaESLIFMethodCacheArrayp[27].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "setCanContinue",                  "(Z)V", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZER_CLASS_setExhausted_METHODP                     marpaESLIFMethodCacheArrayp[28].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZER_CLASSCACHE, "setExhausted",                    "(Z)V", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_read_METHODP                    marpaESLIFMethodCacheArrayp[29].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "read",                   "()S", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isEof_METHODP                   marpaESLIFMethodCacheArrayp[30].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isEof",                  "()Z", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isCharacterStream_METHODP       marpaESLIFMethodCacheArrayp[31].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isCharacterStream",      "()Z", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_encoding_METHODP                marpaESLIFMethodCacheArrayp[32].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "encoding",               "()Ljava/lang/String;", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_data_METHODP                    marpaESLIFMethodCacheArrayp[33].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "data",                   "()[B", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithDisableThreshold_METHODP  marpaESLIFMethodCacheArrayp[34].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isWithDisableThreshold", "()Z", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithExhaustion_METHODP        marpaESLIFMethodCacheArrayp[35].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isWithExhaustion",       "()Z", 0, NULL },

  #define MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithNewline_METHODP           marpaESLIFMethodCacheArrayp[36].methodp
  {      &MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASSCACHE, "isWithNewline",          "()Z", 0, NULL },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getLoggerInterfacep_METHODP                   marpaESLIFMethodCacheArrayp[37].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getLoggerInterface",                   "()Lorg/parser/marpa/ESLIFLoggerInterface;", 0, NULL },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getEslifRecognizer_METHODP                    marpaESLIFMethodCacheArrayp[38].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getEslifRecognizer",                   "()Lorg/parser/marpa/ESLIFRecognizer;", 0, NULL },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getMarpaESLIFValuep_METHODP                   marpaESLIFMethodCacheArrayp[39].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getMarpaESLIFValuep",                  "()Ljava/nio/ByteBuffer;", 0, NULL },

  #define MARPAESLIF_ESLIFVALUE_CLASS_setMarpaESLIFValuep_METHODP                   marpaESLIFMethodCacheArrayp[40].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "setMarpaESLIFValuep",                  "(Ljava/nio/ByteBuffer;)V", 0, NULL },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getEslifValueInterface_METHODP                marpaESLIFMethodCacheArrayp[41].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getEslifValueInterface",               "()Lorg/parser/marpa/ESLIFValueInterface;", 0, NULL },

  #define MARPAESLIF_ESLIFVALUE_CLASS_getMarpaESLIFValueContextp_METHODP            marpaESLIFMethodCacheArrayp[42].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "getMarpaESLIFValueContextp",           "()Ljava/nio/ByteBuffer;", 0, NULL },

  #define MARPAESLIF_ESLIFVALUE_CLASS_setMarpaESLIFValueContextp_METHODP            marpaESLIFMethodCacheArrayp[43].methodp
  {      &MARPAESLIF_ESLIFVALUE_CLASSCACHE, "setMarpaESLIFValueContextp",           "(Ljava/nio/ByteBuffer;)V", 0, NULL },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithHighRankOnly_METHODP           marpaESLIFMethodCacheArrayp[44].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "isWithHighRankOnly",          "()Z", 0, NULL },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithOrderByRank_METHODP            marpaESLIFMethodCacheArrayp[45].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "isWithOrderByRank",           "()Z", 0, NULL },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithAmbiguous_METHODP              marpaESLIFMethodCacheArrayp[46].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "isWithAmbiguous",             "()Z", 0, NULL },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithNull_METHODP                   marpaESLIFMethodCacheArrayp[47].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "isWithNull",                  "()Z", 0, NULL },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_maxParses_METHODP                    marpaESLIFMethodCacheArrayp[48].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "maxParses",                   "()I", 0, NULL },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setResult_METHODP                    marpaESLIFMethodCacheArrayp[49].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "setResult",                   "(Ljava/lang/Object;)V", 0, NULL },

  #define MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_getResult_METHODP                    marpaESLIFMethodCacheArrayp[50].methodp
  {      &MARPAESLIF_ESLIFVALUEINTERFACE_CLASSCACHE, "getResult",                   "()Ljava/lang/Object;", 0, NULL },

  #define MARPAESLIF_OBJECT_CLASS_getClass_METHODP                                  marpaESLIFMethodCacheArrayp[51].methodp
  {      &MARPAESLIF_OBJECT_CLASSCACHE, "getClass",                                 "()Ljava/lang/Class;", 0, NULL },

  #define MARPAESLIF_CLASS_CLASS_getName_METHODP                                    marpaESLIFMethodCacheArrayp[52].methodp
  {      &MARPAESLIF_CLASS_CLASSCACHE, "getName",                                   "()Ljava/lang/String;", 0, NULL },

  #define MARPAESLIF_ESLIFEVENTTYPE_CLASS_get_METHODP                               marpaESLIFMethodCacheArrayp[53].methodp
  {      &MARPAESLIF_ESLIFEVENTTYPE_CLASSCACHE, "get",                              "(I)Lorg/parser/marpa/ESLIFEventType;", 1 /* static */, NULL },

  #define MARPAESLIF_ESLIFEVENTTYPE_CLASS_getCode_METHODP                           marpaESLIFMethodCacheArrayp[54].methodp
  {      &MARPAESLIF_ESLIFEVENTTYPE_CLASSCACHE, "getCode",                          "()I", 0, NULL },

  #define MARPAESLIF_ESLIFEVENT_CLASS_init_METHODP                                  marpaESLIFMethodCacheArrayp[55].methodp
  {      &MARPAESLIF_ESLIFEVENT_CLASSCACHE, "<init>",                               "(Lorg/parser/marpa/ESLIFEventType;Ljava/lang/String;Ljava/lang/String;)V", 0, NULL },

  { NULL }
};

static marpaESLIFFieldCache_t marpaESLIFFieldCacheArrayp[] = {
  { NULL }
};

/* -------------- */
/* Static methods */
/* -------------- */
static void  generateStringWithLoggerCallbackv(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs);
static void  genericLoggerCallbackv(void *userDatavp, genericLoggerLevel_t logLeveli, const char *msgs);
static short recognizerReaderCallbackb(void *userDatavp, char **inputcpp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingOfEncodingsp, char **encodingsp, size_t *encodinglp);
static short ESLIF_contextb(JNIEnv *envp, jobject eslifp, jobject objectp, jmethodID getLoggerInterfacep,
                            genericLogger_t **genericLoggerpp,
                            genericLoggerContext_t **genericLoggerContextpp,
                            marpaESLIF_t **marpaESLIFpp);
static short ESLIFGrammar_contextb(JNIEnv *envp, jobject eslifGrammarp, jobject objectp, jmethodID getLoggerInterfacep,
                                   genericLogger_t **genericLoggerpp,
                                   genericLoggerContext_t **genericLoggerContextpp,
                                   marpaESLIF_t **marpaESLIFpp,
                                   marpaESLIFGrammar_t **marpaESLIFGrammarpp);
static short ESLIFRecognizer_contextb(JNIEnv *envp, jobject eslifRecognizerp, jobject objectp, jmethodID getLoggerInterfacep,
                                      genericLogger_t **genericLoggerpp,
                                      genericLoggerContext_t **genericLoggerContextpp,
                                      marpaESLIF_t **marpaESLIFpp,
                                      marpaESLIFGrammar_t **marpaESLIFGrammarpp,
                                      marpaESLIFRecognizer_t **marpaESLIFRecognizerpp);
static short ESLIFValue_contextb(JNIEnv *envp, jobject eslifValuep, jobject objectp, jmethodID getLoggerInterfacep,
                                 genericLogger_t **genericLoggerpp,
                                 genericLoggerContext_t **genericLoggerContextpp,
                                 marpaESLIF_t **marpaESLIFpp,
                                 marpaESLIFGrammar_t **marpaESLIFGrammarpp,
                                 marpaESLIFRecognizer_t **marpaESLIFRecognizerpp,
                                 marpaESLIFValue_t **marpaESLIFValuepp);
static marpaESLIFValueRuleCallback_t   marpaESLIFValueRuleActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions);
static marpaESLIFValueSymbolCallback_t marpaESLIFValueSymbolActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions);
static short marpaESLIFValueRuleCallback(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb);
static short marpaESLIFValueSymbolCallback(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *bytep, size_t bytel, int resulti);
static jmethodID marpaESLIFValueActionResolver(JNIEnv *envp, valueInterfaceContext_t *valueInterfaceContextp, char *methods, char *signatures);
static void marpaESLIFValueInterfaceContextFree(JNIEnv *envp, valueInterfaceContext_t *valueInterfaceContextp, short onStackb);
static void marpaESLIFRecognizerInterfaceContextFree(JNIEnv *envp, recognizerInterfaceContext_t *recognizerInterfaceContextp, short onStackb);
static void marpaESLIFRecognizerInterfaceContextCleanup(JNIEnv *envp, recognizerInterfaceContext_t *recognizerInterfaceContextp);

/* --------------- */
/* Internal macros */
/* --------------- */

/* Check if there is a pending exception - if yes, our raise will NOT raise a new one */
/* Since we asked for JNI_VERSION_1_4 per def ExceptionCheck() is available */
#define HAVEEXCEPTION(envp) (*envp)->ExceptionCheck(envp)

/* Raise exception - I use a macro because I did not want to include stdarg in this file */
#define RAISEEXCEPTION(envp, message) do {                              \
    if (! HAVEEXCEPTION(envp)) {                                        \
      if (MARPAESLIF_ESLIFEXCEPTION_CLASSP != NULL) {                   \
        genericLogger_t              *genericLoggerp = NULL;            \
        marpaESLIF_stringGenerator_t  marpaESLIF_stringGenerator;       \
                                                                        \
        marpaESLIF_stringGenerator.s   = NULL;                          \
        marpaESLIF_stringGenerator.l   = 0;                             \
        marpaESLIF_stringGenerator.okb = 0;                             \
                                                                        \
        genericLoggerp = GENERICLOGGER_CUSTOM(generateStringWithLoggerCallbackv, (void *) &marpaESLIF_stringGenerator, GENERICLOGGER_LOGLEVEL_TRACE); \
        if (genericLoggerp != NULL) {                                   \
          GENERICLOGGER_TRACEF(genericLoggerp, "%s", message);          \
          if (marpaESLIF_stringGenerator.okb) {                         \
            (*envp)->ThrowNew(envp, MARPAESLIF_ESLIFEXCEPTION_CLASSP, marpaESLIF_stringGenerator.s); \
          }                                                             \
          free(marpaESLIF_stringGenerator.s);                           \
          GENERICLOGGER_FREE(genericLoggerp);                           \
        } else {                                                        \
          (*envp)->ThrowNew(envp, MARPAESLIF_ESLIFEXCEPTION_CLASSP, "Exception (no description)"); \
        }                                                               \
      }                                                                 \
    }                                                                   \
    goto err;                                                           \
} while (0)

#define RAISEEXCEPTIONF(envp, fmts, ...) do {                           \
    if (! HAVEEXCEPTION(envp)) {                                        \
      if (MARPAESLIF_ESLIFEXCEPTION_CLASSP != NULL) {                   \
        genericLogger_t              *genericLoggerp = NULL;            \
        marpaESLIF_stringGenerator_t  marpaESLIF_stringGenerator;       \
                                                                        \
        marpaESLIF_stringGenerator.s   = NULL;                          \
        marpaESLIF_stringGenerator.l   = 0;                             \
        marpaESLIF_stringGenerator.okb = 0;                             \
                                                                        \
        genericLoggerp = GENERICLOGGER_CUSTOM(generateStringWithLoggerCallbackv, (void *) &marpaESLIF_stringGenerator, GENERICLOGGER_LOGLEVEL_TRACE); \
        if (genericLoggerp != NULL) {                                   \
          GENERICLOGGER_TRACEF(genericLoggerp, fmts, __VA_ARGS__);      \
          if (marpaESLIF_stringGenerator.okb) {                         \
            (*envp)->ThrowNew(envp, MARPAESLIF_ESLIFEXCEPTION_CLASSP, marpaESLIF_stringGenerator.s); \
          }                                                             \
          free(marpaESLIF_stringGenerator.s);                           \
          GENERICLOGGER_FREE(genericLoggerp);                           \
        } else {                                                        \
          (*envp)->ThrowNew(envp, MARPAESLIF_ESLIFEXCEPTION_CLASSP, "Exception (no description)"); \
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
  jint                     rci;
  JNIEnv                  *envp;
  jclass                   classp                 = NULL;
  marpaESLIFClassCache_t  *marpaESLIFClassCachep  = marpaESLIFClassCacheArrayp;
  marpaESLIFMethodCache_t *marpaESLIFMethodCachep = marpaESLIFMethodCacheArrayp;
  marpaESLIFFieldCache_t  *marpaESLIFFieldCachep  = marpaESLIFFieldCacheArrayp;

  /* ------------------------------------------------ */
  /* It is safe to store JavaVMp in a global variable */
  /* ------------------------------------------------ */
  marpaESLIF_vmp = JavaVMp;

  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    RAISEEXCEPTION(envp, "Failed to get environment");
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
      /* We want OUR exception to be raised */
      (*envp)->ExceptionClear(envp);
      RAISEEXCEPTIONF(envp, "Failed to find class \"%s\"", marpaESLIFClassCachep->classs);
    }
    /* classp is a local reference and will vanish itself */
    marpaESLIFClassCachep->classp = (*envp)->NewGlobalRef(envp, classp);
    if (marpaESLIFClassCachep->classp == NULL) {
      RAISEEXCEPTION(envp, "NewGlobalRef failure");
    }
    marpaESLIFClassCachep++;
  }
  if (classp != NULL) {
    (*envp)->DeleteLocalRef(envp, classp);
    classp = NULL;
  }

  /* -------------------- */
  /* Get cache of methods */
  /* -------------------- */
  while (marpaESLIFMethodCachep->marpaESLIFClassCachep != NULL) {
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
      /* We want OUR exception to be raised */
      (*envp)->ExceptionClear(envp);
      RAISEEXCEPTIONF(envp,
                      "Failed to find %smethod \"%s\" with signature \"%s\" in class \"%s\"",
                      marpaESLIFMethodCachep->staticb ? "static " : "",
                      marpaESLIFMethodCachep->methods,
                      marpaESLIFMethodCachep->signatures,
                      marpaESLIFMethodCachep->marpaESLIFClassCachep->classs);
    }
    marpaESLIFMethodCachep++;
  }

  /* ------------------- */
  /* Get cache of fields */
  /* ------------------- */
  while (marpaESLIFFieldCachep->marpaESLIFClassCachep != NULL) {
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
      /* We want OUR exception to be raised */
      (*envp)->ExceptionClear(envp);
      RAISEEXCEPTIONF(envp,
                      "Failed to find %sfield \"%s\" with signature \"%s\" in class \"%s\"",
                      marpaESLIFMethodCachep->staticb ? "static " : "",
                      marpaESLIFFieldCachep->fields,
                      marpaESLIFFieldCachep->signatures,
                      marpaESLIFFieldCachep->marpaESLIFClassCachep->classs);
    }
    marpaESLIFFieldCachep++;
  }

  rci = MARPAESLIF_JNI_VERSION;
  goto done;

 err:
  rci = JNI_ERR;

 done:
  if (classp != NULL) {
    (*envp)->DeleteLocalRef(envp, classp);
  }
  return rci;
}

/*****************************************************************************/
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM *vmp, void* reservedp)
/*****************************************************************************/
{
  JNIEnv                 *envp;
  marpaESLIFClassCache_t *marpaESLIFClassCachep  = marpaESLIFClassCacheArrayp;

  /* ------------------------------------------------- */
  /* It was safe to store JavaVMp in a global variable */
  /* ------------------------------------------------- */
  if (((*vmp)->GetEnv(vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    RAISEEXCEPTION(envp, "Failed to get environment");
  }

  /* ---------------------------------------------- */
  /* Remove global references created by JNI_OnLoad
  /* ----------------------------------g------------ -*/
  while (marpaESLIFClassCachep->classs != NULL) {
    if (marpaESLIFClassCachep->classp != NULL) {
      (*envp)->DeleteGlobalRef(envp, marpaESLIFClassCachep->classp);
    }
    marpaESLIFClassCachep++;
  }

 err:
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIF_jniNew(JNIEnv *envp, jobject eslifp)
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

  genericLoggerContextp->objectp             = eslifp;
  genericLoggerContextp->getLoggerInterfacep = MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP;

  MARPAESLIF_PTR2BYTEBUFFER(genericLoggerContext, genericLoggerContextp);
 (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setGenericLoggerContextp_METHODP, BYTEBUFFER(genericLoggerContext));

 /* ------------------------------ */
 /* Create and save generic logger */
 /* ------------------------------ */
  genericLoggerp = genericLogger_newp(genericLoggerCallbackv, (void *) genericLoggerContextp, GENERICLOGGER_LOGLEVEL_TRACE);
  if (genericLoggerp == NULL) {
    RAISEEXCEPTIONF(envp, "genericLogger_newp failure, %s", strerror(errno));
  }
  MARPAESLIF_PTR2BYTEBUFFER(genericLogger, genericLoggerp);
  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setGenericLoggerp_METHODP, BYTEBUFFER(genericLogger));

  /* --------------------------- */
  /* Create and save marpaESLIFp */
  /* --------------------------- */
  marpaESLIFOption.genericLoggerp = genericLoggerp;
  marpaESLIFp = marpaESLIF_newp(&marpaESLIFOption);
  if (marpaESLIFp == NULL) {
    RAISEEXCEPTION(envp, "marpaESLIF_newp failure");
  }
  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIF, marpaESLIFp);
  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setMarpaESLIFp_METHODP, BYTEBUFFER(marpaESLIF));

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
  /* Always update genericLogger context */
  if (ESLIF_contextb(envp, eslifp, eslifp, MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP,
                       NULL /* genericLoggerpp */,
                       NULL /* genericLoggerContextpp */,
                       NULL /* marpaESLIFpp */)) {
    return (*envp)->NewStringUTF(envp, marpaESLIF_versions());
  }

  return NULL;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIF_jniFree(JNIEnv *envp, jobject eslifp)
/*****************************************************************************/
{
  genericLoggerContext_t *genericLoggerContextp;
  genericLogger_t        *genericLoggerp;
  marpaESLIF_t           *marpaESLIFp;

  /* Always update genericLogger context */
  if (! ESLIF_contextb(envp, eslifp, eslifp, MARPAESLIF_ESLIF_CLASS_getLoggerInterfacep_METHODP,
                       &genericLoggerp,
                       &genericLoggerContextp,
                       &marpaESLIFp)) {
    /* An exception was raised */
    goto done;
  }

  marpaESLIF_freev(marpaESLIFp); /* This is NULL protected */
  genericLogger_freev(&genericLoggerp); /* Ditto */
  if (genericLoggerContextp != NULL) {
    free(genericLoggerContextp);
  }

  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setGenericLoggerContextp_METHODP, NULL);
  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setGenericLoggerp_METHODP, NULL);
  (*envp)->CallVoidMethod(envp, eslifp, MARPAESLIF_ESLIF_CLASS_setMarpaESLIFp_METHODP, NULL);

 done:
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFGrammar_jniNew(JNIEnv *envp, jobject eslifGrammarp, jbyteArray utf8byteArrayp)
/*****************************************************************************/
{
  jbyte                            *utf8bytep = NULL;
  marpaESLIFGrammar_t              *marpaESLIFGrammarp;
  marpaESLIF_t                     *marpaESLIFp;
  marpaESLIFGrammarOption_t         marpaESLIFGrammarOption;
  jobject                           BYTEBUFFER(marpaESLIFGrammar);
  jsize                             utf8lengthl;
  jboolean                          isCopyb = JNI_FALSE;

  /* Always update genericLogger context */
  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              &marpaESLIFp,
                              NULL /* marpaESLIFGrammarpp */)) {
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
  marpaESLIFGrammarOption.encodingOfEncodings = "ASCII";

  marpaESLIFGrammarp = marpaESLIFGrammar_newp(marpaESLIFp, &marpaESLIFGrammarOption);
  if (marpaESLIFGrammarp == NULL) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_newp failure");
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
  int                  ngrammari;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
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
  int                  leveli;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
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
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  marpaESLIFString_t  *descp;
  jstring              stringp;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammar_currentb(marpaESLIFGrammarp, NULL, &descp)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_grammar_currentb failure");
  }

  /* We enforced UTF-8 when parsing the grammar, so description is also in UTF-8 */
  stringp = (*envp)->NewStringUTF(envp, descp->bytep);
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
  int                  leveli = (jint) level;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  marpaESLIFString_t  *descp;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammar_by_levelb(marpaESLIFGrammarp, leveli, NULL, NULL, &descp)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_grammar_by_levelb failure for level %d", leveli);
  }

  /* We enforced UTF-8 when parsing the grammar, so description is also in UTF-8 */
  /* If it fails the exception will be seen by the Java layer */
  return (*envp)->NewStringUTF(envp, descp->bytep);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniCurrentRuleIds(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  jintArray            intArray = NULL;
  jint                *intp = NULL;
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                 *ruleip;
  size_t               rulel;
  size_t               i;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
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
    RAISEEXCEPTIONF(envp, "malloc failure at %s:%d; %s", MARPAESLIF_FILENAMES, __LINE__, strerror(errno));
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
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleIdsByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  jintArray            intArray;
  jint                *intp = NULL;
  int                 *ruleip;
  size_t               rulel;
  size_t               i;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_rulearray_by_levelb(marpaESLIFGrammarp, &ruleip, &rulel, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_rulearray_by_levelb failure for level %d", leveli);
  }
  if (rulel <= 0) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_rulearray_currentb returned no rule");
  }
  intp = (jint *) malloc(sizeof(jint) * rulel);
  if (intp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure; %s", strerror(errno));
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
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplay(JNIEnv *envp, jobject eslifGrammarp, jint rule)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  rulei = (int) rule;
  char                *ruledisplays;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruledisplayform_currentb(marpaESLIFGrammarp, rulei, &ruledisplays)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_ruledisplayform_currentb failure for rule %d", rulei);
  }

  /* If it fails the exception will be seen by the Java layer */
  return (*envp)->NewStringUTF(envp, ruledisplays);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleShow(JNIEnv *envp, jobject eslifGrammarp, jint rule)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  rulei = (int) rule;
  char                *ruleshows;
  genericLogger_t     *genericLoggerp = NULL;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruleshowform_currentb(marpaESLIFGrammarp, rulei, &ruleshows)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_ruleshowform_currentb failure for rule %d", rulei);
  }

  /* If it fails the exception will be seen by the Java layer */
  return (*envp)->NewStringUTF(envp, ruleshows);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleDisplayByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  int                  rulei = (int) rule;
  char                *ruledisplays;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruledisplayform_by_levelb(marpaESLIFGrammarp, rulei, &ruledisplays, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_ruledisplayform_currentb failure for rule %d and level %d", rulei, leveli);
  }

  /* If it fails the exception will be seen by the Java layer */
  return (*envp)->NewStringUTF(envp, ruledisplays);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jintArray JNICALL Java_org_parser_marpa_ESLIFGrammar_jniRuleShowByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level, jint rule)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  int                  rulei = (int) rule;
  char                *ruleshows;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_ruleshowform_by_levelb(marpaESLIFGrammarp, rulei, &ruleshows, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_ruleshowform_by_levelb failure for rule %d and level %d", rulei, leveli);
  }

  /* If it fails the exception will be seen by the Java layer */
  return (*envp)->NewStringUTF(envp, ruleshows);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIFGrammar_jniShow(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  char                *shows;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammarshowform_currentb(marpaESLIFGrammarp, &shows)) {
    RAISEEXCEPTION(envp, "marpaESLIFGrammar_grammarshowform_currentb failure");
  }

  /* If it fails the exception will be seen by the Java layer */
  return (*envp)->NewStringUTF(envp, shows);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_org_parser_marpa_ESLIFGrammar_jniShowByLevel(JNIEnv *envp, jobject eslifGrammarp, jint level)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFGrammarp;
  int                  leveli = (int) level;
  char                *shows;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  if (! marpaESLIFGrammar_grammarshowform_by_levelb(marpaESLIFGrammarp, &shows, leveli, NULL)) {
    RAISEEXCEPTIONF(envp, "marpaESLIFGrammar_grammarshowform_by_levelb for level %d", leveli);
  }

  /* If it fails the exception will be seen by the Java layer */
  return (*envp)->NewStringUTF(envp, shows);

 err:
  return NULL;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFGrammar_jniParse(JNIEnv *envp, jobject eslifGrammarp, jobject eslifRecognizerInterfacep, jobject eslifValueInterfacep)
/*****************************************************************************/
{
  jclass                        classp  = NULL;
  jstring                       stringp = NULL;
  const char                   *classs  = NULL;
  jobject                       result  = NULL;
  marpaESLIFGrammar_t          *marpaESLIFGrammarp;
  marpaESLIFRecognizerOption_t  marpaESLIFRecognizerOption;
  marpaESLIFValueOption_t       marpaESLIFValueOption;
  short                         exhaustedb;
  recognizerInterfaceContext_t  recognizerInterfaceContext;
  valueInterfaceContext_t       valueInterfaceContext;
  jboolean                      rcb;
  jboolean                      isCopy;

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    goto err;
  }

  recognizerInterfaceContext.eslifRecognizerInterfacep = eslifRecognizerInterfacep;
  recognizerInterfaceContext.previousByteArrayp        = NULL;
  recognizerInterfaceContext.previousDatap             = NULL;
  recognizerInterfaceContext.previousEncodingp         = NULL;
  recognizerInterfaceContext.previousUTFCharp          = NULL;

  marpaESLIFRecognizerOption.userDatavp                = &recognizerInterfaceContext;
  marpaESLIFRecognizerOption.marpaESLIFReaderCallbackp = recognizerReaderCallbackb;
  marpaESLIFRecognizerOption.disableThresholdb         = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithDisableThreshold_METHODP) == JNI_TRUE);
  marpaESLIFRecognizerOption.exhaustedb                = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithExhaustion_METHODP) == JNI_TRUE);
  marpaESLIFRecognizerOption.newlineb                  = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithNewline_METHODP) == JNI_TRUE);
  marpaESLIFRecognizerOption.bufsizl                   = 0; /* Recommended value */
  marpaESLIFRecognizerOption.buftriggerperci           = 50; /* Recommended value */
  marpaESLIFRecognizerOption.bufaddperci               = 50; /* Recommended value */

  valueInterfaceContext.objectp                        = eslifValueInterfacep;
  valueInterfaceContext.classCache.classs              = NULL;
  valueInterfaceContext.classCache.classp              = NULL;
  valueInterfaceContext.methodCachep                   = NULL;
  valueInterfaceContext.methodCacheSizel               = 0;
  valueInterfaceContext.methodp                        = 0;
  valueInterfaceContext.actions                        = NULL;
  valueInterfaceContext.stackp                         = NULL;

  /* For run-time resolving of actions we need current jclass */
  classp = (*envp)->GetObjectClass(envp, eslifValueInterfacep);
  if (classp == NULL) {
    /* An exception was (must have been) raised */
    RAISEEXCEPTION(envp, "GetObjectClass failure");
  }

  stringp = (*envp)->CallObjectMethod(envp, classp, MARPAESLIF_CLASS_CLASS_getName_METHODP);
  if (stringp == NULL) {
    /* An exception was (must have been) raised */
    RAISEEXCEPTION(envp, "CallObjectMethod failure");
  }

  classs = (*envp)->GetStringUTFChars(envp, stringp, &isCopy);
  if (classs == NULL) {
    /* An exception was (must have been) raised */
    RAISEEXCEPTION(envp, "GetStringUTFChars failure");
  }
  valueInterfaceContext.classCache.classs = strdup(classs);
  if (valueInterfaceContext.classCache.classs == NULL) {
    RAISEEXCEPTIONF(envp, "strdup failure, %s", strerror(errno));
  }

  valueInterfaceContext.classCache.classp = (*envp)->NewGlobalRef(envp, classp);
  if (valueInterfaceContext.classCache.classp == NULL) {
    /* An exception was (must have been) raised */
    RAISEEXCEPTION(envp, "NewGlobalRef failure");
  }

  GENERICSTACK_NEW(valueInterfaceContext.stackp);
  if (GENERICSTACK_ERROR(valueInterfaceContext.stackp)) {
    RAISEEXCEPTIONF(envp, "Stack initialization failure, %s", strerror(errno));
  }
  
  marpaESLIFValueOption.userDatavp                     = &valueInterfaceContext;
  marpaESLIFValueOption.ruleActionResolverp            = marpaESLIFValueRuleActionResolver;
  marpaESLIFValueOption.symbolActionResolverp          = marpaESLIFValueSymbolActionResolver;
  marpaESLIFValueOption.freeActionResolverp            = NULL; /* We always push only integers */
  marpaESLIFValueOption.highRankOnlyb                  = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithHighRankOnly_METHODP) == JNI_TRUE);
  marpaESLIFValueOption.orderByRankb                   = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithOrderByRank_METHODP)  == JNI_TRUE);
  marpaESLIFValueOption.ambiguousb                     = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithAmbiguous_METHODP)    == JNI_TRUE);
  marpaESLIFValueOption.nullb                          = ((*envp)->CallBooleanMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_isWithNull_METHODP)         == JNI_TRUE);
  marpaESLIFValueOption.maxParsesi                     = (*envp)->CallIntMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_maxParses_METHODP);

  /* We never need marpaESLIF to keep the stack of values because it is managed directly in Java */
  if (! marpaESLIFGrammar_parseb(marpaESLIFGrammarp, &marpaESLIFRecognizerOption, &marpaESLIFValueOption, &exhaustedb, NULL)) {
    /* In theory the logger has been called with the ERROR level, already raising an exception. We will not overwrite the exception */
    /* in this case, c.f. the RAISEEXCEPTIONF macro. */
    RAISEEXCEPTION(envp, "Parse failure");
  }

  /* By definition, result is at indice 0 of our stack */
  if (GENERICSTACK_IS_PTR(valueInterfaceContext.stackp, 0)) {
    result = GENERICSTACK_GET_PTR(valueInterfaceContext.stackp, 0);
    /* We do NOT want this reference to be destroyed */
    GENERICSTACK_SET_NA(valueInterfaceContext.stackp, 0);
    if (GENERICSTACK_ERROR(valueInterfaceContext.stackp)) {
      /* We do not want a double free of result, c.f. the call to marpaESLIFValueInterfaceContextFree() */
      result = NULL;
      RAISEEXCEPTIONF(envp, "Stack set failure, %s", strerror(errno));
    }
  } else {
    result = NULL;
  }
  (*envp)->CallVoidMethod(envp, eslifValueInterfacep, MARPAESLIF_ESLIFVALUEINTERFACE_CLASS_setResult_METHODP, result);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  
  rcb = JNI_TRUE;
  goto done;

 err:
  if (envp != NULL) {
    if (result != NULL) {
      (*envp)->DeleteGlobalRef(envp, stringp);
    }
  }
  rcb = JNI_FALSE;

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
  marpaESLIFValueInterfaceContextFree(envp, &valueInterfaceContext, 1 /* onStackb */);
  marpaESLIFRecognizerInterfaceContextFree(envp, &recognizerInterfaceContext, 1 /* onStackb */);

  return rcb;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFGrammar_jniFree(JNIEnv *envp, jobject eslifGrammarp)
/*****************************************************************************/
{
  marpaESLIFGrammar_t *marpaESLIFGrammarp;

  if (ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
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
  if ((genericLoggerContextp->objectp == NULL) || (genericLoggerContextp->getLoggerInterfacep == NULL)) {
    /* Ditto */
    return;
  }

  /* Logging callack is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    return;
  }

  loggerInterfacep = (*envp)->CallObjectMethod(envp, genericLoggerContextp->objectp, genericLoggerContextp->getLoggerInterfacep);
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
    /* marpaESLIF is never logging with characters outside of 7-bits ASCII */
    stringp = (*envp)->NewStringUTF(envp, msgs);
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
  marpaESLIF_stringGenerator_t *contextp = (marpaESLIF_stringGenerator_t *) userDatavp;
  char                         *tmps;

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
static short ESLIF_contextb(JNIEnv *envp, jobject eslifp, jobject objectp, jmethodID getLoggerInterfacep, genericLogger_t **genericLoggerpp, genericLoggerContext_t **genericLoggerContextpp, marpaESLIF_t **marpaESLIFpp)
/*****************************************************************************/
{
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
  genericLoggerContextp->objectp             = objectp;
  genericLoggerContextp->getLoggerInterfacep = getLoggerInterfacep;

  if (genericLoggerContextpp != NULL) {
    *genericLoggerContextpp = genericLoggerContextp;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short ESLIFGrammar_contextb(JNIEnv *envp, jobject eslifGrammarp, jobject objectp, jmethodID getLoggerInterfacep, genericLogger_t **genericLoggerpp, genericLoggerContext_t **genericLoggerContextpp, marpaESLIF_t **marpaESLIFpp, marpaESLIFGrammar_t **marpaESLIFGrammarpp)
/*****************************************************************************/
{
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

  if (! ESLIF_contextb(envp, eslifp, objectp, getLoggerInterfacep, genericLoggerpp, genericLoggerContextpp, marpaESLIFpp)) {
    goto err;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
static short ESLIFRecognizer_contextb(JNIEnv *envp, jobject eslifRecognizerp, jobject objectp, jmethodID getLoggerInterfacep, genericLogger_t **genericLoggerpp, genericLoggerContext_t **genericLoggerContextpp, marpaESLIF_t **marpaESLIFpp, marpaESLIFGrammar_t **marpaESLIFGrammarpp, marpaESLIFRecognizer_t **marpaESLIFRecognizerpp)
/*****************************************************************************/
{
  jobject                       BYTEBUFFER(marpaESLIFRecognizer);
  jobject                       BYTEBUFFER(recognizerInterfaceContext);
  jobject                       eslifGrammarp;
  marpaESLIFRecognizer_t       *marpaESLIFRecognizerp;
  recognizerInterfaceContext_t *recognizerInterfaceContextp;
    
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
  BYTEBUFFER(recognizerInterfaceContext) = (*envp)->CallObjectMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getMarpaESLIFRecognizerContextp_METHODP);
  if (BYTEBUFFER(recognizerInterfaceContext) == NULL) {
    RAISEEXCEPTION(envp, "recognizerInterfaceContext bytebuffer is NULL");
  }
  recognizerInterfaceContextp = (*envp)->GetDirectBufferAddress(envp, BYTEBUFFER(recognizerInterfaceContext));
  if (recognizerInterfaceContextp == NULL) {
    RAISEEXCEPTION(envp, "recognizerInterfaceContextp is NULL");
  }
  recognizerInterfaceContextp->eslifRecognizerInterfacep = (*envp)->CallObjectMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifRecognizerInterface_METHODP);
  
  eslifGrammarp = (*envp)->CallObjectMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifGrammar_METHODP);
  if (eslifGrammarp == NULL) {
    RAISEEXCEPTION(envp, "eslifGrammarp is NULL");
  }

  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, objectp, getLoggerInterfacep, genericLoggerpp, genericLoggerContextpp, marpaESLIFpp, marpaESLIFGrammarpp)) {
    goto err;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniNew(JNIEnv *envp, jobject eslifRecognizerp, jobject eslifGrammarp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t           *marpaESLIFRecognizerp  = NULL;
  marpaESLIFGrammar_t              *marpaESLIFGrammarp;
  marpaESLIFRecognizerOption_t      marpaESLIFRecognizerOption;
  recognizerInterfaceContext_t     *recognizerInterfaceContextp;
  jobject                           eslifRecognizerInterfacep;
  jobject                           BYTEBUFFER(marpaESLIFRecognizer);
  jobject                           BYTEBUFFER(recognizerInterfaceContext);

  /* Always update genericLogger context */
  if (! ESLIFGrammar_contextb(envp, eslifGrammarp, eslifGrammarp, MARPAESLIF_ESLIFGRAMMAR_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              &marpaESLIFGrammarp)) {
    /* An exception was raised */
    goto err;
  }

  /* Get recognizer interface - class is protected to not accept the null argument */
  eslifRecognizerInterfacep = (*envp)->CallObjectMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getEslifRecognizerInterface_METHODP);

  /* Recognizer context is a structure containing the current recognizer object instance - updated at every recognizer call */
  recognizerInterfaceContextp = (recognizerInterfaceContext_t *) malloc(sizeof(recognizerInterfaceContext_t));
  if (recognizerInterfaceContextp == NULL) {
    RAISEEXCEPTIONF(envp, "malloc failure, %s", strerror(errno));
  }
  recognizerInterfaceContextp->eslifRecognizerInterfacep = eslifRecognizerInterfacep;
  recognizerInterfaceContextp->previousByteArrayp        = NULL;
  recognizerInterfaceContextp->previousDatap             = NULL;
  recognizerInterfaceContextp->previousEncodingp         = NULL;
  recognizerInterfaceContextp->previousUTFCharp          = NULL;

  /* Create C object */
  marpaESLIFRecognizerOption.userDatavp                = recognizerInterfaceContextp;
  marpaESLIFRecognizerOption.marpaESLIFReaderCallbackp = recognizerReaderCallbackb;
  marpaESLIFRecognizerOption.disableThresholdb         = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithDisableThreshold_METHODP) == JNI_TRUE);
  marpaESLIFRecognizerOption.exhaustedb                = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithExhaustion_METHODP) == JNI_TRUE);
  marpaESLIFRecognizerOption.newlineb                  = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isWithNewline_METHODP) == JNI_TRUE);
  marpaESLIFRecognizerOption.bufsizl                   = 0; /* Recommended value */
  marpaESLIFRecognizerOption.buftriggerperci           = 50; /* Recommended value */
  marpaESLIFRecognizerOption.bufaddperci               = 50; /* Recommended value */

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

  MARPAESLIF_PTR2BYTEBUFFER(recognizerInterfaceContext, recognizerInterfaceContextp);  
  (*envp)->CallVoidMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_setMarpaESLIFRecognizerContextp_METHODP, BYTEBUFFER(recognizerInterfaceContext));
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  goto done;
  
 err:
  if (recognizerInterfaceContextp != NULL) {
    free(recognizerInterfaceContextp);
  }
  Java_org_parser_marpa_ESLIFRecognizer_jniFree(envp, eslifRecognizerp);

 done:
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniScan(JNIEnv *envp, jobject eslifRecognizerp, jboolean initialEvents)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  short                   initialEventsb = (initialEvents == JNI_TRUE);
  short                   continueb;
  short                   exhaustedb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_scanb(marpaESLIFRecognizerp, initialEventsb, &continueb, &exhaustedb)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_scanb failure");
  }

  (*envp)->CallVoidMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_setCanContinue_METHODP, continueb ? JNI_TRUE : JNI_FALSE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  
  (*envp)->CallVoidMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_setExhausted_METHODP, exhaustedb ? JNI_TRUE : JNI_FALSE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  return;

 err:
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniResume(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  short                   continueb;
  short                   exhaustedb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_resumeb(marpaESLIFRecognizerp, &continueb, &exhaustedb)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_scanb failure");
  }

  (*envp)->CallVoidMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_setCanContinue_METHODP, continueb ? JNI_TRUE : JNI_FALSE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }
  
  (*envp)->CallVoidMethod(envp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_setExhausted_METHODP, exhaustedb ? JNI_TRUE : JNI_FALSE);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  return;

 err:
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeAlternativeLength(JNIEnv *envp, jobject eslifRecognizerp, jint lengthi)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_lexeme_alternative_lengthb(marpaESLIFRecognizerp, (size_t) lengthi)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_lexeme_alternative_lengthb failure");
  }

  return;

 err:
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeAlternative(JNIEnv *envp, jobject eslifRecognizerp, jstring namep)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  const char             *names = NULL;
  jboolean                isCopy;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (namep != NULL) {
    names = (*envp)->GetStringUTFChars(envp, namep, &isCopy);
    if (names == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  if (!  marpaESLIFRecognizer_lexeme_alternativeb(marpaESLIFRecognizerp, (char *) names)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_lexeme_alternativeb failure");
  }

 err: /* err and done share the same code */
  if (envp != NULL) {
    if ((namep != NULL) && (names != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, namep, names);
    }
  }
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeComplete(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_lexeme_completeb(marpaESLIFRecognizerp)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_lexeme_completeb failure");
  }

 err: /* err and done share the same code */
  return;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeRead(JNIEnv *envp, jobject eslifRecognizerp, jstring namep, jint lengthi)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  const char             *names = NULL;
  jboolean                isCopy;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (namep != NULL) {
    names = (*envp)->GetStringUTFChars(envp, namep, &isCopy);
    if (names == NULL) {
      RAISEEXCEPTION(envp, "GetStringUTFChars failure");
    }
  }

  if (!  marpaESLIFRecognizer_lexeme_readb(marpaESLIFRecognizerp, (char *) names, (size_t) lengthi)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_lexeme_readb failure");
  }

 err: /* err and done share the same code */
  if (envp != NULL) {
    if ((namep != NULL) && (names != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, namep, names);
    }
  }
  return;
}

/*****************************************************************************/
JNIEXPORT jobjectArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniLexemeExpected(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  size_t                  nLexeme;
  size_t                  i;
  char                  **lexemesArrayp;
  jobjectArray            objectArray = NULL;
  jstring                 string      = NULL;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_lexeme_expectedb(marpaESLIFRecognizerp, &nLexeme, &lexemesArrayp)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_lexeme_expectedb failure");
  }

  if (nLexeme > 0) {
    objectArray = (*envp)->NewObjectArray(envp, nLexeme, MARPAESLIF_STRING_CLASSP, NULL /* initialElement */);
    if (objectArray == NULL) {
      RAISEEXCEPTION(envp, "NewObjectArray failure");
    }
    for (i = 0; i < nLexeme; i++) {
      string = (*envp)->NewStringUTF(envp, (lexemesArrayp[i] != NULL) ? lexemesArrayp[i] : "");
      if (string == NULL) {
        RAISEEXCEPTION(envp, "NewStringUTF failure");
      }
      (*envp)->SetObjectArrayElement(envp, objectArray, i, string);
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
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniEof(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_eofb(marpaESLIFRecognizerp)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_eofb failure");
  }

 err: /* err and done share the same code */
  return;
}

/*****************************************************************************/
JNIEXPORT jboolean JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniIsEof(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  short                   eofb;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
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
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniRead(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (!  marpaESLIFRecognizer_readb(marpaESLIFRecognizerp, NULL /* inputsp */, NULL /* inputlp */, NULL /* eofbp */)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_readb failure");
  }

 err: /* err and done share the same code */
  return;
}

/*****************************************************************************/
JNIEXPORT jobjectArray JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniEvent(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  size_t                  eventArrayl;
  marpaESLIFEvent_t      *eventArrayp;
  jobjectArray            objectArray = NULL;
  jobject                 object      = NULL;
  jobject                 eventType   = NULL;
  jstring                 symbol      = NULL;
  jstring                 event       = NULL;
  size_t                  i;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    goto err;
  }

  if (! marpaESLIFRecognizer_eventb(marpaESLIFRecognizerp, &eventArrayl, &eventArrayp)) {
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_eventb failure");
  }

  if (eventArrayl > 0) {
    objectArray = (*envp)->NewObjectArray(envp, eventArrayl, MARPAESLIF_ESLIFEVENT_CLASSP, NULL /* initialElement */);
    if (objectArray == NULL) {
      RAISEEXCEPTION(envp, "NewObjectArray failure");
    }

    for (i = 0; i < eventArrayl; i++) {
      eventType = NULL;
      symbol    = NULL;
      event     = NULL;

      eventType = (*envp)->CallStaticObjectMethod(envp, MARPAESLIF_ESLIFEVENTTYPE_CLASSP, MARPAESLIF_ESLIFEVENTTYPE_CLASS_get_METHODP, eventArrayp[i].type);
      if (eventType == NULL) {
        RAISEEXCEPTION(envp, "CallStaticObjectMethod failure");
      }

      symbol = (*envp)->NewStringUTF(envp, (eventArrayp[i].symbols != NULL) ? eventArrayp[i].symbols : "");
      if (symbol == NULL) {
        RAISEEXCEPTION(envp, "NewStringUTF failure");
      }

      event = (*envp)->NewStringUTF(envp, (eventArrayp[i].events != NULL) ? eventArrayp[i].events : "");
      if (event == NULL) {
        RAISEEXCEPTION(envp, "NewStringUTF failure");
      }

      object = (*envp)->NewObject(envp, MARPAESLIF_ESLIFEVENT_CLASSP, MARPAESLIF_ESLIFEVENT_CLASS_init_METHODP, eventType, symbol, event);
      if (object == NULL) {
        RAISEEXCEPTION(envp, "NewObject failure");
      }

      (*envp)->SetObjectArrayElement(envp, objectArray, i, object);
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
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;
  jboolean                isCopy;
  const char             *symbols    = (symbolp != NULL) ? (*envp)->GetStringUTFChars(envp, symbolp, &isCopy) : NULL;
  marpaESLIFEventType_t   eventSeti  = MARPAESLIF_EVENTTYPE_NONE;
  short                   onoffb     = (onOff == JNI_TRUE);
  jobject                 eventTypep = NULL;
  jsize                   eventTypesLengthl;
  jint                    codei;
  jsize                   i;

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
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
        RAISEEXCEPTION(envp, "GetObjectArrayElement failure");
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
    RAISEEXCEPTION(envp, "marpaESLIFRecognizer_event_onoffb failure");
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
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFRecognizer_jniFree(JNIEnv *envp, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFRecognizer_t *marpaESLIFRecognizerp;

  if (ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              NULL /* marpaESLIFGrammarpp */,
                              &marpaESLIFRecognizerp)) {
    marpaESLIFRecognizer_freev(marpaESLIFRecognizerp); /* This is NULL protected */
  }
}

/*****************************************************************************/
static short ESLIFValue_contextb(JNIEnv *envp, jobject eslifValuep, jobject objectp, jmethodID getLoggerInterfacep, genericLogger_t **genericLoggerpp, genericLoggerContext_t **genericLoggerContextpp, marpaESLIF_t **marpaESLIFpp, marpaESLIFGrammar_t **marpaESLIFGrammarpp, marpaESLIFRecognizer_t **marpaESLIFRecognizerpp, marpaESLIFValue_t **marpaESLIFValuepp)
/*****************************************************************************/
{
  jobject            BYTEBUFFER(marpaESLIFValue);
  jobject            eslifRecognizerp;
  marpaESLIFValue_t *marpaESLIFValuep;
    
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

  eslifRecognizerp = (*envp)->CallObjectMethod(envp, eslifValuep, MARPAESLIF_ESLIFVALUE_CLASS_getEslifRecognizer_METHODP);
  if (eslifRecognizerp == NULL) {
    RAISEEXCEPTION(envp, "eslifRecognizerp is NULL");
  }

  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, objectp, getLoggerInterfacep, genericLoggerpp, genericLoggerContextpp, marpaESLIFpp, marpaESLIFGrammarpp, marpaESLIFRecognizerpp)) {
    goto err;
  }

  return 1;

 err:
  return 0;
}

/*****************************************************************************/
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFValue_jniNew(JNIEnv *envp, jobject eslifValuep, jobject eslifRecognizerp)
/*****************************************************************************/
{
  marpaESLIFValue_t                *marpaESLIFValuep  = NULL;
  marpaESLIFRecognizer_t           *marpaESLIFRecognizerp;
  marpaESLIFValueOption_t           marpaESLIFValueOption;
  jobject                           BYTEBUFFER(marpaESLIFValue);

  /* Always update genericLogger context */
  if (! ESLIFRecognizer_contextb(envp, eslifRecognizerp, eslifRecognizerp, MARPAESLIF_ESLIFRECOGNIZER_CLASS_getLoggerInterfacep_METHODP,
                                 NULL /* genericLoggerpp */,
                                 NULL /* genericLoggerContextpp */,
                                 NULL /* marpaESLIFpp */,
                                 NULL /* marpaESLIFGrammarpp */,
                                 &marpaESLIFRecognizerp)) {
    /* An exception was raised */
    goto err;
  }

  /* Create C object */
  marpaESLIFValueOption.userDatavp                = NULL;
  marpaESLIFValueOption.ruleActionResolverp       = NULL;
  marpaESLIFValueOption.symbolActionResolverp     = NULL;
  marpaESLIFValueOption.freeActionResolverp       = NULL;
  marpaESLIFValueOption.highRankOnlyb             = 0;
  marpaESLIFValueOption.ambiguousb                = 0;
  marpaESLIFValueOption.nullb                     = 0;
  marpaESLIFValueOption.maxParsesi                = 0;

  marpaESLIFValuep = marpaESLIFValue_newp(marpaESLIFRecognizerp, &marpaESLIFValueOption);
  if (marpaESLIFValuep == NULL) {
    RAISEEXCEPTION(envp, "marpaESLIFValue_newp failure");
  }

  /* Store the object */
  MARPAESLIF_PTR2BYTEBUFFER(marpaESLIFValue, marpaESLIFValuep);  
  (*envp)->CallVoidMethod(envp, eslifValuep, MARPAESLIF_ESLIFVALUE_CLASS_setMarpaESLIFValuep_METHODP, BYTEBUFFER(marpaESLIFValue));
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
JNIEXPORT void JNICALL Java_org_parser_marpa_ESLIFValue_jniFree(JNIEnv *envp, jobject eslifValuep)
/*****************************************************************************/
{
  marpaESLIFValue_t *marpaESLIFValuep;

  if (ESLIFValue_contextb(envp, eslifValuep, eslifValuep, MARPAESLIF_ESLIFVALUE_CLASS_getLoggerInterfacep_METHODP,
                              NULL /* genericLoggerpp */,
                              NULL /* genericLoggerContextpp */,
                              NULL /* marpaESLIFpp */,
                              NULL /* marpaESLIFGrammarpp */,
                              NULL /* marpaESLIFRecognizerpp */,
                              &marpaESLIFValuep)) {
    marpaESLIFValue_freev(marpaESLIFValuep); /* This is NULL protected */
  }
}

/*****************************************************************************/
static short recognizerReaderCallbackb(void *userDatavp, char **inputcpp, size_t *inputlp, short *eofbp, short *characterStreambp, char **encodingOfEncodingsp, char **encodingsp, size_t *encodinglp)
/*****************************************************************************/
{
  recognizerInterfaceContext_t *recognizerInterfaceContextp;
  jobject                       eslifRecognizerInterfacep;
  JNIEnv                        *envp;
  jbyteArray                     byteArrayp;
  jbyte                         *datap;
  jstring                        encodingp;
  const char                    *charp;
  jboolean                       isCopy;

  recognizerInterfaceContextp = (recognizerInterfaceContext_t *) userDatavp;
  eslifRecognizerInterfacep   = recognizerInterfaceContextp->eslifRecognizerInterfacep;

  /* Reader callack is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    return 0;
  }

  marpaESLIFRecognizerInterfaceContextCleanup(envp, recognizerInterfaceContextp);

  /* Call the read interface */
  if ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_read_METHODP) != JNI_TRUE) {
    return 0;
  }

  byteArrayp = (*envp)->CallObjectMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_data_METHODP);
  datap      = (byteArrayp != NULL) ? (*envp)->GetByteArrayElements(envp, byteArrayp, &isCopy) : NULL;
  encodingp  = (*envp)->CallObjectMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_encoding_METHODP);
  charp      = (encodingp != NULL) ? (*envp)->GetStringUTFChars(envp, encodingp, &isCopy) : NULL;  /* => The famous UTF-8 hardcoded below */

  *inputcpp             = (char *) datap;
  *inputlp              = (size_t) (byteArrayp != NULL) ? (*envp)->GetArrayLength(envp, byteArrayp) : 0;
  *eofbp                = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isEof_METHODP) == JNI_TRUE);
  *characterStreambp    = ((*envp)->CallBooleanMethod(envp, eslifRecognizerInterfacep, MARPAESLIF_ESLIFRECOGNIZERINTERFACE_CLASS_isCharacterStream_METHODP) == JNI_TRUE);
  *encodingOfEncodingsp = (charp != NULL) ? "UTF-8" : NULL;
  *encodingsp           = (char *) charp;
  *encodinglp           = (size_t) (charp != NULL ? strlen(charp) : 0);

  recognizerInterfaceContextp->previousByteArrayp = byteArrayp;         /* to prevent exhaustion of local references */
  recognizerInterfaceContextp->previousDatap      = datap;
  recognizerInterfaceContextp->previousEncodingp  = encodingp;
  recognizerInterfaceContextp->previousUTFCharp   = charp;

  return 1;
}

/*****************************************************************************/
static marpaESLIFValueRuleCallback_t marpaESLIFValueRuleActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions)
/*****************************************************************************/
{
  JNIEnv                  *envp;
  valueInterfaceContext_t *valueInterfaceContextp = (valueInterfaceContext_t *) userDatavp;
  jmethodID                methodp;

  /* resolver callack is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Make sure rule action method is resolved */
  methodp = marpaESLIFValueActionResolver(envp, valueInterfaceContextp, actions, MARPAESLIF_ESLIFVALUEINTERFACE_RULEACTION_SIGNATURE);
  if (methodp == NULL) {
    goto err;
  }

  valueInterfaceContextp->actions = actions;
  valueInterfaceContextp->methodp = methodp;

  return marpaESLIFValueRuleCallback;

 err:
  return NULL;
}

/*****************************************************************************/
static marpaESLIFValueSymbolCallback_t marpaESLIFValueSymbolActionResolver(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *actions)
/*****************************************************************************/
{
  JNIEnv                  *envp;
  valueInterfaceContext_t *valueInterfaceContextp = (valueInterfaceContext_t *) userDatavp;
  jmethodID                methodp;

  /* resolver callack is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Make sure symbol action method is resolved */
  methodp = marpaESLIFValueActionResolver(envp, valueInterfaceContextp, actions, MARPAESLIF_ESLIFVALUEINTERFACE_SYMBOLACTION_SIGNATURE);
  if (methodp == NULL) {
    goto err;
  }

  valueInterfaceContextp->actions = actions;
  valueInterfaceContextp->methodp = methodp;

  return marpaESLIFValueSymbolCallback;

 err:
  return NULL;
}

/*****************************************************************************/
static short marpaESLIFValueRuleCallback(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, int arg0i, int argni, int resulti, short nullableb)
/*****************************************************************************/
{
  JNIEnv                         *envp;
  valueInterfaceContext_t        *valueInterfaceContextp = (valueInterfaceContext_t *) userDatavp;
  genericStack_t                 *stackp                 = valueInterfaceContextp->stackp;
  jobjectArray                    list                   = NULL;
  jobject                         actionResult           = NULL;
  jobject                         actionResultGlobalRef  = NULL;
  jobject                         byteBuffer             = NULL;
  jobject                         actionResultPreviousGlobalRef;
  short                           rcb;
  int                             i;
  short                           intb;
  int                             indice;
  short                           arrayb;
  void                           *bytep;
  size_t                          bytel;

  /* Reader callack is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Prototype of a rule action is: public Object action(List<Object> list) */
  if (! nullableb) {
    /* Make the list */
    list = (*envp)->NewObjectArray(envp, argni - arg0i + 1, MARPAESLIF_OBJECT_CLASSP, NULL /* initialElement */);
    if (list == NULL) {
      RAISEEXCEPTION(envp, "NewObjectArray failure");
    }

    for (i = arg0i; i <= argni; i++) {
      if (! marpaESLIFValue_stack_is_intb(marpaESLIFValuep, i, &intb)) {
        RAISEEXCEPTION(envp, "marpaESLIFValue_stack_is_intb failure");
      }
      if (intb) {
        if (! marpaESLIFValue_stack_get_intb(marpaESLIFValuep, i, NULL /* contextip */, &indice)) {
          RAISEEXCEPTION(envp, "marpaESLIFValue_stack_is_intb failure");
        }
        /* This is an object created by the user interface */
        (*envp)->SetObjectArrayElement(envp, list, i - arg0i, (jobject) GENERICSTACK_GET_PTR(stackp, indice));
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }
      } else {
        /* This must be a lexeme - always in the form of an array */
        if (! marpaESLIFValue_stack_is_arrayb(marpaESLIFValuep, i, &arrayb)) {
          RAISEEXCEPTION(envp, "marpaESLIFValue_stack_is_arrayb failure");
        }
        if (! arrayb) {
          RAISEEXCEPTIONF(envp, "At indice %d pf stack, item is neither a PTR or an ARRAY", i);
        }
        if (! marpaESLIFValue_stack_get_arrayb(marpaESLIFValuep, i, NULL /* contextip */, &bytep, &bytel, NULL /* shallowbp */)) {
          RAISEEXCEPTION(envp, "marpaESLIFValue_stack_get_arrayb failure");
        }
        if (byteBuffer != NULL) {
          (*envp)->DeleteLocalRef(envp, byteBuffer);
        }
        byteBuffer = (*envp)->NewDirectByteBuffer(envp, bytep, (jlong) bytel);
        if (byteBuffer == NULL) {
          RAISEEXCEPTION(envp, "NewDirectByteBuffer failure");
        }
        (*envp)->SetObjectArrayElement(envp, list, i - arg0i, byteBuffer);
        if (HAVEEXCEPTION(envp)) {
          goto err;
        }
      }
    }
  }

  /* Call the action */
  actionResult = (*envp)->CallObjectMethod(envp, valueInterfaceContextp->objectp, valueInterfaceContextp->methodp, list);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  if (actionResult != NULL) {
    actionResultGlobalRef = (*envp)->NewGlobalRef(envp, actionResult);
    if (actionResultGlobalRef == NULL) {
      RAISEEXCEPTION(envp, "NewGlobalRef failure");
    }
  }

  /* Remember the global ref for the destroy phase */
  if (GENERICSTACK_IS_PTR(stackp, resulti)) {
    actionResultPreviousGlobalRef = (jobject) GENERICSTACK_GET_PTR(stackp, resulti);
    if (actionResultPreviousGlobalRef != NULL) {
      (*envp)->DeleteGlobalRef(envp, actionResultPreviousGlobalRef);
    }
  }
  GENERICSTACK_SET_PTR(stackp, actionResultGlobalRef, resulti);
  if (GENERICSTACK_ERROR(stackp)) {
    RAISEEXCEPTIONF(envp, "Stack set failure, %s", strerror(errno));
  }
  /* shallowb to a true value is very important because we get independant of an eventual free-action in the grammar */
  rcb =  marpaESLIFValue_stack_set_intb(marpaESLIFValuep, resulti, 1 /* context: any value != 0 */, resulti);
  if (! rcb) {
    RAISEEXCEPTION(envp, "marpaESLIFValue_stack_set_intb failure");
  }

  goto done;

 err:
  if (envp != NULL) {
    if (actionResultGlobalRef != NULL) {
      (*envp)->DeleteGlobalRef(envp, actionResultGlobalRef);
    }
  }
  rcb = 0;

 done:
  if (envp != NULL) {
    if (byteBuffer != NULL) {
      (*envp)->DeleteLocalRef(envp, byteBuffer);
    }
    if (list != NULL) {
      (*envp)->DeleteLocalRef(envp, list);
    }
    if (actionResult != NULL) {
      (*envp)->DeleteLocalRef(envp, actionResult);
    }
  }

  return rcb;
}

/*****************************************************************************/
static short marpaESLIFValueSymbolCallback(void *userDatavp, marpaESLIFValue_t *marpaESLIFValuep, char *bytep, size_t bytel, int resulti)
/*****************************************************************************/
{
  /* Almost exactly like marpaESLIFValueRuleCallback except that we construct a list of one element containing a jbyteArray that we do ourself */
  JNIEnv                  *envp;
  valueInterfaceContext_t *valueInterfaceContextp = (valueInterfaceContext_t *) userDatavp;
  genericStack_t          *stackp                 = valueInterfaceContextp->stackp;
  jobject                  byteBuffer             = NULL;
  jobject                  actionResult           = NULL;
  jobject                  actionResultGlobalRef  = NULL;
  jobject                  actionResultPreviousGlobalRef;
  short                    rcb;

  /* Reader callack is never running in another thread - no need to attach */
  if (((*marpaESLIF_vmp)->GetEnv(marpaESLIF_vmp, (void **) &envp, MARPAESLIF_JNI_VERSION) != JNI_OK) || (envp == NULL)) {
    goto err;
  }

  /* Prototype of a symbol action is: public Object symbolAction(ByteBuffer byteBuffer) */
  byteBuffer = (*envp)->NewDirectByteBuffer(envp, bytep, (jlong) bytel);
  if (byteBuffer == NULL) {
    RAISEEXCEPTION(envp, "NewDirectByteBuffer failure");
  }

  /* Call the symbol action */
  actionResult = (*envp)->CallObjectMethod(envp, valueInterfaceContextp->objectp, valueInterfaceContextp->methodp, byteBuffer);
  if (HAVEEXCEPTION(envp)) {
    goto err;
  }

  if (actionResult != NULL) {
    actionResultGlobalRef = (*envp)->NewGlobalRef(envp, actionResult);
    if (actionResultGlobalRef == NULL) {
      RAISEEXCEPTION(envp, "NewGlobalRef failure");
    }
  }

  /* Remember the global ref for the destroy phase */
  if (GENERICSTACK_IS_PTR(stackp, resulti)) {
    actionResultPreviousGlobalRef = (jobject) GENERICSTACK_GET_PTR(stackp, resulti);
    if (actionResultPreviousGlobalRef != NULL) {
      (*envp)->DeleteGlobalRef(envp, actionResultPreviousGlobalRef);
    }
  }
  GENERICSTACK_SET_PTR(stackp, actionResultGlobalRef, resulti);
  if (GENERICSTACK_ERROR(stackp)) {
    RAISEEXCEPTIONF(envp, "Stack set failure, %s", strerror(errno));
  }
  /* shallowb to a true value is very important because we get independant of an eventual free-action in the grammar */
  rcb =  marpaESLIFValue_stack_set_intb(marpaESLIFValuep, resulti, 1 /* context: any value != 0 */, resulti);
  if (! rcb) {
    RAISEEXCEPTION(envp, "marpaESLIFValue_stack_set_intb failure");
  }

  goto done;

 err:
  if (envp != NULL) {
    if (actionResultGlobalRef != NULL) {
      (*envp)->DeleteGlobalRef(envp, actionResultGlobalRef);
    }
  }
  rcb = 0;

 done:
  if (envp != NULL) {
    if (byteBuffer != NULL) {
      (*envp)->DeleteLocalRef(envp, byteBuffer);
    }
    if (actionResult != NULL) {
      (*envp)->DeleteLocalRef(envp, actionResult);
    }
  }

  return rcb;
}

/*****************************************************************************/
static jmethodID marpaESLIFValueActionResolver(JNIEnv *envp, valueInterfaceContext_t *valueInterfaceContextp, char *methods, char *signatures)
/*****************************************************************************/
{
  /* We do not used an internal hash, believing it is not worth the overhead */
  marpaESLIFMethodCache_t *methodCachep      = valueInterfaceContextp->methodCachep;
  size_t                   methodCacheSizel  = valueInterfaceContextp->methodCacheSizel;
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

  valueInterfaceContextp->methodCachep     = methodCachep;
  valueInterfaceContextp->methodCacheSizel++;

  localMethodCachep                        = &(methodCachep[methodCacheSizel]);
  localMethodCachep->marpaESLIFClassCachep = &(valueInterfaceContextp->classCache);
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
                                 valueInterfaceContextp->classCache.classp,
                                 methods,
                                 signatures);
  if (methodp == NULL) {
    /* We want OUR exception to be shown */
    (*envp)->ExceptionClear(envp);
    RAISEEXCEPTIONF(envp,
                    "Failed to find method \"%s\" with signature \"%s\" in class \"%s\"",
                    methods,
                    signatures,
                    valueInterfaceContextp->classCache.classs);
  }

  localMethodCachep->methodp = methodp;
  return methodp;

 err:
  return NULL;
}

/*****************************************************************************/
static void marpaESLIFValueInterfaceContextFree(JNIEnv *envp, valueInterfaceContext_t *valueInterfaceContextp, short onStackb)
/*****************************************************************************/
{
  marpaESLIFMethodCache_t *methodCachep;
  size_t                   methodCacheSizel;
  marpaESLIFMethodCache_t *localMethodCachep;
  genericStack_t          *stackp;
  jobject                  globalRef;
  int                      i; 

  if (valueInterfaceContextp != NULL) {
    if (valueInterfaceContextp->classCache.classp != NULL) {
      if (envp != NULL) {
        (*envp)->DeleteGlobalRef(envp, valueInterfaceContextp->classCache.classp);
      }
    }
    if (valueInterfaceContextp->classCache.classs != NULL) {
      free(valueInterfaceContextp->classCache.classs);
    }
    if (valueInterfaceContextp->methodCachep != NULL) {
      methodCachep      = valueInterfaceContextp->methodCachep;
      methodCacheSizel  = valueInterfaceContextp->methodCacheSizel;

      for (i = 0, localMethodCachep = methodCachep; i < (int) methodCacheSizel; i++, localMethodCachep++) {
        if (localMethodCachep->methods != NULL) {
          free(localMethodCachep->methods);
        }
        if (localMethodCachep->signatures != NULL) {
          free(localMethodCachep->signatures);
        }
      }
      free(valueInterfaceContextp->methodCachep);
    }
    stackp = valueInterfaceContextp->stackp;
    if (stackp != NULL) {
      for (i = 0; i < GENERICSTACK_USED(stackp); i++) {
        if (GENERICSTACK_IS_PTR(stackp, i)) {
          globalRef = GENERICSTACK_GET_PTR(stackp, i);
          if (globalRef != NULL) {
            (*envp)->DeleteGlobalRef(envp, globalRef);
          }
        }
      }
      GENERICSTACK_FREE(stackp);
    }
    if (! onStackb) {
      free(valueInterfaceContextp);
    }
  }
}
 
/*****************************************************************************/
static void marpaESLIFRecognizerInterfaceContextCleanup(JNIEnv *envp, recognizerInterfaceContext_t *recognizerInterfaceContextp)
/*****************************************************************************/
{
  if (recognizerInterfaceContextp != NULL) {
    /* Prevent local references exhaustion */
    if ((recognizerInterfaceContextp->previousByteArrayp != NULL) && (recognizerInterfaceContextp->previousDatap != NULL)) {
      (*envp)->ReleaseByteArrayElements(envp, recognizerInterfaceContextp->previousByteArrayp, recognizerInterfaceContextp->previousDatap, JNI_ABORT);
    }
    recognizerInterfaceContextp->previousByteArrayp = NULL;
    recognizerInterfaceContextp->previousDatap      = NULL;

    if ((recognizerInterfaceContextp->previousEncodingp != NULL) && (recognizerInterfaceContextp->previousUTFCharp != NULL)) {
      (*envp)->ReleaseStringUTFChars(envp, recognizerInterfaceContextp->previousEncodingp, recognizerInterfaceContextp->previousUTFCharp);
    }
    recognizerInterfaceContextp->previousEncodingp = NULL;
    recognizerInterfaceContextp->previousUTFCharp  = NULL;
  }
}

/*****************************************************************************/
static void marpaESLIFRecognizerInterfaceContextFree(JNIEnv *envp, recognizerInterfaceContext_t *recognizerInterfaceContextp, short onStackb)
/*****************************************************************************/
{
  if (recognizerInterfaceContextp != NULL) {
    marpaESLIFRecognizerInterfaceContextCleanup(envp, recognizerInterfaceContextp);
    if (! onStackb) {
      free(recognizerInterfaceContextp);
    }
  }
}
