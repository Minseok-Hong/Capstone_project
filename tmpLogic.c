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

#define CALL 'A'

#define BUFF_SIZE   1024
<<<<<<< HEAD
#define MAX_BUILDING 4 //서버에서 가동할 수 있는 최대 빌딩의 수
=======
#define MAX_BUILDING 2 //서버에서 가동할 수 있는 최대 빌딩의 수
//#define FLOOR 20
//#define NUM_ELEVATORS 6
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
#define MAX_PEOPLE 15 // 엘리베이터 정원
#define MAX_TOTAL 150 // 점검 받아야하는 수

/* 요청 구조체 */
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


int building_pid[MAX_BUILDING];
///////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
Elevator *LOOK(Elevator **elevators, int num, Request *current, char **userID);
=======
Elevator *LOOK(Elevator **elevators, int num, Request *current);
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
Elevator *C_SCAN(Elevator **elevators, int num, Request *current);
Elevator *cluster(Elevator **elevators, int num, Request *current);

F_node *Look_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target);
F_node *C_SCAN_up_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target);
F_node *cluster_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target);


F_node *find_scheduled_place(F_node *start, F_node *end, int start_floor, int dest_floor, int target);
F_node *find_direction_change_location(F_node *current, int current_direction);

void *simul_f(void *data);
void *input_f(void *data);
void *db_f(void *data);


<<<<<<< HEAD
void DB_Calling_updater(char *userID, int Time, int elevator_id);
void DB_Elevator_updater(int building_id);
void DB_Flag_updater(char *userID);
=======
void DB_updater(char *userID);
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
int DBconector_floor(int id);
int DBconector_ele_num(int id);
void move_elevator(Elevator **elevators, int num, int max_floor);
//void socket_server();

void init_input(Input **input);
void init_simul(Input **input, Simul **simul);
void init_elevator(Elevator **elevators, int num);

void get_request(Input *input, int elevator_id, int current_floor, int dest_floor, int people_num);
void insert_into_queue(int current_floor, int dest_floor, int num_people, int max_floor);

void F_list_insert(F_list list, F_node *after, int floor, int people);
int F_list_size(F_list list);
void F_list_remove(F_list list);
F_node *F_list_peek(F_list list);

Request *R_list_remove(R_list list);
int R_list_size(R_list list);
void R_list_insert(R_list list, int current_floor, int dest_floor, int num_people);

int find_time(F_list list, F_node *target, int start, int end);
int find_min(int *arr, int n);
<<<<<<< HEAD
int find_min_time(int numA, int numB);
=======
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17

void print_F_list(F_list list);
void print_elevator_info(Elevator **elevators, int num);
void print_UI(Elevator **elevators, int num, int floor_num);

//////////////////////////////////////////////////////////////////////

R_list reqs;
int flag = 0;
///////////////////////////////////////////////////////////////////////
int main()
{
 
 	Input *input;

 	pthread_t input_thr; 
 	pthread_t simul_thr;
 	pthread_t db_thr;
	
 	//pthread_t socket_thr;
	int tid_db;
	int tid_input;
	int tid_simul;
	//원래는 소켓통신으로 할 예정이지만 현재는 알고리즘이 잘 돌아가는지 볼것이므로 scanf를 받아서 실행한다.

	system("clear");
	init_input(&input);

	tid_input = pthread_create(&input_thr, NULL, input_f, (void *)input);
	if (tid_input != 0){
	    perror("thread creation error: ");
	    exit(0);
	}

<<<<<<< HEAD
	for(int i =  1; i <= MAX_BUILDING; i++){
=======
	for(int i =  1; i <= 2; i++){
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
  	//현재 등록된 건물의 수만큼 thread를 생성한다.
		Simul *simul;

		init_simul(&input, &simul);
  		tid_simul = pthread_create(&simul_thr, NULL, simul_f, (void *)simul);

  		printf("%d Thread Creat\n",i);
  		if (tid_simul != 0){
     		perror("thread creation error: ");
     		exit(0);
   		}

   		building_pid[i] = (int)simul_thr;
   		printf("%dth simul_thr : %u\n",i,building_pid[i]);
	}


  	for(int i = 0 ; i < MAX_BUILDING;i++){

		pthread_join(simul_thr, NULL);
	}



	pthread_join(input_thr, NULL);
	pthread_join(db_thr, NULL);
	

    return 0;
}

void *input_f(void *data){

	Input *input = (Input *)data;
	while(1){
		if (*input->mode == CALL){
		}
		else{
			scanf(" %c", input->mode);
		}
	}
}



void *simul_f(void *data){

	//printf("Thread Create\n");
	pthread_t id;
    // 현재 쓰레드의 id 를 얻어서 출력합니다
	id = pthread_self();
	int max_floor;
	int ele_num;
	int i;

	char *tmpUserId;
	char *tmpTimel;
	char *tmpElevatorId;

	Simul *simul = (Simul *)data;
	Elevator *elevators;
	Elevator *response; // 요청에 응답하는 엘리베이터
	F_node *location;   // 요청이 들어가는 위치
	Request current;    // 처리할 요청

	MYSQL *conn;
 	MYSQL_RES *res;
 	MYSQL_ROW row;

 	char *server = "localhost";
 	char *user = "root";
 	char *password = "root";
 	char *database = "capstone";


<<<<<<< HEAD
    max_floor = DBconector_floor(1);
    //max_floor = 11;
    //printf("max_floor : %d\n",max_floor);

    ele_num = DBconector_ele_num(1);
    //ele_num = 3;
=======
    MYSQL *conn;
 	MYSQL_RES *res;
 	MYSQL_ROW row;

 	char *server = "localhost";
 	char *user = "root";
 	char *password = "root";
 	char *database = "capstone";

    //max_floor = DBconector_floor(1);
    max_floor = 11;
    //printf("max_floor : %d\n",max_floor);

    //ele_num = DBconector_ele_num(1);
    ele_num = 3;
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
	//printf("ele_num : %d\n",ele_num);

	init_elevator(&elevators,ele_num);
	(*simul).elevators = &elevators;
	simul->input->mode = (char *)malloc(sizeof(char));
 	conn = (MYSQL *)malloc(sizeof(MYSQL )*1);
 	res = (MYSQL_RES *)malloc(sizeof(MYSQL_RES )*4);
 	row = (MYSQL_ROW )malloc(sizeof(MYSQL_ROW )*5);
<<<<<<< HEAD
 	tmpUserId = (char *)malloc(sizeof(char) * 100);
	
	//printf("1!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	conn = mysql_init(NULL);
	/*
	 for(int i = 1; i <= MAX_BUILDING ; i++){

 		if(building_pid[*(simul->input->req_elevator_id)] == (int)id){
 			
 		}

 	}*/
=======
	
	//printf("1!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	conn = mysql_init(NULL);
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17

	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
 		exit(1);
  	}
  	//printf("2!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	if(mysql_query(conn,"show tables")){

   		exit(1);
   	}
   	res = mysql_use_result(conn);
  	//printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){

  		printf("%s \n",row[0]);
  	}
    while(1){

<<<<<<< HEAD
    	sleep(1);
=======
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17

    	if(mysql_query(conn,"SELECT * FROM Calling where Flag = '0'"))
  		{
  		        //return 1;
  		}
	
	  	res = mysql_use_result(conn);

	   	while((row = mysql_fetch_row(res)) != NULL){
			printf("%s %s %s %s %s %s %s \n",row[0],row[1],row[2],row[3],row[4],row[5],row[6]);
			if(atoi(row[6]) == 0){
				printf("This Flag is ZERO!!!!!!!!!!!!!!!!!\n");
				*simul->input->mode = CALL;
				//printf("*simul->input->mode : %s\n", simul->input->mode);
			}

			if (*simul->input->mode == CALL)
			{
				get_request(simul->input,atoi(row[4]),atoi(row[1]),atoi(row[2]),1 );
			}
<<<<<<< HEAD
			printf("ROW[0] : %s \n",row[0]);
			strcpy(tmpUserId,row[0]);
		}
		//printf("tmpUserId : %s \n",tmpUserId);
		printf("simul->input->req_elevator_id : %d\n",*simul->input->req_elevator_id);
		printf("|%d    |   %d  |\n",building_pid[*(simul->input->req_elevator_id)+1], (int)id);
		printf("------------------------------\n");
		printf("|%d    |   %d  |\n",building_pid[*(simul->input->req_elevator_id)+1], (int)id);
			

		if(building_pid[*(simul->input->req_elevator_id)] != (int)id){
			printf("IN IF\n\n\n");
			//return 0;
			continue;

 		}

 		system("clear");
 		print_UI((&elevators),ele_num,max_floor );
 		
 		print_elevator_info((&elevators),ele_num);
 		printf("\n");		printf("------------------------------\n");		printf("|현재 엘리베이터 아이디 |  %d |\n ",*(simul->input->req_elevator_id));		printf("------------------------------\n");		//printf("building_pid[*(simul->input->req_elevator_id)] != id)\n");		printf("|%d    |   %d  |\n",building_pid[*(simul->input->req_elevator_id)], (int)id);		printf("------------------------------\n");		printf("|%dth elevator called         |\n",*(simul->input->req_elevator_id));		printf("------------------------------\n");
		printf("DB_Flag_updater\n\n");
       
       DB_Flag_updater(tmpUserId);
       insert_into_queue(*simul->input->req_current_floor, *simul->input->req_dest_floor, *simul->input->req_num_people, max_floor);

       if (R_list_size(reqs) != 0)
       {
            //여기는 LOOK알고리즘

            current = *R_list_remove(reqs);
            response = LOOK((simul->elevators),ele_num, &current, &tmpUserId);
=======
			
		}
			printf("simul->input->req_elevator_id : %d\n",*simul->input->req_elevator_id);
			printf("|%d    |   %d  |\n",building_pid[*(simul->input->req_elevator_id)+1], (int)id);
			printf("------------------------------\n");
			printf("|%d    |   %d  |\n",building_pid[*(simul->input->req_elevator_id)+1], (int)id);
			

			if(building_pid[*(simul->input->req_elevator_id)] != (int)id){
				printf("IN IF\n\n\n");
				//return 0;
				continue;
 
 			}

 			printf("#############\n");
 
 			system("clear");
 			print_UI((&elevators),ele_num,max_floor );
 			printf("\n");
 			print_elevator_info((&elevators),ele_num);
 			printf("\n");

			printf("------------------------------\n");
			printf("|현재 엘리베이터 아이디 |  %d |\n ",*(simul->input->req_elevator_id));
			printf("------------------------------\n");
			//printf("building_pid[*(simul->input->req_elevator_id)] != id)\n");
			printf("|%d    |   %d  |\n",building_pid[*(simul->input->req_elevator_id)], (int)id);
			printf("------------------------------\n");
			printf("|%dth elevator called         |\n",*(simul->input->req_elevator_id));
			printf("------------------------------\n");


        insert_into_queue(*simul->input->req_current_floor, *simul->input->req_dest_floor, *simul->input->req_num_people, max_floor);

        if (R_list_size(reqs) != 0)
        {
            

            //여기는 LOOK알고리즘

            current = *R_list_remove(reqs);
            response = LOOK((simul->elevators),ele_num, &current);
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
            printf("#1_LOOK\n");
            // 요청에 응답하는 엘리베이터에 정보 추가하기
            // 사람 태울 층 추가하기
            location = Look_find_ideal_location(&response, current.start_floor, current.dest_floor, current.start_floor);
            printf("#2_LOOK\n");
            F_list_insert(response->pending, location, current.start_floor, current.num_people);
<<<<<<< HEAD
            // 사람 내릴 층 추가하기
=======
  			// 사람 내릴 층 추가하기
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
            printf("#3_LOOK\n");
            location = Look_find_ideal_location(&response, current.start_floor, current.dest_floor, current.dest_floor);
            F_list_insert(response->pending, location, current.dest_floor, current.num_people * -1);
        	
            /*
			//여기는 C-SCAN알고리즘
        	current = *R_list_remove(reqs);
            response = C_SCAN((simul->elevators),ele_num, &current);
            //printf("1_LOOK\n");
            // 요청에 응답하는 엘리베이터에 정보 추가하기
            // 사람 태울 층 추가하기
            location = C_SCAN_up_find_ideal_location(&response, current.start_floor, current.dest_floor, current.start_floor);
            //printf("2_LOOK\n");
            F_list_insert(response->pending, location, current.start_floor, current.num_people);
  			// 사람 내릴 층 추가하기
            //printf("3_LOOK\n");
            location = C_SCAN_up_find_ideal_location(&response, current.start_floor, current.dest_floor, current.dest_floor);
            F_list_insert(response->pending, location, current.dest_floor, current.num_people * -1);
			*/

        }
        // 엘리베이터 이동시키기
<<<<<<< HEAD
        printf("move_elevator\n\n");
        move_elevator(simul->elevators, ele_num, max_floor);
        
        printf("sleep\n\n");
=======
        move_elevator(simul->elevators, ele_num, max_floor);
        
        DB_updater(row[0]);
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
        sleep(1);
		
    }
}

<<<<<<< HEAD
Elevator *LOOK(Elevator **elevators, int num, Request *current, char **userID){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size =num;
	int min = 100000;

	printf("#1 Look start\n");
	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 	time_required = (int *)malloc(sizeof(int) * size);


 	printf("#2 Look start\n");
 	for(int i = 0 ; i < num;i++){

 		ideal[i] = Look_find_ideal_location( (elevators + sizeof(Elevator)*i), current->start_floor, current->dest_floor, current->start_floor);
  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*i ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*i ))->current_floor, current->start_floor);
  		printf("%d번째 엘리베이터 소요시간: %d초 \n", i + 1, time_required[i]);
  		min = find_min_time(min,time_required[i]);

 	}

 	ideal_index = find_min(time_required, size);
  	free(time_required);
  	free(ideal);
  	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  	printf("MIN : %d\n",min);
  	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  	DB_Calling_updater(*userID, min, (ideal_index+1) );
    printf("엘리베이터 %d 호출에 응답 \n", ideal_index + 1);
    return *(elevators + sizeof(Elevator)*ideal_index);
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
  	printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s \n",row[0]);
  	}


	char sql[100] = "";
	sprintf( sql,"UPDATE Calling SET Flag = '2',Time = '%d', Elevator_Id = '%d' where userID = '%s';",Time, elevator_id, userID);

  	if(mysql_query(conn,sql))
  	{
  		printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	printf("########%s\n",sql);

   mysql_free_result(res);
   mysql_close(conn);

}

void DB_Flag_updater(char *userID){
=======
void DB_updater(char *userID){

>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17

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
  	printf("MYSQL Tables in mysql database : ");
  	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s \n",row[0]);
  	}


	char sql[100] = "";
	sprintf( sql,"UPDATE Calling SET Flag = '1' where userID = '%s';",userID);

  	if(mysql_query(conn,sql))
  	{
  		printf("###UPDATA ERROR!!!!!!!\n");
  		//return 1;
  	}
  	printf("########%s\n",sql);

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
		tmp = atoi(row[2]);
	}


   mysql_free_result(res);
   mysql_close(conn);

   return tmp;
}


void get_request(Input *input, int elevator_id, int current_floor, int dest_floor, int people_num)
{
    while (1)
    {
        printf("엘리베이터 아이디, 현재 층, 목적 층, 몇 명이 타는지 입력하시오. \n");
        fflush(stdout);
        //scanf("%d %d %d %d",input->req_elevator_id, input->req_current_floor, input->req_dest_floor, input->req_num_people);
        //printf("@@@@@@@@@@\n");
        *input->req_elevator_id =  elevator_id;
        *input->req_current_floor = current_floor;
        *input->req_dest_floor = dest_floor;
        *input->req_num_people = people_num;
        //printf("@@@@@@@@@@\n");
        tcflush(0, TCIFLUSH);
        *input->mode = 0;
        flag = 1;
       // printf("@@@@@@@@@@\n");
        break;
    }
}

Elevator *cluster(Elevator **elevators, int num, Request *current){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size =num;

	printf("#1 cluster start\n");
	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 	time_required = (int *)malloc(sizeof(int) * size);

 	printf("#2 cluster start\n");
 	for(int i = 0 ; i < num;i++){

 		ideal[i] = cluster_find_ideal_location( (elevators + sizeof(Elevator)*i), current->start_floor, current->dest_floor, current->start_floor);
  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*i ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*i ))->current_floor, current->start_floor);
  		printf("%d번째 엘리베이터 소요시간: %d초 \n", i + 1, time_required[i]);

 	}

 	ideal_index = find_min(time_required, size);
  	free(time_required);
  	free(ideal);

    printf("엘리베이터 %d 호출에 응답 \n", ideal_index + 1);
    return *(elevators + sizeof(Elevator)*ideal_index);

}
F_node *cluster_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target){
<<<<<<< HEAD

	int elevator_direction = 0;
	int call_direction = 0;

	F_list list = (*elevator)->pending;

=======

	int elevator_direction = 0;
	int call_direction = 0;

	F_list list = (*elevator)->pending;

>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
	F_node *start = list.head->next;
	F_node *end;


}

<<<<<<< HEAD
=======
Elevator *LOOK(Elevator **elevators, int num, Request *current){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size =num;

	printf("#1 Look start\n");
	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 	time_required = (int *)malloc(sizeof(int) * size);


 	printf("#2 Look start\n");
 	for(int i = 0 ; i < num;i++){

 		ideal[i] = Look_find_ideal_location( (elevators + sizeof(Elevator)*i), current->start_floor, current->dest_floor, current->start_floor);
  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*i ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*i ))->current_floor, current->start_floor);
  		printf("%d번째 엘리베이터 소요시간: %d초 \n", i + 1, time_required[i]);
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17

 	}

<<<<<<< HEAD
F_node *Look_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target)
{
	//printf("1Look_find_ideal_location\n");
	int elevator_direction = 0;
	int call_direction = 0;
	//printf("2Look_find_ideal_location\n");
	//printf("3Look_find_ideal_location\n");
    F_list list = (*elevator)->pending;
     //list = (F *)malloc(sizeof(F_list));
    //printf("4Look_find_ideal_location\n");

    F_node *start = list.head->next;
    F_node *end;

    if (F_list_size(list) == 0)
    {
        return start;
    }//현재 이동할 노드가 없다면
    //printf("Look_find_ideal_location\n");
    elevator_direction = start->floor - (*elevator)->current_floor;
    if(elevator_direction == 0)// 만약 제자리에 가만히 있는 엘리베이터라면
    {
        if(F_list_size(list) == 1)
        {
            return start->next;
        }
        else
        {
            elevator_direction = start->next->floor - start->floor;
        }
    }

    call_direction = dest_floor - start_floor;

    if (call_direction > 0)// 요청이 올라가려는 요청이라면
    {
        if (elevator_direction > 0)// 현재 올라가고 있는 엘리베이터라면
        {
            if (target >= (*elevator)->current_floor)//만약 올라가는 방향인데 내 경로에 요청이 같다면
            {
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_direction_change_location(start, elevator_direction);
            return find_scheduled_place(start, end, start_floor, dest_floor, target);
        }
    }
    else
    {
        if (elevator_direction < 0)
        {
            if (target <= (*elevator)->current_floor)
            {
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_direction_change_location(start, elevator_direction);
            return find_scheduled_place(start, end, start_floor, dest_floor, target);
        }
    }
}

F_node *find_scheduled_place(F_node *start, F_node *end, int start_floor, int dest_floor, int target)
{
    F_node *current = start;
    int call_direction = dest_floor - start_floor;
    if (call_direction > 0)//올라가는 방향이라면

    {
        while (1)
        {
            if(current->next == NULL)
            {
                return current;
            }
            if(current == end->next)
            {
                return current;
            }
            if(target < current->floor)
            {
                return current;
            }
            current = current->next;
        }
    }
    else//내려가는 방향이라면
    {
        while (1)
        {
            if(current->next == NULL)
            {
                return current;
            }
            if(current == end->next)
            {
                return current;
            }
            if(target > current->floor)
            {
                return current;
            }
            current = current->next;
        }
    }
}

Elevator *C_SCAN(Elevator **elevators, int num, Request *current){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size =num;

	printf("#1 C_SCAN start\n");
	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 	time_required = (int *)malloc(sizeof(int) * size);

 	printf("#2 C_SCAN start\n");
 	for(int i = 0 ; i < num;i++){

 		ideal[i] = C_SCAN_up_find_ideal_location( (elevators + sizeof(Elevator)*i), current->start_floor, current->dest_floor, current->start_floor);
  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*i ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*i ))->current_floor, current->start_floor);
  		printf("%d번째 엘리베이터 소요시간: %d초 \n", i + 1, time_required[i]);

 	}

 	ideal_index = find_min(time_required, size);
  	free(time_required);
  	free(ideal);

    printf("엘리베이터 %d 호출에 응답 \n", ideal_index + 1);
    return *(elevators + sizeof(Elevator)*ideal_index);
}

F_node *C_SCAN_up_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target)
{
	//일단은 올라가는 요청만 받는 알고리즘
	int elevator_direction = 0;
	int call_direction = 0;
    F_list list = (*elevator)->pending;
=======
 	ideal_index = find_min(time_required, size);
  	free(time_required);
  	free(ideal);

    printf("엘리베이터 %d 호출에 응답 \n", ideal_index + 1);
    return *(elevators + sizeof(Elevator)*ideal_index);
}
F_node *Look_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target)
{
	//printf("1Look_find_ideal_location\n");
	int elevator_direction = 0;
	int call_direction = 0;
	//printf("2Look_find_ideal_location\n");
	//printf("3Look_find_ideal_location\n");
    F_list list = (*elevator)->pending;
     //list = (F *)malloc(sizeof(F_list));
    //printf("4Look_find_ideal_location\n");
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17

    F_node *start = list.head->next;
    F_node *end;

    if (F_list_size(list) == 0)
    {
        return start;
    }//현재 이동할 노드가 없다면
    //printf("Look_find_ideal_location\n");
    elevator_direction = start->floor - (*elevator)->current_floor;
    if(elevator_direction == 0)// 만약 제자리에 가만히 있는 엘리베이터라면
    {
        if(F_list_size(list) == 1)
        {
            return start->next;
        }
        else
        {
            elevator_direction = start->next->floor - start->floor;
        }
    }
<<<<<<< HEAD

    call_direction = dest_floor - start_floor;

    if (call_direction > 0)// 요청이 올라가려는 요청이라면
    {
        if (elevator_direction > 0)// 현재 올라가고 있는 엘리베이터라면
        {
            if (target >= (*elevator)->current_floor)//만약 올라가는 방향인데 내 경로에 요청이 같다면
            {
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_direction_change_location(start, elevator_direction);
            return find_scheduled_place(start, end, start_floor, dest_floor, target);
        }
    }
    else
    {
    	/*
        if (elevator_direction < 0)
        {
            if (target <= (*elevator)->current_floor)
            {
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_direction_change_location(start, elevator_direction);
            return find_scheduled_place(start, end, start_floor, dest_floor, target);
        }*/
    }
}

void init_input(Input **input)
{
    int i, j;
    reqs.head = (R_node *)malloc(sizeof(R_node));
    reqs.tail = (R_node *)malloc(sizeof(R_node));
    reqs.head->prev = NULL;
    reqs.head->next = reqs.tail;
    reqs.tail->prev = reqs.head;
    reqs.tail->next = NULL;

    *input = (Input *)malloc(sizeof(Input));
    (*input)->mode = (char *)malloc(sizeof(char));
    (*input)->req_elevator_id = (int *)malloc(sizeof(int));
    (*input)->req_current_floor = (int *)malloc(sizeof(int));
    (*input)->req_dest_floor = (int *)malloc(sizeof(int));
    (*input)->req_num_people = (int *)malloc(sizeof(int));

}

void init_simul(Input **input, Simul **simul)
{
    int i, j;

    *input = (Input *)malloc(sizeof(Input));
    (*input)->mode = (char *)malloc(sizeof(char));
    (*input)->req_elevator_id = (int *)malloc(sizeof(int));
    (*input)->req_current_floor = (int *)malloc(sizeof(int));
    (*input)->req_dest_floor = (int *)malloc(sizeof(int));
    (*input)->req_num_people = (int *)malloc(sizeof(int));


    reqs.head = (R_node *)malloc(sizeof(R_node));
    reqs.tail = (R_node *)malloc(sizeof(R_node));
    reqs.head->prev = NULL;
    reqs.head->next = reqs.tail;
    reqs.tail->prev = reqs.head;
    reqs.tail->next = NULL;

    *simul = (Simul *)malloc(sizeof(Simul));
    (*simul)->input = *input;

}

void init_elevator(Elevator **elevators, int num){

	//printf("init_Start\n");
    for (int i = 0; i <= num; i++)
    {
         *(elevators + sizeof(Elevator)*i ) = (Elevator *)malloc(sizeof(Elevator));
    }

    //엘리베이터 : 1, 2 - 저층, 3, 4 - 전층, 5, 6 - 고층
    for (int i = 0; i <= num; i++)
    {
        (*(elevators + sizeof(Elevator)*i ))->pending.head = (F_node *)malloc(sizeof(F_node));
        (*(elevators + sizeof(Elevator)*i ))->pending.tail = (F_node *)malloc(sizeof(F_node));
        (*(elevators + sizeof(Elevator)*i ))->pending.head->floor = 0;
        (*(elevators + sizeof(Elevator)*i ))->pending.tail->floor = 0;
        (*(elevators + sizeof(Elevator)*i ))->pending.head->prev = NULL;
        (*(elevators + sizeof(Elevator)*i ))->pending.head->next = (*(elevators + sizeof(Elevator)*i ))->pending.tail;
        (*(elevators + sizeof(Elevator)*i ))->pending.tail->prev = (*(elevators + sizeof(Elevator)*i ))->pending.head;
        (*(elevators + sizeof(Elevator)*i ))->pending.tail->next = NULL;

        (*(elevators + sizeof(Elevator)*i ))->current_floor = 1;
        (*(elevators + sizeof(Elevator)*i ))->next_dest = 1;
        (*(elevators + sizeof(Elevator)*i ))->current_people = 0;
        (*(elevators + sizeof(Elevator)*i ))->total_people = 0;
        (*(elevators + sizeof(Elevator)*i ))->fix = 0;
        (*(elevators + sizeof(Elevator)*i ))->fix_time = 0;
    }
    //printf("init_END\n\n");

}
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

=======

    call_direction = dest_floor - start_floor;

    if (call_direction > 0)// 요청이 올라가려는 요청이라면
    {
        if (elevator_direction > 0)// 현재 올라가고 있는 엘리베이터라면
        {
            if (target >= (*elevator)->current_floor)//만약 올라가는 방향인데 내 경로에 요청이 같다면
            {
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_direction_change_location(start, elevator_direction);
            return find_scheduled_place(start, end, start_floor, dest_floor, target);
        }
    }
    else
    {
        if (elevator_direction < 0)
        {
            if (target <= (*elevator)->current_floor)
            {
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_direction_change_location(start, elevator_direction);
            return find_scheduled_place(start, end, start_floor, dest_floor, target);
        }
    }
}

F_node *find_scheduled_place(F_node *start, F_node *end, int start_floor, int dest_floor, int target)
{
    F_node *current = start;
    int call_direction = dest_floor - start_floor;
    if (call_direction > 0)//올라가는 방향이라면

    {
        while (1)
        {
            if(current->next == NULL)
            {
                return current;
            }
            if(current == end->next)
            {
                return current;
            }
            if(target < current->floor)
            {
                return current;
            }
            current = current->next;
        }
    }
    else//내려가는 방향이라면
    {
        while (1)
        {
            if(current->next == NULL)
            {
                return current;
            }
            if(current == end->next)
            {
                return current;
            }
            if(target > current->floor)
            {
                return current;
            }
            current = current->next;
        }
    }
}

Elevator *C_SCAN(Elevator **elevators, int num, Request *current){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size =num;

	printf("#1 C_SCAN start\n");
	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 	time_required = (int *)malloc(sizeof(int) * size);

 	printf("#2 C_SCAN start\n");
 	for(int i = 0 ; i < num;i++){

 		ideal[i] = C_SCAN_up_find_ideal_location( (elevators + sizeof(Elevator)*i), current->start_floor, current->dest_floor, current->start_floor);
  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*i ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*i ))->current_floor, current->start_floor);
  		printf("%d번째 엘리베이터 소요시간: %d초 \n", i + 1, time_required[i]);

 	}

 	ideal_index = find_min(time_required, size);
  	free(time_required);
  	free(ideal);

    printf("엘리베이터 %d 호출에 응답 \n", ideal_index + 1);
    return *(elevators + sizeof(Elevator)*ideal_index);
}

F_node *C_SCAN_up_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target)
{
	//일단은 올라가는 요청만 받는 알고리즘
	int elevator_direction = 0;
	int call_direction = 0;
    F_list list = (*elevator)->pending;

    F_node *start = list.head->next;
    F_node *end;

    if (F_list_size(list) == 0)
    {
        return start;
    }//현재 이동할 노드가 없다면
    //printf("Look_find_ideal_location\n");
    elevator_direction = start->floor - (*elevator)->current_floor;
    if(elevator_direction == 0)// 만약 제자리에 가만히 있는 엘리베이터라면
    {
        if(F_list_size(list) == 1)
        {
            return start->next;
        }
        else
        {
            elevator_direction = start->next->floor - start->floor;
        }
    }

    call_direction = dest_floor - start_floor;

    if (call_direction > 0)// 요청이 올라가려는 요청이라면
    {
        if (elevator_direction > 0)// 현재 올라가고 있는 엘리베이터라면
        {
            if (target >= (*elevator)->current_floor)//만약 올라가는 방향인데 내 경로에 요청이 같다면
            {
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_direction_change_location(start, elevator_direction);
            return find_scheduled_place(start, end, start_floor, dest_floor, target);
        }
    }
    else
    {
    	/*
        if (elevator_direction < 0)
        {
            if (target <= (*elevator)->current_floor)
            {
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_direction_change_location(start, elevator_direction);
                return find_scheduled_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_direction_change_location(start, elevator_direction);
            return find_scheduled_place(start, end, start_floor, dest_floor, target);
        }*/
    }
}

void init_input(Input **input)
{
    int i, j;
    reqs.head = (R_node *)malloc(sizeof(R_node));
    reqs.tail = (R_node *)malloc(sizeof(R_node));
    reqs.head->prev = NULL;
    reqs.head->next = reqs.tail;
    reqs.tail->prev = reqs.head;
    reqs.tail->next = NULL;

    *input = (Input *)malloc(sizeof(Input));
    (*input)->mode = (char *)malloc(sizeof(char));
    (*input)->req_elevator_id = (int *)malloc(sizeof(int));
    (*input)->req_current_floor = (int *)malloc(sizeof(int));
    (*input)->req_dest_floor = (int *)malloc(sizeof(int));
    (*input)->req_num_people = (int *)malloc(sizeof(int));

}

void init_simul(Input **input, Simul **simul)
{
    int i, j;

    *input = (Input *)malloc(sizeof(Input));
    (*input)->mode = (char *)malloc(sizeof(char));
    (*input)->req_elevator_id = (int *)malloc(sizeof(int));
    (*input)->req_current_floor = (int *)malloc(sizeof(int));
    (*input)->req_dest_floor = (int *)malloc(sizeof(int));
    (*input)->req_num_people = (int *)malloc(sizeof(int));


    reqs.head = (R_node *)malloc(sizeof(R_node));
    reqs.tail = (R_node *)malloc(sizeof(R_node));
    reqs.head->prev = NULL;
    reqs.head->next = reqs.tail;
    reqs.tail->prev = reqs.head;
    reqs.tail->next = NULL;

    *simul = (Simul *)malloc(sizeof(Simul));
    (*simul)->input = *input;

}

void init_elevator(Elevator **elevators, int num){

	//printf("init_Start\n");
    for (int i = 0; i <= num; i++)
    {
         *(elevators + sizeof(Elevator)*i ) = (Elevator *)malloc(sizeof(Elevator));
    }

    //엘리베이터 : 1, 2 - 저층, 3, 4 - 전층, 5, 6 - 고층
    for (int i = 0; i <= num; i++)
    {
        (*(elevators + sizeof(Elevator)*i ))->pending.head = (F_node *)malloc(sizeof(F_node));
        (*(elevators + sizeof(Elevator)*i ))->pending.tail = (F_node *)malloc(sizeof(F_node));
        (*(elevators + sizeof(Elevator)*i ))->pending.head->floor = 0;
        (*(elevators + sizeof(Elevator)*i ))->pending.tail->floor = 0;
        (*(elevators + sizeof(Elevator)*i ))->pending.head->prev = NULL;
        (*(elevators + sizeof(Elevator)*i ))->pending.head->next = (*(elevators + sizeof(Elevator)*i ))->pending.tail;
        (*(elevators + sizeof(Elevator)*i ))->pending.tail->prev = (*(elevators + sizeof(Elevator)*i ))->pending.head;
        (*(elevators + sizeof(Elevator)*i ))->pending.tail->next = NULL;

        (*(elevators + sizeof(Elevator)*i ))->current_floor = 1;
        (*(elevators + sizeof(Elevator)*i ))->next_dest = 1;
        (*(elevators + sizeof(Elevator)*i ))->current_people = 0;
        (*(elevators + sizeof(Elevator)*i ))->total_people = 0;
        (*(elevators + sizeof(Elevator)*i ))->fix = 0;
        (*(elevators + sizeof(Elevator)*i ))->fix_time = 0;
    }
    //printf("init_END\n\n");

}
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

>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
   	//printf("Returning List of Names : \n");
   	while((row = mysql_fetch_row(res)) != NULL){
		printf("%s %s %s %s \n",row[0],row[1],row[2],row[3]);
		tmp = atoi(row[3]);
	}


   mysql_free_result(res);
   mysql_close(conn);

   return tmp;
}

void move_elevator(Elevator **elevators, int num, int max_floor)
{
    int i;
    int to_ride = 0; // 태워야 할 사람 수
    int available;   // 정원이 초과될 시 최대로 태울수 있는 사람 수
    int leftover;    // 못 타고 남아있는 사람 수
    F_node *next_floor;
    F_node *pair;

    for (i = 0; i < num; i++)
    {
        if (F_list_size((*(elevators + sizeof(Elevator)*i ))->pending) > 0)
        {

            next_floor = F_list_peek((*(elevators + sizeof(Elevator)*i ))->pending);
            if (next_floor->floor == -1)
            {

            }
            else
            {
                if (  (*(elevators + sizeof(Elevator)*i ))->next_dest != next_floor->floor)
                {
                     (*(elevators + sizeof(Elevator)*i ))->next_dest = next_floor->floor;
                }

                if ((*(elevators + sizeof(Elevator)*i ))->next_dest > (*(elevators + sizeof(Elevator)*i ))->current_floor)
                {
                    ((*(elevators + sizeof(Elevator)*i ))->current_floor)++;
                }
                else if ((*(elevators + sizeof(Elevator)*i ))->next_dest < (*(elevators + sizeof(Elevator)*i ))->current_floor)
                {
                    ((*(elevators + sizeof(Elevator)*i ))->current_floor)--;
                }
                else
                {
                    if ((*(elevators + sizeof(Elevator)*i ))->current_floor == next_floor->floor)
                    {
                        available = MAX_PEOPLE - (*(elevators + sizeof(Elevator)*i ))->current_people;
                        if (next_floor->people <= available)
                        {
                            (*(elevators + sizeof(Elevator)*i ))->current_people += next_floor->people;
                            if (next_floor->people > 0)
                            {
                                (*(elevators + sizeof(Elevator)*i ))->total_people += next_floor->people;
                            }
                            F_list_remove((*(elevators + sizeof(Elevator)*i ))->pending);
                        }
                        else
                        {
                            (*(elevators + sizeof(Elevator)*i ))->current_people += available;
                            (*(elevators + sizeof(Elevator)*i ))->total_people += available;
                            leftover = next_floor->people - available;
                            pair = next_floor->next;
                            while (1)
                            {
                                if (next_floor->people + pair->people == 0)
                                {
                                    break;
                                }
                                pair = pair->next;
                            }
                            F_list_remove((*(elevators + sizeof(Elevator)*i ))->pending);

                            pair->people = available * -1;

                            flag = 1;
                            insert_into_queue((*(elevators + sizeof(Elevator)*i ))->current_floor, pair->floor, leftover, max_floor);
                        }
                    }
                }
            }
        }
    }

}

<<<<<<< HEAD
int find_min_time(int numA, int numB){

	if(numA > numB){
		return numB;
	}else{
		return numA;
	}



}
=======
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
int find_time(F_list list, F_node *target, int start, int end)
{
    int time = 0;
    F_node *curr = list.head->next;

    if(curr == target)
    {
        time += abs(end - start);
        return time;
    }

    if (curr->next == NULL)
    {
        time += abs(end - start);
        return time;
    }

    time += abs(curr->floor - start);
    time++;

    while (curr->next != target)
    {
        time += abs(curr->next->floor - curr->floor);
        time++;
        curr = curr->next;
    }
    time += abs(end - curr->floor);
    return time;
}

void insert_into_queue(int current_floor, int dest_floor, int num_people, int max_floor)
{
	//printf("insert_into_queue\n");
    if (flag == 0)
    {
        return;
    }

    if (current_floor == dest_floor)
    {
        return;
    }
    //printf("insert_into_queue\n");
    printf("R_list_insert : %d %d %d \n", current_floor, dest_floor, num_people);
    if (current_floor > max_floor || current_floor < 1 || dest_floor > max_floor || dest_floor < 1)
    {
        return;
    }
    printf("insert_into_queue\n");
    printf("R_list_insert : %d %d %d \n", current_floor, dest_floor, num_people);
    R_list_insert(reqs, current_floor, dest_floor, num_people);

    flag = 0;
}

int R_list_size(R_list list)
{
    int size = 0;
    R_node *curr = list.head->next;
    while (curr != list.tail)
    {
        size++;
        curr = curr->next;
    }

    return size;
}

Request *R_list_remove(R_list list)
{
    R_node *to_remove = list.head->next;
    Request *ret = &to_remove->req;

    to_remove->prev->next = to_remove->next;
    to_remove->next->prev = to_remove->prev;
    to_remove->next = NULL;
    to_remove->prev = NULL;

    free(to_remove);
    return ret;
}


void R_list_insert(R_list list, int current_floor, int dest_floor, int num_people)
{
	printf("R_list_insert(%d %d %d)\n", current_floor,dest_floor,num_people);
    R_node *new_node = (R_node *)malloc(sizeof(R_node));
    new_node->next = list.tail;
    new_node->prev = new_node->next->prev;
    new_node->prev->next = new_node;
    new_node->next->prev = new_node;
    new_node->req.start_floor = current_floor;
    new_node->req.dest_floor = dest_floor;
    new_node->req.num_people = num_people;
}

void F_list_insert(F_list list, F_node *after, int floor, int people)
{
    F_node *new_node = (F_node *)malloc(sizeof(F_node));
    new_node->next = after;
    new_node->prev = new_node->next->prev;
    new_node->next->prev = new_node;
    new_node->prev->next = new_node;
    new_node->floor = floor;
    new_node->people = people;
}

int F_list_size(F_list list)
{
    F_node *curr = list.head->next;
    int num = 0;

    while (curr != list.tail)
    {
        num++;
        curr = curr->next;
    }

    return num;
}

void F_list_remove(F_list list)
{
    F_node *to_remove = list.head->next;

    to_remove->prev->next = to_remove->next;
    to_remove->next->prev = to_remove->prev;
    to_remove->next = NULL;
    to_remove->prev = NULL;

    free(to_remove);
}

F_node *F_list_peek(F_list list)
{
    return list.head->next;
}

void print_F_list(F_list list)
{
    F_node *curr = list.head->next;
    while (curr != list.tail)
    {
        printf("(%dF %d명) ", curr->floor, curr->people);
        curr = curr->next;
    }
}

F_node *find_direction_change_location(F_node *current, int current_direction)
{
    int new_direction;
    F_node *target = current;

    while (1)
    {
        if(target->next == NULL)
        {
            return target;
        }
        else if(target->next->next == NULL)
        {
            return target->next;
        }

        new_direction = target->next->floor - target->floor;
        if(new_direction * current_direction < 0)
        {
            return target;
        }

        target = target->next;
    }
}

int find_min(int *arr, int n)
{
    // 우선순위 규칙
    // 1. 시간이 최소로 걸리는 엘리베이터
    // 2. 운행 범위가 좁은 엘리베이터(저층 > 전층)
    // 3. 현재 사람 수가 적은 엘리베이터(이미 만원인 엘리베이터는 제외)
    // 4. 인덱스가 적은 엘리베이터

    int i;
    int min = 0;
    for (i = 1; i < n; i++)
    {
        if (arr[i] < arr[min])
        {
            min = i;
        }
    }
    //추가 우선순위에 대한 고려 필요
    return min;
}



void print_UI(Elevator **elevators, int num, int floor_num)
{
    int i, j;
    for (i = 0; i < floor_num; i++)
    {
        printf("    -------------"); //윗칸

        for(int i = 0; i < num-1;i++){

        	printf("------------");
        }
        printf("\n");
        printf("%2dF ", floor_num - i);
        for (j = 0; j < num; j++)
        {
            if ( (*(elevators + sizeof(Elevator)*j ))->current_floor == floor_num - i)
            {
                printf("|");
                if ((*(elevators + sizeof(Elevator)*j ))->current_floor == (*(elevators + sizeof(Elevator)*j ))->next_dest)
                {
                    printf("   %2dF ", (*(elevators + sizeof(Elevator)*j ))->next_dest);
                }
                else if ((*(elevators + sizeof(Elevator)*j ))->current_floor > (*(elevators + sizeof(Elevator)*j ))->next_dest)
                {
                    printf(" ▼");
                    printf(" %2dF ", (*(elevators + sizeof(Elevator)*j ))->next_dest);
                }
                else
                {
                    printf("▲ ");
                    printf(" %2dF ", (*(elevators + sizeof(Elevator)*j ))->next_dest);
                }
                printf("%2d명", (*(elevators + sizeof(Elevator)*j ))->current_people);
            }
            else
            {
                printf("|           ");
            }
        }
        printf("| ");

        printf("\n");
    }
    printf("    -------------"); //윗칸

        for(int i = 0; i < num-1;i++){

        	printf("------------");
        }
        printf("\n");

}

void print_elevator_info(Elevator **elevators, int num)
{
    int i;
    for (i = 0; i < num; i++)
    {
        printf("엘리베이터 %d | ", i + 1);
        if ((*(elevators + sizeof(Elevator)*i ))->fix)
        {

        }
        else if ((*(elevators + sizeof(Elevator)*i ))->current_floor == (*(elevators + sizeof(Elevator)*i ))->next_dest)
        {
            printf("대기 중 | ");
        }
        else
        {
            printf("운행 중 | ");
        }
        printf("%2d명 탑승 중 | ", (*(elevators + sizeof(Elevator)*i ))->current_people);
        printf("총 %3d명 탑승 | ", (*(elevators + sizeof(Elevator)*i ))->total_people);
        printf("대기 요청 : ");
        print_F_list((*(elevators + sizeof(Elevator)*i ))->pending);
        printf("\n");
    }
}

/*
void socket_server(){
  //여기서 php랑 통신을 통해 파라미터를 받는다.
  int   client_socket;

  struct sockaddr_in   server_addr;

  char   buff[BUFF_SIZE+5];

  int building_id;
  int req_current_floor;
  int req_dest_floor;
  int req_num_people;

  client_socket  = socket( PF_INET, SOCK_STREAM, 0);
	if( -1 == client_socket){

		printf( "socket creat fail\n");
		exit( 1);
	}

	memset( &server_addr, 0, sizeof( server_addr));
	server_addr.sin_family     = AF_INET;
	server_addr.sin_port       = htons( 60000);
	server_addr.sin_addr.s_addr= inet_addr( "127.0.0.1");

	if( -1 == connect( client_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) ){

		printf( "connect fail\n");
		exit( 1);
	}

	//write( client_socket, argv[1], strlen( argv[1])+1);
	read ( client_socket, buff, BUFF_SIZE);
	printf( "PHP BUFFER == %s\n", buff);
	close( client_socket);


}
<<<<<<< HEAD
*/
=======
*/
>>>>>>> cdc995d69f621fbddc803648d4470f4eda0a4a17
