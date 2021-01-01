#include "tree.h"

map<string,map<int,int> > int_scope;
map<string,map<int,char> > char_scope;

symbol_table *beginTable=new symbol_table("globl");          //开始时创建一个全局的符号表，符号表是树状的。

int TreeNode::node_seq=0;
int TreeNode::temp_var_seq=-1;
int TreeNode::label_seq=0;

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
        if(node->nodeType==NODE_OP)
            if(node->opType>=OP_ADD&&node->opType<=OP_MOD)
                TreeNode::temp_var_seq++;
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
        cout<<opTypeToString(this->opType)<<" "<<"temp_var "<<this->temp_var<<" ";
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
            }else{
                temp->belong_table=table->findtable(table,temp->var_name); //为出现的每个变量都设置归属符号表
            }
        }
        if(temp->nodeType==NODE_OP){
            if(temp->opType>=OP_ADD&&temp->opType<=OP_MOD){              //对所有运算符
                get_temp_var(temp);
                if(temp->child->nodeType!=NODE_OP){                      //左儿子不为运算符
                    if(temp->child->nodeType==NODE_CONST&&temp->child->consType!=CONS_INTEGER){
                        cout<<"Type Check Error!"<<endl;
                        return;
                    }
                    if(temp->child->nodeType==NODE_VAR){
                        if(table->findtype(table,temp->child->var_name)!=VAR_INTEGER){
                        cout<<"Type Check Error!"<<endl;
                        return;
                        }
                    }
                }
                if(temp->child->sibling->nodeType!=NODE_OP){            //右儿子不为运算符
                    if(temp->child->sibling->nodeType==NODE_CONST&&temp->child->sibling->consType!=CONS_INTEGER){
                        cout<<"Type Check Error!"<<endl;
                        return;
                    }
                    if(temp->child->sibling->nodeType==NODE_VAR){
                        if(table->findtype(table,temp->child->sibling->var_name)!=VAR_INTEGER){
                        cout<<"Type Check Error!"<<endl;
                        return;
                        }
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

void TreeNode::get_temp_var(TreeNode *node){
    //待完善。。

    TreeNode *arg1=node->child;
    TreeNode *arg2=node->child->sibling;

    if(arg1->nodeType==NODE_OP)
        temp_var_seq--;
    if(arg2&&arg2->nodeType==NODE_OP)
        temp_var_seq--;
    node->temp_var=temp_var_seq;
    temp_var_seq++;

}

string TreeNode::new_label(){
    string temp="@";
    temp+=to_string(label_seq);
    label_seq++;
    return temp;
}

void TreeNode::recursive_get_label(TreeNode *node){
    if(node->nodeType==NODE_STMT)
        stmt_get_label(node);
    else if(node->nodeType==NODE_OP)
        expr_get_label(node);
}

void TreeNode::stmt_get_label(TreeNode *node){
    switch (node->stmtType)
    {
    case STMT_WHILE:
    {   
        TreeNode *e= node->child;               //循环条件
        TreeNode *s= node->child->sibling;      //循环体

        if(node->label.begin_label=="")
            node->label.begin_label=new_label();

        //循环体的下一条语句(循环开始)
        s->label.next_label = node->label.begin_label;

        //循环体的开始标号即为循环条件的真值的标号
        s->label.begin_label = e->label.true_label = new_label();

        //循环结束的标号
        if(node->label.next_label=="")
            node->label.next_label=new_label();
        
        //循环条件的假值标号即为循环的下一条语句标号
        e->label.false_label = node->label.next_label;

        //兄弟节点的开始标号即为当前节点的下一条语句标号
        if(node->sibling)
            node->sibling->label.begin_label=node->label.next_label;
        //递归生成
        recursive_get_label(e);
        recursive_get_label(s);
        break;
    }


    default:
        break;
    }


}

void TreeNode::expr_get_label(TreeNode *node){
    if(node->opType>OP_LTOE)
        return;
    TreeNode *e1= node->child;
    TreeNode *e2= node->child->sibling;
    switch (node->opType)
    {
    case OP_AND:
        e1->label.true_label=new_label();
        e2->label.true_label=node->label.true_label;
        e1->label.false_label=e2->label.false_label=node->label.false_label;
        break;
    case OP_OR:

        break;
    case OP_NOT:

        break;
    case OP_EQUAL:

        break;
    case OP_NEQUAL:

        break;
    case OP_MT:

        break;
    case OP_MTOE:

        break;
    case OP_LT:

        break;
    case OP_LTOE:

        break;
    default:
        break;
    }
}

void TreeNode::gen_header(ostream &out){
    out<< "# your asm code header here"<<endl;


}

void TreeNode::gen_decl(ostream &out,TreeNode *node){
    out<<endl<< "# define your veriables and temp veriables here" << endl;
    out << "\t.bss" << endl;
    for(;node->stmtType==STMT_DECL;node=node->sibling){
        for(TreeNode *p=node->child;p;p=p->sibling){
            if(p->varType==VAR_INTEGER)
                out<<"_"<<endl;         //待补充
                out<<"\t.zero\t4"<<endl;
                out<<"\t.align\t4"<<endl;
        }

    }

    for(int i=0;i<temp_var_seq;i++){
        out<<"t"<<i<<":"<<endl;
        out<<"\t.zero\t4"<<endl;
        out<<"\t.align\t4"<<endl;
    }

}

void TreeNode::recursive_gen_code(ostream &out,TreeNode *node){
    if(node->nodeType==NODE_OP){
        expr_gen_code(out,node);
    }else if(node->nodeType==NODE_STMT){
        stmt_gen_code(out,node);
    }
}

void TreeNode::stmt_gen_code(ostream &out,TreeNode *node){
    if(node->nodeType==NODE_STMT){
        switch (node->stmtType)
        {
        case STMT_WHILE:
        {

            break;
        }
        default:
            break;
        }
    }
    
}

void TreeNode::expr_gen_code(ostream &out,TreeNode *node){
    TreeNode *e1=node->child;
    TreeNode *e2=node->child->sibling;
    switch (node->opType)
    {
    case OP_ADD:
    {
        out << "\tmovl $";
		if (e1->nodeType == NODE_VAR)
			out << "_" ;              //待更新
		else if (e1->nodeType == NODE_CONST)
			out<<"something";//待更新
		else out << "t" << e1->temp_var;
		out << ", %eax" <<endl;
		out << "\taddl $";
		if (e2->nodeType == NODE_VAR)
			out << "_" ;                //待更新
		else if (e2->nodeType == NODE_CONST)
			out << "something";       //待更新
		else out << "t" << e2->temp_var;
		out << ", %eax" << endl;
		out << "\tmovl %eax, $t" << node->temp_var << endl;
        break;
    }
    
    
    default:
        break;
    }
}

void TreeNode::get_label(TreeNode *node){
    node->label.begin_label="_start";
    recursive_get_label(node);
}

void TreeNode::gen_code(ostream &out,TreeNode *node){
    gen_header(out);
    
    TreeNode *p=node->child;
    if(p->nodeType==NODE_STMT&&p->stmtType==STMT_DECL)    //打印变量的声明
        gen_decl(out,p);

    out<<endl<<endl<<"# your asm code here" << endl;
    out << "\t.text" << endl;
    out << "\t.globl _start" << endl;
    recursive_gen_code(out, node);                        //打印
    if(node->label.next_label!="")
        out<<node->label.next_label<<":"<<endl;
    out<<"\tret"<<endl;
}