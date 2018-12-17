
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


struct opcode_ent {
	char *  name;
	uint8_t code;
};

static const struct opcode_ent opcode_table[] = {
	{ "noop", TOKEN_OPCODE_NOOP },
	{ "add" , TOKEN_OPCODE_ADD  },
	{ "movl", TOKEN_OPCODE_MOVL },
	{ "movh", TOKEN_OPCODE_MOVH },
	{ "ld"  , TOKEN_OPCODE_LD   },
	{ "sw"  , TOKEN_OPCODE_SW   },
	{ "beq" , TOKEN_OPCODE_BEQ  },
	{ "jmp" , TOKEN_OPCODE_JMP  },
	{ "jr"  , TOKEN_OPCODE_JR   },
	{ "int" , TOKEN_OPCODE_INT  },
	{ NULL  , 0                 },
};

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

static int read_hex(FILE *fp, int *out) {

	int c, val = 0;

	while((c = fgetc(fp)) != EOF) {
		char n = 0;
		if (number(c)) {
			n = c - '0';
		}
		else if (  (c >= 'a' && c <= 'f')
			|| (c >= 'A' && c <= 'F')) {
			n = (c % 0x20) + 9;
		}
		else {
			ungetc(c, fp);
			break;
		}

		val = (val * 16) + n;
		if (val > 0xFF)
			goto overflow;
	}
	*out = val;
	return 0;

overflow:
	*out = 0xFF;
	return -1;
}

static int read_dec(FILE *fp, int neg, int *out) {

	int c, val = 0;

	while((c = fgetc(fp)) != EOF) {
		if (!number(c)) {
			ungetc(c, fp);
			break;
		}
		val = (val * 10) + (c - '0');

		// Cool trick here.
		// because the range is -128 (0x80) to +127 (0x7F)
		// We can do 0x80 - 1 if it is NOT a negative number.
		if (val > (0x80 - !neg))
			goto overflow;
	}

	*out = neg ? -1 * val : val;
	return 0;

overflow:
	*out = neg ? -1 * 0x80 : 0x7F;
	return -1;
}

static int read_number(FILE *fp, int *out) {

	int neg = 0, c = fgetc(fp);

	// Check for '0x'.
	if (c == '0') {
		c = fgetc(fp);
		if (c == 'x') {
			// We have a hexadecimal number.
			return read_hex(fp, out);
		}
		ungetc(c, fp);
		ungetc('0', fp);
	}
	// While we are at it. check for a negative sign.
	else if (c == '-') {
		neg = 1;
	}
	// We got something else. put it back.
	else {
		ungetc(c, fp);
	}

	return read_dec(fp, neg, out);
}

static int parse_number(struct lexer *lex) {

	int num;

	if (read_number(lex->fp, &num) < 0)
		fprintf(stderr, "WARNING: Value truncated on line: %i\n", lex->lineno);

	lex->token.value.n = num;
	return 0;
}

static int read_string(FILE *fp, char *buf, size_t len) {

	int c, label_decl = 0, i = 0;

	while((c = fgetc(fp)) != EOF && i < len) {

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

	if (label_decl)
		return TOKEN_LABEL_DECL;

	for(i = 0; opcode_table[i].name; i++) {

		if (!strcmp(opcode_table[i].name, buf))
			return opcode_table[i].code;
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
		if (parse_number(lex) < 0)
			return -1;
		break;
	default:
		if (first_number(ch)) {
			ungetc(ch, lex->fp);
			lex->token.type = TOKEN_NUMBER;
			if (parse_number(lex) < 0)
				return -1;
		} else if (first_string(ch)) {
			char buf[32];
			ungetc(ch, lex->fp);
			lex->token.type = read_string(lex->fp, buf, sizeof(buf));
			if (lex->token.type == TOKEN_LABEL_DECL || lex->token.type == TOKEN_LABEL)
				strcpy(lex->token.value.s, buf);
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
	case TOKEN_LABEL : printf(" [LABEL \"%s\"] ", token->value.s);
		break;
	case TOKEN_LABEL_DECL : printf(" [LABEL DECL \"%s\"] ", token->value.s);
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
