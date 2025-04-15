#include"front/syntax.h"

#include<iostream>
#include<cassert>

using frontend::Parser;

// #define DEBUG_PARSER
#define TODO assert(0 && "todo")
#define CUR_TOKEN_IS(tk_type) (token_stream[index].type == TokenType::tk_type)
#define PARSE_TOKEN(tk_type) root->children.push_back(parseTerm(root, TokenType::tk_type))
#define PARSE(name, type) auto name = new type(root); assert(parse##type(name)); root->children.push_back(name); 


Parser::Parser(const std::vector<frontend::Token>& tokens): index(0), token_stream(tokens) {}

Parser::~Parser() {}

frontend::Term* frontend::Parser::parseTerm(AstNode *parent, TokenType expected) {
    if(token_stream[index].type == expected) {
        Term* node = new Term(token_stream[index]);
        parent->children.push_back(node);
        index++;
    }
    else {
        assert(0 && "Term wrong");
    }
}

frontend::CompUnit* Parser::get_abstract_syntax_tree(){
    CompUnit* root;
    if(parseCompUnit(root) == false) {
        assert(0 && "get_abstruct_syntax_tree wrong");
    }
    return root;
}

bool frontend::Parser::parseCompUnit(CompUnit* root) {
    
}

bool frontend::Parser::parseDecl(Decl* root) {
    
}

bool frontend::Parser::parseFuncDef(FuncDef* root) {
    
}

bool frontend::Parser::parseConstDecl(ConstDecl* root) {
    
}

bool frontend::Parser::parseBType(BType* root) {
    
}

bool frontend::Parser::parseConstDef(ConstDef* root) {
    
}

bool frontend::Parser::parseConstInitVal(ConstInitVal* root) {
    
}

bool frontend::Parser::parseVarDecl(VarDecl* root) {
    
}

bool frontend::Parser::parseVarDef(VarDef* root) {
    
}

bool frontend::Parser::parseInitVal(InitVal* root) {
    
}

bool frontend::Parser::parseFuncType(FuncType* root) {
    
}

bool frontend::Parser::parseFuncFParam(FuncFParam* root) {
    
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
