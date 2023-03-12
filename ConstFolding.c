/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2023                              *
 *  Student Version                          *
 *  Author: Rishik Sarkar                    *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

/*
* Will traverse instructions in groups of 3, searching for a patten of LOADI, LOADI, and ADD | SUB | MUL instructions 
*/

Instruction *constant_folding(Instruction *head) {

		/* STUDENTS - BEGIN */

		Instruction *current = head;
		Instruction *prev = NULL;
		Instruction *next = NULL;

		while (current != NULL)
		{
			if (current->prev != NULL)
			{
				prev = current->prev;
			}
			else 
			{
				current = current->next;
				continue;
			}

			if (current->next != NULL)
			{
				next = current->next;
			}
			else 
			{
				current = current->next;
				continue;
			}

			if (prev->opcode == LOADI && current->opcode == LOADI)
			{
				if (next->opcode == ADD)
				{
					int reg = next->field3;
					next->opcode = LOADI;
					next->field1 = prev->field1 + current->field1;
					next->field2 = reg;
					current = current->next;
					continue;
				}
				else if (next->opcode == SUB)
				{
					int reg = next->field3;
					next->opcode = LOADI;
					next->field1 = prev->field1 - current->field1;
					next->field2 = reg;
					current = current->next;
					continue;
				}
				else if (next->opcode == MUL)
				{
					int reg = next->field3;
					next->opcode = LOADI;
					next->field1 = prev->field1 * current->field1;
					next->field2 = reg;
					current = current->next;
					continue;
				}
			}

			current = current->next;
		}

 		/* STUDENTS - END */

	return head;
}

int main()
{
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	head = constant_folding(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

