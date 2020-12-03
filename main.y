%{
    #include"common.h"
    extern TreeNode * root;
    int yylex();
    int yyerror( char const * );
%}
%defines

%start program

%token ID INTEGER CONSTRING
%token IF ELSE WHILE FOR
%token INT VOID CHAR
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA
%token TRUE FALSE


%token ASSIGN ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN MODASSIGN
%token OR A
%token AND
%token EQUAL NEQUAL
%token MT LT MTOE LTOE
%token ADD SUB
%token MUL DIV MOD
%token AA SS
%token NOT
%token PRINTF SCANF

%right ASSIGN
%left OR
%left AND
%left EQUAL NEQUAL
%left MT LT MTOE LTOE
%left ADD SUB
%left MUL DIV MOD
%right NOT
%right UMINUS UADD
%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE 
%%
program
    : statements {root=new TreeNode(NODE_PROG);root->addChild($1);}
    ;
statements
    : statement {$$=$1;}
    | statements statement{$$=$1;$$->addSibling($2);}
    ;
statement
    : instruction {$$=$1;}
    | if_else {$$=$1;}
    | while {$$=$1;}
    | for {$$=$1;}
    | LBRACE statements RBRACE {$$=$2;}
    | func_decl {$$=$1;}
    ;

func_decl
    : type ID LPAREN RPAREN statement{
        TreeNode *node=new TreeNode(NODE_FUNC);
        node->var_name=$2->var_name;
        node->addChild($1);
        node->addChild($5);
        $$=node;
    }

if_else
    : IF bool_statment statement %prec LOWER_THEN_ELSE {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_IF;
        node->addChild($2);
        node->addChild($3);
        $$=node;
    }
    | IF bool_statment statement ELSE statement {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_IF;
        node->addChild($2);
        node->addChild($3);
        node->addChild($5);
        $$=node;
    }
    ;
while
    : WHILE bool_statment statement {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_WHILE;
        node->addChild($2);
        node->addChild($3);
        $$=node;
    }
    ;
for
    : FOR for_statment statement {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_FOR;
        node->addChild($2);
        node->addChild($3);
        $$=node;
    }
for_statment
    : LPAREN instruction bool_expr SEMICOLON expr RPAREN {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_FOR_ST;
        node->addChild($2);
        node->addChild($3);
        node->addChild($5);
        $$=node;
    }
bool_statment
    : LPAREN bool_expr RPAREN {$$=$2;}
    ;
instruction
    : type ID ASSIGN expr SEMICOLON {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_DECL;
        node->addChild($1);
        node->addChild($2);
        node->addChild($4);
        $$=node;
    }
    | ID exassign expr SEMICOLON {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_ASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    | type IDlist SEMICOLON {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_DECL;
        node->addChild($1);
        node->addChild($2);
        $$=node;
    }
    | printf SEMICOLON {$$=$1;}
    | scanf SEMICOLON {$$=$1;}
    ;
IDlist
    : ID {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_IDLIST;
        node->addChild($1);
        $$=node;
    }
    | IDlist COMMA ID {
        $1->addChild($3);
        $$=$1;
    }

printf
    : PRINTF LPAREN io_stmt RPAREN {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_PRINTF;
        node->addChild($3);
        $$=node;
    }
    ;
scanf
    : SCANF LPAREN io_stmt RPAREN {
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_SCANF;
        node->addChild($3);
        $$=node;
    }
io_stmt
    : CONSTRING COMMA io_expr{
        TreeNode *node=new TreeNode(NODE_STMT);
        node->stmtType=STMT_IO;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | io_stmt COMMA io_expr {
        $1->addChild($3);
        $$=$1;
    }
io_expr
    : INTEGER {$$=$1;}
    | A ID {$$=$2;}
    | ID {$$=$1;}
    ;
bool_expr
    : TRUE {$$=$1;}
    | FALSE {$$=$1;}
    | expr EQUAL expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_EQUAL;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr NEQUAL expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_NEQUAL;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | NOT bool_expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_NOT;
        node->addChild($2);
        $$=node;        
    }
    | bool_expr AND bool_expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_AND;
        node->addChild($1);
        node->addChild($3);
        $$=node;     
    }
    | bool_expr OR bool_expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_OR;
        node->addChild($1);
        node->addChild($3);
        $$=node;     
    }
    | expr MT expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_MT;
        node->addChild($1);
        node->addChild($3);
        $$=node; 
    }
    | expr LT expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_LT;
        node->addChild($1);
        node->addChild($3);
        $$=node; 
    }
    | expr MTOE expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_MTOE;
        node->addChild($1);
        node->addChild($3);
        $$=node; 
    }
    | expr LTOE expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_LTOE;
        node->addChild($1);
        node->addChild($3);
        $$=node; 
    }
    ;
expr
    : ID {$$=$1;}
    | INTEGER {$$=$1;}
    | CONSTRING {$$=$1;}
    | ID AA {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_AA;
        node->addChild($1);
        $$=node;
    }
    | ID SS {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_SS;
        node->addChild($1);
        $$=node;
    }
    | AA ID {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_AA;
        node->addChild($2);
        $$=node;
    }
    | SS ID {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_SS;
        node->addChild($2);
        $$=node;
    }
    | expr ADD expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_ADD;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | expr SUB expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_SUB;
        node->addChild($1);
        node->addChild($3);
        $$=node; 
    }
    | expr MUL expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_MUL;
        node->addChild($1);
        node->addChild($3);
        $$=node; 
    }
    | expr DIV expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_DIV;
        node->addChild($1);
        node->addChild($3);
        $$=node; 
    }
    | expr MOD expr {
        TreeNode *node=new TreeNode(NODE_OP);
        node->opType=OP_MOD;
        node->addChild($1);
        node->addChild($3);
        $$=node; 
    }
    | SUB expr %prec UMINUS {
        $2->int_val=-$2->int_val;
        $$=$2;
    }
    | ADD expr %prec UADD {
        $$=$2;
    }
    ;
type
    : INT {
        TreeNode *node=new TreeNode(NODE_TYPE);
        node->varType=VAR_INTEGER;
        $$=node; 
    }
    | VOID {
        TreeNode *node=new TreeNode(NODE_TYPE);
        node->varType=VAR_VOID;
        $$=node;         
    }
    | CHAR {
        TreeNode *node=new TreeNode(NODE_TYPE);
        node->varType=VAR_CHAR;
        $$=node;         
    }
    ;
exassign
    : ASSIGN | ADDASSIGN | SUBASSIGN | MULASSIGN | DIVASSIGN | MODASSIGN
    ;
%%