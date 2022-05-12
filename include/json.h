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
    if(v == 0) { nec_push(*s, '0'); }
    else
    {
        char* sinv = 0;
        while(v > 0)
        {
            nec_push(sinv, '0' + (v % 10));
            v *= 0.1;
        }
        for(int i = nec_size(sinv) - 1; i >= 0; i--)
        {
            nec_push(*s, sinv[i]);
        }
    }
    nec_push(*s, '\0');
}

struct json;
struct json
{
    struct json *objects;
    double* numbers;
    char** strings;
    char** keys;
};
typedef struct json json;

json json_init()
{
    json o;
    o.objects = 0;
    o.numbers = 0;
    o.strings = 0;
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
                    v += s[it++] - '0';
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

void json_push_number(json *o, char* key, double value)
{
    char* k = 0;
    int i = 0;
    do nec_push(k, key[i]); while(key[i++] != '\0' && key[i] != '"');
    if(key[i] == '"') key[i] = '\0';
    nec_push(k, 'd');
    itonecs(&k, nec_size(o->numbers));
    nec_push(o->keys, k);
    nec_push(o->numbers, value);
}
void json_push_string(json *o, char* key, char* value)
{
    char* k = 0;
    int i = 0;
    do nec_push(k, key[i]); while(key[i++] != '\0' && key[i] != '"');
    if(key[i] == '"') key[i] = '\0';
    nec_push(k, 's');
    itonecs(&k, nec_size(o->strings));
    nec_push(o->keys, k);
    nec_push(o->strings, value);
}
void json_push_object(json *o, char* key, json value)
{
    char* k = 0;
    int i = 0;
    do nec_push(k, key[i]); while(key[i++] != '\0' && key[i] != '"');
    if(key[i] == '"') key[i] = '\0';
    nec_push(k, 'o');
    itonecs(&k, nec_size(o->objects));
    nec_push(o->keys, k);
    nec_push(o->objects, value);
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
                    nec_push(k, data[*i] == '{' ? 'o' : 'a');
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
                else
                {
                    nec_push(k, 'd');
                    if(mode == 0) itonecs(&k, nec_size(root->numbers));
                    nec_push(root->numbers, stod(v));
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
            v += object.keys[i][it++] - '0';
        }
        it = 0;
        if(object.keys[i][j+1] == 'd')
        {
            itonecs(data, object.numbers[v]);
            (*data)[nec_size(*data) - 1] = '.';
            double a = object.numbers[v] - (double)( (int)(object.numbers[v]) );
            while(a - (double)( (int)(a) ) > 0.0000001) a *= 10.0;
            itonecs(data, a);
            nec_remove_at(char, *data, nec_size(*data) - 1);
        }
        else if(object.keys[i][j+1] == 's')
        {
            nec_push(*data, '"');
            while(object.strings[v][it] != '\0')
            {
                nec_push(*data, object.strings[v][it++]);
            }
            nec_push(*data, '"');
        }
        else if(object.keys[i][j+1] == 'o')
        {
            json_to_string(object.objects[v], data);
        }
        else printf("JSON: Error.\n");
        nec_push(*data, ',');
    }
    nec_remove_at(char, *data, nec_size(*data) - 1);
    nec_push(*data, '}');
}

#define json_parse(d) ({ json o_new = json_init(); int i = 0; parse(&o_new, d, &i, 0); o_new; })

void json_free(json o)
{
    if(o.numbers) nec_free(o.numbers);
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