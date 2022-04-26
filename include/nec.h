typedef struct {
    unsigned int size;
} nec_array;

#define nec(a) ((nec_array*)(a) - 1)
#define nec_size(a) ((a) ? nec(a)->size : 0)
#define nec_realloc(a, s)\
({\
    void* __nec_b;\
    a ?\
    ((a) = realloc(nec(a), (s) * sizeof(*(a)) + sizeof(nec_array)), __nec_b = (nec_array*)(a) + 1, (a) = __nec_b):\
    ((a) = realloc((a), (s) * sizeof(*(a)) + sizeof(nec_array)), __nec_b = (nec_array*)(a) + 1, (a) = __nec_b);\
    nec(a)->size = s;\
})
#define nec_free(a) ({ void* __nec_b = nec(a); free(__nec_b); a = 0; })
#define nec_realloc_typecast(t, a, s) a = realloc(a, s * sizeof(t) + sizeof(nec_array))
#define nec_push(a, v)\
{\
    nec_realloc(a, nec_size(a) + 1);\
    (a)[nec_size(a) - 1] = v;\
}

#define nec_push_typecast(t, a, v)\
{\
    void* __nec_b; t* c; (a) ?\
    (__nec_b = (nec_array*)(a) - 1, a = __nec_b, nec_realloc_typecast(t, a, ++((nec_array*)(a))->size), __nec_b = (nec_array*)(a) + 1, (a) = __nec_b, 0):\
    ((nec_realloc_typecast(t, a, 1), __nec_b = (nec_array*)(a) + 1, nec(__nec_b)->size = 1, (a) = __nec_b), 0),\
    c = (t*)(a),\
    c[nec_size(a) - 1] = v,\
    a = (void*)c;\
}

#define nec_remove_at2(t, a, i)\
{\
    if(i >= 0 && i < nec_size(a))\
    {\
        void* __nec_b;\
        void* c;\
        if(nec_size(a) == 1)\
        {\
            nec_free(a);\
            (a) = 0;\
        }\
        else\
        {\
            __nec_b = (a);\
            (a) = 0;\
            (nec_realloc(a, nec_size(__nec_b) - 1), c = (nec_array*)(a) + 1, nec(c)->size = nec_size(__nec_b) - 1, (a) = c);\
            int offset = 0;\
            for(int it = 0; it < nec_size(__nec_b); it++)\
            {\
                if(it != i) a[it - offset] = (((t*)(__nec_b))[it]);\
                else offset++;\
            }\
            nec_free(__nec_b);\
        }\
    }\
}

//TODO: remake this function
#define nec_remove_at(t, a, i)\
{\
    if(i < nec_size(a))\
    {\
        void* __nec_b;\
        if(nec_size(a) == 1)\
        {\
            nec_free(a);\
            a = 0;\
        }\
        else\
        {\
            t* new_a = malloc(sizeof(t) * (nec_size(a) - 1) + sizeof(nec_array)); /*This is the reason for the remake and it also doesn't work*/\
            __nec_b = (nec_array*)new_a + 1;\
            new_a = __nec_b;\
            nec(new_a)->size = nec_size(a) - 1;\
            int offset = 0;\
            for(int it = 0; it < nec_size(a); it++)\
            {\
                if(it != i) new_a[it - offset] = a[it];\
                else offset++;\
            }\
            nec_free(a);\
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