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
#include <math.h>
#include "InstrUtils.h"
#include "Utils.h"

/*
* Will return log2(n) if it is an integer (meaning n is a power of 2), and -1 otherwise
*/
int int_log2(int n) {

	int s = 1, i = 0;

	while (s < n) {
		s = s << 1;
		i++;
	}

	if (s == n)
		return i;
	else
		return -1;

}

/*
* Will traverse instructions in pairs, searching for a patten of LOADI and MUL | DIV instructions 
* where LOADI is a power of 2, and collapse these instructions into on shift operation of LSHIFTI | RSHIFTI
*/
Instruction *strength_reduction(Instruction *head) {

		/* STUDENTS - BEGIN */
		
		// Skip loadI 1024 => r0
		Instruction *current = head->next;

		while (current != NULL)
		{
			// Check if current instruction is LOADI, and c is a power of 2 (and if current->next exists)
			if ((current->opcode == LOADI && int_log2(current->field1) != -1) && (current->next != NULL))
			{
				Instruction *next = current->next;

				if (next->opcode == MUL)
				{
					// Case where first field is from previous LOADI instruction
					if (current->field2 == next->field1)
					{
						next->opcode = LSHIFTI;
						next->field1 = int_log2(current->field1);
					}
					// Case where second field is from previous LOADI instruction
					else if (current->field2 == next->field2)
					{
						next->opcode = LSHIFTI;
						next->field2 = int_log2(current->field1);
					}
				}
				else if (next->opcode == DIV)
				{
					if (current->field2 == next->field2)
					{
						next->opcode = RSHIFTI;
						next->field2 = int_log2(current->field1);
					}
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

	head = strength_reduction(head);

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

