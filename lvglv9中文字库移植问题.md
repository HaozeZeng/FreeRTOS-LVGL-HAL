lvgl v9的字库

```c
lv_font_t SanJiBang16 = {
    .get_glyph_bitmap = __user_font_get_bitmap,
    .get_glyph_dsc = __user_font_get_glyph_dsc,
    .line_height = 20,
    .base_line = 0,
};
```

其中    

```c
.get_glyph_bitmap = __user_font_get_bitmap,
.get_glyph_dsc = __user_font_get_glyph_dsc,
```

改变了

```c
#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
        static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
        .glyph_bitmap = NULL,
        .glyph_dsc = NULL,
        .cmaps = NULL,
        .kern_dsc = NULL,
        .kern_scale = 0,
        .cmap_num = 0,
        .bpp = 4,
        .kern_classes = 0,
        .bitmap_format = LV_FONT_FMT_TXT_PLAIN,
#if LVGL_VERSION_MAJOR == 8
        .cache = &cache
#endif
};

//SimSun,,-1
//字模高度：18
//XBF字体,外部bin文件
const lv_font_t myFontSongJianTi16 = {
        .get_glyph_bitmap = __user_font_get_bitmap,
        .get_glyph_dsc = __user_font_get_glyph_dsc,
        .line_height = 18,
        .base_line = 0,
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
        .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
        .underline_position = -2,
        .underline_thickness = 1,
#endif
        .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
        .fallback = NULL,
#endif
        .user_data = NULL,
};
```

```c
static bool __user_font_get_glyph_dsc(const lv_font_t * font, lv_font_glyph_dsc_t * dsc_out, uint32_t unicode_letter, uint32_t unicode_letter_next) {
    if( unicode_letter>__g_xbf_hd.max || unicode_letter<__g_xbf_hd.min ) {
        return NULL;
    }
    uint32_t unicode_offset = sizeof(x_header_t)+(unicode_letter-__g_xbf_hd.min)*4;  // 每个字符的编码都是4个字节
    uint32_t *p_pos = (uint32_t *)__user_font_getdata(unicode_offset, 4);  // 这里应该是取该编码字符具体位图信息地址（基础信息+字模位置）
    if( p_pos[0] != 0 ) {
        glyph_dsc_t * gdsc = (glyph_dsc_t*)__user_font_getdata(p_pos[0], sizeof(glyph_dsc_t));
        dsc_out->adv_w = (uint8_t)gdsc->adv_w;
        dsc_out->box_h = (uint8_t)gdsc->box_h;
        dsc_out->box_w = (uint8_t)gdsc->box_w;
        dsc_out->ofs_x = (int8_t)gdsc->ofs_x;
        dsc_out->ofs_y = (int8_t)gdsc->ofs_y;
        //dsc_out->bpp   = __g_xbf_hd.bpp; //改成下面
        dsc_out->format = (uint8_t)__g_xbf_hd.bpp;
        dsc_out->gid.index = unicode_letter; //官方工具生成的字库赋的值就是uicode的id
        dsc_out->is_placeholder = false;
        return true;
    }
    return false;
}
```

```c
static const void * __user_font_get_bitmap(lv_font_glyph_dsc_t * g_dsc, lv_draw_buf_t * draw_buf)
{
    uint32_t unicode_letter = g_dsc->gid.index;
    uint8_t * bitmap_out = draw_buf->data;
    const lv_font_t *font = g_dsc->resolved_font;
    lv_font_fmt_txt_dsc_t * fdsc = (lv_font_fmt_txt_dsc_t *) font->dsc;

    if(unicode_letter >__g_xbf_hd.max || unicode_letter<__g_xbf_hd.min ) {
        return NULL;
    }

    uint32_t unicode_offset = sizeof(x_header_t)+(unicode_letter-__g_xbf_hd.min)*4;
    uint32_t *p_pos = (uint32_t *)__user_font_getdata(unicode_offset, 4);
    if( p_pos[0] != 0 ) {
        uint32_t pos = p_pos[0];
        glyph_dsc_t * gdsc = (glyph_dsc_t*)__user_font_getdata(pos, sizeof(glyph_dsc_t));
        glyph_dsc_t gdscPoint = {0};
        memcpy(&gdscPoint, gdsc, sizeof(glyph_dsc_t));
        gdsc = &gdscPoint;

        int32_t gsize = (int32_t) gdsc->box_w * gdsc->box_h;
        if(gsize == 0) return NULL;
        if(fdsc->bitmap_format == LV_FONT_FMT_TXT_PLAIN) {
            const uint8_t * bitmap_in = __user_font_getdata(pos+sizeof(glyph_dsc_t), gdsc->box_w*gdsc->box_h*__g_xbf_hd.bpp/8);
            uint8_t * bitmap_out_tmp = bitmap_out;
            int32_t i = 0;
            int32_t x, y;
            uint32_t stride = lv_draw_buf_width_to_stride(gdsc->box_w, LV_COLOR_FORMAT_A8);
            if(fdsc->bpp == 1) {
                for(y = 0; y < gdsc->box_h; y ++) {
                    for(x = 0; x < gdsc->box_w; x++, i++) {
                        i = i & 0x7;
                        if(i == 0) bitmap_out_tmp[x] = (*bitmap_in) & 0x80 ? 0xff : 0x00;
                        else if(i == 1) bitmap_out_tmp[x] = (*bitmap_in) & 0x40 ? 0xff : 0x00;
                        else if(i == 2) bitmap_out_tmp[x] = (*bitmap_in) & 0x20 ? 0xff : 0x00;
                        else if(i == 3) bitmap_out_tmp[x] = (*bitmap_in) & 0x10 ? 0xff : 0x00;
                        else if(i == 4) bitmap_out_tmp[x] = (*bitmap_in) & 0x08 ? 0xff : 0x00;
                        else if(i == 5) bitmap_out_tmp[x] = (*bitmap_in) & 0x04 ? 0xff : 0x00;
                        else if(i == 6) bitmap_out_tmp[x] = (*bitmap_in) & 0x02 ? 0xff : 0x00;
                        else if(i == 7) {
                            bitmap_out_tmp[x] = (*bitmap_in) & 0x01 ? 0xff : 0x00;
                            bitmap_in++;
                        }
                    }
                    bitmap_out_tmp += stride;
                }
            }
            else if(fdsc->bpp == 2) {
                for(y = 0; y < gdsc->box_h; y ++) {
                    for(x = 0; x < gdsc->box_w; x++, i++) {
                        i = i & 0x3;
                        if(i == 0) bitmap_out_tmp[x] = opa2_table[(*bitmap_in) >> 6];
                        else if(i == 1) bitmap_out_tmp[x] = opa2_table[((*bitmap_in) >> 4) & 0x3];
                        else if(i == 2) bitmap_out_tmp[x] = opa2_table[((*bitmap_in) >> 2) & 0x3];
                        else if(i == 3) {
                            bitmap_out_tmp[x] = opa2_table[((*bitmap_in) >> 0) & 0x3];
                            bitmap_in++;
                        }
                    }
                    bitmap_out_tmp += stride;
                }

            }
            else if(fdsc->bpp == 4) {
                for(y = 0; y < gdsc->box_h; y ++) {
                    for(x = 0; x < gdsc->box_w; x++, i++) {
                        i = i & 0x1;
                        if(i == 0) {
                            bitmap_out_tmp[x] = opa4_table[(*bitmap_in) >> 4];
                        }
                        else if(i == 1) {
                            bitmap_out_tmp[x] = opa4_table[(*bitmap_in) & 0xF];
                            bitmap_in++;
                        }
                    }
                    bitmap_out_tmp += stride;
                }
            }
            return draw_buf;
        }
    }
    return NULL;
}
```

待具体研究。。。