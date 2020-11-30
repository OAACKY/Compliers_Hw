#include "tree.h"

void TreeNode::addChild(TreeNode *temp){
    if(this->child==nullptr)
        this->child=temp;
    else{
        TreeNode *tempNode;
        tempNode=this;
        while(tempNode->sibling!=nullptr){
            tempNode=tempNode->sibling;
        }
        tempNode->sibling=temp;
    }
}

void TreeNode::addSibling(TreeNode *temp){
    this->sibling=temp; 
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
}

void printDfs(TreeNode *node){
    while(node!=nullptr){
        node->printNodeInfo();
        if(node->child!=nullptr)
            node->child->printNodeConnection();
        dfs(node->child);
        node=node->sibling;
    }
}

void TreeNode::printAST(){
    printDfs(this);
}

void TreeNode::printNodeInfo(){
    cout<<"@"<<this->nodeID<<" ";
    cout<<this->nodeTypeInfo();
}
void TreeNode::printNodeConnection(){

}

string TreeNode::nodeTypeInfo(){
    switch (this->nodeType)
    {
    case NODE_PROG:
        
        break;
    case NODE_STMT:

        break;
    case NODE_CONST:

        break;
    case NODE_BOOL:

        break;
    case NODE_EXPR:

        break;
    case NODE_OP:

        break;
    case NODE_VAR:

        break;
    case NODE_TYPE:

        break;
    default:
        return "";
        break;
    }
}

string TreeNode::opTypeToString(OpType type){
    return "";
}

string TreeNode::nodeTypeToString(NodeType type){
    return "";
}

string TreeNode::sTypeToString(StmtType type){
    return "";
}

TreeNode::TreeNode(NodeType mytype){
    this->nodeType=mytype;
}