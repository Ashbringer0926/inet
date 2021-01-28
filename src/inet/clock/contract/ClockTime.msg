cplusplus {{
#include "inet/clock/contract/ClockTime.h"
}}

import inet.common.INETDefs;

namespace inet;

class clocktime_t
{
    @existingClass;
    @overwritePreviousDefinition;
    @descriptor(false);
    @opaque;
    @byValue;
    @cppType(inet::clocktime_t);
    @fromString(SIMTIME_AS_CLOCKTIME(string2simtime($))); @toString(simtime2string(CLOCKTIME_AS_SIMTIME($)));
    @defaultValue(CLOCKTIME_ZERO);
}

