#include "zqparser.h"														// pf_ziminski (c) 2009
#include <stdlib.h>
#include "utstrings.h"
#include "zdbg.h"

/*
z-query language - схематичный язык запросов

tid->dc_pacts.id:nmb
tid->dc_pacts.id:cid->dc_contractors.id:name


ROW
	: QUERY . $end
;

QUERY
	: T_LABEL										{ $$.id = m.$1; $$.from = "main_table AS m" }
	| QUERY '->' T_LABEL '.' T_LABEL ':' T_LABEL	{ $$.id = jX.$7; $$.from .= LEFT JOIN $3 AS jX ON jX.$5 = $1.id }
;

SELECT j2.name FROM m LEFT JOIN dc_pacts AS j1 ON m.tid = j1.id LEFT JOIN dc_contractors AS j2 ON j1.cid = j2.id


line
	: T_LABEL T_EOF
	| query T_EOF
;

query
	| T_LABEL -> T_LABEL . T_LABEL : T_LABEL
	: query -> T_LABEL . T_LABEL : T_LABEL
;
*/

#define MC_STACK_SIZE		256

#define T_LABEL		0
#define T_ARROW		1
#define T_DOT		2
#define T_COLON		3
#define T_EOF		4
#define LEX_ERROR	255

typedef struct _ZZVAL
{
	char *nstr;
	char *jstr;
} ZZVAL;

int zzlex(ZZVAL *zzval, char *data, unsigned long *ip)
{
	unsigned long sp;
	char temp_str[1024];
	unsigned long p;

	zzval->nstr = NULL;
	zzval->jstr = NULL;
	p = 0;

	//ZTRACE("lexer:\n");

//lb_start:

	sp = *ip;

	//ZTRACE("%s - %c-%d\n", data, data[*ip], *ip);
	switch(data[*ip])
	{
		case '-': (*ip)++; goto arrow1;
		case '.': (*ip)++; return T_DOT;
		case ':': (*ip)++; return T_COLON;
		case '\0': return T_EOF;
	}

	while((p < 1024) && data[*ip] && (data[*ip] != '-') && (data[*ip] != '>') && (data[*ip] != '.') && (data[*ip] != ':'))
	{
		temp_str[p++] = data[*ip];
		(*ip)++;
	}

	temp_str[p++] = 0;

	zzval->nstr = alloc_string(temp_str);

	return T_LABEL;

arrow1:
	switch(data[*ip])
	{
		case '>': (*ip)++; return T_ARROW;
	}

	goto lb_error;

lb_error:

	return LEX_ERROR;
}

// return:	HR_OK    - result value = (char **) col - result of calculation
//          1        - result value = (char **) col - error message
//          HR_ERROR - function not registered
void *my_query_parse(char *query, char **col, char **join, unsigned long *j)
{
	static const unsigned char states[18][5] =
	{
		{	1,	0,	0,	0,	0	},	// 0
		{	0,	2,	0,	0,	8	},	// 1
		{	3,	0,	0,	0,	0	},	// 2
		{	0,	0,	4,	0,	0	},	// 3
		{	5,	0,	0,	0,	0	},	// 4
		{	0,	0,	0,	6,	0	},	// 5
		{	7,	0,	0,	0,	0	},	// 6
		{	202,202,202,202,202	},	// 7
		{	200,200,200,200,200	},	// 8
		{	255,255,255,255,255	},	// 9
		{	0,	12,	0,	0,	11	},	// 10
		{	201,201,201,201,201	},	// 11
		{	13,	0,	0,	0,	0	},	// 12
		{	0,	0,	14,	0,	0	},	// 13
		{	15,	0,	0,	0,	0	},	// 14
		{	7,	0,	0,	16,	0	},	// 15
		{	17,	0,	0,	0,	0	},	// 16
		{	203,203,203,203,203	}	// 17
	};

	static const unsigned char go[18][2] =
	{
		{	9,	10	},	// 0
		{	0,	0	},	// 1
		{	0,	0	},	// 2
		{	0,	0	},	// 3
		{	0,	0	},	// 4
		{	0,	0	},	// 5
		{	0,	0	},	// 6
		{	0,	0	},	// 7
		{	0,	0	},	// 8
		{	0,	0	},	// 9
		{	0,	10	},	// 10
		{	0,	0	},	// 11
		{	0,	0	},	// 12
		{	0,	0	},	// 13
		{	0,	0	},	// 14
		{	0,	0	},	// 15
		{	0,	0	},	// 16
		{	0,	0	}	// 17
	};

	static const unsigned char noneterm[4] =
	{
		{	0	},	// 0
		{	0	},	// 0
		{	1	},	// 0
		{	1	}	// 0
	};

	static const unsigned char rulelen[4] =
	{
		{	2	},	// 0
		{	2	},	// 0
		{	7	},	// 0
		{	7	}	// 0
	};

	ZZVAL vstack[MC_STACK_SIZE];
	ZZVAL *vsp;
	ZZVAL result;
	ZZVAL zzval;

	char sstack[MC_STACK_SIZE];

	char *ssp;
	unsigned long eip;

	unsigned char act;
	unsigned char nst;
	unsigned char rlen;
	unsigned char zzterm;

	//unsigned long reduce_i;
	char reduce_temp_str[33];

	sstack[0] = 0;

	eip = 0;
	ssp = sstack;
	vsp = vstack;

	zzterm = zzlex(&zzval, query, &eip);
	if(zzterm == LEX_ERROR)
	{
		goto lb_error;
	}

lb_loop:
	nst = states[*ssp][zzterm];
	//act = action[*ssp][zzterm];
	act = nst;

	ZTRACE("states[%d][%d] -> %d\n", *ssp, zzterm, nst);

	if(act == 255) // accept
	{
		ZTRACE("accept (result %d)\n", (long) vsp[0].nstr);
		*col = vsp[0].nstr;
		*join = vsp[0].jstr;
		// pop(result);
		return (void *) -1;
	}

	if(act >= 200) // reduce
	{
		rlen = rulelen[nst-200];
		result.nstr = vsp[1-rlen].nstr;
		result.jstr = vsp[1-rlen].jstr;

		//ZTRACE("reduce %d ", nst);
		switch(nst)
		{
			case 200:
				result.jstr = nullstring;
				result.nstr = alloc_string("m.`");
				alloc_strcat(&result.nstr, vsp[-1].nstr);
				alloc_strcat(&result.nstr, "`");
				free_str(vsp[-1].nstr);
				break;
			case 201:
				result.nstr = vsp[-1].nstr;
				result.jstr = vsp[-1].jstr;
				//free_str(vsp[-1].nstr);
				//free_str(vsp[-1].jstr);
				break;
			case 202:
				ZTRACE("reduce: query: T_LABEL -> T_LABEL . T_LABEL : T_LABEL :%s\n", vsp[-6].nstr);// (long) vsp[1].fval
				(*j)++;
				_ultoa(*j, reduce_temp_str, 10);
				result.nstr = alloc_string("j");
				alloc_strcat(&result.nstr, reduce_temp_str);
				alloc_strcat(&result.nstr, ".`");
				alloc_strcat(&result.nstr, vsp[0].nstr);
				alloc_strcat(&result.nstr, "`");
				result.jstr = alloc_string(" LEFT JOIN ");
				alloc_strcat(&result.jstr, vsp[-4].nstr);
				alloc_strcat(&result.jstr, " AS j");
				alloc_strcat(&result.jstr, reduce_temp_str);
				alloc_strcat(&result.jstr, " ON j");
				alloc_strcat(&result.jstr, reduce_temp_str);
				alloc_strcat(&result.jstr, ".`");
				alloc_strcat(&result.jstr, vsp[-2].nstr);
				alloc_strcat(&result.jstr, "` = m.`");
				alloc_strcat(&result.jstr, vsp[-6].nstr);
				alloc_strcat(&result.jstr, "`");
				free_str(vsp[0].nstr);
				free_str(vsp[-2].nstr);
				free_str(vsp[-4].nstr);
				free_str(vsp[-6].nstr);
				break;
			case 203:
				ZTRACE("reduce: query: query -> T_LABEL . T_LABEL : T_LABEL :%s\n", vsp[-6].nstr);// (long) vsp[1].fval
				//result.nstr = "jX." vsp[0].nstr
				//result.jstr += " LEFT JOIN vsp[-4].nstr AS jX ON jX.vsp[-2].nstr = vsp[-6].nstr"
				(*j)++;
				_ultoa(*j, reduce_temp_str, 10);
				result.nstr = alloc_string("j");
				alloc_strcat(&result.nstr, reduce_temp_str);
				alloc_strcat(&result.nstr, ".`");
				alloc_strcat(&result.nstr, vsp[0].nstr);
				alloc_strcat(&result.nstr, "`");
				result.jstr = vsp[-6].jstr;
				alloc_strcat(&result.jstr, " LEFT JOIN ");
				alloc_strcat(&result.jstr, vsp[-4].nstr);
				alloc_strcat(&result.jstr, " AS j");
				alloc_strcat(&result.jstr, reduce_temp_str);
				alloc_strcat(&result.jstr, " ON j");
				alloc_strcat(&result.jstr, reduce_temp_str);
				alloc_strcat(&result.jstr, ".`");
				alloc_strcat(&result.jstr, vsp[-2].nstr);
				alloc_strcat(&result.jstr, "` = ");
				alloc_strcat(&result.jstr, vsp[-6].nstr);
				free_str(vsp[0].nstr);
				free_str(vsp[-2].nstr);
				free_str(vsp[-4].nstr);
				free_str(vsp[-6].nstr);
				break;
		}

		ssp -= rlen;
		vsp -= rlen;

		ssp++;
		vsp++;

		if(ssp >= sstack + MC_STACK_SIZE)
		{
			goto lb_overflow;
		}

		vsp->nstr = result.nstr;
		vsp->jstr = result.jstr;

		ZTRACE("(go[%d][%d]) and shift %d\n", ssp[-1], noneterm[nst-200], go[ssp[-1]][noneterm[nst-200]]);

		*ssp = go[ssp[-1]][noneterm[nst-200]];

		//ZTRACE("(go[%d][%d]) ", ssp[-1], noneterm[nst]);
		//ZTRACE("and shift %d\n", *ssp);

		goto lb_loop;
	}
	
	if(act > 0) // shift
	{
		//ZTRACE("shift %d\n", nst);
		ssp++;
		vsp++;

		if(ssp >= sstack + MC_STACK_SIZE)
		{
			goto lb_overflow;
		}

		*vsp = zzval; // push terminal
		*ssp = nst; // push new state

		zzterm = zzlex(&zzval, query, &eip);
		if(zzterm == LEX_ERROR)
		{
			goto lb_error;
		}

		goto lb_loop;
	}

lb_error:
	ZTRACE("qparser: parse error\n");
	*col = "Syntax error";
	return (void *) -2;

lb_overflow:
	ZTRACE("qparser: stack overflow\n");
	*col = "Stack overflow";
	return (void *) -2;
}
