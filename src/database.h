#ifndef DEF_DATABASE
#define DEF_DATABASE

void add_img(char* s, int id);
void delete_person(char* name);
void delete_name(int id);
int search_id(char *s);
char* search_name(int id);
void delete_img(char *s);
void delete_dir(int id);
char *toString(int i);
int toInt(char *str);
char *concat(char *s1, char *s2);
char* add(char* picture, char* name);
void display_name();
void change_name(char* name, char *new_name);

#endif
