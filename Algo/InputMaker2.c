/*
* 실행할때는 ./InputMaeker (만드려는 사람수) (이용시간 초단위) (건물의 높이) (올라가는사람비율) (1층에서 출발하거나 1층으로 도착하는 사람의 비율)
* 이렇게 입력을 해서 실행합니다.
*
* 출력순서 : (탑승시간) (현재층) (목적층) 
* ex) ./InputMaker 100 180 10 50 100
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <limits.h>
#include <math.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "/usr/include/mysql/mysql.h"
#include <time.h>

#define ASCII 48
#define MAX 100000

void DB_testcase(int time, int start, int dest);
void DB_delete();

/**/
typedef struct _TESTCASE
{
	int start_time;
	int current_floor;
	int go_Floor;

} Test_struct;

int compare_with_size(const void *a, const void  *b)
{
    Test_struct* ptr_a = (Test_struct*)a;
    Test_struct* ptr_b = (Test_struct*)b;
    
    if (ptr_a->start_time < ptr_b->start_time) return -1;
    else if (ptr_a->start_time == ptr_b->start_time) return 0;
    else return 1;
}

int inputNum;
int testCase; //-ASCII;
int timeInterval;
int Building_Floor;
int upRate;
int oneRate;
	
int start_time, current_floor, go_Floor, re_start_time, re_go_Floor; 

	
Test_struct test[MAX];

FILE *f;
int upNum; 
int oneNum;

void makeTest();
void makeOnlyOneTest();

int main(int argc, char **argv)
{
	char *result = malloc(sizeof(char)*10);


	DB_delete();
	inputNum = argc -1;
	testCase = atoi(argv[1]); //-ASCII;
	timeInterval = atoi(argv[2]);
	Building_Floor = atoi(argv[3]);
	upRate = atoi(argv[4]);
	oneRate = atoi(argv[5]);
	
	f=fopen("testCase2.txt" , "w");
	srand(time(NULL));
	//printf("@@@@@@@@@@@@@@@@@\n");
	if(upRate == 0){
		upNum = 0;
	}
	else{
		upNum= testCase * (upRate)/100;
	}
/////////////////////////////////////////////////////////////////////////////
	if(oneRate == 100){
		makeOnlyOneTest();
	}
	else{
		oneNum = (Building_Floor * oneRate) / (100 - oneRate);	
		//printf(" oneNum : %d\n",oneNum);
		makeTest();
	}

	qsort(test, testCase, sizeof(Test_struct), compare_with_size);

	for(int i = 0 ;i < testCase ; i++){
		//sprintf(result,"%d %d %d\n",test[i].start_time, test[i].current_floor, test[i].go_Floor );
		//fprintf(f,"%s",result);
		DB_testcase(test[i].start_time, test[i].current_floor, test[i].go_Floor);
	}
	fclose(f);

}

void makeTest(){

	for(int i = 0 ;i < testCase ; i++){
				
		test[i].start_time = rand()%timeInterval;

		if(i < upNum){//올라가는 비율만큼 만든다.
			test[i].current_floor = rand()%(Building_Floor+oneNum);

			if(test[i].current_floor >= Building_Floor){
				test[i].current_floor =1;
			}

			while(1){
				if(test[i].current_floor == 0){
					test[i].current_floor = rand()%Building_Floor;
				}
				else break;

			}
			test[i].go_Floor = rand()%(Building_Floor- test[i].current_floor) + test[i].current_floor +1;
		}
		else{
			test[i].go_Floor = rand()%(Building_Floor+oneNum);

			if(test[i].go_Floor >= Building_Floor){
				test[i].go_Floor =1;
			}

			while(1){
				if(test[i].go_Floor == 0){
					test[i].go_Floor = rand()%Building_Floor;
				}
				else break;

			}
			test[i].current_floor = rand()%(Building_Floor- test[i].go_Floor) + test[i].go_Floor +1;
		}
	}
}

void makeOnlyOneTest(){
	//printf("@@@@@@@@@\n");
	for(int i = 0 ;i < testCase ; i++){
		test[i].start_time = rand()%timeInterval;

		if(i < upNum){//올라가는 비율만큼 만든다.
			test[i].current_floor = 1;
			test[i].go_Floor = rand()%(Building_Floor- test[i].current_floor) + test[i].current_floor +1;
		}
		else{

			test[i].current_floor = rand()%Building_Floor +1;

			while(1){
				if(test[i].current_floor == 1 || test[i].current_floor == 0){
					test[i].current_floor = rand()%Building_Floor;
			
				}
				else break;
			}

			test[i].go_Floor = 1;

		}
	}
}

void DB_testcase(int time, int start, int dest){

	MYSQL *conn;
 	MYSQL_RES *res;
 	MYSQL_ROW row;

 	char *server = "localhost";
 	char *user = "root";
 	char *password = "root";
 	char *database = "capstone";

 	int tmp;

 	conn = (MYSQL *)malloc(sizeof(MYSQL )*1);
 	res = (MYSQL_RES *)malloc(sizeof(MYSQL_RES )*4);
 	row = (MYSQL_ROW )malloc(sizeof(MYSQL_ROW )*5);

	conn = mysql_init(NULL);

 	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
 		exit(1);
  	}

  	 if(mysql_query(conn,"show tables")){

   		exit(1);
   	}

	res = mysql_use_result(conn);
  	////printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){
		//printf("%s ",row[0]);
  	}


	char sql[100] = "";
	sprintf( sql,"INSERT INTO testcase VALUES(%d,%d,%d);",time, start, dest);

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	//printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}


void DB_delete(){

	MYSQL *conn;
 	MYSQL_RES *res;
 	MYSQL_ROW row;

 	char *server = "localhost";
 	char *user = "root";
 	char *password = "root";
 	char *database = "capstone";

 	int tmp;

 	conn = (MYSQL *)malloc(sizeof(MYSQL )*1);
 	res = (MYSQL_RES *)malloc(sizeof(MYSQL_RES )*4);
 	row = (MYSQL_ROW )malloc(sizeof(MYSQL_ROW )*5);

	conn = mysql_init(NULL);

 	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
 		exit(1);
  	}

  	 if(mysql_query(conn,"show tables")){

   		exit(1);
   	}

	res = mysql_use_result(conn);
  	////printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){
		//printf("%s ",row[0]);
  	}


	char sql[100] = "";
	sprintf( sql,"DELETE FROM testcase ;");

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	//printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}
