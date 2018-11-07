#include<stdio.h>







float tri_steering_1_pt = 80;
float tri_steering_1_mid = 0;
float tri_steering_2_pt1 = 0;
float tri_steering_2_pt2 = 160;
float tri_steering_3_pt1 = 80;
float tri_steering_3_pt2 = 240;
float tri_steering_4_pt1 = 160;
float tri_steering_4_pt2 = 320;
float tri_steering_5_pt = 240;
float tri_steering_5_mid = 320;

float steering_1_val=-2.0;
float steering_2_val=-1.0;
float steering_3_val=0.0;
float steering_4_val=1.0;
float steering_5_val=2.0;


float tri_throttle_1_pt = 60;
float tri_throttle_1_mid = 0;
float tri_throttle_2_pt1 = 0;
float tri_throttle_2_pt2 = 120;
float tri_throttle_3_pt1 = 60;
float tri_throttle_3_pt2 = 180;
float tri_throttle_4_pt1 = 120;
float tri_throttle_4_pt2 = 240;
float tri_throttle_5_pt = 180;
float tri_throttle_5_mid = 240;

float throttle_1_val=4;
float throttle_2_val=3;
float throttle_3_val=2;
float throttle_4_val=1;
float throttle_5_val=0;


float steering_fuzzy(float box_x);
float throttle_fuzzy(float box_h);
float tmf(float val, float pt1, float pt2);
float tmfl(float val, float pt, float mid);
float tmfr(float val, float pt, float mid);
void setup();
void loop();

int main(){
  setup();
  //while(1)
  //  loop();
}

void setup(){
  float box_x = 161;
  float box_h = 120;

  printf("\nsteering:\n%f\n\n", steering_fuzzy(box_x));
  printf("\nthrottle:\n%d\n\n",(int) throttle_fuzzy(box_h));

}

void loop(){

}


float steering_fuzzy(float box_x){

  float memberships[5]= {0,0,0,0,0};
  float adjusted_values[5]= {0,0,0,0,0};


  memberships[0]= tmfl(box_x,tri_steering_1_pt,tri_steering_1_mid);
  memberships[1]= tmf(box_x,tri_steering_2_pt1,tri_steering_2_pt2);
  memberships[2]= tmf(box_x,tri_steering_3_pt1,tri_steering_3_pt2);
  memberships[3]= tmf(box_x,tri_steering_4_pt1,tri_steering_4_pt2);
  memberships[4]= tmfr(box_x,tri_steering_5_pt,tri_steering_5_mid);

  adjusted_values[0]=memberships[0]* steering_1_val;
  adjusted_values[1]=memberships[1]* steering_2_val;
  adjusted_values[2]=memberships[2]* steering_3_val;
  adjusted_values[3]=memberships[3]* steering_4_val;
  adjusted_values[4]=memberships[4]* steering_5_val;


  return  adjusted_values[0] + adjusted_values[1] + adjusted_values[2] + adjusted_values[3] + adjusted_values[4];

}

float throttle_fuzzy(float box_h){

  float memberships[5]= {0,0,0,0,0};
  float adjusted_values[5]= {0,0,0,0,0};


  memberships[0]= tmfl(box_h,tri_throttle_1_pt,tri_throttle_1_mid);
  memberships[1]= tmf(box_h,tri_throttle_2_pt1,tri_throttle_2_pt2);
  memberships[2]= tmf(box_h,tri_throttle_3_pt1,tri_throttle_3_pt2);
  memberships[3]= tmf(box_h,tri_throttle_4_pt1,tri_throttle_4_pt2);
  memberships[4]= tmfr(box_h,tri_throttle_5_pt,tri_throttle_5_mid);

  adjusted_values[0]=memberships[0]* throttle_1_val;
  adjusted_values[1]=memberships[1]* throttle_2_val;
  adjusted_values[2]=memberships[2]* throttle_3_val;
  adjusted_values[3]=memberships[3]* throttle_4_val;
  adjusted_values[4]=memberships[4]* throttle_5_val;


  return  adjusted_values[0] + adjusted_values[1] + adjusted_values[2] + adjusted_values[3] + adjusted_values[4];

}



float tmf(float val, float pt1, float pt2)
{
  if(val<pt1 || val>pt2){
    return 0.0;
  }

  int mid = (pt2-pt1)/2 + pt1;
  //printf("%d\n",mid);

  if(val>pt1 && val<= mid){
    return (1/(mid-pt1))*val + pt1 * -1*((1/(mid-pt1)));
  }
  else if(val<pt2 && val> mid){
    return -1*(1/(pt2-mid))*val + pt2 * -1*((1/(mid-pt2)));
  }
  return 0.0;
}

float tmfl(float val, float pt, float mid)
{
  if(val>pt){
    return 0.0;
  }

  if(val<=pt && val>= mid){
    return -1*(1/(pt-mid))*val + pt * -1*((1/(mid-pt)));
  }
  return 1.0;
}

float tmfr(float val, float pt, float mid)
{
  if(val<pt){
    return 0.0;
  }

  if(val>=pt && val<= mid){
    return (1/(mid-pt))*val + pt * -1*((1/(mid-pt)));
  }
  return 1.0;
}
