// constant

#define UG_VERSION_NAME "0.1" 
#define UG_VERSION_CODE 0.1 

// token types
#define TTYPE_NAME "TTYPE_NAME"
#define TTYPE_KEY "TTYPE_KEY"
#define TTYPE_WORD "TTYPE_WORD"
#define TTYPE_STRING "TTYPE_STRING"
#define TTYPE_NUMBER "TTYPE_NUMBER"
#define TTYPE_BOOL "TTYPE_BOOL"
#define TTYPE_EMPTY "TTYPE_EMPTY"
#define TTYPE_BOX "TTYPE_BOX"
#define TTYPE_SCOPE "TTYPE_SCOPE"

// token value of keywords
//
#define TVALUE_NUM "san"
#define TVALUE_STR "xet"
#define TVALUE_EMPTY "quruq"
#define TVALUE_BOX "sanduq"
#define TVALUE_BOOLEAN "logika"
//
#define TVALUE_WHILE "nawada"
#define TVALUE_IF "eger"
#define TVALUE_IF_ELSE "egerde"
#define TVALUE_IF_OK "bolsa"
#define TVALUE_IF_NO "bolmisa"
#define TVALUE_CODE_END "tamamlansun"
// 
#define TVALUE_OUTPUT "yezilsun"
#define TVALUE_INPUT "oqulsun"
// 
#define TVALUE_DOT "."
#define TVALUE_AT "@"
//
#define TVALUE_TRUE "rast"
#define TVALUE_FALSE "yalghan"
//
#define TVALUE_AND "hemde"
#define TVALUE_OR "yaki"
#define TVALUE_NOT "ekische"
//
#define TVALUE_ADD "qushulghan"
#define TVALUE_SUB "elinghan"
#define TVALUE_MUL "kupeytilgen"
#define TVALUE_DIV "bulungen"
#define TVALUE_LESS "kichik"
#define TVALUE_MORE "chong"
// 
#define TVALUE_CONCAT "ulanghan"
#define TVALUE_EQUAL "teng"
//
#define TVALUE_TARGET_FROM "ikrandin" // TODO implement with standart library -> box+func
#define TVALUE_TARGET_TO "ikrangha" // TODO implement with standart library -> box+func (screen, file, etc)
//
#define TVALUE_VARIABLE "miqdar"
#define TVALUE_CREATE "tewellut"
#define TVALUE_FREE "azad"
#define TVALUE_REMOVE "wapat"
#define TVALUE_VALUE "qimmiti"
#define TVALUE_MADE "bolsun"

#define TVALUE_FUNCTION "fonkisiye"
#define TVALUE_CONTENT "mezmuni"
#define TVALUE_WITH "bilen"
#define TVALUE_CALL "ishlitilsun"
#define TVALUE_RETURN "qayturulsun"
#define TVALUE_FURTHER "we"
#define TVALUE_RESULT "netije"

// ast type
#define ASTTYPE_PROGRAM "ASTTYPE_PROGRAM"
#define ASTTYPE_END "ASTTYPE_END"
#define ASTTYPE_VARIABLE "ASTTYPE_VARIABLE"
#define ASTTYPE_ASSIGN "ASTTYPE_ASSIGN"
#define ASTTYPE_TRANSFORM "ASTTYPE_TRANSFORM"
#define ASTTYPE_RESULT "ASTTYPE_RESULT"
#define ASTTYPE_FUNC "ASTTYPE_FUNC"
#define ASTTYPE_CODE "ASTTYPE_CODE"
#define ASTTYPE_CALL "ASTTYPE_CALL"
#define ASTTYPE_IF "ASTTYPE_IF"
#define ASTTYPE_IF_FIRST "ASTTYPE_IF_FIRST"
#define ASTTYPE_IF_MIDDLE "ASTTYPE_IF_MIDDLE"
#define ASTTYPE_IF_LAST "ASTTYPE_IF_LAST"
#define ASTTYPE_WHILE "ASTTYPE_WHILE"
#define ASTTYPE_EXPRESSION_SINGLE "ASTTYPE_EXPRESSION_SINGLE"
#define ASTTYPE_EXPRESSION_DOUBLE "ASTTYPE_EXPRESSION_DOUBLE" // TODO complex
#define ASTTYPE_OPERATE "ASTTYPE_OPERATE"

#define TTYPES_GROUP_DEFINE 7, TTYPE_NAME, TTYPE_KEY, TTYPE_STRING, TTYPE_NUMBER, TTYPE_BOOL, TTYPE_EMPTY, TTYPE_BOX
#define TTYPES_GROUP_STRING 3, TTYPE_NAME, TTYPE_KEY, TTYPE_STRING
#define TTYPES_GROUP_NUMBER 3, TTYPE_NAME, TTYPE_KEY, TTYPE_NUMBER
#define TTYPES_GROUP_LOGICS 4, TTYPE_NAME, TTYPE_KEY, TTYPE_BOOL, TTYPE_EMPTY
#define TTYPES_GROUP_KEYS 3, TTYPE_NAME, TTYPE_STRING, TTYPE_NUMBER

#define TTYPES_GROUP_TARGETS 2, TVALUE_TARGET_FROM, TVALUE_TARGET_TO

#define TVALUE_GROUP_EXP_STRING 2, TVALUE_EQUAL, TVALUE_CONCAT
#define TVALUE_GROUP_EXP_NUMBER 7, TVALUE_EQUAL, TVALUE_ADD, TVALUE_SUB, TVALUE_MUL, TVALUE_DIV, TVALUE_LESS, TVALUE_MORE
#define TVALUE_GROUP_EXP_BOOL 3, TVALUE_EQUAL, TVALUE_AND, TVALUE_OR
#define TVALUE_GROUP_EXP_ALL 10, TVALUE_EQUAL, TVALUE_CONCAT, TVALUE_ADD, TVALUE_SUB, TVALUE_MUL, TVALUE_DIV, TVALUE_LESS, TVALUE_MORE, TVALUE_AND, TVALUE_OR
#define TVALUE_GROUP_EXP_BOX 1, TVALUE_EQUAL

#define TVAUE_GROUP_EXP_SINGLE 6, TVALUE_NUM, TVALUE_STR, TVALUE_EMPTY, TVALUE_BOOLEAN, TVALUE_FUNCTION, TVALUE_NOT

// runtime types
#define RTYPE_UNKNOWN "RTYPE_UNKNOWN"
#define RTYPE_NUMBER "RTYPE_NUMBER"
#define RTYPE_STRING "RTYPE_STRING"
#define RTYPE_EMPTY "RTYPE_EMPTY"
#define RTYPE_BOX "RTYPE_BOX"
#define RTYPE_BOOLEAN "RTYPE_BOOLEAN"
#define RTYPE_FUNCTION "RTYPE_FUNCTION"
#define RTYPE_CFUNCTION "RTYPE_CFUNCTION"

#define RTYPE_GROUP_BASE 3, RTYPE_STRING, RTYPE_NUMBER, RTYPE_BOOLEAN

#define CACHE_STRING_MAX_LENGTH 128
