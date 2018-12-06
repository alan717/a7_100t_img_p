#ifndef DATA_DEF_H

#pragma pack(1)
typedef struct {
	BYTE uid;
	char alph;
	int data;
}protocol_def_pack;
#pragma pack()


typedef struct {
	BYTE uid;
	char alph;
	int data;
}protocol_def_unpack;
#endif // !DATA_DEF_H
