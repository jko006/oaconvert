/*
  Copyright 2011 Bart Vandewoestyne, Yves Frederix.
  
  This file is part of oaconvert.
  
  oaconvert is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  oaconvert is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with oaconvert.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "LatitudeTest.h"
#include "Latitude.h"
#include "Longitude.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LatitudeTest );

void
LatitudeTest::setUp()
{
}

void
LatitudeTest::tearDown()
{
}

void
LatitudeTest::testConstructors()
{
  Latitude lat;


  lat = Latitude(12.34);
  CPPUNIT_ASSERT_EQUAL('N', lat.getDirection());

  lat = Latitude(-12.34);
  CPPUNIT_ASSERT_EQUAL('S', lat.getDirection());


  lat = Latitude(12.34, 'N');
  CPPUNIT_ASSERT_EQUAL('N', lat.getDirection());

  lat = Latitude(-12.34, 'N');
  CPPUNIT_ASSERT_EQUAL('N', lat.getDirection());

  lat = Latitude(12.34, 's');
  CPPUNIT_ASSERT_EQUAL('S', lat.getDirection());

  lat = Latitude(-12.34, 's');
  CPPUNIT_ASSERT_EQUAL('S', lat.getDirection());


  lat = Latitude(12, 34.56, 'N');
  CPPUNIT_ASSERT_EQUAL('N', lat.getDirection());

  lat = Latitude(-12, 34.56, 'N');
  CPPUNIT_ASSERT_EQUAL('N', lat.getDirection());

  lat = Latitude(12, 34.56, 's');
  CPPUNIT_ASSERT_EQUAL('S', lat.getDirection());

  lat = Latitude(-12, 34.56, 's');
  CPPUNIT_ASSERT_EQUAL('S', lat.getDirection());


  lat = Latitude(12, 34, 56.78, 'N');
  CPPUNIT_ASSERT_EQUAL('N', lat.getDirection());

  lat = Latitude(-12, 34, 56.78, 'N');
  CPPUNIT_ASSERT_EQUAL('N', lat.getDirection());

  lat = Latitude(12, 34, 56.78, 's');
  CPPUNIT_ASSERT_EQUAL('S', lat.getDirection());

  lat = Latitude(-12, 34, 56.78, 's');
  CPPUNIT_ASSERT_EQUAL('S', lat.getDirection());
}

void
LatitudeTest::testGetDegrees()
{
  Latitude lat;

  lat = Latitude(-12.34);
  CPPUNIT_ASSERT_EQUAL(12, lat.getDegrees());

  lat = Latitude(12.34);
  CPPUNIT_ASSERT_EQUAL(12, lat.getDegrees());

  lat = Latitude(-12, 34, 'N');
  CPPUNIT_ASSERT_EQUAL(12, lat.getDegrees());

  lat = Latitude(12, 34, 'N');
  CPPUNIT_ASSERT_EQUAL(12, lat.getDegrees());
}


/*
 * Tests based on values returned from
 * http://www.csgnetwork.com/degreelenllavcalc.html
 */
void
LatitudeTest::testGetArcDegree()
{
  // The best we can agree with the results from the above website...
  double tol = 491.0;

  // Longitude doesn't really matter.
  Longitude lon(45, 0, 0, 'N');

  Latitude lat;

  lat = Latitude(45, 0, 0, 'N');
  CPPUNIT_ASSERT_DOUBLES_EQUAL(111131.745, lat.getArcDegree(), tol);

  lat = Latitude(20, 0, 0, 'N');
  CPPUNIT_ASSERT_DOUBLES_EQUAL(110704.27818646189, lat.getArcDegree(), tol);

  lat = Latitude(60, 0, 0, 'N');
  CPPUNIT_ASSERT_DOUBLES_EQUAL(111412.24020000001, lat.getArcDegree(), tol);
}
