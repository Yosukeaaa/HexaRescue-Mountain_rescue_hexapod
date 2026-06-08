#ifndef GAITS_H
#define GAITS_H

#include "Leg.h"
#include "inverseKine.h"
#include "SSC32.h"

class Gaits {

public:
  Gaits();
  void tripodGaitForward(Leg legs[6], SSC32& ssc);
  void tripodGaitBackward(Leg legs[6], SSC32& ssc);
  void tripodGaitLeftTurn(Leg legs[6], SSC32& ssc);
  void tripodGaitRightTurn(Leg legs[6], SSC32& ssc);


  void neutralPosition(Leg legs[6], SSC32& ssc);
  void shutDown(Leg legs[6], SSC32& ssc);
  void turnOn(Leg legs[6], SSC32& ssc);

  void hello(Leg legs[6], SSC32& ssc);
  void twist(Leg legs[6], SSC32& ssc);
  void jump(Leg legs[6], SSC32& ssc);
};

#endif
