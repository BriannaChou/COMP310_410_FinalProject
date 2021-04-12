#ifndef __LIST_H
#define __LIST_H

struct listElement {
    struct listElement *next;
    struct listElement *prev;
};

void listAdd(struct listElement **head, struct listElement *newElement);
void listRemove(struct listElement *b);
#endif
