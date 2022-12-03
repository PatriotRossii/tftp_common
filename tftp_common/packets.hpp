#include <cstdint>
#include <string>
#include <vector>

namespace tftp_common {
	
namespace packets {


/*
      2 bytes    string   1 byte     string   1 byte
      -------------------------------------------------
RRQ/  | 01/02 |  Filename  |   0  |    Mode    |   0  |
WRQ   -------------------------------------------------
*/

struct prq {
	std::string filename;
	std::string mode;
};
using wrq = prq;


/*
      2 bytes    2 bytes       n bytes
      -----------------------------------
DATA  | 03    |   Block #  |    Data    |
      -----------------------------------
*/

struct data {
	uint16_t block;
	std::vector<std::byte> data;
};

/*
      2 bytes    2 bytes
      ----------------------
ACK   | 04    |   Block #  |
      ----------------------
*/

struct ack {
	uint16_t block;	
};

/*
      2 bytes  2 bytes        string    1 byte
      ------------------------------------------
ERROR | 05    |  ErrorCode |   ErrMsg   |   0  |
      ------------------------------------------
*/

struct error {
	uint16_t ErrorCode;
	std::string ErrMsg;
};

}

}
