#ifndef TREE_H
#define TREE_H
#include "type.h"
#include "symbol.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;


struct Label {
    string true_label;
    string false_label;
    string begin_label;
    string next_label;
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
    static int node_seq=0;       //对所有node均为相同的值
    static int temp_var_seq=0;
    static int label_seq=0;
    Label label;     //每个语法树节点的标签

    int temp_var;     //变量的标号
    //标签与代码生成
    void get_temp_var(TreeNode *);
    string new_label(void);
    void recursive_get_label(TreeNode *);
    void stmt_get_label(TreeNode *);
    void expr_get_label(TreeNode *);
    void gen_header(ostream &out);
    void gen_decl(ostream &out,TreeNode *);
    void recursive_gen_code(ostream &out,TreeNode *);
    void stmt_gen_code(ostream &out,TreeNode *);
    void expr_gen_code(ostream &out,TreeNode *);

    void get_label(TreeNode *);
    void gen_code(ostream &out,TreeNode *);

};
#endif