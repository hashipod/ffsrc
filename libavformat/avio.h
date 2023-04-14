#ifndef AVIO_H
#define AVIO_H

// �ļ���дģ�鶨������ݽṹ�ͺ�������

#define URL_EOF (-1)

typedef int64_t offset_t;

// �򵥵��ļ���ȡ�궨��
#define URL_RDONLY 0
#define URL_WRONLY 1
#define URL_RDWR 2

// URLContext �ṹ��ʾ�������еĵ�ǰ���������ļ�ʹ�õ������ģ�
// ���������й��������ļ����е�����(�������ڳ�������ʱ����ȷ����ֵ)�͹��������ṹ���ֶ�
typedef struct URLContext {
    struct URLProtocol *prot; // prot �ֶι�����Ӧ�Ĺ��������ļ�
    int flags;                // �ļ���д����
    int max_packet_size;      // �����0����ʾ������С�����ڷ����㹻�Ļ��档
    void *priv_data;          // �ڱ����У�����һ���ļ����
    char filename[1];         // specified filename
} URLContext;

// ��ʾ����������ļ��������ڹ��ܺ�����������ffplayֻ֧��fileһ�������ļ�
typedef struct URLProtocol {
    const char *name;
    int (*url_open)(URLContext *h, const char *filename, int flags);
    int (*url_read)(URLContext *h, unsigned char *buf, int size);
    int (*url_write)(URLContext *h, unsigned char *buf, int size);
    offset_t (*url_seek)(URLContext *h, offset_t pos, int whence);
    int (*url_close)(URLContext *h);
    struct URLProtocol
        *next; // ���ڰ�����֧�ֵĹ���������ļ����ӳ��������ڱ������ҡ�
} URLProtocol;

// ByteIOContext�ṹ
//	+-------------------+-------------------+--------------------------+--------------------+
//	|					|	�����Ѿ�ʹ��	   |  ����δʹ������	|	�ļ�δ������
//|
//	+-------------------+-------------------+--------------------------+--------------------+
//	|                   |			|                  |				|
// �ļ���ʼλ��		  buffer			 buf_ptr            buf_end

// ��չURLProtocol�ṹ���ڲ��л�����ƵĹ㷺�����ϵ��ļ������ƹ��������ļ���IO���ܡ�
typedef struct ByteIOContext {
    unsigned char *buffer;            // �����׵�ַ
    int buffer_size;                  // �����С
    unsigned char *buf_ptr, *buf_end; // �����ָ���ĩָ��
    void *opaque;                     // ����URLContext
    int (*read_buf)(void *opaque, uint8_t *buf, int buf_size);
    int (*write_buf)(void *opaque, uint8_t *buf, int buf_size);
    offset_t (*seek)(void *opaque, offset_t offset, int whence);
    offset_t pos;        // position in the file of the current buffer
    int must_flush;      // true if the next seek should flush
    int eof_reached;     // true if eof reached
    int write_flag;      // true if open for writing
    int max_packet_size; // �����0����ʾ�������֡��С�����ڷ����㹻�Ļ��档
    int error;           // contains the error code or 0 if no error happened
} ByteIOContext;

int url_open(URLContext **h, const char *filename, int flags);
int url_read(URLContext *h, unsigned char *buf, int size);
int url_write(URLContext *h, unsigned char *buf, int size);
offset_t url_seek(URLContext *h, offset_t pos, int whence);
int url_close(URLContext *h);
int url_get_max_packet_size(URLContext *h);

int register_protocol(URLProtocol *protocol);

int init_put_byte(ByteIOContext *s, unsigned char *buffer, int buffer_size,
                  int write_flag, void *opaque,
                  int (*read_buf)(void *opaque, uint8_t *buf, int buf_size),
                  int (*write_buf)(void *opaque, uint8_t *buf, int buf_size),
                  offset_t (*seek)(void *opaque, offset_t offset, int whence));

offset_t url_fseek(ByteIOContext *s, offset_t offset, int whence);
void url_fskip(ByteIOContext *s, offset_t offset);
offset_t url_ftell(ByteIOContext *s);
offset_t url_fsize(ByteIOContext *s);
int url_feof(ByteIOContext *s);
int url_ferror(ByteIOContext *s);

int url_fread(ByteIOContext *s, unsigned char *buf, int size); // get_buffer
int get_byte(ByteIOContext *s);
unsigned int get_le32(ByteIOContext *s);
unsigned int get_le16(ByteIOContext *s);

int url_setbufsize(ByteIOContext *s, int buf_size);
int url_fopen(ByteIOContext *s, const char *filename, int flags);
int url_fclose(ByteIOContext *s);

int url_open_buf(ByteIOContext *s, uint8_t *buf, int buf_size, int flags);
int url_close_buf(ByteIOContext *s);

#endif
