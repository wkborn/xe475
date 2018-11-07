#include<Servo.h>
#include<Pixy.h>

Servo throttle;
Servo steer;
Pixy pixy;

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

float tri_delta_steering_1_pt = 10;
float tri_delta_steering_1_mid = 20;
float tri_delta_steering_2_pt1 = 20;
float tri_delta_steering_2_pt2 = 0;
float tri_delta_steering_3_pt1 = 10;
float tri_delta_steering_3_pt2 = -10;
float tri_delta_steering_4_pt1 = 0;
float tri_delta_steering_4_pt2 = -20;
float tri_delta_steering_5_pt = -10;
float tri_delta_steering_5_mid = -20;
;

float steering_1_val = -800.0;
float steering_2_val = -200.0;
float steering_3_val = 0.0;
float steering_4_val = 200.0;
float steering_5_val = 800.0;



float tri_throttle_1_pt = 25;
float tri_throttle_1_mid = 0;
float tri_throttle_2_pt1 = 0;
float tri_throttle_2_pt2 = 50;
float tri_throttle_3_pt1 = 25;
float tri_throttle_3_pt2 = 75;
float tri_throttle_4_pt1 = 50;
float tri_throttle_4_pt2 = 100;
float tri_throttle_5_pt = 75;
float tri_throttle_5_mid = 100;

float tri_delta_throttle_1_pt = 5;
float tri_delta_throttle_1_mid = 0;
float tri_delta_throttle_2_pt1 = 0;
float tri_delta_throttle_2_pt2 = 10;
float tri_delta_throttle_3_pt1 = 5;
float tri_delta_throttle_3_pt2 = 15;
float tri_delta_throttle_4_pt1 = 10;
float tri_delta_throttle_4_pt2 = 20;
float tri_delta_throttle_5_pt = 15;
float tri_delta_throttle_5_mid = 20;

float throttle_1_val = 475;
float throttle_2_val = 350;
float throttle_3_val = 125;
float throttle_4_val = 50;
float throttle_5_val = 0;


float steering_fuzzy(float box_x, float delta_blk_x);
float throttle_fuzzy(float box_h, float delta_blk_h);
float tmf(float val, float pt1, float pt2);
float tmfl(float val, float pt, float mid);
float tmfr(float val, float pt, float mid);

float delta_time = 0;
float last_time = 0;

float last_blk_h = 0;
float last_blk_x = 0;


//-------------------------------/



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Starting...\n");

  throttle.attach(3);
  throttle.writeMicroseconds(1500);
  steer.attach(5);
  steer.writeMicroseconds(1500);

  pixy.init();

}

void loop() {
  // put your main code here, to run repeatedly:

  delta_time = (float) millis() - (float) last_time;
  last_time = (float) millis();

  delta_time = (float) delta_time / 1000.0;


  int j;
  uint16_t blocks;
  char buf[32];

  // grab blocks!
  blocks = pixy.getBlocks();

  // If there are detect blocks, print them!
  if (blocks)
  {

    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino

    int blk_h = pixy.blocks[0].height;
    int blk_x = pixy.blocks[0].x;
    float delta_blk_h = (float) (pixy.blocks[0].height - last_blk_h) / delta_time;
    float delta_blk_x = (float) (pixy.blocks[0].x - last_blk_x) / delta_time;

    last_blk_h = (float) pixy.blocks[0].height;
    last_blk_x = (float) pixy.blocks[0].x;

    int throttle_value = 1500 - (int) throttle_fuzzy(blk_h, delta_blk_h);
    int steering_value = 1500 - (int) steering_fuzzy(blk_x, delta_blk_x);

    throttle.writeMicroseconds(throttle_value);
    steer.writeMicroseconds(steering_value);
  }


  delay(10);

}


//--------------------------------/


float steering_fuzzy(float box_h, float delta_blk_x) {

  float memberships[5] = {0, 0, 0, 0, 0};
  float delta_memberships[5] = {0, 0, 0, 0, 0};
  float adjusted_values[5] = {0, 0, 0, 0, 0};


  memberships[0] = tmfl(box_h, tri_steering_1_pt, tri_steering_1_mid);
  memberships[1] = tmf(box_h, tri_steering_2_pt1, tri_steering_2_pt2);
  memberships[2] = tmf(box_h, tri_steering_3_pt1, tri_steering_3_pt2);
  memberships[3] = tmf(box_h, tri_steering_4_pt1, tri_steering_4_pt2);
  memberships[4] = tmfr(box_h, tri_steering_5_pt, tri_steering_5_mid);

  delta_memberships[0] = tmfl(box_h, tri_delta_steering_1_pt, tri_delta_steering_1_mid);
  delta_memberships[1] = tmf(box_h, tri_delta_steering_2_pt1, tri_delta_steering_2_pt2);
  delta_memberships[2] = tmf(box_h, tri_delta_steering_3_pt1, tri_delta_steering_3_pt2);
  delta_memberships[3] = tmf(box_h, tri_delta_steering_4_pt1, tri_delta_steering_4_pt2);
  delta_memberships[4] = tmfr(box_h, tri_delta_steering_5_pt, tri_delta_steering_5_mid);

  adjusted_values[0] = (memberships[0]) * steering_1_val;
  adjusted_values[1] = (memberships[1]) * steering_2_val;
  adjusted_values[2] = (memberships[2]) * steering_3_val;
  adjusted_values[3] = (memberships[3]) * steering_4_val;
  adjusted_values[4] = (memberships[4]) * steering_5_val;


  return  adjusted_values[0] + adjusted_values[1] + adjusted_values[2] + adjusted_values[3] + adjusted_values[4];

}

float throttle_fuzzy(float box_h, float delta_blk_h) {

  float memberships[5] = {0, 0, 0, 0, 0};
  float delta_memberships[5] = {0, 0, 0, 0, 0};
  float adjusted_values[5] = {0, 0, 0, 0, 0};


  memberships[0] = tmfl(box_h, tri_throttle_1_pt, tri_throttle_1_mid);
  memberships[1] = tmf(box_h, tri_throttle_2_pt1, tri_throttle_2_pt2);
  memberships[2] = tmf(box_h, tri_throttle_3_pt1, tri_throttle_3_pt2);
  memberships[3] = tmf(box_h, tri_throttle_4_pt1, tri_throttle_4_pt2);
  memberships[4] = tmfr(box_h, tri_throttle_5_pt, tri_throttle_5_mid);

  delta_memberships[0] = tmfl(box_h, tri_delta_throttle_1_pt, tri_delta_throttle_1_mid);
  delta_memberships[1] = tmf(box_h, tri_delta_throttle_2_pt1, tri_delta_throttle_2_pt2);
  delta_memberships[2] = tmf(box_h, tri_delta_throttle_3_pt1, tri_delta_throttle_3_pt2);
  delta_memberships[3] = tmf(box_h, tri_delta_throttle_4_pt1, tri_delta_throttle_4_pt2);
  delta_memberships[4] = tmfr(box_h, tri_delta_throttle_5_pt, tri_delta_throttle_5_mid);

  adjusted_values[0] = (memberships[0]) * throttle_1_val;
  adjusted_values[1] = (memberships[1]) * throttle_2_val;
  adjusted_values[2] = (memberships[2]) * throttle_3_val;
  adjusted_values[3] = (memberships[3]) * throttle_4_val;
  adjusted_values[4] = (memberships[4]) * throttle_5_val;


  return  adjusted_values[0] + adjusted_values[1] + adjusted_values[2] + adjusted_values[3] + adjusted_values[4];

}



float tmf(float val, float pt1, float pt2)
{
  if (val < pt1 || val > pt2) {
    return 0.0;
  }

  int mid = (pt2 - pt1) / 2 + pt1;
  //printf("%d\n",mid);

  if (val > pt1 && val <= mid) {
    return (1 / (mid - pt1)) * val + pt1 * -1 * ((1 / (mid - pt1)));
  }
  else if (val < pt2 && val > mid) {
    return -1 * (1 / (pt2 - mid)) * val + pt2 * -1 * ((1 / (mid - pt2)));
  }
  return 0.0;
}

float tmfl(float val, float pt, float mid)
{
  if (val > pt) {
    return 0.0;
  }

  if (val <= pt && val >= mid) {
    return -1 * (1 / (pt - mid)) * val + pt * -1 * ((1 / (mid - pt)));
  }
  return 1.0;
}

float tmfr(float val, float pt, float mid)
{
  if (val < pt) {
    return 0.0;
  }

  if (val >= pt && val <= mid) {
    return (1 / (mid - pt)) * val + pt * -1 * ((1 / (mid - pt)));
  }
  return 1.0;
}
