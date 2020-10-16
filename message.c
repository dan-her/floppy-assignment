#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>


void help()
{
    printf("fmount \t\t\tmounts a local floppy disk from the specified .img file\n"
           "fumount \t\t\t\t\t\t\tunmount the mounted floppy disk\nstructure \t\t\t\t\t\t\t"
           "lists the structure of the floppy disk\ntraverse [-l] \t\t\t\t\t\tlists the content in the root"
           " directory. switch l shows all detailed information about a file\nshowsector [sector number]\t\t"
           "\tshow the content of the specified sector (512 bytes of hex dump)\nshowfat \t\t\t\t\t\t\tshows"
           " the content of the first 256 entries of the FAT table (as a hex dump)\nshowfile [filename] "
           "\t\t\t\tshows the content of the file (as a hex dump)\nquit\t\t\t\t\t\t\t\tquits the Floppy Disk Browser shell\n");
}

void quit()
{

}

int main(int argc, char *argv[])
{
    char command[128];
    char *path;
    char *buf;

    int mounted = 0;//for checking if floppy has actually been mounted in fumount
    int sectorNum = 0;
    int floppyDrive;

    printf("enter floppy command: ");
    scanf("%s", command);

    while (strcmp(command, "quit") != 0)
    {
        if (strcmp(command, "fmount") == 0)
        {
            printf("enter path of floppy to be mounted: \n");
            char floppyPath[128];
            scanf("%s", floppyPath);
            floppyDrive = open(floppyPath, O_RDONLY, 0);//removed the DIR use since DIR can't actually open files, only directories.
			if (floppyDrive == -1)//checks if the file opened successfully
			{
				printf("Cannot open image at %s. The path may be incorrect, or it may not be an image file.\n", floppyPath);
			} 
			else
			{
            printf("Image at %s mounted.\n", floppyPath);
			}
            mounted = 1;

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
        else if (strcmp(command, "structure") == 0)
        {
            buf = (char *) malloc(512);
            read(floppyDrive, buf, 512); //puts the boot sector into memory
            char bytesPerSector = buf[11];
			printf("bytesPerSector = %d\n", bytesPerSector);
            char sectorsPerCluster = buf[13];
			printf("sectorsPerCluster = %d\n", sectorsPerCluster);
            char fatCount = buf[16];
			printf("fatCount = %d\n", fatCount);
            unsigned char rootEntries = buf[17];
			printf("rootEntries = %d\n", rootEntries);
            int sectorsPerFat = buf[22];
			printf("sectorsPerFat = %d\n", sectorsPerFat);

            printf("number of FAT:			    %c\nnumber of sectors used by FAT:	    %d\nnumber of sectors per cluster:	    %c\n"
            "number of ROOT Entries:		  %d\nnumber of bytes per sector:	  %d\n",
            fatCount, sectorsPerFat, sectorsPerCluster, rootEntries, bytesPerSector); // just printing in given format, separated into two parts so more comments can fit
            printf(""
                   "---Sector #---     ---Sector Types---\n"
                   "0                   BOOT\n"    // since the boot sector has to be 0 on any floppy disk, can we assume this and hardcode it? how else to find boot sector #?
                   "1 -- %d				FAT1\n" 	// the location of the first part of fat1 depends on the boot sector's location, so it would have to be hardcoded too, probably
                   "%d -- %d			FAT2\n"	// both of the above comments are probably the wrong way to do it, since we aren't supposed to hardcode the numbers in this part
                   "%d -- %d			ROOT DIRECTORY\n", (sectorsPerFat), (sectorsPerFat+1), ((int)fatCount*sectorsPerFat), (((int)fatCount*sectorsPerFat)+1), ((rootEntries*32)/3/*temp value while we fix bytesPerSector*/ /*bytesPerSector*/) );
        }
        else if (strcmp(command, "traverse") == 0)
        {

        }
        else if (strcmp(command, "showsector") == 0)
        {

        }
        else if (strcmp(command, "showfat") == 0)
        {

        }

        else
        {
            printf("unknown command. to see a list of commands, type help\n");
        }


        printf("enter floppy command: ");
        scanf("%s", command);
    }


}
