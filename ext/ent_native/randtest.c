/*

   Apply various randomness tests to a stream of bytes

      by John Walker  --  September 1996
           http://www.fourmilab.ch/


      Minor modifications made for use as a library
      in ruby bindings -- Eric Monti
*/

#include <math.h>
#include <stdlib.h>
#include <strings.h>
#include "randtest.h"

#define FALSE 0
#define TRUE  1

static double incirc; /* In-circle distance for Monte Carlo */

#define log2of10 3.32192809488736234787
static double rt_log2(double x)
{
    return log2of10 * log10(x);
}

void rt_init() {
    incirc = pow(pow(256.0, (double) (MONTEN / 2)) - 1, 2.0);
}

/*  RT_INIT  --  Initialise random test counters.  */
rt_ctx * rt_new()
{
    rt_ctx * ctx;

    if (ctx=(rt_ctx *) malloc(sizeof(rt_ctx))) {
      bzero(ctx, sizeof(rt_ctx));

      /* Initialise for calculations */
      ctx->sccfirst = TRUE;         /* Mark first time for serial correlation */
    }

    return ctx;
}

/*  RT_ADD  --  Add one or more bytes to accumulation.  */

void rt_add(rt_ctx * ctx, void *buf, size_t bufl)
{
    unsigned char *bp = buf;
    int oc, c, bean;

    while (bean = 0, (bufl-- > 0)) {
       oc = *bp++;

       do {
	  if (ctx->binary) {
	     c = !!(oc & 0x80);
	  } else {
	     c = oc;
	  }
	  ctx->ccount[c]++;		  /* Update counter for this bin */
	  ctx->totalc++;

	  /* Update inside / outside circle counts for Monte Carlo
	     computation of PI */

	  if (bean == 0) {
	      ctx->monte[ctx->mp++] = oc;       /* Save character for Monte Carlo */
	      if (ctx->mp >= MONTEN) {     /* Calculate every MONTEN character */
		 int mj;

		 ctx->mp = 0;
		 ctx->mcount++;
		 ctx->montex = ctx->montey = 0;
		 for (mj = 0; mj < MONTEN / 2; mj++) {
		    ctx->montex = (ctx->montex * 256.0) + ctx->monte[mj];
		    ctx->montey = (ctx->montey * 256.0) + ctx->monte[(MONTEN / 2) + mj];
		 }
		 if ((ctx->montex * ctx->montex + ctx->montey *  ctx->montey) <= incirc) {
		    ctx->inmont++;
		 }
	      }
	  }

	  /* Update calculation of serial correlation coefficient */

	  ctx->sccun = c;
	  if (ctx->sccfirst) {
	     ctx->sccfirst = FALSE;
	     ctx->scclast = 0;
	     ctx->sccu0 = ctx->sccun;
	  } else {
	     ctx->scct1 = ctx->scct1 + ctx->scclast * ctx->sccun;
	  }
	  ctx->scct2 = ctx->scct2 + ctx->sccun;
	  ctx->scct3 = ctx->scct3 + (ctx->sccun * ctx->sccun);
	  ctx->scclast = ctx->sccun;
	  oc <<= 1;
       } while (ctx->binary && (++bean < 8));
    }
}

/*  RT_END  --	Complete calculation and return results.  */

void rt_end(rt_ctx * ctx)
{
    int i;
    double ent, chisq, datasum, montepi, scc;
    ent = chisq = datasum = montepi = scc = 0.0;

    /* Complete calculation of serial correlation coefficient */

    ctx->scct1 = ctx->scct1 + ctx->scclast * ctx->sccu0;
    ctx->scct2 = ctx->scct2 * ctx->scct2;
    scc = ctx->totalc * ctx->scct3 - ctx->scct2;
    if (scc == 0.0) {
       scc = -100000;
    } else {
       scc = (ctx->totalc * ctx->scct1 - ctx->scct2) / scc;
    }

    /* Scan bins and calculate probability for each bin and
       Chi-Square distribution.  The probability will be reused
       in the entropy calculation below.  While we're at it,
       we sum of all the data which will be used to compute the
       mean. */
       
    /* Expected count per bin */
    ctx->cexp = ctx->totalc / (ctx->binary ? 2.0 : 256.0);  

    for (i = 0; i < (ctx->binary ? 2 : 256); i++) {
       double a = ctx->ccount[i] - ctx->cexp;;
       
       ctx->prob[i] = ((double) ctx->ccount[i]) / ctx->totalc;       
       chisq += (a * a) / ctx->cexp;
       datasum += ((double) i) * ctx->ccount[i];
    }

    /* Calculate entropy */

    for (i = 0; i < (ctx->binary ? 2 : 256); i++) {
       if (ctx->prob[i] > 0.0) {
	  ent += ctx->prob[i] * rt_log2(1 / ctx->prob[i]);
       }
    }

    /* Calculate Monte Carlo value for PI from percentage of hits
       within the circle */

    montepi = 4.0 * (((double) ctx->inmont) / ctx->mcount);

    /* Return results through arguments */

    ctx->r_ent = ent;
    ctx->r_chisq = chisq;
    ctx->r_mean = datasum / ctx->totalc;
    ctx->r_montepicalc = montepi;
    ctx->r_scc = scc;
    ctx->ended = 1;
}

void rt_free(rt_ctx *ctx) {
  free(ctx);
}
