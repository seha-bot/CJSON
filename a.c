#include<json.h>

// void fib()
// {
//     int a = 1, b = 1, i = 5;
//     while(i--)
//     {
//         b = a + b;
//         a = b - a;
//         printf("%d\n", b);
//     }
// }

int main()
{
    // char* data = "{\"ajdi\":3,\"dupli\":123312.32321,\"ime\":\"bob\",\"jel\":true,\"nijel\":false}";
    // char* data = "{\"ajdi\":{\"a\":1,\"b\":423}}";
    // char* data = "{\"ajdi\":[\"1\",\"423\"]}";
    char* data = "{\"Ime_ID\":300,\"Ime\":\"Mujo\",\"Nesto\":654.321,\"wasd\":true,\"obby\":{\"int\":5}}";
    json o = json_parse(data);
    data = 0;
    json_to_string(o, &data);
    for(int i = 0; i < nec_size(data); i++)
    {
        if(data[i] == '\0') printf("error");
        else printf("%c", data[i]);
    }
    printf("\n");
    
    // double* slova = 0;
    // for(int i = 1; i <= 10000; i++)
    // {
    //     nec_push(slova, (double)i);
    // }
    // nec_remove_at2(double, slova, 0);
    // printf("%d\n", nec_size(slova));

    // json o = json_init();
    // json_push(o, o.strings, "ajdi", "bob");
    // json_push(o, o.strings, "ajdi2", "bobby");

    // json ob = o.objects[json_get(&o, "ajdi")];
    // printf("k[]=%s\n", ob.strings[0]);
    // printf("k[]=%d\n", o.ints[json_get(&o, "Nesto")]);

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

/*
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;

    float r = 0.0, g = 0.0, b = 0.0;
    uv *= vec2(50.0);
    
    
    //r = sin(uv.y + sin(uv.x + iTime) * 2.0);
    
    float sinWave = sin(uv.y + sin(uv.x * 3.0 + iTime));
    r = distance(uv, vec2(uv.x, sinWave + 22.0));
    r = smoothstep(0.5, 0.0, r);
    
    
    fragColor = vec4(r, g, b, 1.0);
}
*/