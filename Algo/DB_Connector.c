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

typedef struct _REQUEST
{
    int start_floor; //현재층(요청이 이루어지는)
    int dest_floor;  //목적층
    int num_people;  //몇 명이 타는지
} Request;

typedef struct _FLOORNODE
{
    struct _FLOORNODE *prev;
    struct _FLOORNODE *next;
    int floor;  // -1 : 점검
    int people; //+ 태운다, - 내린다
} F_node;

typedef struct _REQUESTNODE
{
    struct _REQUESTNODE *prev;
    struct _REQUESTNODE *next;
    Request req;
} R_node;

typedef struct _FLOORLIST
{
    F_node *head;
    F_node *tail;
} F_list;
// 해당엘리베이터에 맞는 연결 리스트를 만든다. 이 리스트를 통해서 최고 마지막 도착지를 알 수 있다.

typedef struct _REQUESTLIST
{
    R_node *head;
    R_node *tail;
} R_list;

/* 엘리베이터 구조체 */
typedef struct _ELEVATOR
{
    int current_floor;
    int next_dest;
    int current_people;
    int total_people;
    int fix;
    int fix_time;
    F_list pending;
} Elevator;

typedef struct _INPUT
{
    char *mode;
    int *req_elevator_id;
    int *req_current_floor;
    int *req_dest_floor;
    int *req_num_people;
} Input;

typedef struct _SIMUL
{
    Elevator **elevators;
    Input *input;
} Simul; // 시뮬레이션 구조체

typedef struct _DATABASE
{
	int *USER_ID;
	int *start_floor;
	int *dest_floor;
	char **time;
	int *Building_Id;
	int *Elevator_id;
	int flag;

} Database;

void DB_People_Num_updater(int building_id, int Elevator_Id, int people_num);
void DB_Elevator_updater(int building_id, int Elevator_Id, int current_floor);
void DB_Calling_updater(char *userID, int Time, int elevator_id);
void DB_Flag_updater(char *userID);
void DB_Flag2_updater(char *userID);
int DBconector_floor(int id);
int DBconector_ele_num(int id);
int DBconector_flag();

int DBconector_ele_num(int id){
	//나중에 사용할껀데 일단은 로컬에서 테스트 할꺼니깐 주석처리

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
  	//printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL)
  		printf("%s ",row[0]);


  	if(mysql_query(conn,"SELECT * FROM building"))
  	{
  	        return 1;
  	}

  	res = mysql_use_result(conn);

   	//printf("Returning List of Names : \n");
   	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s %s %s %s \n",row[0],row[1],row[2],row[3]);
		tmp = atoi(row[3]);
	}


   mysql_free_result(res);
   mysql_close(conn);

   return tmp;
}

void DB_Elevator_updater(int building_id, int Elevator_Id, int current_floor){

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
	//sprintf( sql,"UPDATE getCurr SET Current_Floor = %d where Elevator_Id = %d AND Building_Id = %d;",current_floor, Elevator_Id, building_id);
	sprintf( sql,"UPDATE getCurr SET Current_Floor = %d where Elevator_Id = %d ;",current_floor, Elevator_Id);

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	//printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}

void DB_People_Num_updater(int building_id, int Elevator_Id, int people_num){

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
	//sprintf( sql,"UPDATE getCurr SET Current_Floor = %d where Elevator_Id = %d AND Building_Id = %d;",current_floor, Elevator_Id, building_id);
	sprintf( sql,"UPDATE getCurr SET People_num = %d where Elevator_Id = %d ANC building_id = %d ;",people_num, Elevator_Id,building_id);

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	//printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}

void DB_Calling_updater(char *userID, int Time, int elevator_id){

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
  	//printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s ",row[0]);
  	}


	char sql[100] = "";
	sprintf( sql,"UPDATE Calling SET Time = '%d', Elevator_Id = '%d' where userID = '%s';",Time, elevator_id, userID);

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}

void DB_Flag_updater(char *userID){

	printf("#1_DB_Flag_updater\n");
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
		printf("%s ",row[0]);
  	}


	char sql[100] = "";
	sprintf( sql,"UPDATE Calling SET Flag = '1' where userID = '%s';",userID);

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	//printf("########%s\n",sql);
  	DB_Flag2_updater(userID);

   mysql_free_result(res);
   mysql_close(conn);

}

void DB_Flag2_updater(char *userID){

	printf("#1_DB_Flag_updater\n");
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
		printf("%s ",row[0]);
  	}


	char sql[100] = "";
	sprintf( sql,"UPDATE Calling SET Flag = '2' where userID = '%s';",userID);

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	//printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}

int DBconector_floor(int id){
	//나중에 사용할껀데 일단은 로컬에서 테스트 할꺼니깐 주석처리

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
  	while((row = mysql_fetch_row(res)) != NULL)
  		printf("%s ",row[0]);


  	if(mysql_query(conn,"SELECT * FROM building"))
  	{
  	        return 1;
  	}

  	res = mysql_use_result(conn);

   	//printf("Returning List of Names : \n");
   	while((row = mysql_fetch_row(res)) != NULL){
		//printf("%s %s %s %s \n",row[0],row[1],row[2],row[3]);
		tmp = atoi(row[2]);
	}


   mysql_free_result(res);
   mysql_close(conn);

   return tmp;
}

int DBconector_flag(){

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
  		printf("%s ",row[0]);
  	}
  		


  	if(mysql_query(conn,"SELECT Flag FROM SimulInfo"))
  	{
  	        return 1;
  	}

  	res = mysql_use_result(conn);

   	while((row = mysql_fetch_row(res)) != NULL){
		//printf("%s ",row[0]);
		tmp = atoi(row[0]);
	}


   mysql_free_result(res);
   mysql_close(conn);

   return tmp;
}

int DBconector_Simul_flag(int Simul_ID){

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
  		printf("%s ",row[0]);
  	}
  		
  	char query[100] ="";
  	sprintf(query,"SELECT Flag FROM SimulMode WHERE Simul_Id = %d",Simul_ID);

  	if(mysql_query(conn,query))
  	{
  	        return 1;
  	}

  	res = mysql_use_result(conn);

   	while((row = mysql_fetch_row(res)) != NULL){
		//printf("%s ",row[0]);
		tmp = atoi(row[0]);
	}


   mysql_free_result(res);
   mysql_close(conn);

   return tmp;
}


int DBconector_Simul_Mode(int Simul_ID){

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
  		printf("%s ",row[0]);
  	}
  		
  	char query[100] ="";
  	sprintf(query,"SELECT Mode FROM SimulMode WHERE Simul_Id = %d",Simul_ID);

  	if(mysql_query(conn,query))
  	{
  	        return 1;
  	}

  	res = mysql_use_result(conn);

   	while((row = mysql_fetch_row(res)) != NULL){
		//printf("%s ",row[0]);
		tmp = atoi(row[0]);
	}


   mysql_free_result(res);
   mysql_close(conn);

   return tmp;
}


void DBconector_Simul_Result(int Simul_ID, int Forecast_Latency , int Real_Latency , int User_Num , int Throughput ,float Avg_Forecast_Latency , float Avg_Real_Latency , float Accuration_Latency ){

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
  		printf("%s ",row[0]);
  	}
  	
  	char query[1000] ="";
  	//sprintf(query,"SELECT Mode FROM SimulMode WHERE Simul_Id = %d",Simul_ID);
	sprintf( query,"UPDATE Simul_Result SET Forecast_Latency = %d, Real_Latency = %d,  User_Num = %d, Throughput = %d, Avg_Forecast_Latency = %f, Avg_Real_Latency = %f, Accuration_Latency = %f where Simul_Id = %d",Forecast_Latency, Real_Latency, User_Num, Throughput, Avg_Forecast_Latency, Avg_Real_Latency, Accuration_Latency, Simul_ID);

  	if(mysql_query(conn,query))
  	{
  	        //return 1;
  	}
/*
  	res = mysql_use_result(conn);

   	while((row = mysql_fetch_row(res)) != NULL){
		//printf("%s ",row[0]);
		tmp = atoi(row[0]);
	}
*/

   mysql_free_result(res);
   mysql_close(conn);

}
void DB_Simul_Current(int Simul_ID, int ele_id, int current_floor){

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
	sprintf( sql,"UPDATE Simul_getCurr SET Current_Floor = %d where Simul_ID = %d AND Elevator_Id = %d;",current_floor,Simul_ID,ele_id);

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	//printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}

void DB_Simul_People(int Simul_ID, int ele_id, int People_Num){

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
	sprintf( sql,"UPDATE Simul_getCurr SET People_Num = %d where Simul_ID = %d AND Elevator_Id = %d;",People_Num,Simul_ID,ele_id);

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	//printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}
