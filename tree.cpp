#include "tree.h"

void TreeNode::addChild(TreeNode *temp){
    if(this->child==nullptr)
        this->child=temp;
    else{
        TreeNode *tempNode;
        tempNode=this->child;
        while(tempNode->sibling!=nullptr){
            tempNode=tempNode->sibling;
        }
        tempNode->sibling=temp;
    }
}

void TreeNode::addSibling(TreeNode *temp){
    TreeNode *myTemp=this;
    while(myTemp->sibling!=nullptr){
        myTemp=myTemp->sibling;
    }
    myTemp->sibling=temp;
}

int tempNodeId=0;

void dfs(TreeNode *node){
    while(node!=nullptr){
        node->nodeID=tempNodeId++;
        dfs(node->child);
        node=node->sibling;
    }
}

void TreeNode::genNodeId(){
    dfs(this);
    cout<<tempNodeId<<endl;
}

void printDfs(TreeNode *node){
    if(node!=nullptr)
        node->printNodeInfo();
    TreeNode *temp=node->child;
    if(temp!=nullptr)
        temp->printNodeConnection();
    cout<<endl;
    while(temp!=nullptr){
        printDfs(temp);
        temp=temp->sibling;
    }
}

void TreeNode::printAST(){
    printDfs(this);
}

void TreeNode::printNodeInfo(){
    cout<<"@"<<std::left<<std::setw(3)<<this->nodeID;
    this->nodeTypeInfo();
}
void TreeNode::printNodeConnection(){
    cout<<"children [";
    TreeNode *temp=this;
    while(true){
        cout<<"@"<<std::left<<setw(3)<<temp->nodeID;
        temp=temp->sibling;
        if(temp==nullptr)
            break;
    }
    cout<<"]";
}

void TreeNode::nodeTypeInfo(){
    switch (this->nodeType)
    {
    case NODE_PROG:
        cout<<"program  ";
        break;
    case NODE_STMT:
        cout<<"statement   stmt: "<<sTypeToString(this->stmtType)<<" ";
        break;
    case NODE_CONST:
        cout<<"const type: "<<consTypeToString(this->consType)<<" ";
        break;
    case NODE_OP:
        cout<<opTypeToString(this->opType)<<" ";
        break;
    case NODE_VAR:
        cout<<"variable varname: "<<this->var_name;
        break;
    case NODE_TYPE:
        cout<<"type type: "<<varTypeToString(this->varType)<<" ";
        break;
    default:
        break;
    }
}

string TreeNode::opTypeToString(OpType type){
    switch (type)
    {
    case OP_EQUAL:
        return "equal";
        break;
    case OP_NEQUAL:
        return "nequal";
        break;
    case OP_LT:
        return "lessthan";
        break;
    case OP_LTOE:
        return "loequal";
        break;
    case OP_MT:
        return "morethan";
        break;
    case OP_MTOE:
        return "moequal";
        break;
    case OP_ADD:
        return "add";
        break;
    case OP_SUB:
        return "sub";
        break;
    case OP_MUL:
        return "mul";
        break;
    case OP_DIV:
        return "div";
        break;
    case OP_MOD:
        return "mod";
        break;
    case OP_OR:
        return "or";
        break;
    case OP_AND:
        return "and";
        break;
    case OP_NOT:
        return "not";
        break;
    default:
        return "error";
    }
}

string TreeNode::consTypeToString(ConsType type){
    switch (type)
    {
    case CONS_INTEGER:
        return "integer";
        break;
    case CONS_STRING:
        return "string";
        break;
    case CONS_CHAR:
        return "char";
        break;
    default:
        return "error";
    }
}

string TreeNode::varTypeToString(VarType type){
    switch (type)
    {
    case VAR_CHAR:
        return "char";
        break;
    case VAR_INTEGER:
        return "integer";
        break;
    case VAR_VOID:
        return "void";
        break;
    default:
        return "error";
    }
}

string TreeNode::sTypeToString(StmtType type){
    switch (type)
    {
    case STMT_IF:
        return "if";
        break;
    case STMT_PRINTF:
        return "printf";
        break;
    case STMT_SCANF:
        return "scanf";
        break;
    case STMT_WHILE:
        return "while";
        break;
    case STMT_DECL:
        return "decl";
        break;
    case STMT_ASSIGN:
        return "assign";
        break;
    case STMT_IDLIST:
        return "idlist";
        break;
    default:
        return "error";
    }
}

TreeNode::TreeNode(NodeType mytype){
    this->nodeType=mytype;
}