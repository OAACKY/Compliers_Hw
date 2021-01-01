#include "tree.h"

map<string,map<int,int> > int_scope;
map<string,map<int,char> > char_scope;

symbol_table *beginTable=new symbol_table("globl");          //开始时创建一个全局的符号表，符号表是树状的。

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
    //cout<<tempNodeId<<endl;    输出节点数
    symbolTable(this,beginTable);
    //Type_Check(this,beginTable);  符号表和类型检查合并
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
        if(this->stmtType==STMT_LOOP)
        cout<<loopTypeToString(this->loopType)<<" ";
        break;
    case NODE_CONST:
        cout<<"const type: "<<consTypeToString(this->consType)<<" ";
        if(this->consType==CONS_INTEGER)
        cout<<this->int_val<<" ";
        else if(this->consType==CONS_STRING)
        cout<<this->str_val<<" ";
        else if(this->consType==CONS_CHAR)
        cout<<this->char_val<<" ";
        break;
    case NODE_OP:
        cout<<opTypeToString(this->opType)<<" ";
        break;
    case NODE_VAR:
        cout<<"variable varname: "<<this->var_name<<" type: "<<varTypeToString(this->varType)<<" ";
        break;
    case NODE_TYPE:
        cout<<"type type: "<<varTypeToString(this->varType)<<" ";
        break;
    case NODE_FUNC:
        cout<<"function funcname: "<<this->var_name<<" ";
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
    case OP_AA:
        return "selfadd";
        break;
    case OP_SS:
        return "selfsub";
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

string TreeNode::loopTypeToString(LoopType type){
    switch (type)
    {
    case LOOP_RETURN:
        return "return";
        break;
    case LOOP_BREAK:
        return "break";
        break;
    case LOOP_CONTINUE:
        return "continue";
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
    case STMT_IO:
        return "io_content";
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
    case STMT_FOR:
        return "for";
        break;
    case STMT_FOR_ST:
        return "for_st";
        break;
    case STMT_LOOP:
        return "loop";
        break;
    default:
        return "error";
    }
}

TreeNode::TreeNode(NodeType mytype){
    this->nodeType=mytype;
}

void TreeNode::symbolTable(TreeNode *node,symbol_table *table){
    //symbol_table *tempTable=beginTable;
    TreeNode *temp=node->child;
    while(temp!=nullptr){
        if(temp->nodeType==NODE_FUNC||(temp->nodeType==NODE_STMT&&(temp->stmtType==STMT_IF||temp->stmtType==STMT_WHILE))){
            symbol_table *newChild = new symbol_table(to_string(temp->nodeID));   //表名为stmt或fun在语法树中的节点标号
            table->addchild(newChild);
            symbolTable(temp,newChild);
            temp=temp->sibling;
            continue;
        }
        if(temp->nodeType==NODE_STMT&&temp->stmtType==STMT_DECL){
            if(temp->child->sibling->nodeType==NODE_VAR){
                //如果是单个变量的声明
                map<string,symbol_entry *>::iterator it;
                it = table->table_content.find(temp->child->sibling->var_name);
                if(it==table->table_content.end()){
                    symbol_entry * entry = new symbol_entry(temp->child->sibling->nodeID,true,temp->child->varType);
                    table->table_content[temp->child->sibling->var_name]=entry;
                    temp->child->sibling->belong_table=table;
                }else{
                    cout<<"redifine error!"<<temp->child->sibling->var_name<<endl;
                }
            }else{
                //如果是idlist的声明
                TreeNode *tempIdlist=temp->child->sibling->child;
                while(tempIdlist!=nullptr){
                    map<string,symbol_entry *>::iterator it;
                    it = table->table_content.find(tempIdlist->var_name);
                    if(it==table->table_content.end()){
                        symbol_entry * entry = new symbol_entry(tempIdlist->nodeID,true,temp->child->varType);
                        table->table_content[tempIdlist->var_name]=entry;
                        tempIdlist->belong_table=table;
                        //cout<<"已存入 "<<tempIdlist->var_name<<endl;
                    }else{
                        cout<<"redifine error!"<<tempIdlist->var_name<<endl;
                    }
                    tempIdlist=tempIdlist->sibling;
                }
            }
            temp=temp->sibling;    //跳过这个子树
            continue;
        }
        if(temp->nodeType==NODE_VAR){
            if(!table->checkExist(table,temp->var_name)){
                cout<<"nodifine error!"<<temp->var_name<<endl;
            }
        }
        if(temp->nodeType==NODE_OP){
            if(temp->opType>=OP_ADD&&temp->opType<=OP_MOD){              //对所有运算符
                if(temp->child->nodeType!=NODE_OP){                      //左儿子不为运算符
                    if(temp->child->nodeType==NODE_CONST&&temp->child->consType!=CONS_INTEGER){
                        cout<<"Type Check Error!"<<endl;
                        return;
                    }
                    if(temp->child->nodeType==NODE_VAR&&temp->child->varType!=VAR_INTEGER){
                        cout<<"Type Check Error!"<<endl;
                        return;
                    }
                }
                if(temp->child->sibling->nodeType!=NODE_OP){            //右儿子不为运算符
                    if(temp->child->sibling->nodeType==NODE_CONST&&temp->child->sibling->consType!=CONS_INTEGER){
                        cout<<"Type Check Error!"<<endl;
                        return;
                    }
                    if(temp->child->sibling->nodeType==NODE_VAR&&table->table_content[temp->child->sibling->var_name]->entry_type!=VAR_INTEGER){
                        cout<<"Type Check Error!"<<endl;
                        return;
                        //temp->child->sibling->belong_table->table_content[node->child->sibling->var_name]->entry_type
                    }
                    //cout<<"检查结束"<<node->child->sibling->varType<<endl;
                }
            }
        }
        symbolTable(temp,table);
        temp=temp->sibling;
    }

}

// void TreeNode::Type_Check(TreeNode *node,symbol_table *table){
//     while(node!=nullptr){
//         if(node->nodeType==NODE_FUNC||(node->nodeType==NODE_STMT&&(node->stmtType==STMT_IF||node->stmtType==STMT_WHILE))){
//             Type_Check(node,table->child);
//             node=node->sibling;
//             continue;
//         }
//         if(node->nodeType==NODE_OP){
//             if(node->opType>=OP_ADD&&node->opType<=OP_MOD){              //对所有运算符
//                 if(node->child->nodeType!=NODE_OP){                      //左儿子不为运算符
//                     if(node->child->nodeType==NODE_CONST&&node->child->consType!=CONS_INTEGER){
//                         cout<<"Type Check Error!"<<endl;
//                         return;
//                     }
//                     if(node->child->nodeType==NODE_VAR&&node->child->varType!=VAR_INTEGER){
//                         cout<<"Type Check Error!"<<endl;
//                         return;
//                     }
//                 }
//                 if(node->child->sibling->nodeType!=NODE_OP){            //右儿子不为运算符
//                     if(node->child->sibling->nodeType==NODE_CONST&&node->child->sibling->consType!=CONS_INTEGER){
//                         cout<<"Type Check Error!"<<endl;
//                         return;
//                     }
//                     if(node->child->sibling->nodeType==NODE_VAR&&node->child->sibling->varType!=VAR_INTEGER){
//                         cout<<"Type Check Error!"<<endl;
//                         return;
//                         //node->child->sibling->belong_table->table_content[node->child->sibling->var_name]->entry_type
//                     }
//                     //cout<<"检查结束"<<node->child->sibling->varType<<endl;
//                 }
//             }
//         }
//     Type_Check(node->child,table);
//     node=node->sibling;
//     }
// }