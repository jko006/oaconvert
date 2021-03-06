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

#ifndef POLISHSTATE_H
#define POLISHSTATE_H

#include <vector>
#include "OutputState.h"
#include "Constants.h"
#include "Coordinate.h"


class PolishState : public OutputState
{

public:

    static PolishState* getInstance();

    virtual void writeHeader(std::ostream &out) const;
    virtual void writeFooter(std::ostream& ) const;

    //! Use this method to write out an airspace.
    virtual void write(std::ostream& stream, const Airspace& airspace) const;


protected:

    PolishState();

private:
    static PolishState* _instance;

    //! Helper method to write out an airspace
    void write(std::ostream& stream, const CurvedPolygon& p) const;
    void write(std::ostream& stream, const std::vector<Coordinate>& coords) const;
    void write(std::ostream& out, const Coordinate& c) const;

    std::string getPolygonType(const Airspace& space) const;
    std::string getLineType(const Airspace& space) const;
    std::string getPolishLabel(const Airspace& space) const;

};

#endif /* POLISHSTATE_H */
