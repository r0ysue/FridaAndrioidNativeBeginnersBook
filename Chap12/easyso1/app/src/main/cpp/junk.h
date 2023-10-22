//
// Created by ting on 2019-09-17.
//

#ifndef _JUNK_H_
#define _JUNK_H_

#define JUNK_CODE        //是否插入垃圾代码的开关
#ifdef JUNK_CODE

#define junk_fun0                 li11li1o0
#define junk_fun1                 li11li1o1
#define junk_fun2                 li11li1o2
#define junk_fun3                 li11li1o3

static inline int junk_fun0(void) {
    volatile int i = 138, j = 1949;

    if ((i++) % 2 > 0) j *= i;
    if (j < 0) i *= 2;
    else return 0;

    i = 1;
    while (i++ < 2) {
        j /= i;
        j++;
        i++;
    }
    return i;
}

static inline int junk_fun1(void) {
    volatile int i = 21, j = 75;

    if ((i--) % 3 > 0) j *= i;
    if (j > 1) i *= 3;
    else return 1;

    i = 1;
    while (i++ < 3) {
        j /= i;
        j--;
        i++;
    }
    return j;
}

static inline int junk_fun2(void) {
    volatile int i = 56, j = 17;

    if ((i--) % 5 > 0) j *= i;
    if (j > 2) i *= 5;
    else return 0;

    i = 1;
    while (i++ < 5) {
        j *= i;
        j += 3;
        i += 3;
    }
    return i;
}

static inline int junk_fun3(void) {
    volatile int i = 1909, j = 131;

    if ((i--) % 7 > 0) j *= i;
    if (j > 3) i *= 7;
    else return 1;

    i = 1;
    while (i++ < 7) {
        j /= i;
        j -= 5;
        i += 5;
    }
    return i;
}

#define _JUNK_FUN_0 {if(junk_fun2())junk_fun1();if(junk_fun0()) junk_fun3();if(junk_fun1()) junk_fun2();if(junk_fun3()) junk_fun1(); \
                       if(junk_fun1())junk_fun0();if(junk_fun2()) junk_fun3();if(junk_fun3()) junk_fun1();if(junk_fun1()) junk_fun0();}
#define _JUNK_FUN_1 {if(junk_fun3())junk_fun1();if(junk_fun1()) junk_fun2();if(junk_fun2()) junk_fun0();if(junk_fun0()) junk_fun1(); \
                       if(junk_fun2())junk_fun1();if(junk_fun0()) junk_fun3();if(junk_fun1()) junk_fun2();if(junk_fun3()) junk_fun1();}
#define _JUNK_FUN_2 {if(junk_fun1())junk_fun0();if(junk_fun2()) junk_fun3();if(junk_fun3()) junk_fun1();if(junk_fun1()) junk_fun0(); \
                       if(junk_fun0())junk_fun2();if(junk_fun3()) junk_fun0();if(junk_fun0()) junk_fun3();if(junk_fun2()) junk_fun3();}
#define _JUNK_FUN_3 {if(junk_fun0())junk_fun2();if(junk_fun3()) junk_fun0();if(junk_fun0()) junk_fun3();if(junk_fun2()) junk_fun3(); \
                       if(junk_fun3())junk_fun1();if(junk_fun1()) junk_fun2();if(junk_fun2()) junk_fun0();if(junk_fun0()) junk_fun1();}

#else

#define _JUNK_FUN_0 {}
#define _JUNK_FUN_1 {}
#define _JUNK_FUN_2 {}
#define _JUNK_FUN_3 {}

#endif
#endif

