#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include"commands.h"

int main(int argc, char *argv[])
{
	char input[128];
    char *command;
	char *arg;
	char mover; // used for re-ordering character arrays
    char *path;
    char *buf;

    int mounted = 0; // for checking if floppy has actually been mounted in fumount
    int sectorNum = 0;
    int floppyDrive;

    printf("enter floppy command: ");
    fgets(input, 128, stdin);
	command = strtok(input, " \n\r");
	arg = strtok(NULL, " \n\r");
    while (strstr(command, "quit") == NULL)
    {
        if (strstr(command, "fmount") != NULL)
        { // requires arg pass, must return floppyDrive so fumount can close properly. set mounted after calling the function in main()
			if (arg != NULL)
			{
            	floppyDrive = fmount(arg);
				mounted = 1;
			}
			else
			{
				printf("Please input a path to an imagefile when mounting a floppy.\n");
			}

        }
        else if (strstr(command, "fumount") != NULL)
        { // pass floppyDrive in, set mounted after calling the function in main()
            if (mounted == 1)
            {
                fumount(floppyDrive);
				mounted = 0;
            }
            else
            {
                printf("cannot unmount disk: no disk mounted.\n");
            }
        }
        else if (strstr(command, "help") != NULL)
        { // requires no pass or anything
			help();
        }
        else if (strstr(command, "quit") != NULL)
        { 
			; // this just exits the loop without giving an error
        }
        else if ((strstr(command, "structure") != NULL) && (mounted == 1))
        { // requires floppyDrive passed in. FREE BUF AT END OF FCN
			structure(floppyDrive);
        }
        else if ((strstr(command, "traverse") != NULL) && (mounted == 1)) 
        { // needs arg and floppyDrive passed in, FREE BUFF AT END OF FCN
			traverse(arg, floppyDrive, 0, "aeiou");
        }

        else if ((strstr(command, "showsector") != NULL) && (mounted == 1))
        { // needs arg, floppyDrive passed in. FREE BUF AT END OF FCN
			if (arg != NULL) 
			{
				showsector(arg, floppyDrive);
			}
			else
			{
				printf("Please input a sector number to show a sector.\n");
			}
        }


        else if ((strstr(command, "showfat") != NULL) && (mounted == 1))
        { // this isn't even here yet
			printf("showfat!\n");

        }
		else if ((strstr(command, "showfile") != NULL) && (mounted == 1))
		{ // nor is this one, but you still need to FREE BUF AT END OF FCN
		    //int foundFile = 0;
            //buf = (char *) malloc(512);
            //char fileName[128];
            //scanf("%s", fileName);
			printf("showfile! \n");


        }
        else if (mounted == 1) // this can be here instead of in every else-if because the ones that don't need it come first.
        {
            printf("unknown command. to see a list of commands, type help\n");
        }
		else
		{
			printf("please mount a floppy disk before trying to read it.\n");
		}
		

        printf("enter floppy command: ");
		fgets(input, 128, stdin);
		command = strtok(input, " \n\r");
		arg = strtok(NULL, " \n\r");
    }

	return 0;
}
