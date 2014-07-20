//---------------------------------------------- Devices segments

#define TIMER_BASE      0xd3200000
#define TIMER_SIZE      0x00000080
#define TIMER_SPAN      4

#define ICU_BASE        0xd2200000
#define ICU_SIZE        0x00000020
#define ICU_SPAN        5

#define DMA_BASE        0xd1200000
#define DMA_SIZE        0x00000014
#define DMA_SPAN        5

#define TTY_BASE        0xd0200000
#define TTY_SIZE        0x00000040
#define TTY_SPAN        4

#define BD_BASE         0xd5200000
#define BD_SIZE         0x20
#define BD_SPAN         8

#define FB_XSIZE        512
#define FB_YSIZE        512
#define FB_BASE         0x52200000
#define FB_SIZE         FB_XSIZE*FB_YSIZE*2

//---------------------------------------------- ROM segments

#define KTEXT_LMA_BASE  0xbf800000
#define KTEXT_LMA_SIZE  0x00020000

#define KDATA_LMA_BASE  0xbf820000
#define KDATA_LMA_SIZE  0x00020000

#define BOOT_BASE       0xbfc00000
#define BOOT_SIZE       0x00001000

//---------------------------------------------- RAM segments

#define RAM_BASE        0x7F400000
#define RAM_SIZE_USER   0x00C00000
#define RAM_SIZE        0x01000000

//---------------------------------------------- Application segments

#define KTEXT_BASE      0x80000000
#define KDATA_BASE      0x80020000
#define KDATA_SIZE      0x003E0000

#define USR_TEXT_BASE   RAM_BASE
#define USR_TEXT_SIZE   0x00060000
#define USR_DATA_BASE   USR_TEXT_BASE + USR_TEXT_SIZE
#define USR_DATA_SIZE   0x00B9F000
