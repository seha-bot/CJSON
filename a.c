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
                return stod(&o->keys[i][j + 1]);
            }
            if(o->keys[i][j] == '\0' || key[j] == '\0') j = -2;
            j++;
        }
    }
    printf("JSON: object not found.\n");
    return 0;
}

void keyappend(char* key, int v)
{
    while(v > 0)
    {
        nec_push(key, v % 10);
        v *= 0.1;
    }
}

json parse(char* data, int *i)
{
    json root = json_init();

    int state = 0;
    int string = 0;
    char* k = 0;
    char* v = 0;
    (*i)++;

    while(data[++(*i)] != '\0')
    {
        if(state == 0)
        {
            if(data[*i] == '"')
            {
                state = 1;
                *i += 2;
            }
            else nec_push(k, data[*i]);
        }
        if(state == 1)
        {
            if(data[*i] == '"') string++;
            if(string != 1 && (data[*i] == ',' || data[*i] == '}'))
            {
                nec_push(k, '\0');
                nec_push(v, '\0');

                if(v[nec_size(v) - 2] == '"')
                {
                    keyappend(k, nec_size(root.strings));
                    char* value = malloc(nec_size(v) - 2);
                    for(int j = 0; j < nec_size(v) - 1; j++) value[j] = v[j + 1];
                    value[nec_size(v) - 3] = '\0';
                    nec_push(root.strings, value);
                }
                else if(v[0] == 't' || v[0] == 'f')
                {
                    keyappend(k, nec_size(root.booleans));
                    nec_push(root.booleans, v[0] == 't' ? 1 : 0);
                }
                else
                {
                    double val = stod(v);
                    double temp = val;
                    if(temp < 0) temp = -val;
                    if(temp - (int)temp == 0.0)
                    {
                        keyappend(k, nec_size(root.ints));
                        nec_push(root.ints, val);
                    }
                    else
                    {
                        keyappend(k, nec_size(root.doubles));
                        nec_push(root.doubles, val);
                    }
                }

                nec_push(root.keys, k);
                nec_free(v);
                k = 0;
                v = 0;

                if(data[(*i)++] == '}') return root;
                state = string = 0;
                continue;
            }
            else nec_push(v, data[*i]);
        }
        // printf("%c %d\n", data[*i], state);
    }
    printf("error");
}

int main()
{
    char* data = "{\"ajdi\":3,\"dupli\":123312.32321,\"ime\":\"bob\",\"jel\":true,\"nijel\":false}";
    int i = 0;
    json o = parse(data, &i);

    // printf("k[]=%d\n", o.ints[json_get(&o, "ajdi")]);

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