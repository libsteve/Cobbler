#include "primitive.h"
#include "string.h"
#include "list.h"
#include <stdio.h>

primitive_declare(custom);

custom *method(custom, create);
void    method(custom, destroy);

primitive_define(custom, primitive, {
    list *words;
}, using_virtual(custom, create), using_virtual(custom, destroy));

custom *
method(custom, create)
{
    this = (custom *)SuperCreate();
    if (this) {
        this->words = create(list);
        static_call(list, push, this->words, pstring("hello"));
        static_call(list, push, this->words, pstring("goodbye"));
    }
    return this;
}

void
method(custom, destroy)
{
    disown(this->words);
    SuperDestroy();
}

void
method(custom, print)
{
    LIST_FOREACH(it, this->words)
    {
        const char *c = static_call(string, c_string, VALUE(it));
        printf("%s\n", c);
    }
}

int main()
{
    context {
        custom *c = auto(custom);
        static_call(custom, print, c);
    }
    return 0;
}
