#ifndef PIELANG_AST_H
#define PIELANG_AST_H

#include "bool.h"

#define ASSIGN_PRECEDENCE 1
#define COMMA_PRECEDENCE 2
#define ASYNC_AWAIT_PRECEDENCE 3
#define IN_OP_PRECEDENCE 4
#define CONDITIONAL_PRECEDENCE 5
#define ADDITION_SUBTRACTION_PRECEDENCE 6
#define MULTIPLICATION_DIVISION_MOD_PRECEDENCE 7
#define EXPONENT_PRECEDENCE 8
#define CALL_PRECEDENCE 10
#define LIST_PRECEDENCE 11
#define MEMBER_PRECEDENCE 12
#define RANGE_PRECEDENCE 13

#include "lexer.h"

typedef enum {
  ASSIGN_OP = 1,
  ADDITION_OP,
  ASSIGN_ADDITION_OP,
  SUBTRACTION_OP,
  ASSIGN_SUBTRACTION_OP,
  MULTIPLICATION_OP,
  ASSIGN_MULTIPLICATION_OP,
  DIVISION_OP,
  ASSIGN_DIVISION_OP,
  INTEGER_DIVISION_OP,
  ASSIGN_INTEGER_DIVISION_OP,
  EXPONENT_OP,
  ASSIGN_EXPONENT_OP,
  MOD_OP,
  ASSIGN_MOD_OP,
  L_PARENTHESIS_OP,
  L_BRACKET_OP,
  NOT_OP,
  CHECK_EQUALITY_OP,
  CHECK_NOT_EQUALITY_OP,
  CHECK_BIGGER_OP,
  CHECK_BIGGER_EQUAL_OP,
  CHECK_SMALLER_OP,
  CHECK_SMALLER_EQUAL_OP,
  MEMBER_OP,
  IN_OP,
  RANGE_OP,
  COMMA_OP,
  ASYNC_OP,
  AWAIT_OP,
} Operator;

typedef enum {
  StatementTypeExpressionStatement = 1,
  StatementTypeBlockDefinitionStatement,
  StatementTypeReturnStatement,
  StatementTypeImportStatement,
} StatementType;

typedef enum {
  ExpressionTypeNullExpression = 0,
  ExpressionTypeIntegerExpression,
  ExpressionTypeFloatExpression,
  ExpressionTypeStringExpression,
  ExpressionTypeBoolExpression,
  ExpressionTypeIdentifierExpression,
  ExpressionTypeInfixExpression,
  ExpressionTypePrefixExpression,
  ExpressionTypeCallExpression,
  ExpressionTypeArrayExpression,
  ExpressionTypeIndexExpression,
  ExpressionTypeFunctionExpression,
} ExpressionType;

typedef enum {
  BlockDefinitionTypeIfBlock = 1,
  BlockDefinitionTypeElseBlock,
  BlockDefinitionTypeIfElseGroupBlock,
  BlockDefinitionTypeForBlock,
} BlockDefinitionType;

typedef enum {
  ArrayExpressionTypeList = 1,
  ArrayExpressionTypeTuple,
} ArrayExpressionType;

typedef struct {
  StatementType statement_type;
} Statement;

typedef struct {
  ExpressionType expression_type;
  Literal *literal;
} Expression;

typedef struct {
  Statement statement;
  Expression *expression;
} ExpressionStatement;

typedef struct {
  Statement statement;
} DefinitionStatement;

typedef struct {
  Expression expression;
  Operator operator;
  Expression *left_expression;
  Expression *right_expression;
} InfixExpression;

typedef struct {
  Expression expression;
  Operator operator;
  Expression *right_expression;
} PrefixExpression;

typedef struct {
  Expression expression;
  Expression **expressions;
  size_t expression_count;
  ArrayExpressionType array_expression_type;
  bool has_finished;
} ArrayExpression;

typedef struct {
  Expression expression;
  Expression *left_expression;
  Expression *right_expression;
} IndexExpression;

typedef struct {
  Expression expression;
  Expression *identifier_expression;
  Expression *tuple_expression;
} CallExpression;

typedef struct {
  Statement statement;
  Expression *right_expression;
} ReturnStatement, ImportStatement;

typedef struct {
  Statement **statements;
  size_t statement_count;
} Block;

typedef struct {
  BlockDefinitionType block_definition_type;
} BlockDefinition;

typedef struct {
  BlockDefinition block_definition;
  Block *block;
  Expression *pre_expression;
  Expression *condition;
} IfBlockDefinition;

typedef struct {
  BlockDefinition block_definition;
  Block *block;
} ElseBlockDefinition;

typedef struct {
  BlockDefinition block_definition;
  size_t if_block_definitions_length;
  IfBlockDefinition **if_block_definitions;
  ElseBlockDefinition *else_block_definition;
} IfElseGroupBlockDefinition;

typedef struct {
  BlockDefinition block_definition;
  Block *block;
  Expression *pre_expression;
  Expression *condition;
  Expression *post_expression;
} ForBlockDefinition;

typedef struct {
  Expression expression;
  Block *block;
  Expression *identifier;
  char **arguments;
  size_t argument_count;
} FunctionExpression;


typedef struct {
  Statement statement;
  BlockDefinition *block_definition;
} BlockDefinitionStatement;

typedef struct {
  Block *block;
} AST;

typedef enum {
  DEFAULT_BLOCK_PARSER_LIMITER = 1,
  DEFAULT_EXPRESSION_PARSER_LIMITER,
  GROUPED_EXPRESSION_PARSER_LIMITER,
  ARRAY_EXPRESSION_PARSER_LIMITER,
  INDEX_EXPRESSION_PARSER_LIMITER,
  IF_BLOCK_EXPRESSION_PARSER_LIMITER,
  FOR_BLOCK_EXPRESSION_PARSER_LIMITER,
} ParserLimiter;


void printf_alignment(unsigned int alignment);


void printf_expression(Expression *expression, unsigned int alignment);


void printf_block(Block *block, unsigned int alignment);


void printf_block_definition(BlockDefinition *block_definition, unsigned int alignment);


void printf_statement(Statement *statement, unsigned int alignment);


void printf_ast(AST *ast);


void free_expression(Expression *expression);


void free_statement(Statement *statement);


void free_block(Block *block);


void free_block_definition(BlockDefinition *block_definition);


void free_ast(AST *ast);


Operator token_to_operator(Token token);


bool check_if_token_is_operator(Token token);


bool is_operator_right_associative(Operator operator);


unsigned short get_operator_precedence(Operator operator, bool next);


bool has_finished(Token token, ParserLimiter limiter);


void *parser_error();


Expression *eval_token(Token token);


Expression *parse_grouped_expression(Lexer *lexer);


Expression *parse_list_expression(Lexer *lexer);


Expression *parse_prefix_expression(Lexer *lexer, ParserLimiter limiter);


Expression *parse_array_expression(Lexer *lexer, Expression *left);


Expression *parse_index_expression(Lexer *lexer, Expression *left);


Expression *parse_call_expression(Lexer *lexer, Expression *identifier);


Expression *parse_infix_expression(Lexer *lexer, ParserLimiter limiter, Expression *left);


Expression *parse_function_expression(Lexer *lexer);


Expression *parse_expression(Lexer *lexer, unsigned short precedence, ParserLimiter limiter);


Block *parse_block(Lexer *lexer, ParserLimiter limiter);


BlockDefinition *parse_if_block_definition(Lexer *lexer, ParserLimiter limiter);


BlockDefinition *parse_for_block_definition(Lexer *lexer, ParserLimiter limiter);


BlockDefinition *parse_block_definition(Lexer *lexer, ParserLimiter limiter);


Statement *parse_block_definition_statement(Lexer *lexer, ParserLimiter limiter);


Statement *parse_return_statement(Lexer *lexer, ParserLimiter limiter);


Statement *parse_import_statement(Lexer *lexer, ParserLimiter limiter);


Statement *parse_expression_statement(Lexer *lexer, ParserLimiter limiter);


Statement *parse_statement(Lexer *lexer);


AST *parse_ast(Lexer *lexer);


#endif //PIELANG_AST_H
