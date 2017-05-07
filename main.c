#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BODY_SIZE 50

typedef struct {
	int number;
	char * c;
} Body;

typedef struct {
	int number;
	Body * list;
} Production;

int getInput(int * variables, int * terminals, Production productions[256]);
int * isLeftRecursive(Production production, int variable);

int main()
{
	int variables[256] = {0};
	int terminals[256] = {0};
	Production productions[256];
	getInput(variables, terminals, productions);

	//display productions
	int i;
	int j;
	printf("Non-Terminals:\n");
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			printf("%c ", i);
		}
	}
	printf("\n");
	printf("Terminals:\n");
	for (i = 0; i < 256; i++) {
		if (terminals[i]) {
			printf("%c ", i);
		}
	}
	printf("\n");
	printf("Productions:\n");
	for (i = 0; i < 256; i++) {
		if (productions[i].number) {
			for (j = 0; j < productions[i].number; j++) {
				printf("%c -> %s\n", i, productions[i].list[j].c);
			}

		}
	}
	eliminateLeftRecursion(variables, terminals, productions);
	return 0;
}

int eliminateLeftRecursion(int variables[256], int terminals[256], \
	Production productions[256])
{
	int i;
	int j;
	printf("Modified Productions:\n");
	for (i = 0; i < 256; i++) {
		if (variables[i]) {
			int * bodyStatus = isLeftRecursive(productions[i], i);
			if (bodyStatus) {
				/*first find a free variable*/
				int newVariable = 0;
				for (j = 'A'; j <= 'Z'; j++) {
					if (!variables[j]) {
						newVariable = j;
						break;
					}
				}
				variables[newVariable] = 1;

				char variableStr[2] = {(char) i, '\0'};
				char newVariableStr[2] = {(char) newVariable, '\0'};
				//printf("Old Variable: %s\n", variableStr);
				//printf("New Variable: %s\n", newVariableStr);
				Body * oldList = productions[i].list;
				for (j = 0; j < productions[i].number; j++) {
					if (bodyStatus[j] == 0) {
						printf("%s -> %s\n", variableStr, \
							strncat(oldList[j].c, newVariableStr, \
								BODY_SIZE - 1 - oldList[j].number));
					} else if (bodyStatus[j] == 1) {
						printf("%s -> %s\n", newVariableStr, \
							strncat(oldList[j].c + 1, newVariableStr, \
								BODY_SIZE - 1 - oldList[j].number));
					} else {
						printf("An error has occured\n");
						exit(EXIT_FAILURE);
					}
				}
				//printf("%c: true\n", i);
			} else if (productions[i].number) {
				for (j = 0; j < productions[i].number; j++) {
					printf("%c -> %s\n", i, productions[i].list[j].c);
				}
			}
		}
	}
	return 0;
}

int * isLeftRecursive(Production production, int variable)
{
	int i;
	int * recursiveBody = NULL;
	if (production.number) {
		int flag = 0;
		recursiveBody = (int *) malloc(sizeof(int) * production.number);
		for (i = 0; i < production.number; i++) {
			if (production.list[i].number) {
				if (production.list[i].c[0] == (char) variable) {
					flag = 1;
					recursiveBody[i] = 1;
					continue;
				}
			}
			recursiveBody[i] = 0;
		}
		if (!flag) {
			free(recursiveBody);
			recursiveBody = NULL;
		}
	}
	return recursiveBody;
}

int getInput(int * variables, int * terminals, Production productions[256])
{
	int i;
	int j;
	int c;
	int numVariables;
	int numTerminals;
	int numProductions;
	char head;
	char body[BODY_SIZE];//just an assumption for input length
	printf("Variables: ");
	scanf(" %d", &numVariables);
	for (i = 0; i < numVariables; i++) {
		scanf(" %c", &c);
		variables[c] = 1;
	}
	printf("Terminals: ");
	scanf(" %d", &numTerminals);
	for (i = 0; i < numTerminals; i++) {
		scanf(" %c", &c);
		terminals[c] = 1;
	}
	/*initialize production array*/
	for (i = 0; i < 256; i++) {
		productions[i].number = 0;
		productions[i].list= NULL;
	}
	printf("Number of Productions: ");
	scanf(" %d", &numProductions);
	for (i = 0; i < numProductions; i++) {
		scanf(" %c %s", &head, body);
		if (variables[head]) {
			productions[head].number++;
			productions[head].list = (Body *) realloc(productions[head].list, productions[head].number * sizeof(Body));
			productions[head].list[productions[head].number - 1].number = 0;
			productions[head].list[productions[head].number - 1].c = NULL;
			for (j = 0; j < strlen(body); j++) {
				productions[head].list[productions[head].number - 1].number++; 
				productions[head].list[productions[head].number - 1].c = (char *) realloc(productions[head].list[productions[head].number - 1].c, productions[head].list[productions[head].number - 1].number * sizeof(char));
				if (variables[body[j]] || terminals[body[j]]) {
					productions[head].list[productions[head].number - 1].c[productions[head].list[productions[head].number - 1].number - 1]  = body[j];
				} else {
					//this character is treated as a null character (which is neither in variables nor terminals)
					productions[head].list[productions[head].number - 1].c[productions[head].list[productions[head].number - 1].number - 1]  = '\0';
				}
			}
		}
	}
	return 0;
}
