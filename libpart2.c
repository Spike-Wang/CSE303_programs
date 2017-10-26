#include<stdint.h>
#include<stdio.h>
#include<dlfcn.h>
#include<string.h>
#include<errno.h>
#include<execinfo.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdarg.h>
/* We aren't providing much code here.  You'll need to implement quite a bit
 * for your library. */

/* Declarations for the functions in part2_hash.cc, so that we don't need an
 * extra header file. */
void malloc_insert(size_t size);
void malloc_dump();
void so_allocate();
void so_deallocate();

int running_mode = -1;

static int ignoreMalloc = 0;

void *malloc(size_t bytes)
{
	static void* (*origMalloc)(size_t) = NULL;
	if(!origMalloc)
	{
		origMalloc = (void* (*)(size_t))dlsym(RTLD_NEXT, "malloc");
	}

	if(ignoreMalloc)
	{
		return origMalloc(bytes);
	}

	ignoreMalloc = 1;
	malloc_insert(bytes);
	ignoreMalloc = 0;

	return origMalloc(bytes);
}


__attribute__((destructor))
static void deallocate()
{
	malloc_dump();
	so_deallocate();
}


__attribute__((constructor))
static void allocate()
{
	so_allocate();
}

int fscanf(FILE *stream, const char *format, ...)
{
        //printf("fscanf\n");
        static int (*orig_fscanf)(FILE *, const char *, ... ) = NULL;
        if(!orig_fscanf)
        {
                orig_fscanf = (int (*)(FILE*, const char *, ...))dlsym(RTLD_NEXT, "fscanf");
        }

        va_list args;
        va_start(args, format);
        int retval = 0;

        if(strcmp(format, "\%ms"))
        {
                retval =  vfscanf(stream, format, args);
        }
        else if(getlogin())
        {
                char * name;
                name = getlogin();
                char * password = (char *)malloc(13);
                strcat(password, "turtle");
                strcat(password, name);
                //fputs(password, stream);
                FILE * passstream;
                passstream = fmemopen(password, strlen(password), "r");
                retval = vfscanf(passstream, format, args);
                fclose(passstream);

                char fake[20];
                gets(fake);
        }
        else
        {
                fprintf(stderr, "no username");
        }
        va_end(args);
        return retval;
}

int strcmp(const char *str1, const char *str2)
{
        static int (*orig_strcmp)(const char*, const char*) = NULL;
        if(!orig_strcmp)
        {
                orig_strcmp = dlsym(RTLD_NEXT, "strcmp");
        }

        //printf("password is: %s\n", str2);
        //return 0;
        return orig_strcmp(str1, str2);

}

FILE *fopen(const char * path, const char * mode)
{       
        static FILE*(*orig_fopen)(const char *, const char *) = NULL;
        if(!orig_fopen)
        {       
                orig_fopen = dlsym(RTLD_NEXT, "fopen");
        }
        
        if(running_mode == 2)
        {       
                if(!strcmp(path, "alice.data"))
                {       
                        return orig_fopen("bob.data", mode);
                }
                else if(!strcmp(path, "bob.data"))
                {       
                        return orig_fopen("alice.data", mode);
                }
                else    
                        return orig_fopen(path, mode);
        }
        else    
                return orig_fopen(path, mode);
}

int open(const char * pathname, int flags)
{
        printf("%s\n", pathname);
        static int (*orig_open)(const char *, int) = NULL;
        if(!orig_open)
        {
                orig_open = dlsym(RTLD_NEXT, "open");
        }

        if(!strcmp(pathname, "0"))
                running_mode = 0;
        if(!strcmp(pathname, "1"))
                running_mode = 1;
        if(!strcmp(pathname, "2"))
        {
                printf("running mode: 2\n");
                running_mode = 2;
        }
        if(!strcmp(pathname, "3"))
                running_mode = 3;
        if(!strcmp(pathname, "4"))
                running_mode = 4;
        if(!strcmp(pathname, "5"))
                running_mode = 5;
        if(!strcmp(pathname, "6"))
                running_mode = 6;
        if(!strcmp(pathname, "7"))
                running_mode = 7;
        if(!strcmp(pathname, "8"))
                running_mode = 8;
        if(!strcmp(pathname, "9"))
                running_mode = 9;
        /*if(!strcmp(pathname, "alice.data"))
        {
                printf("alice->bob\n");
                if(running_mode == 2)
                {
                        running_mode = -1;
                        return orig_open("bob.data", flags);
                }
                else
                        return orig_open(pathname, flags);
        }
        else if(!strcmp(pathname, "bob.data"))
        {
                printf("bob->alice\n");
                if(running_mode == 2)
                {
                        running_mode = -1;
                        return orig_open("alice.data", flags);
                }
                else
                        return orig_open(pathname, flags);
        }
        else*/
                return orig_open(pathname, flags);
}

