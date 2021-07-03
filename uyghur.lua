--[[
    main compiler program
]]

-- https://github.com/kompasim/pure-lua-tools.git
require "pure-lua-tools.initialize"

local TOKEN_TYPE = {
    NAME = "NAME", -- variable
    STRING = "STRING", -- string
    NUMBER = "NUMBER", -- number
    BOOL = "BOOL", -- value
    EMPTY = "EMPTY", -- value
    --
    CODE_START = "CODE_START",
    CODE_END = "CODE_END",
    -- 
    VARIABLE = "VARIABLE",
    VALUE = "VALUE",
    MADE = "MADE",
    --
    RESULT = "RESULT",
    RETURN = "RETURN",
    --
    FUNC = "FUNC",
    WITH = "WITH",
    CALL = "CALL",
    FURTHER = "FURTHER",
    -- 
    IF = "IF",
    WHILE = "WHILE",
    --
    TARGET = "TARGET",
    HANDLE = "HANDLE",
    LOGIC = "LOGIC", -- logic
    NOT = "NOT", -- not
    -- 
    OPERATION_NUM = "OPERATION_NUM", -- operation
    OPERATION_STR = "OPERATION_STR", -- operation
}

local TOKEN_TYPES_VALUES = {TOKEN_TYPE.NAME, TOKEN_TYPE.STRING, TOKEN_TYPE.NUMBER, TOKEN_TYPE.BOOL, TOKEN_TYPE.EMPTY}
local TOKEN_TYPES_STRING = {TOKEN_TYPE.NAME, TOKEN_TYPE.STRING}
local TOKEN_TYPES_NUMBER = {TOKEN_TYPE.NAME, TOKEN_TYPE.NUMBER}
local TOKEN_TYPES_LOGICS = {TOKEN_TYPE.NAME, TOKEN_TYPE.BOOL, TOKEN_TYPE.EMPTY}

local TOKEN_TYPE_MAP = {
    -- block
    bolsa = TOKEN_TYPE.CODE_START,
    tamamlansun = TOKEN_TYPE.CODE_END,
    -- variable
    mixtar = TOKEN_TYPE.VARIABLE,
    qimmiti = TOKEN_TYPE.VALUE,
    bolsun = TOKEN_TYPE.MADE,
    -- result retrun
    netije = TOKEN_TYPE.RESULT,
    qayturulsun = TOKEN_TYPE.RETURN,
    -- function
    fonkisiye = TOKEN_TYPE.FUNC,
    bilen = TOKEN_TYPE.WITH,
    ishlitilsun = TOKEN_TYPE.CALL,
    we = TOKEN_TYPE.FURTHER,
    -- if
    eger = TOKEN_TYPE.IF,
    -- while
    nawada = TOKEN_TYPE.WHILE,
    -- io
    ikrangha = TOKEN_TYPE.TARGET,
    ikrandin = TOKEN_TYPE.TARGET,
    yezilsun = TOKEN_TYPE.HANDLE,
    oqulsun = TOKEN_TYPE.HANDLE,
    -- types
    quruq = TOKEN_TYPE.EMPTY,
    -- bool
    rast = TOKEN_TYPE.BOOL,
    yalghan = TOKEN_TYPE.BOOL,
    -- logic
    xemde = TOKEN_TYPE.LOGIC,
    yaki = TOKEN_TYPE.LOGIC,
    ekische = TOKEN_TYPE.NOT,
    -- operation
    qushulghan = TOKEN_TYPE.OPERATION_NUM,
    elinghan = TOKEN_TYPE.OPERATION_NUM,
    kupeytilgen = TOKEN_TYPE.OPERATION_NUM,
    bulungen = TOKEN_TYPE.OPERATION_NUM,
    --
    kichik = TOKEN_TYPE.OPERATION_NUM,
    chong = TOKEN_TYPE.OPERATION_NUM,
    -- operation
    ulanghan = TOKEN_TYPE.OPERATION_STR,
    teng = TOKEN_TYPE.OPERATION_STR,
}

local SIGNS = {
    LINE = "LINE",
    EMPTY = "EMPTY",
    COMMENT = "COMMENT",
    NUMBER = "NUMBER",
    LETTER = "LETTER",
    OPEN = "OPEN",
    CLOSE = "CLOSE",
    OTHER = "OTHER",
}

local ORDER = {
    SIGNS.LINE,
    SIGNS.EMPTY,
    SIGNS.COMMENT,
    SIGNS.NUMBER,
    SIGNS.LETTER,
    SIGNS.OPEN,
    SIGNS.CLOSE,
    SIGNS.OTHER,
}

local EXPRESSIONS = {
    [SIGNS.LINE] = "\n",
    [SIGNS.EMPTY] = "%s",
    [SIGNS.COMMENT] = "%#",
    [SIGNS.NUMBER] = "%d",
    [SIGNS.LETTER] = "%a",
    [SIGNS.OPEN] = "%[",
    [SIGNS.CLOSE] = "%]",
    [SIGNS.OTHER] = ".",
}

local STATE = {
    NEW = "NEW",
    COMMENT = "COMMENT",
    NUMBER = "NUMBER",
    LETTER = "LETTER",
    OPEN = "OPEN",
    STRING = "STRING",
    CLOSE = "CLOSE",
    END = "END",
    ERROR = "ERROR",
}

local TOKENIZER_STATE_MAP = {
    [STATE.NEW] = {
        [SIGNS.LINE] = STATE.NEW,
        [SIGNS.EMPTY] = STATE.NEW,
        [SIGNS.COMMENT] = STATE.COMMENT,
        [SIGNS.NUMBER] = STATE.NUMBER,
        [SIGNS.LETTER] = STATE.LETTER,
        [SIGNS.OPEN] = STATE.OPEN,
        [SIGNS.CLOSE] = STATE.ERROR,
        [SIGNS.OTHER] = STATE.ERROR,

    },
    [STATE.COMMENT] = {
        [SIGNS.LINE] = STATE.NEW,
        [SIGNS.EMPTY] = STATE.COMMENT,
        [SIGNS.COMMENT] = STATE.COMMENT,
        [SIGNS.NUMBER] = STATE.COMMENT,
        [SIGNS.LETTER] = STATE.COMMENT,
        [SIGNS.OPEN] = STATE.COMMENT,
        [SIGNS.CLOSE] = STATE.COMMENT,
        [SIGNS.OTHER] = STATE.COMMENT,

    },
    [STATE.NUMBER] = {
        [SIGNS.LINE] = STATE.END,
        [SIGNS.EMPTY] = STATE.END,
        [SIGNS.NUMBER] = STATE.NUMBER,
    },
    [STATE.LETTER] = {
        [SIGNS.LINE] = STATE.END,
        [SIGNS.EMPTY] = STATE.END,
        [SIGNS.LETTER] = STATE.LETTER,
    },
    [STATE.OPEN] = {
        [SIGNS.LINE] = STATE.ERROR,
        [SIGNS.EMPTY] = STATE.STRING,
        [SIGNS.COMMENT] = STATE.STRING,
        [SIGNS.NUMBER] = STATE.STRING,
        [SIGNS.LETTER] = STATE.STRING,
        [SIGNS.OPEN] = STATE.ERROR,
        [SIGNS.CLOSE] = STATE.CLOSE,
        [SIGNS.OTHER] = STATE.STRING,
    },
    [STATE.STRING] = {
        [SIGNS.LINE] = STATE.ERROR,
        [SIGNS.EMPTY] = STATE.STRING,
        [SIGNS.COMMENT] = STATE.STRING,
        [SIGNS.NUMBER] = STATE.STRING,
        [SIGNS.LETTER] = STATE.STRING,
        [SIGNS.OPEN] = STATE.ERROR,
        [SIGNS.CLOSE] = STATE.CLOSE,
        [SIGNS.OTHER] = STATE.STRING,
    },
    [STATE.CLOSE] = {
        [SIGNS.LINE] = STATE.NEW,
        [SIGNS.EMPTY] = STATE.NEW,
    },
}

local AST_TYPE = {
    AST_TOKEN = "AST_TOKEN",
    AST_PROGRAM = "AST_PROGRAM",
    AST_END = "AST_END",
    AST_VARIABLE = "AST_VARIABLE",
    AST_RESULT = "AST_RESULT",
    AST_FUNC = "AST_FUNC",
    AST_CALL = "AST_CALL",
    AST_IF = "AST_IF",
    AST_WHILE = "AST_WHILE",
    AST_EXPRESSION = "AST_EXPRESSION",
    AST_EXPRESSION_LOGIC = "AST_EXPRESSION_LOGIC",
    AST_OPERATE = "AST_OPERATE",
}

local PARSER_STATE_MAP = {
    [TOKEN_TYPE.CODE_END] = AST_TYPE.AST_END,
    [TOKEN_TYPE.VARIABLE] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.VALUE] = {
                [TOKEN_TYPE.NAME] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
                [TOKEN_TYPE.STRING] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
                [TOKEN_TYPE.NUMBER] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
                [TOKEN_TYPE.BOOL] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
                [TOKEN_TYPE.EMPTY] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_VARIABLE,
                },
            }
        }
    },
    [TOKEN_TYPE.RESULT] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
        [TOKEN_TYPE.STRING] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
        [TOKEN_TYPE.NUMBER] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
        [TOKEN_TYPE.BOOL] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
        [TOKEN_TYPE.EMPTY] = {
            [TOKEN_TYPE.RETURN] = AST_TYPE.AST_RESULT,
        },
    },
    [TOKEN_TYPE.FUNC] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_FUNC,
            [TOKEN_TYPE.VARIABLE] = AST_TYPE.AST_FUNC,
            [TOKEN_TYPE.CALL] = AST_TYPE.AST_CALL,
            [TOKEN_TYPE.WITH] = AST_TYPE.AST_CALL,
        },
    },
    [TOKEN_TYPE.IF] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.VALUE] = {
                [TOKEN_TYPE.BOOL] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_IF,
                }
            }
        }
    },
    [TOKEN_TYPE.WHILE] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.VALUE] = {
                [TOKEN_TYPE.BOOL] = {
                    [TOKEN_TYPE.CODE_START] = AST_TYPE.AST_WHILE,
                }
            }
        }
    },
    [TOKEN_TYPE.NAME] = {
        [TOKEN_TYPE.VALUE] = {
            [TOKEN_TYPE.NAME] = {
                [TOKEN_TYPE.OPERATION_NUM] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.NUMBER] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                },
                [TOKEN_TYPE.OPERATION_STR] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.STRING] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.NUMBER] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                },
                [TOKEN_TYPE.LOGIC] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                    [ TOKEN_TYPE.BOOL] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                },
                [ TOKEN_TYPE.NOT] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                },
            },
            [TOKEN_TYPE.STRING] = {
                [TOKEN_TYPE.OPERATION_STR] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.STRING] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                },
            },
            [TOKEN_TYPE.NUMBER] = {
                [TOKEN_TYPE.OPERATION_NUM] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.NUMBER] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                },
                [TOKEN_TYPE.OPERATION_STR] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.STRING] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                    [ TOKEN_TYPE.NUMBER] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION,
                    },
                },
            },
            [TOKEN_TYPE.BOOL] = {
                [ TOKEN_TYPE.LOGIC] = {
                    [ TOKEN_TYPE.NAME] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                    [ TOKEN_TYPE.BOOL] = {
                        [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                    },
                },
                [ TOKEN_TYPE.NOT] = {
                    [TOKEN_TYPE.MADE] = AST_TYPE.AST_EXPRESSION_LOGIC,
                },
            },
        },
    },
    [TOKEN_TYPE.TARGET] = {
        [TOKEN_TYPE.NAME] = {
            [TOKEN_TYPE.HANDLE] = AST_TYPE.AST_OPERATE,
        },
        [TOKEN_TYPE.STRING] = {
            [TOKEN_TYPE.HANDLE] = AST_TYPE.AST_OPERATE,
        },
        [TOKEN_TYPE.NUMBER] = {
            [TOKEN_TYPE.HANDLE] = AST_TYPE.AST_OPERATE,
        },
        [TOKEN_TYPE.BOOL] = {
            [TOKEN_TYPE.HANDLE] = AST_TYPE.AST_OPERATE,
        },
        [TOKEN_TYPE.EMPTY] = {
            [TOKEN_TYPE.HANDLE] = AST_TYPE.AST_OPERATE,
        },
    },
}

local function str(value)
    local res = ""
    if type(value) ~= "table" then
        res = tostring(value)
    elseif #value == table.count(value) then
        for i,v in ipairs(value) do
            res = res .. v
            if i ~= #value then
                res = res .. ","
            end
        end
    else
        res = res .. "{"
        for k,v in pairs(value) do
            res = res .. k .. "=" .. tostring(v) .. ","
        end
        res = res .. "}"
    end
    return res
end

-- tokenizer
local tokenizer = {}

function tokenizer:tokenize(input, line, path)
    self.input = input .. " "
    self.line = line or 1
    self.path = path or "ikran"
    self.length = #self.input
    self.tokens = {}
    self.index = 0
    self.column = 0
    self.state = STATE.NEW
    self.value = ""
    for i=1,self.length do
        self:process(i)
    end
    return self.tokens
end

function tokenizer:process(i)
    --
    self.index = i
    self.column = self.column + 1
    -- get sign type
    local char = string.sub(self.input, self.index, self.index)
    local signType = nil
    for _,tp in pairs(ORDER) do
        local exp = EXPRESSIONS[tp]
        if not signType then
            if string.match(char, exp) then
                signType = tp
            end
        end
    end
    -- invalid sign
    if not signType
    or (signType == SIGNS.OTHER and self.state ~= STATE.COMMENT) and (signType == SIGNS.OTHER and self.state ~= STATE.STRING) then
        self:assert(false, "nime bu yazghining, taza bilelmidimghu")
    end
    -- get new state
    local machineMap = TOKENIZER_STATE_MAP[self.state]
    self:assert(machineMap ~= nil, string.format("xalet nami [%s] inawatsiz", self.state))
    local newState = machineMap[signType]
    self:assert(newState ~= nil, string.format("belge tipi [%s] inawatsiz", signType))
    local lastState = self.state
    self.state = newState
    self:assert(self.state ~= STATE.ERROR, string.format('xata xalet kuruldi'))
    -- handle current info
    if self.state == STATE.NEW then
        -- ignore
    elseif self.state == STATE.COMMENT then
        --ignore
    elseif self.state == STATE.NUMBER then
        self.value = self.value .. char
    elseif self.state == STATE.LETTER then
        self.value = self.value .. char
    elseif self.state == STATE.OPEN then
        --ignore
    elseif self.state == STATE.STRING then
        self.value = self.value .. char
    elseif self.state == STATE.CLOSE then
        self:save(lastState)
    elseif self.state == STATE.END then
        self:save(lastState)
    else
        assert(false, string.format("xalet [%s] inawetsiz", self.state))
    end
end

function tokenizer:save(lastState)
    local token = nil
    local value = self.value
    if lastState == STATE.NUMBER then
        token = self:token(TOKEN_TYPE.NUMBER, value)
    elseif lastState == STATE.LETTER then
        if TOKEN_TYPE_MAP[value] then
            token = self:token(TOKEN_TYPE_MAP[value], value)
        else
            token = self:token(TOKEN_TYPE.NAME, value)
        end
    elseif lastState == STATE.STRING then
        token = self:token(TOKEN_TYPE.STRING, value)
    elseif lastState == STATE.OPEN then
        token = self:token(TOKEN_TYPE.STRING, value) -- empty string
    end
    assert(token ~= nil)
    table.insert(self.tokens, token)
    self.value = ""
    self.state = STATE.NEW
end

function tokenizer:token(tp, value)
    return {
        isToken = true,
        type = tp,
        value = value,
        path = self.path,
        line = self.line,
        column = self.column,
    }
end

function tokenizer:assert(v, msg)
    local char = string.sub(self.input, self.index, self.index)
    assert(v == true, string.format("%s: xojjet:[%s], qur:[%d], qatar:[%d], belge:[%s]", msg, self.path, self.line, self.column, char))
end


-- parser
local parser = {}

function parser:init()
    self.tree = self:node(AST_TYPE.AST_PROGRAM)
    self.current = {self.tree}
end

function parser:parse(tokens, line, path)
    self.line = line or 1
    self.path = path or "ikran"
    self.tokens = tokens
    self.length = #self.tokens
    self.index = 1
    if #tokens == 0 then
        return
    end
    if not self.tree then
        self:init()
    end
    self:consume()
    return self.tree
end

function parser:next(tp)
    self.index = self.index + 1
    if tp then self:check(tp) end
    return self.tokens[self.index]
end

function parser:last(tp)
    self.index = self.index - 1
    if tp then self:check(tp) end
    return self.tokens[self.index]
end

function parser:expect(tp)
    if tp then self:check(tp) end
    return self.tokens[self.index]
end

function parser:check(tp)
    local token = self.tokens[self.index]
    if not token then
        self:assert(false, string.format("keynidin sanliq melumat tipi [%s] umut qilindi emma tepilmidi", tp), self:last())
    end
    tp = type(tp) == "string" and {tp} or tp
    local is = self:is(tp)
    self:assert(is, string.format("sanliq melumat tipi [%s] umut qilindi emma tepilmidi", str(tp)))
end

function parser:is(tp)
    local token = self.tokens[self.index]
    if not token then
        return false, true
    end
    tp = type(tp) == "string" and {tp} or tp
    local isValid = false
    for i,v in ipairs(tp) do
        if v == token.type then
            isValid = true
        end
    end
    return isValid, false
end

function parser:consume()
    -- ast type
    local index = self.index
    local token = self.tokens[index]
    local tokenType = token.type
    local nextState = PARSER_STATE_MAP[tokenType]
    while type(nextState) == "table" do
        index = index + 1
        token = self.tokens[index]
        if token then
            tokenType = token.type
            nextState = nextState[tokenType]
        else
            tokenType = nil
            nextState = nil
        end
    end
    -- invalid
    if not tokenType then
        self:assert(false, "inawetsiz qur", self.tokens[1])
    elseif not nextState then
        self:assert(false, "inawetsiz sozluk", self.tokens[index])
    end
    -- consume
    local astType = nextState
    local astFunc = self["consume_" .. astType]
    assert(astFunc ~= nil, string.format("consume func for [%s] is unimplemented", astType))
    local astNode = astFunc(self)
    self:insert(astType, astNode)
    -- surplus
    if self.index ~= self.length then
        self:assert(false, string.format("artuxche sozluk", self:next()))
    end
end

function parser:consume_AST_VARIABLE()
    self:expect(TOKEN_TYPE.VARIABLE)
    local name = self:next(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg = self:next(TOKEN_TYPES_VALUES)
    self:next(TOKEN_TYPE.MADE)
    return {name, arg}
end

function parser:consume_AST_EXPRESSION()
    local name = self:expect(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg1 = self:next(TOKEN_TYPES_VALUES)
    local exp = self:next({TOKEN_TYPE.OPERATION_NUM, TOKEN_TYPE.OPERATION_STR})
    local arg2 = self:next(TOKEN_TYPES_VALUES)
    self:next(TOKEN_TYPE.MADE)
    return {name, arg1, exp, arg2}
end

function parser:consume_AST_EXPRESSION_LOGIC()
    local name = self:expect(TOKEN_TYPE.NAME)
    self:next(TOKEN_TYPE.VALUE)
    local arg1 = self:next(TOKEN_TYPES_LOGICS)
    local exp = self:next({TOKEN_TYPE.LOGIC, TOKEN_TYPE.NOT})
    local arg2 = exp.type == TOKEN_TYPE.LOGIC and self:next(TOKEN_TYPES_LOGICS) or nil
    self:next(TOKEN_TYPE.MADE)
    return {name, arg1, exp, arg2}
end

function parser:consume_AST_OPERATE()
    local target = self:expect(TOKEN_TYPE.TARGET)
    local arg = self:next(TOKEN_TYPES_VALUES)
    local handle = self:next({TOKEN_TYPE.HANDLE})
    return {target, arg, handle}
end

function parser:consume_AST_IF()
    self:expect(TOKEN_TYPE.IF)
    local arg1 = self:next(TOKEN_TYPES_LOGICS)
    self:next(TOKEN_TYPE.VALUE)
    local arg2 = self:next(TOKEN_TYPES_LOGICS)
    self:next({TOKEN_TYPE.CODE_START})
    return {arg1, arg2}
end

function parser:consume_AST_WHILE()
    self:expect(TOKEN_TYPE.WHILE)
    local arg1 = self:next(TOKEN_TYPES_LOGICS)
    self:next(TOKEN_TYPE.VALUE)
    local arg2 = self:next(TOKEN_TYPES_LOGICS)
    self:next({TOKEN_TYPE.CODE_START})
    return {arg1, arg2}
end

function parser:consume_AST_RESULT()
    self:expect(TOKEN_TYPE.RESULT)
    local arg1 = self:next(TOKEN_TYPES_VALUES)
    self:next(TOKEN_TYPE.RETURN)
    return {arg1}
end

function parser:consume_AST_FUNC()
    self:expect(TOKEN_TYPE.FUNC)
    local funcName = self:next(TOKEN_TYPE.NAME)
    local tokens = {}
    table.insert(tokens, funcName)
    --
    local next = self:next()
    if next.type == TOKEN_TYPE.VARIABLE then
        next = self:next()
    end
    while next.type == TOKEN_TYPE.NAME do
        table.insert(tokens, next)
        next = self:next()
    end
    self:check(TOKEN_TYPE.CODE_START)
    return tokens
end

function parser:consume_AST_CALL()
    self:expect(TOKEN_TYPE.FUNC)
    local funcName = self:next(TOKEN_TYPE.NAME)
    local tokens = {}
    table.insert(tokens, funcName)
    --
    local next = self:next()
    if next.type == TOKEN_TYPE.WITH then
        next = self:next()
    end
    while self:is(TOKEN_TYPES_VALUES) do
        table.insert(tokens, next)
        next = self:next()
    end
    self:check(TOKEN_TYPE.CALL)
    next = self:next()
    if not next then
        self:last()
    else
        self:check(TOKEN_TYPE.FURTHER)
        self:next(TOKEN_TYPE.RESULT)
        local name = self:next(TOKEN_TYPE.NAME)
        self:next(TOKEN_TYPE.MADE)
        table.insert(tokens, name)
    end
    return tokens
end

function parser:consume_AST_END()
    self:expect(TOKEN_TYPE.CODE_END)
end

function parser:node(name, ...)
    return {
        name = name,
        children = {},
        tokens = {...},
    }
end

function parser:insert(name, ...)
    local node = self:node(name, ...)
    local current = self.current[#self.current]
    table.insert(current.children, node)
    --
    local lastToken = self:expect()
    assert(lastToken ~= nil)
    local lastType = lastToken.type
    if lastType == TOKEN_TYPE.CODE_START then
        table.insert(self.current, node)
    elseif lastType == TOKEN_TYPE.CODE_END then
        table.remove(self.current, #self.current)
    end
    --
    self:assert(#self.current > 0, "asasiy programmini tamamlash inawetsiz")
    return node
end

function parser:assert(v, msg, token)
    token = token or self.tokens[self.index]
    assert(v == true, string.format("%s: xojjet:[%s], qur:[%d], soz:[%s]", msg, self.path, self.line, token.value))
end

local function runLine(line, lineNo, fileName)
    local tokens = tokenizer:tokenize(line, lineNo, fileName)
    local tree = parser:parse(tokens, lineNo, fileName)
    if not tree then return end -- empty lines

    -- TODO:
    -- print("=====================================================================START")
    -- print("tokens:", tokens)
    -- print("tree:", tree)
    -- if tree then
    --     table.print(tree)
    -- end
    -- print("=======================================================================END")

    -- local newAst = transform(oldAst)
    -- local output = generate(newAst)
end

local function runFile(path)
    local lines = {}
    local file = io.open(path, "r")
    assert(file ~= nil, string.format("bu hojjet tepilmidi:[%s]", path))
    io.input(file)
    for line in io.lines() do
        table.insert(lines, line)
    end
    io.close(file)
    for lineNo,line in ipairs(lines) do
        runLine(line, lineNo, path)
    end
end

local function runInput()
    print("\n UyghurScript:\n")
    local status = 0
    while status == 0 do
        io.write("ug > ")
        local isOK, err = xpcall(function()
            local line = io.read()
            if not line then
                status = -1
            elseif line == "tamam" then
                status = -2
            elseif line then
                runLine(line, nil, nil)
            end
        end, function(err)
            status = -3
            print("\nHataliq:", debug.traceback(err), "\n")
        end)
        if status == -1 then
            print("\n\n Tamam!\n")
        elseif status == -2 then
            print("\n Tamam!\n")
        end
    end
end

local function entry()
    if arg[1] then
        runFile(arg[1])
    else
        runInput()
    end
end

entry()
