#ifndef MATHEMATICS_H
#define MATHEMATICS_H

// ��ѧ�ϵ��������㡣Ϊ���������������������������ʾ����ȷ���������㡣Ϊ��ֹ���������ǿ��ת��Ϊint
// 64 λ��������㡣
// �˴�����һЩ�򻯣����㾫�Ȼή�ͣ�����ͨ���˺��Ѹ�֪��������
static inline int64_t av_rescale(int64_t a, int64_t b, int64_t c) {
    return a * b / c;
}

#endif
