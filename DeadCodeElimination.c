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
		int offsetArray[10] = {0};

		while (current != NULL && current != head)
		{
			current->critical = 0;
			
			if (current->opcode == OUTPUTAI)
			{
				current->critical = 1;
				regArray[current->field1] = 1;
				offsetArray[(current->field2) / 4] = 1;
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
				if (regArray[current->field2] == 1 && offsetArray[(current->field3) / 4] == 1)
				{
					current->critical = 1;
					regArray[current->field1] = 1;
					offsetArray[(current->field3) / 4] = 0;
				}
			}
			else if (current->opcode == LOADAI)
			{
				if (regArray[current->field3] == 1)
				{
					current->critical = 1;
					regArray[current->field1] = 1;
					offsetArray[(current->field2) / 4] = 1;
				}
			}
			else
			{
				if (regArray[current->field3] == 1)
				{
					current->critical = 1;

					if (current->opcode == LSHIFTI || current->opcode == RSHIFTI)
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
			if (current->critical == 0)
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

