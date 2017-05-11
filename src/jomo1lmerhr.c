#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "pdflib.h"
#include "wishart.h"
#include<R.h>
#include<Rinternals.h>
#include<Rmath.h>

SEXP jomo1lmerhr(SEXP Ysub, SEXP Ysubimp, SEXP submod, SEXP ordersub, SEXP submodran, SEXP Y, SEXP Yimp, SEXP Yimp2, SEXP Yimpcat, SEXP X, SEXP Z, SEXP clus, SEXP betaY, SEXP betaYpost, SEXP beta, SEXP u, SEXP uY, SEXP betapost, SEXP upost, SEXP uYpost, SEXP varY, SEXP varYpost, SEXP omega, SEXP omegapost, SEXP covuY, SEXP covuYpost, SEXP covu, SEXP covupost, SEXP nstep, SEXP varYprior, SEXP covuYprior, SEXP Sp, SEXP Sup, SEXP Y_numcat, SEXP num_con, SEXP a_start, SEXP a_prior, SEXP flagrng){
int indic=0,i,j,k, IY,JY, IX, JX, Io, Jo, Ib, Jb, ns, nmiss=0,t, countm=0, counto=0, countoo=0, jj, tt, kk, ncon,ncat, pos,flag=0,nmaxx,h=0;
int Iu, Ju, IZ, JZ, nj,c,fl, currncat, Is, Il=0, JXm, Ir=0,Jr, JZm, Jum, accratio=0, totprop=0;
SEXP RdimY, RdimX, Rdimo, Rdimb, RdimZ, Rdimu, Rdims, Rdimr;
double *betaX, *Yobs, *Ymiss, *mumiss, *omegadrawmiss, *betamiss, *betaobs, *omegaoo, *omegamo, *omegamm, *invomega, *invomega2, *help, *help2, *help3, *imp, *zi, *yicategorized, *impsub;
double *sumzy, *incrzz, *incrzy, *mu, *mu2, *newbeta, *newomega, *sumzi, *yi, *invomega3, *help4, *help5, *help6, *missing, *fixomega,meanom,sdom, *resid, logLH, newlogLH,detom, *residsub;
double maxx,maxim,maxim2, *sumxy, *sumxi, *uj, *xi, *ziu, *incrxx, *incrxy, *newu, *mu3,  *mu4, *help7, *newomega2, *Xsub, *Zsub, *Xsubprop, *Zsubprop, *allinvomega;
double gamma,eta,dx,u_new,precision, *invgamma, *invA, *Gammastar,u_m,con2,deriv2,u_prop, lambda, aj, a, *cumclus, *clusnum;

/* Protecting R objects from garbage collection and saving matrices dimensions*/ 

RdimY=PROTECT(getAttrib(Yimp,R_DimSymbol));
IY=INTEGER(RdimY)[0];
JY=INTEGER(RdimY)[1];
Rdims=PROTECT(getAttrib(submod,R_DimSymbol));
Is=INTEGER(Rdims)[0];
Rdimr=PROTECT(getAttrib(submodran,R_DimSymbol));
Jr=INTEGER(Rdimr)[1];
RdimX=PROTECT(getAttrib(X,R_DimSymbol));
IX=INTEGER(RdimX)[0];
JX=INTEGER(RdimX)[1];
RdimZ=PROTECT(getAttrib(Z,R_DimSymbol));
IZ=INTEGER(RdimZ)[0];
JZ=INTEGER(RdimZ)[1];
Rdimb=PROTECT(getAttrib(beta,R_DimSymbol));
Ib=INTEGER(Rdimb)[0];
Jb=INTEGER(Rdimb)[1];
Rdimo=PROTECT(getAttrib(omega,R_DimSymbol));
Io=INTEGER(Rdimo)[0];
Jo=INTEGER(Rdimo)[1];
Rdimu=PROTECT(getAttrib(u,R_DimSymbol));
Iu=INTEGER(Rdimu)[0];
Ju=INTEGER(Rdimu)[1];
Ysub=PROTECT(coerceVector(Ysub,REALSXP));
submod=PROTECT(coerceVector(submod,INTSXP));
ordersub=PROTECT(coerceVector(ordersub,INTSXP));
submodran=PROTECT(coerceVector(submodran,INTSXP));
Ysubimp=PROTECT(coerceVector(Ysubimp,REALSXP));
Y=PROTECT(coerceVector(Y,REALSXP));
Yimpcat=PROTECT(coerceVector(Yimpcat,REALSXP));
Y_numcat=PROTECT(coerceVector(Y_numcat,INTSXP));
Yimp=PROTECT(coerceVector(Yimp,REALSXP));
Yimp2=PROTECT(coerceVector(Yimp2,REALSXP));
X=PROTECT(coerceVector(X,REALSXP));
Z=PROTECT(coerceVector(Z,REALSXP));
clus=PROTECT(coerceVector(clus,INTSXP));
beta=PROTECT(coerceVector(beta,REALSXP));
betaY=PROTECT(coerceVector(betaY,REALSXP));
betapost=PROTECT(coerceVector(betapost,REALSXP));
betaYpost=PROTECT(coerceVector(betaYpost,REALSXP));
varY=PROTECT(coerceVector(varY,REALSXP));
varYpost=PROTECT(coerceVector(varYpost,REALSXP));
omega=PROTECT(coerceVector(omega,REALSXP));
omegapost=PROTECT(coerceVector(omegapost,REALSXP));
varYprior=PROTECT(coerceVector(varYprior,REALSXP));
u=PROTECT(coerceVector(u,REALSXP));
uY=PROTECT(coerceVector(uY,REALSXP));
upost=PROTECT(coerceVector(upost,REALSXP));
uYpost=PROTECT(coerceVector(uYpost,REALSXP));
covuY=PROTECT(coerceVector(covuY,REALSXP));
covu=PROTECT(coerceVector(covu,REALSXP));
covuYpost=PROTECT(coerceVector(covuYpost,REALSXP));
covupost=PROTECT(coerceVector(covupost,REALSXP));
covuYprior=PROTECT(coerceVector(covuYprior,REALSXP));
Sp=PROTECT(coerceVector(Sp,REALSXP));
Sup=PROTECT(coerceVector(Sup,REALSXP));
nstep=PROTECT(coerceVector(nstep,INTSXP));
ns=INTEGER(nstep)[0];
num_con=PROTECT(coerceVector(num_con,INTSXP));
ncon=INTEGER(num_con)[0];
flagrng=PROTECT(coerceVector(flagrng,INTSXP));
fl=INTEGER(flagrng)[0];
if (REAL(Yimpcat)[0]==(-999)) ncat=0;
else ncat=length(Y_numcat);
a_start=PROTECT(coerceVector(a_start,REALSXP));
a=REAL(a_start)[0];
a_prior=PROTECT(coerceVector(a_prior,REALSXP));
nj=Iu;

for (i=0;i<XLENGTH(ordersub);i++) {
	for (j=0;j<INTEGER(ordersub)[i];j++) {
		maxx=maxx*(INTEGER(submod)[3+4*h]);
		h++;
	}
	Il=Il+maxx;
	maxx=1;
}
Il=Il+1;
JXm=JY;
if (Il>JY) JXm=Il;
if (JX>JXm) JXm=JX;

for (i=0;i<Jr;i++) {			
	Ir=Ir+(INTEGER(submodran)[2+3*i]);
}
JZm=JY;
if (Ir>JY) JZm=Ir;
if (JZ>JZm) JZm=JZ;

Jum=Ju;
if (Ir>Jum) Jum=Ir;

/*Allocating memory for C objects in R*/

help = ( double * ) R_alloc ( (Ju*Ju) , sizeof ( double ) );
invomega= (double * ) R_alloc ( (JY*JY) , sizeof ( double ) );
fixomega = ( double * ) R_alloc ( JY * JY , sizeof ( double ) );
sumzi = ( double * ) R_alloc ( Jum * Jum , sizeof ( double ) );
sumzy = ( double * ) R_alloc ( JY * JZm , sizeof ( double ) );
sumxi = ( double * ) R_alloc ( JY * JXm * JY * JXm , sizeof ( double ) );
sumxy = ( double * ) R_alloc ( JY * JXm , sizeof ( double ) );
zi = ( double * ) R_alloc ( JY * JZm * JY , sizeof ( double ) );
xi = ( double * ) R_alloc ( JY * JXm * JY , sizeof ( double ) );
yi = ( double * ) R_alloc ( JY , sizeof ( double ) );
uj = ( double * ) R_alloc ( Jum * Jum , sizeof ( double ) );
newu = ( double * ) R_alloc ( JY * JZm , sizeof ( double ) );
ziu = ( double * ) R_alloc ( JY , sizeof ( double ) );
help2 = ( double * ) R_alloc ( JY *JX * Ju , sizeof ( double ) );
incrzz = ( double * ) R_alloc ( Jum * Jum, sizeof ( double ) );
incrzy = ( double * ) R_alloc ( JY *JZm , sizeof ( double ) );
incrxx = ( double * ) R_alloc ( JY *JXm * JY *JXm , sizeof ( double ) );
incrxy = ( double * ) R_alloc ( JY *JXm , sizeof ( double ) );
help3 = ( double * ) R_alloc ( JY * JXm * JY * JXm ,sizeof ( double ) );
invomega2= (double * ) R_alloc ( Jum * JXm * Jum * JXm, sizeof ( double ) );
mu = ( double * ) R_alloc ( Ju * JXm, sizeof ( double ) );
newbeta = ( double * ) R_alloc ( JY*JXm  ,sizeof ( double ) );
mu2 = ( double * ) R_alloc ( JY * JZm, sizeof ( double ) );
mu3 = ( double * ) R_alloc ( Jum * Jum ,sizeof ( double ) );
mu4 = ( double * ) R_alloc ( JY * JY,sizeof ( double ) );
newomega = ( double * ) R_alloc ( JY * JY , sizeof ( double ) );
newomega2 = ( double * ) R_alloc ( Jum*Jum , sizeof ( double ) );
betaX=( double * ) R_alloc ( JY, sizeof ( double ) );
imp=( double * ) R_alloc ( IY * JY,sizeof ( double ) );
impsub=( double * ) R_alloc ( IY ,sizeof ( double ) );
resid=( double * ) R_alloc ( IY * JY,sizeof ( double ) );
residsub=( double * ) R_alloc ( IY,sizeof ( double ) );
Yobs=( double * ) R_alloc ( Ju, sizeof ( double ) );
Ymiss=( double * ) R_alloc ( JY, sizeof ( double ) );
mumiss = ( double * ) R_alloc ( JY, sizeof ( double ) );
omegadrawmiss = ( double * ) R_alloc ( JY*JY ,sizeof ( double ) );
betamiss = ( double * ) R_alloc ( JY ,sizeof ( double ) );
betaobs = ( double * ) R_alloc ( Ju, sizeof ( double ) );
omegaoo= ( double * ) R_alloc ( Ju*Ju , sizeof ( double ) );
omegamo= ( double * ) R_alloc (  Ju*JY  , sizeof ( double ) );
omegamm= ( double * ) R_alloc (  JY*JY  , sizeof ( double ) );
invomega3= ( double * ) R_alloc ( Jum * Jum , sizeof ( double ) );
help4 = ( double * ) R_alloc ( Jum*Jum , sizeof ( double ) );
help5 = ( double * ) R_alloc ( Jum*Jum , sizeof ( double ) );
help6 = ( double * ) R_alloc ( Ju*Ju , sizeof ( double ) );
help7 = ( double * ) R_alloc ( Ju*Ju , sizeof ( double ) );
missing = ( double * ) R_alloc ( IY , sizeof ( double ) );
Xsub = ( double * ) R_alloc ( IY* Il , sizeof ( double ) );
Xsubprop = ( double * ) R_alloc ( Il , sizeof ( double ) );
Zsub = ( double * ) R_alloc ( IY* Ir , sizeof ( double ) );
Zsubprop = ( double * ) R_alloc ( Ir , sizeof ( double ) );
yicategorized=( double * ) R_alloc (  JY,sizeof ( double ) );
allinvomega= (double * ) R_alloc ( (nj*JY*JY) , sizeof ( double ) );
invgamma= (double * )  R_alloc ( JY * JY , sizeof(double) );
invA= (double * )  R_alloc ( JY * JY , sizeof(double) );
Gammastar= (double * )  R_alloc ( JY * JY , sizeof(double) );
clusnum = ( double * )  R_alloc ( nj , sizeof(double));
cumclus = ( double * )  R_alloc ( nj+1 , sizeof(double));

/* Some initializations */

for (j=0; j<IY; j++) {
	missing[j]=0;
	for (k=0;k<JY;k++) {
		if (ISNAN(REAL(Yimp)[j+k*IY])) {
			missing[j]++;
		}
	}
}
r8mat_copy_new(IY, JY, REAL(Yimp2), imp);
r8mat_copy_new(IY, 1, REAL(Ysubimp), impsub);

for (i=0;i<Ib*Jb;i++) REAL(betapost)[i]=0;
for (i=0;i<JY*JY;i++) fixomega[i]=0;
pos=ncon;
if (ncat>0) {
	for (i=0;i<ncat;i++) {
		for (j=0;j<(INTEGER(Y_numcat)[i]-1);j++) {
			for (k=0;k<(INTEGER(Y_numcat)[i]-1);k++) {
				if (j==k) for (t=0;t<nj;t++) REAL(omega)[(pos+j+t*JY)+JY*nj*(pos+k)]=1;
				else for (t=0;t<nj;t++) REAL(omega)[(pos+j+t*JY)+JY*nj*(pos+k)]=0.5;
				fixomega[(pos+j)+Jo*(pos+k)]=1;
			}
		}
		pos=pos+INTEGER(Y_numcat)[i]-1;
	}
}

for (i=0;i<Ju*Ju;i++) newomega2[i]=REAL(covu)[i];

cumclus[0]=0;
for (i=0;i<nj;i++) {
	clusnum[i]=0;
	for (j=0; j<IY; j++) {		
		if (INTEGER(clus)[j]==i) {
			clusnum[i]++;
			
		}
	}
	cumclus[i+1]=cumclus[i]+clusnum[i];
}


for (i=0;i<(IY*Il);i++) Xsub[i]=1;
h=0;
for (i=0;i<IY;i++) {
	for (j=0;j<XLENGTH(ordersub);j++) {
		pos=1;
		currncat=1;
		for (k=0;k<INTEGER(ordersub)[j];k++) {
			pos=pos*(INTEGER(submod)[3+(h+k)*4]);
		}
		for (k=0;k<INTEGER(ordersub)[j];k++) {
			if (INTEGER(submod)[1+h*4]==1) {
				for (jj=0;jj<pos;jj++) {
					Xsub[i+IY*(1+jj+indic)]=Xsub[i+IY*(1+jj+indic)]*pow(imp[i+IY*(INTEGER(submod)[h*Is]-1)],INTEGER(submod)[2+h*Is]);
				}	
			}
			else {
				for (jj=0;jj<pos;jj++) {
					kk=(jj*currncat)%INTEGER(submod)[3+h*4]+2;
					Xsub[i+IY*(1+jj+indic)]=Xsub[i+IY*(1+jj+indic)]*(REAL(Yimpcat)[i+IY*(INTEGER(submod)[h*Is]-1)]==kk);
				}
			}
			currncat=currncat*INTEGER(submod)[3+h*4];
			h=h+1;
		}
		currncat=1;
		indic=indic+pos;
	}
	h=0;
	indic=0;
}

for (i=0;i<(IY*Ir);i++) Zsub[i]=1;
for (i=0;i<IY;i++) {
	pos=0;
	for (k=0;k<Ir;k++) {
		if (INTEGER(submodran)[1+pos*3]==1) {
			Zsub[i+IY*pos]=imp[i+IY*(INTEGER(submodran)[k*3]-1)];
		}
		else if (INTEGER(submodran)[1+pos*3]==2) {
			currncat=(INTEGER(submodran)[2+pos*3]);
			for (jj=1;jj<currncat;jj++) {
				Zsub[i+IY*(pos+jj-1)]=(REAL(Yimpcat)[i+IY*(INTEGER(submodran)[k*3]-1)]==jj);
			}
		}
		pos=pos+1;
	}
}

gamma=JY+1;
//a=JY+1;
eta=REAL(a_prior)[0];
dx=0.001;
u_new=log(a+JY);
precision=0.001;

r8mat_pofac(JY,REAL(Sp),help,1);
r8mat_poinv(JY, help, invgamma);
for (jj=1;jj<JY;jj++) for (tt=0;tt<jj;tt++) invgamma[jj+JY*tt]=invgamma[tt+JY*jj];

GetRNGstate();

for (c=0;c<nj;c++) {
		for (j=0;j<JY;j++) {
			for (t=0;t<JY;t++) newomega[j+t*JY]=REAL(omega)[(c*JY+j)+t*(JY*nj)];
			}
		r8mat_pofac(JY,newomega, help,2);
		r8mat_poinv(JY,help, invomega);
		for (jj=1;jj<JY;jj++) for (tt=0;tt<jj;tt++) invomega[jj+JY*tt]=invomega[tt+JY*jj];
		for (j=0;j<JY;j++) {
			for (t=0;t<JY;t++) allinvomega[(c*JY+j)+t*(JY*nj)]=invomega[j+t*JY];
			}
		}
	for (j=0;j<JY*JY*JX*JX;j++) sumxi[j]=0;	
	for (j=0;j<JY*JX;j++) sumxy[j]=0;
	
/* Running ns iterations of Gibbs sampler*/

for (i=0;i<ns;i++) {
	
	// Rejection Sampling
	if (ncat>0) {
			pos=ncon;
			for (j=0;j<ncat;j++) {
				currncat=INTEGER(Y_numcat)[j]-1;
				for (t=0;t<IY;t++) {
					if (!ISNAN(REAL(Y)[t+(ncon+j)*IY])) {
						for (k=0;k<currncat;k++) betaX[k]=0;
						for (tt=0;tt<JX;tt++) {
							for (k=0;k<currncat;k++) {
								betaX[k]=betaX[k]+REAL(beta)[tt+(k+pos)*Ib]*REAL(X)[t+tt*IX];
							}
						}
						for (tt=0;tt<JZ;tt++) {
							for (k=0;k<currncat;k++) {
								betaX[k]=betaX[k]+REAL(u)[(INTEGER(clus)[t])+nj*(tt+(k+pos)*JZ)]*REAL(Z)[t+tt*IZ];
							}
						}

						for (k=0;k<(JY-currncat);k++) help[k]=0;
						for (tt=0;tt<JX;tt++) {
							for (k=0;k<pos;k++) {
								help[k]=help[k]+REAL(beta)[tt+k*Ib]*REAL(X)[t+tt*IX];
							}
							for (k=(pos+currncat);k<JY;k++) {
								help[k-currncat]=help[k-currncat]+REAL(beta)[tt+k*Ib]*REAL(X)[t+tt*IX];
							}
						}
						for (tt=0;tt<JZ;tt++) {
							for (k=0;k<pos;k++) {
								help[k]=help[k]+REAL(u)[(INTEGER(clus)[t])+nj*(tt+k*JZ)]*REAL(Z)[t+tt*IZ];
							}
							for (k=(pos+currncat);k<JY;k++) {
								help[k-currncat]=help[k-currncat]+REAL(u)[(INTEGER(clus)[t])+nj*(tt+k*JZ)]*REAL(Z)[t+tt*IZ];
							}
						}

						for (k=0;k<pos;k++) {
							help[k]=imp[t+k*IY]-help[k];
						}
				
						for (k=(pos+currncat);k<JY;k++) {
							help[k-currncat]=imp[t+k*IY]-help[k-currncat];
						}
						for (k=0;k<JY;k++) {
							for (kk=0;kk<JY;kk++) {
								if (((kk<pos)||(kk>(pos+currncat-1)))&&((k<pos)||(k>(pos+currncat-1)))) {
									help4[countm]=REAL(omega)[kk+JY*INTEGER(clus)[t]+JY*nj*k];
									countm++;
								}
								else if (((kk<pos)||(kk>(pos+currncat-1)))&&((k>(pos-1))||(k<(pos+currncat)))) {
									help5[counto]=REAL(omega)[kk+JY*INTEGER(clus)[t]+JY*nj*k];
									counto++;
								}
							}
						}
						countm=0;
						counto=0;
						r8mat_pofac((JY-currncat),help4, help6,1);
						r8mat_poinv((JY-currncat),help6, invomega);
			
						for (jj=1;jj<(JY-currncat);jj++) for (tt=0;tt<jj;tt++) invomega[jj+(JY-currncat)*tt]=invomega[tt+(JY-currncat)*jj];
						r8mat_mm_new((JY-currncat),(JY-currncat),currncat,invomega,help5, help2);
						r8mat_mm_new(1,(JY-currncat),currncat,help,help2, mumiss);
						r8mat_add(currncat,1,betaX,mumiss);
						r8mat_mtm_new(currncat,(JY-currncat),currncat,help2,help5, omegadrawmiss);
						r8mat_divide(currncat,currncat,-1,omegadrawmiss);
						for (k=0;k<currncat;k++) {
							omegadrawmiss[k+k*currncat]=omegadrawmiss[k+k*currncat]+1;
							for (kk=0;kk<currncat;kk++) if (k!=kk) omegadrawmiss[k+kk*currncat]=omegadrawmiss[k+kk*currncat]+0.5;
						}
						r8mat_pofac(currncat,omegadrawmiss, omegamm,2);
						flag=0;
						kk=0;
						if (REAL(Y)[t+(ncon+j)*IY]==INTEGER(Y_numcat)[j]) {
							while ((flag==0)&(kk<10000)) {
								r8vec_multinormal_sample((INTEGER(Y_numcat)[j]-1), mumiss,omegamm, newbeta,mu4,0);
								maxim=maxvec((INTEGER(Y_numcat)[j]-1),newbeta);
								if (maxim<0) {
						
									for (k=0;k<(INTEGER(Y_numcat)[j]-1);k++) imp[t+(k+pos)*IY]=newbeta[k];
									flag=1;
									indic++;
								}
							kk++;
							}
						}
						else {
							while ((flag==0)&(kk<10000)) {
								r8vec_multinormal_sample((INTEGER(Y_numcat)[j]-1), mumiss,omegamm, newbeta,mu4,0);
								maxim=argmaxvec((INTEGER(Y_numcat)[j]-1),newbeta);
								maxim2=maxvec((INTEGER(Y_numcat)[j]-1),newbeta);
								if (((maxim+1)==REAL(Y)[t+(ncon+j)*IY])&(maxim2>0)) {
									for (k=0;k<(INTEGER(Y_numcat)[j]-1);k++) imp[t+(k+pos)*IY]=newbeta[k];
									flag=1;
									indic++;
								}
								kk++;
							}
						}
						flag=0;

					}
				}
				pos=pos+INTEGER(Y_numcat)[j]-1;
			}

		}
		

	//Updating beta

	for (j=0;j<JY*JY*JX*JX;j++) sumxi[j]=0;	
	for (j=0;j<JY*JX;j++) sumxy[j]=0;
	for (j=0;j<IY;j++) {
		for (t=0;t<JY;t++) for (k=0;k<JY;k++) invomega[t+k*JY]=allinvomega[INTEGER(clus)[j]*JY+t+k*nj*JY];
		for (t=0;t<JY*JZ*JY;t++) zi[t]=0;
		for (t=0;t<JY*JX*JY;t++) xi[t]=0; 
		for (t=0;t<JY;t++) {
			yi[t]=imp[j+t*IY];
			for (k=0;k<JX;k++) {
				xi[t+(k+t*JX)*JY]=REAL(X)[j+IY*k];
			}
			for (k=0;k<JZ;k++) {
				zi[t+(k+t*JZ)*JY]=REAL(Z)[j+IY*k];
				uj[k+t*JZ]=REAL(u)[(INTEGER(clus)[j])+nj*(k+t*JZ)];
			}
		}
		r8mat_mtm_new(JY*JX,JY,JY,xi,invomega,help2);
		r8mat_mm_new(JY*JX,JY,JX*JY,help2,xi,incrxx);
		r8mat_mm_new(JY,JY*JZ,1,zi,uj,ziu);
		r8mat_divide(JY,1,-1,ziu);
		r8mat_add(JY,1,ziu,yi);
		r8mat_mm_new(JY*JX,JY,1,help2,yi,incrxy);
		r8mat_add(JY*JX,JY*JX,incrxx,sumxi);
		r8mat_add(JY*JX,1,incrxy,sumxy);	
	}
	r8mat_pofac(JY * JX,sumxi,help3,4);
	r8mat_poinv(JY * JX, help3,invomega2);
	for (jj=1;jj<JX*JY;jj++) for (tt=0;tt<jj;tt++) invomega2[jj+JX*JY*tt]=invomega2[tt+JX*JY*jj];
	r8mat_mm_new(JY*JX,JY*JX,1,invomega2,sumxy,mu);
	r8mat_pofac(JY * JX,invomega2,help3,5);
	r8vec_multinormal_sample(JY*JX, mu,help3, REAL(beta),newbeta,0);
	r8mat_add(Ib,Jb,REAL(beta),REAL(betapost));
	
	//Updating random effects
		
	r8mat_pofac(JY*JZ,REAL(covu), help5,1);
	r8mat_poinv(JY*JZ,help5, invomega2);
	for (jj=1;jj<JY*JZ;jj++) for (tt=0;tt<jj;tt++) invomega2[jj+JY*JZ*tt]=invomega2[tt+JY*JZ*jj];
	
	for (c=0;c<nj;c++) {
		for (t=0;t<JY;t++) for (k=0;k<JY;k++) invomega[t+k*JY]=allinvomega[c*JY+t+k*nj*JY];
		for (j=0;j<JY*JY*JZ*JZ;j++) sumzi[j]=0;
		for (j=0;j<JY*JZ;j++) sumzy[j]=0;
		for (j=0;j<IY;j++) {
			if (INTEGER(clus)[j]==c) {
				for (t=0;t<JY*JZ*JY;t++) zi[t]=0;
				for (t=0;t<JY*JX*JY;t++) xi[t]=0; 
				for (t=0;t<JY;t++) {
					yi[t]=imp[j+t*IY];
					for (k=0;k<JX;k++) {
						xi[t+(k+t*JX)*JY]=REAL(X)[j+IX*k];
					}
					for (k=0;k<JZ;k++) {
						zi[t+(k+t*JZ)*JY]=REAL(Z)[j+IZ*k];
					}
				}
				r8mat_mtm_new(JY*JZ,JY,JY,zi,invomega,help4);
				r8mat_mm_new(JY*JZ,JY,JZ*JY,help4,zi,incrzz);
				r8mat_mm_new(JY,JY*JX,1,xi,REAL(beta),ziu);
				r8mat_divide(JY,1,-1,ziu);
				r8mat_add(JY,1,ziu,yi);
				r8mat_mm_new(JY*JZ,JY,1,help4,yi,incrzy);
				r8mat_add(JY*JZ,JY*JZ,incrzz,sumzi);
				r8mat_add(JY*JZ,1,incrzy,sumzy);
				
			}
		}
		
		r8mat_add(JY*JZ,JY*JZ,invomega2,sumzi);
		
		r8mat_pofac(JY * JZ,sumzi,help5,8);
		r8mat_poinv(JY * JZ, help5,invomega3);
		for (jj=1;jj<JZ*JY;jj++) for (tt=0;tt<jj;tt++) invomega3[jj+JZ*JY*tt]=invomega3[tt+JZ*JY*jj];
		r8mat_mm_new(JY*JZ,JY*JZ,1,invomega3,sumzy,mu2);
		r8mat_pofac(JY * JZ,invomega3,help5,9); 
		r8vec_multinormal_sample(JY*JZ, mu2,help5,newu, incrzy,0);
		for (t=0;t<JY;t++) for (k=0;k<JZ;k++) REAL(u)[c+nj*(k+t*JZ)] = newu[k+t*JZ];
		
	}
	r8mat_add(Iu,Ju,REAL(u),REAL(upost));
	
	//Updating level 2 covariance matrix

		for (j=0;j<Ju*Ju;j++) mu3[j]=0;
		for (j=0;j<nj; j++) {
			for (t=0;t<Ju;t++) uj[t]=REAL(u)[j+nj*t];
			r8mat_mmt_new(Ju,1,Ju,uj,uj,help4);
			r8mat_add(Ju,Ju,help4,mu3);	
		}
		r8mat_add(Ju,Ju,REAL(Sup),mu3);
		r8mat_pofac(Ju,mu3,help4,10);
		r8mat_poinv(Ju, help4, invomega3);
		for (jj=1;jj<(Ju);jj++) for (tt=0;tt<jj;tt++) invomega3[jj+(Ju)*tt]=invomega3[tt+(Ju)*jj];

		wishart_sample(Ju,(nj+Ju),invomega3,newomega2, help4,sumzi,incrzz,mu3,0);
	
		r8mat_pofac(Ju,newomega2, help4,11);
		r8mat_poinv(Ju, help4,invomega3);
		for (jj=1;jj<(Ju);jj++) for (tt=0;tt<jj;tt++) invomega3[jj+(Ju)*tt]=invomega3[tt+(Ju)*jj];
		for(k=0;k<(Ju);k++)  for(j=0;j<(Ju);j++)  REAL(covu)[j+(Ju)*k]=invomega3[j+(Ju)*k];
	
		r8mat_add(Ju,Ju,REAL(covu),REAL(covupost));

	//Updating residuals

	for (t=0;t<IY;t++) {
		for (j=0; j<JY; j++) {
			resid[t+j*IY]=imp[t+j*IY];
			for (k=0;k<JX;k++) resid[t+j*IY]=resid[t+j*IY]-REAL(beta)[k+j*JX]*REAL(X)[t+k*IX];
			for (k=0;k<JZ;k++) resid[t+j*IY]=resid[t+j*IY]-REAL(u)[INTEGER(clus)[t]+nj*(k+j*JZ)]*REAL(Z)[t+k*IZ];
		}
	}

	//Updating omega

	//Updating scale matrix of inverse Wishart
	
	for (t=0;t<JY*JY;t++) help[t]=0;
	for (c=0;c<nj;c++) {
		for (t=0;t<JY;t++) {
			for (k=0;k<JY;k++) {
				help[t+k*JY]=help[t+k*JY]+allinvomega[(c*JY+t)+k*(JY*nj)];
			}
		}
	}
	
	r8mat_add(JY,JY,invgamma,help);
	r8mat_pofac(JY,help,help7,10);
	r8mat_poinv(JY, help7, Gammastar);
	for (jj=1;jj<JY;jj++) for (tt=0;tt<jj;tt++) Gammastar[jj+JY*tt]=Gammastar[tt+JY*jj];
	wishart_sample(JY,(nj*a+gamma),Gammastar,invA,help2, omegaoo,omegamo,omegamm,0);

	u_new=log(a+JY);	
	u_m=newton_raphson(u_new,precision, dx,eta,JY,nj,allinvomega,invomega,invgamma,help,help2);	
	if (u_m==(-9999)) u_m=u_new;
	deriv2=derive2_log_f_u(dx,eta, u_m, JY, nj, allinvomega, invomega,  invA,  help,  help2);
	lambda=sqrt(-5/(4*deriv2));
	u_prop=lambda*t_sample(4,0)+u_m;								
	con2=exp(log_f_u(eta, u_prop, JY, nj, allinvomega, invomega, invA,help,help2)-log_f_u(eta, u_new, JY, nj, allinvomega, invomega, invA,help,help2))*h_u(u_new,u_m,lambda)/h_u(u_prop,u_m,lambda);
	if ((( double ) unif_rand ( ) )<r8_min(1,con2)) u_new=u_prop;
	if (isnan(exp(u_new)-JY)) u_new=log(a+JY);
	if ((exp(u_new)-JY)<JY) u_new=log(a+JY);
	a=exp(u_new)-JY;	
		
	//Updating matrices
	
	if (ncat==0) {
		for (c=0;c<nj;c++) {	
			for (j=0;j<JY*JY;j++) mu4[j]=0;
			for (j=cumclus[c];j<cumclus[c+1];j++) {
				for (t=0;t<JY;t++) {
					yi[t]=resid[j+t*IY];
				}
				r8mat_mmt_new(JY,1,JY,yi,yi,help);
				r8mat_add(JY,JY,help,mu4);
			}

			r8mat_add(JY,JY,invA,mu4);
			r8mat_pofac(JY,mu4,help,11);
			r8mat_poinv(JY, help,invomega);
			for (jj=1;jj<JY;jj++) for (tt=0;tt<jj;tt++) invomega[jj+JY*tt]=invomega[tt+JY*jj];

			wishart_sample(JY,clusnum[c]+a,invomega,newomega,help, omegaoo,omegamo,omegamm,0);
			r8mat_pofac(JY,newomega,help,12);
			r8mat_poinv(JY, help,invomega);
			for (jj=1;jj<JY;jj++) for (tt=0;tt<jj;tt++) invomega[jj+JY*tt]=invomega[tt+JY*jj];
			for(k=0;k<JY;k++)  for(j=0;j<JY;j++)  REAL(omega)[(c*JY+j)+k*(JY*nj)]=invomega[j+JY*k];

		}
	}
	else {
		for (c=0;c<nj;c++) {	
			aj=a+clusnum[c];
			for (kk=0;kk<JY;kk++) {
				for (tt=0;tt<JY;tt++) {
					newomega[kk+tt*JY]=REAL(omega)[(c*JY+kk)+tt*(JY*nj)];
					}
				}
			flag=0;
			r8mat_pofac(JY,newomega,help,6);
			detom=r8mat_podet(JY, help);

			r8mat_poinv(JY, help,invomega);
			for (jj=1;jj<JY;jj++) for (tt=0;tt<jj;tt++) invomega[jj+JY*tt]=invomega[tt+JY*jj];
			logLH=0;
			for (t=0;t<JY*JY;t++) help5[t]=0;
			for (t=cumclus[c];t<cumclus[c+1];t++) {
				for (jj=0;jj<JY;jj++) help4[jj]=resid[t+jj*IY];
					r8mat_mmt_new(JY,1,JY,help4,help4,help7);
					r8mat_add(JY,JY,help7, help5);
			}
			r8mat_add(JY,JY,invA, help5);

			r8mat_mm_new(JY,JY,JY,help5,invomega,help6);
			for (t=0;t<JY;t++) logLH=logLH+help6[t+t*JY];
			logLH=logLH*(-0.5)-(aj+JY+1)*log(detom)/2;
			for (j=0;j<JY;j++) {
				for (k=0;k<JY;k++) {
					if ((fixomega[j+JY*k]==0)&(j<=k)) {
						if (j==k) {
							sdom=REAL(omega)[(c*JY+j)+k*(JY*nj)]*sqrt(11.6/clusnum[c]);
							meanom=REAL(omega)[(c*JY+j)+k*(JY*nj)];
						}
						else {
							sdom=0.1*sqrt(REAL(omega)[(c*JY+j)+j*(JY*nj)]*REAL(omega)[(c*JY+k)+k*(JY*nj)]);
							meanom=REAL(omega)[(c*JY+j)+k*(JY*nj)];
						}
						kk=0;
						while ((flag==0)&(kk<100)) {
							newomega[j+JY*k]=r8_normal_sample(meanom,sdom,0);
							newomega[k+JY*j]=newomega[j+JY*k];
							flag=checkposdef(JY,newomega,help,help6);
							kk++;
						}
						r8mat_pofac(JY,newomega,help,7);
						detom=r8mat_podet(JY, help);
						r8mat_poinv(JY, help,invomega);
						for (jj=1;jj<JY;jj++) for (tt=0;tt<jj;tt++) invomega[jj+JY*tt]=invomega[tt+JY*jj];
						newlogLH=0;
						r8mat_mm_new(JY,JY,JY,help5,invomega,help6);
						for (t=0;t<JY;t++) newlogLH=newlogLH+help6[t+t*JY];
						newlogLH=newlogLH*(-0.5)-(aj+JY+1)*log(detom)/2;
						if (((( double ) unif_rand ( ))<exp(newlogLH-logLH))&(flag==1)) {
							REAL(omega)[(c*JY+j)+k*(JY*nj)]=newomega[j+JY*k];
							REAL(omega)[(c*JY+k)+j*(JY*nj)]=newomega[k+JY*j];
							logLH=newlogLH;
						}
						else {
							newomega[j+JY*k]=REAL(omega)[(c*JY+j)+k*(JY*nj)];
							newomega[k+JY*j]=REAL(omega)[(c*JY+k)+j*(JY*nj)];
						}
						flag=0;
					}
				}
			}
		}
	}
	r8mat_add(Io,Jo,REAL(omega),REAL(omegapost));
	
	for (c=0;c<nj;c++) {
		for (j=0;j<JY;j++) {
			for (t=0;t<JY;t++) newomega[j+t*JY]=REAL(omega)[(c*JY+j)+t*(JY*nj)];
			}
		r8mat_pofac(JY,newomega, help,2);
		r8mat_poinv(JY,help, invomega);
		for (jj=1;jj<JY;jj++) for (tt=0;tt<jj;tt++) invomega[jj+JY*tt]=invomega[tt+JY*jj];
		for (j=0;j<JY;j++) {
			for (t=0;t<JY;t++) allinvomega[(c*JY+j)+t*(JY*nj)]=invomega[j+t*JY];
			}
		}
	for (j=0;j<JY*JY*JX*JX;j++) sumxi[j]=0;	
	for (j=0;j<JY*JX;j++) sumxy[j]=0;
		
	//imputing missing values compatibly with substantive model
	
	for (j=0; j<IY; j++) {
		for (t=0;t<JY;t++) for (k=0;k<JY;k++) invomega[t+k*JY]=allinvomega[INTEGER(clus)[j]*JY+t+k*nj*JY];
		nmiss=missing[j];
		k=0;
		if (nmiss>0) {
			for (t=0;t<JY;t++) betaX[t]=0;		
			for (t=0;t<JY*JX*JY;t++) xi[t]=0; 
			for (t=0;t<JY*JZ*JY;t++) zi[t]=0; 
			for (t=0;t<Il;t++) Xsubprop[t]=1;
			for (t=0;t<Ir;t++) Zsubprop[t]=1;
			for (t=0;t<JY;t++) {
				yi[t]=imp[j+t*IY];
				for (tt=0;tt<JX;tt++) {
					xi[t+(tt+t*JX)*JY]=REAL(X)[j+IY*tt];
				}
				for (tt=0;tt<JZ;tt++) {
					zi[t+(tt+t*JZ)*JY]=REAL(Z)[j+IY*tt];
				}
				uj[t]=REAL(u)[(INTEGER(clus)[j])+nj*t];
			}

			r8mat_mm_new(JY,JY*JX,1,xi,REAL(beta),help6);
			r8mat_add(JY,1,help6,betaX);
			r8mat_mm_new(JY,JY*JZ,1,zi,uj,incrzz);
			r8mat_add(JY,1,incrzz,betaX);
			
			for (t=0;t<JY;t++) help4[t]=yi[t]-betaX[t];
			r8mat_mm_new(1,JY,JY,help4,invomega,help5);
			r8mat_mmt_new(1,JY,1,help5,help4,help6);
			mu2[0]=impsub[j];
			for (t=0;t<Il;t++) mu2[0]=mu2[0]-REAL(betaY)[t]*Xsub[j+IY*t];
			for (t=0;t<Ir;t++) mu2[0]=mu2[0]-REAL(uY)[(INTEGER(clus)[j])+nj*t]*Zsub[j+IY*t];
			logLH=-0.5*mu2[0]*mu2[0]/REAL(varY)[0]-help6[0]/2;
		}
		while (nmiss>0) {
			if (ISNAN(REAL(Yimp)[j+k*IY])) {
				betamiss[0]=betaX[k];
				omegamm[0]=REAL(omega)[INTEGER(clus)[j]*JY+k+k*Io];
				for (t=0;t<JY;t++) {
					if (t!=k) {
						betaobs[counto]=betaX[t];
						omegamo[counto]=REAL(omega)[INTEGER(clus)[j]*JY+k+t*Io];
						Yobs[counto]=imp[j+t*IY];
						for (tt=0;tt<JY;tt++) {
							if (tt!=k) {
								omegaoo[countoo]=REAL(omega)[INTEGER(clus)[j]*JY+tt+t*Io];
								countoo++;
							}
						}
						counto++;
					}
				}

				r8mat_pofac((JY-1),omegaoo,help2,14);
				r8mat_poinv((JY-1),help2,invomega3);
				for (jj=1;jj<JY-1;jj++) for (tt=0;tt<jj;tt++) invomega3[jj+(JY-1)*tt]=invomega3[tt+(JY-1)*jj];
				r8mat_mmt_new((JY-1),(JY-1),1,invomega3,omegamo,help3);
				r8mat_divide((JY-1),1,-1,betaobs);
				r8mat_add((JY-1),1,betaobs,Yobs);
				r8mat_mtm_new(1,(JY-1),1,Yobs,help3,mumiss);

				mumiss[0]=mumiss[0]+betamiss[0];
				r8mat_mm_new(1,(JY-1),1,omegamo,help3,omegadrawmiss);
				omegadrawmiss[0]=omegamm[0]-omegadrawmiss[0];
				Ymiss[0]=r8_normal_sample(yi[k],sqrt(omegamm[0]/10),0);
				
				mu2[0]=impsub[j];
				for (t=0;t<Il;t++) mu2[0]=mu2[0]-REAL(betaY)[t]*Xsub[j+IY*t];
				for (t=0;t<Ir;t++) mu2[0]=mu2[0]-REAL(uY)[(INTEGER(clus)[j])+nj*t]*Zsub[j+IY*t];

				logLH=-0.5*mu2[0]*mu2[0]/REAL(varY)[0]-0.5*(pow(yi[k]-mumiss[0],2))/omegadrawmiss[0];;

				// Controllare se accettabile
				yi[k]=Ymiss[0];								
				if (ncat>0) {
					h=0;
					for (jj=0;jj<ncat;jj++) {
						maxx=yi[(ncon+h)];
						nmaxx=0;
						for (kk=1;kk<(INTEGER(Y_numcat)[jj]-1);kk++) {
							if (yi[(ncon+h+kk)]>maxx) {
								maxx=yi[(ncon+h+kk)];
								nmaxx=kk;
							}
						}			
						if (maxx>0) yicategorized[jj]=nmaxx;
						else yicategorized[jj]=INTEGER(Y_numcat)[j]-1;
						h=h+INTEGER(Y_numcat)[jj]-1;
					}
				}
					
					//Update Xsubprop
				h=0;
				indic=0;
				for (t=0;t<Il;t++) Xsubprop[t]=1;

				for (jj=0;jj<XLENGTH(ordersub);jj++) {
					pos=1;
					currncat=1;
					for (t=0;t<INTEGER(ordersub)[jj];t++) {
						pos=pos*(INTEGER(submod)[3+(h+t)*4]);
					}
					for (t=0;t<INTEGER(ordersub)[jj];t++) {
						if (INTEGER(submod)[1+h*4]==1) {
							for (tt=0;tt<pos;tt++) {
								Xsubprop[(1+tt+indic)]=Xsubprop[(1+tt+indic)]*pow(yi[(INTEGER(submod)[h*Is]-1)],INTEGER(submod)[2+h*Is]);

							}	
						} else {
							for (tt=0;tt<pos;tt++) {
								kk=(tt*currncat)%INTEGER(submod)[3+h*4]+1;
								Xsubprop[(1+tt+indic)]=Xsubprop[(1+tt+indic)]*(yicategorized[(INTEGER(submod)[h*Is]-1)]==kk);
							}
						}	
						currncat=currncat*INTEGER(submod)[3+h*4];
						h=h+1;
					}
					currncat=1;
					indic=indic+pos;
				}
				
				   // Update Zsubprop

				for (t=0;t<Ir;t++) Zsubprop[t]=1;
				
				for (t=0;t<Ir;t++) {
					if (INTEGER(submodran)[1+t*3]==1) {
						Zsubprop[t]=yi[(INTEGER(submodran)[t*3]-1)];
					} else if (INTEGER(submodran)[1+t*3]==2) {
						currncat=(INTEGER(submodran)[2+t*3]);
						for (jj=1;jj<currncat;jj++) {
							Zsubprop[t+jj-1]=(yicategorized[(INTEGER(submodran)[t*3]-1)]==jj);
						}
					}
				pos=pos+1;
				}				
				
				mu2[0]=impsub[j];
				for (t=0;t<Il;t++) mu2[0]=mu2[0]-REAL(betaY)[t]*Xsubprop[t];
				for (t=0;t<Ir;t++) mu2[0]=mu2[0]-REAL(uY)[(INTEGER(clus)[j])+nj*t]*Zsubprop[t];
				newlogLH=-0.5*mu2[0]*mu2[0]/REAL(varY)[0]-0.5*(pow(Ymiss[0]-mumiss[0],2))/omegadrawmiss[0];
	
				if ((( double ) unif_rand ( ) )<exp(newlogLH-logLH)) {	

					imp[j+k*IY]=Ymiss[0];
					for (t=0;t<Il;t++) Xsub[j+t*IY]=Xsubprop[t];
					for (t=0;t<Ir;t++) Zsub[j+t*IY]=Zsubprop[t];
					logLH=newlogLH;
					 accratio=accratio+1;
				} else {
					yi[k]=imp[j+k*IY];
				}
				totprop=totprop+1;				
				nmiss--;
				counto=0;
				countoo=0;
			}				
			k++;			
		}			
		counto=0;
		countoo=0;			
	}

	
	// Update beta of substantive model
	
	for (j=0;j<Il*Il;j++) sumxi[j]=0;	
	for (j=0;j<Il;j++) sumxy[j]=0;
	for (j=0;j<IY;j++) {
		yi[0]=impsub[j];
		for (k=0;k<Il;k++) {
			xi[k]=Xsub[j+IY*k];
		}
		for (k=0;k<Ir;k++) {
			yi[0]=yi[0]-Zsub[j+IY*k]*REAL(uY)[(INTEGER(clus)[j])+nj*(k)];
			}
		r8mat_mtm_new(Il,1,Il,xi,xi,incrxx);
		r8mat_divide(Il,Il,REAL(varY)[0],incrxx);
		r8mat_mtm_new(Il,1,1,xi,yi,incrxy);
		r8mat_divide(Il,1,REAL(varY)[0],incrxy);
		r8mat_add(Il,Il,incrxx,sumxi);
		r8mat_add(Il,1,incrxy,sumxy);	
	}
	r8mat_pofac(Il,sumxi,help3,4);
	r8mat_poinv(Il, help3,invomega2);
	for (jj=1;jj<Il;jj++) for (tt=0;tt<jj;tt++) invomega2[jj+Il*tt]=invomega2[tt+Il*jj];
	r8mat_mm_new(Il,Il,1,invomega2,sumxy,mu);
	r8mat_pofac(Il,invomega2,help3,5);
	r8vec_multinormal_sample(Il, mu,help3, REAL(betaY),newbeta,0);	

	r8mat_add(1,Il,REAL(betaY),REAL(betaYpost));

	// Update random effects of substantive model
	r8mat_pofac(Ir,REAL(covuY), help5,1);
	r8mat_poinv(Ir,help5, invomega2);
	for (jj=1;jj<Ir;jj++) for (tt=0;tt<jj;tt++) invomega2[jj+Ir*tt]=invomega2[tt+Ir*jj];

	
	for (c=0;c<nj;c++) {
		for (j=0;j<Ir*Ir;j++) sumzi[j]=0;
		for (j=0;j<Ir;j++) sumzy[j]=0;
		for (j=0;j<IY;j++) {
			if (INTEGER(clus)[j]==c) {
				yi[0]=impsub[j];
				for (k=0;k<Il;k++) {
					yi[0]=yi[0]-REAL(betaY)[k]*Xsub[j+IY*k];
				}
				for (k=0;k<Ir;k++) {
					zi[k]=Zsub[j+IY*k];
				}
				r8mat_mtm_new(Ir,1,Ir,zi,zi,incrzz);
				r8mat_divide(Ir,Ir,REAL(varY)[0],incrzz);				
				r8mat_mm_new(Ir,1,1,zi,yi,incrzy);
				r8mat_divide(Ir,1,REAL(varY)[0],incrzy);
				r8mat_add(Ir,Ir,incrzz,sumzi);
				r8mat_add(Ir,1,incrzy,sumzy);				
			}
		}

		r8mat_add(Ir,Ir,invomega2,sumzi);
		r8mat_pofac(Ir,sumzi,help5,8);
		r8mat_poinv(Ir, help5,invomega3);
		for (jj=1;jj<Ir;jj++) for (tt=0;tt<jj;tt++) invomega3[jj+Ir*tt]=invomega3[tt+Ir*jj];

		r8mat_mm_new(Ir,Ir,1,invomega3,sumzy,mu2);
		r8mat_pofac(Ir,invomega3,help5,9); 
		r8vec_multinormal_sample(Ir, mu2,help5,newu, incrzy,0);
		for (t=0;t<Ir;t++) REAL(uY)[c+nj*t] = newu[t];
		
	}
	r8mat_add(nj,Ir,REAL(uY),REAL(uYpost));
	
		//Updating level 2 covariance matrix of substantive model 
	
	for (j=0;j<Ir*Ir;j++) mu3[j]=0;
	for (j=0;j<nj; j++) {
		for (t=0;t<Ir;t++) uj[t]=REAL(uY)[j+nj*t];
		r8mat_mmt_new(Ir,1,Ir,uj,uj,help4);
		r8mat_add(Ir,Ir,help4,mu3);	
	}
	r8mat_add(Ir,Ir,REAL(covuYprior),mu3);
	r8mat_pofac(Ir,mu3,help4,10);
	r8mat_poinv(Ir, help4, invomega3);
	for (jj=1;jj<Ir;jj++) for (tt=0;tt<jj;tt++) invomega3[jj+Ir*tt]=invomega3[tt+Ir*jj];

	wishart_sample(Ir,(nj+Ir),invomega3,newomega2, help4,sumzi,incrzz,mu3,0);
	
	r8mat_pofac(Ir,newomega2, help4,11);
	r8mat_poinv(Ir, help4,invomega3);
	for (jj=1;jj<Ir;jj++) for (tt=0;tt<jj;tt++) invomega3[jj+Ir*tt]=invomega3[tt+Ir*jj];
	for(k=0;k<Ir*Ir;k++) REAL(covuY)[k]=invomega3[k];
	
	r8mat_add(Ir,Ir,REAL(covuY),REAL(covuYpost));

	
			//Updating residuals

	for (t=0;t<IY;t++) {
		residsub[t]=impsub[t];
		for (k=0;k<Il;k++) {
			residsub[t]=residsub[t]-REAL(betaY)[k]*Xsub[t+k*IX];
		}
		for (k=0;k<Ir;k++) {
			residsub[t]=residsub[t]-Zsub[t+IY*k]*REAL(uY)[(INTEGER(clus)[t])+nj*(k)];
			}
	}
	
		//Updating omega
	
	r8mat_mmt_new(1,IY,1,residsub,residsub,mu2);
	mu2[0]=mu2[0]+REAL(varYprior)[0];
	invomega3[0]=1/mu2[0];
	wishart_sample(1,IY+1,invomega3,newomega,help, omegaoo,omegamo,omegamm,0);	
	REAL(varY)[0]=1/newomega[0];
//REAL(varY)[0]=1;
	REAL(varYpost)[0]=REAL(varYpost)[0]+REAL(varY)[0];

		// Imputing missing outcomes
	for (t=0;t<IY;t++) {
		if (ISNAN(REAL(Ysub)[t])) {
			mu2[0]=0;
			for (k=0;k<Il;k++) mu2[0]=mu2[0]+REAL(betaY)[k]*Xsub[t+k*IX];
			for (k=0;k<Ir;k++) mu2[0]=mu2[0]+REAL(uY)[(INTEGER(clus)[t])+nj*k]*Zsub[t+k*IX];

			impsub[t]=r8_normal_sample(mu2[0],sqrt(REAL(varY)[0]),0);
		}
	}
	
	if ((i+1)%fl==0) Rprintf("Iteration %d completed\n",i+1);
}

if (((double)accratio/((double)totprop))<0.3) Rprintf("Warning: acceptance ratio = %f. This might be a sign that the chain did not mix well. \n" , ((double)accratio/((double)totprop)));

for(i=0;i<IY;i++)  {
	for(j=0;j<JY;j++)  {
		REAL(Yimp2)[i+IY*j]=imp[i+IY*j];
	}
	
	REAL(Ysubimp)[i]=impsub[i];

	if (ncat>0) {
		h=0;
	    for (j=0;j<ncat;j++) {
			maxx=imp[i+(ncon+h)*IY];
			nmaxx=0;
			for (k=1;k<(INTEGER(Y_numcat)[j]-1);k++) {
				if (imp[i+(ncon+h+k)*IY]>maxx) {
					maxx=imp[i+(ncon+h+k)*IY];
					nmaxx=k;
				}
			}			
		if (maxx>0) REAL(Yimpcat)[i+IY*j]=nmaxx+1;
		else REAL(Yimpcat)[i+IY*j]=INTEGER(Y_numcat)[j];
		h=h+INTEGER(Y_numcat)[j]-1;
		}
	}	
}

	
r8mat_divide(Ib,Jb,ns,REAL(betapost));
r8mat_divide(Iu,Ju,ns,REAL(upost));
r8mat_divide(Io,Jo,ns,REAL(omegapost));
r8mat_divide(Ju,Ju,ns,REAL(covupost));
r8mat_divide(Il,1,ns,REAL(betaYpost));
r8mat_divide(nj,Ir,ns,REAL(uYpost));
REAL(varYpost)[0]=REAL(varYpost)[0]/ns;
r8mat_divide(Ir,Ir,ns,REAL(covuYpost));
PutRNGstate();
UNPROTECT(46);
return R_NilValue;
}
