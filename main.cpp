#include"main.tab.hh"
#include"common.h"
#include <fstream>


using namespace std;
TreeNode *root=nullptr;
int main()
{
    yyparse();
    if(root){//若存在语法树结点
        root->genNodeId();//将整棵语法树赋予id
        root->printAST();//打印相关信息
    }
    return 0;
}
int yyerror(char const* message)
{
  cout << message << endl;
  return -1;
}