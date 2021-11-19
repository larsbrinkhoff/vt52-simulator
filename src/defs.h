extern void step (void);
extern unsigned char ROM[1024];
extern void trace (const char *);
extern void timing (void);

extern void vcd_value (int index, int value);
extern void vcd_start (const char *file);

extern int scan, fly, horiz, vert, tos;
extern long long cycles;


#define INDEX_E2_74164   0
#define INDEX_E27_74161  1
#define INDEX_E18_7490   2
#define INDEX_E13_7493   3
#define INDEX_E11_7493   4
#define INDEX_E5_74161   5
#define INDEX_E6_74161   6
#define INDEX_HORIZ      7
#define INDEX_SCAN       8
#define INDEX_FLY        9
#define INDEX_VERT       10
#define INDEX_TOS        11
