#include "app.h"
#include "string.h"
#include "malloc.h"
#include "lvgl.h"
#include "ff.h"


#define scr_get_width() lv_obj_get_width(lv_scr_act())
#define scr_get_height() lv_obj_get_height(lv_scr_act())
LV_FONT_DECLARE(SanJiBang16)

FRESULT MenuInit(const char *path, lv_obj_t *submenu);

static lv_obj_t *MainMenu;

struct MYFILEINFO {
    char *path;
    char *name;
};
static lv_style_t font_style;

void test() {
//    FRESULT result;
//    FIL file;
//    char file_content[2048];
//    char text[100];
//    UINT bytes_read;
    lv_style_init(&font_style);
    lv_style_set_text_font(&font_style, &SanJiBang16);
//    lv_style_set_text_align(&font_style, LV_TEXT_ALIGN_CENTER);

    /*Create a label with the new style*/
//    lv_obj_t *label = lv_label_create(lv_screen_active());
//    lv_obj_add_style(label, &font_style, 0);
//    lv_label_set_text(label, "你好");
//    lv_obj_center(label);



//    result = f_open(&file, "0:/EBOOK/hello.txt", FA_READ);
//    if (result != FR_OK) {
//        printf("open error: %d\r\n", result);
//        return;
//    }
//    lv_obj_t *win = lv_win_create(lv_scr_act());
//    lv_obj_center(win);
//    lv_obj_set_size(win, scr_get_width(), scr_get_height());
//    lv_win_add_title(win, "test1");
//    lv_obj_t *btnexit = lv_win_add_button(win, LV_SYMBOL_CLOSE, 60);
//    lv_obj_t *cont = lv_win_get_content(win);  /*Content can be aded here*/
//    lv_obj_t *temp = NULL;  // ȷ�� temp �ڿ�ʼʱΪ NULL
//    lv_obj_t *label = lv_label_create(cont);  // �ڸ����� cont �д����±�ǩ
//    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
//    file_content[0] = '\0';  // ȷ�� file_content �ڶ�ȡǰ��һ�����ַ���
//
//    do {
//        result = f_read(&file, text, sizeof(text) - 1, &bytes_read);  // ÿ������ȡ 99 ���ֽ�
//        if (result != FR_OK) {
//            printf("read error: %d\r\n", result);
//            break;
//        }
//
//        text[bytes_read] = '\0';  // ȷ���ַ����� '\0' ��β
//
//        // ��� file_content ���Ƿ����㹻�Ŀռ���׷�� text
//        if ((strlen(file_content) + bytes_read) < 2048) {
//            strncat(file_content, text, bytes_read);  // ʹ�� strncat ��ֹ���
//        } else {
//            printf("Buffer overflow: file content too large.\r\n");
//            break;
//        }
//
//    } while (bytes_read > 0);
//    lv_obj_add_style(label, &font_style, 0);
//    lv_label_set_text(label, file_content);



///*使用字体*/
///*如果不再需要字体，释放字体*/
    char path[30] = "0:";
    //  lv_style_set_text_font(&label_style, &yahei16);
    MainMenu = lv_list_create(lv_scr_act());
    lv_obj_center(MainMenu);
    lv_obj_set_size(MainMenu, scr_get_width(), scr_get_height());
    MenuInit(path, MainMenu);
    //MenuInit(const char* path,lv_obj_t* submenu)


}

static void BtnDirEvent_Cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *tgt = lv_event_get_target(e);
    lv_obj_t *submenu = (lv_obj_t *) (lv_event_get_user_data(e));
    if (code == LV_EVENT_CLICKED) {

        lv_obj_set_style_bg_color(tgt, lv_color_hex(0x47a7f1), LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(tgt, 150, LV_STATE_FOCUSED);
        lv_obj_clear_flag((lv_obj_t *) (lv_event_get_user_data(e)), LV_OBJ_FLAG_HIDDEN);
        //printf("Clicked %s\r\n", lv_list_get_btn_text(submenu,tgt));
    }

}

static void BtnExitEvent_Cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *tgt = lv_event_get_target(e);
    lv_obj_t *submenu = (lv_obj_t *) (lv_event_get_user_data(e));
    if (code == LV_EVENT_CLICKED) {

        lv_obj_set_style_bg_color(tgt, lv_color_hex(0x47a7f1), LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(tgt, 150, LV_STATE_FOCUSED);
        lv_obj_add_flag((lv_obj_t *) (lv_event_get_user_data(e)), LV_OBJ_FLAG_HIDDEN);
        //printf("Clicked %s\r\n", lv_list_get_btn_text(submenu,tgt));
    }

}

static void BtnFileEvent_Cb(lv_event_t *e) {
    struct MYFILEINFO *info = (struct MYFILEINFO *) (lv_event_get_user_data(e));
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *tgt = lv_event_get_target(e);
    FIL file;
    FRESULT result;
    UINT bytes_read = 0;
    char file_content[2048] = "";
    char text[100];


    if (code == LV_EVENT_CLICKED) {
        lv_obj_set_style_bg_color(tgt, lv_color_hex(0x47a7f1), LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(tgt, 150, LV_STATE_FOCUSED);
        printf("info->path is %s\r\n", info->path);
        result = f_open(&file, info->path, FA_READ);
        if (result != FR_OK) {
            printf("open error: %d\r\n", result);
            return;
        }
        lv_obj_t *win = lv_win_create(lv_scr_act());
        lv_obj_center(win);
        lv_obj_set_size(win, scr_get_width(), scr_get_height());
        lv_obj_t *title =lv_win_add_title(win, info->name);
        lv_obj_add_style(title, &font_style, 0);

        lv_obj_t *btnexit = lv_win_add_button(win, LV_SYMBOL_CLOSE, 60);
        lv_obj_add_event_cb(btnexit, BtnExitEvent_Cb, LV_EVENT_CLICKED, win);
        lv_obj_t *cont = lv_win_get_content(win);  /*Content can be aded here*/

        lv_obj_t *temp = NULL;  // ȷ�� temp �ڿ�ʼʱΪ NULL
        lv_obj_t *label = lv_label_create(cont);  // �ڸ����� cont �д����±�ǩ
        lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);

        file_content[0] = '\0';  // ȷ�� file_content �ڶ�ȡǰ��һ�����ַ���

        do {
            result = f_read(&file, text, sizeof(text) - 1, &bytes_read);  // ÿ������ȡ 99 ���ֽ�
            if (result != FR_OK) {
                printf("read error: %d\r\n", result);
                break;
            }

            text[bytes_read] = '\0';  // ȷ���ַ����� '\0' ��β

            // ��� file_content ���Ƿ����㹻�Ŀռ���׷�� text
            if ((strlen(file_content) + bytes_read) < 2048) {
                strncat(file_content, text, bytes_read);  // ʹ�� strncat ��ֹ���
            } else {
                printf("Buffer overflow: file content too large.\r\n");
                break;
            }

        } while (bytes_read > 0);
        lv_obj_add_style(label, &font_style, 0);
        lv_label_set_text(label, file_content);
        printf("file_content is %s\r\n", file_content);

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
    char DirPath[128] = {0}; // ʹ���������洢Ŀ¼·��
    res = f_opendir(&dir, path); // ��Ŀ¼������״̬��Ŀ¼�����ָ��
    struct MYFILEINFO *myfileinfo;
    if (res == FR_OK) {
        if (strcmp(path, "0:") == 0) {
            snprintf(DirPath, sizeof(DirPath), "%s/..", path);
        } else {
            snprintf(DirPath, sizeof(DirPath), "%s..", path);
        }
        lv_obj_t *exitbtn = lv_list_add_btn(submenu, LV_SYMBOL_DIRECTORY, NULL);
        lv_obj_t *label = lv_label_create(exitbtn);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
//                lv_obj_set_style_text_font(label, &yahei16, 0);
        lv_label_set_text(label, DirPath);
        lv_obj_add_style(label, &font_style, 0);
        lv_obj_add_event_cb(exitbtn, BtnExitEvent_Cb, LV_EVENT_CLICKED, submenu);

        while (1) {
            res = f_readdir(&dir, &fno); // ��ȡĿ¼������״̬���ļ���Ϣ��ָ��
            if (res != FR_OK || fno.fname[0] == 0) {
                printf("f_readdir end!\r\n");
                break; // ����ʧ�ܻ򵽽�β�����˳�
            }

            if ((fno.fattrib & AM_DIR) && !(fno.fattrib & AM_SYS)) { // ��Ŀ¼
                printf(" path:%s, File:%s\r\n", path, fno.fname);
                snprintf(Filepath, sizeof(Filepath), "%s/%s", path, fno.fname);
                printf(" DirPath:%s\r\n", Filepath);
                lv_obj_t *btn = lv_list_add_btn(submenu, LV_SYMBOL_DIRECTORY, NULL);
                lv_obj_t *label = lv_label_create(btn);
                lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
//                lv_obj_set_style_text_font(label, &yahei16, 0);
                lv_label_set_text(label, fno.fname);
                lv_obj_add_style(label, &font_style, 0);

                lv_obj_t *SubMenu = lv_list_create(lv_scr_act());
                lv_obj_center(SubMenu);
                lv_obj_set_size(SubMenu, scr_get_width(), scr_get_height());
                lv_obj_add_flag(SubMenu, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_event_cb(btn, BtnDirEvent_Cb, LV_EVENT_CLICKED, SubMenu);

                res = MenuInit(Filepath, SubMenu);
                if (res != FR_OK) {
                    printf("MenuInit fail! res:%d", res); // ��ʧ��
                    break; // �˳�ѭ��
                }
            } else if (!(fno.fattrib & AM_SYS)) { // ���ļ�
                snprintf(Filepath, sizeof(Filepath), "%s/%s", path, fno.fname);
                printf(" File:%s/%s\r\n", path, fno.fname);
                printf("Filepath is %s\r\n", Filepath);
                lv_obj_t *btn = lv_list_add_btn(submenu, LV_SYMBOL_FILE, NULL);
                lv_obj_t *label = lv_label_create(btn);
                lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
//                lv_obj_set_style_text_font(label, &yahei16, 0);
                lv_label_set_text(label, fno.fname);
                lv_obj_add_style(label, &font_style, 0);
//                lv_obj_t * label = lv_label_create(btn);
//                lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
//                lv_obj_set_style_text_font(label, &yahei16, 0);
//                lv_label_set_text(label, fno.fname);


                // �����������ļ����¼��ص�
                myfileinfo = (struct MYFILEINFO *) mymalloc(SRAMIN, sizeof(struct MYFILEINFO));
                if (myfileinfo != NULL) {
                    // �����ַ������ݵ� myfileinfo ��
                    myfileinfo->path = (char *) mymalloc(SRAMIN, strlen(Filepath) + 1);
                    myfileinfo->name = (char *) mymalloc(SRAMIN, strlen(fno.fname) + 1);

                    if (myfileinfo->path != NULL && myfileinfo->name != NULL) {
                        strcpy((char *) myfileinfo->path, Filepath);
                        strcpy((char *) myfileinfo->name, fno.fname);

                        // ����ָ�� myfileinfo ��ָ��
                        lv_obj_add_event_cb(btn, BtnFileEvent_Cb, LV_EVENT_CLICKED, myfileinfo);
                    } else {
                        // �����ڴ����ʧ�ܵ����
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
    f_closedir(&dir); // �ر�Ŀ¼
    printf(" f_closedir end!\r\n");
    return res;
}



//FRESULT MenuInit(const char *path, lv_obj_t *submenu) {
//    FRESULT res;
//    DIR dir;
//    UINT i; //�������
//    static FILINFO fno;
//    char Filepath[128] = {0};
//    char *DirPath;
//    char *temp;
//    res = f_opendir(&dir, path); //��Ŀ¼������״̬ �� Ŀ¼�����ָ��
//    if (res == FR_OK) {
//        strcpy(Filepath, path);
//        DirPath = strcat(Filepath, "/..");
//        lv_obj_t *exitbtn = (lv_obj_t *) mymalloc(SRAMIN, sizeof(lv_obj_t));
//        exitbtn = lv_list_add_btn(submenu, LV_SYMBOL_DIRECTORY, DirPath);
//        lv_obj_add_event_cb(exitbtn, BtnExitEvent_Cb, LV_EVENT_CLICKED, submenu);
//        memset(Filepath, 0, sizeof(Filepath) / sizeof(Filepath[0]));


//        while (1) {
//            res = f_readdir(&dir, &fno); //��ȡĿ¼������״̬ �� �ļ���Ϣ��ָ��
//            if (res != FR_OK || fno.fname[0] == 0)//Ϊnull��
//            {
//                printf("f_readdir end!\r\n");
//                break;        //����ʧ�� �� ����β�����˳�
//            }
//            if ((fno.fattrib & AM_DIR) && !(fno.fattrib & AM_SYS)) //��Ŀ¼
//            {
//                printf(" path:%s,i=%d,File:%s\r\n", path, i, fno.fname);
//                strcpy(Filepath, path);
//                if (strcmp("0:/", path) != 0)  // �����Ŀ¼
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
//                    printf("MenuInit fail! res:%d", res); //��ʧ��
//                    break; //�˳�ѭ��
//                }
//            } else if (!(fno.fattrib & AM_SYS)) {

//                struct MYFILEINFO* myfileinfo = (struct MYFILEINFO *) mymalloc(SRAMIN, sizeof(struct MYFILEINFO));
//                printf("my_mem_perused is %d%%\r\n", my_mem_perused(SRAMIN));
//                printf(" File:%s/%s\r\n", path, fno.fname); //���ļ�
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
//        printf(" DIR OPEN FAIL \r\n"); //���ļ�
//    }
//    f_closedir(&dir); //�ر�Ŀ¼
//    printf(" f_closedir end!\r\n");
//    return res;
//}