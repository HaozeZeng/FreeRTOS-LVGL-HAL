```c
带pack和不带pack的不同点
首先结构体变大,不够位就填充,但编译器不会读这些位
带pack按单字节填充该怎样就怎样
不带pack编译器会对齐不够就填充无意义的数但是编译器实际不会读这些无意义的数,
但是如果你非要在这些无意义的地方写 "((u8*)dest)[i]""结果这些地方不会放到正确的位置,结构体出现错位,填充的数据湮灭"
人工写((u8*)dest)[i]可以访问到这些无意义的数"结果你的有效数据就湮灭了"
Aligned: fontok=1, ugbkaddr=134678021, ugbksize=202050057
Packed: fontok=1, ugbkaddr=84148994, ugbksize=151521030
fontok Aligned编译器自动填充3个无意义数 0x2, 0x3, 0x4添加了这个空穴里湮灭了读不到
ugbkaddr读到 0x5 0x6 0x7 0x8为 0x08070605 为134678021 结果134678021成立
size读到 0x09,0x0A,0x0B,0X0C 为0x0C0B0A09 为202050057 print结果202050057成立
fontok Packed添加了这个空穴里湮灭了读不到
ugbkaddr读到 0x2, 0x3, 0x4, 0x5 0x05040302 为84,148,994 结84148994成立
size读到        0x6, 0x7, 0x8,0x09 为0x09080706 为151521030 print结果151521030成立
```

```c
typedef struct {
    u8 fontok;        // 字体状态
    u32 ugbkaddr;    // unigbk地址
    u32 ugbksize;    // unigbk大小
} _font_info_aligned;

// 定义一个打包的结构体
typedef struct __attribute__((packed)) {
    u8 fontok;        // 字体状态
    u32 ugbkaddr;    // unigbk地址
    u32 ugbksize;    // unigbk大小
} _font_info_packed;

void read_data(void* dest, size_t size) {
    // 模拟从 Flash 读取数据
    // 假设我们从 Flash 中读取到以下数据
    u8 flash_data[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8,0x09,0x0A,0x0B,0X0C}; // fontok=0x55, ugbkaddr=1, ugbksize=0
    for (size_t i = 0; i < size; i++) {
        ((u8*)dest)[i] = flash_data[i];
    }
}


void test() {

    _font_info_aligned ftinfo_aligned;
    _font_info_packed ftinfo_packed;
    printf("Size of aligned structure: %zu bytes\r\n", sizeof(ftinfo_aligned));
    printf("Size of packed structure: %zu bytes\r\n", sizeof(ftinfo_packed));


    read_data((u8*)&ftinfo_aligned, sizeof(ftinfo_aligned));
    read_data((u8*)&ftinfo_packed, sizeof(ftinfo_packed));


    printf("Aligned: fontok=%d, ugbkaddr=%lu, ugbksize=%lu\r\n", ftinfo_aligned.fontok, (unsigned long)ftinfo_aligned.ugbkaddr, (unsigned long)ftinfo_aligned.ugbksize);
    printf("Packed: fontok=%d, ugbkaddr=%lu, ugbksize=%lu\r\n", ftinfo_packed.fontok, (unsigned long)ftinfo_packed.ugbkaddr, (unsigned long)ftinfo_packed.ugbksize);
```

}