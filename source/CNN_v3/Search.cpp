#include "NN.h"

void search_element(int gie, GlobalMesh  *global, double pre_x, double pre_y, double pre_z, int* result){
  int i, dummy, t_elem=global->t_elem, inop, oldgie = gie, newgie = gie, nel = global->nel; 
  double lam1, lam2, lam3, lam4, eps = 1.0e-3, pnod_x[4], pnod_y[4], pnod_z[4];
  
    /*-  search  -*/
    for(dummy=0; dummy<nel; dummy++){ 
        for (i=0;i<t_elem;i++){
      	  inop = global->nop[newgie * t_elem + i];
          pnod_x[i] = global->crd[inop*3];
          pnod_y[i] = global->crd[inop*3 + 1];
          pnod_z[i] = global->crd[inop*3 + 2];
          oldgie = newgie;
          }
      brycntr(pre_x, pre_y, pre_z, pnod_x, pnod_y, pnod_z, &lam1, &lam2, &lam3, &lam4 );
      if ( (lam1 >= -eps) && (lam2 >= -eps) && (lam3 >= -eps) && (lam4 >= -eps) ){
        result[0] = dummy;
        result[1] = newgie;
        return;
      }   
      else if ( (lam1 < -eps) && (lam1 <= lam2) && (lam1 <= lam3) && (lam1 <= lam4) && (global->elel[newgie * t_elem] != -1) ) newgie=global->elel[newgie * t_elem];
      else if ( (lam2 < -eps) && (lam2 <= lam1) && (lam2 <= lam3) && (lam2 <= lam4) && (global->elel[newgie * t_elem+1] != -1) ) newgie=global->elel[newgie * t_elem+1];
      else if ( (lam3 < -eps) && (lam3 <= lam1) && (lam3 <= lam2) && (lam3 <= lam4) && (global->elel[newgie * t_elem+2] != -1) ) newgie=global->elel[newgie * t_elem+2]; 
      else if ( (lam4 < -eps) && (lam4 <= lam1) && (lam4 <= lam2) && (lam4 <= lam3) && (global->elel[newgie * t_elem+3] != -1) ) newgie=global->elel[newgie * t_elem+3];
      else break;

    }// End of dummy loop
    result[0] = dummy;
    result[1] = newgie;
    return;
}

void brycntr(double xx, double yy, double zz, double x[], double y[], double z[], double *lam1, double *lam2, double *lam3, double *lam4 ){
  int i,j,k,l; 
  double s, a[4], b[4], c[4], d[4];

	for(i = 0; i < 4 ;i++){
		if(i == 0){
			j = 1;
			k = 2;
			l = 3;
		}
		else if(i == 1){
			j = 3;
			k = 2;
			l = 0;
		}
		else if(i == 2){
			j = 0;
			k = 1;
			l = 3;
		}
		else if(i == 3){
			j = 2;
			k = 1;
			l = 0;
		}
	a[i] = x[j]*y[k]*z[l] + y[j]*z[k]*x[l] + z[j]*x[k]*y[l] - z[j]*y[k]*x[l] - y[j]*x[k]*z[l]- x[j]*z[k]*y[l];
	b[i] = -( y[k]*z[l] + y[j]*z[k] + z[j]*y[l] - z[j]*y[k] - y[j]*z[l] - z[k]*y[l] );
	c[i] = x[k]*z[l] + x[j]*z[k] + z[j]*x[l] - z[j]*x[k] - x[j]*z[l] - z[k]*x[l];
	d[i] = -( x[k]*y[l] + x[j]*y[k] + y[j]*x[l] - y[j]*x[k] - x[j]*y[l] - y[k]*x[l] );
//    printf("a[%d]:%f, b[%d]:%f, c[%d]:%f, d[%d]:%f\n",i,i,i,i,a[i],b[i],c[i],d[i]);
//    printf("x[%d]:%f, y[%d]:%f, z[%d]:%f\n",i,i,i,x[i],y[i],z[i]);

	}

    s =   x[1]*y[2]*z[3] + y[1]*z[2]*x[3] + z[1]*x[2]*y[3] - z[1]*y[2]*x[3] - y[1]*x[2]*z[3] - x[1]*z[2]*y[3]  
  	    -(x[0]*y[2]*z[3] + y[0]*z[2]*x[3] + z[0]*x[2]*y[3] - z[0]*y[2]*x[3] - y[0]*x[2]*z[3] - x[0]*z[2]*y[3])
  	    + x[0]*y[1]*z[3] + y[0]*z[1]*x[3] + z[0]*x[1]*y[3] - z[0]*y[1]*x[3] - y[0]*x[1]*z[3] - x[0]*z[1]*y[3]
  	    -(x[0]*y[1]*z[2] + y[0]*z[1]*x[2] + z[0]*x[1]*y[2] - z[0]*y[1]*x[2] - y[0]*x[1]*z[2] - x[0]*z[1]*y[2]);
  	
    
    *lam1 = (a[0] + b[0]*xx + c[0]*yy + d[0]*zz)/s; 
    *lam2 = (a[1] + b[1]*xx + c[1]*yy + d[1]*zz)/s; 
    *lam3 = (a[2] + b[2]*xx + c[2]*yy + d[2]*zz)/s; 
    *lam4 = (a[3] + b[3]*xx + c[3]*yy + d[3]*zz)/s; 
    return;
    
}

void Splitter(int range, int nproc, int *start, int *end ){
  int i, band;
  /* Divide the range */
  band = (range)/nproc;
  if ((range)-(nproc * band)){//remainder
    for (i = 0; i <(range)-(nproc * band); i++){
        start[i] = i * (band+1);
        end[i]   = start[i] + band + 1;
       }
    for (i = (range)-(nproc * band); i < nproc; i++){
        start[i] = end[i-1];
        end[i]   = start[i] + band;
      }
  }
  else{
    for (i = 0; i < nproc; i++){
       start[i] = i*band;
       end[i]   = start[i] + band;
    }  
  }
  
}