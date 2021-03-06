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

#ifndef ARC_H
#define ARC_H

#include "Point.h"
#include "GeometricShape.h"

class Arc : public GeometricShape
{

private:

    /* Radius in Nautical Miles. */
    double radiusNM;

    /* Arc center. */
    Point center;

    /* Start and end angle of the arc, following airspace coordinate frame conventions!!! */
    double angleStart, angleEnd;

    /* Direction in which the arc should be drawn:
        '+' clockwise (default)
        '-' counterclockwise */
    char direction;

public:

    /* Create an (invalid) arc. */
    Arc();

    virtual ~Arc() {};

    /* Create an arc around a given coordinate, with radius in nautical miles */
    Arc(const Point& center, double radius, double angleStart, double angleEnd, char direction);

    double getRadiusNM() const;
    double getRadiusM() const;
    const Point& getCenter() const;
    double getStartAngle() const;
    double getEndAngle() const;
    char getDirection() const;

    friend std::ostream& operator <<(std::ostream& outputStream, const Arc& c);

    virtual void discretize( std::vector<Coordinate>& coords, double resolution ) const;

};

#endif /* ARC_H */
