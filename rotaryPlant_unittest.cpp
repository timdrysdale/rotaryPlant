//pid_unittest.cpp

#include "rotaryPlant.cpp"
#include <gtest/gtest.h>

TEST(RotaryPlantTest, wrap) {

  long ppr = 500;
  float lpf = 0;
  float timeToSeconds = 1e-6;
  
  RotaryPlant plant = RotaryPlant(ppr, lpf, timeToSeconds);
  
  EXPECT_EQ(0, plant.wrap(0));
  EXPECT_EQ(1, plant.wrap(1));
  EXPECT_EQ(248, plant.wrap(248));
  EXPECT_EQ(249, plant.wrap(249));
  EXPECT_EQ(250, plant.wrap(250));  //250 stays as 250
  EXPECT_EQ(-249, plant.wrap(251));
  EXPECT_EQ(-248, plant.wrap(252));
  EXPECT_EQ(11, plant.wrap(11));
  EXPECT_EQ(-248, plant.wrap(-248));
  EXPECT_EQ(-249, plant.wrap(-249));
  EXPECT_EQ(-250, plant.wrap(-250));  //-250 stays as -250 
  EXPECT_EQ(249, plant.wrap(-251));
  EXPECT_EQ(248, plant.wrap(-252));
  EXPECT_EQ(200, plant.wrap(-300));
  EXPECT_EQ(198, plant.wrap(-302));
  EXPECT_EQ(148, plant.wrap(-352));
  EXPECT_EQ(148, plant.wrap(-852)); //add a turn

  long input = (ppr * 2452345) + 75;
  EXPECT_EQ(75, plant.wrap(5075));
}
TEST(RotaryPlantTest, fractionalPosition) {

  long ppr = 500;
  float lpf = 0;
  float timeToSeconds = 1e-6;
  
  RotaryPlant plant = RotaryPlant(ppr, lpf, timeToSeconds);
  
  EXPECT_FLOAT_EQ(0, plant.fractionalPosition(0));
  EXPECT_FLOAT_EQ(0.002, plant.fractionalPosition(1));
  EXPECT_FLOAT_EQ(0.496, plant.fractionalPosition(248));
  EXPECT_FLOAT_EQ(0.498, plant.fractionalPosition(249));
  EXPECT_FLOAT_EQ(0.5, plant.fractionalPosition(250));  //250 stays as 250
  EXPECT_FLOAT_EQ(-0.498, plant.fractionalPosition(251));
  EXPECT_FLOAT_EQ(-0.496, plant.fractionalPosition(252));
  EXPECT_FLOAT_EQ(0.022, plant.fractionalPosition(11));
  EXPECT_FLOAT_EQ(-0.496, plant.fractionalPosition(-248));
  EXPECT_FLOAT_EQ(-0.498, plant.fractionalPosition(-249));
  EXPECT_FLOAT_EQ(-0.5, plant.fractionalPosition(-250));  //-250 stays as -250 
  EXPECT_FLOAT_EQ(0.498, plant.fractionalPosition(-251));
  EXPECT_FLOAT_EQ(0.496, plant.fractionalPosition(-252));
  EXPECT_FLOAT_EQ(0.4, plant.fractionalPosition(-300));
  EXPECT_FLOAT_EQ(0.396, plant.fractionalPosition(-302));
  EXPECT_FLOAT_EQ(0.296, plant.fractionalPosition(-352));
  EXPECT_FLOAT_EQ(0.296, plant.fractionalPosition(-852)); //add a turn

  long input = (ppr * 123123) + 75;
  EXPECT_FLOAT_EQ(0.15, plant.fractionalPosition(input));
}


TEST(RotaryPlantTest, positions) {

  long ppr = 500;
  float lpf = 0;
  float timeToSeconds = 1e-6;
  
  RotaryPlant plant = RotaryPlant(ppr, lpf, timeToSeconds);

  long dt = 20000; //20ms
  long t0 = 2435234525;
  long t1 = t0 + dt;

  long dc = 20;
  long c0 = (ppr *  3134) + 148;
  long c1 = c0 + dc;
  
  plant.initialise(t0,c0);

  float expected = 0.296; 

  float actual = plant.getPosition();
  
  EXPECT_FLOAT_EQ(expected, actual);
 
  plant.sample(t1,c1);

  expected = -0.464;
  
  actual = plant.getPosition();

  EXPECT_FLOAT_EQ(expected, actual);

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
