/*
                        ADM_PointArrayList.cpp
                        ----------------------
    This module is specialized implementation of array list. It can insert
    or remove points from list at specified position. Maximum capacity
    of PointArrayList is set to value 32 (ADM_PointArrayList.h).
    PointArrayList can be also serialized (or deserialized) to convert
    class data into string (from string).
    email: george.janec@gmail.com
    
    Copyright (C) 2009 Jiri Janecek

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ADM_Utils.h"
#include "ADM_PointArrayList.h"


/*
 * Initialization of class parameters.
 */
PointArrayList::PointArrayList()
{
    itemCount = 0;
    reset();
}

/*
 * Destroying whole list.
 */
PointArrayList::~PointArrayList()
{
    freeMem();
}

/*
 * Copies parameters from input object src.
 */
void PointArrayList::copy(const PointArrayList &src)
{
    freeMem();
    itemCount = src.itemCount;
    for (register int i = 0; i < itemCount; i++)
    {
        items[i] = new Point(*src.items[i]);
    }
}

/*
 * Frees all items of list.
 */
void PointArrayList::freeMem()
{
    for (register int i = 0; i < itemCount; i++)
    {
        if (items[i]) delete items[i];
    }
    memset(items, 0, sizeof(items));
    itemCount = 0;
}

/*
 * Sets default list.
 */
void PointArrayList::reset()
{
    freeMem();
    items[0] = new Point(0, 0);
    items[1] = new Point(255, 255);
    itemCount = 2;
}

/*
 * If list is empty returns true else returns false.
 */
bool PointArrayList::isEmpty() const
{
    return itemCount == 0;
}

/*
 * Returns item count.
 */
int PointArrayList::count() const
{
    return itemCount;
}

/*
 * Returns pointer to structure Point on specified position index.
 * If index is out of bounds then resulting value is NULL.
 */
p_Point PointArrayList::get(const int index) const
{
    if (index >= 0 && index < itemCount) return items[index];
    if (itemCount == 0)
    {
        printf("Error: List is empty!\n");
    }
    else
    {
        printf("Error: Index out of bounds (0,%d)! ", itemCount-1);
        PRINT_VAR(index);
    }
    return NULL;
}

/*
 * Searches list for key x (member of Point).
 * Returns position of Point if item exists. If item does not exist, method
 * returns insert position coded as -(insert_position + 1).
 */
int PointArrayList::search(const int x) const
{
    register int i;
    for (i = 0; i < itemCount && items[i]->x < x; i++)
        ;
    if (i == itemCount || items[i]->x != x)
        return -(i+1);  // can't find x, returns -(insert_position + 1)
    else
        return i;       // successfully found, returns item index.
}

/*
 * Inserts new Point at position index.
 */
bool PointArrayList::insert(const int index, int x, int y)
{
    if (itemCount >= LIST_CAPACITY) return false;
    
    if (index < 0 || index > itemCount)
    {
        printf("Error: Index out of bounds (0,%d)! ", itemCount);
        PRINT_VAR(index);
        return false;
    }

    // data shifting to the right
    for (register int i = itemCount - 1; i >= index; i--)
    {
        items[i+1] = items[i];
    }
    items[index] = new Point(x, y);  // insert at position index
    itemCount++;
    return true;
}

/*
 * Removes item at position index.
 */
void PointArrayList::remove(const int index)
{
    if (itemCount < 3) return;

    if (index < 0 || index > itemCount-1)
    {
        printf("Error: Index out of bounds (0,%d)! ", itemCount-1);
        PRINT_VAR(index);
        return;
    }
    
    if (items[index]) delete items[index];
    
    // data shifting to the left
    if (index < itemCount - 1)
    {
        memcpy(&items[index], &items[index+1], (itemCount - index - 1) * sizeof(p_Point));
    }
    items[itemCount - 1] = NULL;
    itemCount--;
}

/*
 * Serializes class parameters into string.
 */
char *PointArrayList::serialize() const
{
    char *tempPtr = new char[24 + itemCount * 26 + 1];
    char itemStr[26];
    char *p = tempPtr;
    p += sprintf(p, "%d;", itemCount);
    for (int i = 0; i < itemCount; i++)
    {
        p += items[i]->serialize(p);
        *p++ = ' ';
    }
    *p = '\0';
    return tempPtr;
}

/*
 * Restores class parameters from string.
 */
void PointArrayList::deserialize(char *str)
{
    char itemStr[26];
    if (!str)
    {
        printf("Can't deserialize, because no input string specified! ");
        PRINT_VAR(str);
        return;
    }
    freeMem();
    itemCount = atoi(strtok(str, ";"));
    for (int i = 0; i < itemCount; i++)
    {
        items[i] = new Point(strtok(NULL, " "));
    }
}

/* ===== Item printing methods (only for debuging) ===== */

void PointArrayList::println() const
{
    printf("PointArrayList(c:%d):\n{", itemCount);
    items[0]->print();
    for (register int i = 1; i < itemCount; i++)
    {
        printf(",");
        items[i]->print();
    }
    printf("}\n");
}

void PointArrayList::printAll() const
{
    printf("PointArrayList(c:%d):\n{", itemCount);
    if (items[0] == NULL)
        printf("NULL");
    else {
        printf("0x%X", items[0]); items[0]->print();
    }
    for (register int i = 1; i < LIST_CAPACITY; i++)
    {
        printf(",");
        if (items[i] == NULL)
            printf("NULL");
        else {
            printf("0x%X", items[i]); items[i]->print();
        }
    }
    printf("}\n");
}

