void DB_Elevator_updater(int building_id, int Elevator_Id, int current_floor);
void DB_Calling_updater(char *userID, int Time, int elevator_id);
void DB_Flag_updater(char *userID);
void DB_Flag2_updater(char *userID);
int DBconector_floor(int id);
int DBconector_ele_num(int id);
int DBconector_flag();
void DB_People_Num_updater(int building_id, int Elevator_Id, int people_num);
