
#include <string.h>
#include <stdlib.h>
#include <vector.h>
#include <stdio.h>
#include "ast.h"

void ast_init(struct ast* ast) {

	vector_init(&ast->instr, 16 * sizeof(struct ast));
	vector_init(&ast->labels, 16 * sizeof(char**));
	ast->symbols = symtab_init();
}

void ast_free(struct ast* ast) {

	int i;

	// Free all label strings
	for(int i = 0; i < ast->labels.size; i++) {
		char *ptr = *((char**) ast->labels.base + i);
		free(ptr);
	}
	vector_destory(&ast->labels);
	vector_destory(&ast->instr);
	symtab_free(ast->symbols);
}

void ast_instr(struct ast* ast, uint8_t opcode) {

	struct ast_instr instr = { 0 };

	instr.opcode = opcode;
	vector_append(&ast->instr, &instr, sizeof(struct ast_instr));
}

void ast_instr_operand(struct ast* ast, enum ast_datatype type, void* value) {

	struct ast_instr *instr;

	if (ast->instr.size < 1)
		return;

	// Fetch latest instruction.
	instr = ast->instr.base + (ast->instr.size - sizeof(struct ast_instr));

	if (instr->n_operands < 3) {
		struct ast_instr_operand *n = instr->operands + instr->n_operands;

		n->type = type;
		if (n->type == DATATYPE_STRING) {
			n->s = strdup(value);
			vector_append(&ast->labels, &n->s, sizeof(char**));
		} else {
			n->r = ((intptr_t) value) & 0xFF;
		}

		instr->n_operands++;
	}
}

void ast_location(struct ast* ast, const char *label, uint16_t loc) {

	symtab_set(ast->symbols, label, loc);
}

void ast_print(struct ast* ast) {

	printf("{\n");

	for(int i = 0; i < ast->instr.size; i += sizeof(struct ast_instr)) {
		struct ast_instr *instr = ast->instr.base + i;

		printf("\topcode: %u", instr->opcode);
		for(int j = 0; j < instr->n_operands; j++) {
			struct ast_instr_operand *op = instr->operands + j;
			printf(", operand%i: ", j);
			switch(op->type) {
			case DATATYPE_REGISTER :
				printf("%u (reg)", op->r);
				break;
			case DATATYPE_NUMBER :
				printf("%i (num)", op->n);
				break;
			case DATATYPE_STRING :
				printf("%s (string)", op->s);
				break;
			}
		}
		printf("\n");
	}

	printf("}\n");

}
