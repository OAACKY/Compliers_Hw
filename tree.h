#ifndef TREE_H
#define TREE_H
#include "type.h"
#include "symbol.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;




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
    //void Type_Check(TreeNode *,symbol_table *);   //类型检查与符号表进行合并
    /***
     * 以下的几个函数皆为在printAST过程中辅助输出使用
    ***/
    void printNodeInfo();
    void printNodeConnection();
    void nodeTypeInfo();

    void symbolTable(TreeNode *,symbol_table *);

    symbol_table *belong_table;
    //Type* type;  // 变量、类型、表达式结点，有类型。
    
    int scope;

    int int_val;
    char char_val;
    bool bool_val;
    StmtType stmtType;
    OpType opType;  //如果是表达式
    VarType varType;
    ConsType consType;
    LoopType loopType;
    string str_val;
    string var_name;

    static string loopTypeToString(LoopType type);
    static string consTypeToString(ConsType type);
    static string varTypeToString(VarType type);
    static string opTypeToString(OpType type);
    static string sTypeToString(StmtType type);

    TreeNode(NodeType mytype);
};
#endif