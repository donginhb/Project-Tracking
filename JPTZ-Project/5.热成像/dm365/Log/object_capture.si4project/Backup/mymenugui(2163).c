#include "mymenugui.h"
#include "../char_table.h"
#include "searchdir.h"
#define 	GREENPAN		1

static int  GREENPICFLAG      = 1;
static int  PICTURESELCETFLAG = 1;
static int 	VIDEOSELCETFLAG = 1;

static int  LinkCreateFlag    = 0;


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <pthread.h>



#define BASE_VIDIOC_PRIVATE	192		

#define VPFE_CMD_Capture_led_play _IOR('V', BASE_VIDIOC_PRIVATE + 3, void *)
#define VPFE_CMD_Capture_led_stop _IOR('V', BASE_VIDIOC_PRIVATE + 4, void *)

/*********************************************************************
    osd_print_Mypic(40,0xff,450,480,ptr,MYPHOTO);
	osd_print_Mypic(40,0xff,500,480,ptr,MYLUXIANG);
	osd_print_Mypic(40,0xff,550,480,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,600,480,ptr,MYBUCHANG);
	osd_print_Mypic(40,0xff,650,480,ptr,MYENTER);

	osd_print_Mypic(40,0,100,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,108,108,ptr,MYMOSHI);
	
	osd_print_Mypic(40,0,150,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,158,108,ptr,MYTUPIAN);
	
	osd_print_Mypic(40,0,200,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,208,108,ptr,MYSHIPIN);
	
	osd_print_Mypic(40,0,250,100,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,258,108,ptr,MYCAIDAN);

*********************************************************************/
void ShowPower(unsigned char *ptr){
	
	int batval = glUsartInfoSt.powerInfo;
	char batstr[8];
	sprintf(batstr, "%d%\r\n", batval);
	osd_print_string(batstr, 4, 624, 25,0xff,ptr,MYASCIIDOT);  //printf battery value width = 4*6=24
	osd_print_battery(658, 26, batval, ptr,BATTICON);

}
static void InitMenuHead(unsigned char*ptr){
	char timeShow[10];	
	if(gl_cam_time/60 < 10 && gl_cam_time%60 < 10)
		sprintf(timeShow,"0%d:0%d",gl_cam_time/60,gl_cam_time%60);
	else if(gl_cam_time/60 < 10 && gl_cam_time%60 >= 10)
		sprintf(timeShow,"0%d:%d",gl_cam_time/60,gl_cam_time%60);
	else if(gl_cam_time/60 >= 10 && gl_cam_time%60 < 10)
		sprintf(timeShow,"%d:0%d",gl_cam_time/60,gl_cam_time%60);
	else if(gl_cam_time/60 >= 10 && gl_cam_time%60 >= 10)
		sprintf(timeShow,"%d:%d",gl_cam_time/60,gl_cam_time%60);
	
	osd_print_Mypic(40,0xff,680,268,ptr,MYPHOTO);
	osd_print_Mypic(40,0xff,20,268,ptr,MYBUCHANG);
	if(gl_system_status == STATUS_REC){
		osd_print_Red_Point(350,40,ptr);
		osd_print_string(timeShow,5,365,38,0xff,ptr,MYASCIIDOT);
	}
}

static PT_MenuGuiSt DealMenuHead(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	
	if(glUsartInfoSt.LeftRoll || glUsartInfoSt.RightRoll){
		pt_retnode = ptr->ChildMenu1;	
		glUsartInfoSt.LeftRoll  = 0;
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftKeyInfo)
	{
		
	}
	if(glUsartInfoSt.RightKeyInfo == 1)
	{
		printf("----------picture get--------.\n");
		fflush(NULL);
		gl_get_picture = 1;
		gl_picture_osd = 50;
		printf("\r\nPHOTO command\r\n");

		glUsartInfoSt.RightKeyInfo = 0;
	}
	if(glUsartInfoSt.RightKeyInfo == 2)
	{
		if(gl_system_status == STATUS_STOP)
		{
			video_command = CMD_REC;
			printf("\r\nSTART REC command\r\n");	
			if(gl_capture_fd)
			ioctl(gl_capture_fd, VPFE_CMD_Capture_led_play);
		
		}else if(gl_system_status == STATUS_REC)
		{			
			video_command = CMD_STOP;							
			printf("\r\nSTOP REC command\r\n");
			if(gl_capture_fd)
				ioctl(gl_capture_fd, VPFE_CMD_Capture_led_stop);
		}
			glUsartInfoSt.RightKeyInfo = 0;
	}

	return pt_retnode;
}
/*

*/

static void InitMenuSec(unsigned char*ptr){

	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);
		
	if(GREENPICFLAG & 0x01)		
		osd_print_Mypic(40,GREENPAN,260,450,ptr,MYHEISEYUAN);			
	else
		osd_print_Mypic(40,0,260,450,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,268,458,ptr,MYMOSHI);

	if(GREENPICFLAG & 0x02)
		osd_print_Mypic(40,GREENPAN,310,450,ptr,MYHEISEYUAN);
	else
		osd_print_Mypic(40,0,310,450,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,318,458,ptr,MYTUPIAN);
	
	if(GREENPICFLAG & 0x04)
		osd_print_Mypic(40,GREENPAN,360,450,ptr,MYHEISEYUAN);
	else
		osd_print_Mypic(40,0,360,450,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,368,458,ptr,MYSHIPIN);
	
	if(GREENPICFLAG & 0x08)
		osd_print_Mypic(40,GREENPAN,410,450,ptr,MYHEISEYUAN);
	else
		osd_print_Mypic(40,0,410,450,ptr,MYHEISEYUAN);
	osd_print_Mypic(24,0xff,418,458,ptr,MYCAIDAN);
}

static PT_MenuGuiSt DealMenuSec(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;

	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
			GREENPICFLAG = GREENPICFLAG >> 1;
			if(GREENPICFLAG == 0){
				GREENPICFLAG = 8;
			}
		glUsartInfoSt.RightRoll = 0;
	}
	if(glUsartInfoSt.LeftRoll){
			GREENPICFLAG = GREENPICFLAG << 1;
			printf("GREENPICFLAG:%d\n",GREENPICFLAG);
			if(GREENPICFLAG > 8){
				GREENPICFLAG = 1;
			}
	    glUsartInfoSt.LeftRoll = 0;
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		
		if(GREENPICFLAG & 0x01){}
			pt_retnode = ptr->ChildMenu1;
		if(GREENPICFLAG & 0x02){
			pt_retnode = ptr->ChildMenu2;
		}
		if(GREENPICFLAG & 0x04){
			pt_retnode = ptr->ChildMenu3;
		}
		if(GREENPICFLAG & 0x08){
			pt_retnode = ptr->ChildMenu4;
		}
		glUsartInfoSt.RightKeyInfo = 0;
	}
	return pt_retnode;
}

static void InitMenuThree_MoShi(unsigned char*ptr){
	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);
	
}

static PT_MenuGuiSt DealMenuThree_MoShi(PT_MenuGuiSt ptr){

	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		
	}
	if(glUsartInfoSt.LeftRoll){
		
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		
	}
	return pt_retnode;
}

static int 	gl_num = 0;
static void CreatTuPianShow(int num,unsigned char*ptr,LLIST *head,int offset,int type){
	int i = 0;
	PT_MyFileSt			  data = NULL;
	struct llist_node_st *tmp = head->head.next;
	
	//struct llist_node_st *test = gl_MyBmpLList->head.next;
	for(i = 0;i < offset && tmp != &head->head;i++){
			tmp = tmp->next;
	}
	for(i = 0;i < num ;i++){
		data = tmp->data;
	//	printf("data->name:%s.\n",data->name);
		if(i == (type-1)){
				osd_print_rect(200+80*i+(4-num)*80,450,GREENPAN,ptr);
				ShowImageNode = tmp;
			}
		else
			 osd_print_rect(200+80*i+(4-num)*80,450,0,ptr);
		osd_print_string(data->name,7,202+80*i+(4-num)*80,453,0xff,ptr,MYASCIIDOT);	
		
		tmp = tmp->next; 
	}
}	

static int gl_maxfilecnt;
static void InitMenuThree_TuPian(unsigned char*ptr){
	int maxfilecnt;
	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);	
	
	if(LinkCreateFlag == 0){
		gl_MyBmpLList = mySearchDirFun("/home/root/media/",".bmp");
		if(gl_MyBmpLList == NULL){
			printf("Search Dir error.\n");
			exit(1);
		}
		gl_MyBmpLList->getMemSum(gl_MyBmpLList,&gl_maxfilecnt);
		
		LinkCreateFlag = 1;	
	}
	if(gl_maxfilecnt == 0){
		osd_print_rect(550,490,0,ptr);
		osd_print_string("NO FILE",7,555,492,GREENPAN,ptr,MYASCIIDOT);
		return ;
	}

	if(gl_maxfilecnt < 4){
		CreatTuPianShow(gl_maxfilecnt,ptr,gl_MyBmpLList,gl_num,PICTURESELCETFLAG);
	}else{
		CreatTuPianShow(4,ptr,gl_MyBmpLList,gl_num,PICTURESELCETFLAG);
	}
	
	printf("gl_maxfilecnt:%d.\n",gl_maxfilecnt);
}

static PT_MenuGuiSt DealMenuThree_TuPian(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		PICTURESELCETFLAG -= 1;
		if(PICTURESELCETFLAG < 1){
				PICTURESELCETFLAG = 1;
				if(gl_num > 0)gl_num--;
				gl_num = 0;		
		}
		glUsartInfoSt.RightRoll = 0;
	}
	
	if(glUsartInfoSt.LeftRoll){
		PICTURESELCETFLAG += 1;
		if(gl_maxfilecnt > 4){
			if(PICTURESELCETFLAG > 4){
				PICTURESELCETFLAG = 4;
				if(gl_num != (gl_maxfilecnt - 4)) 
						gl_num++;
			}
		}else{
			if(PICTURESELCETFLAG > gl_maxfilecnt){
				PICTURESELCETFLAG = gl_maxfilecnt;
			}
		}
		glUsartInfoSt.LeftRoll = 0;
	}
	
	if(glUsartInfoSt.LeftKeyInfo == 1){
		llist_destroy(gl_MyBmpLList);
		PICTURESELCETFLAG = 1;
		LinkCreateFlag  = 0;
		pt_retnode = ptr->FarthMenu;
		glUsartInfoSt.LeftKeyInfo = 0;
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		gl_ShowPicFlag = 1;		
		glUsartInfoSt.RightKeyInfo = 0;	
	}
	return pt_retnode;
}

static int maxvideocnt;
static int gl_videonum = 0;

static void InitMenuThree_Shipin(unsigned char*ptr){
	
		osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
		osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);	
		//PICTURESELCETFLAG
		
		if(LinkCreateFlag == 0){
			gl_MyH264LList = mySearchDirFun("/home/root/media/",".264");
			if(gl_MyH264LList == NULL){
				printf("Search Dir error.\n");
				exit(1);
			}
			gl_MyH264LList->getMemSum(gl_MyH264LList,&maxvideocnt);
			LinkCreateFlag = 1; 
		}
		if(maxvideocnt == 0){
				osd_print_rect(550,490,0,ptr);
				osd_print_string("NO FILE",7,555,492,GREENPAN,ptr,MYASCIIDOT);
				return ;
		}
		if(maxvideocnt < 4){
			CreatTuPianShow(maxvideocnt,ptr,gl_MyH264LList,gl_videonum,VIDEOSELCETFLAG);
		}else{
			CreatTuPianShow(4,ptr,gl_MyH264LList,gl_videonum,VIDEOSELCETFLAG);
		}
		
}

static PT_MenuGuiSt DealMenuThree_Shipin(PT_MenuGuiSt ptr){
	PT_MenuGuiSt pt_retnode = NULL;
		//if(glUsartInfoSt.RightKeyInfo)
		//if(glUsartInfoSt.LeftKeyInfo)
		if(glUsartInfoSt.RightRoll){
			VIDEOSELCETFLAG -= 1;
			if(VIDEOSELCETFLAG < 1){
					VIDEOSELCETFLAG = 1;
					if(gl_videonum > 0)gl_videonum--;
					gl_videonum = 0; 	
			}
			glUsartInfoSt.RightRoll = 0;
		}
		
		if(glUsartInfoSt.LeftRoll){
			VIDEOSELCETFLAG += 1;
			if(maxvideocnt > 4){
				if(VIDEOSELCETFLAG > 4){
					VIDEOSELCETFLAG = 4;
					if(gl_videonum != (maxvideocnt - 4)) 
							gl_videonum++;
				}
			}else{
				if(VIDEOSELCETFLAG > maxvideocnt){
					VIDEOSELCETFLAG = maxvideocnt;
				}
			}
			glUsartInfoSt.LeftRoll = 0;
		}
		
		if(glUsartInfoSt.LeftKeyInfo == 1){
			llist_destroy(gl_MyH264LList);
			VIDEOSELCETFLAG = 1;
			LinkCreateFlag  = 0;
			pt_retnode = ptr->FarthMenu;
			glUsartInfoSt.LeftKeyInfo = 0;
		}
	
		if(glUsartInfoSt.RightKeyInfo == 1){
			gl_ShowVideoFlag = 1;		
			glUsartInfoSt.RightKeyInfo = 0;	
		}
		return pt_retnode;
}

static void InitMenuThree_CaiDan(unsigned char*ptr){	
	osd_print_Mypic(40,0xff,20,268,ptr,MYRETURN);
	osd_print_Mypic(40,0xff,680,268,ptr,MYENTER);
	osd_print_rect(478,490,ptr);

}

static PT_MenuGuiSt DealMenuThree_CaiDan(PT_MenuGuiSt ptr){

	PT_MenuGuiSt pt_retnode = NULL;
	//if(glUsartInfoSt.RightKeyInfo)
	//if(glUsartInfoSt.LeftKeyInfo)
	if(glUsartInfoSt.RightRoll){
		
	}
	if(glUsartInfoSt.LeftRoll){
		;
	}

	if(glUsartInfoSt.LeftKeyInfo == 1){
	
	}

	if(glUsartInfoSt.RightKeyInfo == 1){
		
	}
	return pt_retnode;
}

PT_MenuGuiSt MyCreateMenuHead(void){
	PT_MenuGuiSt MenuHead = NULL;
	MenuHead = malloc(sizeof(struct myMenuGui_St));
	if(MenuHead == NULL){
		printf("malloc MenuHead error.\n");
		return NULL;
	}

	strcpy(MenuHead->name,"MenuHead");
	MenuHead->count 	 = 1;
	MenuHead->InitMenu   = InitMenuHead;
	MenuHead->DealMenu   = DealMenuHead;
	MenuHead->FarthMenu  = NULL;
	MenuHead->ChildMenu1 = NULL;
	MenuHead->ChildMenu3 = NULL;
	MenuHead->ChildMenu2 = NULL;
	MenuHead->ChildMenu4 = NULL;
	return MenuHead;
}

static void InsertChildMenuHead(PT_MenuGuiSt FarthMenu,int num,MYINITFUN initfun,MYDEALFUN dealfun){
	PT_MenuGuiSt MenuChild = NULL;
	
	char str[20];
	MenuChild = malloc(sizeof(struct myMenuGui_St));
	if(MenuChild == NULL){
			printf("malloc MenuHead error.\n");
			return NULL;
	}

	sprintf(str,"Menu%d",num);
	strcpy(MenuChild->name,str);
	MenuChild->count 	  = 1;
	MenuChild->InitMenu   = initfun;
	MenuChild->DealMenu   = dealfun;
	MenuChild->FarthMenu  = FarthMenu;
	MenuChild->ChildMenu1 = NULL;
	MenuChild->ChildMenu2 = NULL;
	MenuChild->ChildMenu3 = NULL;
	MenuChild->ChildMenu4 = NULL;
	if(num == 1)
		FarthMenu->ChildMenu1 = MenuChild;
	if(num == 2)
		FarthMenu->ChildMenu2 = MenuChild;
	if(num == 3)
		FarthMenu->ChildMenu3 = MenuChild;
	if(num == 4)
		FarthMenu->ChildMenu4 = MenuChild;
	
}

void CreateSecMenu(PT_MenuGuiSt Head){
	InsertChildMenuHead(Head,1,InitMenuSec,DealMenuSec);
	InsertChildMenuHead(Head->ChildMenu1,1,InitMenuThree_MoShi ,DealMenuThree_MoShi);
	InsertChildMenuHead(Head->ChildMenu1,2,InitMenuThree_TuPian,DealMenuThree_TuPian);
	InsertChildMenuHead(Head->ChildMenu1,3,InitMenuThree_Shipin,DealMenuThree_Shipin);
	InsertChildMenuHead(Head->ChildMenu1,4,InitMenuThree_CaiDan,DealMenuThree_CaiDan);
}




