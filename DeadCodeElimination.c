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


Instruction *dead_code_elimination(Instruction *head) 
{

		/* STUDENTS - BEGIN */
		
		// Set current as the head of the list
		Instruction *current = head;

		// Iterate to the end of the linked list: current is now the last instruction
		while (current->next != NULL)
		{
			current = current->next;
		}
		
		int regArray[4096] = {0};	// Boolean array of registers (default value: 0)
		int offsetArray[10] = {0};	// Boolean array of offset values for r0 (default value: 0)

		// Iterate backwards from the last instruction
		while (current != NULL && current != head)
		{
			current->critical = 0;		// The instruction is initially marked non-critical (just in case default is not 0)
			
			// Case where instruction is OUTPUTAI: Thus, inherently critical
			if (current->opcode == OUTPUTAI)
			{
				current->critical = 1;							// Set instruction as critical
				regArray[current->field1] = 1;					// Set value at the index of the register as 1
				offsetArray[(current->field2) / 4] = 1;			// Set value at the index of the offset/4 as 1
			}
			// Case where instruction is LOADI: Only critical if register at field2 is critical
			else if (current->opcode == LOADI)
			{
				if (regArray[current->field2] == 1)
				{
					current->critical = 1;						// Set instruction as critical
				}
			}
			// Case where instruction is STOREAI: Only critical if register at field2 and the offset are critical
			else if (current->opcode == STOREAI)
			{
				if (regArray[current->field2] == 1 && offsetArray[(current->field3) / 4] == 1)
				{
					current->critical = 1;						// Set instruction as critical
					regArray[current->field1] = 1;				// Set value at the index of the field1 register as 1
					offsetArray[(current->field3) / 4] = 0;		// Set value at the index of the offset/4 back to 0
				}
			}
			// Case where instruction is LOADAI: Only critical if register at field3 is critical
			else if (current->opcode == LOADAI)
			{
				if (regArray[current->field3] == 1)
				{
					current->critical = 1;						// Set instruction as critical
					regArray[current->field1] = 1;				// Set value at the index of the field1 register as 1
					offsetArray[(current->field2) / 4] = 1;		// Set value at the index of the offset/4 as 1
				}
			}
			// Case where instruction is LSHIFTI, RSHIFTI, ADD, SUB, MUL, or DIV: Only critical if register at field3 is critical
			else
			{
				if (regArray[current->field3] == 1)
				{
					current->critical = 1;						// Set instruction as critical

					if (current->opcode == LSHIFTI || current->opcode == RSHIFTI)
					{
						regArray[current->field1] = 1;			// Set only value at the index of the field1 register as 1
					}
					else
					{
						regArray[current->field1] = 1;			// Set value at the index of the field1 register as 1
						regArray[current->field2] = 1;			// Set value at the index of the field2 register as 1
					}
				}
			}

			current = current->prev;
		}

		// Set current to the instruction after "loadI 1024 => r0"
		current = head->next;

		while (current != NULL)
		{	
			// If instruction is not critical, remove from linked list
			if (current->critical == 0 && current != NULL && current->prev != NULL)
			{
				Instruction *temp = current;
				temp = temp->prev;
				temp->next = current->next;			// Make the previous node point to the node after current
				current->next->prev = temp;			// Make the next node point to the node before current
				free(current);						// Free the current node
				current = temp;						// Set the previous node as the new current node
				continue;
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

	head = dead_code_elimination(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

