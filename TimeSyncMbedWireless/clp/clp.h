#pragma once

#include "clp_api.h"

// Put this define before an include of this file to enable extended clp features
#ifdef _CLP_USE_EXTENSIONS_
  #include "clp_api_ext.h"
#endif

// Put this define before an include of this file to enable serial packet sending
#ifdef _CLP_USE_SERIAL_
  #include "clp_serial.h"
#endif