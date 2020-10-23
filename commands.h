#ifndef __COMMANDS__
#define __COMMANDS__

int fmount(char *arg);
void fumount(int floppyDrive);
void help();
void structure(int floppyDrive);
void traverse(char *arg, int floppyDrive, int clusterRecur, char *topDir);
void showsector(char *arg, int floppyDrive);

#endif
