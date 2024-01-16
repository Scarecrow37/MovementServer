#pragma once
#ifndef __PACKET_H__
#define __PACKET_H__

enum Type
{
    Location = 10,
    Move = 20,
    MAX
};

#pragma pack(push, 1)

typedef struct header
{
    unsigned short Length;
    unsigned short Type;
} HEADER;

typedef struct location
{
    unsigned short X;
    unsigned short Y;
} LOCATION;

#pragma pack(pop)

#endif
