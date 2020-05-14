#ifndef SOCKPIPE_MAGIC
#define SOCKPIPE_MAGIC

typedef int type_t;

#define VIRTUAL

#define INVOKE_VIRTUAL(O, M, ...) ((O)->meta->M((O), ##__VA_ARGS__))
#define GET_VIRTUAL(O, M) (O)->meta->M

#define _INVOKE_METHOD(O, T, M, ...) (T ## _ ## M((T *)(O), ##__VA_ARGS__))

#define INVOKE_METHOD(O, T, M, ...) _INVOKE_METHOD(O, T, M, ##__VA_ARGS__)

#define _TYPE_OF(O) TYPE_OF_ ## O
#define TYPE_OF(O) _TYPE_OF(O)

#define INVOKE(O, ...) INVOKE_METHOD(O, TYPE_OF(O), __VA_ARGS__)

#define _DELETE_WITH_T(T, O) Destroy_ ## T (O)
#define DELETE_WITH_T(T, O) _DELETE_WITH_T(T, O)

#define DELETE(O) DELETE_WITH_T(TYPE_OF(O), O)

#define _DEF_METHOD(T, M) T ## _ ## M
#define DEF_METHOD(T, M) _DEF_METHOD(T, M)

#define _DEF_META(T) struct _Meta_ ## T
#define DEF_META(T) _DEF_META(T)


#define _ZIP14(T1,V1,T2,V2,T3,V3,T4,V4,T5,V5,T6,V6,T7,V7)\
    ,T1 V1, T2 V2, T3 V3, T4 V4, T5 V5, T6 V6, T7 V7
#define _ZIP12(T1,V1,T2,V2,T3,V3,T4,V4,T5,V5,T6,V6)\
    ,T1 V1, T2 V2, T3 V3, T4 V4, T5 V5, T6 V6
#define _ZIP10(T1,V1,T2,V2,T3,V3,T4,V4,T5,V5)\
    ,T1 V1, T2 V2, T3 V3, T4 V4, T5 V5
#define _ZIP8(T1,V1,T2,V2,T3,V3,T4,V4)\
    ,T1 V1, T2 V2, T3 V3, T4 V4
#define _ZIP6(T1,V1,T2,V2,T3,V3)\
    ,T1 V1, T2 V2, T3 V3
#define _ZIP4(T1,V1,T2,V2)\
    ,T1 V1, T2 V2
#define _ZIP2(T1,V1)\
    ,T1 V1
#define _ZIP0(...) 

#define _NZIP14(T1,V1,T2,V2,T3,V3,T4,V4,T5,V5,T6,V6,T7,V7)\
    T1 V1, T2 V2, T3 V3, T4 V4, T5 V5, T6 V6, T7 V7
#define _NZIP12(T1,V1,T2,V2,T3,V3,T4,V4,T5,V5,T6,V6)\
    T1 V1, T2 V2, T3 V3, T4 V4, T5 V5, T6 V6
#define _NZIP10(T1,V1,T2,V2,T3,V3,T4,V4,T5,V5)\
    T1 V1, T2 V2, T3 V3, T4 V4, T5 V5
#define _NZIP8(T1,V1,T2,V2,T3,V3,T4,V4)\
    T1 V1, T2 V2, T3 V3, T4 V4
#define _NZIP6(T1,V1,T2,V2,T3,V3)\
    T1 V1, T2 V2, T3 V3
#define _NZIP4(T1,V1,T2,V2)\
    T1 V1, T2 V2
#define _NZIP2(T1,V1)\
    T1 V1
#define _NZIP0(...) 

#define _CVALUE14(T1,V1,T2,V2,T3,V3,T4,V4,T5,V5,T6,V6,T7,V7)\
    ,V1, V2, V3, V4, V5, V6, V7
#define _CVALUE12(T1,V1,T2,V2,T3,V3,T4,V4,T5,V5,T6,V6)\
    ,V1, V2, V3, V4, V5, V6
#define _CVALUE10(T1,V1,T2,V2,T3,V3,T4,V4,T5,V5)\
    ,V1, V2, V3, V4, V5
#define _CVALUE8(T1,V1,T2,V2,T3,V3,T4,V4)\
    ,V1, V2, V3, V4
#define _CVALUE6(T1,V1,T2,V2,T3,V3)\
    ,V1, V2, V3
#define _CVALUE4(T1,V1,T2,V2)\
    ,V1, V2
#define _CVALUE2(T1,V1)\
    ,V1
#define _CVALUE0(...)

#define __NARG__(...)  __NARG_I_(_0, ##__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define __RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0

// general definition for any function name
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

#define _ZIP(...) VFUNC(_ZIP, ##__VA_ARGS__)
#define _NZIP(...) VFUNC(_NZIP, ##__VA_ARGS__)
#define _VALUE(...) VFUNC(_VALUE, ##__VA_ARGS__)
#define _CVALUE(...) VFUNC(_CVALUE, ##__VA_ARGS__)

#define _VIRTUAL(O, M, ...) (O)->meta->M(O __VA_ARGS__)

#define _GEN_Virtual(R, MT, T, M, ...) \
static inline R T ## _ ## M (T * self _ZIP(__VA_ARGS__)) {\
    return _VIRTUAL((MT *)self, M, _CVALUE(__VA_ARGS__));\
}

#define _GEN_Create(T, ...) \
static inline T * Create_ ## T (_NZIP(__VA_ARGS__)) {\
    T * self = (T *) SP_Alloc(sizeof(T));\
    memset(self, 0, sizeof(T));\
    INVOKE_METHOD(self, T, init _CVALUE(__VA_ARGS__));\
    return self;\
}

#define _GEN_Destroy(T) \
static inline void Destroy_ ## T (T * self) {\
    INVOKE_METHOD(self, T, del);\
    SP_Free(self);\
}

#define _GEN_Cleanup(T)\
static inline void __Cleanup_ ## T ( T ** selfp) {\
    Destroy_ ## T(*selfp);\
}

#define _CLEANUP(T) __Cleanup_ ## T
#define CLEANUP(T) _CLEANUP(T)

#define META(T) DEF_META(T) * meta

#define GEN_Create(...) _GEN_Create(__VA_ARGS__)
#define GEN_Destroy(...) _GEN_Destroy(__VA_ARGS__)
#define GEN_Virtual(...) _GEN_Virtual(__VA_ARGS__)
#define GEN_Cleanup(...) _GEN_Cleanup(__VA_ARGS__)

#define SCOPE(T) __attribute__((cleanup (CLEANUP(T)))) T


#endif //SOCKPIPE_MAGIC
