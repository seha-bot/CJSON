#include<stdio.h>
#include<stdlib.h>
#include<nec.h>

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
                return o->keys[i][j + 1];
            }
            if(o->keys[i][j] == '\0' || key[j] == '\0') j = -2;
            j++;
        }
    }
    printf("JSON: object not found.\n");
    return 0;
}

int stoi(char* s)
{
    int v = 0;
    int i = 0;
    if(s[0] == '-') i = 1;
    while(s[i] != '\0')
    {
        v *= 10;
        v += s[i++] - '0';
    }
    if(s[0] == '-') v = -v;
    return v;
}
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
        else if(data[i] == ',' && state == 2 || data[i] == ',' && state == 3)
        {
            state = 0;

            nec_push(k, '\0');
            nec_push(v, '\0');

            if(v[nec_size(v) - 2] == '"')
            {
                nec_push(k, nec_size(root.strings));
                nec_push(root.keys, k);
                v[nec_size(v) - 2] = '\0';
                nec_push(root.strings, v);
            }
            else
            {
                if(v[0] == 't')
                {
                    nec_push(k, nec_size(root.booleans));
                    nec_push(root.keys, k);
                    nec_push(root.booleans, 1);
                }
                else if(v[0] == 'f')
                {
                    nec_push(k, nec_size(root.booleans));
                    nec_push(root.keys, k);
                    nec_push(root.booleans, 0);
                }
                else
                {
                    char isint = 1;
                    for(int j = 0; j < nec_size(v); j++)
                    {
                        if(v[j] == '.')
                        {
                            isint = 0;
                            break;
                        }
                    }
                    if(isint)
                    {
                        nec_push(k, nec_size(root.ints));
                        nec_push(root.keys, k);
                        nec_push(root.ints, stoi(v));
                    }
                    else
                    {
                        nec_push(k, nec_size(root.doubles));
                        nec_push(root.keys, k);
                        nec_push(root.doubles, stod(v));
                    }
                }
                nec_free(v);
            }

            k = 0;
            v = 0;

            continue;
        }
        else if(state == 0 && data[i] == '}')
        {
            return root;
        }

        if(data[i] == '"' && state == 2)
        {
            state = 3;
            i++;
        }

        if(state == 1)
        {
            nec_push(k, data[i]);
        }
        else if(state == 2 || state == 3)
        {
            nec_push(v, data[i]);
        }
        printf("%c %d\n", data[i], state);
    }
    printf("error");
}

int main()
{
    char* data = "{\"ajdi\":3,\"dupli\":123312.32321,\"ime\":\"bob\",\"jel\":true,\"nijel\":false,}";
    json o = parse(data, 0);

    printf("INTS:\n");
    for(int i = 0; i < nec_size(o.ints); i++)
    {
        printf("%d\n", o.ints[i]);
    }
    printf("DOUBLES:\n");
    for(int i = 0; i < nec_size(o.doubles); i++)
    {
        printf("%f\n", o.doubles[i]);
    }
    printf("STRINGS:\n");
    for(int i = 0; i < nec_size(o.strings); i++)
    {
        printf("%s\n", o.strings[i]);
    }
    printf("BOOLEANS:\n");
    for(int i = 0; i < nec_size(o.booleans); i++)
    {
        printf("%s\n", o.booleans[i] ? "true" : "false");
    }
    return 0;
}