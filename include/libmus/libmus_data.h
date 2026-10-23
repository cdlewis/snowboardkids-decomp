#ifndef LIBMUS_DATA_H
#define LIBMUS_DATA_H

/* Sequence opcode names from the libmus libmus_data.h interface.
 * Values match the command dispatcher used by this game.
 * Reference: https://github.com/AngheloAlf/puzzleleague64/blob/master/include/libmus/libmus_data.h
 */
#define Cstop 0x80
#define Cwave 0x81
#define Cport 0x82
#define Cportoff 0x83
#define Cdefa 0x84
#define Ctempo 0x85
#define Ccutoff 0x86
#define Cendit 0x87
#define Cvibup 0x88
#define Cvibdown 0x89
#define Cviboff 0x8A
#define Clength 0x8B
#define Cignore 0x8C
#define Ctrans 0x8D
#define Cignoretrans 0x8E
#define Cdistort 0x8F
#define Cadsr 0x90
#define Cenvoff 0x91
#define Cenvon 0x92
#define Ctroff 0x93
#define Ctron 0x94
#define Cfor 0x95
#define Cnext 0x96
#define Cwobble 0x97
#define Cwobbleoff 0x98
#define Cvelon 0x99
#define Cveloff 0x9A
#define Cvelocity 0x9B
#define Cpan 0x9C
#define Cstereo 0x9D
#define Cdrums 0x9E
#define Cdrumsoff 0x9F
#define Cprint 0xA0
#define Cgoto 0xA1
#define Creverb 0xA2
#define Crandnote 0xA3
#define Crandvolume 0xA4
#define Crandpan 0xA5
#define Cvolume 0xA6
#define Cstartfx 0xA7
#define Cbendrange 0xA8

#endif
