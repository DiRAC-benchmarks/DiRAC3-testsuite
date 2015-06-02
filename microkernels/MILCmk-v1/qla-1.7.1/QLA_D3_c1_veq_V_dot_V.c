/**************** QLA_D3_c_veq_V_dot_V.c ********************/

#include <stdio.h>
#include <qla_config.h>
#include <qla_types.h>
#include <qla_random.h>
#include <qla_cmath.h>
#include <qla_d3.h>
#include <math.h>

void QLA_D3_c_veq_V_dot_V ( QLA_D_Complex *restrict r, QLA_D3_ColorVector *restrict a, QLA_D3_ColorVector *restrict b, int n)
{
#ifdef HAVE_XLC
#pragma disjoint(*r,*a,*b)
  __alignx(16,r);
  __alignx(16,a);
  __alignx(16,b);
#endif
  QLA_D_Complex sum;
  QLA_c_eq_r(sum,0.);
#pragma omp parallel
  {
    QLA_D_Complex sum_local;
    QLA_c_eq_r(sum_local,0.);
#pragma omp for
    for(int i=0; i<n; i++) {
      for(int i_c=0; i_c<3; i_c++) {
        QLA_c_peq_ca_times_c(sum_local, QLA_D3_elem_V(a[i],i_c), QLA_D3_elem_V(b[i],i_c));
      }
    }
#pragma omp critical
    {
      QLA_c_peq_c(sum,sum_local);
    }
  }
  QLA_c_eq_c(*r,sum);
}
