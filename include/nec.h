typedef struct {
    unsigned int size;
} nec_array;

#define nec(a) ((nec_array*)(a) - 1)
#define nec_size(a) ((a) ? nec(a)->size : 0)
#define nec_realloc(a, s) a = realloc(a, s * sizeof(*(a)) + sizeof(nec_array))
#define nec_free(a) ({ void* b = (nec_array*)(a) - 1; free(b); a = 0; })
#define nec_realloc_typecast(t, a, s) a = realloc(a, s * sizeof(t) + sizeof(nec_array))
#define nec_push(a, v)\
{\
    void* b; (a) ?\
    (b = (nec_array*)(a) - 1, a = b, nec_realloc(a, ++((nec_array*)(a))->size), b = (nec_array*)(a) + 1, (a) = b, 0):\
    ((nec_realloc(a, 1), b = (nec_array*)(a) + 1, nec(b)->size = 1, a = b), 0),\
    (a)[nec_size(a) - 1] = v;\
}

#define nec_push_typecast(t, a, v)\
{\
    void* b; t* c; (a) ?\
    (b = (nec_array*)(a) - 1, a = b, nec_realloc_typecast(t, a, ++((nec_array*)(a))->size), b = (nec_array*)(a) + 1, a = b, 0):\
    ((nec_realloc_typecast(t, a, 1), b = (nec_array*)(a) + 1, nec(b)->size = 1, a = b), 0),\
    c = (t*)(a),\
    c[nec_size(a) - 1] = v,\
    a = (void*)c;\
}

#define nec_remove_at(t, a, i)\
{\
    if(i < nec_size(a))\
    {\
        void* b;\
        if(nec_size(a) == 1)\
        {\
            free(a);\
            a = 0;\
        }\
        else\
        {\
            t* new_a = malloc(sizeof(t) * (nec_size(a) - 1) + sizeof(nec_array));\
            b = (nec_array*)new_a + 1;\
            new_a = b;\
            nec(new_a)->size = nec_size(a) - 1;\
            int offset = 0;\
            for(int it = 0; it < nec_size(a); it++)\
            {\
                if(it != i) new_a[it - offset] = a[it];\
                else offset++;\
            }\
            free(a);\
            a = new_a;\
        }\
    }\
}

#define nec_remove(t, a, v)\
{\
    for(int i = 0; i < nec_size(a); i++)\
    {\
        if((a)[i] == v)\
        {\
            nec_remove_at(t, a, i);\
            break;\
        }\
    }\
}


/*
TODO:
add nec_push_norealloc
*/