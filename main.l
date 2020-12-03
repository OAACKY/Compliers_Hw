%option noyywrap
%{
    #include"common.h"
    #include"main.tab.hh"
%}

mulcommentbegin "/*"
sincommentbegin "//"
commentelement     .|\n
sincommentelement  .
mulcommentend   "*/"
sincommentend   \n
%x MULCOMMENT
%x SINCOMMENT

INTEGER [0-9]+
ID [[:alpha:]_][[:alpha:][:digit:]_]*

CONSTRING \"([^\"]*)\"

EOL (\r\n|\n)
WHITE [\t ]
%%

{mulcommentbegin} {BEGIN MULCOMMENT;}
{sincommentbegin} {BEGIN SINCOMMENT;}
<MULCOMMENT>{commentelement} {}
<MULCOMMENT>{mulcommentend} {BEGIN INITIAL;}
<SINCOMMENT>{sincommentelement} {}
<SINCOMMENT>{sincommentend}   {BEGIN INITIAL;}

"true" {
    TreeNode *node = new TreeNode(NODE_BOOL);
    node->bool_val = true;
    yylval = node;
    return TRUE;
}
"false" {
    TreeNode *node = new TreeNode(NODE_BOOL);
    node->bool_val = false;
    yylval = node;
    return FALSE;
}


"int" return INT;
"void" return VOID;
"char" return CHAR;


"if" return IF;
"while" return WHILE;
"else" return ELSE;
"for" return FOR;

"printf" return PRINTF;
"scanf" return SCANF;

"=" return ASSIGN;
"+=" return ADDASSIGN;
"-=" return SUBASSIGN;
"*=" return MULASSIGN;
"/=" return DIVASSIGN;
"%=" return MODASSIGN;

"+" return ADD;
"-" return SUB;
"*" return MUL;
"/" return DIV;
"%" return MOD;

"++" return AA;
"--" return SS;


"!" return NOT;
"&" return A;

"&&" return AND;
"||" return OR;

"==" return EQUAL;
"!=" return NEQUAL;
">"  return MT;
"<"  return LT;
">=" return MTOE;
"<=" return LTOE;

"," return COMMA;
";" return SEMICOLON;
"(" return LPAREN;
")" return RPAREN;
"{" return LBRACE;
"}" return RBRACE;

{CONSTRING} {
    TreeNode *node = new TreeNode(NODE_CONST);
    string tempString=string(yytext);
    int temp=1;
    while(temp!=tempString.length()-1){
        node->str_val+=yytext[temp];
        temp++;
    }
    yylval = node;
    node->consType = CONS_STRING;
    return CONSTRING;
}


{INTEGER} {
    TreeNode *node = new TreeNode(NODE_CONST);
    node->int_val = atoi(yytext);
    node->consType = CONS_INTEGER;
    yylval = node;
    return INTEGER;
}
{ID} {
    TreeNode *node = new TreeNode(NODE_VAR);
    node->var_name = string(yytext);
    yylval = node;
    return ID;
}
{EOL}
{WHITE}

%%