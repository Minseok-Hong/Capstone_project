#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "DB_Connector.h"
#include "/usr/include/mysql/mysql.h"

#define CALL 'A'

#define ASCII 48
#define MAX 10000

#define BUFF_SIZE   1024
#define MAX_BUILDING 2 //서버에서 가동할 수 있는 최대 빌딩의 수
#define MAX_PEOPLE 15 // 엘리베이터 정원
#define MAX_TOTAL 150 // 점검 받아야하는 수

int Simul_Id=2;
int Time_InterNal =100000;
int timeCheck =0;
int testCount=0;

typedef struct _TESTCASE
{
	int start_time;
	int current_floor;
	int go_Floor;

} Test_struct;

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
    int waiting_time;
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
    int Simul_name;
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Elevator *LOOK(Elevator **elevators, int num, Request *current, int *person_forecast_latency);
Elevator *High_Low(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int floor);
Elevator *Even_Odd(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int floor);
Elevator *C_SCAN(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int uprate);
Elevator *C_SCAN_LOOK(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int uprate);
Elevator *All_Scheduling(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int floor, int uprate, char *mode);

F_node *In_Look_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target);
//F_node *Out_Look_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target);
F_node *Out_Look_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target, F_node *tmp_start);

F_node *find_scheduled_out_place(F_node *start, F_node *end, int start_floor, int dest_floor, int target);
F_node *find_scheduled_in_place(F_node *start, F_node *end, int start_floor, int dest_floor, int target);
F_node *find_start_direction_change_location(F_node *current, int current_direction);

void *simul_f(void *data);
void *input_f(void *data);
void *db_f(void *data);

void move_elevator(Elevator **elevators, int num, int max_floor, int *Throughput);
//void socket_server();

void init_input(Input **input);
void init_simul(Input **input, Simul **simul);
void init_elevator(Elevator **elevators, int num);
void init_elevator_High_low(Elevator **elevators, int num, int floor);

void get_request(Input *input, int elevator_id, int current_floor, int dest_floor, int people_num, int *cumulative_user_number);
void insert_into_queue(int current_floor, int dest_floor, int num_people, int max_floor);
int makeUpNum(int num, int uprate);
char *inputFilter(int start_floor, int dest_floor, int floor);

//void F_list_insert(F_list list, F_node *after, int floor, int people);
F_node *F_list_insert(F_list list, F_node *after, int floor, int people);
int F_list_size(F_list list);
F_node *F_list_remove(F_list list);
F_node *F_list_peek(F_list list);

Request *R_list_remove(R_list list);
int R_list_size(R_list list);
void R_list_insert(R_list list, int current_floor, int dest_floor, int num_people);

int find_time(F_list list, F_node *target, int start, int end);
int find_min(int *arr, int n);
int find_min_time(int numA, int numB);
void calculate_real_time(F_list list, int *person_real_latency);

void print_F_list(F_list list);
void print_elevator_info(Elevator **elevators, int num , int person_forecast_latency, int cumulative_user_number, int *person_real_latency, int Throughput, int Simul_Id);
void print_UI(Elevator **elevators, int num, int floor_num, int Simul_Id);

int compare_with_size(const void *a, const void  *b);
void makeOnlyOneTest();
void makeTest();

int DBconector_testcase_pick();
//////////////////////////////////////////////////////////////////////

R_list reqs;
int flag = 0;


int inputNum;
int testCase; //-ASCII;
int timeInterval;
int Building_Floor;
int upRate;
int oneRate;
int upNum; 
int oneNum;	
int start_time, current_floor, go_Floor, re_start_time, re_go_Floor; 
int Elevator_num, Building_Floor, Test_Num, Start_time, Finish_time, Uprate, OneRate;

Test_struct test[MAX];

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
	
	system("clear");
	init_input(&input);

	tid_input = pthread_create(&input_thr, NULL, input_f, (void *)input);
	if (tid_input != 0){
	    perror("thread creation error: ");
	    exit(0);
	}

	while(1){

		if(DBconector_flag() == 1){
			break;
		}
		usleep(Time_InterNal);
	}
	
	

	while(1){

		if(DBconector_Simul_flag(1)==1){
			break;
		}
		usleep(Time_InterNal);
	}
	DB_Simul_Info_Flag();
	DB_Simul_Mode_Flag();
	
	for(int i =  1; i <= 1; i++){
  	//현재 등록된 건물의 수만큼 thread를 생성한다.
		Simul *simul;

		init_simul(&input, &simul);
		simul->Simul_name = i;
  		tid_simul = pthread_create(&simul_thr, NULL, simul_f, (void *)simul);
  		
  		printf("simul->Simul_name : %d\n",simul->Simul_name);
  		printf("아이디가 %d인 건물의 엘리베이터가 생성되었습니다.\n",i);

  		if (tid_simul != 0){
     		perror("thread creation error: ");
     		exit(0);
   		}

   		building_pid[i] = (int)simul_thr;
   		printf("building_pid[i] : %d\n",building_pid[i]);
   		printf("i : %d \n" ,i);
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

	pthread_t id;
    // 현재 쓰레드의 id 를 얻어서 출력합니다
	id = pthread_self();
	int max_floor;
	int ele_num;
	int i;
	
	int tmp_start_time, tmp_current_floor, tmp_go_floor;
	long pointer_location;

	FILE *f;

	Simul *simul = (Simul *)data;
	Elevator *elevators;
	Elevator *response; // 요청에 응답하는 엘리베이터
	F_node *location;   // 요청이 들어가는 위치
	Request current;    // 처리할 요청
	
	
	int Algo_mode;
	
	int person_forecast_latency =0;//현재까지 누적된 예측 평균 대기시간
	int cumulative_user_number =0;//누적 이용 사람 숫자
	int person_real_latency =0;//실제 개인이 기다린 대기시간
	int moved_people_number =0;  //현재까지 이동시킨 누적 사람수
	int Throughput = 0;
   
   /*
	//Simul_Id = *(simul->input->req_elevator_id);
	if(building_pid[1] == (int)id){
		Simul_Id=1;
		
	}
	else if(building_pid[2] == (int)id){
		Simul_Id=2;

	}*/
/*
	printf("Simul_Id : %d\n",Simul_Id);
	printf("building_pid1 : %d\n",building_pid[0]);
	printf("building_pid2 : %d\n",building_pid[1]);
	printf("building_pid3 : %d\n",building_pid[2]);
	printf("id : %d\n",(int)id);
	exit(0);
	*/
	/*
	Simul_Id =simul ->Simul_name;
	printf("Simul_Id : %d\n",Simul_Id);

	if(Simul_Id == 2){
		printf("Simul_Id : %d\n",Simul_Id);
		exit(0);
	}*/

	//
	//exit(0);
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
//////////////////////////////데이터베이스에서 원하는 정보를 전부 가져온다.////////////////////////////////////////////////////////////////////////////
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

  	if(mysql_query(conn,"SELECT * FROM SimulInfo"))
  	{

  	}
  	res = mysql_use_result(conn);

   	while((row = mysql_fetch_row(res)) != NULL){
		printf("\n----------------------------------------------\n");
		printf("Elevator_num | Building_Floor | Test_Num | Start_time | Finish_time | Uprate |\n");
		printf("%10s | %10s | %10s | %10s | %10s | %10s |",row[0],row[1],row[2],row[3],row[4],row[5]);

		Elevator_num = atoi(row[0]);
		Building_Floor =atoi(row[1]);
		Test_Num = atoi(row[2]);
		Start_time = atoi(row[3]);
		Finish_time = atoi(row[4]);
		Uprate = atoi(row[5]);
		OneRate = atoi(row[6]);
		printf("\n----------------------------------------------\n");
	}
	testCase = Test_Num;
	mysql_free_result(res);
	mysql_close(conn);
	char inputMaker[100] ="";
	//sprintf(inputMaker,"./InputMaker2 %d %d %d %d %d",Test_Num, (Finish_time- Start_time)*60 , Building_Floor, Uprate, OneRate);
	//system(inputMaker);

	DBconector_testcase_pick();
	testCount=0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 	max_floor = Building_Floor;
 	ele_num = Elevator_num;

	init_elevator(&elevators,ele_num);
	//init_elevator_High_low(&elevators,ele_num,max_floor);


	(*simul).elevators = &elevators;
	simul->input->mode = (char *)malloc(sizeof(char));
 	//*(simul->input->req_elevator_id) = 3;

 	f = fopen("testCase2.txt" ,"r");

 	//fscanf(f,"%d %d %d",&tmp_start_time, &tmp_current_floor, &tmp_go_floor);

 	tmp_start_time = test[testCount].start_time;
 	tmp_current_floor = test[testCount].current_floor;
 	tmp_go_floor = test[testCount].go_Floor;
 	testCount++;
/*
 	printf("tmp_start_time : %d",tmp_start_time);
 	printf("tmp_current_floor : %d",tmp_current_floor);
 	printf("tmp_go_floor : %d",tmp_go_floor);
 	exit(0);
 */
 	Algo_mode = DBconector_Simul_Mode(Simul_Id);
 	char *char_Algo_mode = malloc(sizeof(char)*100); 	
 	sprintf(char_Algo_mode,"%d",Algo_mode);

 	while(1){
 		system("clear");
 		printf("\n==========================\n");
 		printf("|현재 시간 : %6d      |\n",timeCheck);
 		printf("|tmp_start_time : %6d |\n",tmp_start_time);
 		printf("|tmp_current_floor : %3d |\n",tmp_current_floor);
 		printf("|tmp_go_floor :  %3d     |\n",tmp_go_floor);
 		printf("==========================\n");

    	if(tmp_start_time == timeCheck){//현재 시간과 데이터의 시간이 같은 경우
    		*simul->input->mode = CALL;

    	}
		else{//현재 시간과 데이터의 시간이 다른경우

 			print_UI((&elevators),ele_num,max_floor ,Simul_Id);
 			//usleep(Time_InterNal);
 			print_elevator_info((&elevators),ele_num, person_forecast_latency,cumulative_user_number, &person_real_latency,Throughput,Simul_Id);
			move_elevator(simul->elevators, ele_num, max_floor, &Throughput);
			usleep(Time_InterNal);
			timeCheck++;
			continue;
		}
		
		if (*simul->input->mode == CALL)
		{
			get_request(simul->input,1,tmp_current_floor, tmp_go_floor,1, &cumulative_user_number );
			//fscanf(f,"%d %d %d",&tmp_start_time, &tmp_current_floor, &tmp_go_floor);
			
			tmp_start_time = test[testCount].start_time;
 			tmp_current_floor = test[testCount].current_floor;
 			tmp_go_floor = test[testCount].go_Floor;
 			testCount++;
			
		}
		insert_into_queue(*simul->input->req_current_floor, *simul->input->req_dest_floor, *simul->input->req_num_people, max_floor);
		
		while(testCount !=testCase) {
			//sleep(1);
			//printf("@@@@@@@@@@@@@@@@@@@@@@@@\n");
			if(tmp_start_time != timeCheck){
				break;
			}
			else{
				*simul->input->mode = CALL;
				get_request(simul->input,1,tmp_current_floor, tmp_go_floor,1, &cumulative_user_number );
				insert_into_queue(*simul->input->req_current_floor, *simul->input->req_dest_floor, *simul->input->req_num_people, max_floor);
				//fscanf(f,"%d %d %d",&tmp_start_time, &tmp_current_floor, &tmp_go_floor);
				//printf("Test : %d %d %d\n",tmp_start_time, tmp_current_floor, tmp_go_floor);
				//return 0;
				
				tmp_start_time = test[testCount].start_time;
 				tmp_current_floor = test[testCount].current_floor;
 				tmp_go_floor = test[testCount].go_Floor;
 				testCount++;
				
			}
		}
		//printf("@@@@@@@@@@@@@@@@@@@@@@@@\n");
/*
		if(building_pid[*(simul->input->req_elevator_id)] != (int)id){
			continue;
 		}
*/
 		system("clear");
 		printf("\n==========================\n");
 		printf("|현재 시간 : %6d      |\n",timeCheck);
 		printf("|tmp_start_time : %6d |\n",tmp_start_time);
 		printf("|tmp_current_floor : %3d |\n",tmp_current_floor);
 		printf("|tmp_go_floor :  %3d     |\n",tmp_go_floor);
 		printf("==========================\n");
 		
 		print_UI((&elevators),ele_num,max_floor,Simul_Id );

 		print_elevator_info((&elevators),ele_num, person_forecast_latency,cumulative_user_number, &person_real_latency,Throughput,Simul_Id);
       //insert_into_queue(*simul->input->req_current_floor, *simul->input->req_dest_floor, *simul->input->req_num_people, max_floor);

       
       //if (R_list_size(reqs) != 0)
       while(R_list_size(reqs) != 0){
       	//여기는 LOOK알고리즘
       	//printf("R_list_size(reqs) : %d\n",R_list_size(reqs));
            current = *R_list_remove(reqs);
            
            //response = LOOK((simul->elevators),ele_num, &current, &person_forecast_latency);
            //look

            //response = High_Low((simul->elevators),ele_num, &current, &person_forecast_latency, max_floor);
            //Hig_Low

            //response = Even_Odd((simul->elevators),ele_num, &current, &person_forecast_latency, max_floor);
            //even-odd

            //response = C_SCAN((simul->elevators),ele_num, &current, &person_forecast_latency, Uprate);
            //C_SCAN

            //response = C_SCAN_LOOK((simul->elevators),ele_num, &current, &person_forecast_latency, Uprate);
            //C_SCAN_LOOK

            //response = Existing_Look((simul->elevators),ele_num, &current, &person_forecast_latency);
            //Existing_Look

            response = All_Scheduling((simul->elevators),ele_num, &current, &person_forecast_latency, max_floor, Uprate,char_Algo_mode);
            //All_Scheduling
            
            // 요청에 응답하는 엘리베이터에 정보 추가하기
            // 사람 태울 층 추가하기
            location = In_Look_find_ideal_location(&response, current.start_floor, current.dest_floor, current.start_floor);
            F_node *tmp_F_node = F_list_insert(response->pending, location, current.start_floor, current.num_people);
             
            // 사람 내릴 층 추가하기
            //location = Out_Look_find_ideal_location(&response, current.start_floor, current.dest_floor, current.dest_floor);
            location = Out_Look_find_ideal_location(&response, current.start_floor, current.dest_floor, current.dest_floor, tmp_F_node);
            F_list_insert(response->pending, location, current.dest_floor, current.num_people * -1);
        }
        // 엘리베이터 이동시키기

        move_elevator(simul->elevators, ele_num, max_floor, &Throughput);
        usleep(Time_InterNal);
        timeCheck++;

        if(testCount == testCase){
			printf("모든 데이터가 스케쥴링 되었습니다.\n");
			printf("총 소요시간 : %d\n",timeCheck);
			exit(0);

		}
    }
}
Elevator *LOOK(Elevator **elevators, int num, Request *current, int *person_forecast_latency){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size =num;
	int min = 100000;

	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 	time_required = (int *)malloc(sizeof(int) * size);

 	for(int i = 0 ; i < num;i++){

 		if((*(elevators + sizeof(Elevator)*(i) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 		}
 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*i), current->start_floor, current->dest_floor, current->start_floor);
  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*i ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*i ))->current_floor, current->start_floor);
  		//printf("%d번째 엘리베이터 소요시간: %d초 \n", i + 1, time_required[i]);
  		min = find_min_time(min,time_required[i]);

 	}
 	//sleep(1);
 	ideal_index = find_min(time_required, size);
  	free(time_required);
  	free(ideal);

  	*person_forecast_latency += min;
  	//printf("엘리베이터 %d 호출에 응답 \n", ideal_index + 1);
  	return *(elevators + sizeof(Elevator)*ideal_index);

}

Elevator *All_Scheduling(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int floor, int uprate, char *mode){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size =num;
	int min = 100000;
	char *inputmode = inputFilter(current->start_floor, current->dest_floor, floor);

	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 	time_required = (int *)malloc(sizeof(int) * size);

 	for(int i = 0 ; i<num; i++){
 		int tmp_Elevator_Mode = mode[i] -'0';
 		//1:Look 2:짝수 3:홀수 4:저층 5:고층 6:상향 7:하향 8:기존 Look
 		for(int j = 0 ; j < strlen(inputmode); j++){
 
 			if(tmp_Elevator_Mode == inputmode[j] - '0'){

 				if((*(elevators + sizeof(Elevator)*(i) ))->pending.tail->prev->floor == -1){
 						time_required[i] = MAX;

 						continue;
 				}
 				ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*i), current->start_floor, current->dest_floor, current->start_floor);
  				time_required[i] = find_time( (*(elevators + sizeof(Elevator)*i ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*i ))->current_floor, current->start_floor);
  				//printf("%d번째 엘리베이터 소요시간: %d초 \n", i + 1, time_required[i]);
  				min = find_min_time(min,time_required[i]);
  				break;
 			}else{
 				time_required[i] = MAX;
 				continue;
 			}
 		}
 	}
 	//sleep(1);
 	ideal_index = find_min(time_required, size);
 	min = time_required[ideal_index];
  	free(time_required);
  	free(ideal);
  	

  	*person_forecast_latency += min;
  	//printf("엘리베이터 %d 호출에 응답 \n", ideal_index + 1);
  	return *(elevators + sizeof(Elevator)*ideal_index);

}


Elevator *Even_Odd(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int floor){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size;
	int min = 100000;
	int s = 0;
	int mid_floor = (floor/2);

    if((current->start_floor == 1 && current->dest_floor%2 == 0) || (current->start_floor%2 ==0 && current->dest_floor == 1) || (current->dest_floor%2 == 0) && (current->start_floor%2 ==0)){
    	//짝수 
    	s =0;
    	size = num/3;

    	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
 		
 		for(int i = 0 ; i < (num/3);i++){

 			if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
    }

    else if((current->start_floor == 1 && current->dest_floor%2 == 1) || (current->start_floor%2 ==1 && current->dest_floor == 1) || (current->dest_floor%2 == 1) && (current->start_floor%2 ==1)){
		//홀수 
		s = num/3;
		size = num -(num/3);
		ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
		
		for(int i = 0 ; i < num - 2*(num/3);i++){
			if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	 	}
    }
    else{
    	//예외 사항
    	s = 2*(num/3);
    	size = num - 2*(num/3);
    	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
		
    	for(int i = 0 ; i < (num/3);i++){
    		if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
	
	 	
    }
    ideal_index = find_min(time_required, size);
    free(time_required);
    free(ideal);
	
  	*person_forecast_latency += min;

    return *(elevators + sizeof(Elevator)*(ideal_index + s));
}

Elevator *High_Low(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int floor){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size;
	int min = 100000;
	int s = 0;
	int mid_floor = (floor/2);

    if((current->start_floor > mid_floor && current->dest_floor <=mid_floor) || (current->start_floor <= mid_floor && current->dest_floor > mid_floor)){
    	//전층
    	s =num/3;
    	size = num - 2*(num/3);


    	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
 		
 		for(int i = 0 ; i < num - 2*(num/3);i++){

 			if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
    }

    else if ((current->start_floor > mid_floor) || (current->dest_floor > mid_floor)){
		//고층  
		s=num/3;
		size = num -(num/3);
		ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
		
		for(int i = 0 ; i < num - (num/3);i++){
			if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	 	}
    }
    else{
    	//저층
    	size = num - 2*(num/3);
    	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
		
    	for(int i = 0 ; i < (num/3);i++){
    		if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
	
	 	
    }
    ideal_index = find_min(time_required, size);
    free(time_required);
    free(ideal);
	
  	*person_forecast_latency += min;

    return *(elevators + sizeof(Elevator)*(ideal_index + s));
}

Elevator *Existing_Look(Elevator **elevators, int num, Request *current, int *person_forecast_latency){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size =num;
	int min = 100000;

	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 	time_required = (int *)malloc(sizeof(int) * size);

 	for(int i = 0 ; i < num;i++){
 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*i), current->start_floor, current->dest_floor, current->start_floor);
  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*i ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*i ))->current_floor, current->start_floor);
  		min = find_min_time(min,time_required[i]);
 	}

 	ideal_index = find_min(time_required, size);
  	free(time_required);
  	free(ideal);

  	*person_forecast_latency += min;
  	return *(elevators + sizeof(Elevator)*ideal_index);
}

Elevator *C_SCAN(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int uprate){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size;
	int min = 100000;
	int s = 0;
	int upNum = makeUpNum(num,uprate);
	
	//printf("upNum : %d(num(%d),uprate(%d))\n",upNum,num,uprate);
	
	int dir = current->dest_floor - current->start_floor;

    if(dir >0){
    	//상향
    	//printf("//상향\n");
    	s =0;
    	size = upNum;


    	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
 		
 		for(int i = 0 ; i < upNum;i++){

 			if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
    }
    /*
    else if ((current->start_floor > ) || (current->dest_floor > )){
		//전층
		s=num/3;
		size = num -(num/3);
		ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
		
		for(int i = 0 ; i < num - (num/3);i++){
			if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
		
    }*/
    else{
    	//햐향
    	//printf("//햐향\n");
    	
    	s = upNum;
    	size = num - upNum;
    	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
		
    	for(int i = 0 ; i < num - upNum; i++){
    		if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
	
	 	
    }
    ideal_index = find_min(time_required, size);
    free(time_required);
    free(ideal);
	
  	*person_forecast_latency += min;

    return *(elevators + sizeof(Elevator)*(ideal_index + s));
}

Elevator *C_SCAN_LOOK(Elevator **elevators, int num, Request *current, int *person_forecast_latency, int uprate){

	F_node **ideal;
	int *time_required;
	int ideal_index;
	int size;
	int min = 100000;
	int s = 0;
	int upNum = makeUpNum(num,uprate);
	
	//printf("upNum : %d(num(%d),uprate(%d))\n",upNum,num,uprate);
	
	int dir = current->dest_floor - current->start_floor;

    if(dir >0){
    	//상향
    	//printf("//상향\n");
    	s =0;
    	size = num;


    	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
 		
 		for(int i = 0 ; i < num;i++){

 			if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
    }
    /*
    else if ((current->start_floor > ) || (current->dest_floor > )){
		//전층
		s=num/3;
		size = num -(num/3);
		ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
		
		for(int i = 0 ; i < num - (num/3);i++){
			if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
		
    }*/
    else{
    	//햐향
    	//printf("//햐향\n");
    	
    	s = upNum;
    	size = num - upNum;
    	ideal = (F_node **)malloc(sizeof(F_node *) * size);
 		time_required = (int *)malloc(sizeof(int) * size);
		
    	for(int i = 0 ; i < num - upNum; i++){
    		if((*(elevators + sizeof(Elevator)*(i+s) ))->pending.tail->prev->floor == -1){
 				time_required[i] = MAX;
 				continue;
 			}
	 		ideal[i] = In_Look_find_ideal_location( (elevators + sizeof(Elevator)*(i+s)), current->start_floor, current->dest_floor, current->start_floor);
	  		time_required[i] = find_time( (*(elevators + sizeof(Elevator)*(i+s) ))->pending, ideal[i],  (*(elevators + sizeof(Elevator)*(i+s) ))->current_floor, current->start_floor);
	  		min = find_min_time(min,time_required[i]);
	
	 	}
	
	 	
    }
    ideal_index = find_min(time_required, size);
    free(time_required);
    free(ideal);
	
  	*person_forecast_latency += min;

    return *(elevators + sizeof(Elevator)*(ideal_index + s));
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

void init_elevator_High_low(Elevator **elevators, int num, int floor){

	int low = num/3;
	int all; // num/3 +1 ~ num - nim/3
	int high; // (num - nim/3)+1 ~ num
	//printf("init_Start\n");
    for (int i = 0; i <= num; i++)
    {
         *(elevators + sizeof(Elevator)*i ) = (Elevator *)malloc(sizeof(Elevator));
    }

    //
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
    for(int i =(num - num/3); i<= num; i++){
    	(*(elevators + sizeof(Elevator)*i ))->current_floor = floor/2 +1;
    	(*(elevators + sizeof(Elevator)*i ))->next_dest = floor/2 +1;
    }

    //num/3 max = 전층엘리베이터 : 1, 2 - 저층, 3, 4 - 전층, 5, 6 - 고층
    //printf("init_END\n\n");

}


void move_elevator(Elevator **elevators, int num, int max_floor, int *Throughput)
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
                            F_node *temp_Node = F_list_remove((*(elevators + sizeof(Elevator)*i ))->pending);
                            if(temp_Node->people < 0){
                            	*Throughput += 1;
                            }

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

int find_min_time(int numA, int numB){

	if(numA > numB){
		return numB;
	}else{
		return numA;
	}
}

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
    //printf("R_list_insert : %d %d %d \n", current_floor, dest_floor, num_people);
    if (current_floor > max_floor || current_floor < 1 || dest_floor > max_floor || dest_floor < 1)
    {
        return;
    }
    //printf("insert_into_queue\n");
    //printf("R_list_insert : %d %d %d \n", current_floor, dest_floor, num_people);
    R_list_insert(reqs, current_floor, dest_floor, num_people);

    flag = 0;
}

F_node *find_start_direction_change_location(F_node *current, int current_direction)
{
    int new_direction;
    F_node *target = current;
    F_node *temp;

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
        	while(target->prev->floor == target->floor){
				target = target->prev;        		
        	}

            return target;
        }

        target = target->next;
    }
}

F_node *find_end_direction_change_location(F_node *current, int current_direction)
{
    int new_direction;
    F_node *target = current;
    F_node *temp;

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

        	while(target->prev->floor == target->floor){
				target = target->prev;        		
        	}

            return target;
        }

        target = target->next;
    }
}

F_node *find_scheduled_in_place(F_node *start, F_node *end, int start_floor, int dest_floor, int target)
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

F_node *find_scheduled_out_place(F_node *start, F_node *end, int start_floor, int dest_floor, int target)
{
    F_node *current = start;
    //F_node *current = end;
	//F_node *current = end->prev;
    int call_direction = dest_floor - start_floor;
    
    
    if (call_direction > 0)//올라가는 방향이라면
    {
        while (1)
        {

            if(current->next == NULL)
            {
                return current;
            }
            /*
            if(current == end->next)
            {
                return current;
            }*/
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

//F_node *Out_Look_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target)
F_node *Out_Look_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target, F_node *tmp_start)
{
	//printf("tmp_start : %d\n",tmp_start->floor);
	int elevator_direction = 0;
	int call_direction = 0;

    F_list list = (*elevator)->pending;
    //F_node *start = list.head->next;
    F_node *start = tmp_start;
    F_node *end;
    
    if (F_list_size(list) == 0)
    {
        return start;
    }//현재 이동할 노드가 없다면
    
    elevator_direction = start->floor - (*elevator)->current_floor;

    if(elevator_direction == 0)// 만약 제자리에 가만히 있는 엘리베이터라면
    {
    	 if(F_list_size(list) == 1){
            return start->next;
        }
        else{
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
                end = find_end_direction_change_location(start, elevator_direction);
                return find_scheduled_out_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_start_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_start_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = tmp_start;
                end = find_end_direction_change_location(start, elevator_direction);
                return find_scheduled_out_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_start_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            start = tmp_start;
            end = find_end_direction_change_location(start, elevator_direction);
            return find_scheduled_out_place(start, end, start_floor, dest_floor, target);
        }
    }
    else
    {
        if (elevator_direction < 0)
        {
            if (target <= (*elevator)->current_floor)
            {
            		start = tmp_start;
                end = find_end_direction_change_location(start, elevator_direction);
                return find_scheduled_out_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_start_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_start_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = tmp_start;
                end = find_end_direction_change_location(start, elevator_direction);
                return find_scheduled_out_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_start_direction_change_location(start, elevator_direction);
            //printf("START : %d\n",start->floor);
            elevator_direction *= -1;
            start = tmp_start;
            end = find_end_direction_change_location(start, elevator_direction);
            //printf("END : %d\n",end->floor);
            return find_scheduled_out_place(start, end, start_floor, dest_floor, target);
        }
    }
}

F_node *In_Look_find_ideal_location(Elevator **elevator, int start_floor, int dest_floor, int target)
{
	
	int elevator_direction = 0;
	int call_direction = 0;

    F_list list = (*elevator)->pending;
    F_node *start = list.head->next;
    F_node *end;
    
    if (F_list_size(list) == 0)
    {
        return start;
    }//현재 이동할 노드가 없다면
    
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
            	//printf("IF\n");
                end = find_end_direction_change_location(start, elevator_direction);
                return find_scheduled_in_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
            	//printf("ELSE\n");
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_start_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_start_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_end_direction_change_location(start, elevator_direction);
                return find_scheduled_in_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_start_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_end_direction_change_location(start, elevator_direction);
            return find_scheduled_in_place(start, end, start_floor, dest_floor, target);
        }
    }
    else
    {
        if (elevator_direction < 0)
        {
            if (target <= (*elevator)->current_floor)
            {
                end = find_end_direction_change_location(start, elevator_direction);
                return find_scheduled_in_place(start, end, start_floor, dest_floor, target);
            }
            else
            {
                // 다음 방향 같아질 때 까지 찾아야 함!
                start = find_start_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                start = find_start_direction_change_location(start, elevator_direction);
                elevator_direction *= -1;
                end = find_end_direction_change_location(start, elevator_direction);
                return find_scheduled_in_place(start, end, start_floor, dest_floor, target);
            }
        }
        else
        {
            // 다음 방향 바뀔 때 까지 찾아야 함!
            start = find_start_direction_change_location(start, elevator_direction);
            elevator_direction *= -1;
            end = find_end_direction_change_location(start, elevator_direction);
            return find_scheduled_in_place(start, end, start_floor, dest_floor, target);
        }
    }
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
    //printf("R_list_size\n");
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
	//printf("R_list_insert(%d %d %d)\n", current_floor,dest_floor,num_people);
    R_node *new_node = (R_node *)malloc(sizeof(R_node));
    new_node->next = list.tail;
    new_node->prev = new_node->next->prev;
    new_node->prev->next = new_node;
    new_node->next->prev = new_node;
    new_node->req.start_floor = current_floor;
    new_node->req.dest_floor = dest_floor;
    new_node->req.num_people = num_people;
}

//void F_list_insert(F_list list, F_node *after, int floor, int people)
F_node *F_list_insert(F_list list, F_node *after, int floor, int people)
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

//void F_list_remove(F_list list)
F_node *F_list_remove(F_list list)
{
    F_node *to_remove = list.head->next;

    to_remove->prev->next = to_remove->next;
    to_remove->next->prev = to_remove->prev;
    to_remove->next = NULL;
    to_remove->prev = NULL;

    free(to_remove);

    return to_remove;
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

void calculate_real_time(F_list list, int *person_real_latency)
{

    F_node *curr = list.head->next;
    //printf("1.3\n");
    while (curr != list.tail)
    {
    	//printf("#curr->floor : %d\n",curr->floor);
        //curr->waiting_time = curr->waiting_time+1;
        //curr = curr->next;
    	//printf("2\n");
    	if(curr->people>0){
    		(*person_real_latency) +=1;
    	}
    	//printf("3\n");
    	curr = curr->next;
    	//printf("4\n");
    }
    //printf("5\n");
}


int find_min(int *arr, int n)
{

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



void print_UI(Elevator **elevators, int num, int floor_num, int Simul_Id)
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
            	DB_Simul_Current(Simul_Id, j+1, (*(elevators + sizeof(Elevator)*j ))->current_floor);
            	DB_Simul_People(Simul_Id, j+1, (*(elevators + sizeof(Elevator)*j ))->current_people);
            	//exit(0);
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
                /*
                if((*(elevators + sizeof(Elevator)*j ))->current_people <0){
                	 exit(0);
                }*/

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

void print_elevator_info(Elevator **elevators, int num , int person_forecast_latency, int cumulative_user_number, int *person_real_latency, int Throughput, int Simul_Id)
{
    int i;
    for (i = 0; i < num; i++)
    {
        printf("엘리베이터 %d | ", i + 1);
        if ((*(elevators + sizeof(Elevator)*i ))->fix){

        }
        else if ((*(elevators + sizeof(Elevator)*i ))->current_floor == (*(elevators + sizeof(Elevator)*i ))->next_dest){
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
        calculate_real_time((*(elevators + sizeof(Elevator)*i ))->pending, person_real_latency);
        printf("\n");
    }
    printf("\n====================================\n");
    printf("|예측 대기시간 누적       |%6d초|\n",person_forecast_latency);
    printf("|실제 대기시간 누적       |%6d초|\n",*person_real_latency);
    printf("|처리량                   |%6d명|\n",Throughput);
    printf("|누적 이용 사람 숫자      |%6d명|\n",cumulative_user_number);
    printf("====================================\n");
    
    double person_forecast_latency_average;
    double person_real_latency_average;
    double accuration_latency_average;

    if(cumulative_user_number == 0){
    	person_forecast_latency_average =0;
    	person_real_latency_average=0;
    }
    else{
    	person_forecast_latency_average = ((double)person_forecast_latency/(double)cumulative_user_number) /*+ ((double)person_forecast_latency % (double)cumulative_user_number)*/;	
    	person_real_latency_average =((double)*person_real_latency/(double)cumulative_user_number) ;
    	accuration_latency_average = ((double)person_forecast_latency_average/(double)person_real_latency_average)*100 ;
    }
    printf("|예상 대기시간 정확도     | %3.2f  |\n",accuration_latency_average);
    printf("|개인당 평균 예상 대기시간| %3.3f초|\n",person_forecast_latency_average);
    printf("|개인당 평균 실제 대기시간| %3.3f초|\n",person_real_latency_average);
    printf("====================================\n");
    DBconector_Simul_Result(Simul_Id,person_forecast_latency,*person_real_latency,cumulative_user_number,Throughput, person_forecast_latency_average, person_real_latency_average, accuration_latency_average);
}

void get_request(Input *input, int elevator_id, int current_floor, int dest_floor, int people_num, int *cumulative_user_number)
{
    while (1)
    {
        fflush(stdout);

        //*input->req_elevator_id =  elevator_id;
        *input->req_current_floor = current_floor;
        *input->req_dest_floor = dest_floor;
        *input->req_num_people = people_num;
        *cumulative_user_number +=1;

        tcflush(0, TCIFLUSH);
        *input->mode = 0;
        flag = 1;
        break;
    }
}

int makeUpNum(int num, int uprate){

	double res;
	int temp = ((num*uprate)/100) ;

	res = ((double)num * (double)uprate)/100;
	if(res>temp+ 0.5){
		
		if(temp+1 == num){
			return temp;
		}
		return temp+1;

	}
	else{
		if(temp == num){
			return temp-1;
		}
		return temp;
	}
}

char *inputFilter(int start_floor, int dest_floor, int floor){

	int mid_floor = floor/2;
	char *mode = malloc(sizeof(char)*10);
	mode[0] ='\0';

	strcat(mode,"1"); 

	//printf("mode : %s\n",mode);

	int dir = dest_floor - start_floor;

	if((start_floor == 1 && dest_floor%2 == 0) || (start_floor%2 ==0 && dest_floor == 1) || (dest_floor%2 == 0) && (start_floor%2 ==0)){
    	strcat(mode,"2"); 	
    }
    if((start_floor == 1 && dest_floor%2 == 1) || (start_floor%2 ==1 && dest_floor == 1) || (dest_floor%2 == 1) && (start_floor%2 ==1)){
    	strcat(mode,"3");
    }
    if(start_floor<mid_floor && dest_floor<mid_floor){
    	strcat(mode,"4");	
    }
    if(start_floor > mid_floor && dest_floor > mid_floor){
    	strcat(mode,"5");	
    }
    if(dir >0){
    	strcat(mode,"6");	
    }
    if(dir<0){
    	strcat(mode,"7");	
    }
    //strcat(mode,"8");
    //printf("mode : %s\n",mode);
    //exit(0);
    return mode;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void makeTest(){

	for(int i = 0 ;i < testCase ; i++){
				
		test[i].start_time = rand()%timeInterval;
		//printf("test[i].start_time : %d\n",test[i].start_time);
		//sleep(1000);
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
		//printf("test[i].go_Floor : %d\n",test[i].go_Floor);
	}
}

int compare_with_size(const void *a, const void  *b)
{
    Test_struct* ptr_a = (Test_struct*)a;
    Test_struct* ptr_b = (Test_struct*)b;
    
    if (ptr_a->start_time < ptr_b->start_time) return -1;
    else if (ptr_a->start_time == ptr_b->start_time) return 0;
    else return 1;
}

*/
int DBconector_testcase_pick(){

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
  	sprintf(query,"SELECT * FROM testcase;");

  	if(mysql_query(conn,query))
  	{

  	}

  	res = mysql_use_result(conn);

   	while((row = mysql_fetch_row(res)) != NULL){
		//printf("%s ",row[0]);

		test[testCount].start_time = atoi(row[0]);
		test[testCount].current_floor = atoi(row[1]);
		test[testCount].go_Floor = atoi(row[2]);
		testCount++;
	}


   mysql_free_result(res);
   mysql_close(conn);

}
