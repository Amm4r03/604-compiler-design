%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    // AST node types
    typedef enum { NODE_ID, NODE_ASSIGN, NODE_EXPR_PLUS } NodeType;
    
    typedef struct ASTNode {
        NodeType type;
        char *value;
        struct ASTNode *left, *right;
    } ASTNode;
    
    // Helper to create AST nodes
    ASTNode* create_node(NodeType type, char* value, ASTNode* left, ASTNode* right) {
        ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
        node->type = type;
        node->value = value ? strdup(value) : NULL;
        node->left = left;
        node->right = right;
        return node;
    }
    
    void print_ast(ASTNode* node, int level) {
        if (!node) return;
        for (int i = 0; i < level; i++) printf("  ");
        switch (node->type) {
            case NODE_ID: printf("ID(%s)\n", node->value); break;
            case NODE_ASSIGN: printf("ASSIGN\n"); break;
            case NODE_EXPR_PLUS: printf("PLUS\n"); break;
        }
        print_ast(node->left, level + 1);
        print_ast(node->right, level + 1);
    }
    
    int yylex();
    void yyerror(const char* s) { fprintf(stderr, "Parse error: %s\n", s); }
    %}
    
    %union {
        char* id;
        struct ASTNode* ast;
    }
    
    %token <id> ID
    %left '+' '='
    %type <ast> stmt expr
    
    %%
    
    stmt: ID '=' expr ';' {
        $$ = create_node(NODE_ASSIGN, NULL, create_node(NODE_ID, $1, NULL, NULL), $3);
        printf("Parsed AST:\n");
        print_ast($$, 0);
    }
    ;
    
    expr: expr '+' ID {
        $$ = create_node(NODE_EXPR_PLUS, NULL, $1, create_node(NODE_ID, $3, NULL, NULL));
    }
    | ID {
        $$ = create_node(NODE_ID, $1, NULL, NULL);
    }
    ;
    
    %%
    
int main() {
    printf("Enter expression:\n> ");
    return yyparse();  // Starts the parser, which reads from stdin via the lexer
}
