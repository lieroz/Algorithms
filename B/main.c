#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define INVALID_INPUT 0

#define MULTIPLIER_COEF 2
#define compare(a, b) a < b ? a : b

#define DECLARE_STACK(TYPE)         \
	typedef struct {                \
		TYPE* base;                 \
		TYPE* next_item;            \
									\
		size_t base_size;           \
		size_t container_size;      \
	} STACK_##TYPE;                 \
\
	STACK_##TYPE* STACK_CONSTRUCT_##TYPE() {                                \
		STACK_##TYPE* self = (STACK_##TYPE*) malloc(sizeof(STACK_##TYPE));  \
																			\
		if (self == NULL) {                                                 \
			return NULL;                                                    \
		}                                                                   \
																			\
		self->base = NULL;                                                  \
		self->next_item = NULL;                                             \
		self->base_size = 0;                                                \
		self->container_size = 0;                                           \
																			\
		return self;                                                        \
	}                                                                       \
\
	void STACK_DESTRUCT_##TYPE(STACK_##TYPE* self) {    \
		if (self->base != NULL) {                       \
			free(self->base);                           \
		}                                               \
														\
		self->base = NULL;                              \
		self->next_item = NULL;                         \
		self->base_size = 0;                            \
		self->container_size = 0;                       \
														\
		if (self != NULL) {                             \
			free(self);                                 \
		}                                               \
	}                                                   \
\
	void COPY_##TYPE(const TYPE* from, TYPE* to, size_t count) { \
		if (from ==NULL || to == NULL) {                         \
			return;                                              \
		}                                                        \
																 \
		const TYPE* f_ptr = from;                                \
		TYPE* t_ptr = to;                                        \
														         \
		for (size_t i = 0; i < count; ++i) {                     \
			*(t_ptr + i) = *(f_ptr + i);                         \
		}                                                        \
	}                                                            \
\
	void RESIZE_##TYPE(STACK_##TYPE* self, size_t new_size) {           \
		if (self == NULL) {                                             \
			return;                                                     \
		}                                                               \
																		\
		TYPE* new_base = (TYPE*) malloc(sizeof(TYPE) * new_size);       \
																		\
		if (new_base == NULL) {                                         \
			return;                                                     \
		}                                                               \
																		\
		size_t size_to_copy = compare(self->container_size, new_size);  \
																	    \
		if (self->base && new_base) {                                   \
			COPY_##TYPE(self->base, new_base, size_to_copy);            \
		}                                                               \
																		\
		free(self->base);                                               \
																		\
		self->base = new_base;                                          \
		self->base_size = new_size;                                     \
	}                                                                   \
\
	void RESERVE_##TYPE(STACK_##TYPE* self, size_t size) {    \
		if (self == NULL) {                                   \
			return;                                           \
		}                                                     \
															  \
		int diff = self->base_size - size;                \
															  \
		if (diff <= 0) {                                      \
			RESIZE_##TYPE(self, size * MULTIPLIER_COEF);      \
			self->next_item = self->base + size;              \
		} else {                                              \
			self->next_item += size - self->container_size;   \
		}                                                     \
															  \
		self->container_size = size;                          \
	}                                                         \
\
	void STACK_PUSH_##TYPE(STACK_##TYPE* self, TYPE value) { \
		RESERVE_##TYPE(self, self->container_size + 1);      \
		*(self->next_item - 1) = value;                      \
	}                                                        \
\
	TYPE STACK_POP_##TYPE(STACK_##TYPE* self) {              \
		if (self->container_size == 0) {                     \
			exit(0);                                         \
		}                                                    \
															 \
		TYPE result = *(self->next_item - 1);                \
		RESERVE_##TYPE(self, self->container_size - 1);      \
															 \
		return result;                                       \
	}                                                        \
\
	void STACK_PRINT_##TYPE(STACK_##TYPE* self, char* format) {  \
		for (size_t i = 0; i < self->container_size; ++i) {      \
			fprintf(stdout ,format, self->base[i]);              \
		}                                                        \
																 \
		fprintf(stdout, "\n");                                   \
	}                                                            \
\
	TYPE STACK_TOP_##TYPE(STACK_##TYPE* self) {  \
		return *(self->next_item - 1);           \
	}                                            \
\
	size_t STACK_SIZE_##TYPE(STACK_##TYPE* self) {  \
		return self->container_size;                \
	}                                               \
\
	bool STACK_EMPTY_##TYPE(STACK_##TYPE* self) {  \
		return self->container_size == 0;          \
	}                                              \

#define STACK(TYPE) \
	STACK_##TYPE

#define CONSTRUCTOR(TYPE) \
	STACK_CONSTRUCT_##TYPE()

#define DESTRUCTOR(TYPE, STACK) \
	STACK_DESTRUCT_##TYPE(STACK)

#define PUSH(TYPE, STACK, VALUE) \
	STACK_PUSH_##TYPE(STACK, VALUE)

#define POP(TYPE, STACK) \
	STACK_POP_##TYPE(STACK)

#define PRINT(TYPE, STACK, FORMAT) \
	STACK_PRINT_##TYPE(STACK, FORMAT)

#define TOP(TYPE, STACK) \
	STACK_TOP_##TYPE(STACK)

#define SIZE(TYPE, STACK) \
	STACK_SIZE_##TYPE(STACK)

#define EMPTY(TYPE, STACK) \
	STACK_EMPTY_##TYPE(STACK)

int comparator(const void* a, const void* b) {
	return (*(int*) a - *(int*) b);
}

#define SORT(TYPE, SET) \
	qsort(SET->base, SIZE(TYPE, SET), sizeof(TYPE), comparator);

DECLARE_STACK(char)
DECLARE_STACK(int);

bool IsValidSymbol(char ch) {
	char valid_symbols[] =
			"0123456789"
			"-,U()[]^\\";

	for (size_t i = 0; i < sizeof(valid_symbols) - 1; ++i) {

		if (ch == valid_symbols[i]) {
			return true;
		}
	}

	return false;
}

bool IsOperator(char op) {
	return op == 'U' || op == '^' || op == '\\';
}

int Priority(char op) {
	return op == 'U' || op == '\\' ? 1 : op == '^' ? 2 : -1;
}

void GetSet(STACK(int)* base, STACK(int)* buffer) {
	int size = POP(int, base);

	for (size_t i = 0; i < size; ++i) {
		PUSH(int, buffer, POP(int, base));
	}
}

int Count(STACK(int)* set, int element) {
	int count = 0;

	for (size_t i = 0; i < SIZE(int, set); ++i) {

		if (set->base[i] == element) {
			++count;
		}
	}

	return count;
}

bool IsIn(int element, STACK(int)* set) {
	for (size_t i = 0; i < SIZE(int, set); ++i) {

		if (element == set->base[i]) {
			return true;
		}
	}

	return false;
}

void ProcessOperation(STACK(int)* multiset, char op) {
	STACK(int)* right = CONSTRUCTOR(int);
	STACK(int)* left = CONSTRUCTOR(int);

	GetSet(multiset, right);
	GetSet(multiset, left);

	STACK(int)* buffer = CONSTRUCTOR(int);
	switch (op) {

		case 'U':

			for (size_t i = 0; i < SIZE(int, right); ++i) {

				if (Count(left, right->base[i]) == 0) {
					PUSH(int, left, right->base[i]);
				}
			}

			for (size_t i = 0; i < SIZE(int, left); ++i) {
				PUSH(int, multiset, left->base[i]);
			}

			PUSH(int, multiset, (int) SIZE(int, left));

			break;

		case '\\':

			for (size_t i = 0; i < SIZE(int, left); ++i) {

				if (Count(right, left->base[i]) == 0) {
					PUSH(int, multiset, left->base[i]);
				}
			}

			PUSH(int, multiset, (int) SIZE(int, multiset));

			break;

		case '^':

			for (size_t i = 0; i < SIZE(int, left); ++i) {

				for (size_t j = 0; j < SIZE(int, right); ++j) {

					if (left->base[i] == right->base[j] && !IsIn(left->base[i], buffer)) {
						PUSH(int, buffer, left->base[i]);
					}
				}
			}

			for (size_t i = 0; i < SIZE(int, buffer); ++i) {
				PUSH(int, multiset, buffer->base[i]);
			}

			PUSH(int, multiset, (int) SIZE(int, buffer));

			DESTRUCTOR(int, buffer);
			break;
	}

	DESTRUCTOR(int, left);
	DESTRUCTOR(int, right);
}

int main() {
	STACK(char)* expression = CONSTRUCTOR(char);
	char ch;

	while (true) {
		ch = fgetc(stdin);

		if (ch == ' ') {
			continue;
		}

		if (ch == EOF || ch == '\n') {
			break;
		}

		if (!IsValidSymbol(ch)) {
			fprintf(stderr, "[error]\n");
			return INVALID_INPUT;
		}

		PUSH(char, expression, ch);
	}

	fprintf(stdout, "\n");

	STACK(int)* multiset = CONSTRUCTOR(int);
	STACK(char)* operators = CONSTRUCTOR(char);
	int chunk_size = 0;

	for (size_t i = 0; i < SIZE(char, expression); ++i) {

		if (expression->base[i] == '(') {
			PUSH(char, operators, expression->base[i]);

		} else if (expression->base[i] == ')') {

			while (TOP(char, operators) != '(') {
				ProcessOperation(multiset, POP(char, operators));
			}

			POP(char, operators);

		} else if (IsOperator(expression->base[i])) {

			while (!EMPTY(char, operators) && (
				(expression->base[i] >= 0 && Priority(TOP(char, operators)) >= Priority(expression->base[i])))) {
				ProcessOperation(multiset, POP(char, operators));
			}

			PUSH(char, operators, expression->base[i]);

		} else if (isdigit(expression->base[i])) {
			STACK(char)* operand = CONSTRUCTOR(char);
			++chunk_size;

			while (i < SIZE(char, expression) && isdigit(expression->base[i])) {
				PUSH(char, operand, expression->base[i++]);
			}

			PUSH(char, operand, '\0');

			if (isdigit(operand->base[0]) || operand->base[0] == '-') {
				PUSH(int, multiset, atoi(operand->base));
			}

			if (expression->base[i] == ']') {
				PUSH(int, multiset, chunk_size);
				chunk_size = 0;
			}

			DESTRUCTOR(char, operand);

		} else if (expression->base[i] == '[' && expression->base[i + 1] == ']') {
			PUSH(int, multiset, 0);
			++i;
		}
	}

	while (!EMPTY(char, operators)) {
		ProcessOperation(multiset, POP(char, operators));
	}

	POP(int, multiset);
	SORT(int, multiset);

	fprintf(stdout, "[");
	for (size_t i = 0; i < SIZE(int, multiset); ++i) {

		if (i == SIZE(int, multiset) - 1) {
			fprintf(stdout, "%d", multiset->base[i]);
		} else {
			fprintf(stdout, "%d,", multiset->base[i]);
		}
	}
	fprintf(stdout, "]\n");

	DESTRUCTOR(char, operators);
	DESTRUCTOR(int, multiset);
	DESTRUCTOR(char, expression);

	return 0;
}