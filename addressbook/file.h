#ifndef FILE_H
#define FILE_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "contact.h"

#include<ctype.h>

void saveContactsToFile(Addressbook *Book);

void loadContactsFromFile(Addressbook *Book);

#endif
