#include<json.h>

int main()
{
    char* data = "{\"Ime_ID\":300,\"Ime\":\"Mujo\",\"Nesto\":654.321,\"wasd\":true,\"obby\":{\"int\":5},\"aray\":[true,4.32,\"5\",6,{\"test\":true}]}";
    json o = json_parse(data);
    data = 0;
    json_to_string(o, &data);
    printf("%s\n", data);

    return 0;
}