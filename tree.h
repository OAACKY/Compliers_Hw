#ifndef TREE_H
#define TREE_H
#include "type.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

enum NodeType{
    NODE_CONST,
    NODE_VAR,
    NODE_TYPE,
    NODE_STMT,
    NODE_PROG,
    NODE_OP,
    NODE_FUNC,
    NODE_BOOL
};

enum StmtType{
    STMT_IF,
    STMT_WHILE,
    STMT_DECL,
    STMT_ASSIGN,
    STMT_PRINTF,
    STMT_SCANF,
    STMT_IDLIST
};

enum OpType{
    OP_EQUAL,
    OP_NEQUAL,
    OP_OR,
    OP_AND,
    OP_NOT,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_MT,
    OP_LT,
    OP_MTOE,
    OP_LTOE
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

struct TreeNode {
public:
    int nodeID;
    NodeType nodeType;

    TreeNode *child = nullptr;
    TreeNode *sibling = nullptr;

    void addChild(TreeNode *);
    void addSibling(TreeNode *);

    void genNodeId();//从根节点开始逐个赋Id 实现方式同学们可以自行修改

    void printAST();//打印语法树结点
    /***
     * 以下的几个函数皆为在printAST过程中辅助输出使用
     * 同学们可以根据需要自己使用其他方法
    ***/
    void printNodeInfo();
    void printNodeConnection();
    void nodeTypeInfo();

    Type* type;  // 变量、类型、表达式结点，有类型。

    int int_val;
    char char_val;
    bool bool_val;
    StmtType stmtType;
    OpType opType;  //如果是表达式
    VarType varType;
    ConsType consType;
    string str_val;
    string var_name;

    static string consTypeToString(ConsType type);
    static string varTypeToString(VarType type);
    static string opTypeToString(OpType type);
    static string sTypeToString(StmtType type);

    TreeNode(NodeType mytype);
};
#endif