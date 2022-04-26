#include<stdio.h>
#include<stdlib.h>
#include<nec.h>

double stod(char* s)
{
    double v = 0;
    int i = 0;
    int point = 0;
    if(s[0] == '-') i = 1;
    while(s[i] != '\0')
    {
        if(s[i] == '.')
        {
            point = i++;
            continue;
        }
        v *= 10;
        v += s[i++] - '0';
    }
    if(point) while(--i > point) v *= 0.1;
    if(s[0] == '-') v = -v;
    return v;
}

void itonecs(char** s, int v)
{
    while(v > 0)
    {
        nec_push(*s, v % 10);
        v *= 0.1;
    }
    nec_push(*s, '\0');
}

struct json;
struct json
{
    struct json *objects;
    int* ints;
    double* doubles;
    char** strings;
    char* booleans;
    char** keys;
};
typedef struct json json;

json json_init()
{
    json o;
    o.objects = 0;
    o.ints = 0;
    o.doubles = 0;
    o.strings = 0;
    o.booleans = 0;
    o.keys = 0;
    return o;
}

unsigned int json_get(json *o, char* key)
{
    for(unsigned int i = 0; i < nec_size(o->keys); i++)
    {
        int j = 0;
        while(j >= 0)
        {
            if(o->keys[i][j] != key[j]) break;
            if(o->keys[i][j] == '\0' && key[j] == '\0')
            {
                char *s = &(o->keys[i][j + 2]);
                int v = 0;
                int it = 0;
                while(s[it] != '\0')
                {
                    v *= 10;
                    v += s[it++];
                }
                return v;
            }
            if(o->keys[i][j] == '\0' || key[j] == '\0') j = -2;
            j++;
        }
    }
    printf("JSON: object not found.\n");
    return 0;
}

void parse(json *root, char* data, int *i, int mode)
{
    int state = 0;
    int string = 0;
    char* k = 0;
    char* v = 0;
    (*i)++;

    while(data[++(*i)] != '\0')
    {
        if(state == 0)
        {
            if(mode == 1)
            {
                state = 1;
                (*i)--;
            }
            else if(data[*i] == '"')
            {
                state = 1;
                *i += 2;
            }
            else nec_push(k, data[*i]);
        }
        if(state == 1)
        {
            if(data[*i] == '"') string++;
            if(string != 1 && (data[*i] == ',' || data[*i] == '{' || data[*i] == '}' || data[*i] == '[' || data[*i] == ']'))
            {
                if(mode == 0) nec_push(k, '\0');
                nec_push(v, '\0');

                if(data[*i] == '{' || data[*i] == '[')
                {
                    nec_push(k, 'o');
                    if(mode == 0) itonecs(&k, nec_size(root->objects));
                    json arr = json_init();
                    parse(&arr, data, i, data[*i] == '{' ? 0 : 1);
                    nec_push(root->objects, arr);
                }
                else if(v[nec_size(v) - 2] == '"')
                {
                    nec_push(k, 's');
                    if(mode == 0) itonecs(&k, nec_size(root->strings));
                    char* value = malloc(nec_size(v) - 2);
                    for(int j = 0; j < nec_size(v) - 1; j++) value[j] = v[j + 1];
                    value[nec_size(v) - 3] = '\0';
                    nec_push(root->strings, value);
                }
                else if(v[0] == 't' || v[0] == 'f')
                {
                    nec_push(k, 'b');
                    if(mode == 0) itonecs(&k, nec_size(root->booleans));
                    nec_push(root->booleans, v[0] == 't' ? 1 : 0);
                }
                else
                {
                    double val = stod(v);
                    double temp = val;
                    if(temp < 0) temp = -val;
                    if(temp - (int)temp == 0.0)
                    {
                        nec_push(k, 'i');
                        if(mode == 0) itonecs(&k, nec_size(root->ints));
                        nec_push(root->ints, val);
                    }
                    else
                    {
                        nec_push(k, 'd');
                        if(mode == 0) itonecs(&k, nec_size(root->doubles));
                        nec_push(root->doubles, val);
                    }
                }

                if(mode == 0) nec_push(root->keys, k);
                nec_free(v);
                k = 0;
                v = 0;

                if(data[(*i)++] == '}' || data[(*i) - 1] == ']') return;
                state = string = 0;
                continue;
            }
            else nec_push(v, data[*i]);
        }
        // printf("%c %d\n", data[*i], state);
    }
    printf("JSON: parsing error EOF.\n");
}

void json_to_string(json object, char** data)
{
    nec_push(*data, '{');
    for(int i = 0; i < nec_size(object.keys); i++)
    {
        nec_push(*data, '"');
        int j = 0;
        while(object.keys[i][j] != '\0')
        {
            nec_push(*data, object.keys[i][j++]);
        }
        nec_push(*data, '"');
        nec_push(*data, ':');
        int v = 0;
        int it = j + 2;
        while(object.keys[i][it] != '\0')
        {
            v *= 10;
            v += object.keys[i][it++];
        }
        it = 0;
        if(object.keys[i][j+1] == 'i')
        {
            itonecs(data, object.ints[v]);
            nec_remove_at2(char, *data, nec_size(*data) - 1);
        }
        if(object.keys[i][j] == 's')
        {
            nec_push(*data, '"');
            while(object.strings[v][it] != '\0')
            {
                nec_push(*data, object.strings[v][it++]);
            }
            nec_push(*data, '"');
        }
        else printf("ERrOR = %d\n", j);
        nec_push(*data, ',');
    }
    nec_push(*data, '}');
}

#define json_parse(d) ({ json o_new = json_init(); int i = 0; parse(&o_new, d, &i, 0); o_new; })
#define json_push(o, a, k, v) ({ char* key = 0; int i = 0; do nec_push(key, k[i]) while(k[i++] != '\0'); itonecs(&key, nec_size(a)); nec_push(o.keys, key); nec_push(a, v); })

void json_free(json o)
{
    if(o.ints) nec_free(o.ints);
    if(o.doubles) nec_free(o.doubles);
    if(o.booleans) nec_free(o.booleans);
    if(o.objects)
    {
        for(int i = 0; i < nec_size(o.objects); i++) json_free(o.objects[i]);
        nec_free(o.objects);
    }
    if(o.keys)
    {
        for(int i = 0; i < nec_size(o.keys); i++) nec_free(o.keys[i]);
        nec_free(o.keys);
    }
    if(o.strings)
    {
        for(int i = 0; i < nec_size(o.strings); i++) free(o.strings[i]);
        nec_free(o.strings);
    }
}