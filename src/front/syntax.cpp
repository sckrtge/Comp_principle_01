#include"front/syntax.h"

#include<iostream>
#include<cassert>

using frontend::Parser;

// #define DEBUG_PARSER
#define TODO assert(0 && "todo")
#define CUR_TOKEN_IS(tk_type) (token_stream[index].type == TokenType::tk_type)
#define NXT_TOKEN_IS(tk_type, nxt) (index + nxt < token_stream.size() && token_stream[index + nxt].type == TokenType::tk_type)
#define PARSE_TOKEN(tk_type) root->children.push_back(parseTerm(root, TokenType::tk_type))
#define PARSE(name, type) auto name = new type(root); assert(parse##type(name)); root->children.push_back(name); 


Parser::Parser(const std::vector<frontend::Token>& tokens): index(0), token_stream(tokens) {}

Parser::~Parser() {}

bool frontend::Parser::isExp() {
    return CUR_TOKEN_IS(LPARENT) || CUR_TOKEN_IS(IDENFR) 
    || CUR_TOKEN_IS(INTLTR) || CUR_TOKEN_IS(FLOATLTR) 
    || CUR_TOKEN_IS(LPARENT) || CUR_TOKEN_IS(PLUS) 
    || CUR_TOKEN_IS(MINU) || CUR_TOKEN_IS(NOT);
}

frontend::Term* frontend::Parser::parseTerm(AstNode *parent, TokenType expected) {
    if(token_stream[index].type == expected) {
        Term* node = new Term(token_stream[index],parent);
        index++;
        return node;
    }
    else {
        assert(0 && "Term error");
    }
}

frontend::CompUnit* Parser::get_abstract_syntax_tree(){
    CompUnit* root;
    if(parseCompUnit(root) == false) {
        assert(0 && "get_abstruct_syntax_tree error");
    }
    return root;
}

bool frontend::Parser::parseCompUnit(CompUnit* root) {
    bool error = true;
    if(CUR_TOKEN_IS(CONSTTK) || CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK) || CUR_TOKEN_IS(VOIDTK)) {
        if((CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK) || CUR_TOKEN_IS(VOIDTK)) && NXT_TOKEN_IS(IDENFR, 1) && NXT_TOKEN_IS(LPARENT, 2) ) {
            PARSE(son, FuncDef);
            error &= parseFuncDef(son);
        }
        else {
            PARSE(son, Decl);
            error &= parseDecl(son);
        }
    }
    else return false;
    if(index < token_stream.size()) {
        if(CUR_TOKEN_IS(CONSTTK) || CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK) || CUR_TOKEN_IS(VOIDTK)) {
            PARSE(son, CompUnit);
            error &= parseCompUnit(son);
        }
    }
    return error;
}

bool frontend::Parser::parseDecl(Decl* root) {
    bool error = true;
    if(CUR_TOKEN_IS(CONSTTK)) {
        PARSE(son, ConstDecl);
        error &= parseConstDecl(son);
    }
    else if(CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)) {
        PARSE(son, VarDecl);
        error &= parseVarDecl(son);
    }
    else return false;
    return error;
}

bool frontend::Parser::parseConstDecl(ConstDecl* root) {
    bool error = true;
    if(CUR_TOKEN_IS(CONSTTK)) {
        PARSE_TOKEN(CONSTTK);
    }
    else return false;
    if(CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)) {
        PARSE(son, BType);
        error &= parseBType(son);
    }
    else return false;
    if(CUR_TOKEN_IS(IDENFR)) {
        PARSE(son, ConstDef);
        error &= parseConstDef(son);
    }
    else return false;
    while(index < token_stream.size()) {
        if(CUR_TOKEN_IS(COMMA)) {
            PARSE_TOKEN(COMMA);
        }
        else break;
        if(CUR_TOKEN_IS(IDENFR)) {
            PARSE(son, ConstDef);
            error &= parseConstDef(son);
        }
        else return false;
    }
    if(CUR_TOKEN_IS(SEMICN)) {
        PARSE_TOKEN(SEMICN);
    }
    else return false;
    return error;
}

bool frontend::Parser::parseBType(BType* root) {
    if(CUR_TOKEN_IS(INTTK)) {
        PARSE_TOKEN(INTTK);
    }
    else if(CUR_TOKEN_IS(FLOATTK)) {
        PARSE_TOKEN(FLOATTK);
    }
    else return false;
    return true;
}

bool frontend::Parser::parseConstDef(ConstDef* root) {
    bool error = true;
    if(CUR_TOKEN_IS(IDENFR)) {
        PARSE_TOKEN(IDENFR);
    }
    else return false;
    while(index < token_stream.size()) {
        if(CUR_TOKEN_IS(LBRACK)) {
            PARSE_TOKEN(LBRACK);
        }
        else break;
        if(isExp()) {
            PARSE(son, ConstExp);
            error &= parseConstExp(son);
        }
        else return false;
        if(CUR_TOKEN_IS(RBRACK)) {
            PARSE_TOKEN(RBRACK);
        }
        else return false;
    }
    if(CUR_TOKEN_IS(ASSIGN)) {
        PARSE_TOKEN(ASSIGN);
    }
    else return false;
    if(isExp() || CUR_TOKEN_IS(LBRACE)) {
        PARSE(son, ConstInitVal);
        error &= parseConstInitVal(son);
    }
    else return false;
    return error;
}

bool frontend::Parser::parseConstInitVal(ConstInitVal* root) {
    bool error = true;
    if(isExp()) {
        PARSE(son, ConstExp);
        error &= parseConstExp(son);
    }
    else if(CUR_TOKEN_IS(LBRACE)) {
        PARSE_TOKEN(LBRACE);
        if(index < token_stream.size()) {
                if((isExp() || CUR_TOKEN_IS(LBRACE))) {
                PARSE(son, ConstInitVal);
                error &= parseConstInitVal(son);
                while(index < token_stream.size()) {
                    if(CUR_TOKEN_IS(COMMA)) {
                        PARSE_TOKEN(COMMA);
                    }
                    else break;
                    if(isExp() || CUR_TOKEN_IS(LBRACE)) {
                        PARSE(son, ConstInitVal);
                        error &= parseConstInitVal(son);
                    }
                    else return false;
                }
            }
        }
    }
    else return false;
    return error;
}

bool frontend::Parser::parseVarDecl(VarDecl* root) {
    bool error = true;
    if(CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)) {
        PARSE(son, BType);
        error &= parseBType(son);
    }
    else return false;
    if(CUR_TOKEN_IS(IDENFR)) {
        PARSE(son, VarDef);
        error &= parseVarDef(son);
    }
    else return false;
    while(index < token_stream.size()) {
        if(CUR_TOKEN_IS(COMMA)) {
            PARSE_TOKEN(COMMA);
        }
        else break;
        if(CUR_TOKEN_IS(IDENFR)) {
            PARSE(son, VarDef);
            error &= parseVarDef(son);
        }
        else return false;
    }
    if(CUR_TOKEN_IS(SEMICN)) {
        PARSE_TOKEN(SEMICN);
    }
    else return false;
    return error;
}

bool frontend::Parser::parseVarDef(VarDef* root) {
    bool error = true;
    if(CUR_TOKEN_IS(IDENFR)) {
        PARSE_TOKEN(IDENFR);
    }
    else return false;
    while(index < token_stream.size()) {
        if(CUR_TOKEN_IS(LBRACK)) {
            PARSE_TOKEN(LBRACK);
        }
        else break;
        if(isExp()) {
            PARSE(son, ConstExp);
            error &= parseConstExp(son);
        }
        else return false;
        if(CUR_TOKEN_IS(RBRACK)) {
            PARSE_TOKEN(RBRACK);
        }
        else return false;
    }
    if(index < token_stream.size()) {
        if(CUR_TOKEN_IS(ASSIGN)) {
            PARSE_TOKEN(ASSIGN);
            if(isExp() || CUR_TOKEN_IS(LBRACE)) {
                PARSE(son, InitVal);
                error &= parseInitVal(son);
            }
            else return false;
        }
    }
    return error;
}

bool frontend::Parser::parseInitVal(InitVal* root) {
    bool error = true;
    if(isExp()) {
        PARSE(son, Exp);
        error &= parseExp(son);
    }
    else if(CUR_TOKEN_IS(LBRACE)) {
        PARSE_TOKEN(LBRACE);
        if(index < token_stream.size()) {
            if(isExp() || CUR_TOKEN_IS(LBRACE)) {
                PARSE(son, InitVal);
                error &= parseInitVal(son);
                while(index < token_stream.size()) {
                    if(CUR_TOKEN_IS(COMMA)) {
                        PARSE_TOKEN(COMMA);
                    }
                    else break;
                    if(isExp() || CUR_TOKEN_IS(LBRACE)) {
                        PARSE(son, InitVal);
                        error &= parseInitVal(son);
                    }
                    else return false;
                }
            }
        }
        if(CUR_TOKEN_IS(RBRACE)) {
            PARSE_TOKEN(RBRACE);
        }
        else return false;
    }
    else return false;
    return error;
}

bool frontend::Parser::parseFuncDef(FuncDef* root) {
    bool error = true;
    if(CUR_TOKEN_IS(VOIDTK) || CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)) {
        PARSE(son, FuncType);
        error &= parseFuncType(son);
    }
    else return false;
    if(CUR_TOKEN_IS(IDENFR)) {
        PARSE_TOKEN(IDENFR);
    }
    else return false;
    if(CUR_TOKEN_IS(LPARENT)) {
        PARSE_TOKEN(LPARENT);
    }
    else return false;
    if(index < token_stream.size()) {
        if(CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)) {
            PARSE(son, FuncFParams);
            error &= parseFuncFParams(son);
        }
    }
    if(CUR_TOKEN_IS(RPARENT)) {
        PARSE_TOKEN(RPARENT);
    }
    else return false;
    if(CUR_TOKEN_IS(LBRACE)) {
        PARSE(son,Block);
        error &= parseBlock(son);
    }
    else return false;
    return error;
}

bool frontend::Parser::parseFuncType(FuncType* root) {
    bool error = true;
    if(CUR_TOKEN_IS(VOIDTK)) {
        PARSE_TOKEN(VOIDTK);
    }
    else if(CUR_TOKEN_IS(INTTK)) {
        PARSE_TOKEN(INTTK);
    }
    else if(CUR_TOKEN_IS(FLOATTK)) {
        PARSE_TOKEN(FLOATTK);
    }
    else return false;
    return error;
}

bool frontend::Parser::parseFuncFParam(FuncFParam* root) {
    bool error = true;
    if(CUR_TOKEN_IS(INTTK) || CUR_TOKEN_IS(FLOATTK)) {
        PARSE(son, BType);
        error &= parseBType(son);
    }
    else return false;
    if(CUR_TOKEN_IS(IDENFR)) {
        PARSE_TOKEN(IDENFR);
    }
    else return false;
    if(index < token_stream.size()) {
        if(CUR_TOKEN_IS(LBRACK)) {
            PARSE_TOKEN(LBRACK);
            if(CUR_TOKEN_IS(RBRACK)) {
                PARSE_TOKEN(RBRACK);
            }
            else return false;
            while(index < token_stream.size()) {
                if(CUR_TOKEN_IS(LBRACK)) {
                    PARSE_TOKEN(LBRACK);
                }
                else break;
                if(isExp()) {
                    PARSE(son,Exp);
                    error &= parseExp(son);
                }
                else return false;
                if(CUR_TOKEN_IS(RBRACK)) {
                    PARSE_TOKEN(RBRACK);
                }
                else return false;
            }
        }
    }
    return error;
}

bool frontend::Parser::parseFuncFParams(FuncFParams* root) {
    
}

bool frontend::Parser::parseBlock(Block* root) {
    
}

bool frontend::Parser::parseBlockItem(BlockItem* root) {
    
}

bool frontend::Parser::parseStmt(Stmt* root) {
    
}

bool frontend::Parser::parseExp(Exp* root) {
    
}

bool frontend::Parser::parseCond(Cond* root) {
    
}

bool frontend::Parser::parseLVal(LVal* root) {
    
}

bool frontend::Parser::parseNumber(Number* root) {
    
}

bool frontend::Parser::parsePrimaryExp(PrimaryExp* root) {
    
}

bool frontend::Parser::parseUnaryExp(UnaryExp* root) {
    
}

bool frontend::Parser::parseUnaryOp(UnaryOp* root) {
    
}

bool frontend::Parser::parseFuncRParams(FuncRParams* root) {
    
}

bool frontend::Parser::parseMulExp(MulExp* root) {
    
}

bool frontend::Parser::parseAddExp(AddExp* root) {
    
}

bool frontend::Parser::parseRelExp(RelExp* root) {
    
}

bool frontend::Parser::parseEqExp(EqExp* root) {
    
}

bool frontend::Parser::parseLAndExp(LAndExp* root) {
    
}

bool frontend::Parser::parseLOrExp(LOrExp* root) {
    
}

bool frontend::Parser::parseConstExp(ConstExp* root) {
    
}


void Parser::log(AstNode* node){
#ifdef DEBUG_PARSER
        std::cout << "in parse" << toString(node->type) << ", cur_token_type::" << toString(token_stream[index].type) << ", token_val::" << token_stream[index].value << '\n';
#endif
}
