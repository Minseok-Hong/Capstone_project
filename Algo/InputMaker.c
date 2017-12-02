/*
* 실행할때는 ./InputMaeker (만드려는 사람수) (이용시간 초단위) (건물의 높이) (올라가는사람비율)
* 이렇게 입력을 해서 실행합니다.
*
* 출력순서 : (탑승시간) (현재층) (목적층) (재탑승 시간) (그후 목적층)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ASCII 48

int main(int argc, char **argv)
{

	int inputNum = argc -1;
	int testCase = atoi(argv[1]); //-ASCII;
	int timeInterval = atoi(argv[2]);
	int Building_Floor = atoi(argv[3]);
	int upRate = atoi(argv[4]);
	
	int start_time, current_floor, go_Floor, re_start_time, re_go_Floor; 
	char result[100] ="";

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

		if(i < upNum){//올라가는 비율만큼 만든다.
			current_floor = rand()%Building_Floor;

			while(1){
				if(current_floor == 0){
					current_floor = rand()%Building_Floor;
				}
				else break;

			}
			go_Floor = rand()%(Building_Floor- current_floor) + current_floor +1;

		}
		else{
			//printf("DOWN_CNT : %d\n",i+1);
			current_floor = rand()%Building_Floor +1;
			//printf("DOWN_1\n");
			while(1){
				if(current_floor == 1 || current_floor == 0){
					current_floor = rand()%Building_Floor;
				}
				else break;
			}

			while(1){
				go_Floor = rand()%(current_floor);
				if(go_Floor == 0){
					go_Floor = rand()%(current_floor);
				}
				else break;
			}

		}
		

		sprintf(result,"%d %d %d\n",start_time, current_floor, go_Floor );
		fprintf(f,"%s",result);
	}

	
	fclose(f);

}