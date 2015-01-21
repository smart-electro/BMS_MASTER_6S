struct Cells_statuses{
	unsigned  CELL1_ROK : 1;
	unsigned  CELL2_ROK : 1;
	unsigned  CELL3_ROK : 1;
	unsigned  CELL4_ROK : 1;
	unsigned  CELL5_ROK : 1;
	unsigned  CELL6_ROK : 1;
	unsigned  ALL_ROK : 1;
	unsigned  : 1;
};
typedef struct Cells_statuses CELL_STAT;

/*
enum {
  CELL1_ROK  = (1 << 0)
  CELL2_ROK  = (1 << 1),
  CELL3_ROK  = (1 << 2),
  CELL4_ROK  = (1 << 3),
  CELL5_ROK  = (1 << 4),
  CELL6_ROK  = (1 << 5),
};
typedef uint32_t Cells_status;
*/

#define NO_CELLS 8
struct BatteryCell{ 
	char Voltage; 
	char PWM; 
	char MaxVoltage;
	char MinVoltage;
	char MaxBalancing;
};
typedef struct BatteryCell BATT_CELL;

void readBmsSlaves();

void NaslednjiPort();


