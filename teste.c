#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TAM 1024

//void lsdir(char *argv[]);
void movedir(int argc,char *argv[]);
void catdir( int argc,char *argv[1]);
void renamedir(int argc, char *argv[]);
void renamedir(int argc, char **argv);
void lsdir(const char *dir,int op_a,int op_l);
void chdiretorio(char *argstring[],int argint);
void mkdiretorio(char *argstring[]);
void rmvdiretorio(char *argstring[]);
//void lsdir();

int main(int argc, char* argv[]){
    char user_input[TAM];  //get command line
    char* argstring[120]; //user command
    int argint ; //argument count
    char* path= "/bin/";
  //set path at bin  
    char file_path[50];//full file path
    char argumentov[50];
    char comandoinicial[50];

	//bota o diretorio na tela
  char dActual[1024];
	
    while(1){
      getcwd(dActual, 1024);  
        printf("%s > KITTYSHELL> ",dActual);             
        if(!fgets(user_input,TAM, stdin)){
            break;
        }
        size_t length = strlen(user_input);
        if(length == 0){
            break;
        }
        if (user_input[length - 1] == '\n'){
            user_input[length - 1] = '\0'; 
        }
        char *token;                  
        token = strtok(user_input," ");
        int argint=0;
        if(token == NULL){
            continue;
        }
        while(token!=NULL){
            argstring[argint]=token;      
            token = strtok(NULL," ");
            argint++;
        }
        argstring[argint]=NULL; 
        strcpy(file_path, path);  //Assign path to file_path 
        strcat(file_path, argstring[0]); //conctanate command and file path

        if (strcmp(argstring[0], "cd") == 0){
				printf("cd \n");
          chdiretorio(argstring,argint);
        }

		else if (strcmp(argstring[0], "exit") == 0){
				  exit(0);
        }

		else if (strcmp(argstring[0], "ls") == 0){
            if (argc == 1){
		lsdir(".",0,0);
	}
	else if (argc == 2){
		if (argv[1] == "-")
		{
			//Checking if option is passed
			//Options supporting: a, l
			int op_a = 0, op_l = 0;
			char *p = (char*)(argv[1] + 1);
			while(*p){
				if(*p == 'a') op_a = 1;
				else if(*p == 'l') op_l = 1;
				else{
					perror("Option not available");
					exit(EXIT_FAILURE);
				}
				p++;
			}
			lsdir(".",op_a,op_l);
		}
	}
        
    }
		else if (strcmp(argstring[0], "mkdir") == 0){
          printf("mkdir \n");
          mkdiretorio(argstring);
        }
        else if (strcmp(argstring[0], "rmvdir") == 0){
          rmdir(argstring[1]);
        }
        else if (strcmp(argstring[0], "rename") == 0){
          renamedir(argc,argv); 
        }
		else if (strcmp(argstring[0], "cat") == 0){
				  printf("ABRINDO  >>>>>\n");
          catdir(argc,argstring);
          }
    else if (strcmp(argstring[0], "move") == 0){
				  printf("MOVENDO >>>>>\n");
          movedir(argc,argstring);
          }
    else if (strcmp(argstring[0], "pwd") == 0){
      printf("diretorios: %s", dActual);
          }
    }
    return 0;
}



void chdiretorio(char *argstring[],int argint){
  char s[100];
    if(strcmp(argstring[1],"..")==0){
      printf("%s\n", getcwd(s, 100));
    }
    if(chdir(argstring[1]))
   {
      switch (errno)
      {
      case ENOENT:
         printf( "Não foi possível localizar o diretório: %s\n", argstring[1] );
         break;
      case EINVAL:
         printf( "Buffer inválido.\n");
         break;
      default:
         printf("erro não identificado.\n");
      }
   }
}


void mkdiretorio(char *argstring[]){
  errno = 0;
    int ret = mkdir(argstring[1],S_IRWXU);
    if (ret == -1) {
        switch (errno) {
            case EACCES :
                printf("o diretório raiz não permite gravação");
            case EEXIST:
                printf("pasta já existe");
            case ENAMETOOLONG:
                printf("nome da pasta é mt longo");
            default:
                perror("mkdir");
        }
    }
}
    /*EACCES: If the search permission is denied for one of the components of path.
    EFAULT: If the path points lie outside the accessible address space.
    EIO: If there is an I/O error occurred.
    ELOOP: If there are too many symbolic links were encountered in the resolving path.
    ENAMETOOLONG: If the path is too long.
    ENOENT: If the file does not exist.
    ENOMEM: If there is insufficient kernel memory is available.
    ENOTDIR: If the component of path is not a directory.
 
    */
   ///////////////////////// LS COMMAND ////////////////////////////////////

/////COMANDO LS FOI USADO ESSE AQUI COMO BASE:

//////iq.opengenus.org/ls-command-in-c/
void lsdir(const char *dir,int op_a,int op_l)
{
	//Here we will list the directory
	struct dirent *d;
	DIR *dh = opendir(dir);
	if (!dh)
	{
		if (errno = ENOENT)
		{
			perror("diretório não existe pae");
		}
		else
		{
			perror("não da pra usar esse dir n");
		}
		exit(EXIT_FAILURE);
	}
	while ((d = readdir(dh)) != NULL)
	{
		if (!op_a && d->d_name[0] == '.')
			continue;
		printf("%s  \n", d->d_name);
		if(op_l) printf("\n");
	}
	if(!op_l)
	printf("\n");
}

void renamedir(int argc, char *argv[]){

  struct stat statbuf_src, statbuf_dest;
  char *src, *dest, *new_src, *new_dest;
  char *current_directory;

  if (argc != 3) {
    fprintf(stderr, "usage: %s src dest\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  // work on copy
  src = malloc(strlen(argv[1]) + 1);
  dest = malloc(strlen(argv[2]) + 1);
  strcpy(src, argv[1]);
  strcpy(dest, argv[2]);

  printf("current directory is \"%s\"\n", current_directory);
  rename(src,dest);
  fprintf(stderr,"rename failed with error %s\n",strerror(errno));
  }

void catdir( int argc,char *argv[1]){
  int fd,i;
  char buf[2];
  fd=open(argv[1],O_RDONLY,0777);
  if(fd==-argc){
  printf("file open error");
  }
  else
  {
  while((i=read(fd,buf,1))>0){
    printf("%c",buf[0]);
    }
    close(fd);
  }
}

///////////////////não funcionando/////////////////
void movedir(int argc,char *argv[]){
  int i,fd1,fd2;
  char *file1,*file2,buf[2];
  file1=argv[1];
  file2=argv[2];
  printf("file1=%s file2=%s \n",file1,file2);
  fd1=open(file1,O_RDONLY,0777);
  fd2=creat(file2,0777);
  while(i=read(fd1,buf,1)>0)
  write(fd2,buf,1);
  close(fd1);
  close(fd2);
}
