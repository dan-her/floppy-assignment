#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>

int main(int argc, char *argv[])
{
    char command[128];
	char mover; // used for re-ordering character arrays
    char *path;
    char *buf;

    int mounted = 0; // for checking if floppy has actually been mounted in fumount
    int sectorNum = 0;
    int floppyDrive;
	int i, j = 0, k; // iteration variable(s)

    printf("enter floppy command: ");
    scanf("%s", command);

    while (strcmp(command, "quit") != 0)
    {
        if (strcmp(command, "fmount") == 0)
        {
            char floppyPath[128];
            scanf("%s", floppyPath);
            floppyDrive = open(floppyPath, O_RDONLY, 0); // removed the DIR use since DIR can't actually open files, only directories.
			if (floppyDrive == -1) // checks if the file opened successfully
			{
				printf("Cannot open image at %s. The path may be incorrect, or it may not be an image file.\n", floppyPath);
			} 
			else
			{
            printf("Image at %s mounted.\n", floppyPath);
			mounted = 1;
			}

        }
        else if (strcmp(command, "fumount") == 0)
        {
            if (mounted == 1)
            {
                close(floppyDrive);
                printf("disc unnmounted\n");
            }
            else
            {
                printf("cannot unmount disk: no disk mounted.\n");
            }
        }
        else if (strcmp(command, "help") == 0)
        {
            printf("fmount \t\t\tmounts a local floppy disk from the specified .img file\n"
           "fumount \t\t\t\t\t\t\tunmount the mounted floppy disk\nstructure \t\t\t\t\t\t\t"
           "lists the structure of the floppy disk\ntraverse [-l] \t\t\t\t\t\tlists the content in the root"
           " directory. switch l shows all detailed information about a file\nshowsector [sector number]\t\t"
           "\tshow the content of the specified sector (512 bytes of hex dump)\nshowfat \t\t\t\t\t\t\tshows"
           " the content of the first 256 entries of the FAT table (as a hex dump)\nshowfile [filename] "
           "\t\t\t\tshows the content of the file (as a hex dump)\nquit\t\t\t\t\t\t\t\tquits the Floppy Disk Browser shell\n");
        }
        else if (strcmp(command, "quit") == 0)
        {
            exit(0);
        }
        else if ((strcmp(command, "structure") == 0) && (mounted == 1))
        {
            buf = (char *) malloc(512);
            read(floppyDrive, buf, 512); // puts the boot sector into memory
            int bytesPerSector = buf[11] + buf[12] << 8;
            char sectorsPerCluster = buf[13];
            char fatCount = buf[16];
            unsigned char rootEntries = buf[17];
            int sectorsPerFat = buf[22];

            printf("number of FAT:			    %d\nnumber of sectors used by FAT:	    %d\nnumber of sectors per cluster:	    %d\n"
            "number of ROOT Entries:		  %d\nnumber of bytes per sector:	  %d\n",
            fatCount, sectorsPerFat, sectorsPerCluster, rootEntries, bytesPerSector); // just printing in given format, separated into two parts so more comments can fit
            printf(""
                   "---Sector #---     ---Sector Types---\n"
                   "0                   BOOT\n"    // since the boot sector has to be 0 on any floppy disk, can we assume this and hardcode it? how else to find boot sector #?
                   "1 -- %d				FAT1\n" 	// the location of the first part of fat1 depends on the boot sector's location, so it would have to be hardcoded too, probably
                   "%d -- %d			FAT2\n"	// both of the above comments are probably the wrong way to do it, since we aren't supposed to hardcode the numbers in this part
                   "%d -- %d			ROOT DIRECTORY\n", (sectorsPerFat), (sectorsPerFat+1), ((int)fatCount*sectorsPerFat), (((int)fatCount*sectorsPerFat)+1), (((rootEntries*32)/bytesPerSector)+(int)fatCount*sectorsPerFat) );
			lseek(floppyDrive, 0L, 0);// needed to return to start of file
        }
        else if ((strcmp(command, "traverse") == 0) && (mounted == 1)) // this is not working! do not use it yet, unless you mean to test it
        {
			char potentialFlag[128];
			char filename;
			scanf("%s", potentialFlag); // makes a flag required, not sure how to skip it if there's no flag
			buf = (char *) malloc(32); // buf is going to be used on a per-entry basis here, so we just give it the file size's space
			lseek(floppyDrive, 9728L, 0); // seeks past the first 19 sectors of the floppy (19 sectors * 512 bytes each) to get to root
			if (potentialFlag == "-l")
			{
				// should be about the same as the else, but with some extra prints and extra value checking in buf
			}
			else
			{
				//for (i = 0; i < 224; i++) // iterates thru the whole of root
				//{
					read(floppyDrive, buf, 32); // reads the file
					for (j = 0; j < 32; j++)
					{						
						printf("%d ", j);
						for (k = 0; k < 8; k++)
						{
							printf("%d", !!((buf[j] << k) & 0x80));
						}		
						printf("\n");
					}
					printf("a.cla?");
/*					for (j = 1; j < 24; j++)
					{
						if (j % 2 != 0)
						{
						filename = buf[j];
						printf("%c-", filename);//for some reason there's an extra blank character printed between each character, shown by the "-"
						}
					}
					printf("\n"); // moves to the next line for the next filename
					/* this conditional might not be needed, but I'm keeping it around on the off-chance it is
					if () // the whole of buf is empty, meaning that there is no file.
					{
						;  //just do nothing for that file
					}
					else
					{
						
					}*/
				//}
			}
			lseek(floppyDrive, 0L, 0);// needed to return to start of file
        }
        else if ((strcmp(command, "showsector") == 0) && (mounted == 1))
        {
			char sectornum[4] = {'\0', '\0', '\0', '\0'};// these are not keyboard characters, so we they won't ever be accidentally input
			scanf("%s", sectornum); // since scanf can't scan for integers, we have to get crafty
			j = 0; // ensure that j starts at 0 every time we run the loop
			for (i = 3; i >= 0; i--) // we have to make sure the sector number is actually valid, so we need to check the elements of sectornum
			{
				if (sectornum[i] == '0') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 0*(int)(pow((double)10, (double)j));
					j++;
				}
				else if (sectornum[i] == '1') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 1*(int)(pow((double)10, (double)j));
					j++;
				}
				else if (sectornum[i] == '2') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 2*(int)(pow((double)10, (double)j));
					j++;
				}
				else if (sectornum[i] == '3') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 3*(int)(pow((double)10, (double)j));
					j++;
				}
				else if (sectornum[i] == '4') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 4*(int)(pow((double)10, (double)j));
					j++;
				}
				else if (sectornum[i] == '5') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 5*(int)(pow((double)10, (double)j));
					j++;
				}
				else if (sectornum[i] == '6') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 6*(int)(pow((double)10, (double)j));
					j++;
				}
				else if (sectornum[i] == '7') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 7*(int)(pow((double)10, (double)j));
					j++;
				}
				else if (sectornum[i] == '8') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 8*(int)(pow((double)10, (double)j));
					j++;
				}
				else if (sectornum[i] == '9') // every one of these is the same, for every digit. puts the digit at its place in sectorNum using the pow fcn
				{
					sectorNum = sectorNum + 9*(int)(pow((double)10, (double)j));
					j++;
				}

				else // default to not having a value at a power of 10 position
				{
					sectorNum = sectorNum + 0;
				}
			}
			buf = (char *) malloc(512);
			lseek(floppyDrive, (long)(512*sectorNum), 0);
			read(floppyDrive, buf, 512); 
			printf("     0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F");
			for (i = 0; i < 512; i ++)
			{
				if ((i % 16) == 0)
				{
					printf("\n%X  ", i); // prints hex of i for the 0, 10, 20, etc. values to the left of the table when we need a new line
					if (i < 0x100) // this keeps spacing consistent
					{
						printf(" ");
						if (i < 0x10) // this also keeps spacing consistent, only happens on the first line of output
						{
							printf(" ");
						}		
					}
				}
				printf("%X  ", buf[i] & 0xff); // prints the lowest byte (by pointing to bits 0-7 of the buffer item) of the int in hex for each value in buf
				if((buf[i] & 0xff) < 0x10){ // another spacing if, gives one-digit values an extra space
					printf(" ");
				}
			}
			printf("\n"); // move to a new line after the printing
			lseek(floppyDrive, 0L, 0); // needed to return to start of file
			sectorNum = 0; // reset sector number for repeated use
        }
        else if ((strcmp(command, "showfat") == 0) && (mounted == 1))
        {

        }
		else if ((strcmp(command, "showfile") == 0) && (mounted == 1))
		{

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
        scanf("%s", command);
    }


}
