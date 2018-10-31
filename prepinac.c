#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "prepinac.h"

int slow(char infile[], char outfile[]) {
	char buf[1];
	
	int in;
	int out;
	
	ssize_t rd, wr;
		
	in = open(infile, O_RDONLY);
	if (in == -1) { 
		fprintf(stderr, "-a: %d\n", errno);
    	fprintf(stderr, "-a: %s\n", strerror(errno));
    	fprintf(stderr, "-a: SUBOR NEEXISTUJE\n");
		return 21;
	}
	out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); //opening a file for 
	if (out == -1) { //writing and truncating it to zero length. If file does not exist 
		fprintf(stderr, "-d: %d\n", errno); //- create one.
    	fprintf(stderr, "-d: %s\n", strerror(errno));
    	fprintf(stderr, "-d: INA CHYBA\n");
		return 21;
	}

	while ( (rd = read(in, buf, 1)) != 0 ) { //in normal conditions, we will have
		if (rd == -1) { //int ret number of bytes, written to the buf. 0 - EOF
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 21;
		}
		wr = write(out, buf, rd); //writing all bytes, that was read
		if (wr == -1) { //if fail, inform
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 21;
		}
		
	}
		
	close(in);
	close(out);
	
	return 0;
}

int mytruncate(char infile[], char outfile[], int size) {
	char buf[256];
	
	int in;
	int out;
	
	ssize_t rd, wr;
		
	in = open(infile, O_RDWR); 
	if (in == -1) {
		fprintf(stderr, "-t: %d\n", errno);
    	fprintf(stderr, "-t: %s\n", strerror(errno));
    	fprintf(stderr, "-t: INA CHYBA\n");
		return 31;
	}
	out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
	if (out == -1) {
		fprintf(stderr, "-t: %d\n", errno);
    	fprintf(stderr, "-t: %s\n", strerror(errno));
    	fprintf(stderr, "-t: INA CHYBA\n");
		return 31;
	}

	while ( (rd = read(in, buf, 256)) != 0 ) { 
		if (rd == -1) { 
			fprintf(stderr, "-t: %d\n", errno);
    		fprintf(stderr, "-t: %s\n", strerror(errno));
    		fprintf(stderr, "-t: INA CHYBA\n");
			return 31;
		}
		wr = write(out, buf, rd); 
		if (wr == -1) { 
			fprintf(stderr, "-t: %d\n", errno);
    		fprintf(stderr, "-t: %s\n", strerror(errno));
    		fprintf(stderr, "-t: INA CHYBA\n");
			return 31;
		}
		
	}

	struct stat *infl = calloc(1, sizeof(struct stat));
	fstat(in, infl);

	close(in);

	if ((S_ISREG(infl->st_mode)) != 0) {
		in = open(infile, O_RDWR);
		if (in == -1) {
			fprintf(stderr, "-t: %d\n", errno);
    		fprintf(stderr, "-t: %s\n", strerror(errno));
    		fprintf(stderr, "-t: INA CHYBA\n");
			return 31;
		}
		if (ftruncate(in, size) != 0) {
			fprintf(stderr, "-t: %d\n", errno);
			fprintf(stderr, "-t: %s\n", strerror(errno));
			fprintf(stderr, "-t: VSTUPNY SUBOR NEZMENENY\n");
			return 31;
		}
		
		close(in);
	}
	
	close(out);
	free(infl);

	return 0;
}

int overwrite(char infile[], char outfile[]) {
	int in;
	int out;
	
	char buf[256];
		
	ssize_t rd, wr;

	in = open(infile, O_RDONLY);
	if (in == -1) {
		fprintf(stderr, "-o: %d\n", errno);
    	fprintf(stderr, "-o: %s\n", strerror(errno));
    	fprintf(stderr, "-o: INA CHYBA\n");
		return 24;
	}
	out = open(outfile, O_WRONLY); 
	if (out == -1) {
		fprintf(stderr, "-o: %d\n", errno);
    	fprintf(stderr, "-o: %s\n", strerror(errno));
    	fprintf(stderr, "-o: SUBOR NEEXISTUJE\n");
		return 24;
	}

	while ( (rd = read(in, buf, 256)) != 0 ) {
		if (rd == -1) {
			fprintf(stderr, "-o: %d\n", errno);
    		fprintf(stderr, "-o: %s\n", strerror(errno));
    		fprintf(stderr, "-o: INA CHYBA\n");
			return 26;
		}
		wr = write(out, buf, rd);
		if (wr == -1) { 
			fprintf(stderr, "-o: %d\n", errno);
    		fprintf(stderr, "-o: %s\n", strerror(errno));
    		fprintf(stderr, "-o: INA CHYBA\n");
			return 26;
		}
		
	}

	close(in);
	close(out);
	
	return 0;
}

int mchmod(char infile[], char outfile[], char *argument) {

	char buf[256];
	
	int in;
	int out;
	
	ssize_t rd, wr;
		
	in = open(infile, O_RDONLY); 
	if (in == -1) { 
		fprintf(stderr, "-a: %d\n", errno);
    	fprintf(stderr, "-a: %s\n", strerror(errno));
    	fprintf(stderr, "-a: SUBOR NEEXISTUJE\n");
		return 21;
	}
	out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
	if (out == -1) { 
		fprintf(stderr, "-d: %d\n", errno); 
    	fprintf(stderr, "-d: %s\n", strerror(errno));
    	fprintf(stderr, "-d: INA CHYBA\n");
		return 21;
	}

	while ( (rd = read(in, buf, 256)) != 0 ) { 
		if (rd == -1) { 
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 21;
		}
		wr = write(out, buf, rd); 
		if (wr == -1) { 
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 21;
		}
		
	}
		
	close(in);
	close(out);

	mode_t flmode = 0;

	//if digits in the argument
	bool isright = true;
	int arglen = strlen(argument);
	if (arglen != 4) isright = false; else {
		for (int i = 0; i < arglen; i++) {
			if (isdigit(argument[i]) == 0) isright = false;
		}
	}

	if (isright) {

		char *ptr;
		long ret = strtoul(argument, &ptr, 8);

		rd = chmod(outfile, (mode_t) ret);
		if (rd != 0) {
			fprintf(stderr, "-m: %d\n", errno);
    		fprintf(stderr, "-m: %s\n", strerror(errno));
    		fprintf(stderr, "-m: INA CHYBA\n");
			return 21;
		}

	} else {
		isright = true;
		if (arglen != 9) isright = false; else {
			for (int i = 0; i < arglen; i++) {
				if (i % 3 == 0) {
					//checking for the r or -
					if (argument[i] != 'r' && argument[i] != '-') isright = false;
				}
				if (i % 3 == 1) {
					//checking for the w or -
					if (argument[i] != 'w' && argument[i] != '-') isright = false;
				}
				if (i % 3 == 2) {
					//checking for the x or -
					if (argument[i] != 'x' && argument[i] != '-') isright = false;
				}
			}
		}

		if (!isright) {
			fprintf(stderr, "-m: %d\n", errno);
    		fprintf(stderr, "-m: %s\n", strerror(errno));
    		fprintf(stderr, "-m: ZLE PRAVA\n");
			return 21;
		}

		//here we're performing chmod if argument is in rwx form
		for (int i = 0; i < arglen; i++) {
			if (i >= 0 && i <= 2) {
				if (argument[i] == 'r') {
					flmode = flmode | S_IRUSR;
				}
				else if (argument[i] == 'w') {
					flmode = flmode | S_IWUSR;
				}
				else if (argument[i] == 'x') {
					flmode = flmode | S_IXUSR;
				}
			}

			if (i >= 3 && i <=5) {
				if (argument[i] == 'r') {
					flmode = flmode | S_IRGRP;
				}
				else if (argument[i] == 'w') {
					flmode = flmode | S_IWGRP;
				}
				else if (argument[i] == 'x') {
					flmode = flmode | S_IXGRP;
				}
			}

			if (i >= 6 && i <=8) {
				if (argument[i] == 'r') {
					flmode = flmode | S_IROTH;
				}
				else if (argument[i] == 'w') {
					flmode = flmode | S_IWOTH;
				}
				else if (argument[i] == 'x') {
					flmode = flmode | S_IXOTH;
				}
			}
		}

		rd = chmod(outfile, flmode);
		if (rd != 0) {
			fprintf(stderr, "-m: %d\n", errno);
    		fprintf(stderr, "-m: %s\n", strerror(errno));
    		fprintf(stderr, "-m: INA CHYBA\n");
			return 21;
		}

	}

	return 0;

}

int mumask(char infile[], char outfile[], char *argument) {

	int arglen = strlen(argument);

	if (arglen < 3) {
		fprintf(stderr, "-u: %d\n", errno);
    	fprintf(stderr, "-u: %s\n", strerror(errno));
    	fprintf(stderr, "-u: ZLA MASKA\n");
		return 32;
	}

	mode_t oldmask, newmask;
	newmask = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	oldmask = umask(newmask);

	for (int i = 0; i < arglen; i++) {

		if (i % 4 == 0) {
			//checking for the parameters
			if (i+2 >= arglen) {
				fprintf(stderr, "-u: %d\n", errno);
    			fprintf(stderr, "-u: %s\n", strerror(errno));
    			fprintf(stderr, "-u: ZLA MASKA\n");
				return 32;
			}

			if ((argument[i] != 'u' && argument[i] != 'g' && argument[i] != 'o') || (argument[i+1] != '+' && argument[i+1] != '-') || (argument[i+2] != 'r' && argument[i+2] != 'w' && argument[i+2] != 'x')) {
				fprintf(stderr, "-u: %d\n", errno);
    			fprintf(stderr, "-u: %s\n", strerror(errno));
    			fprintf(stderr, "-u: ZLA MASKA\n");
				return 32;
			}

			if (argument[i] == 'u') {
				if (argument[i+1] == '+') {
					if (argument[i+2] == 'r') {
						if (oldmask & S_IRUSR) {
							oldmask = (oldmask ^ S_IRUSR);
						}
					}
					if (argument[i+2] == 'w') {
						if (oldmask & S_IWUSR) {
							oldmask = (oldmask ^ S_IWUSR);
						}
					}
					if (argument[i+2] == 'x') {
						if (oldmask & S_IXUSR) {
							oldmask = (oldmask ^ S_IXUSR);
						}
					}
				}
				if (argument[i+1] == '-') {
					if (argument[i+2] == 'r') {
						oldmask = (oldmask | S_IRUSR);
					}
					if (argument[i+2] == 'w') {
						oldmask = (oldmask | S_IWUSR);
					}
					if (argument[i+2] == 'x') {
						oldmask = (oldmask | S_IXUSR);
					}
				}
			}

			if (argument[i] == 'g') {
				if (argument[i+1] == '+') {
					if (argument[i+2] == 'r') {
						if (oldmask & S_IRGRP) {
							oldmask = (oldmask ^ S_IRGRP);
						}
					}
					if (argument[i+2] == 'w') {
						if (oldmask & S_IWGRP) {
							oldmask = (oldmask ^ S_IWGRP);
						}
					}
					if (argument[i+2] == 'x') {
						if (oldmask & S_IXGRP) {
							oldmask = (oldmask ^ S_IXGRP);
						}
					}
				}

				if (argument[i+1] == '-') {
					if (argument[i+2] == 'r') {
						oldmask = (oldmask | S_IRGRP);
					}
					if (argument[i+2] == 'w') {
						oldmask = (oldmask | S_IWGRP);
					}
					if (argument[i+2] == 'x') {
						oldmask = (oldmask | S_IXGRP);
					}
				}

			}

			if (argument[i] == 'o') {
				if (argument[i+1] == '+') {
					if (argument[i+2] == 'r') {
						if (oldmask & S_IROTH) {
							oldmask = (oldmask ^ S_IROTH);
						}
					}
					if (argument[i+2] == 'w') {
						if (oldmask & S_IWOTH) {
							oldmask = (oldmask ^ S_IWOTH);
						}
					}
					if (argument[i+2] == 'x') {
						if (oldmask & S_IXOTH) {
							oldmask = (oldmask ^ S_IXOTH);
						}
					}
				}
				
				if (argument[i+1] == '-') {
					if (argument[i+2] == 'r') {
						oldmask = (oldmask | S_IROTH);
					}
					if (argument[i+2] == 'w') {
						oldmask = (oldmask | S_IWOTH);
					}
					if (argument[i+2] == 'x') {
						oldmask = (oldmask | S_IXOTH);
					}
				}

			}

		}
	}

	umask(oldmask);

	char buf[256];
	
	int in;
	int out;
	
	ssize_t rd, wr;
		
	in = open(infile, O_RDONLY); 
	if (in == -1) { 
		fprintf(stderr, "-a: %d\n", errno);
    	fprintf(stderr, "-a: %s\n", strerror(errno));
    	fprintf(stderr, "-a: SUBOR NEEXISTUJE\n");
		return 21;
	}
	out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
	if (out == -1) {  
		fprintf(stderr, "-d: %d\n", errno); 
    	fprintf(stderr, "-d: %s\n", strerror(errno));
    	fprintf(stderr, "-d: INA CHYBA\n");
		return 21;
	}

	while ( (rd = read(in, buf, 256)) != 0 ) { 
		if (rd == -1) { 
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 21;
		}
		wr = write(out, buf, rd); 
		if (wr == -1) { 
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 21;
		}
		
	}
		
	close(in);
	close(out);
	
	return 0;

}

int nothing(char infile[], char outfile[]) {
	char buf[256];
	
	int in;
	int out;
	
	ssize_t rd, wr;
		
	in = open(infile, O_RDONLY); //getting file descriptor of infile
	if (in == -1) { //if opening failed - print error
		fprintf(stderr, "-a: %d\n", errno);
    	fprintf(stderr, "-a: %s\n", strerror(errno));
    	fprintf(stderr, "-a: SUBOR NEEXISTUJE\n");
		return 21;
	}
	out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); //opening a file for 
	if (out == -1) { //writing and truncating it to zero length. If file does not exist 
		fprintf(stderr, "-d: %d\n", errno); //- create one.
    	fprintf(stderr, "-d: %s\n", strerror(errno));
    	fprintf(stderr, "-d: INA CHYBA\n");
		return 21;
	}

	while ( (rd = read(in, buf, 256)) != 0 ) { //in normal conditions, we will have
		if (rd == -1) { //int ret number of bytes, written to the buf. 0 - EOF
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 21;
		}
		wr = write(out, buf, rd); //writing all bytes, that was read
		if (wr == -1) { //if fail, inform
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 21;
		}
		
	}
		
	close(in);
	close(out);
	
	return 0;
}

int append(char infile[], char outfile[]) {
	
	char buf[256];
	
	int in;
	int out;
	
	ssize_t rd, wr;
		
	in = open(infile, O_RDONLY); //getting file descriptor of infile
	if (in == -1) { //if opening failed - print error
		fprintf(stderr, "-a: %d\n", errno);
    	fprintf(stderr, "-a: %s\n", strerror(errno));
    	fprintf(stderr, "-a: SUBOR NEEXISTUJE\n");
		return 22;
	}
	out = open(outfile, O_WRONLY | O_APPEND); //opening a file for  writing
	if (out == -1) {  
		fprintf(stderr, "-a: %d\n", errno);
    	fprintf(stderr, "-a: %s\n", strerror(errno));
    	fprintf(stderr, "-a: SUBOR NEEXISTUJE\n");
		return 22;
	}
		
	while ( (rd = read(in, buf, 256)) != 0 ) { //in normal conditions, we will have
		if (rd == -1) { //int ret number of bytes, written to the buf. 0 - EOF
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 22;
		}
		wr = write(out, buf, rd); //writing all bytes, that was read
		if (wr == -1) { //if fail, inform
			fprintf(stderr, "-a: %d\n", errno);
    		fprintf(stderr, "-a: %s\n", strerror(errno));
    		fprintf(stderr, "-a: INA CHYBA\n");
			return 22;
		}
		
	}
		
	close(in);
	close(out);
	
	return 0;
	
}

int delete(char infile[], char outfile[]) {
	
	int in;
	int out;
	
	char buf[256];
		
	ssize_t rd, wr;
		
	in = open(infile, O_RDONLY); //getting file descriptor of infile
	if (in == -1) { //if opening failed - print error
		fprintf(stderr, "-d: %d\n", errno);
    	fprintf(stderr, "-d: %s\n", strerror(errno));
    	fprintf(stderr, "-d: INA CHYBA\n");
		return 26;
	}
	out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); //opening a file for 
	if (out == -1) { //writing and truncating it to zero length. If file does not exist 
		fprintf(stderr, "-d: %d\n", errno); //- create one.
    	fprintf(stderr, "-d: %s\n", strerror(errno));
    	fprintf(stderr, "-d: INA CHYBA\n");
		return 26;
	}
		
	while ( (rd = read(in, buf, 256)) != 0 ) { //in normal conditions, we will have
		if (rd == -1) { //int ret number of bytes, written to the buf. 0 - EOF
			fprintf(stderr, "-d: %d\n", errno);
    		fprintf(stderr, "-d: %s\n", strerror(errno));
    		fprintf(stderr, "-d: INA CHYBA\n");
			return 26;
		}
		wr = write(out, buf, rd); //writing all bytes, that was read
		if (wr == -1) { //if fail, inform
			fprintf(stderr, "-d: %d\n", errno);
    		fprintf(stderr, "-d: %s\n", strerror(errno));
    		fprintf(stderr, "-d: INA CHYBA\n");
			return 26;
		}
		
	}
	
	close(in);
	close(out);
	
	int ret = remove(infile);
	
	if (ret == -1) {
		fprintf(stderr, "-d: %d\n", errno);
    	fprintf(stderr, "-d: %s\n", strerror(errno));
    	fprintf(stderr, "-d: SUBOR NEBOL ZMAZANY\n");
		return 26;
	}
	
	return 0;
	
}

int linkk(char infile[], char outfile[]) {
	int in;
	int out;

	in = open(infile, O_RDONLY);
	if (in == -1) {
		fprintf(stderr, "-K: %d\n", errno);
    	fprintf(stderr, "-K: %s\n", strerror(errno));
    	fprintf(stderr, "-K: VSTUPNY SUBOR NEEXISTUJE\n");
		return 30;
	}

	int r = link(infile, outfile);
	if (r != 0) {
		fprintf(stderr, "-K: %d\n", errno);
    	fprintf(stderr, "-K: %s\n", strerror(errno));
    	fprintf(stderr, "-K: VYSTUPNY SUBOR NEVYTVORENY\n");
		return 30;
	}
	
	out = open(outfile, O_RDONLY);
	if (out == -1) {
		fprintf(stderr, "-K: %d\n", errno);
    	fprintf(stderr, "-K: %s\n", strerror(errno));
    	fprintf(stderr, "-K: INA CHYBA\n");
		return 30;
	}

	return 0;
}

int fast(char infile[], char outfile[]) {
	int in;
	int out;
	
	ssize_t rd, wr;

	in = open(infile, O_RDONLY);
	if (in == -1) { 
		fprintf(stderr, "-a: %d\n", errno);
    	fprintf(stderr, "-a: %s\n", strerror(errno));
    	fprintf(stderr, "-a: SUBOR NEEXISTUJE\n");
		return 21;
	}
	out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out == -1) {
		fprintf(stderr, "-d: %d\n", errno);
    	fprintf(stderr, "-d: %s\n", strerror(errno));
    	fprintf(stderr, "-d: INA CHYBA\n");
		return 21;
	}

	struct stat *infl = calloc(1, sizeof(struct stat));
	fstat(in, infl);
	int size = infl->st_size;
	free(infl);
	char *buf = calloc(size, sizeof(char));

	rd = read(in, buf, size);
	if (rd == -1) {
		fprintf(stderr, "-a: %d\n", errno);
    	fprintf(stderr, "-a: %s\n", strerror(errno));
    	fprintf(stderr, "-a: INA CHYBA\n");
		return 21;
	}

	wr = write(out, buf, size);
	if (wr == -1) {
		fprintf(stderr, "-a: %d\n", errno);
    	fprintf(stderr, "-a: %s\n", strerror(errno));
    	fprintf(stderr, "-a: INA CHYBA\n");
		return 21;
	}

	close(in);
	close(out);
	free(buf);

	return 0;
}

int mylseek(char infile[], char outfile[], char *argument) {

	int alen = strlen(argument);
	int comas = 0;

	for (int x = 0; x < alen; x++) {
		if (argument[x] == ',') comas++;
	}

	if (comas != 3) {
		fprintf(stderr, "-l: %d\n", errno);
    	fprintf(stderr, "-l: %s\n", strerror(errno));
    	fprintf(stderr, "-l: INA CHYBA\n");
		return 33;
	}

	char x = argument[0];
	if (x != 'e' && x != 'b' && x != 'c') {
		fprintf(stderr, "-l: %d\n", errno);
    	fprintf(stderr, "-l: %s\n", strerror(errno));
    	fprintf(stderr, "-l: INA CHYBA\n");
		return 33;
	}
	char pos1[alen];
	char pos2[alen];
	char num[alen];

	int i = 2;

	while (i != alen) {
		if (argument[i] == ',') {
			pos1[i-2] = '\0';
			break;
		}
		if (isdigit(argument[i]) == 0) {
			fprintf(stderr, "-l: %d\n", errno);
    		fprintf(stderr, "-l: %s\n", strerror(errno));
    		fprintf(stderr, "-l: CHYBA POZICIE %s\n", infile);
			return 33;
		}

		pos1[i-2] = argument[i];

		i++;
	}

	i++;
	comas = 0;

	while (i != alen) {
		if (argument[i] == ',') {
			pos2[comas] = '\0';
			break;
		}
		if (isdigit(argument[i]) == 0) {
			fprintf(stderr, "-l: %d\n", errno);
    		fprintf(stderr, "-l: %s\n", strerror(errno));
    		fprintf(stderr, "-l: CHYBA POZICIE %s\n", outfile);
			return 33;
		}

		pos2[comas] = argument[i];

		comas++;
		i++;
	}

	i++;
	comas = 0;

	while (i != alen) {
		if (isdigit(argument[i]) == 0) {
			fprintf(stderr, "-l: %d\n", errno);
    		fprintf(stderr, "-l: %s\n", strerror(errno));
    		fprintf(stderr, "-l: INA CHYBA\n");
			return 33;
		}

		num[comas] = argument[i];

		comas++;
		i++;
	}

	num[comas] = '\0';

	int ipos1 = atoi(pos1);
	int ipos2 = atoi(pos2);
	int inum = atoi(num);

	int in = open(infile, O_RDONLY);
	if (in == -1) { 
		fprintf(stderr, "-l: %d\n", errno);
    	fprintf(stderr, "-l: %s\n", strerror(errno));
    	fprintf(stderr, "-l: INA CHYBA\n");
		return 33;
	}
	int out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out == -1) {
		fprintf(stderr, "-l: %d\n", errno);
    	fprintf(stderr, "-l: %s\n", strerror(errno));
    	fprintf(stderr, "-l: INA CHYBA\n");
		return 33;
	}

	struct stat *infl = calloc(1, sizeof(struct stat));
	fstat(in, infl);
	int insize = infl->st_size;

	if (ipos1 > insize) {
		fprintf(stderr, "-l: %d\n", errno);
    	fprintf(stderr, "-l: %s\n", strerror(errno));
    	fprintf(stderr, "-l: CHYBA POZICIE %s\n", infile);
		return 33;
	}

	int rt = lseek(in, (off_t) ipos1, SEEK_SET);
	if (rt != (off_t) ipos1) {
		fprintf(stderr, "-l: %d\n", errno);
    	fprintf(stderr, "-l: %s\n", strerror(errno));
    	fprintf(stderr, "-l: CHYBA POZICIE %s\n", infile);
		return 33;
	}

	char buf[inum + 1];
	rt = read(in, buf, inum);
	if (rt == -1) {
		fprintf(stderr, "-l: %d\n", errno);
    	fprintf(stderr, "-l: %s\n", strerror(errno));
    	fprintf(stderr, "-l: INA CHYBA\n");
		return 33;
	}

	if (x == 'b') {
		rt = lseek(out, (off_t) ipos2, SEEK_SET);
		if (rt == (off_t) -1) {
			fprintf(stderr, "-l: %d\n", errno);
    		fprintf(stderr, "-l: %s\n", strerror(errno));
    		fprintf(stderr, "-l: INA CHYBA\n");
			return 33;
		}
	}
	else if (x == 'e') {
		rt = lseek(out, (off_t) ipos2, SEEK_END);
		if (rt == (off_t) -1) {
			fprintf(stderr, "-l: %d\n", errno);
    		fprintf(stderr, "-l: %s\n", strerror(errno));
    		fprintf(stderr, "-l: INA CHYBA\n");
			return 33;
		}
	}
	else if (x == 'c') {
		rt = lseek(out, (off_t) ipos2, SEEK_CUR);
		if (rt == (off_t) -1) {
			fprintf(stderr, "-l: %d\n", errno);
    		fprintf(stderr, "-l: %s\n", strerror(errno));
    		fprintf(stderr, "-l: INA CHYBA\n");
			return 33;
		}
	}

	rt = write(out, buf, inum);
	if (rt == -1) {
		fprintf(stderr, "-l: %d\n", errno);
    	fprintf(stderr, "-l: %s\n", strerror(errno));
    	fprintf(stderr, "-l: INA CHYBA\n");
		return 33;
	}

	close(in);
	close(out);
	free(infl);

	return 0;
}

int inode(char infile[], char outfile[], long number) {

	int in = open(infile, O_RDONLY);
	if (in == -1) { 
		fprintf(stderr, "-i: %d\n", errno);
    	fprintf(stderr, "-i: %s\n", strerror(errno));
    	fprintf(stderr, "-i: INA CHYBA\n");
		return 27;
	}

	struct stat *infl = calloc(1, sizeof(struct stat));
	fstat(in, infl);

	if ((S_ISREG(infl->st_mode)) == 0) {
		fprintf(stderr, "-i: %d\n", errno);
    	fprintf(stderr, "-i: %s\n", strerror(errno));
    	fprintf(stderr, "-i: ZLY TYP VSTUPNEHO SUBORU\n");
    	free (infl);
		return 27;
	}

	if (number != (long) infl->st_ino) {
		fprintf(stderr, "-i: %d\n", errno);
    	fprintf(stderr, "-i: %s\n", strerror(errno));
    	fprintf(stderr, "-i: ZLY INODE\n");
    	free (infl);
		return 27;
	}

	int out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
	if (out == -1) { 
		fprintf(stderr, "-i: %d\n", errno); 
    	fprintf(stderr, "-i: %s\n", strerror(errno));
    	fprintf(stderr, "-i: INA CHYBA\n");
    	free (infl);
		return 27;
	}

	char buf[256];
	ssize_t rd, wr;

	while ( (rd = read(in, buf, 256)) != 0 ) { 
		if (rd == -1) { 
			fprintf(stderr, "-i: %d\n", errno);
    		fprintf(stderr, "-i: %s\n", strerror(errno));
    		fprintf(stderr, "-i: INA CHYBA\n");
    		free (infl);
			return 27;
		}
		wr = write(out, buf, rd); 
		if (wr == -1) { 
			fprintf(stderr, "-i: %d\n", errno);
    		fprintf(stderr, "-i: %s\n", strerror(errno));
    		fprintf(stderr, "-i: INA CHYBA\n");
    		free (infl);
			return 27;
		}
		
	}
		
	close(in);
	close(out);

	free(infl);

	return 0;
}

int create(char infile[], char outfile[], char *argument) {

	char buf[256];
	
	int in;
	int out;
	
	ssize_t rd, wr;
		
	in = open(infile, O_RDONLY); 
	if (in == -1) { 
		fprintf(stderr, "-c: %d\n", errno);
    	fprintf(stderr, "-c: %s\n", strerror(errno));
    	fprintf(stderr, "-c: INA CHYBA\n");
		return 23;
	}

	out = open(outfile, O_WRONLY);
	if (out != -1) {
		fprintf(stderr, "-c: %d\n", errno); 
    	fprintf(stderr, "-c: %s\n", strerror(errno));
    	fprintf(stderr, "-c: SUBOR EXISTUJE\n");
		return 23;
	}

	out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out == -1) { 
		fprintf(stderr, "-c: %d\n", errno); 
    	fprintf(stderr, "-c: %s\n", strerror(errno));
    	fprintf(stderr, "-c: INA CHYBA\n");
		return 23;
	}





	mode_t flmode = 0;

	//if digits in the argument
	bool isright = true;
	int arglen = strlen(argument);
	if (arglen != 4) isright = false; else {
		for (int i = 0; i < arglen; i++) {
			if (isdigit(argument[i]) == 0) isright = false;
		}
	}

	if (isright) {

		char *ptr;
		long ret = strtoul(argument, &ptr, 8);

		rd = chmod(outfile, (mode_t) ret);
		if (rd != 0) {
			fprintf(stderr, "-c: %d\n", errno);
    		fprintf(stderr, "-c: %s\n", strerror(errno));
    		fprintf(stderr, "-c: INA CHYBA\n");
			return 23;
		}

	} else {
		isright = true;
		if (arglen != 9) isright = false; else {
			for (int i = 0; i < arglen; i++) {
				if (i % 3 == 0) {
					//checking for the r or -
					if (argument[i] != 'r' && argument[i] != '-') isright = false;
				}
				if (i % 3 == 1) {
					//checking for the w or -
					if (argument[i] != 'w' && argument[i] != '-') isright = false;
				}
				if (i % 3 == 2) {
					//checking for the x or -
					if (argument[i] != 'x' && argument[i] != '-') isright = false;
				}
			}
		}

		if (!isright) {
			fprintf(stderr, "-c: %d\n", errno);
    		fprintf(stderr, "-c: %s\n", strerror(errno));
    		fprintf(stderr, "-c: ZLE PRAVA\n");
			return 23;
		}

		//here we're performing chmod if argument is in rwx form
		for (int i = 0; i < arglen; i++) {
			if (i >= 0 && i <= 2) {
				if (argument[i] == 'r') {
					flmode = flmode | S_IRUSR;
				}
				else if (argument[i] == 'w') {
					flmode = flmode | S_IWUSR;
				}
				else if (argument[i] == 'x') {
					flmode = flmode | S_IXUSR;
				}
			}

			if (i >= 3 && i <=5) {
				if (argument[i] == 'r') {
					flmode = flmode | S_IRGRP;
				}
				else if (argument[i] == 'w') {
					flmode = flmode | S_IWGRP;
				}
				else if (argument[i] == 'x') {
					flmode = flmode | S_IXGRP;
				}
			}

			if (i >= 6 && i <=8) {
				if (argument[i] == 'r') {
					flmode = flmode | S_IROTH;
				}
				else if (argument[i] == 'w') {
					flmode = flmode | S_IWOTH;
				}
				else if (argument[i] == 'x') {
					flmode = flmode | S_IXOTH;
				}
			}
		}

		rd = chmod(outfile, flmode);
		if (rd != 0) {
			fprintf(stderr, "-c: %d\n", errno);
    		fprintf(stderr, "-c: %s\n", strerror(errno));
    		fprintf(stderr, "-c: INA CHYBA\n");
			return 23;
		}

	}



	while ( (rd = read(in, buf, 256)) != 0 ) { 
		if (rd == -1) { 
			fprintf(stderr, "-c: %d\n", errno);
    		fprintf(stderr, "-c: %s\n", strerror(errno));
    		fprintf(stderr, "-c: INA CHYBA\n");
			return 23;
		}
		wr = write(out, buf, rd); 
		if (wr == -1) { 
			fprintf(stderr, "-c: %d\n", errno);
    		fprintf(stderr, "-c: %s\n", strerror(errno));
    		fprintf(stderr, "-c: INA CHYBA\n");
			return 23;
		}
		
	}
		
	close(in);
	close(out);

	return 0;

}