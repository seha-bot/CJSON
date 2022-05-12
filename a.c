#include<json.h>

int main()
{
    // char* data = "{\"Ime_ID\":300,\"Ime\":\"Mujo\",\"Nesto\":654.321}";
    // json o = json_parse(data);
    json o0 = json_init();
    json_push_string(&o0, "555", "Tajmer");

    json o = json_init();
    json_push_number(&o, "Ucenik_ID", 1);
    json_push_string(&o, "Ime", "Mujo");
    json_push_object(&o, "Elektro", o0);
    char* data = 0;
    json_to_string(o, &data);
    printf("%s\n", data);

    return 0;
}