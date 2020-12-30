#include"main.tab.hh"
#include"common.h"
#include <fstream>


using namespace std;

extern FILE *yyin;
TreeNode *root=nullptr;
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        FILE *fin = fopen(argv[1], "r");
        if (fin != nullptr)
        {
            yyin = fin;
        }
        else
        {
            cerr << "failed to open file: " << argv[1] << endl;
        }
    }
    yyparse();
    if(root != NULL) {
        root->genNodeId();
        root->printAST();
    }
    root->Type_Check(root);//类型检查
    return 0;
}
int yyerror(char const* message)
{
  cout << message << endl;
  return -1;
}