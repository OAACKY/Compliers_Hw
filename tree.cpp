#include "tree.h"

map<string,map<int,int> > int_scope;
map<string,map<int,char> > char_scope;

symbol_table *beginTable=new symbol_table("globl");          //开始时创建一个全局的符号表，符号表是树状的。

int TreeNode::node_seq=0;
int TreeNode::temp_var_seq=0;
int TreeNode::label_seq=0;

int cons_num = 0;


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
        // if(node->nodeType==NODE_OP)
        //     if(node->opType>=OP_ADD&&node->opType<=OP_MOD)
        //         TreeNode::temp_var_seq++;
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
        cout<<"variable varname: "<<this->var_name<<" type: "<<varTypeToString(this->belong_table->findtype(this->belong_table,this->var_name))<<" ";
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
                    if(temp->child->sibling->sibling->nodeType==NODE_VAR)
                        temp->child->sibling->sibling->belong_table=table->findtable(table,temp->child->sibling->sibling->var_name);
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
                temp->varType=table->findtype(table,temp->var_name);       //设置每个变量的符号
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

    // if(arg1->nodeType==NODE_OP)
    //     temp_var_seq--;
    // if(arg2&&arg2->nodeType==NODE_OP)
    //     temp_var_seq--;
    node->temp_var=temp_var_seq;
    temp_var_seq++;

}

string TreeNode::new_label(){
    string temp=".L";
    temp+=to_string(label_seq);
    label_seq++;
    return temp;
}

void TreeNode::recursive_get_label(TreeNode *node){
    if(node==nullptr)
        return;
    if(node->nodeType==NODE_STMT||node->nodeType==NODE_PROG||node->nodeType==NODE_FUNC)
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
    case STMT_IF:
    {
        TreeNode *e=node->child;
        TreeNode *s1=node->sibling;
        TreeNode *s2=node->sibling;

        if(node->label.begin_label=="")
            node->label.begin_label=new_label();

        s1->label.begin_label=e->label.true_label=new_label();

        if(node->label.next_label=="")
            node->label.next_label=new_label();
        s1->label.next_label=node->label.next_label;
        
        if(s2==nullptr){
            e->label.false_label=node->label.next_label;
        }
        else{
            e->label.false_label=s2->label.begin_label=new_label();
            s2->label.next_label=node->label.next_label;
        }

        if(node->sibling)
            node->sibling->label.begin_label=node->label.next_label;
        recursive_get_label(e);
        recursive_get_label(s1);
        recursive_get_label(s2);
        break;
    }
    case STMT_FOR:
    {

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

void TreeNode::dfs_gen_decl(ostream &out,TreeNode *node){
    while(node!=nullptr){
        if(node->nodeType==NODE_STMT&&node->stmtType==STMT_DECL){
            if(node->child->sibling->nodeType!=NODE_STMT)      //对于单个变量类型声明
            {
                TreeNode *p=node->child->sibling;
                if(node->child->varType==VAR_INTEGER){
                    out<<"_"<<p->belong_table->table_name<<"_"<<p->var_name<<":"<<endl;       
                    out<<"\t.zero\t4"<<endl;
                    out<<"\t.align\t4"<<endl;
                }else{
                    out<<"_"<<p->belong_table->table_name<<"_"<<p->var_name<<":"<<endl;       
                    out<<"\t.zero\t1"<<endl;
                }
            }else{                                             //对于idlist声明
            for(TreeNode *p=node->child->sibling->child;p;p=p->sibling){
                if(node->child->varType==VAR_INTEGER){
                    out<<"_"<<p->belong_table->table_name<<"_"<<p->var_name<<":"<<endl;       
                    out<<"\t.zero\t4"<<endl;
                    out<<"\t.align\t4"<<endl;
                }else{
                    out<<"_"<<p->belong_table->table_name<<"_"<<p->var_name<<":"<<endl;       
                    out<<"\t.zero\t1"<<endl;
                }
            }
            }
            node=node->sibling;
            continue;
        }
        dfs_gen_decl(out,node->child);
        node=node->sibling;
    }

}

void TreeNode::dfs_gen_cons(ostream &out,TreeNode *node){
    while(node!=nullptr){
        if(node->nodeType==NODE_CONST&&node->consType==CONS_STRING)
        {     node->temp_cons=cons_num++;
              out<<"LC"<<node->temp_cons<<":"<<endl;
              out<<"\t.string "<<"\""<<node->str_val<<"\""<<endl;
        }
        dfs_gen_cons(out,node->child);
        node=node->sibling;
    }

}

void TreeNode::gen_decl(ostream &out,TreeNode *node){
    out<<endl<< "# define your veriables and temp veriables here" << endl;
    out << "\t.bss" << endl;
    dfs_gen_decl(out,node);

    for(int i=0;i<temp_var_seq;i++){
        out<<"t"<<i<<":"<<endl;
        out<<"\t.zero\t4"<<endl;
        out<<"\t.align\t4"<<endl;
    }
    out<<"\t.section\t.rodata"<<endl;
    dfs_gen_cons(out,node);
}

void TreeNode::recursive_gen_code(ostream &out,TreeNode *node){
    if(node==nullptr)
        return;
    if(node->nodeType==NODE_OP){
        expr_gen_code(out,node);
    }else if(node->nodeType==NODE_STMT||node->nodeType==NODE_PROG||node->nodeType==NODE_FUNC){
        stmt_gen_code(out,node);
    }
}

void TreeNode::stmt_gen_code(ostream &out,TreeNode *node){
    if(node==nullptr)
        return;
    if(node->nodeType==NODE_STMT){
        switch (node->stmtType)
        {
        case STMT_WHILE:
        {
            if(node->label.begin_label!="")
                out<<node->label.begin_label<<":"<<endl;
            recursive_gen_code(out,node->child);
            recursive_gen_code(out,node->child->sibling);
            out<<"\tjmp"<<node->label.begin_label<<endl;
            break;
        }
        case STMT_IF:
        { 
            break;
        }
        case STMT_PRINTF:
        {   TreeNode *io_stmt = node->child->child;
            int numchild=0;
            while(io_stmt!=nullptr){
                numchild++;
                io_stmt=io_stmt->sibling;
            }
            for(int i=numchild;i>0;i--){
                int t=i;
                TreeNode *temp_node=node->child->child;
                while(--t){
                    temp_node=temp_node->sibling;
                }
                if(temp_node->nodeType==NODE_VAR&&temp_node->varType==VAR_INTEGER){
                    out<<"\tmovl "<<"_"<<temp_node->belong_table->table_name<<"_"<<temp_node->var_name;
                    out<<",%edx"<<endl;
                    out<<"\tpushl %edx"<<endl;
                }else if(temp_node->nodeType==NODE_VAR&&temp_node->varType==VAR_CHAR){
                    out<<"\tmovzbl "<<"_"<<temp_node->belong_table->table_name<<"_"<<temp_node->var_name;
                    out<<",%edx"<<endl;
                    out<<"\tmovsbl %dl,%edx"<<endl;
                    out<<"\tpushl %edx"<<endl;
                }else {
                    out<<"\tpushl $LC"<<temp_node->temp_cons<<endl;
                }
            }
            out<<"\tcall printf"<<endl;
            out<<"\taddl $"<<numchild*4<<", %esp"<<endl;
            recursive_gen_code(out,node->sibling);
            break;
        }
        case STMT_SCANF:
        {   TreeNode *io_stmt = node->child->child;
            int numchild=0;
            while(io_stmt!=nullptr){
                numchild++;
                io_stmt=io_stmt->sibling;
            }
            for(int i=numchild;i>0;i--){
                int t=i;
                TreeNode *temp_node=node->child->child;
                while(--t){
                    temp_node=temp_node->sibling;
                }
                if(temp_node->nodeType==NODE_VAR&&temp_node->varType==VAR_INTEGER){
                    out<<"\tleal "<<"_"<<temp_node->belong_table->table_name<<"_"<<temp_node->var_name;
                    out<<",%edx"<<endl;
                    out<<"\tpushl %edx"<<endl;
                }else if(temp_node->nodeType==NODE_VAR&&temp_node->varType==VAR_CHAR){
                    out<<"\tleal "<<"_"<<temp_node->belong_table->table_name<<"_"<<temp_node->var_name;
                    out<<",%edx"<<endl;
                    out<<"\tpushl %edx"<<endl;
                }else {
                    out<<"\tpushl $LC"<<temp_node->temp_cons<<endl;
                }
            }
            out<<"\tcall scanf"<<endl;
            out<<"\taddl $"<<numchild*4<<", %esp"<<endl;
            recursive_gen_code(out,node->sibling);
            break;
        }
        case STMT_FOR:
        {

            break;
        }
        case STMT_ASSIGN:
        {
            if(node->child->sibling->nodeType==NODE_VAR||node->child->sibling->nodeType==NODE_CONST){
                if(node->child->sibling->nodeType==NODE_VAR&&node->child->sibling->varType==VAR_INTEGER){
                    out<<"\tmovl "<<"_" <<node->child->sibling->belong_table->table_name<<"_"<<node->child->sibling->var_name;
                    out<<", %edx"<<endl;
                    if(node->child->sibling->isNeg)
                        out<<"\tnegl %edx"<<endl;
                    out<<"\tmovl "<<"%edx";
                    out<<", "<<"_"<<node->child->belong_table->table_name<<"_"<<node->child->var_name<<endl;
                }else if(node->child->sibling->nodeType==NODE_VAR&&node->child->sibling->varType==VAR_CHAR){
                    out<<"\tmovzbl "<<"_" <<node->child->sibling->belong_table->table_name<<"_"<<node->child->sibling->var_name;
                    out<<", %edx"<<endl;
                    out<<"\tmovb "<<"%dl";
                    out<<", "<<"_"<<node->child->belong_table->table_name<<"_"<<node->child->var_name<<endl;
                }else if(node->child->sibling->nodeType==NODE_CONST&&node->child->sibling->consType==CONS_INTEGER){
                    out<<"\tmovl "<<"$"<<node->child->sibling->int_val;
                    out<<", "<<"_"<<node->child->belong_table->table_name<<"_"<<node->child->var_name<<endl;
                }else{
                    out<<"\tmovb "<<"$"<<(int)node->child->sibling->char_val;
                    out<<", "<<"_"<<node->child->belong_table->table_name<<"_"<<node->child->var_name<<endl;
                }
            }else if(node->child->sibling->nodeType==NODE_OP){
                expr_gen_code(out,node->child->sibling);             //丢给expr递归处理
                out<<"\tmovl "<<"t"<<node->child->sibling->temp_var;
                out<<" ,%edx"<<endl;
                if(node->child->sibling->isNeg)
                        out<<"\tnegl %edx"<<endl;
                out<<"\tmovl "<<"%edx";
                out<<", "<<"_"<<node->child->belong_table->table_name<<"_"<<node->child->var_name<<endl;
            }else if(node->child->sibling->nodeType==NODE_STMT){
                //对于a=b=c 连等情况 则递归，从右向左处理。
                stmt_gen_code(out,node->child->sibling);
                //可能会出现多重嵌套，已更改
                if(node->child->belong_table->findtype(node->child->belong_table,node->child->var_name)==VAR_INTEGER){
                    out<<"\tmovl "<<"_"<<node->child->sibling->child->belong_table->table_name<<"_"<<node->child->sibling->child->var_name;
                    out<<", %edx"<<endl;
                    out<<"\tmovl "<<"%edx";
                    out<<", "<<"_"<<node->child->belong_table->table_name<<"_"<<node->child->var_name<<endl;
                }else{
                    //对于类型的转换 记得改。。
                    out<<"\tmovzbl "<<"_"<<node->child->sibling->child->belong_table->table_name<<"_"<<node->child->sibling->child->var_name;
                    out<<", %edx"<<endl;
                    out<<"\tmovb "<<"%dl";
                    out<<", "<<"_"<<node->child->belong_table->table_name<<"_"<<node->child->var_name<<endl;
                }
            }
            recursive_gen_code(out,node->sibling);
            break;
        }
        case STMT_DECL:
        {
            if(node->child->sibling->nodeType==NODE_VAR){           //对于单个变量的定义
                if(node->child->sibling->sibling->nodeType==NODE_VAR||node->child->sibling->sibling->nodeType==NODE_CONST){
                    if(node->child->sibling->sibling->nodeType==NODE_VAR&&node->child->sibling->sibling->varType==VAR_INTEGER){
                        out<<"\tmovl "<<"_" <<node->child->sibling->sibling->belong_table->table_name<<"_"<<node->child->sibling->sibling->var_name;
                        out<<", %edx"<<endl;
                        if(node->child->sibling->sibling->isNeg)
                            out<<"\tnegl %edx"<<endl;
                        out<<"\tmovl "<<"%edx";
                        out<<", "<<"_"<<node->child->sibling->belong_table->table_name<<"_"<<node->child->sibling->var_name<<endl;
                    }else if(node->child->sibling->sibling->nodeType==NODE_VAR&&node->child->sibling->sibling->varType==VAR_CHAR){
                        out<<"\tmovzbl "<<"_" <<node->child->sibling->sibling->belong_table->table_name<<"_"<<node->child->sibling->sibling->var_name;
                        out<<", %edx"<<endl;
                        out<<"\tmovb "<<"%dl";
                        out<<", "<<"_"<<node->child->sibling->belong_table->table_name<<"_"<<node->child->sibling->var_name<<endl;
                    }else if(node->child->sibling->sibling->nodeType==NODE_CONST&&node->child->sibling->sibling->consType==CONS_INTEGER){
                        out<<"\tmovl "<<"$"<<node->child->sibling->sibling->int_val;
                        out<<", "<<"_"<<node->child->sibling->belong_table->table_name<<"_"<<node->child->sibling->var_name<<endl;
                    }else{
                        out<<"\tmovb "<<"$"<<(int)node->child->sibling->sibling->char_val;
                        out<<", "<<"_"<<node->child->sibling->belong_table->table_name<<"_"<<node->child->sibling->var_name<<endl;
                    }
                }else{ 
                    expr_gen_code(out,node->child->sibling->sibling);
                    out<<"\tmovl "<<"t"<<node->child->sibling->sibling->temp_var;
                    out<<" ,%edx"<<endl;
                    if(node->child->sibling->sibling->isNeg)
                            out<<"\tnegl %edx"<<endl;
                    out<<"\tmovl "<<"%edx";
                    out<<", "<<"_"<<node->child->sibling->belong_table->table_name<<"_"<<node->child->sibling->var_name<<endl;
                }
            }
            recursive_gen_code(out,node->sibling);
            break;
        }
        default:
            break;
        }
    }else if(node->nodeType==NODE_PROG){
        out<<node->label.begin_label<<":"<<endl;
        recursive_gen_code(out,node->child);
    }else if(node->nodeType==NODE_FUNC){
        recursive_gen_code(out,node->child->sibling);
        recursive_gen_code(out,node->sibling);          //如果有多个函数，就生成下一个函数的代码段
    }
    
}

void TreeNode::expr_gen_code(ostream &out,TreeNode *node){
    TreeNode *e1=node->child;
    TreeNode *e2=node->child->sibling;
    if(e1->nodeType==NODE_OP)
        expr_gen_code(out,e1);
    if(e2->nodeType==NODE_OP)
        expr_gen_code(out,e2);
    switch (node->opType)
    {
    case OP_ADD:
    {
        out << "\tmovl ";
		if (e1->nodeType == NODE_VAR)
			out << "_" <<e1->belong_table->table_name<<"_"<<e1->var_name;             
		else if (e1->nodeType == NODE_CONST)
			out<<e1->int_val;
		else out << "t" << e1->temp_var;
		out << ", %eax" <<endl;
        if(e1->isNeg)
            out<<"\tnegl %eax"<<endl;
        if(e2->isNeg){
            out<<"\tmovl ";
            if (e2->nodeType == NODE_VAR)
			out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;              
		else if (e2->nodeType == NODE_CONST)
			out <<"$"<<e2->int_val;    
		else out << "t" << e2->temp_var;
        out<<", %ecx"<<endl;
        out<<"\tnegl %ecx"<<endl;
        out<<"\taddl %ecx";
        }else{
		if (e2->nodeType == NODE_VAR){
            out << "\taddl ";
			out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;
        }               
		else if (e2->nodeType == NODE_CONST){
            out << "\taddl $";
			out <<e2->int_val;
        }
		else {
            out << "\taddl ";
            out << "t" << e2->temp_var;
        }
        }
		out << ", %eax" << endl;
		out << "\tmovl %eax, t" << node->temp_var << endl;
        break;
    }
    case OP_SUB:
    {
        out << "\tmovl ";
		if (e1->nodeType == NODE_VAR)
			out << "_" <<e1->belong_table->table_name<<"_"<<e1->var_name;             
		else if (e1->nodeType == NODE_CONST)
			out<<"$"<<e1->int_val;
		else out << "t" << e1->temp_var;
		out << ", %eax" <<endl;
        if(e1->isNeg)
            out<<"\tnegl %eax"<<endl;
        if(e2->isNeg){
            out<<"\tmovl ";
        if (e2->nodeType == NODE_VAR)
			out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;              
		else if (e2->nodeType == NODE_CONST)
			out <<"$"<<e2->int_val;    
		else out << "t" << e2->temp_var;
         out<<", %ecx"<<endl;
         out<<"\tnegl %ecx"<<endl;
         out<<"\tsubl %ecx";
        }else{
		out << "\tsubl ";
		if (e2->nodeType == NODE_VAR)
			out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;              
		else if (e2->nodeType == NODE_CONST)
			out <<"$"<<e2->int_val;    
		else out << "t" << e2->temp_var;
        }
		out << ", %eax" << endl;
		out << "\tmovl %eax, t" << node->temp_var << endl;
        break;
    }
    case OP_MUL:
    {
        out << "\tmovl ";
		if (e1->nodeType == NODE_VAR)
			out << "_" <<e1->belong_table->table_name<<"_"<<e1->var_name;             
		else if (e1->nodeType == NODE_CONST)
			out<<"$"<<e1->int_val;
		else out << "t" << e1->temp_var;
		out << ", %eax" <<endl;
        if(e1->isNeg)
            out<<"\tnegl %eax"<<endl;
        if(e2->isNeg){
            out<<"\tmovl ";
            if (e2->nodeType == NODE_VAR)
                out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;              
            else if (e2->nodeType == NODE_CONST)
                out <<"$"<<e2->int_val;    
            else out << "t" << e2->temp_var;
            out<<", %ecx"<<endl;
            out<<"\tnegl %ecx"<<endl;
            out<<"\timull %ecx";
        }else{
            out << "\timull ";
            if (e2->nodeType == NODE_VAR)
                out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;              
            else if (e2->nodeType == NODE_CONST)
                out <<"$"<<e2->int_val;    
            else out << "t" << e2->temp_var;
        }
		out << ", %eax" << endl;
		out << "\tmovl %eax, t" << node->temp_var << endl;
        break;
    }
    case OP_DIV:
    {
        out << "\tmovl ";
		if (e1->nodeType == NODE_VAR)
			out << "_" <<e1->belong_table->table_name<<"_"<<e1->var_name;             
		else if (e1->nodeType == NODE_CONST)
			out<<"$"<<e1->int_val;
		else out << "t" << e1->temp_var;
		out << ", %eax" <<endl;
        if(e1->isNeg)
            out<<"\tnegl %eax"<<endl;
        if(e2->isNeg){
            out<<"\tmovl ";
            if (e2->nodeType == NODE_VAR)
                out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;              
            else if (e2->nodeType == NODE_CONST)
                out <<"$"<<e2->int_val;    
            else out << "t" << e2->temp_var;
            out<<", %ecx"<<endl;
            out<<"\tnegl %ecx"<<endl;
            out<<"\tcltd"<<endl;
            out << "\tidivl %ecx"<<endl; 
        }else{
            out<<"\tcltd"<<endl;
            out << "\tidivl ";                   //对于除法只有单目
            if (e2->nodeType == NODE_VAR)
                out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;              
            else if (e2->nodeType == NODE_CONST)
                out <<"$"<<e2->int_val;    
            else out << "t" << e2->temp_var;
            out<<endl;
            }
            out << "\tmovl %eax, t" << node->temp_var << endl;
        break;
    }
    case OP_MOD:
    {
        out << "\tmovl ";
		if (e1->nodeType == NODE_VAR)
			out << "_" <<e1->belong_table->table_name<<"_"<<e1->var_name;             
		else if (e1->nodeType == NODE_CONST)
			out<<"$"<<e1->int_val;
		else out<< "t" << e1->temp_var;
		out << ", %eax" <<endl;
        if(e1->isNeg)
            out<<"\tnegl %eax"<<endl;
        if(e2->isNeg){
            out<<"\tmovl ";
            if (e2->nodeType == NODE_VAR)
                out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;              
            else if (e2->nodeType == NODE_CONST)
                out <<"$"<<e2->int_val;    
            else out << "t" << e2->temp_var;
            out<<", %ecx"<<endl;
            out<<"\tnegl %ecx"<<endl;
            out<<"\tcltd"<<endl;
            out << "\tidivl %ecx"<<endl; 
        }else{
            out<<"\tcltd"<<endl;
            out << "\tidivl ";                   //对于取模取的是除法的余数
            if (e2->nodeType == NODE_VAR)
                out << "_"<<e2->belong_table->table_name<<"_"<<e2->var_name;              
            else if (e2->nodeType == NODE_CONST)
                out <<"$"<<e2->int_val;    
            else out << "t" << e2->temp_var;
            out<<endl;
        }
		out << "\tmovl %edx, t" << node->temp_var << endl;
        break;
    }
    case OP_EQUAL:
    {

        break;
    }
    case OP_NEQUAL:
    {

        break;
    }
    case OP_MT:
    {

        break;
    }
    case OP_MTOE:
    {

        break;
    }
    case OP_LT:
    {

        break;
    }
    case OP_LTOE:
    {

        break;
    }
    default:
        break;
    }
}

void TreeNode::get_label(TreeNode *node){
    node->label.begin_label="main";
    recursive_get_label(node);
}

void TreeNode::gen_code(ostream &out,TreeNode *node){
    gen_header(out);
    
    TreeNode *p=node->child;
    //if(p->nodeType==NODE_STMT&&p->stmtType==STMT_DECL)    //打印变量的声明
    gen_decl(out,p);

    out<<endl<<endl<<"# your asm code here" << endl;
    out << "\t.text" << endl;
    out << "\t.globl main" << endl;
    recursive_gen_code(out, node);                        //打印
    if(node->label.next_label!="")
        out<<node->label.next_label<<":"<<endl;
    out<<"\tret"<<endl;

    out<<"	.section	.note.GNU-stack,\"\",@progbits"<<endl;
}