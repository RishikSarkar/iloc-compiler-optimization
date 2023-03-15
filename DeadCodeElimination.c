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
		
		Instruction *current = head->next;

		while (current->next != NULL)
		{
			current = current->next;
		}

		while (current->opcode != OUTPUTAI)
		{
			if (current != NULL && current->prev != NULL)
			{
				Instruction *temp = current;
				current = current->prev;
				current->next = temp->next;
				//free(temp);
				continue;
			}
		}
		
		int regArray[4096] = {0};

		while (current != NULL)
		{
			current->critical = 0;
			
			if (current->opcode == OUTPUTAI)
			{
				current->critical = 1;
				regArray[current->field1] = 1;
			}
			else if (current->opcode == LOADI)
			{
				if (regArray[current->field2] == 1)
				{
					current->critical = 1;
				}
			}
			else if (current->opcode == STOREAI)
			{
				if (regArray[current->field2] == 1)
				{
					current->critical = 1;
					regArray[current->field1] = 1;
				}
			}
			else
			{
				if (regArray[current->field3] == 1)
				{
					current->critical = 1;

					if (current->opcode == LOADAI || current->opcode == LSHIFTI || current->opcode == RSHIFTI)
					{
						regArray[current->field1] = 1;
					}
					else
					{
						regArray[current->field1] = 1;
						regArray[current->field2] = 1;
					}
				}
			}

			current = current->prev;
		}

		current = head->next;

		while (current != NULL)
		{
			if (current->critical != 1)
			{
				if (current != NULL && current->prev != NULL)
				{
					Instruction *temp = current;
					current = current->prev;
					current->next = temp->next;
					//free(temp);
					continue;
				}
			}

			current = current->next;
		}


		/*
		// Begin on the instruction after: loadI 1024 => r0
		Instruction *current = head->next;

		int reg;

		//int regArray[4096] = {0};
		//int *index;
		int index;

		while (current != NULL)
		{
			// Case where instruction is LOADI or STOREAI: Thus, stored register is in field2
			if (current->opcode == LOADI || current->opcode == STOREAI)
			{
				reg = current->field2;
			}
			// Case where instruction is OUTPUTAI: Thus, no register is stored
			else if (current->opcode == OUTPUTAI)
			{
				current = current->next;
				continue;
			}
			// Case where instruction is anything else: Thus, stored register is in field3
			else
			{
				reg = current->field3;
			}

			//index = &regArray[reg];
			index = 0;

			// Case where register is not guaranteed to be critical, so check all subsequent instructions
			//if (*index == 0)
			//{
			Instruction *nextIterator = current->next;

			while (nextIterator != NULL)
			{
				// Case where instruction is LOADI: Thus, register in current is not applicable
				if (nextIterator->opcode == LOADI)
				{
					nextIterator = nextIterator->next;
					continue;
				}
				// Case where instruction is LOADAI, STOREAI, LSHIFTI, RSHIFTI, or OUTPUTAI: Thus, register is field1
				else if (nextIterator->opcode == LOADAI || nextIterator->opcode == STOREAI || nextIterator->opcode == LSHIFTI || nextIterator->opcode == RSHIFTI || nextIterator->opcode == OUTPUTAI)
				{
					if (reg == nextIterator->field1)
					{
						// Change regArray[*reg] from 0 to 1, since register is guaranteed to be critical
						//index = 1;
						index = 1;
						break;
					}
				}
				// Case where instruction is ADD, SUB, MUL, or DIV: Thus, register can be field1 or field 2
				else
				{
					if (reg == nextIterator->field1 || reg == nextIterator->field2)
					{
						// Change regArray[*reg] from 0 to 1, since register is guaranteed to be critical
						//index = 1;
						index = 1;
						break;
					}
				}
				
				nextIterator = nextIterator->next;
			}
			//}

			// If register is not critical, remove instruction
			if (index == 0)
			{
				//Instruction *temp = current;
				//current->prev->next = current->next;
				//free(temp);

				if (current != NULL && current->prev != NULL)
				{
					Instruction *temp = current;
					current = current->prev;
					current->next = temp->next;
					//free(temp);
					continue;
				}
			}

			current = current->next;
		}
		*/
		

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

