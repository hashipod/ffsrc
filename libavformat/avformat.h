#ifndef AVFORMAT_H
#define AVFORMAT_H

// ����ʶ���ļ���ʽ��ý�����Ϳ�ʹ�õĺꡢ���ݽṹ�ͺ�����ͨ����Щ�ꡢ���ݽṹ�ͺ����ڴ�ģ�������ȫ����Ч��
#ifdef __cplusplus
extern "C" {
#endif

#define LIBAVFORMAT_VERSION_INT ((50 << 16) + (4 << 8) + 0)
#define LIBAVFORMAT_VERSION 50.4.0
#define LIBAVFORMAT_BUILD LIBAVFORMAT_VERSION_INT

#define LIBAVFORMAT_IDENT "Lavf" AV_STRINGIFY(LIBAVFORMAT_VERSION)

#include "../libavcodec/avcodec.h"
#include "avio.h"

// ����ֵ
#define AVERROR_UNKNOWN (-1)     // unknown error
#define AVERROR_IO (-2)          // i/o error
#define AVERROR_NUMEXPECTED (-3) // number syntax expected in filename
#define AVERROR_INVALIDDATA (-4) // invalid data found
#define AVERROR_NOMEM (-5)       // not enough memory
#define AVERROR_NOFMT (-6)       // unknown format
#define AVERROR_NOTSUPP (-7)     // operation not supported

#define AVSEEK_FLAG_BACKWARD 1 // seek backward
#define AVSEEK_FLAG_BYTE 2     // seeking based on position in bytes
#define AVSEEK_FLAG_ANY 4      // seek to any frame, even non keyframes

#define AVFMT_NOFILE 0x0001 // no file should be opened

#define PKT_FLAG_KEY 0x0001

#define AVINDEX_KEYFRAME 0x0001

#define AVPROBE_SCORE_MAX 100

#define MAX_STREAMS 20

// ��������Ƶ����֡�����е�������һЩ��ǣ�ʱ����Ϣ����ѹ�������׵�ַ����С����Ϣ��
// ����Ƶ���ݰ����壬��������ffplay �У�ÿһ������һ������������֡��
// ע�Ᵽ������Ƶ���ݰ����ڴ���malloc �����ģ������Ӧ��ʱ��free �黹��ϵͳ��
typedef struct AVPacket {
  int64_t pts;      // ��ʾʱ�䣬����Ƶ����ʾʱ��
  int64_t dts;      // ����ʱ�䣬������Ǻ���Ҫ?
  int64_t pos;      // byte position in stream, -1 if unknown
  uint8_t *data;    // ʵ�ʱ�������Ƶ���ݻ�����׵�ַ
  int size;         // ʵ�ʱ�������Ƶ���ݻ���Ĵ�С
  int stream_index; // ��ǰ����Ƶ���ݰ���Ӧ�����������ڱ���������������Ƶ������Ƶ��
  int flags;        // ���ݰ���һЩ��ǣ������Ƿ��ǹؼ�֡�ȡ�
  void (*destruct)(struct AVPacket *);
} AVPacket;

// ������ƵAVPacket���һ��С����
// ����Ƶ���ݰ������壬ע��ÿһ��AVPacketList
// ������һ��AVPacket���ʹ�ͳ�ĺܶ�ܶ�ڵ��list��ͬ����Ҫ��list �����Ի�
typedef struct AVPacketList {
  AVPacket pkt;
  struct AVPacketList *next; // ���ڰѸ���AVPacketList ����������
} AVPacketList;

// �ͷŵ�����Ƶ���ݰ�ռ�õ��ڴ棬���׵�ַ�ÿ���һ���ܺõ�ϰ�ߡ�
static inline void av_destruct_packet(AVPacket *pkt) {
  av_free(pkt->data);
  pkt->data = NULL;
  pkt->size = 0;
}

// �ͷŵ�����Ƶ���ݰ�ռ�õ��ڴ档
static inline void av_free_packet(AVPacket *pkt) {
  if (pkt && pkt->destruct)
    pkt->destruct(pkt);
}

// ���ļ������ݰ��������ݣ�ע������ܵ�����ʱ���ļ�ƫ������ȷ����Ҫ�����ݵĴ�СҲȷ����
// �������ݰ��Ļ���û�з��䡣������ڴ��Ҫ��ʼ������һЩ������
static inline int av_get_packet(ByteIOContext *s, AVPacket *pkt, int size) {
  int ret;
  unsigned char *data;
  if ((unsigned)size > (unsigned)size + FF_INPUT_BUFFER_PADDING_SIZE)
    return AVERROR_NOMEM;
  // �������ݰ�����
  data = av_malloc(size + FF_INPUT_BUFFER_PADDING_SIZE);
  if (!data)
    return AVERROR_NOMEM;

  memset(data + size, 0, FF_INPUT_BUFFER_PADDING_SIZE);

  // ��ʼ�����ֵ
  pkt->pts = AV_NOPTS_VALUE;
  pkt->dts = AV_NOPTS_VALUE;
  pkt->pos = -1;
  pkt->flags = 0;
  pkt->stream_index = 0;
  pkt->data = data;
  pkt->size = size;
  pkt->destruct = av_destruct_packet;

  pkt->pos = url_ftell(s);
  // ʵ�ʶ������ļ�������ݰ���������ļ�����ʱͨ���ǵ���ĩβ��Ҫ�黹�ո�malloc�������ڴ档
  ret = url_fread(s, pkt->data, size);
  if (ret <= 0)
    av_free_packet(pkt);
  else
    pkt->size = ret;

  return ret;
}

// Ϊʶ���ļ���ʽ�� Ҫ��һ�����ļ�ͷ����������ƥ��ffplay֧�ֵ��ļ���ʽ�ļ�������
typedef struct AVProbeData {
  const char *filename;
  unsigned char *buf;
  int buf_size;
} AVProbeData;

// �ļ������ṹ��flags��sizeλ������Ϊ�˽�ʡ�ڴ档
typedef struct AVIndexEntry {
  int64_t pos;
  int64_t timestamp;
  int flags : 2;
  int size : 30; // yeah trying to keep the size of this small to reduce memory
                 // requirements (its 24 vs 32 byte due to possible 8byte align)
} AVIndexEntry;

// ��ʾ��ǰý�����������ģ�����������ý�������е�����(�������ڳ�������ʱ����ȷ����ֵ)�͹��������ṹ���ֶ�
typedef struct AVStream {
  AVCodecContext *actx; // ������ǰ����Ƶý��ʹ�õı������

  void *priv_data; // AVIStream ����������������ý�������ļ������йصĶ��е�����

  AVRational time_base; // �� av_set_pts_info()������ʼ��

  AVIndexEntry *index_entries; // only used if the format does not support
                               // seeking natively
  int nb_index_entries;
  int index_entries_allocated_size;

  double frame_last_delay; // ֡����ӳ�
} AVStream;

// AVFormatParameters
// �ṹ��������ffplay��û��ʵ�����壬Ϊ��֤�����ӿڲ��䣬û��ɾ����
typedef struct AVFormatParameters {
  int dbg; // only for debug
} AVFormatParameters;

// AVInputFormat ���������ļ�������ʽ�������ڹ��ܺ�����
// һ���ļ�������ʽ��Ӧһ��AVInputFormat�ṹ���ڳ�������ʱ�ж��ʵ�����������ffplay
// ��һ��ʵ����
typedef struct AVInputFormat {
  const char *name; // �ļ�������ʽ�����������Ի��Ķ���ά������

  int priv_data_size; // ��ʾ������ļ�������ʽ��Ӧ��Context�Ĵ�С

  int (*read_probe)(AVProbeData *);

  int (*read_header)(struct AVFormatContext *, AVFormatParameters *ap);

  int (*read_packet)(struct AVFormatContext *, AVPacket *pkt);

  int (*read_close)(struct AVFormatContext *);

  const char *extensions; // �ļ���չ��

  struct AVInputFormat
      *next; // ���ڰ�ffplay ֧�ֵ������ļ�������ʽ����һ������

} AVInputFormat;

// AVFormatContext
// �ṹ��ʾ�������еĵ�ǰ�ļ�������ʽʹ�õ������ģ������������ļ��������е�����(��
// �����ڳ�������ʱ����ȷ����ֵ)�ͳ������к��һ��ʵ����
typedef struct AVFormatContext // format I/O context
{
  struct AVInputFormat *iformat; // ������Ӧ���ļ�������ʽ

  void *
      priv_data; // ָ�������ļ�������ʽ��������Context��priv_data_size���ʹ��

  ByteIOContext pb; // �㷺����������ļ� ????

  int nb_streams; // ���������ļ���ý��������

  AVStream *streams[MAX_STREAMS]; // ��������Ƶ��

} AVFormatContext;

int avidec_init(void);

void av_register_input_format(AVInputFormat *format);

void av_register_all(void);

AVInputFormat *av_probe_input_format(AVProbeData *pd, int is_opened);
int match_ext(const char *filename, const char *extensions);

int av_open_input_stream(AVFormatContext **ic_ptr, ByteIOContext *pb,
                         const char *filename, AVInputFormat *fmt,
                         AVFormatParameters *ap);

int av_open_input_file(AVFormatContext **ic_ptr, const char *filename,
                       AVInputFormat *fmt, int buf_size,
                       AVFormatParameters *ap);

int av_read_frame(AVFormatContext *s, AVPacket *pkt);
int av_read_packet(AVFormatContext *s, AVPacket *pkt);
void av_close_input_file(AVFormatContext *s);
AVStream *av_new_stream(AVFormatContext *s, int id);
void av_set_pts_info(AVStream *s, int pts_wrap_bits, int pts_num, int pts_den);

int av_index_search_timestamp(AVStream *st, int64_t timestamp, int flags);
int av_add_index_entry(AVStream *st, int64_t pos, int64_t timestamp, int size,
                       int distance, int flags);

int strstart(const char *str, const char *val, const char **ptr);
void pstrcpy(char *buf, int buf_size, const char *str);

#ifdef __cplusplus
}

#endif

#endif
