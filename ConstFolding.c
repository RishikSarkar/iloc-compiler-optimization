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
			// Check if there exists a previous instruction
			if (current->prev != NULL)
			{
				prev = current->prev;
			}
			else 
			{
				current = current->next;
				continue;
			}

			// Check if there exists an instruction after current
			if (current->next != NULL)
			{
				next = current->next;
			}
			else 
			{
				current = current->next;
				continue;
			}

			// Check if there are two LOADI instructions in a row (prev and current)
			if (prev->opcode == LOADI && current->opcode == LOADI)
			{
				// Case where next instruction is ADD
				if (next->opcode == ADD && ((next->field1 == prev->field2 && next->field2 == current->field2) 
										|| (next->field1 == current->field2 && next->field2 == prev->field2)))
				{
					int reg = next->field3;
					next->opcode = LOADI;
					next->field1 = prev->field1 + current->field1;
					next->field2 = reg;
					current = current->next;
					continue;
				}
				// Case where next instruction is SUB and LOADI registers appear in the same order as SUB registers
				else if (next->opcode == SUB && (next->field1 == prev->field2 && next->field2 == current->field2))
				{
					int reg = next->field3;
					next->opcode = LOADI;
					next->field1 = prev->field1 - current->field1;
					next->field2 = reg;
					current = current->next;
					continue;
				}
				// Case where next instruction is SUB and LOADI registers appear in the opposite order as SUB registers
				else if (next->opcode == SUB && (next->field1 == current->field2 && next->field2 == prev->field2))
				{
					int reg = next->field3;
					next->opcode = LOADI;
					next->field1 = current->field1 - prev->field1;
					next->field2 = reg;
					current = current->next;
					continue;
				}
				// Case where next instruction is MUL
				else if (next->opcode == MUL && ((next->field1 == prev->field2 && next->field2 == current->field2) 
											|| (next->field1 == current->field2 && next->field2 == prev->field2)))
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

