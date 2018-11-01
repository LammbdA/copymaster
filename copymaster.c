#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "options.h"
#include "prepinac.h"


void FatalError(char c, const char* msg, int exit_status);
void PrintCopymasterOptions(struct CopymasterOptions* cpm_options);


int main(int argc, char* argv[])
{
    
	const char* usage_error_msg_format = "Usage: %s [OPTION]... SOURCE DEST\n(%s)\n";
	extern int optind;
	int rt;
	int trunc_size; //for truncating
	char *argument; //for lseek and chmod
	long number; //for inode
	
	struct CopymasterOptions cpm_options = ParseCopymasterOptions(argc, argv);

    //-------------------------------------------------------------------
    // Kontrola hodnot prepinacov
    //-------------------------------------------------------------------

    // Vypis hodnot prepinacov odstrante z finalnej verzie
    
    PrintCopymasterOptions(&cpm_options);
    
    //-------------------------------------------------------------------
    // Osetrenie prepinacov pred kopirovanim
    //-------------------------------------------------------------------
    
    if (cpm_options.fast && cpm_options.slow) {
        fprintf(stderr, "CHYBA PREPINACOV\n"); 
        exit(EXIT_FAILURE);
    }

	if (argc <= 2) {
		fprintf(stderr, "CHYBA PREPINACOV\n");
		return 20;
	}
	
	
	if (argc == 3) {
		rt = nothing(argv[argc-2], argv[argc-1]);
		if (rt != 0) return rt;
	}
	    
    // TODO Nezabudnut dalsie kontroly kombinacii prepinacov ...
    
    //-------------------------------------------------------------------
    // Kopirovanie suborov
    //-------------------------------------------------------------------
    
    // TODO Implementovat kopirovanie suborov

    if (optind + 2 != argc) {
        fprintf(stderr, usage_error_msg_format, argv[0], "infile or outfile is missing");
        exit(EXIT_FAILURE);
    }
    
    cpm_options.infile = argv[optind];
    cpm_options.outfile = argv[optind + 1];

    
    //-------------------------------------------------------------------
    // Vypis adresara
    //-------------------------------------------------------------------    
    	if (cpm_options.directory) {
        	// TODO Implementovat vypis adresara
    	}
    	else if (cpm_options.append) { 
			rt = append(argv[argc-2], argv[argc-1]);
			if (rt != 0) return rt;
		} 
		else if (cpm_options.overwrite) {
			rt = overwrite(argv[argc-2], argv[argc-1]);
			if (rt != 0) return rt;
		}
		else if (cpm_options.delete_opt) {
			rt = delete(argv[argc-2], argv[argc-1]);
			if (rt != 0) return rt;
		}
		else if (cpm_options.truncate) {
			rt = mytruncate(argv[argc-2], argv[argc-1], trunc_size);
			if (rt != 0) return rt;
		}
		else if (cpm_options.link) {
			rt = linkk(argv[argc-2], argv[argc-1]);
			if (rt != 0) return rt;
		}
		else if (cpm_options.slow) {
			rt = slow(argv[argc-2], argv[argc-1]);
			if (rt != 0) return rt;
		}
		else if (cpm_options.fast) {
			rt = fast(argv[argc-2], argv[argc-1]);
			if (rt != 0) return rt;
		}
		else if (cpm_options.lseek) {
			rt = mylseek(argv[argc-2], argv[argc-1], argument);
			if (rt != 0) return rt;
		}
		else if (cpm_options.inode) {
			rt = inode(argv[argc-2], argv[argc-1], number);
			if (rt != 0) return rt;
		}
		else if (cpm_options.chmod) {
			rt = mchmod(argv[argc-2], argv[argc-1], argument);
			if (rt != 0) return rt;
		}
		else if (cpm_options.create) {
			rt = create(argv[argc-2], argv[argc-1], argument);
			if (rt != 0) return rt;
		}
		else if (cpm_options.umask) {
			rt = mumask(argv[argc-2], argv[argc-1], argument);
			if (rt != 0) return rt;
		}
	
        
    //-------------------------------------------------------------------
    // Osetrenie prepinacov po kopirovani
    //-------------------------------------------------------------------
    
    // TODO Implementovat osetrenie prepinacov po kopirovani
    
    return 0;
}


void FatalError(char c, const char* msg, int exit_status)
{
    fprintf(stderr, "%c:%d\n", c, errno); 
    fprintf(stderr, "%c:%s\n", c, strerror(errno));
    fprintf(stderr, "%c:%s\n", c, msg);
    exit(exit_status);
}

void PrintCopymasterOptions(struct CopymasterOptions* cpm_options)
{
    if (cpm_options == 0)
        return;
    
    printf("infile:        %s\n", cpm_options->infile);
    printf("outfile:       %s\n", cpm_options->outfile);
    
    printf("fast:          %d\n", cpm_options->fast);
    printf("slow:          %d\n", cpm_options->slow);
    printf("create:        %d\n", cpm_options->create);
    printf("create_mode:   %o\n", (unsigned int)cpm_options->create_mode);
    printf("overwrite:     %d\n", cpm_options->overwrite);
    printf("append:        %d\n", cpm_options->append);
    printf("lseek:         %d\n", cpm_options->lseek);
    
    printf("lseek_options.x:    %d\n", cpm_options->lseek_options.x);
    printf("lseek_options.pos1: %ld\n", cpm_options->lseek_options.pos1);
    printf("lseek_options.pos2: %ld\n", cpm_options->lseek_options.pos2);
    printf("lseek_options.num:  %lu\n", cpm_options->lseek_options.num);
    
    printf("directory:     %d\n", cpm_options->directory);
    printf("delete_opt:    %d\n", cpm_options->delete_opt);
    printf("chmod:         %d\n", cpm_options->chmod);
    printf("chmod_mode:    %o\n", (unsigned int)cpm_options->chmod_mode);
    printf("inode:         %d\n", cpm_options->inode);
    printf("inode_number:  %lu\n", cpm_options->inode_number);
    
    printf("umask:\t%d\n", cpm_options->umask);
    for(unsigned int i=0; i<kUMASK_OPTIONS_MAX_SZ; ++i) {
        if (cpm_options->umask_options[i][0] == 0) {
            // dosli sme na koniec zoznamu nastaveni umask
            break;
        }
        printf("umask_options[%u]: %s\n", i, cpm_options->umask_options[i]);
    }
    
    printf("link:          %d\n", cpm_options->link);
    printf("truncate:      %d\n", cpm_options->truncate);
    printf("truncate_size: %ld\n", cpm_options->truncate_size);
    printf("sparse:        %d\n", cpm_options->sparse);
}