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

void DB_Elevator_updater(int building_id, int Elevator_Id, int current_floor);
void DB_Calling_updater(char *userID, int Time, int elevator_id);
void DB_Flag_updater(char *userID);
void DB_Flag2_updater(char *userID);
int DBconector_floor(int id);
int DBconector_ele_num(int id);

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
  	printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL)
  		printf("%s \n",row[0]);


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
  	//printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s \n",row[0]);
  	}


	char sql[100] = "";
	//sprintf( sql,"UPDATE getCurr SET Current_Floor = %d where Elevator_Id = %d AND Building_Id = %d;",current_floor, Elevator_Id, building_id);
	sprintf( sql,"UPDATE getCurr SET Current_Floor = %d where Elevator_Id = %d ;",current_floor, Elevator_Id);

  	if(mysql_query(conn,sql))
  	{
  		//printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}

void DB_Calling_updater(char *userID, int Time, int elevator_id){

	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
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
  	printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s \n",row[0]);
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
  	//printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s \n",row[0]);
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
  	//printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s \n",row[0]);
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
  	//printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL)
  		printf("%s \n",row[0]);


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