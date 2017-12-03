/*
* 실행할때는 ./InputMaeker (만드려는 사람수) (이용시간 초단위) (건물의 높이) (올라가는사람비율)
* 이렇게 입력을 해서 실행합니다.
*
* 출력순서 : (탑승시간) (현재층) (목적층) 
* ex) ./InputMaker 100 180 10 100
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ASCII 48
#define MAX 100000

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



int main(int argc, char **argv)
{

	int inputNum = argc -1;
	int testCase = atoi(argv[1]); //-ASCII;
	int timeInterval = atoi(argv[2]);
	int Building_Floor = atoi(argv[3]);
	int upRate = atoi(argv[4]);
	
	int start_time, current_floor, go_Floor, re_start_time, re_go_Floor; 
	char result[100] ="";
	
	Test_struct test[MAX];

	FILE *f;
	int upNum; 


	if(upRate == 0){
		upNum = 0;

	}
	else{

		upNum= testCase * (upRate)/100;

	}

	f=fopen("testCase.txt" , "w");
	srand(time(NULL));

	for(int i = 0 ;i < testCase ; i++){
		start_time = rand()%timeInterval;
		test[i].start_time = start_time;

		if(i < upNum){//올라가는 비율만큼 만든다.
			current_floor = rand()%Building_Floor;
			test[i].current_floor = current_floor;

			while(1){
				if(current_floor == 0){
					current_floor = rand()%Building_Floor;
					test[i].current_floor = current_floor;
				}
				else break;

			}
			go_Floor = rand()%(Building_Floor- current_floor) + current_floor +1;
			test[i].go_Floor = go_Floor;
		}
		else{
			current_floor = rand()%Building_Floor +1;
			test[i].current_floor = current_floor;

			while(1){
				if(current_floor == 1 || current_floor == 0){
					current_floor = rand()%Building_Floor;
					test[i].current_floor = current_floor;
			
				}
				else break;
			}

			while(1){
				go_Floor = rand()%(current_floor);
				test[i].go_Floor = go_Floor;
				if(go_Floor == 0){
					go_Floor = rand()%(current_floor);
					test[i].go_Floor = go_Floor;
				
				}
				else break;
			}

		}
		

		
	}

	qsort(test, testCase, sizeof(Test_struct), compare_with_size);


	for(int i = 0 ;i < testCase ; i++){
		sprintf(result,"%d %d %d\n",test[i].start_time, test[i].current_floor, test[i].go_Floor );
		fprintf(f,"%s",result);
	}
	fclose(f);

	//f=fopen("testCase.txt" , "rw");2
}