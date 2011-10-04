#include "arc.h"

#include <cmath>

#include "constants.h"

using namespace std;
using Constants::pi;

Arc::Arc()
{
  setRadiusNM(-1);
}

Arc::Arc(Coordinate center, double radiusNM, double angleStart, double angleEnd, char direction)
{
  setCenter(center);
  setRadiusNM(radiusNM);
  setStartAngle(angleStart);
  setEndAngle(angleEnd);
  setDirection(direction);
}

bool Arc::isValid() const
{
  if (getRadiusNM() > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Arc::invalidate()
{
  setRadiusNM(-1);
  setDirection('+');
}

/**
 * Return the radius in nautical miles.
 */
double Arc::getRadiusNM() const
{
  return radiusNM;
}

/**
 * Return the radius in metres (1 NM = 1852 m).
 */
double Arc::getRadiusM() const
{
  return radiusNM*1852.0;
}

/**
 * Set the radius to the specified value in Nautical Miles.
 */
void Arc::setRadiusNM(double r)
{
  radiusNM = r;
}

void Arc::setCenter(Coordinate c)
{
  center = c;
}

void Arc::setDirection(char d)
{
  direction = d;
}

char Arc::getDirection() const
{
  return direction;
}

void Arc::setStartAngle(double angle)
{
  angleStart = angle;
}

void Arc::setEndAngle(double angle)
{
  angleEnd = angle;
}

double Arc::getStartAngle() const
{
  return angleStart;
}

double Arc::getEndAngle() const
{
  return angleEnd;
}

const Coordinate& Arc::getCenter() const
{
  return center;
}

/*
 * See http://en.wikipedia.org/wiki/Latitude#Degree_length
 *
 * TODO: check if these calculatiosn (based on the oa2gm source code) are
 *       correct.  I guess not, because they both use 6371e3 for computing
 *       new latitudinal and longitudinal points.  This is not 100% correct.
 *       We should probably follow WGS84 or IERS 2003 ellipsoids.
 */
Polygon Arc::toPolygon(int nbPoints) const
{

  double deg_lat, deg_lon;
  double angle;
  Polygon p;

  Latitude lat = getCenter().getLatitude();
  Longitude lon = getCenter().getLongitude();

  // Reduce angles so that they are in [0, 360[.
  // TODO: this worked for integer angles, but not for floating point values...
  // So for now, we assume that all start and end angles are in [0, 360[.
  //double startAngle = getStartAngle() % 360;
  //double endAngle = getEndAngle() % 360;
  double startAngle = getStartAngle();
  double endAngle = getEndAngle();
  //cout << "startAngle = " << startAngle << endl;
  //cout << "endAngle= " << endAngle << endl;

  // Make sure that start angle is smaller than end angle.
  if (startAngle > endAngle)
  {
    startAngle = startAngle - 360;
    //cout << "Changing startAngle to " << startAngle << endl;
  }

  double interval;
  if (getDirection() == '+')
  {
    interval = endAngle - startAngle;
  }
  else
  {
    interval = -( 360-(endAngle-startAngle) );
  }

  // Compute arcdegree of latitude respectively longitude difference of the center.
  double arcdegree_lat = lat.getArcDegree();
  double arcdegree_lon = lon.getArcDegree(lat);

  // Generate all points of the arc.
  for (int i = 0; i < nbPoints; ++i)
  {
    // 'Normal' geometric angles are COUNTERCLOCKWISE as follows:
    //
    //   0 degrees = 360 degrees = East
    //   90 degrees = North
    //   180 degrees = West
    //   270 degrees = South
    //
    // The convention in the DA-records is CLOCKWISE and as follows:
    //
    //   0 degrees = 360 degrees = North
    //   90 degrees = East
    //   180 degrees = South
    //   270 degrees = West
    //
    // So we have to transform the coordinates from the DA-records
    // to 'normal' geometric angles.  We do this by first mirroring along
    // the vertical axis and then rotating -90 degrees.

    // Generate arc-points in airspace (DA-record) coordinate frame, centered
    // around the origin.
    // TODO: note that delta_lon uses arcdegree_lat and delta_lat uses arcdegree_lon... this
    //       is strange, but it seems to work... check out why!/
    angle = startAngle + (interval*i)/(nbPoints-1);
    double delta_lon = getRadiusM()*cos(pi*angle/180.0)/arcdegree_lat; // TODO: why arcdegree_lat here???
    double delta_lat = getRadiusM()*sin(pi*angle/180.0)/arcdegree_lon; // TODO: why arcdegree_lon here???

    // Transform points to standard coordinate frame:
    //
    //   Step 1: mirror around Y-axis
    //           -> transformation matrix = [-1 0; 0 1]
    //
    //   Step 2: rotate -90 degrees
    //           -> transformation matrix = [0 1; -1 0]
    //
    // The total transformation matrix is:
    //
    //    [0 1; -1 0]*[-1 0; 0 1] = [0 1; 1 0]
    // 
    // so what we actually do is simply swap x and y coordinates of the arc.
    double temp = delta_lon;
    delta_lon = delta_lat;
    delta_lat = temp;

    // Now add the coordinates of the generated circle to the location of the centerpoint
    // (= translate the generated circle from the origin to the correct location).
    deg_lon = lon.getAngle() + delta_lon;
    deg_lat = lat.getAngle() + delta_lat;

    Coordinate c(deg_lat, deg_lon);
    p.add(c);
  }

  return p;

}

ostream& operator <<(ostream& outputStream, const Arc& c)
{
  outputStream << "Arc:" << endl;
  outputStream << "  " << c.center << endl;
  outputStream << "  Radius in NM: " << c.radiusNM;
  return outputStream;
}