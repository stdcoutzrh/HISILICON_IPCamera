
/*-------------------------------------------------------------------------*/
/**
   @file    iniparser.c
   @author  N. Devillard
   @date    Mar 2000
   @version
   @brief   Parser for ini files.
*/
/*--------------------------------------------------------------------------*/

/*




*/

/*---------------------------------------------------------------------------
                                Includes
 ---------------------------------------------------------------------------*/

#include "iniparser.h"
#include "strlib.h"
#if HI_OS_TYPE == HI_OS_LINUX
#include <syslog.h>
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#define ASCIILINESZ         1024
#define INI_INVALID_KEY     ((char*)-1)
#define CNOSK               0xFFFF      /*注释可依赖的section和key*/
//#define KVCSIZE             512         /*KEY and VALUE and COMMENT'size is 512*/
/*---------------------------------------------------------------------------
                        Private to this module
 ---------------------------------------------------------------------------*/
#if HI_OS_TYPE == HI_OS_LINUX

#elif HI_OS_TYPE == HI_OS_WIN32
	#define snprintf _snprintf
#endif

/* Private: add an entry to the dictionary */
int iniparser_add_entry(
    dictionary * d,
    const char * sec,
    const char * key,
    const char * val)
{
    char longkey[2*ASCIILINESZ+1];

    if((d == NULL) || (sec == NULL)) return -1;
    /* Make a key as section:keyword */
    if (key!=NULL) {
        sprintf(longkey, "%s:%s", sec, key);
    } else {
        strcpy(longkey, sec);
    }

    /* Add (key,val) to dictionary */
    if(0 != dictionary_add(d, longkey, val))
    {
        return 1;
    }

    return 0;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get number of sections in a dictionary
  @param    d   Dictionary to examine
  @return   int Number of sections found in dictionary

  This function returns the number of sections found in a dictionary.
  The test to recognize sections is done on the string stored in the
  dictionary: a section name is given as "section" whereas a key is
  stored as "section:key", thus the test looks for entries that do not
  contain a colon.

  This clearly fails in the case a section name contains a colon, but
  this should simply be avoided.

  This function returns -1 in case of error.
 */
/*--------------------------------------------------------------------------*/

int iniparser_getnsec(const dictionary * d)
{
    int i ;
    int nsec ;

    if (d==NULL) return -1 ;
    nsec=0 ;
    for (i=0 ; i<d->size ; i++) {
        if (d->key[i]==NULL)
            continue ;
        if (strchr(d->key[i], ':')==NULL) {
            nsec ++ ;
        }
    }
    return nsec ;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get name for section n in a dictionary.
  @param    d   Dictionary to examine
  @param    n   Section number (from 0 to nsec-1).
  @return   Pointer to char string

  This function locates the n-th section in a dictionary and returns
  its name as a pointer to a string statically allocated inside the
  dictionary. Do not free or modify the returned string!

  This function returns NULL in case of error.
 */
/*--------------------------------------------------------------------------*/

char * iniparser_getsecname(const dictionary * d, const int n)
{
    int i ;
    int foundsec ;

    if (d==NULL || n<0) return NULL ;
    foundsec=0 ;
    for (i=0 ; i<d->size ; i++) {
        if (d->key[i]==NULL)
            continue ;
        if (strchr(d->key[i], ':')==NULL) {
            foundsec++ ;
            if (foundsec>n)
                break ;
        }
    }
    if (foundsec<=n) {
        return NULL ;
    }
    return d->key[i] ;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Dump a dictionary to an opened file pointer.
  @param    d   Dictionary to dump.
  @param    f   Opened file pointer to dump to.
  @return   void

  This function prints out the contents of a dictionary, one element by
  line, onto the provided file pointer. It is OK to specify @c stderr
  or @c stdout as output files. This function is meant for debugging
  purposes mostly.
 */
/*--------------------------------------------------------------------------*/
void iniparser_dump(dictionary * d, FILE * f)
{
    int     i ;

    if (d==NULL || f==NULL) return ;
    for (i=0 ; i<d->size ; i++) {
        if (d->key[i]==NULL)
            continue ;
        if (d->val[i]!=NULL) {
            fprintf(f, "[%s]=[%s]\n", d->key[i], d->val[i]);
        } else {
            fprintf(f, "[%s]=UNDEF\n", d->key[i]);
        }
    }
    return ;
}


static unsigned char iniparser_save_comment(const dictionary * d, FILE * f, const unsigned int hash)
{
    int     ci;
    unsigned char flag = 0; /*标志该行是否有注释*/

    for(ci=0; ci < d->commSize; ci++)
    {
        if(d->commPlace[ci] == 0)
        {
            continue;
        }
        if(d->commHash[ci] == hash)
        {
            flag = 1;
            switch(d->commPlace[ci])
            {
                case 1:
                {
                    fprintf(f, "%s", d->comment[ci] ? d->comment[ci] : " ");
                    break;
                }
                case 2:
                {
                    fseek(f,-1,SEEK_CUR);
                    fprintf(f, "%s\n", d->comment[ci] ? d->comment[ci] : " ");
                    break;
                }
                case 3:
                {
                    fprintf(f, "%48s%s", " ", d->comment[ci] ? d->comment[ci] : " ");
                    break;
                }
                case 4:
                {
                    fprintf(f, "\r\n");
                    break;
                }
                /*case 5:
                {
                    fprintf(f, "%48s\n", " ");
                    break;
                }*/
                default:
                    break;
            }
        }
    }
    return flag;
}
/*-------------------------------------------------------------------------*/
/**
  @brief    Save a dictionary to a loadable ini file
  @param    d   Dictionary to dump
  @param    f   Opened file pointer to dump to
  @return   void

  This function dumps a given dictionary into a loadable ini file.
  It is Ok to specify @c stderr or @c stdout as output files.
 */
/*--------------------------------------------------------------------------*/

void iniparser_dump_ini(const dictionary * d, FILE * f)
{
    int     i, j ;
    char    keym[ASCIILINESZ+1];
    int     nsec ;
    char *  secname ;
    int     seclen ;
    //unsigned char ret;

    if (d==NULL || f==NULL) return ;

    nsec = iniparser_getnsec(d);
    if (nsec<1) {
        /* No section in file: dump all keys as they are */

        /*2006/03/13 blair add : save comment and space Line*/
        iniparser_save_comment(d, f, CNOSK);

        for (i=0 ; i<d->size ; i++)
        {
            if (d->key[i]==NULL)
            {
                continue ;
            }

            /*2006/03/13 blair modify : save comment and space Line*/
            fprintf(f, "%-30s = \"%-15s\"\n", d->key[i], d->val[i]);
            fprintf(f, "%-30s = \"%-15s\"\n", d->key[i],
                    d->val[i] ? d->val[i] : " ");
            /*2006/03/13 blair add : save comment and space Line*/
            iniparser_save_comment(d, f, d->hash[i]);

            /*if(ret == 0)
            {
                fprintf(f, "\n");
            }*/
        }
        return ;
    }

    /*2006/03/13 blair add : save comment and space Line*/
    iniparser_save_comment(d, f, CNOSK);

    for (i=0 ; i<nsec ; i++)
    {
        int hash;
        secname = iniparser_getsecname(d, i) ;
        seclen  = (int)strlen(secname);
        fprintf(f, "[%-48s\n", secname);
#if HI_OS_TYPE == HI_OS_WIN32
        fseek(f, 0-(50-strlen(secname)), SEEK_CUR);
#elif HI_OS_TYPE == HI_OS_LINUX
        fseek(f, 0-(49-strlen(secname)), SEEK_CUR);
#endif
        fprintf(f, "%c", ']');
        fseek(f, 0, SEEK_END);
        /*2006/03/13 blair add : save comment and space Line*/
        hash = dictionary_hash(secname);
        iniparser_save_comment(d, f, hash);
        /*if(ret == 0)
        {
            fprintf(f, "\n");
        }*/

        sprintf(keym, "%s:", secname);
        for (j=0 ; j<d->size ; j++) {
            if (d->key[j]==NULL)
                continue ;
            if (!strncmp(d->key[j], keym, seclen+1))
            {
                fprintf(f, "%-30s = \"%-15s\"\n", d->key[j]+seclen+1,
                        d->val[j] ? d->val[j] : " ");
                /*2006/03/13 blair add : save comment and space Line*/
                iniparser_save_comment(d, f, d->hash[j]);
                /*if(ret == 0)
                {
                    fprintf(f, "\n");
                }*/
            }
        }
    }
    //fprintf(f, "\n");
    return ;
}




/*-------------------------------------------------------------------------*/
/**
  @brief	Get the string associated to a key, return NULL if not found
  @param    d   Dictionary to search
  @param    key Key string to look for
  @return   pointer to statically allocated character string, or NULL.

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  NULL is returned.
  The returned char pointer is pointing to a string allocated in
  the dictionary, do not free or modify it.

  This function is only provided for backwards compatibility with
  previous versions of iniparser. It is recommended to use
  iniparser_getstring() instead.
 */
/*--------------------------------------------------------------------------*/
char * iniparser_getstr(const dictionary * d, const char * key)
{
    return iniparser_getstring(d, key, NULL);
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key
  @param    d       Dictionary to search
  @param    key     Key string to look for
  @param    def     Default value to return if key not found.
  @return   pointer to statically allocated character string

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the pointer passed as 'def' is returned.
  The returned char pointer is pointing to a string allocated in
  the dictionary, do not free or modify it.
 */
/*--------------------------------------------------------------------------*/
char * iniparser_getstring(const dictionary * d, const char * key, char * def)
{
    char * lc_key ;
    char * sval ;

    if (d==NULL || key==NULL)
        return def ;

    lc_key = strdup(strlwc(key));
    sval = dictionary_get(d, lc_key, def);
    free(lc_key);
    return sval ;
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to an int
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   integer

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.
 */
/*--------------------------------------------------------------------------*/
int iniparser_getint(const dictionary * d, const char * key, const int notfound)
{
    char    *   str ;

    str = iniparser_getstring(d, key, INI_INVALID_KEY);
    if (str==INI_INVALID_KEY) return notfound ;
    return (int)strtol(str, NULL, 0); 
    //return atoi(str);
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a double
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   double

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.
 */
/*--------------------------------------------------------------------------*/
double iniparser_getdouble(const dictionary * d, const char * key, const double notfound)
{
    char    *   str ;

    str = iniparser_getstring(d, key, INI_INVALID_KEY);
    if (str==INI_INVALID_KEY) return notfound ;
    return atof(str);
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a boolean
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   integer

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.

  A true boolean is found if one of the following is matched:

  - A string starting with 'y'
  - A string starting with 'Y'
  - A string starting with 't'
  - A string starting with 'T'
  - A string starting with '1'

  A false boolean is found if one of the following is matched:

  - A string starting with 'n'
  - A string starting with 'N'
  - A string starting with 'f'
  - A string starting with 'F'
  - A string starting with '0'

  The notfound value returned if no boolean is identified, does not
  necessarily have to be 0 or 1.
 */
/*--------------------------------------------------------------------------*/
int iniparser_getboolean(const dictionary * d, const char * key, const int notfound)
{
    char    *   c ;
    int         ret ;

    c = iniparser_getstring(d, key, INI_INVALID_KEY);
    if (c==INI_INVALID_KEY) return notfound ;
    if (c[0]=='y' || c[0]=='Y' || c[0]=='1' || c[0]=='t' || c[0]=='T') {
        ret = 1 ;
    } else if (c[0]=='n' || c[0]=='N' || c[0]=='0' || c[0]=='f' || c[0]=='F') {
        ret = 0 ;
    } else {
        ret = notfound ;
    }
    return ret;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Finds out if a given entry exists in a dictionary
  @param    ini     Dictionary to search
  @param    entry   Name of the entry to look for
  @return   integer 1 if entry exists, 0 otherwise

  Finds out if a given entry exists in the dictionary. Since sections
  are stored as keys with NULL associated values, this is the only way
  of querying for the presence of sections in a dictionary.
 */
/*--------------------------------------------------------------------------*/

int iniparser_find_entry(
    const dictionary  *   ini,
    const char        *   entry
)
{
    int found=0 ;
    if (iniparser_getstring(ini, entry, INI_INVALID_KEY)!=INI_INVALID_KEY) {
        found = 1 ;
    }
    return found ;
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Set an entry in a dictionary.
  @param    ini     Dictionary to modify.
  @param    entry   Entry to modify (entry name)
  @param    val     New value to associate to the entry.
  @return   int 0 if Ok, -1 otherwise.

  If the given entry can be found in the dictionary, it is modified to
  contain the provided value. If it cannot be found, -1 is returned.
  It is Ok to set val to NULL.
 */
/*--------------------------------------------------------------------------*/

int iniparser_setstr(const dictionary * ini, const char * entry, const char * val)
{
    if(0 != dictionary_set(ini, strlwc(entry), val))
    {
        return 1;
    }
    return 0 ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete an entry in a dictionary
  @param    ini     Dictionary to modify
  @param    entry   Entry to delete (entry name)
  @return   void

  If the given entry can be found, it is deleted from the dictionary.
 */
/*--------------------------------------------------------------------------*/
void iniparser_unset(dictionary * ini, const char * entry)
{
    dictionary_unset(ini, strlwc(entry));
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Parse an ini file and return an allocated dictionary object
  @param    ininame Name of the ini file to read.
  @return   Pointer to newly allocated dictionary

  This is the parser for ini files. This function is called, providing
  the name of the file to be read. It returns a dictionary object that
  should not be accessed directly, but through accessor functions
  instead.

  The returned dictionary must be freed using iniparser_freedict().
 */
/*--------------------------------------------------------------------------*/

dictionary * iniparser_load(const char * ininame)
{
    dictionary  *   d ;
    char        lin[ASCIILINESZ+1]  = {0};
    char        sec[KVCSIZE+1]      = {0};
    char        key[KVCSIZE+1]      = {0};
    char        val[KVCSIZE+1]      = {0};
    char        comment[KVCSIZE+1]  = {0};
    char    *   where ;
    FILE    *   ini ;
    int         lineno, len;

    //printf("%s\n",ininame);
    if ((ini=fopen(ininame, "r"))==NULL) {
        return NULL ;
    }

    sec[0]=0;

    /*
     * Initialize a new dictionary entry
     */
    d = dictionary_new(0);
    lineno = 0 ;
    while (fgets(lin, ASCIILINESZ, ini)!=NULL)
    {
        lineno++ ;
        where = lin; /* Skip leading spaces */
        comment[0] = 0;
        val[0] = 0;
		len = (int)strlen(lin)-1;
		/* Safety check against buffer overflows */
		if (lin[len]!='\n' && (len == (ASCIILINESZ - 1))) {
			fprintf(stderr,
					"iniparser: input line too long in %s (%d):%s\n",
					ininame,
					lineno,
					lin);
			#if HI_OS_TYPE == HI_OS_LINUX
			syslog(LOG_NOTICE, "iniparser: input line too long in %s (%d)", ininame, lineno);
	    	#endif
			memset(lin, 0, ASCIILINESZ);
			while (fgets(lin, ASCIILINESZ, ini))
			{
				len = (int)strlen(lin)-1;
				if (lin[len]!='\n')
				{

					memset(lin, 0, ASCIILINESZ);
					continue;
				}
				else
				{
					memset(lin, 0, ASCIILINESZ);
					break;
				}
			}
			continue;
		}
        /*2006/03/13 blair modify : save comment and space Line*/
        //if (*where==';' || *where=='#' || *where==0)
        if (*where==';' || *where=='#')/*; comment*/
        {
            strcpy(comment,where);
            /*no space commnet line*/
            iniparser_add_comment(d, sec, key, comment, CL_NSBC);
        }
        else
        {
            where = strskp(lin); /* Skip leading spaces */
            /*if((sscanf (where, " ; %[^\n]", comment) == 1)
                ||(sscanf (where, " # %[^\n]", comment) == 1))*//*   ; comment*/
            if (*where==';' || *where=='#')
            {
                strcpy(comment,where);
                /*comment line with space*/
                iniparser_add_comment(d, sec, key, comment, CL_CLINE);
                continue ; /* Comment lines */
            }
            else
            {
                if ((sscanf(where, "[%[^]] ] %[^\n]", sec, comment)==2)
                    || (sscanf(where, "[%[^]]", sec)==1))/*[sec];comment*/
                {
                    /* Valid section name */
                    key[0] = 0;
                    strcpy(sec, strlwc(sec));
                    iniparser_add_entry(d, sec, NULL, NULL);
                    if(comment[0] != 0)
                    {
                        /*The comment at key line's right*/
                        iniparser_add_comment(d, sec, key, comment, CL_AKLR);
                    }
                }
                else /*key = value ;comment*/
                {
                    /*2006/03/13 blair modify : save comment and space Line*/
                    if (sscanf(where, "%[^=] = \"%[^\"]\" %[^\n]", key, val,
                            comment) == 3
                       ||  sscanf(where, "%[^=] = \"%[^\"]\"", key, val) == 2
                       ||  sscanf(where, "%[^=] = '%[^\']' %[^\n]", key, val,
                                comment) == 3
                       ||  sscanf(where, "%[^=] = '%[^\']'",   key, val) == 2
                       ||  sscanf(where, "%[^=] = %[^;] %[^\n]", key, val,
                                comment) == 3
                       ||  sscanf(where, "%[^=] = %[^;]", key, val ) == 2
                       ||  sscanf(where, "%[^=] = %[^\n]", key,
                                comment ) == 2
                       ||  sscanf(where, "%[^=] = ", key) == 1)
                    {
                        strcpy(key, strlwc(strcrop(key)));
                        /*
                         * sscanf cannot handle "" or '' as empty value,
                         * this is done here
                         */
                        if (!strcmp(val, "\"\"") || !strcmp(val, "''")) {
                            val[0] = (char)0;
                        } else {
                            strcpy(val, strcrop(val));
                        }
                        iniparser_add_entry(d, sec, key, val);

                        if(comment[0] != 0)
                        {
                            /*The comment at key line's right*/
                            iniparser_add_comment(d, sec, key, comment, CL_AKLR);
                        }
                    }
                    else
                    {
                        comment[0] = 0;
                        /*Space Line*/
                        iniparser_add_comment(d, sec, key, comment, CL_SLINE);
                    }
                }
            }
        }
    }
    fclose(ini);
    return d ;
}

/*2006/03/11 blair add : save comment and space Line*/
int iniparser_add_comment(
    dictionary          *d,
    const char          *sec,
    const char          *key,
    const char          *comment,
    const unsigned char place)
{
    char        longkey[1024] = {0};
    unsigned    hash;
    int         i;

    if ((d == NULL) || (sec == NULL)  || (place < 1)) return -1;


    /* Make a key as section:keyword */
    if(sec[0] != 0)
    {
        if (key[0]!=0) {
            snprintf(longkey, 1024, "%s:%s", sec, key);
        } else {
            strncpy(longkey, sec, 1024);
        }
    }
    else
    {
        if (key[0]!=0) {
            snprintf(longkey, 1024, "%s", key);
        } else {
            longkey[0] = 0;
        }
    }

    /* Compute hash for this key */
    if(longkey[0]!='\0')
    {
        hash = dictionary_hash(longkey);
    }
    else
    {
        hash = CNOSK;
    }

	/* Add a new value */
	/* See if dictionary needs to grow */
	if (d->commN == d->commSize)
	{
		/* Reached maximum size: reallocate blackboard */
		d->comment  = (char **)mem_double((void*)d->comment, d->commSize * sizeof(char*));
		d->commHash = (unsigned *)mem_double((void*)d->commHash, d->commSize * sizeof(unsigned));
		d->commPlace= (unsigned char *)mem_double((void*)d->commPlace, d->commSize * sizeof(unsigned char));

		/* Double size */
		d->commSize *= 2 ;
	}

    /* Insert comment in the first empty slot */
    for (i=0 ; i<d->commSize ; i++)
    {
        if (d->commPlace[i] == 0)
        {
            /* Add comment here */
            break ;
        }
    }
    d->commHash[i] = hash;
    d->commPlace[i]= place;
    d->comment[i] ? free(d->comment[i]),d->comment[i]=NULL: NULL;
    d->comment[i]  = (comment[0] != 0) ? strdup(comment) : NULL;
	d->commN ++ ;
	return 0;
}
/*-------------------------------------------------------------------------*/
/**
  @brief    Free all memory associated to an ini dictionary
  @param    d Dictionary to free
  @return   void

  Free all memory associated to an ini dictionary.
  It is mandatory to call this function before the dictionary object
  gets out of the current context.
 */
/*--------------------------------------------------------------------------*/

void iniparser_freedict(dictionary * d)
{
    dictionary_del(d);
}


int iniparser_sec_getNLowLever(const dictionary * d,const char* UperLever)
{
/*    int nsec;
    int i;
    int n = 0;

    if((HI_NULL_PTR == d) || (HI_NULL_PTR == UperLever))
    {
        return 0;
    }

    nsec = iniparser_getnsec(d);
    for(i = 0; i<nsec; i++)
    {

    }

    return n;
*/
    d = d;
    UperLever = UperLever;
    return 0;
}


char* iniparser_sec_getLowLever(const dictionary * d,const char* UpLever, int n)
{
    d = d;
    UpLever = UpLever;
    n = n;
    return 0;
}

void iniparser_mem_printf(const dictionary * d)
{
    int     i, j ;
    char    keym[ASCIILINESZ+1];
    int     nsec ;
    char *  secname ;
    int     seclen ;
    //unsigned char ret;

    if (d==NULL) return ;

    nsec = iniparser_getnsec(d);
    if (nsec<1) {
        /* No section in file: dump all keys as they are */

        /*2006/03/13 blair add : save comment and space Line*/
        //iniparser_save_comment(d, f, CNOSK);

        for (i=0 ; i<d->size ; i++)
        {
            if (d->key[i]==NULL)
            {
                continue ;
            }

            /*2006/03/13 blair modify : save comment and space Line*/
            printf("%-30s = %-15s\n", d->key[i], d->val[i]);
            printf("%-30s = %-15s\n", d->key[i],
                    d->val[i] ? d->val[i] : " ");
            /*2006/03/13 blair add : save comment and space Line*/
            //ret = iniparser_save_comment(d, f, d->hash[i]);

            /*if(ret == 0)
            {
                fprintf(f, "\n");
            }*/
        }
        return ;
    }

    /*2006/03/13 blair add : save comment and space Line*/
    //iniparser_save_comment(d, f, CNOSK);

    for (i=0 ; i<nsec ; i++)
    {
        //int hash;
        secname = iniparser_getsecname(d, i) ;
        seclen  = (int)strlen(secname);
        printf("[%s", secname);
        /*
#if HI_OS_TYPE == HI_OS_WIN32
        fseek(f, 0-(50-strlen(secname)), SEEK_CUR);
#elif HI_OS_TYPE == HI_OS_LINUX
        fseek(f, 0-(49-strlen(secname)), SEEK_CUR);
#endif
        */
        printf("%c", ']');
        //fseek(f, 0, SEEK_END);
        /*2006/03/13 blair add : save comment and space Line*/
        dictionary_hash(secname);
        //ret = iniparser_save_comment(d, f, hash);
        /*if(ret == 0)
        {
            fprintf(f, "\n");
        }*/

        sprintf(keym, "%s:", secname);
        for (j=0 ; j<d->size ; j++) {
            if (d->key[j]==NULL)
                continue ;
            if (!strncmp(d->key[j], keym, seclen+1))
            {
                printf("%-30s = %-15s\n", d->key[j]+seclen+1,
                        d->val[j] ? d->val[j] : " ");
                /*2006/03/13 blair add : save comment and space Line*/
                //ret = iniparser_save_comment(d, f, d->hash[j]);
                /*if(ret == 0)
                {
                    fprintf(f, "\n");
                }*/
            }
        }
    }
    //fprintf(f, "\n");
    return ;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

/* vim: set ts=4 et sw=4 tw=75 */




