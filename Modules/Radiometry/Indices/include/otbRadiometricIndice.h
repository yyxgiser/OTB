/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbRadiometricIndice_h
#define otbRadiometricIndice_h

#include "itkVariableLengthVector.h"
#include <array>
#include <set>
#include <string>

namespace otb
{
namespace Functor
{
enum class Band {COASTAL_BLUE, BLUE, GREEN, RED, RED_EDGE, NIR, SWIR, MIR};

constexpr size_t NumberOfBands = 8;

template <typename TInput, typename TOutput>
class RadiometricIndice
{
public:
  RadiometricIndice(const std::string & name, const std::set<Band>& requiredBands)
    : m_RequiredBands(),
      m_BandIndices(),
    : m_Name(name)
  {
    m_RequiredBands.fill(false);
    m_BandIndices.fill(0);

    for(auto b : requiredBands)
      {
      m_RequiredBands[static_cast<size_t>(b)]=true;
      }
  }
  
  ~RadiometricIndice() = default;
  
  std::set<Band> GetRequiredBands() const
  {
    std::set<Band> resp;
    for(auto i = 0; i < NumberOfBands; ++i)
      {
      resp.insert(static_cast<Band>(i));
      }

    return resp;
  }

  void SetBandIndex(const Band & band, const size_t & index)
  {
    m_BandIndices[static_cast<size_t>(band)]=index;
  }

  const std::string & GetName() const
  {
    return m_Name;
  }

  virtual TOutput operator()(const itk::VariableLengthVector<TInput> & input) const = 0;
 

protected:
  size_t BandIndex(const Band & band) const
  {
    // TODO: No checks here if band index is really set
    
    return m_BandIndices[static_cast<size_t>(band)];
  }
  
  double Value(const Band & band, const itk::VariableLengthVector<TInput> & input) const
  {
    return static_cast<double>(input[BandIndex(band)]);
  }

  static const double    EpsilonToBeTreatedAsZero = 0.0000001;

private:
  using RequiredBandsContainer = std::array<bool,NumberOfBands>;
  using BandIndicesContainer   = std::array<size_t,NumberOfBands>;

  RadiometricIndice() = delete;

  RequiredBandsContainer m_RequiredBands;
  BandIndicesContainer   m_BandIndices;
  std::string            m_Name;
};

} // End namespace Indices
} // End namespace otb

#endif
