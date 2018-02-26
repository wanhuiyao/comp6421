#!/usr/bin/python
import sys, json, copy


class Parser:
    def __init__(self, lex_file):
        self.terminal_symbols_map = {
                "class":                                 1,
                "id":                                    2,
                "{":                                     3,
                "}":                                     4,
                ";":                                     5,
                "(":                                     6,
                ")":                                     7,
                "program":                               8,
                "for":                                   9,
                "+":                                     10,
                "-":                                     11,
                "[":                                     12,
                "]":                                     13,
                "intNum":                                14,
                "float":                                 15,
                "int":                                   16,
                ",":                                     17,
                "=":                                     18,
                "lt":                                    19,
                "leq":                                   20,
                "neq":                                   21,
                "eq":                                    22,
                "gt":                                    23,
                "geq":                                   24,
                "or":                                    25,
                "*":                                     26,
                "/":                                     27,
                "and":                                   28,
                "$":                                     29
                }            

        self.non_terminal_symbols_map = {
                "prog":                                  1,
                "N_classDecl":                           2,
                "classDecl":                             3,
                "RvarDeclfuncDef":                       4,
                "varDeclfuncDef":                        5,
                "varDeclNoTypeIDorfuncDefNoTypeID":      6,
                "varDeclNoTypeID":                       7,
                "funcDefNoTypeID":                       8,
                "N_varDecl":                             9,
                "N_funcDef":                             10,
                "progBody":                              11,
                "funcHead":                              12,
                "funcDef":                               13,
                "funcBody":                              14,
                "N_statement":                           15,
                "varDecl":                               16,
                "N_arraySize":                           17,
                "statement":                             18,
                "assignStat":                            19,
                "statBlock":                             20,
                "expr":                                  21,
                "COMM":                                  22,
                "relExpr":                               23,
                "arithExpr":                             24,
                "arithExprLR":                           25,
                "sign":                                  26,
                "term":                                  27,
                "termLR":                                28,
                "factor":                                29,
                "N_idnest_id":                           30,
                "variable":                              31,
                "N_idnest":                              32,
                "idnest":                                33,
                "N_indice":                              34,
                "indice":                                35,
                "arraySize":                             36,
                "type":                                  37,
                "fParams":                               38,
                "N_fParamsTail":                         39,
                "aParams":                               40,
                "N_aParamsTail":                         41,
                "fParamsTail":                           42,
                "aParamsTail":                           43,
                "assignOp":                              44,
                "relOp":                                 45,
                "addOp":                                 46,
                "multiOp":                               47,
                }

        self.parse_table = [
                # 0,  "class",  "id",  "{",  "}",  ";",  "(",  ")",  "program",  "for",  "+",  "-",  "[",  "]",  "intNum",  "float",  "int",  ",",   "=",  "lt",  "leq",  "neq",  "eq",  "gt",  "geq",  "or",  "*",  "/",  "and",  "$"
                [ 0,  0,        0,     0,    0,    0,    0,    0,    0,          0,      0,    0,    0,    0,    0,          0,        0,      0,     0,    0,     0,      0,      0,     0,     0,      0,     0,    0,    0,      0  ],
                [ 0,  1,        71,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     70 ],
                [ 0,  2,        71,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     3  ],
                [ 0,  4,        71,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     70 ],
                [ 0,  71,       5,     71,   6,    71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         5,        5,      71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       7,     71,   70,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         7,        7,      71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       70,    71,   70,   71,   9,    71,   71,         71,     71,   71,   8,    71,   71,         70,       70,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       70,    71,   70,   71,   71,   71,   71,         71,     71,   71,   10,   71,   71,         70,       70,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       70,    71,   70,   71,   11,   71,   71,         71,     71,   71,   71,   71,   71,         70,       70,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       12,    71,   71,   71,   71,   71,   71,         13,     71,   71,   71,   71,   71,         12,       12,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       14,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         14,       14,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   71,   71,   16,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       17,    70,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         17,       17,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       18,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         18,       18,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    19,   71,   70,   71,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   70,   71,   71,   71,   71,         20,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       21,    71,   71,   71,   71,   71,   71,         70,     71,   71,   71,   71,   71,         21,       21,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   70,   71,   71,   71,         71,     71,   71,   22,   71,   71,         71,       71,     70,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   70,   71,   71,   71,         23,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       24,    71,   71,   71,   71,   70,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    25,   71,   70,   71,   71,   71,         26,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   70,   27,   70,   71,         71,     71,   71,   71,   71,   71,         71,       71,     70,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   28,   71,   28,   71,         71,     71,   71,   71,   71,   71,         71,       71,     28,    71,   29,    29,     29,     29,    29,    29,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   70,   30,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   70,   31,   70,   71,         71,     71,   71,   71,   70,   71,         71,       71,     70,    71,   70,    70,     70,     70,    70,    70,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   70,   71,   70,   71,         71,     32,   32,   71,   70,   71,         71,       71,     70,    71,   70,    70,     70,     70,    70,    70,     32,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   71,   71,   71,         71,     33,   34,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   35,   71,   71,         71,     70,   70,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     70,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   71,   71,   71,         71,     70,   70,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     70,    36,   36,   36,     71 ],
                [ 0,  71,       71,    71,   71,   71,   37,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    70,   70,   70,     71 ],
                [ 0,  71,       71,    71,   71,   71,   38,   71,   71,         71,     71,   71,   39,   71,   71,         71,       71,     71,    70,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       40,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    70,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       41,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       42,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       44,    71,   71,   71,   71,   71,   71,         71,     71,   71,   43,   71,   71,         71,       71,     71,    44,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       70,    71,   71,   71,   71,   71,   71,         71,     71,   71,   45,   71,   71,         71,       71,     71,    70,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   71,   71,   71,         71,     71,   71,   46,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       48,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         47,       49,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       50,    71,   71,   71,   71,   70,   71,         71,     71,   71,   71,   71,   71,         50,       50,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   71,   70,   71,         71,     71,   71,   71,   71,   71,         71,       71,     51,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   52,   53,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   71,   70,   71,         71,     71,   71,   71,   71,   71,         71,       71,     54,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     55,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   71,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     56,    71,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   70,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    57,   71,    71,     71,     71,    71,    71,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   70,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   58,    59,     60,     61,    62,    63,     71,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   70,   71,   71,         71,     64,   65,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     66,    71,   71,   71,     71 ],
                [ 0,  71,       71,    71,   71,   71,   70,   71,   71,         71,     71,   71,   71,   71,   71,         71,       71,     71,    71,   71,    71,     71,     71,    71,    71,     71,    67,   68,   69,     71 ],
                ]

        self.productions = {
                1:          ["N_classDecl"],
                2:          ["classDecl","N_classDecl"],
                3:          ["EPSILON"],
                4:          ["class", "id","{" ,"RvarDeclfuncDef", "}" ,";"],
                5:          ["varDeclfuncDef","RvarDeclfuncDef"],
                6:          ["EPSILON"],
                7:          ["type", "id", "varDeclNoTypeIDorfuncDefNoTypeID"],
                8:          ["varDeclNoTypeID"],
                9:          ["funcDefNoTypeID"],
                10:         ["N_arraySize" ,";"],
                11:         ["(","fparam",")","funcBody",";"],
                12:         ["varDecl", "N_varDecl"],
                13:         ["EPSILON"],
                14:         ["funcDef", "N_funcDef"],
                15:         ["EPSILON"],
                16:         ["program", "funcBody", ";", "N_funcDef"],
                17:         ["type", "id", "(", "fParams",")"],
                18:         ["funcHead","funcBody",";"],
                19:         ["{" "N_varDecl", "N_statement", "}"],
                20:         ["statement", "N_statement"],
                21:         ["type", "id", "N_arraySize", ";"],
                22:         ["arraySize", "N_arraySize"],
                23:         ["for", "(", "type","id", "assignOp", "expr", ";" , "relExpr", ";", "assignStat", ")" ,"statBlock" ,";"],
                24:         ["variable", "assignOp", "expr"],
                25:         ["{", "N_statement", "}"],
                26:         ["statement"],
                27:         ["arithExpr", "COMM"],
                28:         ["EPSILON"],
                29:         ["relOp", "arithExpr"],
                30:         ["arithExpr", "relOp", "arithExpr"],
                31:         ["term", "arithExprLR"],
                32:         ["addOp", "term", "arithExprLR"],
                33:         ["+"],
                34:         ["-"],
                35:         ["factor", "termLR"],
                36:         ["multiOp", "factor", "termLR"],
                37:         ["(", "arithExpr", ")"],
                38:         ["(", "aParams", ")"],
                39:         ["N_indice"],
                40:         ["N_idnest","id","N_idnest_id"],
                41:         ["idnest" ,"N_idnest"],
                42:         ["id","N_indice"],
                43:         ["indice", "N_indice"],
                44:         ["EPSILON"],
                45:         ["[", "arithExpr", "]"],
                46:         ["[", "intNum", "]"],
                47:         ["float"],
                48:         ["id"],
                49:         ["int"],
                50:         ["type", "id", "N_arraySize", "N_fParamsTail"],
                51:         ["fParamsTail", "N_fParamsTail"],
                52:         ["expr" ,"N_aParamsTail"],
                53:         ["EPSILON"],
                54:         ["aParamsTail", "N_aParamsTail"],
                55:         [",", "type","id", "N_arraySize"],
                56:         [",", "expr"],
                57:         ["="],
                58:         ["lt"],
                59:         ["leq"],
                60:         ["neq"],
                61:         ["eq"],
                62:         ["gt"],
                63:         ["geq"],
                64:         ["+"],
                65:         ["-"],
                66:         ["or"],
                67:         ["*"],
                68:         ["/"],
                69:         ["and"]
                }

        self.syntatic_nodes = json.load(open(lex_file))
        node={"type": None, "value": "$", "line": 0}
        self.syntatic_nodes.append(node)
        self.syntatic_nodes_copy = copy.copy(self.syntatic_nodes)

    def is_terminal(self, _symb):
        return _symb in self.terminal_symbols_map

    def parse(self):
        def parse_terminal_symbol(token):
            if self.syntatic_nodes:  #not empty
                del self.syntatic_nodes[0]
            if self.syntatic_nodes:  #not empty
                token = syntatic_nodes[0].type
            if self.stack_inverse_derivation:
                del self.stack_inverse_derivation[-1]

        derivation = ['$']
        stack_inverse_derivation = ["$"]
        stack_inverse_derivation.append("prog")
        token = self.syntatic_nodes[0].type
        while stack_inverse_derivation and stack_inverse_derivation[-1] != "$":
            last_symbol = stack_inverse_derivation[-1]
            if self.is_terminal(last_symbol):
                parse_terminal_symbol(token)
            elif last_symbol == "EPSILON":
                if self.stack_inverse_derivation:
                    del self.stack_inverse_derivation[-1]
            else: #non-terminals
                row = self.non_terminal_symbols_map[last_symbol]
                column = self.terminal_symbols_map[token]
                rule = parse_table[row][column]
                if rule >= 70: #70 is error 71 is pop code
                    self.error_recovery(rule, token, previous_token_value, syntatic_nodes[0].line)
                elif rule == 0:
                    raise Exception("No CFG Rule found")
                else:
                    tmp_symbols = productions[rule]
                    if stack_inverse_derivation:
                        del stack_inverse_derivation[-1]

                    idx = derivation.index(last_symbol)
                    del derivation[idx]

                    if tmp_symbols[0] == "EPSILON":
                        continue;
                    for symbol in tmp_symbols:
                        derivation.insert

                    derivation.insert(idx, tmp_symbols)

                    for symbol in  reversed(tmp_symbols):
                        stack_inverse_derivation.append(symbol)


    def error_recovery(self, code, token, previous_token_value, line)
        if code == 70:
            del stack_inverse_derivation.stack_inverse_derivation[-1]
        else:
            del syntatic_nodes[0]
       token=syntatic_nodes[0].type

def main():
    parser = Parser('token.json')
    parser.parse()

if __name__ == "__main__":
    main()
