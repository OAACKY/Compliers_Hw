#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H
// #include "./pch.h"
// using namespace std;

// enum ValueType
// {
//     VALUE_BOOL,
//     VALUE_INT,
//     VALUE_CHAR,
//     VALUE_STRING,
//     COMPOSE_STRUCT,
//     COMPOSE_UNION,
//     COMPOSE_FUNCTION
// };

// class Type
// {
// public:
//     ValueType type;
//     Type(ValueType valueType);
// public:  
//     /* 如果你要设计复杂类型系统的话，可以修改这一部分 */
//     ValueType* childType; // for union or struct
//     ValueType* paramType, retType; // for function
    
//     void addChild(Type* t);
//     void addParam(Type* t);
//     void addRet(Type* t);
// public:
//     ValueType* sibling; 
// public:
//     string getTypeInfo();
// };

// // 设置几个常量Type，可以节省空间开销
// static Type* TYPE_INT = new Type(VALUE_INT);
// static Type* TYPE_CHAR = new Type(VALUE_CHAR);
// static Type* TYPE_BOOL = new Type(VALUE_BOOL);
// static Type* TYPE_STRING = new Type(VALUE_STRING);

// int getSize(Type* type);

enum NodeType{
    NODE_CONST,
    NODE_VAR,
    NODE_TYPE,
    NODE_STMT,
    NODE_PROG,
    NODE_OP,
    NODE_FUNC,
    NODE_BOOL,
};

enum StmtType{
    STMT_IF,
    STMT_WHILE,
    STMT_DECL,
    STMT_ASSIGN,
    STMT_PRINTF,
    STMT_SCANF,
    STMT_IO,
    STMT_IDLIST,
    STMT_FOR,
    STMT_FOR_ST,
    STMT_LOOP
};

enum OpType{
    OP_EQUAL,
    OP_NEQUAL,
    OP_OR,
    OP_AND,
    OP_NOT,
    OP_MT,
    OP_LT,
    OP_MTOE,
    OP_LTOE,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_AA,
    OP_SS
};

enum VarType{
    VAR_INTEGER,
    VAR_VOID,
    VAR_CHAR
};

enum ConsType{
    CONS_INTEGER,
    CONS_STRING,
    CONS_CHAR
};

enum LoopType{
    LOOP_RETURN,
    LOOP_BREAK,
    LOOP_CONTINUE
};

#endif