#include<stdio.h>

float tmf( float err, float mid1, float mid2, float mid3 );
float tmfr( float err, float mid1, float mid2 );
float tmfl( float err, float mid1, float mid2 );
 
int fuzzy(){
	%script to output a single output based on the inputs given below
	float errorPos = -0.0625;
	int errorVel = 3;

	float[] midError = {-0.5, -.25, 0, .25, .5}; %triangle mid points on error (position) universe
	midVelocity = {-4, -2, 0, 2}; %triangle mid points on velocity (de/dt) universe

	%singleton consequents
	int NL=-10;
	int NS=-5;
	int ZE=0;
	int PS=5;
	int PL=10;

	rule={PL, PL, PL, PS, ZE, PL, PL, PS, ZE, NS, PL, PS, ZE, NS, NL, PS, ZE, NS, NL, NL, ZE, NS, NL, NL, NL};


	%calculate the degree of membership in the 5 error fuzzy sets
	mfErrPos=[tmfl(errorPos,midError(1),midError(2))
	tmf(errorPos,midError(1),midError(2),midError(3))
	tmf(errorPos,midError(2),midError(3),midError(4))
	tmf(errorPos,midError(3),midError(4),midError(5))
	tmfr(errorPos,midError(4),midError(5))];

	%calculate the degree of membership in the 5 rate of change fuzzy sets
	mfVelocity=[tmfl(errorVel,midVelocity(1),midVelocity(2))
	tmf(errorVel,midVelocity(1),midVelocity(2),midVelocity(3))
	tmf(errorVel,midVelocity(2),midVelocity(3),midVelocity(4))
	tmf(errorVel,midVelocity(3),midVelocity(4),midVelocity(5))
	tmfr(errorVel,midVelocity(4),midVelocity(5))];

	%calculate the degree of firing of each of the 25 rules using prod
	%T-norm. premiseCertainty is a 5x5 matrix.
	for (int j=0; j<5; j++){
		for (int k=0; k<5; k++){
			premiseCertainty(j,k)=mfErrPos(k)*mfVelocity(j);
		}
	}

	%crisp output of nonlinear fuzzy controller
	num=sum(sum(rule.*premiseCertainty));
	den=sum(sum(premiseCertainty));
	fuzzyOut=num/den

}

float tmf(){
}