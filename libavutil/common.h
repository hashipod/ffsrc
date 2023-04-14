
#ifndef COMMON_H
#define COMMON_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
#define CONFIG_WIN32
#endif

// ���������Ĺؼ�����linux gcc ��windows vc �еĶ����ǲ�ͬ�ģ�gcc ��inline��vc
// ��__inline�� ��Ϊ�����Ǵ�linux ����ֲ�����ģ���������һ���궨���޸���Լ򵥡�
#ifdef CONFIG_WIN32
#define inline __inline
#endif

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

// �򵥵��������Ͷ��壬 linux gcc ��windows vc ������������ͬ��
// �ú꿪��CONFIG_WIN32������64λ�������͵Ĳ��
#ifdef CONFIG_WIN32
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
#endif

// 64 λ�����Ķ����﷨��linux gcc ��windows vc
// ������������ͬ���ú꿪��CONFIG_WIN32 ������64λ��������Ĳ�� Linux ��LL
// / ULL ����ʾ64 λ������VC ��i64 ����ʾ64
// λ������##�����ӷ�����##ǰ��������ַ������ӳ�һ���ַ�����
#ifdef CONFIG_WIN32
#define int64_t_C(c) (c##i64)
#define uint64_t_C(c) (c##i64)
#else
#define int64_t_C(c) (c##LL)
#define uint64_t_C(c) (c##ULL)
#endif

// ��������64 λ������
#ifndef INT64_MAX
#define INT64_MAX int64_t_C(9223372036854775807)
#endif

// ��Сд���е��ַ����ȽϺ�������ffplay��ֻ�����Ƿ���ȣ�������˭��˭С��
static int strcasecmp(char *s1, const char *s2) {
    while (toupper((unsigned char)*s1) == toupper((unsigned char)*s2++))
        if (*s1++ == '\0')
            return 0;

    return (toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}

// �޷��������������ʹ�ü򵥵ıȽ��߼���ʵ�֣��Ƚ����࣬�����ж�CPU
// ��ָ����ˮ�ߣ��������ܵ��¡� �������a
// ��ȡֵ��Χ�Ƚ�С�������ó���Ŀռ任ʱ��Ĳ�������Ż���
static inline int clip(int a, int amin, int amax) {
    if (a < amin)
        return amin;
    else if (a > amax)
        return amax;
    else
        return a;
}

#endif
