#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include"database.h"
#include<sys/stat.h>

char *toString(int i)
{
  int nbc =3;
  char *res = malloc(4);
  while(nbc>0)
  {
    int tmp = i % 10;
    res[nbc-1] = tmp + 48;
    i = i/10;
    nbc--;
  }
  res[3] = 0;
  return res;
}

int toInt(char *str)
{
  int c = 0;
  for(int i = 0; str[i]; i++)
    c = c*10 + str[i] - 48;
  return c;
}

char *concat(char *s1, char *s2)
{
  char *result = malloc(strlen(s1)+strlen(s2)+1);
  strcpy(result, s1);  
  strcat(result, s2);
  return result;
}

////////////////////////////////////////////////////////////
//////////////////////////ADD_PERSON////////////////////////
////////////////////////////////////////////////////////////

char* add(char* picture, char* name)
{
  FILE *f = NULL;
  int c = 1;
  f = fopen("file.txt", "r+");
  if (f == NULL)
    return "FileNot Found";
  else
  {
    if(fgetc(f) == EOF)
    {
      fprintf(f, "001-%s\n002-", name);
    }
    else
    {
      fseek(f, -4, SEEK_END);
      char *str = malloc(4);
      for(int i=0; i<4;i++)
	{
	  str[i]=0;
	}
      
      fread(str, 1, 3, f);
      fseek(f, 1,SEEK_CUR);
      c = toInt(str)+1;
      char *st = toString(c);
      fprintf(f, "%s\n%s-", name, st);
      c -= 1;
    }
  fclose(f);
  }
  char *s = toString(c);
  mkdir(s, 0777);
  add_img(picture, c);
  return "Added";
}

/////////////////////////////////////////////////////
//////////////////////ADD_IMG////////////////////////
/////////////////////////////////////////////////////

void add_img(char* s, int id)
{
  int i = 0;
  char *st = toString(id);
  DIR* rep = opendir(st);
  struct dirent * file = readdir(rep);
  while(file != NULL)
  {
    i++;
    file = readdir(rep);
  }
  i -= 2;
  char *nb = malloc(2*sizeof(char));
  *nb = i + 48;
	nb[1]=0;
  char *dstname = concat(st, nb);
  //printf("%s\n", dstname);
  FILE *source = fopen(s, "rb");
  char omg[20];
  sprintf(omg,"%s/%s", st, dstname);
  FILE *dst = fopen(omg, "ab+");
  int ch = fgetc(source);
 
  while(ch != EOF)
  {
    fputc(ch, dst);
    ch = fgetc(source);
  }
  fclose(source);
  fclose(dst);
  free(nb);
}


////////////////////////////////////////////////////////////////
///////////////////////////SEARCH///////////////////////////////
////////////////////////////////////////////////////////////////

//////////////////////////SEARCH ID/////////////////////////////

int search_id (char* name)
{
  
  FILE* fichier = NULL;
  fichier = fopen ("file.txt", "r");
  

  if (fichier != NULL)
    {

      char c = fgetc (fichier);
      
      if (c==EOF)
        {
          return 0;
        }
      fseek(fichier, 4, SEEK_SET);
      c=getc(fichier);
      
      char *nom = malloc(30);
      for (int y=0; y<30;y++)
	{
	  nom[y]=0;
	}
      int i=0;
      while (c != EOF )
	{  
	  
	  if (strcmp (nom,name)==0)
	    {
	      fseek (fichier, ((-4)-i),SEEK_CUR);
	      i=0;
	      c=getc(fichier);
	      while (c != '-' )
		{
		  nom[i]=c;
		  i++;
		  c = getc(fichier);		  
		}
	      fclose (fichier);
	      nom[i]=0;
	      int res =toInt(nom);
	      //free(nom);
	      return res;
	      }
	  
	  if (c != '\n' && c!=EOF)
	  {
	    
	    nom[i] = c;
	    i=i+1;
	  }
	  else
	    {
	      int z=0;
	      while (c!=EOF && z<4)
		{
		c=getc(fichier);
		z++;
		}
	      for (int y =0; y<30; y++)
		{
		  nom[y]=0;
		}
	      i=0;

	      }
	  if (c!=EOF)
	    {
	      c=fgetc (fichier);
	    }
	}
    }
  //free(nom);
  fclose(fichier);
  return 0;
} 

///////////////////////////SEARCH NAME//////////////////////////////

char* search_name (int id)
{

  FILE* fichier = NULL;
  char* name;
  name = toString(id);
  fichier = fopen ("file.txt", "r");
  

  if (fichier != NULL)
    {

      
      fseek(fichier, 0, SEEK_SET);
      char c = fgetc (fichier);
      char *nom = malloc(30);
      for (int y=0; y<30;y++)
	{
	  nom[y]=0;
	}
      int i=0;
      while (c != EOF )
	{  
	  
	  if (strcmp (nom,name)==0)
	    {
	      i=0;
	      c=getc(fichier);
	      while (c != '\n' && c!= EOF)
		{
		  
		  
		  nom[i]=c;
		  i++;
		  c = getc(fichier);		  
		}
	      fclose (fichier);
	      nom[i]=0;
	      return nom;
	      }
	  
	  if (c != '-')
	  {
	    
	    nom[i] = c;
	    i=i+1;

	  }
	  else
	    {

	      for (int y =0; y<20; y++)
		{
		  nom[y]=0;
		}
	      i=0;
	      while (c!= '\n' && c!= EOF)
		{
		  
		  c=fgetc (fichier);
		}

	      }
	  if (c!=EOF)
	    {
	      c=fgetc (fichier);
	    }
	}
    }
      fclose(fichier);
      return "404-Not Found";
}

///////////////////////////////////////////////////////////////
////////////////////////DELET//////////////////////////////////
///////////////////////////////////////////////////////////////

void delete_name (int id)
{

  FILE* fichier = NULL;
  char* name;
  name = toString(id);
  fichier = fopen ("file.txt", "r");
  

  if (fichier != NULL)
    {
      FILE* tmp = fopen("tmp.txt", "ab+");
      fseek(fichier, 0, SEEK_SET);
      fseek(tmp,0,SEEK_SET);
      char c;
      char *nom = malloc(30);
      for (int y=0; y<30;y++)
	{
	  nom[y]=0;
	}
      while (c != EOF )
	{  
	  
	  for (int i=0; i<3; i++)
	    {
	      c=fgetc(fichier);
	      nom[i]=c;
	      
	    }

	  if (strcmp(nom,name)==0)
	    {
	      while (c!='\n' && c!=EOF)
		{
                  
		  c=fgetc(fichier);
		}
	      
	    }
	  else
	    {
	      
	      fseek(fichier,-3,SEEK_CUR);
	      c=fgetc(fichier);
	      while (c!='\n' && c!=EOF)
		{
		  fputc(c,tmp);
		  c=fgetc(fichier);
		}
	      if (c !=EOF)
		  fputc(c,tmp);
	    }
	  
	  c=fgetc(fichier);
	  if (c==EOF)
	    {
	      break;
	    }
	  fseek(fichier,-1,SEEK_CUR);	  
	}

      remove("file.txt");
      fclose(fichier);
      rename("tmp.txt","file.txt");
      fclose(tmp);

    }       
}

void delete_dir (int id)
{

  char* name = toString(id);
  DIR* directory = opendir(name);  
  struct dirent *dir;

  while ((dir=readdir(directory))!=0)
    {
      if (dir->d_name[0]!='.')
	{
	  int le = strlen(name)+strlen(dir->d_name)+2;
	  char *tmp = malloc(le);
	  sprintf(tmp, "%s/%s", name,dir->d_name);
	  remove(tmp);
	}
    }
  remove(name);

}

void delete_person (char* name)
{
  int id = search_id(name);
  delete_name(id);
  delete_dir(id);
}

/////////////////////////////////////////////////////////////
////////////////////////Change the name//////////////////////
/////////////////////////////////////////////////////////////

void change_name (char* name, char* new_name)
{

  FILE* fichier = NULL;
  fichier = fopen ("file.txt", "r");
  
  FILE* tmp = fopen("tmp.txt", "ab+");
  fseek(fichier, 0, SEEK_SET);
  fseek(tmp,0,SEEK_SET);
  char c;
  char *nom = malloc(100);
  while (c != EOF )
    {  
      
      int i=0;
      for (int y=0; y<4; y++)
	{
	  c=fgetc(fichier);
	  fputc(c,tmp);
	}
      c=fgetc(fichier);
      while (c!='\n' && c!=EOF)
	{
	  nom[i]=c;
	  i++;
	  c=fgetc(fichier);
	}
       
      if (strcmp(nom,name)==0)
	{
	  char c2=*new_name;
	  while (c2!=0)
	    {
	      fputc(c2,tmp);
	      new_name++;
	      c2=*new_name;
	    }
	  if (c!=EOF && c!= '-')
	    fputc(c,tmp);
	}
      else
	{
	  fseek(fichier,(-1)-i,SEEK_CUR);
	  while (i>=0)
	    {
	      nom[i]=0;
	      i--;
	    }
	  c=fgetc(fichier);
	  if (c==EOF)
	    {
	      break;
	    }
	  while (c!='\n' && c!= EOF)
	    {
	      fputc(c,tmp);
	      c=fgetc(fichier);
	    }
	  if (c!=EOF && c!='-')
	    fputc(c,tmp);
	}
      c=fgetc(fichier);
      if (c==EOF)
	{
	  break;
	}
      fseek(fichier,-1,SEEK_CUR);	  
    }
  
  remove("file.txt");
  fclose(fichier);
  rename("tmp.txt","file.txt");
  fclose(tmp);
}

void delete_img(char *s)
{
  char *number = malloc(2);
  char *dirname = malloc(4);
  for(int i = 0; *(s+i) != 0; i++)
  {
    if(i<3)
    {
      *(dirname+i) = *(s+i);
    }
    else
    {
      *number = *(s+i);
    }
  }
  dirname[3] = 0;
  number[2] = 0;
  printf("dirname %s\n", dirname);
  DIR* rep = opendir(dirname);
  int i = 0;
  struct dirent* file = readdir(rep);
  while(file != NULL)
  {
    i++;
    file = readdir(rep);
  }
  i -= 2;
  char file_remove[25];
  char omg2[25];
  char newname[25];
  char *st = malloc(2*sizeof(char));
  *st = i + 47;
  sprintf(file_remove, "%s/%s", dirname, s);
  printf("%s\n", number);
  printf("file to supp%s\n", file_remove);
  remove(file_remove);
  sprintf(newname, "%s%s", dirname, st);
  sprintf(omg2, "%s/%s",dirname, newname);
  printf("file to rename %s\n", omg2);
  printf("new name %s", file_remove);
  rename(omg2, file_remove);
}

////////////////////////////////////////////////////////////////
//////////////////////////DISPLAY_NAME//////////////////////////
////////////////////////////////////////////////////////////////

void display_name()
{
  FILE *f = fopen("file.txt", "r");
  char *name = malloc(100);
  int i = 0;
  char c = fgetc(f);
  while(c != EOF)
  {
    if(c == '\n')
    {
      *(name+i) = ';';
       i++;
    }
    if(c > 57)
    {
      *(name+i) = c;
      i++;
    }
    c = fgetc(f);
  }
  *(name+i) = 0;
  printf("%s\n", name);
  fclose(f);
}
