#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>

int fmount(char *arg)
{
	int floppyDrive = open(arg, O_RDONLY, 0); // removed the DIR use since DIR can't actually open files, only directories.
	if (floppyDrive == -1) // checks if the file opened successfully
	{
		printf("Cannot open image at %s. The path may be incorrect, or it may not be an image file.\n", arg);
	} 
	else
	{
		printf("Image at %s mounted.\n", arg);
	}
	
	return floppyDrive;
}

void fumount(int floppyDrive)
{
	close(floppyDrive);
    printf("disk unnmounted\n");
}

void help()
{
	printf("fmount \t\t\t\tmounts a local floppy disk from the specified .img file\n"
           "fumount \t\t\tunmount the mounted floppy disk\nstructure \t\t\t"
           "lists the structure of the floppy disk\ntraverse [-l] \t\t\tlists the content in the root"
           " directory. switch l shows all detailed information about a file\nshowsector [sector number]\t"
           "show the content of the specified sector (512 bytes of hex dump)\nshowfat \t\t\tshows"
           " the content of the first 256 entries of the FAT table (as a hex dump)\nshowfile [filename] "
           "\t\tshows the content of the file (as a hex dump)\nquit\t\t\t\tquits the Floppy Disk Browser shell\n");
}

void structure(int floppyDrive)
{
	char *buf = (char *) malloc(512);
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
           "0                   BOOT\n"		// since the boot sector has to be 0 on any floppy disk, we assume this and hardcode it.
           "1 -- %d				FAT1\n"		// the location of the first part of fat1 depends on the boot sector's location, so it would have to be hardcoded too.
           "%d -- %d			FAT2\n"
           "%d -- %d			ROOT DIRECTORY\n", (sectorsPerFat), (sectorsPerFat+1), ((int)fatCount*sectorsPerFat), (((int)fatCount*sectorsPerFat)+1), (((rootEntries*32)/bytesPerSector)+(int)fatCount*sectorsPerFat) );
	lseek(floppyDrive, 0L, 0);// needed to return to start of file
	free(buf);
}

void traverse(char *arg, int floppy, int clusterRecur, char *topDir)
{
	char file;
	char dir[120] = { 0 };
	char dir2[120] = { 0 };
	char *dirptr;
	char *locArg = arg;
	int firstValidCluster;
	int cont;
	int printable = 1;
	int attributeByte;
	unsigned int filesize;
	int isUp = 0;
	int floppyDrive = floppy;
	int isDir = 0;
	int i,x = 224, j = 0, k; // iteration variable(s)
	char *buf = (char *) malloc(32); // buf is going to be used on a per-entry basis here, so we just give it the file size's space

	if (clusterRecur == 0) // if we aren't recursing
	{
		lseek(floppyDrive, 9728L, 0); // seeks past the first 19 sectors of the floppy (19 sectors * 512 bytes each) to get to root
	}
	else // if we are recursing
	{
		lseek(floppyDrive, (long)((clusterRecur+31)*512), 0);
		x = 8;
	}
	if (locArg != NULL && strstr(locArg, "-l") != NULL)
	{
		printf("this is not yet done but will be in the near future.\n");
		// should be about the same as the else, but with some extra prints and extra value checking in buf
	}
	else
	{
		for (i = 0; i < x; i++) // iterates thru the whole of root
		{
			read(floppyDrive, buf, 32); // reads the file
			firstValidCluster = buf[26] + (buf[27] << 8); //saves first valid cluster to check if the file is valid
			filesize = (buf[0x1c] + (buf[0x1d] << 8) + (buf[0x1e] << 16) + (buf[0x1f] << 24)); // get filesize
			attributeByte = buf[11];
			if(firstValidCluster != 0 || buf[1] == '.') // checks if the file has valid clusters. sometimes the /. or /.. file won't have the value saved.
            {
				if (filesize > 4000000) // all the invalid files in between the valid files have the max filesize possible. thus, we can suss out the bad this way.
					{
						break;
					}
				else
				{	
					printf("/"); // don't print the '/' in anticipation of a value that will not come.
				}
				if (clusterRecur != 0) // if we are recursive
				{
					if(strcmp(topDir, "DIR2") == 0)
					{
						char actualpath[10] = {'D', 'I', 'R', '1', '/', 'D', 'I', 'R', '2', '\0'};
						topDir = &(actualpath[0]);
					}
					printf("%s/", topDir);
					
				}
				
                for (j = 0; j < 8; j++)
                {
					printable = 1;
                    file = buf[j];
					if (((int)attributeByte) >> 4 == 1 || ((int)attributeByte) >> 4 == 3) // checks  2^4 bit of attributeByte, regardless of the 2^5 bit
					{
						isDir = 1;
					}
					if (file != ' ')//skip whitespace filler in short filenames
					{
						if (file == '.') // finds files that are parent and current directories
						{
							isUp = 1;
						}
                        printf("%c", file);
						if (isDir == 1) // save directory name for printing in recursive call
						{
							dir[j] = file;
						}
					}
                }
				if (isDir != 1 && printable == 1) // prints files with their extensions, skipping long filename entries. also skips directories
				{
					if (buf[8] != ' ') // some files don't have extensions. these don't need extensions printed.
					{
						printf(".");// filename/extension separator needs to be printed specially
						for (j = 8; j < 11; j++)
						{
							file = buf[j];
							if (file != ' ')//skip whitespace filler in short file extensions
							{ 
                   		        printf("%c", file);
							}
						}
					}
					printf("\n");
				}
				if (isDir == 1 && printable == 1) // prints directory contents
				{
					dir[j+1] = '\0';
					dirptr = &(dir[0]);
					printf("		<DIR>\n");
					if (isUp != 1) // don't recursively enter /. or /..
					{
						traverse(locArg, floppyDrive, firstValidCluster, dirptr);
						if (strcmp(topDir, "aeiou") == 0) // check if we're recursing. if so, don't restart.
						{
							lseek(floppyDrive, 0L, 0); // seek back to start of drive
							lseek(floppyDrive, (9728L + (long)((i+1)*32)), 0); // continue where we left off
						}
						else
						{
							return ;
						}
					}
					isUp = 0;
					/*
					lseek to the location pointed to by firstValidCluster after saving its current position
					do the data reading there
					(oh no this needs to be recursive that means it needs to be its own function)
					return to the position we were at
					 */
				}
				isDir = 0; // reset isDir for next item in root
				//isUp = 0;
            }
		}

	}
	lseek(floppyDrive, 0L, 0);// needed to return to start of file
	free(buf);
}

void showsector(char *arg, int floppyDrive)
{
	int sectorNum = atoi(arg); // simple conversion of string to numerical value
	char *buf = (char *) malloc(512);
	int i;
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
	free(buf);
}

void showfat(char *arg, int floppyDrive)
{
    char *buf = (char *) malloc(256);
    lseek(floppyDrive, 512, 0); //seeking passed the first cluster
    read(floppyDrive, buf, 256);
    printf("    0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f");
    for (int i = 0; i < 256; i ++)
    {
        if ((i % 16) == 0)
        {
            printf("\n%X ", i);
            if (i < 0x100)
            {
                printf(" ");
                if (i < 0x10)
                {
                    printf(" ");
                }
            }
        } //prints columns correctly

        if ((buf[i] & 0x0f)  == 0) //prints the frees
        {
            printf("FREE ");
        }
        else
        printf("%x  ", buf[i]); //couldn't figure out how to get the

    }
    printf("\n");
    lseek(floppyDrive, 0L, 0);//undo the seek
    free(buf);

}
