#include<json.h>

int main()
{
    // char* data = "{\"ajdi\":3,\"dupli\":123312.32321,\"ime\":\"bob\",\"jel\":true,\"nijel\":false}";
    // char* data = "{\"ajdi\":{\"a\":1,\"b\":423}}";
    char* data = "{\"ajdi\":[\"1\",\"423\"]}";
    // json o = json_parse(data);

    json o = json_init();
    json_push(o, o.strings, "ajdi", "bob");
    json_push(o, o.strings, "ajdi2", "bobby");

    // json ob = o.objects[json_get(&o, "ajdi")];
    // printf("k[]=%s\n", ob.strings[0]);
    printf("k[]=%s\n", o.strings[json_get(&o, "ajdi")]);

    // printf("INTS:\n");
    // for(int i = 0; i < nec_size(o.ints); i++)
    // {
    //     printf("%d\n", o.ints[i]);
    // }
    // printf("DOUBLES:\n");
    // for(int i = 0; i < nec_size(o.doubles); i++)
    // {
    //     printf("%f\n", o.doubles[i]);
    // }
    // printf("STRINGS:\n");
    // for(int i = 0; i < nec_size(o.strings); i++)
    // {
    //     printf("%s\n", o.strings[i]);
    // }
    // printf("BOOLEANS:\n");
    // for(int i = 0; i < nec_size(o.booleans); i++)
    // {
    //     printf("%s\n", o.booleans[i] ? "true" : "false");
    // }
    return 0;
}