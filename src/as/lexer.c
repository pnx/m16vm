
#include <stdio.h>
#include <string.h>
#include "lexer.h"

/**
 * macros for the grammar.
 */

// Numbers is defined as [0-9]
#define number(x) ((x) >= '0' && (x) <= '9')

// The first digit can however also contain '-'
#define first_number(x) (number(x) || (x) == '-' )

// First character in strings can be [a-z][A-Z] or '_'
#define first_string(x)			\
	(  ((x) >= 'a' && (x) <= 'z')   \
	|| ((x) >= 'A' && (x) <= 'Z')	\
	||  (x) == '_'			)

// All characters after can also include numbers or ':'
#define string(x) \
	(first_string(x) || number(x))

#define space(x) ((x) == ' ' || (x) == '\t' || (x) == '\r')


/**
 * Helper functions
 */

static int read_next(struct lexer *lex) {

 	int c, comment = 0;

 	while((c = fgetc(lex->fp)) != EOF) {

 		if (c == '\n')
			break;

 		if (comment)
 			continue;

 		if (c == ';') {
 			comment = 1;
 		} else if (!space(c)) {
 			break;
 		}
 	}
 	return c;
 }

static int read_number(FILE *fp) {

	int c, neg = 0, val = 0;
	while((c = fgetc(fp)) != EOF) {
		if (neg == 0 && c == '-') {
			neg = 1;
			continue;
		}
		if (!number(c)) {
			ungetc(c, fp);
			break;
		}
		val = (val * 10) + (c - '0');
	}

	if (neg)
		return -1 * val;
	return val;
}

static int read_string(FILE *fp) {

	int c, label_decl = 0, i = 0;
	char buf[64];

	while((c = fgetc(fp)) != EOF && i < 64) {

		if (string(c)) {
			buf[i++] = c;
		} else {
			if (c == ':') {
				label_decl = 1;
			} else {
				ungetc(c, fp);
			}
			break;
		}
	}
	buf[i] = '\0';

	if (label_decl) {
		return TOKEN_LABEL_DECL;
	} else if (!strcmp("noop", buf)) {
		return TOKEN_OPCODE_NOOP;
	} else if (!strcmp("add", buf)) {
		return TOKEN_OPCODE_ADD;
	} else if (!strcmp("movl", buf)) {
		return TOKEN_OPCODE_MOVL;
	} else if (!strcmp("movh", buf)) {
		return TOKEN_OPCODE_MOVH;
	} else if (!strcmp("ld", buf)) {
		return TOKEN_OPCODE_LD;
	} else if (!strcmp("sw", buf)) {
		return TOKEN_OPCODE_SW;
	} else if (!strcmp("beq", buf)) {
		return TOKEN_OPCODE_BEQ;
	} else if (!strcmp("jmp", buf)) {
		return TOKEN_OPCODE_JMP;
	} else if (!strcmp("jr", buf)) {
		return TOKEN_OPCODE_JR;
	} else if (!strcmp("int", buf)) {
		return TOKEN_OPCODE_INT;
	}
	return TOKEN_LABEL;
}

/**
 * Exposed functions
 */

void lexer_init(struct lexer *lex, FILE *fp) {

	lex->lineno = 1;
	lex->fp = fp;
	lex->token.type = TOKEN_EOI;
}

int lexer_get_next(struct lexer *lex) {

	uint16_t num;
	int ch = read_next(lex);

	if (lex->token.type == TOKEN_EOL)
		lex->lineno++;

	switch(ch) {
	case EOF : lex->token.type = TOKEN_EOI;
		break;
	case '\n' :
		lex->token.type = TOKEN_EOL;
		break;
	case ',' : lex->token.type = TOKEN_ARG_SEP;
		break;
	case '$' :
		lex->token.type = TOKEN_REG;
		num = read_number(lex->fp);
		// Registers is 8-bit only.
		if (num > 0xF) {
			fprintf(stderr, "ERROR: Invalid register value '%i' on line: %i\n", num, lex->lineno);
			return -1;
		}
		lex->token.value.n = num;
		break;
	default:
		if (first_number(ch)) {
			ungetc(ch, lex->fp);
			lex->token.type = TOKEN_NUMBER;
			lex->token.value.n = read_number(lex->fp);
		} else if (first_string(ch)) {
			ungetc(ch, lex->fp);
			lex->token.type = read_string(lex->fp);
		} else {
			fprintf(stderr, "ERROR: Invalid character '%c' on line: %i\n", ch, lex->lineno);
			return -1;
		}
	}

	lex->token.lineno = lex->lineno;
	return 0;
}

void lexer_print_token(struct token *token) {

	static int lineno = 0;

	if (token->lineno != lineno) {
		lineno = token->lineno;
		printf("\n%i: ", lineno);
	}

	switch(token->type) {
	case TOKEN_OPCODE_NOOP : printf(" [OP NOOP] ");
		break;
	case TOKEN_OPCODE_ADD : printf(" [OP ADD] ");
		break;
	case TOKEN_OPCODE_MOVL : printf(" [OP MOVL] ");
		break;
	case TOKEN_OPCODE_MOVH : printf(" [OP MOVH] ");
		break;
	case TOKEN_OPCODE_LD : printf(" [OP LD] ");
		break;
	case TOKEN_OPCODE_SW : printf(" [OP SW] ");
		break;
	case TOKEN_OPCODE_BEQ : printf(" [OP BEQ] ");
		break;
	case TOKEN_OPCODE_JMP : printf(" [OP JMP] ");
		break;
	case TOKEN_OPCODE_JR : printf(" [OP JR] ");
		break;
	case TOKEN_OPCODE_INT : printf(" [OP INT] ");
		break;
	case TOKEN_LABEL : printf(" [LABEL] ");
		break;
	case TOKEN_LABEL_DECL : printf(" [LABEL DECL] ");
		break;
	case TOKEN_REG : printf(" [REG %i] ", token->value.n);
		break;
	case TOKEN_ARG_SEP : printf(" [SEP] ");
		break;
	case TOKEN_NUMBER : printf(" [NUM %i] ", token->value.n);
		break;
	case TOKEN_EOI : printf(" [EOI] ");
		break;
	case TOKEN_EOL : printf(" [EOL] ");
		break;
	default: printf(" [U] ");
	}
}
