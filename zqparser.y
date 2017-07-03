%{

// hello

%}

%pure_parser

%token T_ARROW
%token T_LABEL

%left '.' ':' T_ARROW

%destructor { zfree($$.T_LABEL); } T_LABEL

%%

line
	:	T_LABEL												{ r0 $1 }
	|	query	 											{ r1 $1 }
;

query
	:	T_LABEL T_ARROW T_LABEL '.' T_LABEL ':' T_LABEL		{ r2 $1 $3 $5 $7 }
	|	query T_ARROW T_LABEL '.' T_LABEL ':' T_LABEL		{ r3 $1 $3 $5 $7 }
;

%%

int main()
{
	return 0;
}
