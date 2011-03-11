#define MONTEN  6          /* Bytes used as Monte Carlo co-ordinates.  
                              This should be no more bits than the mantissa
                              of your "double" floating point type. */


typedef struct {
  int    binary;              /* use byte(0) or binary(1) mode ? */
  int    sccfirst;            /* first time for serial correlation ? */
  size_t totalc;              /* Total bytes counted */
  int    mp;                  /* Monte Carlo accumulator pointer */
  size_t inmont;              /* Monte Carlo inside count */
  size_t mcount;              /* Monte Carlo tries */
  double cexp;
  double montex;
  double montey;
  double sccun;
  double sccu0;
  double scclast;
  double scct1;               /* serial correlation term 1 */
  double scct2;               /* serial correlation term 2 */
  double scct3;               /* serial correlation term 3 */
  unsigned int monte[MONTEN]; /* Monte Carlo co-ordinates */
  size_t ccount[256];         /* Bins to count occurrences of values */
  double prob[256];           /* Probabilities per bin for entropy */

  int    ended;
  double r_ent;
  double r_mean;
  double r_chisq;
  double r_datasum;
  double r_montepicalc;
  double r_scc;
} rt_ctx;


