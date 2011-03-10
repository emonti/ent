
/*  Random test function prototypes  */

typedef struct rt_ctx;

extern void rt_init();
extern rt_ctx * rt_new(int binmode);
extern void rt_add(rt_ctx *ctx, void *buf, int bufl);
extern void rt_end(rt_ctx *ctx, 
                   double *r_ent, double *r_chisq, double *r_mean, 
                   double *r_montepicalc, double *r_scc);
