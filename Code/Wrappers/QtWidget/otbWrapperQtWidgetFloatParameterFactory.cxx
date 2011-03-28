/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbWrapperQtWidgetFloatParameterFactory.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperQtWidgetFloatParameter.h"

namespace otb
{

namespace Wrapper
{

QtWidgetFloatParameterFactory::QtWidgetFloatParameterFactory()
{}

QtWidgetFloatParameterFactory::~QtWidgetFloatParameterFactory()
{}

QWidget * QtWidgetFloatParameterFactory::CreateQtWidget(Parameter * param)
{
  // Try to cast to int parameter
  FloatParameter * floatParam = dynamic_cast<FloatParameter *>(param);

  // Check if dynamic cast succeeds
  if(!floatParam)
    {
    return 0;
    }
  
  return new QtWidgetFloatParameter(floatParam);
}
}
}
