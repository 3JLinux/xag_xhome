#ifndef _CAN_COM_H_
#define _CAN_COM_H_

#include <stdint.h>
#include <cstring>

namespace xag_can{
namespace canbus{

	typedef union
	{
		uint8_t id_data[12];
		struct
		{
			uint32_t id;
			uint8_t  data[8];  
			/* data */
		}messages;
		
		/* data */
	}/*__attribute__((packed)) */Can_un;

	typedef enum class ErrorCode
	{
		OK = 0,
		SOCKCAN_ERROR_CREATE_ERROR = 101,
		SOCKCAN_ERROR_FILTER_ERROR = 102,
		SOCKCAN_ERROR_RECV_ERROR = 103,
		SOCKCAN_ERROR_IOCTL_ERROR = 104,
		SOCKCAN_ERROR_BIND_ERROR = 105,
		SOCKCAN_ERROR_CLOSE_ERROR = 106,
	}ErrorCode;

	typedef enum class SAB_CAN_ID{

		SUPERX4_0x100 = 0x100,
		SAB_ID_0X280 = 0X280,
		SAB_ID_0X281 = 0X281,
		SAB_ID_0X282 = 0X282,
		SAB_ID_0X283 = 0X283,
		SAB_ID_0X284 = 0X284,
		SAB_ID_0X285 = 0X285,
		SAB_ID_0X286 = 0X286,
		SAB_ID_0X287 = 0X287,
		SAB_ID_0X290 = 0X290,
		SAB_ID_0X291 = 0X291,
		SAB_ID_0X292 = 0X292,
		SAB_ID_0X293 = 0X293,
		SAB_ID_0X294 = 0X294,
		SAB_ID_0X295 = 0X295,
		SAB_ID_0X296 = 0X296,
		SAB_ID_0X297 = 0X297,
		SAB_ID_0X300 = 0X300,
		SAB_ID_0X301 = 0X301,
		SAB_ID_0X302 = 0X302,
		SAB_ID_0X303 = 0X303,
		SAB_ID_0X304 = 0X304,
		SAB_ID_0X305 = 0X305,
		SAB_ID_0X306 = 0X306,
		SAB_ID_0X307 = 0X307,
		SAB_ID_0X308 = 0X308,
		SAB_ID_0X309 = 0X309,
		SAB_ID_0X310 = 0X280,
		SAB_ID_0X320 = 0X320,
		SAB_ID_0X321 = 0X321,
		SAB_ID_0X322 = 0X322,
	}SAB_CAN_ID;

}
}



#endif // !1