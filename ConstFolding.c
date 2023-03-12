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

		Instruction *instr1 = head;

		while (instr1->next->next != NULL)
		{
			Instruction *instr2 = instr1->next;
			Instruction *instr3 = instr2->next;

			if (instr1->opcode == LOADI && instr2->opcode == LOADI)
			{
				if (instr3->opcode == ADD)
				{
					Instruction *temp = NULL;
					temp->opcode = LOADI;
					temp->field1 = instr1->field1 + instr2->field1;
					temp->field2 = instr3->field3;

					temp->prev = instr2;
					temp->next = instr3->next;
					free(instr3);
				}
			}

			printf("%d", instr1->field1);

			instr1 = instr1->next;
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

