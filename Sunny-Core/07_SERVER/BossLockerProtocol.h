#pragma once

#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255

#define BOARD_WIDTH      800
#define BOARD_HEIGHT     800

#define VIEW_RADIUS        3

#define MAX_USER		   3

#define CHARACTER_14      14
#define CHARACTER_15      15
#define CHARACTER_20      20


#define MY_SERVER_PORT  7711

#define MAX_STR_SIZE     100

#define CS_PLAYER_TYPE     0
#define CS_PLAYER_MAP      1
#define CS_PLAYER_READY    2
#define CS_PLAYER_START    3
#define CS_PLAYER_MOVE     4

#define SC_PLAYER_PUT      0
#define SC_PLAYER_REMOVE   1
#define SC_PLAYER_TYPE     2
#define SC_PLAYER_MAP      3
#define SC_PLAYER_READY    4
#define SC_PLAYER_START    5
#define SC_PLAYER_POS      6

#define SC_CHAT            4

#define MOVE_LEFT          0
#define MOVE_UP            1
#define MOVE_RIGHT         2
#define MOVE_DOWN          3

#define MAP_PLANE          0
#define MAP_ICE            1
#define MAP_DESERT         2


#pragma pack (push, 1)
// 현재 정렬 값을 push하고 n짜리 정렬값으로 바꾼다. 
// 즉 push는 현재 값을 저장하는 키워드이고 n은 새 정렬값을 부여하는 키워드인 것이다
// 1 바이트 정렬 (바이트 패딩)

struct cs_packet_player_type
{
	unsigned char size;
	unsigned char type;
	unsigned char player_type;
};

struct cs_packet_player_map
{
	unsigned char size;
	unsigned char type;
	unsigned char map;
};

struct cs_packet_player_ready
{
	unsigned char size;
	unsigned char type;
};

struct cs_packet_player_start
{
	unsigned char size;
	unsigned char type;
};

struct cs_packet_player_move
{
	unsigned char size;
	unsigned char type;
	unsigned char dir;
};


struct sc_packet_player_put
{
	unsigned char  size;
	unsigned char  type;
	unsigned char  id;
	unsigned char  player_type;
	bool           isMaster;
	bool           isReady;
	unsigned char  map;
};

struct sc_packet_player_type
{
	unsigned char  size;
	unsigned char  type;
	unsigned char  id;
	unsigned char  player_type;
};

struct sc_packet_player_map
{
	unsigned char  size;
	unsigned char  type;
	unsigned char  id;
	unsigned char  map;
};

struct sc_packet_player_ready
{
	unsigned char  size;
	unsigned char  type;
	unsigned char  id;
	bool           isReady;
};

struct sc_packet_player_start
{
	unsigned char  size;
	unsigned char  type;
	unsigned char  id;
	bool           start;
};

struct sc_packet_pos
{
	unsigned char size;
	unsigned char type;
	unsigned short id;
	unsigned short x;
	unsigned short y;
};



struct sc_packet_player_remove
{
	unsigned char size;
	unsigned char type;
	unsigned short id;
};

#pragma pack (pop)