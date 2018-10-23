#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

int getPermissionBits(char * filename);
int convertNewPermissionBits(char * av);

int main(int ac, char * av[]) {
	if ((ac != 2) && (ac != 3)) {
		printf("SYNOPSIS: cm FILE [rwx]\n");
		return 0;
	}
	int currentBits = getPermissionBits(av[1]);
	if (currentBits == -1) {
		printf("File does not exist!\n");
		return 0;
	}
	currentBits = currentBits & 0177077;	//Clear the owner permission bits
	if (ac == 2) {//when permission code is not specified
		if (chmod(av[1],currentBits) == -1)
			printf("Failed to change file permission.\n");
		return 0;
	}
	int newBits = convertNewPermissionBits(av[2]);
	if (newBits == -1) {
		printf("Incorrect permission code!\n");
		return 0;
	}
	newBits = newBits << 6;					//Shift the 3 new bits to the location of owner permission bits
	newBits = newBits | currentBits;
	if (chmod(av[1],newBits) == -1)
		printf("Failed to change file permission.\n");
	return 0;
}

int getPermissionBits(char * filename) {
	struct stat temp;
	if (stat(filename, &temp) == 0) {
			return temp.st_mode;
	}
	return -1;
}
int convertNewPermissionBits(char * av) {
	int newBits = 0;
	if (strlen(av) > 3)
		return -1;
	else if (strlen(av) == 3) {		//rwx
		if (av[0] == 'r')
			newBits += 04;	//newBits = 0100 in binary
		if (av[1] == 'w')
			newBits += 02;	//newBits = 0010 in binary
		if (av[2] == 'x')
			newBits += 01;	//newBits = 0001 in binary
		if (newBits != 07)
			return -1;
	}
	else if (strlen(av) == 2) {
		if (av[0] == 'r')
			newBits += 04;	//newBits = 0100 in binary
		if (av[1] == 'w')
			newBits += 02;	//newBits = 0010 in binary
		if (av[1] == 'x')
			newBits += 01;
		if ((newBits != 05) && (newBits != 06))
			return -1;
	}
	else if (strlen(av) == 1) {
		if (av[0] == 'r')
			newBits += 04;
		if (newBits != 04)
			return -1;
	} else {}
	return newBits;
}