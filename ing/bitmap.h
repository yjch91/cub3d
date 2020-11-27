#ifndef BITMAP_H

# define BITMAP_H

# pragma pack(push, 1)

typedef struct	s_bit     // BMP 비트맵 파일 헤더 구조체
{
	unsigned short	ftype;           // BMP 파일 매직 넘버
    unsigned int	fsize;           // 파일 크기
    unsigned short	freserved1;      // 예약
    unsigned short	freserved2;      // 예약
    unsigned int	foffbits;        // 비트맵 데이터의 시작 위치

    unsigned int   size;           // 현재 구조체의 크기
    int            bit_width;          // 비트맵 이미지의 가로 크기
    int            bit_height;         // 비트맵 이미지의 세로 크기
    unsigned short planes;         // 사용하는 색상판의 수
    unsigned short bitcount;       // 픽셀 하나를 표현하는 비트 수
    unsigned int   compression;    // 압축 방식
    unsigned int   sizeimage;      // 비트맵 이미지의 픽셀 데이터 크기
    int            x_pelspermeter;  // 그림의 가로 해상도(미터당 픽셀)
    int            y_pelspermeter;  // 그림의 세로 해상도(미터당 픽셀)
    unsigned int   colorused;        // 색상 테이블에서 실제 사용되는 색상 수
    unsigned int   colorimportant;   // 비트맵을 표현하기 위해 필요한 색상 인덱스 수
}				t_bit;

# pragma pack(pop)

#endif
