#include"main.tab.hh"
#include"common.h"
#include <fstream>


using namespace std;

extern FILE *yyin;
TreeNode *root=nullptr;
ostream *asm_out;
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
    } else if(argc == 3){
        yyin = fopen(argv[1],"r");
        asm_out = new ofstream(argv[2]);

    }
    yyparse();
    if(root != NULL) {
        root->genNodeId();
        root->printAST();
    }
    //root->Type_Check(root);类型检查放在生成节点标号之后
    //cout<<root->temp_var_seq<<endl; //输出测试中间变量数目
    root->get_label(root);
    //root->gen_code(cout,root);

    return 0;
}
int yyerror(char const* message)
{
  cout << message << endl;
  return -1;
}