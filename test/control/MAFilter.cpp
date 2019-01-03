#include <eeros/control/MAFilter.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/math/Matrix.hpp>

#include <gtest/gtest.h>

#include <iostream>
using namespace eeros;
using namespace eeros::control;

TEST(MAFilterUnitTest, templateInstantiations) {
  double dcoeffs[] = {0.5, 0.5};
  MAFilter<2> f1{dcoeffs};
  
  double d5coeffs[] = {0.2, 0.2, 0.2, 0.2, 0.2};
  MAFilter<5> f2{d5coeffs};
  
  float fcoeffs[] = {0.1, 0.1};
  MAFilter<2,int,float> f3{fcoeffs};
  
  int icoeffs[] = {1, 1}; 
  MAFilter<2,int> f4{icoeffs};
  
  using namespace math; 
  MAFilter<2,Matrix<2,2>, double> f5{dcoeffs};
  ASSERT_TRUE(true); // they would fail at compile time.
}


TEST(MAFilterUnitTest, doubleMAFilter) {
  double coeffs[] = {0.5, 0.5};
  MAFilter<2> ma{coeffs};
  
  Constant<> c1{2};
  c1.run();
  ma.getIn().connect(c1.getOut());
  
  // also tests if enable is true by default.
  
  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue(), 1);
  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue(), 2);
  ma.run();
  ma.run();
  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue(), 2);
  
  ASSERT_EQ (c1.getOut().getSignal().getTimestamp(), ma.getOut().getSignal().getTimestamp());
}


TEST(MAFilterUnitTest, enableDisable) {
  double coeffs[] = {0.5, 0.5};
  MAFilter<2> ma{coeffs};
  
  Constant<> c1{5};
  c1.run();
  ma.getIn().connect(c1.getOut());
  ma.disable();

  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue(), 5);
  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue(), 5);
  
  ma.enable();
  ma.run();
  // previous values will be stored regardless if filter is enabled or not.
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue(), 5);
  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue(), 5);
  
  ASSERT_EQ (c1.getOut().getSignal().getTimestamp(), ma.getOut().getSignal().getTimestamp());
}


TEST(MAFilterUnitTest, vectorMAFilter) {
  using namespace math;
  
  double coeffs[] = {0.1, 0.2, 0.3, 0.4};
  MAFilter<4, Vector3, double> ma{coeffs};
  
  Vector3 vec3{};
  vec3 << 1, 2, 3;
  Constant<Vector3> c1{vec3};
  c1.run();
  ma.getIn().connect(c1.getOut());
  
  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[0], 0.4);
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[1], 0.8);
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[2], 1.2);
  
  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[0], 0.7);
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[1], 1.4);
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[2], 2.1);
  
  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[0], 0.9);
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[1], 1.8);
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[2], 2.7);
  
  ma.run();
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[0], 1);
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[1], 2);
  EXPECT_DOUBLE_EQ (ma.getOut().getSignal().getValue()[2], 3);
  
  ASSERT_EQ (c1.getOut().getSignal().getTimestamp(), ma.getOut().getSignal().getTimestamp());
}
