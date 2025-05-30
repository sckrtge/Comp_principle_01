/**
 * @file lexical.h
 * @author Yuntao Dai (d1581209858@live.com)
 * @brief
 * in this part, we need to design a DFA
 * so what should this DFA do? 
 * input a char, then acording to its current status, change to another state
 * during the changes, from one state to another, there maybe output -> Token
 * 
 * then we need to design a Scanner
 * which takes the input file and output a Token stream
 * 
 * @version 0.1
 * @date 2022-12-14
 *
 * @copyright Copyright (c) 2022
 */

#ifndef LEXICAL_H
#define LEXICAL_H

#include"front/token.h"

#include<set>
#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include <map>
#include <cctype>
#include <cassert>

namespace frontend {

// so we need a enumerate class for status
enum class State {
    Empty,              // space, \n, \r ...
    Ident,              // a keyword or identifier, like 'int' 'a0' 'else' ...
    IntLiteral,         // int literal, like '1' '1900', only in decimal
    FloatLiteral,       // float literal, like '0.1'
    op                  // operators and '{', '[', '(', ',' ...
};

// we should distinguish the keyword and a variable(function) name, so we need a keyword table here
extern std::set<std::string> keywords;

static const std::map<std::string, TokenType> keywordMap = {
    {"const", TokenType::CONSTTK},
    {"void", TokenType::VOIDTK},
    {"int", TokenType::INTTK},
    {"float", TokenType::FLOATTK},
    {"if", TokenType::IFTK},
    {"else", TokenType::ELSETK},
    {"while", TokenType::WHILETK},
    {"continue", TokenType::CONTINUETK},
    {"break", TokenType::BREAKTK},
    {"return", TokenType::RETURNTK}
};

static const std::map<std::string, TokenType> operatorMap = {
    {"<=", TokenType::LEQ},
    {">=", TokenType::GEQ},
    {"==", TokenType::EQL},
    {"!=", TokenType::NEQ},
    {"&&", TokenType::AND},
    {"||", TokenType::OR},
    {"+", TokenType::PLUS},
    {"-", TokenType::MINU},
    {"*", TokenType::MULT},
    {"/", TokenType::DIV},
    {"%", TokenType::MOD},
    {"<", TokenType::LSS},
    {">", TokenType::GTR},
    {":", TokenType::COLON},
    {"=", TokenType::ASSIGN},
    {";", TokenType::SEMICN},
    {",", TokenType::COMMA},
    {"(", TokenType::LPARENT},
    {")", TokenType::RPARENT},
    {"[", TokenType::LBRACK},
    {"]", TokenType::RBRACK},
    {"{", TokenType::LBRACE},
    {"}", TokenType::RBRACE},
    {"!", TokenType::NOT}
};

std::string toString(State);

// definition of DFA
struct DFA {
    /**
     * @brief constructor, set the init state to State::Empty
     */
    DFA();
    
    /**
     * @brief destructor
     */
    ~DFA();
    
    // the meaning of copy and assignment for a DFA is not clear, so we do not allow them
    DFA(const DFA&) = delete;   // copy constructor
    DFA& operator=(const DFA&) = delete;    // assignment

    /**
     * @brief take a char as input, change state to next state, and output a Token if necessary
     * @param[in] input: the input character
     * @param[out] buf: the output Token buffer
     * @return  return true if a Token is produced, the buf is valid then
     */
    
    TokenType str2operator(const std::string &str);
    
    TokenType str2keyword(const std::string &str);
    
    bool change_status(Token &buf, State state, std::string str, bool output, TokenType type, std::string value);

    bool next(char input, Token& buf) ;
    
    /**
     * @brief reset the DFA state to begin
     */
    void reset();

private:
    State cur_state;    // record current state of the DFA
    std::string cur_str;    // record input characters
};

// definition of Scanner
struct Scanner {
    /**
     * @brief constructor
     * @param[in] filename: the input file  
     */
    Scanner(std::string filename);
    
    /**
     * @brief destructor, close the file
     */
    ~Scanner();

    // rejcet copy and assignment
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;

    /**
     * @brief run the scanner, analysis the input file and result a token stream
     * @return std::vector<Token>: the result token stream
     */
    std::vector<Token> run();

private:
    std::ifstream fin;  // the input file
};

} // namespace frontend

#endif