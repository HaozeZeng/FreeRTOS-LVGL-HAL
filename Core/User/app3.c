#include "app.h"
#include "string.h"
#include "malloc.h"
#include "lvgl.h"

#define scr_get_width() lv_obj_get_width(lv_scr_act())
#define scr_get_height() lv_obj_get_height(lv_scr_act())


FRESULT MenuInit(const char *path, lv_obj_t *submenu);

static lv_obj_t *MainMenu;

struct MYFILEINFO {
    char *path;
    char *name;
};

void test() {
//
//    lv_obj_t* obj = lv_btn_create(lv_scr_act());
//    lv_obj_set_size(obj, 300, 300);
//    lv_obj_center(obj);
//
//    lv_obj_t* label = lv_label_create(obj);
//    lv_obj_set_style_text_font(label, &myFont, 0);
//    lv_label_set_text(label, "外部字体测试");
//    lv_obj_center(label);


    char path[30] = "0:";
    MainMenu = lv_list_create(lv_scr_act());
    lv_obj_center(MainMenu);
    lv_obj_set_size(MainMenu, scr_get_width(), scr_get_height());
    MenuInit(path, MainMenu);
    //MenuInit(const char* path,lv_obj_t* submenu)


}

static void BtnDirEvent_Cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *tgt = lv_event_get_target(e);
    lv_obj_t *submenu = (lv_obj_t *) (e->user_data);
    if (code == LV_EVENT_CLICKED) {

        lv_obj_set_style_bg_color(tgt, lv_color_hex(0x47a7f1), LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(tgt, 150, LV_STATE_FOCUSED);
        lv_obj_clear_flag((lv_obj_t *) (e->user_data), LV_OBJ_FLAG_HIDDEN);
        //printf("Clicked %s\r\n", lv_list_get_btn_text(submenu,tgt));
    }

}
static void BtnExitEvent_Cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *tgt = lv_event_get_target(e);
    lv_obj_t *submenu = (lv_obj_t *) (e->user_data);
    if (code == LV_EVENT_CLICKED) {

        lv_obj_set_style_bg_color(tgt, lv_color_hex(0x47a7f1), LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(tgt, 150, LV_STATE_FOCUSED);
        lv_obj_add_flag((lv_obj_t *) (e->user_data), LV_OBJ_FLAG_HIDDEN);
        //printf("Clicked %s\r\n", lv_list_get_btn_text(submenu,tgt));
    }

}
static void BtnFileEvent_Cb(lv_event_t *e) {
    struct MYFILEINFO *info = (struct MYFILEINFO *) (e->user_data);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *tgt = lv_event_get_target(e);
    FIL file;
    FRESULT result;
    UINT bytes_read=0;
    char file_content[2048]="";
    char text[100];
    if (code == LV_EVENT_CLICKED) {
        lv_obj_set_style_bg_color(tgt, lv_color_hex(0x47a7f1), LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(tgt, 150, LV_STATE_FOCUSED);
        printf("info->path is %s\r\n", info->path);
        result = f_open(&file,info->path, FA_READ);
        if (result != FR_OK) {
            printf("open error: %d\r\n", result);
            return;
        }
        lv_obj_t *win = lv_win_create(lv_scr_act(), 40);
        lv_obj_center(win);
        lv_obj_set_size(win, scr_get_width(), scr_get_height());
        lv_win_add_title(win, info->name);
        lv_obj_t *btnexit = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 60);
        lv_obj_add_event_cb(btnexit, BtnExitEvent_Cb, LV_EVENT_CLICKED, win);
        lv_obj_t *cont = lv_win_get_content(win);  /*Content can be aded here*/

        lv_obj_t *temp = NULL;  // 确保 temp 在开始时为 NULL
        lv_obj_t *label = lv_label_create(cont);  // 在父对象 cont 中创建新标签
        lv_label_set_long_mode(label,LV_LABEL_LONG_WRAP);

        file_content[0] = '\0';  // 确保 file_content 在读取前是一个空字符串

        do {
            result = f_read(&file, text, sizeof(text) - 1, &bytes_read);  // 每次最多读取 99 个字节
            if (result != FR_OK) {
                printf("read error: %d\r\n", result);
                break;
            }

            text[bytes_read] = '\0';  // 确保字符串以 '\0' 结尾

            // 检查 file_content 中是否有足够的空间来追加 text
            if ((strlen(file_content) + bytes_read) < 2048) {
                strncat(file_content, text, bytes_read);  // 使用 strncat 防止溢出
            } else {
                printf("Buffer overflow: file content too large.\r\n");
                break;
            }

        } while (bytes_read > 0);
        lv_label_set_text(label,file_content);
        printf("file_content is %s\r\n",file_content);

//        win = (lv_obj_t *) mymalloc(SRAMIN, sizeof(lv_obj_t));
//        btnexit = (lv_obj_t *) mymalloc(SRAMIN, sizeof(lv_obj_t));
        printf("my_mem_perused is %d%%\r\n", my_mem_perused(SRAMIN));



        //printf("Clicked %s\r\n", );
    }

}
FRESULT MenuInit(const char *path, lv_obj_t *submenu) {
    FRESULT res;
    DIR dir;
    static FILINFO fno;
    char Filepath[128] = {0};
    char DirPath[128] = {0}; // 使用数组来存储目录路径
    res = f_opendir(&dir, path); // 打开目录，返回状态和目录对象的指针
    struct MYFILEINFO *myfileinfo;
    if (res == FR_OK) {
        if(strcmp(path,"0:")==0)
        {
            snprintf(DirPath, sizeof(DirPath), "%s/..", path);
        }
        else
        {
            snprintf(DirPath, sizeof(DirPath), "%s..", path);
        }
        lv_obj_t *exitbtn = lv_list_add_btn(submenu, LV_SYMBOL_DIRECTORY, DirPath);
        lv_obj_add_event_cb(exitbtn, BtnExitEvent_Cb, LV_EVENT_CLICKED, submenu);

        while (1) {
            res = f_readdir(&dir, &fno); // 读取目录，返回状态和文件信息的指针
            if (res != FR_OK || fno.fname[0] == 0) {
                printf("f_readdir end!\r\n");
                break; // 若打开失败或到结尾，则退出
            }

            if ((fno.fattrib & AM_DIR) && !(fno.fattrib & AM_SYS)) { // 是目录
                printf(" path:%s, File:%s\r\n", path, fno.fname);
                snprintf(Filepath, sizeof(Filepath), "%s/%s", path, fno.fname);
                printf(" DirPath:%s\r\n", Filepath);
                lv_obj_t *btn = lv_list_add_btn(submenu, LV_SYMBOL_DIRECTORY, fno.fname);
                lv_obj_t *SubMenu = lv_list_create(lv_scr_act());
                lv_obj_center(SubMenu);
                lv_obj_set_size(SubMenu, scr_get_width(), scr_get_height());
                lv_obj_add_flag(SubMenu, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_event_cb(btn, BtnDirEvent_Cb, LV_EVENT_CLICKED, SubMenu);

                res = MenuInit(Filepath, SubMenu);
                if (res != FR_OK) {
                    printf("MenuInit fail! res:%d", res); // 打开失败
                    break; // 退出循环
                }
            } else if (!(fno.fattrib & AM_SYS)) { // 是文件
                snprintf(Filepath, sizeof(Filepath), "%s/%s", path, fno.fname);
                printf(" File:%s/%s\r\n", path, fno.fname);
                printf("Filepath is %s\r\n",Filepath);
                lv_obj_t *btn = lv_list_add_btn(submenu, LV_SYMBOL_FILE, fno.fname);
                // 这里可以添加文件的事件回调
                myfileinfo = (struct MYFILEINFO *) mymalloc(SRAMIN, sizeof(struct MYFILEINFO));
                if (myfileinfo != NULL) {
                    // 复制字符串内容到 myfileinfo 中
                    myfileinfo->path = (char *) mymalloc(SRAMIN, strlen(Filepath) + 1);
                    myfileinfo->name = (char *) mymalloc(SRAMIN, strlen(fno.fname) + 1);

                    if (myfileinfo->path != NULL && myfileinfo->name != NULL) {
                        strcpy((char *) myfileinfo->path, Filepath);
                        strcpy((char *) myfileinfo->name, fno.fname);

                        // 传递指向 myfileinfo 的指针
                        lv_obj_add_event_cb(btn, BtnFileEvent_Cb, LV_EVENT_CLICKED, myfileinfo);
                    } else {
                        // 处理内存分配失败的情况
                        myfree(SRAMIN, myfileinfo);
                    }
                    //
                } else {
                    printf("MYFILEINFO malloc err\r\n");
                }
            }
        }
    } else {
        printf(" DIR OPEN FAIL \r\n");
    }
    f_closedir(&dir); // 关闭目录
    printf(" f_closedir end!\r\n");
    return res;
}



//FRESULT MenuInit(const char *path, lv_obj_t *submenu) {
//    FRESULT res;
//    DIR dir;
//    UINT i; //定义变量
//    static FILINFO fno;
//    char Filepath[128] = {0};
//    char *DirPath;
//    char *temp;
//    res = f_opendir(&dir, path); //打开目录，返回状态 和 目录对象的指针
//    if (res == FR_OK) {
//        strcpy(Filepath, path);
//        DirPath = strcat(Filepath, "/..");
//        lv_obj_t *exitbtn = (lv_obj_t *) mymalloc(SRAMIN, sizeof(lv_obj_t));
//        exitbtn = lv_list_add_btn(submenu, LV_SYMBOL_DIRECTORY, DirPath);
//        lv_obj_add_event_cb(exitbtn, BtnExitEvent_Cb, LV_EVENT_CLICKED, submenu);
//        memset(Filepath, 0, sizeof(Filepath) / sizeof(Filepath[0]));


//        while (1) {
//            res = f_readdir(&dir, &fno); //读取目录，返回状态 和 文件信息的指针
//            if (res != FR_OK || fno.fname[0] == 0)//为null空
//            {
//                printf("f_readdir end!\r\n");
//                break;        //若打开失败 或 到结尾，则退出
//            }
//            if ((fno.fattrib & AM_DIR) && !(fno.fattrib & AM_SYS)) //是目录
//            {
//                printf(" path:%s,i=%d,File:%s\r\n", path, i, fno.fname);
//                strcpy(Filepath, path);
//                if (strcmp("0:/", path) != 0)  // 区别根目录
//                    DirPath = strcat(Filepath, "/");
//                DirPath = strcat(Filepath, (const char *) fno.fname);
//                printf(" DirPath:%s\r\n", DirPath);
//                lv_obj_t *btn = (lv_obj_t *) mymalloc(SRAMIN, sizeof(lv_obj_t));
//                lv_obj_t *SubMenu = (lv_obj_t *) mymalloc(SRAMIN, sizeof(lv_obj_t));
//                btn = lv_list_add_btn(submenu, LV_SYMBOL_DIRECTORY, fno.fname);
//                SubMenu = lv_list_create(lv_scr_act());
//                lv_obj_center(SubMenu);
//                lv_obj_set_size(SubMenu, scr_get_width(), scr_get_height());
//                lv_obj_add_flag(SubMenu, LV_OBJ_FLAG_HIDDEN);
//                lv_obj_add_event_cb(btn, BtnDirEvent_Cb, LV_EVENT_CLICKED, SubMenu);


//                res = MenuInit(DirPath, SubMenu);
//                if (res != FR_OK) {
//                    printf("MenuInit fail! res:%d", res); //打开失败
//                    break; //退出循环
//                }
//            } else if (!(fno.fattrib & AM_SYS)) {

//                struct MYFILEINFO* myfileinfo = (struct MYFILEINFO *) mymalloc(SRAMIN, sizeof(struct MYFILEINFO));
//                printf("my_mem_perused is %d%%\r\n", my_mem_perused(SRAMIN));
//                printf(" File:%s/%s\r\n", path, fno.fname); //是文件
//                lv_obj_t *btn = (lv_obj_t *) mymalloc(SRAMIN, sizeof(lv_obj_t));
//                btn = lv_list_add_btn(submenu, LV_SYMBOL_FILE, fno.fname);
//                memset(Filepath, 0, sizeof(Filepath) / sizeof(Filepath[0]));
//                strcpy(Filepath, path);
//                strcat(Filepath, (const char *) fno.fname);
//                printf("DirPath is %s \r\n",Filepath);
//				strcpy((char*)myfileinfo->path, Filepath);
//				strcpy((char*)myfileinfo->name, fno.fname);
////                myfileinfo->path = (const char*)Filepath;
////                myfileinfo->name =(const char*) fno.fname;
//                lv_obj_add_event_cb(btn, BtnFileEvent_Cb, LV_EVENT_CLICKED, &myfileinfo);
//            }
//        }
//    } else {
//        printf(" DIR OPEN FAIL \r\n"); //是文件
//    }
//    f_closedir(&dir); //关闭目录
//    printf(" f_closedir end!\r\n");
//    return res;
//}