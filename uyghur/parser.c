// parser

#include "others/header.h"

struct Parser
{
    Uyghur *uyghur;
    int position;
    Token *tokens;
    Token *token;
    Leaf *tree;
    Leaf *leaf;
};

void Parser_reset(Parser *this)
{
    this->position = 1;
    this->tokens = NULL;
    this->token = NULL;
    this->tree = NULL;
    this->leaf = NULL;
}

Parser *Parser_new(Uyghur *uyghur)
{
    Parser *parser = malloc(sizeof(Parser));
    parser->uyghur = uyghur;
    return parser;
}

void Parser_error(Parser *this, char *msg)
{
    Token *token = this->token;
    char *m = msg != NULL ? msg : LANG_ERR_PARSER_EXCEPTION;
    char *s = tools_format(LANG_ERR_TOKEN_PLACE, token->file, token->line, token->column, token->value);
    tools_error("Parser: %s, %s", m, s);
}

void Parser_assert(Parser *this, bool value, char *msg)
{
    if (value == true)
        return;
    Parser_error(this, msg);
}

void Parser_moveToken(Parser *this, int indent)
{
    if (indent == 1)
    {
        this->token = this->token->next;
    }
    else if (indent == -1)
    {
        this->token = this->token->last;
    }
    else if (indent == 0)
    {
        //
    }
    else
    {
        tools_error("invalid indent for parser");
    }
}

void Parser_pushLeaf(Parser *this, char tp, int num, Token *token, ...)
{
    Leaf *leaf = Leaf_new(tp);
    va_list valist;
    int i;
    va_start(valist, token);
    for (i = 0; i < num; i++)
    {
        Stack_push(leaf->tokens, token);
        token = va_arg(valist, Token *);
    }
    va_end(valist);
    Leaf_pushLeaf(this->leaf, leaf);
}

void Parser_openBranch(Parser *this)
{
    Leaf *tail = (Leaf *)this->leaf->leafs->tail->data;
    this->leaf = tail;
}

void Parser_closeBranch(Parser *this)
{
    this->leaf = this->leaf->parent;
}

Token *Parser_checkType(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    Parser_assert(this, this->token != NULL, LANG_ERR_INVALID_TYPE);
    bool isMatch = false;
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_equal(this->token->type, s))
        {
            isMatch = true;
            break;
        }
        s = va_arg(valist, char *);
    }
    va_end(valist);
    Parser_assert(this, isMatch, LANG_ERR_INVALID_TYPE);
    return this->token;
}

Token *Parser_checkValue(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    tools_assert(this->token != NULL, LANG_ERR_PARSER_DESIRED_VALUE_NOT_FOUND);
    bool isMatch = false;
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_equal(this->token->value, s))
        {
            isMatch = true;
            break;
        }
        s = va_arg(valist, char *);
    }
    va_end(valist);
    Parser_assert(this, isMatch, NULL);
    return this->token;
}

Token *Parser_getToken(Parser *this, int indent)
{
    Token *token = this->token;
    int count = indent > 0 ? indent : -indent;
    for (size_t i = 0; i < count; i++)
    {
        token = indent > 0 ? token->next : token->last;
    }
    return token;
}

bool Parser_isTypes(Parser *this, int indent, int num, char *s, ...)
{
    Token *token = Parser_getToken(this, indent);
    bool isMatch = false;
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_equal(token->type, s))
        {
            isMatch = true;
            break;
        }
        s = va_arg(valist, char *);
    }
    va_end(valist);
    return isMatch;
}

bool Parser_isType(Parser *this, int indent, char *tp)
{
    Token *token = Parser_getToken(this, indent);
    return token != NULL && is_equal(token->type, tp);
}

bool Parser_isValue(Parser *this, int indent, char *value)
{
    Token *token = Parser_getToken(this, indent);
    return token != NULL && is_equal(token->value, value);
}

bool Parser_isWord(Parser *this, int indent, char *value)
{
    Token *token = Parser_getToken(this, indent);
    return token != NULL && is_equal(token->type, UG_TTYPE_WRD) && is_equal(token->value, value);
}

Token *Parser_checkWord(Parser *this, int indent, int num, char *s, ...)
{
    Parser_moveToken(this, indent);
    Parser_assert(this, this->token != NULL, LANG_ERR_INVALID_WORD);
    Parser_assert(this, is_equal(this->token->type, UG_TTYPE_WRD), LANG_ERR_INVALID_WORD);
    bool isMatch = false;
    va_list valist;
    int i;
    va_start(valist, s);
    for (i = 0; i < num; i++)
    {
        if (is_equal(this->token->value, s))
        {
            isMatch = true;
            break;
        }
        s = va_arg(valist, char *);
    }
    va_end(valist);
    Parser_assert(this, isMatch, LANG_ERR_INVALID_WORD);
    return this->token;
}

// TODO:relace calculation word
void Parser_consumeAstVariable(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_VARIABLE);
    Token *name = Parser_checkType(this, 1, 1, UG_TTYPE_NAM);
    Parser_checkWord(this, 1, 1, TVALUE_INITIAL);
    Token *token = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Parser_checkWord(this, 1, 1, TVALUE_MADE);
    Parser_pushLeaf(this, UG_ATYPE_VAR, 2, name, token);
}

void Parser_consumeAstOperate(Parser *this)
{
    Token *target = Parser_checkWord(this, 0, TTYPES_GROUP_TARGETS);
    Token *name = NULL;
    Token *action = NULL;
    if (is_equal(this->token->value, TVALUE_TARGET_FROM))
    {
        name = Parser_checkType(this, 1, 2, UG_TTYPE_NAM, UG_TTYPE_KEY);
        action = Parser_checkWord(this, 1, 1, TVALUE_INPUT);
    }
    else if (is_equal(this->token->value, TVALUE_TARGET_TO))
    {
        name = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
        action = Parser_checkWord(this, 1, 2, TVALUE_OUTPUT, UG_TTYPE_KEY);
    }
    Parser_pushLeaf(this, UG_ATYPE_OPRT, 3, target, name, action);
}

void Parser_consumeAstConvert(Parser *this)
{
    Token *target = Parser_checkType(this, 0, 2, UG_TTYPE_NAM, UG_TTYPE_KEY);
    Parser_checkWord(this, 1, 1, TVALUE_CONVERT);
    // single
    if (Parser_isWord(this, 2, TVALUE_MADE))
    {
        Token *source = NULL;
        if (Parser_isType(this, 1, UG_TTYPE_WRD))
        {
            source = Parser_checkWord(this, 1, TVAUES_GROUP_CONVERT);
        }
        else
        {
            source = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
        }
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
        Parser_pushLeaf(this, UG_ATYPE_CVT, 2, target, source);
        return;
    }
    Parser_error(this, NULL);
}

void Parser_consumeAstJudge(Parser *this, char aType)
{
    Token *first = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
    if (Parser_isType(this, 1, UG_TTYPE_CLC)) {
        Token *clcltn = Parser_checkType(this, 1, 1, UG_TTYPE_CLC);
        Token *second = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
        Token *action = Parser_checkWord(this, 1, 2, TVALUE_IF_OK, TVALUE_IF_NO);
        Parser_pushLeaf(this, aType, 4, second, clcltn, first, action);
    } else {
        Token *action = Parser_checkWord(this, 1, 2, TVALUE_IF_OK, TVALUE_IF_NO);
        Parser_pushLeaf(this, aType, 2, first, action);
    }
}

void Parser_consumeAstIfFirst(Parser *this)
{
    // open UG_ATYPE_IF
    Parser_pushLeaf(this, UG_ATYPE_IF, 0, NULL);
    Parser_openBranch(this);
    // open UG_ATYPE_IF_F
    Parser_checkWord(this, 0, 1, TVALUE_IF);
    Parser_consumeAstJudge(this, UG_ATYPE_IF_F);
    Parser_openBranch(this);
}

void Parser_consumeAstIfMiddle(Parser *this)
{
    // close UG_ATYPE_IF_F or UG_ATYPE_IF_M
    Token *token = this->token;
    char curType = this->leaf->type;
    Parser_assert(this, curType == UG_ATYPE_IF_F || curType == UG_ATYPE_IF_M, LANG_ERR_PARSER_INVALID_IF);
    Parser_closeBranch(this);
    //  open UG_ATYPE_IF_M
    Parser_checkWord(this, 0, 1, TVALUE_IF_ELSE);
    Parser_consumeAstJudge(this, UG_ATYPE_IF_M);
    Parser_openBranch(this);
}

void Parser_consumeAstIfLast(Parser *this)
{
    // close UG_ATYPE_IF_F or UG_ATYPE_IF_M
    char curType = this->leaf->type;
    Parser_assert(this, curType == UG_ATYPE_IF_F || curType == UG_ATYPE_IF_M, LANG_ERR_PARSER_INVALID_IF);
    Parser_closeBranch(this);
    // open UG_ATYPE_IF_L
    Token *token = Parser_checkWord(this, 0, 1, TVALUE_IF_NO);
    Parser_pushLeaf(this, UG_ATYPE_IF_L, 1, token);
    Parser_openBranch(this);
}

void Parser_consumeAstEnd(Parser *this)
{
    // close UG_ATYPE_IF_F or UG_ATYPE_IF_M or UG_ATYPE_IF_L
    char curType = this->leaf->type;
    if (
        curType == UG_ATYPE_IF_F
        || curType == UG_ATYPE_IF_M
        || curType == UG_ATYPE_IF_L
        || curType == UG_ATYPE_CODE
    ) {
        Parser_closeBranch(this);
    }
    // close ast code
    Parser_checkWord(this, 0, 1, TVALUE_CODE_END);
    Parser_pushLeaf(this, UG_ATYPE_END, 0, NULL);
    Parser_closeBranch(this);
}

void Parser_consumeAstWhile(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_WHILE);
    Parser_consumeAstJudge(this, UG_ATYPE_WHL);
    Parser_openBranch(this);
}

void Parser_consumeAstException(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_EXCEPTION);
    Token *name = Parser_checkType(this, 1, 2, UG_TTYPE_NAM, UG_TTYPE_KEY);
    Parser_checkWord(this, 1, 1, TVALUE_CONTENT);
    Parser_pushLeaf(this, UG_ATYPE_EXC, 1, name);
    Parser_openBranch(this);
}

void Parser_consumeAstResult(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_RESULT);
    Token *name = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
    Parser_checkWord(this, 1, 1, TVALUE_RETURN);
    Parser_pushLeaf(this, UG_ATYPE_RSLT, 1, name);
}

void Parser_consumeAstFunc(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_FUNCTION);
    Token *name = Parser_checkType(this, 1, 2, UG_TTYPE_NAM, UG_TTYPE_KEY);
    Leaf *leaf = Leaf_new(UG_ATYPE_FUN);
    Leaf *code = Leaf_new(UG_ATYPE_CODE);
        // args
    if (Parser_isValue(this, 1, TVALUE_VARIABLE))
    {
        Parser_checkValue(this, 1, 1, TVALUE_VARIABLE);
        Token *variable = Parser_checkType(this, 1, 2, UG_TTYPE_NAM, UG_TTYPE_KEY);
        while (variable != NULL)
        {
            Stack_push(code->tokens, variable);
            variable = Parser_isValue(this, 1, TVALUE_CONTENT) ? NULL : Parser_checkType(this, 1, 2, UG_TTYPE_NAM, UG_TTYPE_KEY);
        }
    }
    // finish
    Parser_checkValue(this, 1, 1, TVALUE_CONTENT);
    Stack_push(leaf->tokens, name);
    Stack_push(code->tokens, name);
    Leaf_pushLeaf(this->leaf, leaf);
    Parser_openBranch(this);
    Leaf_pushLeaf(this->leaf, code);
    Parser_openBranch(this);
}

void Parser_consumeAstCall(Parser *this)
{
    Parser_checkWord(this, 0, 1, TVALUE_FUNCTION);
    Token *name = Parser_checkType(this, 1, 2, UG_TTYPE_NAM, UG_TTYPE_KEY);
    Leaf *leaf = Leaf_new(UG_ATYPE_CALL);
    // args
    if (Parser_isValue(this, 1, TVALUE_WITH))
    {
        Parser_checkValue(this, 1, 1, TVALUE_WITH);
        Token *variable = Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
        while (variable != NULL)
        {
            Stack_push(leaf->tokens, variable);
            variable = Parser_isValue(this, 1, TVALUE_CALL) ? NULL : Parser_checkType(this, 1, TTYPES_GROUP_DEFINE);
        }
    }
    //
    Parser_checkWord(this, 1, 1, TVALUE_CALL);
    // result
    Token *result = Token_empty();
    if (Parser_isValue(this, 1, TVALUE_FURTHER))
    {
        Parser_checkValue(this, 1, 1, TVALUE_FURTHER);
        Parser_checkWord(this, 1, 1, TVALUE_RESULT);
        result = Parser_checkType(this, 1, 2, UG_TTYPE_NAM, UG_TTYPE_KEY);
        Parser_checkWord(this, 1, 1, TVALUE_MADE);
    }
    Stack_push(leaf->tokens, result);
    //
    Stack_push(leaf->tokens, name);
    Leaf_pushLeaf(this->leaf, leaf);
}

void Parser_consumeAstCalculator(Parser *this)
{
    Token *target = Parser_checkType(this, 0, 2, UG_TTYPE_NAM, UG_TTYPE_KEY);
    Parser_checkWord(this, 1, 1, TVALUE_CALCULATOR);
    Token *tempT = NULL;
    Foliage *tempF = NULL;
    Foliage *current = NULL;
    Stack *currents = Stack_new();
    Stack_push(currents, Foliage_new(NULL));
    char *lastType = NULL;
    while (true) {
        current = (Foliage*)currents->tail->data;
        if (Parser_isTypes(this, 1, TTYPES_GROUP_VALUES)) {
            if (lastType == NULL || is_equal(lastType, TVALUE_OPEN)) {
                tempT = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
                tempF = Foliage_new(tempT);
                current->left = tempF;
            } else if (is_calculation_str(lastType)) {
                tempT = Parser_checkType(this, 1, TTYPES_GROUP_VALUES);
                tempF = Foliage_new(tempT);
                current->right = tempF;
            } else {
                break;
            }
            lastType = tempT->type;
            continue;
        } else if (Parser_isType(this, 1, UG_TTYPE_CLC)) {
            Parser_assert(this, is_equal(lastType, TVALUE_CLOSE) || is_values(lastType, TTYPES_GROUP_VALUES), LANG_ERR_PARSER_INVALID_CALCULATOR);
            tempT = Parser_checkType(this, 1, 1, UG_TTYPE_CLC);
            if (current->data != NULL) {
                tempF = Foliage_new(tempT);
                if (is_higher_priority_calculation(tempT->value, ((Token *)current->data)->value)) {
                    tempF->left = current->right;
                    current->right = tempF;
                } else {
                    tempF->left = current;
                    Stack_pop(currents);
                }
                current = tempF;
                Stack_push(currents, tempF);
            }
            current->data = tempT;
            lastType = tempT->value;
            continue;
        } else if (Parser_isWord(this, 1, TVALUE_OPEN)) {
            if (lastType == NULL || is_equal(lastType, TVALUE_OPEN)) {
                Parser_checkWord(this, 1, 1, TVALUE_OPEN);
                tempF = Foliage_new(NULL);
                current->left = tempF;
            } else if (is_calculation_str(lastType)) {
                Parser_checkWord(this, 1, 1, TVALUE_OPEN);
                tempF = Foliage_new(NULL);
                current->right = tempF;
            } else {
                break;
            }
            Stack_push(currents, tempF);
            lastType = TVALUE_OPEN;
            continue;
        } else if (Parser_isWord(this, 1, TVALUE_CLOSE)) {
            Parser_assert(this, is_equal(lastType, TVALUE_CLOSE) || is_values(lastType, TTYPES_GROUP_VALUES), LANG_ERR_PARSER_INVALID_CALCULATOR);
            Parser_checkWord(this, 1, 1, TVALUE_CLOSE);
            Stack_pop(currents);
            lastType = TVALUE_CLOSE;
            continue;
        }
        break;
    }
    current = (Foliage*)currents->head->data;
    Token *body = Token_new(UG_TTYPE_CLC, current);
    Parser_pushLeaf(this, UG_ATYPE_CLC, 2, target, body);
}

void Parser_consumeToken(Parser *this, Token *token)
{
    //
    char *t = token->type;
    char *v = token->value;
    // VARIABLE
    if (is_equal(t, UG_TTYPE_WRD) && is_equal(v, TVALUE_VARIABLE))
    {
        Parser_consumeAstVariable(this);
        return;
    }
    // END
    if (is_equal(v, TVALUE_CODE_END))
    {
        Parser_consumeAstEnd(this);
        return;
    }
    // RESULT
    if (is_equal(v, TVALUE_RESULT))
    {
        Parser_consumeAstResult(this);
        return;
    }
    // FUNC
    if (is_equal(v, TVALUE_FUNCTION) && (Parser_isValue(this, 2, TVALUE_VARIABLE) || Parser_isValue(this, 2, TVALUE_CONTENT)))
    {
        Parser_consumeAstFunc(this);
        return;
    }
    // CALL
    if (is_equal(v, TVALUE_FUNCTION) && (Parser_isValue(this, 2, TVALUE_WITH) || Parser_isValue(this, 2, TVALUE_CALL)))
    {
        Parser_consumeAstCall(this);
        return;
    }
    // IF_FIRST
    if (is_equal(v, TVALUE_IF))
    {
        Parser_consumeAstIfFirst(this);
        return;
    }
    // IF_MIDDLE
    if (is_equal(v, TVALUE_IF_ELSE))
    {
        Parser_consumeAstIfMiddle(this);
        return;
    }
    // IF_LAST
    if (is_equal(v, TVALUE_IF_NO))
    {
        Parser_consumeAstIfLast(this);
        return;
    }
    // WHILE
    if (is_equal(v, TVALUE_WHILE))
    {
        Parser_consumeAstWhile(this);
        return;
    }
    // exception
    if (is_equal(v, TVALUE_EXCEPTION))
    {
        Parser_consumeAstException(this);
        return;
    }
    // EXPRESSION
    if ((is_equal(t, UG_TTYPE_NAM) || is_equal(t, UG_TTYPE_KEY)) && Parser_isWord(this, 1, TVALUE_CONVERT))
    {
        Parser_consumeAstConvert(this);
        return;
    }
    // calculate
    if ((is_equal(t, UG_TTYPE_NAM) || is_equal(t, UG_TTYPE_KEY)) && Parser_isWord(this, 1, TVALUE_CALCULATOR))
    {
        Parser_consumeAstCalculator(this);
        return;
    }
    // OPERATE
    if (is_equal(t, UG_TTYPE_WRD) && (is_equal(v, TVALUE_TARGET_FROM) || is_equal(v, TVALUE_TARGET_TO)))
    {
        Parser_consumeAstOperate(this);
        return;
    }
    //
    Parser_error(this, NULL);
    Token_print(token);
}

Leaf *Parser_parseTokens(Parser *this, Token *tokens)
{
    Parser_reset(this);
    this->tokens = tokens;
    this->tree = Leaf_new(UG_ATYPE_PRG);
    this->leaf = this->tree;
    //
    this->token = this->tokens;
    while (this->token != NULL)
    {
        Parser_consumeToken(this, this->token);
        this->token = this->token->next;
    }
    return this->tree;
}

void Parser_free(Parser *this)
{
    this->uyghur = NULL;
    Parser_reset(this);
    free(this);
}
