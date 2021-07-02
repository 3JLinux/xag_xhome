#ifndef TPS_H
#define TPS_H

#include <stdint.h>

#define TPS_SEND_PORT	7770

#define XAP_PREFIX	0xC4C4c4c4

#define TPS_HEAD 0x1002

#define TPS_CMD_F_TRAN_START 			0x0501
#define TPS_CMD_F_TRAN_END				0x0502
#define TPS_CMD_F_TRAN_STATE			0x0503
#define TPS_CMD_F_TRAN_CANCEL			0x0504
#define TPS_CMD_F_TRAN_CLOUD_START		0x0505
#define TPS_CMD_F_TRAN_CLOUND_END		0x0506
#define TPS_CMD_M_START					0x0201
#define TPS_CMD_M_PAUSE					0x0202
#define TPS_CMD_M_RESUME				0x0203
#define TPS_CMD_M_GET_STATUS			0x0204
#define TPS_CMD_LANDING					0x0205
#define TPS_CMD_M_LOAD					0x0206
#define TPS_CMD_HOMEWARD				0X0207
#define TPS_CMD_SET_GOHOME_OPTION		0x0208
#define TPS_CMD_SET_ENTRY_OPTION		0x0209
#define TPS_CMD_M_CANCEL				0x020A
#define TPS_CMD_M_GET_ROUTE_STATUS		0x020B
#define TPS_CMD_SET_HOME_LOCATION		0x020C
#define TPS_CMD_M_CLEAN_BREAK_POINT		0x020D
#define TPS_CMD_M_CLEAN					0x020E
#define TPS_CMD_SET_FLIGHT_OPTION		0x020F
#define TPS_CMD_COPY2FLY_SWITCH			0x0301
#define TPS_CMD_COPY2FLY_FUNC_SWITC		0x0302
#define TPS_CMD_COPY2FLY_SET			0x0303
#define TPS_CMD_COPY2FLY_GET_STATUS		0x0304
#define TPS_CMD_MISSION_STATUS			0x8001
#define TPS_CMD_ROUTE_MISSION_STATUS	0x8002
#define TPS_CMD_FUNCTION_STATUS			0x8003
#define TPS_CMD_FLIGHT_SEGMENT_STATUS	0x8004
#define TPS_CMD_HOMEWARD_STATUS			0x8005

#define TPS_CMD_NETWORK_CHANGE			0x0410

enum FrameType {
  TYPE_NONE = 0,
  TYPE_REQUEST = 1,
  TYPE_RESPOND = 2,
  TYPE_MESSAGE = 3
};


typedef struct{
	uint32_t prefix;
	uint16_t meta;
	uint16_t sequence;
	uint16_t length;
	uint16_t checksum;
	uint8_t data[];
}XAP_FRAME;

typedef struct{
	uint32_t index;
	uint16_t segment;
	uint16_t flag;
	uint8_t target_id[16];
	int32_t longitude;
	int32_t latitude;
	int32_t height;
	uint32_t height_terrain;
	uint32_t speed;
	int32_t heading;

	uint8_t height_type;
	uint8_t height_source;
	uint8_t height_behavior;
	uint8_t heading_type;

	uint8_t oa_mode;
	uint8_t hold_behavior;
	uint8_t reserved0;
	uint8_t reserved1;
	uint32_t hold_time;

	int32_t LoiterLongitude;
	int32_t LoiterLatitude;
	uint16_t LoiterRadius;
	uint8_t LoiterDirection;
	uint8_t LoiterCircleCount;
}WayPoint;


typedef struct{
	uint8_t opt_path_optimization;
	uint8_t reserved1;
	uint8_t reserved2;
	uint8_t reserved3;
}Options;


typedef struct{
	uint32_t prefix;
	uint16_t version;

	uint16_t mission_source;
	uint16_t mission_type;
	uint16_t reserved0;

	uint32_t create_at;
	uint8_t project_id[16];
	uint8_t mission_id[16];
	uint8_t reserved[128];

	uint32_t options_legth;
	Options options;

	uint32_t waypoint_length;
	uint32_t waypoint_count;
	WayPoint waypoints[];
}Mission;


typedef struct{
	uint32_t id;
	uint16_t type;
	uint16_t reserved;
	int32_t latitude;
	int32_t longitude;
	int32_t alititude;
}field_point;


typedef struct{
	uint32_t index;
	uint16_t type;
	uint16_t reserved;
	uint8_t guid[16];
	uint32_t point_size;
	field_point points[];
}field_object;


typedef struct{
	uint32_t index;
	uint16_t type;
	uint16_t reserved;
	uint32_t object_size;
	field_object object[];
}field_collection;


typedef struct{
	uint32_t index;
	uint16_t typde;
	uint8_t guid[16];
	uint32_t create_at;
	uint8_t reserved[228];
}field_header;


typedef struct{
	field_header header;
	uint32_t collection_size;
	field_collection collections[];
}field;

/*File Transfer Start 0x0501*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t file_name_length;
	uint16_t reserved;
	uint8_t file_name[128];
	uint8_t file_hash[16];
	uint32_t file_size;
	uint32_t file_offset;
}tps_fTransfer_start_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t fid;
	uint16_t status;
	uint16_t error;
	uint16_t port;
}tps_fTransfer_start_ack_t;

/*File Transfer End 0x0502*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t fid;
	uint16_t status;
}tps_fTransfer_end_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t fid;
	uint16_t status;
}tps_fTransfer_end_ack_t;

/*File Transfer State 0x0503*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t fid;
	uint16_t reserved;
}tps_fTransfer_state_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t fid;
	uint16_t status;
	uint16_t error;
	uint16_t type;
	uint16_t prgress;
	uint16_t file_name_length;
	uint16_t file_name[128];
}tps_fTransfer_state_ack_t;

/*File Transfer Cancel 0x0504*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t fid;
	uint16_t reserved;
}tps_fTransfer_cancel_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t fid;
	uint16_t status;
}tps_fTransfer_cancel_ack_t;


/*File Transfer Cloud Start 0x0505*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t file_name_length;
	uint16_t reserved1;
	uint8_t file_name[128];
	uint8_t file_hash[16];

	uint16_t uri_size;
	uint16_t reserved2;
	uint8_t uri[];
}tps_fTransfer_cloud_start_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t fid;
	uint16_t status;
	uint16_t error;
	uint16_t port;
}tps_fTransfer_cloud_start_ack_t;


/*File Transfer Cloud End 0x0506*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t fid;
	uint16_t status;
	uint16_t error;
	uint16_t reserved;
}tps_fTransfer_cloud_end_ack_t;


/*Start Mission 0x0201*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint8_t mission_id[16];
	uint32_t mission_seq;
	uint16_t mission_type;
	uint16_t mission_source;
}tps_mission_start_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_mission_start_ack_t;


/*Pause Mission 0x0202*/
typedef struct{
	uint16_t head;
	uint16_t command;
}tps_mission_pause_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_mission_pause_ack_t;


/*Resume Mission 0x0203*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t mode;
	uint8_t mission_id[16];
	uint32_t mission_seq;
}tps_mission_resume_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_mission_resume_ack_t;


/*Get Mission Status 0x0204*/
typedef struct{
	uint16_t head;
	uint16_t command;
}tps_mission_get_status_t;


/*Landing 0x0205*/
typedef struct{
	uint16_t head;
	uint16_t command;
}tps_landing_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_landing_ack_t;


/*Load Misson 0x0206*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t mission_type;
	uint16_t mission_source;
	uint8_t mission_id[16];

	uint32_t file_name_length;
	uint8_t file_name[128];

	uint32_t field_name_length;
	uint8_t field_name[128];
}tps_mission_load_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_mission_load_ack_t;


/*Homeward 0x0207*/
typedef struct{
	uint16_t head;
	uint16_t command;
}tps_homeward_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_homeward_ack_t;


/*Set GoHome Option 0x0208*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t mode_options;
	int32_t height;
	uint32_t speed;
	uint8_t options[8];

	uint8_t reserved[32];
	uint32_t waypoint_size;
	WayPoint waypoints[];
}tps_homeward_set_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_homeward_set_ack_t;


/*Set Entry Option 0x0209*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint16_t mode;
	uint16_t mode_options;
	int32_t height;
	uint32_t speed;
	uint8_t options[8];
	uint8_t reserved2[32];
	uint32_t waypoint_size;
	WayPoint waypoints[];
}tps_entry_set_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_entry_set_ack_t;


/*Cancel Mission 0x020A*/
typedef struct{
	uint16_t head;
	uint16_t command;
}tps_mission_cancel_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_mission_cancel_ack_t;


/*Get Route Mission Status 0x020B*/
typedef struct{
	uint16_t head;
	uint16_t command;
}tps_mission_route_get_t;


/*Set Home Location 0x020C*/
typedef struct{
	uint16_t head;
	uint16_t command;

	int32_t home_latitude;
	int32_t home_longitude;
	int32_t home_altitude;
}tps_location_set_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_location_set_ack_t;


/*Clean Mission Break Point 0x020D*/
typedef struct{
	uint16_t head;
	uint16_t command;
}tps_mission_break_point_clean_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_mission_break_point_clean_ack_t;


/*Clean Mission 0x020E*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint8_t mission_id[16];
	uint32_t mission_seq;
	uint16_t mission_type;
	uint16_t mission_source;
}tps_mission_clean_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_mission_clean_ack_t;


/*Set Flight Option 0x020F*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint8_t oa_enable;
	uint8_t oa_param;
	uint8_t terrain_enable;
	uint8_t terrain_level;

	uint8_t set_height_offset;
	uint8_t reserved1;
	int16_t height_offset;

	uint8_t set_terrain_protect;
	uint8_t reserved2;
	uint16_t protect_height;

	uint8_t reserved[4];
}tps_flight_option_set_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint8_t oa_enable;
	uint8_t oa_param;
	uint8_t terrain_enable;
	uint8_t terrain_level;
	
	int16_t height_offset;
	int16_t protect_height;

	uint8_t reserved[4];
}tps_flight_option_set_ack_t;


/*Copy to fly switch 0x0301*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t enable;
}tps_copy2flay_sw_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_copy2flay_sw_ack_t;


/*Copy to fly function switch 0x0302*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t functions;
	uint32_t enable;
}tps_copy2fly_func_sw_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_copy2fly_func_sw_ack_t;


/*Copy to fly set 0x0303*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t terrain_level;
}tps_copy2fly_set_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_copy2flay_set_ack_t;


/*Copy to fly status 0x0304*/
typedef struct{
	uint16_t head;
	uint16_t command;
}tps_copy2flay_status_t;


typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t terrain_enabled;
	uint32_t terrain_functions;

	uint32_t terrain_device;
	uint32_t terrain_used;
	uint32_t terrain_height;
	uint32_t terrain_level;
}tps_copy2flay_status_ack_t;


/*Mossion Status 0x8001*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint8_t mission_id[16];
	uint32_t mission_seq;
	uint16_t mission_type;
	uint16_t mission_source;
	uint32_t mission_event;
	uint32_t mission_event_agrs;
	uint32_t mission_time_start;
	uint32_t mission_time_used;
}tps_mission_status_t;


/*Route Mission Status 0x8002*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint8_t mission_id[16];
	uint32_t mission_seq;
	uint16_t mission_type;
	uint16_t mission_source;
	uint32_t mission_state;
	uint32_t mission_event;
	uint32_t mission_event_agrs;
	uint32_t mission_time_start;
	uint32_t mission_time_used;

	uint32_t wp_index;
	uint32_t wp_cout;
	uint16_t wp_segment;
	uint16_t wp_flag;
	uint8_t wp_target[16];

	int32_t takeoff_latitude;
	int32_t takeoff_longitude;
	int32_t takeoff_altitude;

	int32_t home_latitude;
	int32_t home_longitude;
	int32_t home_altitude;

	uint16_t break_state;
	uint16_t break_event;
	uint32_t break_occur_time;
	uint32_t break_wp_index;
	int32_t break_latitude;
	int32_t break_longitude;
	int32_t break_altitude;

	uint32_t entry_index;
	uint32_t entry_count;
	uint32_t work_index;
	uint32_t work_count;
	uint32_t home_index;
	uint32_t home_count;
}tps_mission_route_status_t;


/*Function Status 0x8003*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint32_t terrain_enabled;
	uint32_t terrain_function;

	uint32_t terrain_device;
	uint32_t terrain_used;
	uint32_t terrain_height;
	uint32_t terrain_level;

	uint32_t protect_enable;
}tps_function_status_t;


/*Flight Segment Status 0x8004*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint8_t type;
	uint8_t rev[3];

	uint16_t mode;
	uint16_t mode_options;

	uint32_t current_index;
	uint32_t waypoint_size;
	WayPoint waypoints[];
}tps_flight_segment_status_t;


/*Homeward Status 0x8005*/
typedef struct{
	uint16_t head;
	uint16_t command;

	uint8_t is_return_home;
	uint8_t return_home_source;
	uint8_t rev[2];
}tps_homeward_status_t;


typedef struct 
{
	uint16_t mission_type;
	uint16_t mission_source;
	uint8_t mission_id[16];

	uint32_t file_name_length;
	uint8_t file_name[128];

	uint32_t field_name_length;
	uint8_t field_name[128];
}tps_mission_rcv_t;


typedef struct 
{
	uint16_t head;
	uint16_t command;

	uint8_t net_type;
	uint8_t rev[3];
	uint32_t ip;
	uint16_t port;
	uint16_t rev1;
}tps_change_network_t;


typedef struct 
{
	uint16_t head;
	uint16_t command;

	uint32_t status;
}tps_change_network_ack_t;




int32_t TPS_sendData(void (*tps_send)(char* data, int32_t dataLen), 
        uint16_t cmd);









#endif
