#ifndef PIELANG_AST_H
#define PIELANG_AST_H

#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"

typedef enum {
  ASSIGN_OP = 1,
  MEMBER_OP,
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
  PLUS_PLUS_OP,
  MINUS_MINUS_OP,
  L_PARENTHESIS_OP,
  NOT_OP,
  CHECK_EQUALITY_OP,
  CHECK_NOT_EQUALITY_OP,
  CHECK_BIGGER_OP,
  CHECK_BIGGER_EQUAL_OP,
  CHECK_SMALLER_OP,
  CHECK_SMALLER_EQUAL_OP,
} Operator;

typedef enum {
  ASSIGN_PRECEDENCE = 1,
  CONDITIONAL_PRECEDENCE = 2,
  ADDITION_SUBTRACTION_PRECEDENCE = 3,
  MULTIPLICATION_DIVISION_MOD_PRECEDENCE = 4,
  EXPONENT_PRECEDENCE = 6,
  PREFIX_PRECEDENCE = 8,
  POSTFIX_PRECEDENCE = 9,
  L_PARENTHESIS_PRECEDENCE = 11,
  CALL_PRECEDENCE = 12,
  MEMBER_PRECEDENCE = 13,
} OperatorPrecedence;

typedef enum {
  StatementTypeExpressionStatement = 1,
  StatementTypeBlockDefinitionStatement,
  StatementTypePrintStatement,
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
  ExpressionTypePostfixExpression,
  ExpressionTypeCallExpression,
  ExpressionTypeTupleExpression,
} ExpressionType;

typedef enum {
  BlockDefinitionTypeIfBlock = 1,
  BlockDefinitionTypeForBlock,
} BlockDefinitionType;

typedef struct {
  StatementType statement_type;
} Statement;

typedef struct {
  ExpressionType expression_type;
  Value value;
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
  Operator operator;
  Expression *left_expression;
} PostfixExpression;

typedef struct {
  Expression expression;
  Expression **expressions;
  size_t expression_count;
} TupleExpression;

typedef struct {
  Expression expression;
  Expression *identifier_expression;
  Expression *tuple_expression;
} CallExpression;

typedef struct {
  Statement statement;
  Expression *right_expression;
} PrintStatement, ReturnStatement, ImportStatement;

typedef struct {
  Statement **statements;
  size_t statement_count;
} Block;

typedef struct {
  BlockDefinitionType block_definition_type;
  Block *block;
} BlockDefinition;

typedef struct {
  BlockDefinition block_definition;
  Expression *pre_expression;
  Expression *condition;
} IfBlockDefinition;

typedef struct {
  BlockDefinition block_definition;
  Expression *pre_expression;
  Expression *condition;
  Expression *post_expression;
} ForBlockDefinition;

typedef struct {
  Statement statement;
  BlockDefinition *block_definition;
} BlockDefinitionStatement;

typedef struct {
  Block *block;
} AST;

void printf_alignment(unsigned int alignment);

void printf_expression(Expression *expression);

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

bool check_if_token_is_postfix_operator(Token token);

bool is_operator_right_associative(Operator operator);

unsigned short get_operator_precedence(Operator operator, bool next);

#endif //PIELANG_AST_H
