#ifndef RATIONAL_H
#define RATIONAL_H

typedef struct AVRational {
    int num; // numerator   // 分子
    int den; // denominator // 分母
} AVRational;

static inline double av_q2d(AVRational a) { return a.num / (double)a.den; }

#endif
