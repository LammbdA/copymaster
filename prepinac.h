int nothing(char infile[], char outfile[]);
int append(char infile[], char outfile[] );
int delete(char infile[], char outfile[] );
int fast(char infile[], char outfile[]);
int linkk(char infile[], char outfile[]);
int mylseek(char infile[], char outfile[], char *argument);
int inode(char infile[], char outfile[], long number);
int create(char infile[], char outfile[], char *argument);
int slow(char infile[], char outfile[]);
int mytruncate(char infile[], char outfile[], int size);
int overwrite(char infile[], char outfile[]);
int mchmod(char infile[], char outfile[], char *argument);
int mumask(char infile[], char outfile[], char *argument);