#include<stdio.h>
#include<stdlib.h>
#include<nec.h>

struct json;
struct json
{
    // struct json *objects;
    // void *data;
    char** key;
    void* value;
};
typedef struct json json;


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
        int j = 0;
        while(j >= 0)
        {
            if(o->key[i][j] != key[j]) break;
            if(o->key[i][j] == '\0' && key[j] == '\0') return i;
            if(o->key[i][j] == '\0' || key[j] == '\0') j = -2;
            j++;
        }
    }
    printf("JSON: object not found.\n");
    return 0;
}
#define json_get(t, o, _key) (((t*)o.value)[jg(&o, _key)])

json parse(char* data, int i)
{
    json root = json_init();

    int state = 0;
    char* k = 0;
    char* v = 0;

    while(data[++i] != '\0')
    {
        if(data[i] == '"' && state == 0)
        {
            state = 1;
            i++;
        }
        else if(data[i] == '"' && state == 1)
        {
            state = 2;
            i += 2;
        }
        else if(data[i] == ',' && state == 2)
        {
            state = 0;

            nec_push(k, '\0');
            nec_push(v, '\0');
            json_push(char*, root, k, v);
            k = 0;
            v = 0;

            continue;
        }
        else if(state == 0 && data[i] == '}')
        {
            return root;
        }

        if(state == 1)
        {
            nec_push(k, data[i]);
        }
        else if(state == 2)
        {
            nec_push(v, data[i]);
        }
        printf("%c %d\n", data[i], state);
    }
    printf("error");
}

int main()
{
    char* data = "{\"ajdi\":3,\"ime\":\"bob\",}";
    json o = parse(data, 0);

    printf("VALU = %s\n", json_get(char*, o, "ime"));
    // printf("VALU = %s\n", ((char**)o.value)[1]);

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