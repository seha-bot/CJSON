#include<stdio.h>
#include<stdlib.h>
#include<nec.h>

// struct json;
// struct json
// {
//     struct json *objects;
//     void *data;
// };

typedef struct
{
    char** key;
    void* value;
} json;

json json_init()
{
    json o;
    o.key = 0;
    o.value = 0;
    return o;
}

#define json_push(t, o, _key, _value) ({nec_push(o.key, _key); nec_push_typecast(t, o.value, _value);})

unsigned int jg(json *o, char* key)
{
    for(unsigned int i = 0; i < nec_size(o->key); i++)
    {
        if(o->key[i] == key)
        {
            return i;
        }
    }
    return 0;
}
#define json_get(t, o, _key) (((t*)o.value)[jg(&o, _key)])

json parse(char* buffer)
{

}

int main()
{
    json o = json_init();

    // json_push(int, o, "a", 1);
    // json_push(int, o, "b", 2);
    // json_push(int, o, "c", 3);
    // json_push(int, o, "d", 4);

    // printf("%d\n", json_get(int, o, "d"));

    // for(int i = 1; i <= 40; i++)
    // {
    //     json_push(double, o, "a", i + 0.1);
    // }
    // for(int i = 0; i < nec_size(o.value); i++)
    // {
    //     printf("%f\n", ((double*)o.value)[i]);
    // }

    // printf("SIZE = %d\n", nec_size(o.value));
    return 0;
}