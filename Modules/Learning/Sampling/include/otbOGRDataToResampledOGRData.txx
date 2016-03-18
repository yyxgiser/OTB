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
#ifndef __otbOGRDataToResampledOGRData_txx
#define __otbOGRDataToResampledOGRData_txx

#include "otbMaskedIteratorDecorator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "otbOGRDataSourceWrapper.h"


namespace otb
{
// --------- otb::PersistentOGRDataToResampledOGRData ---------------------

template<class TMaskImage>
PersistentOGRDataToResampledOGRData<TMaskImage>
::PersistentOGRDataToResampledOGRData() :
  m_LayerIndex(0),
  m_MaxSamplingTabSize(1000),
  m_OutputVectorDataPath("")
{
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TMaskImage::New());
}

template<class TMaskImage>
void
PersistentOGRDataToResampledOGRData<TMaskImage>
::SetOGRData(const otb::ogr::DataSource* vector)
{
  this->SetNthInput(1, const_cast<otb::ogr::DataSource *>( vector ));
}

template<class TMaskImage>
const otb::ogr::DataSource*
PersistentOGRDataToResampledOGRData<TMaskImage>
::GetOGRData()
{
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const otb::ogr::DataSource *>(this->itk::ProcessObject::GetInput(1));
}

template<class TMaskImage>
void
PersistentOGRDataToResampledOGRData<TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->SetNthInput(0, const_cast<TMaskImage *>( mask ));
}

template<class TMaskImage>
const TMaskImage*
PersistentOGRDataToResampledOGRData<TMaskImage>
::GetMask()
{
  if (this->GetNumberOfInputs()<1)
    {
      std::cout << "sqdlkcjnamljb" << std::endl;
    return 0;
  
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(0));
}

template<class TMaskImage>
void
PersistentOGRDataToResampledOGRData<TMaskImage>
::Synthetize(void)
{
  otb::ogr::DataSource* vectors = const_cast<otb::ogr::DataSource*>(this->GetOGRData());
  vectors->GetLayer(m_LayerIndex).SetSpatialFilter(NULL);
}

template<class TMaskImage>
void
PersistentOGRDataToResampledOGRData<TMaskImage>
::Reset(void)
{
  // Get OGR field index
  const otb::ogr::DataSource* vectors = this->GetOGRData();
  otb::ogr::Layer::const_iterator featIt = vectors->GetLayer(m_LayerIndex).begin();
  int fieldIndex = featIt->ogr().GetFieldIndex(this->m_FieldName.c_str());
  if (fieldIndex < 0)
    {
    itkGenericExceptionMacro("Field named "<<this->m_FieldName<<" not found!");
    }

  m_TemporaryStats = OGRDataResampler::New();
  m_TemporaryStats->SetFieldIndex(fieldIndex);
  m_TemporaryStats->SetFieldName(this->GetFieldName());
  m_TemporaryStats->SetMaxSamplingTabSize(m_MaxSamplingTabSize);
  m_TemporaryStats->SetInputOGRDataSourcePointer(vectors);
  m_TemporaryStats->SetLayerIndex(this->GetLayerIndex());
  m_TemporaryStats->SetOutputPath(this->GetOutputVectorDataPath());
  m_TemporaryStats->SetMaxSamplingTabSize(m_MaxSamplingTabSize);
  if (!m_OutputSamplingVectorsPath.empty())
      m_TemporaryStats->SetOutputSamplingVectorsPath(m_OutputSamplingVectorsPath);
  if (!m_InputSamplingVectorsPath.empty())
      m_TemporaryStats->SetInputSamplingVectorsPath(m_InputSamplingVectorsPath);
  
  
  if (m_RatesbyClass.empty())
     {itkGenericExceptionMacro("m_RatesbyClass is empty. Use SetRatesbyClass to provide some vector statistics information.");}
  else
     {
       m_TemporaryStats->SetRatesbyClass(m_RatesbyClass); 
       m_TemporaryStats->Prepare();
       
       m_TemporaryStats->PrepareOutputOGRData();
     }
}


template<class TMaskImage>
itk::DataObject::Pointer
PersistentOGRDataToResampledOGRData<TMaskImage>
::MakeOutput(DataObjectPointerArraySizeType idx)
{
  switch (idx)
    {
    case 0:
      return static_cast<itk::DataObject*>(TMaskImage::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TMaskImage::New().GetPointer()); 
      break;
    }
}



template<class TMaskImage>
void
PersistentOGRDataToResampledOGRData<TMaskImage>
::GenerateInputRequestedRegion()
{
  MaskImageType *mask = const_cast<MaskImageType*>(this->GetMask());

  RegionType requested = this->GetOutput()->GetRequestedRegion();
  RegionType emptyRegion = mask->GetLargestPossibleRegion();
  emptyRegion.SetSize(0,0);
  emptyRegion.SetSize(1,0);

  mask->SetRequestedRegion(emptyRegion);

}



template<class TMaskImage>
void
PersistentOGRDataToResampledOGRData<TMaskImage>
::GenerateData()
{
  // Retrieve inputs
  TMaskImage* mask = const_cast<TMaskImage*>(this->GetInput());
  TMaskImage* outputImage = this->GetOutput();
  const otb::ogr::DataSource* vectors = this->GetOGRData();
  PointType point;
  RegionType requestedRegion =outputImage->GetRequestedRegion();
  
  this->ApplyPolygonsSpatialFilter();
  
  ogr::Layer layer = vectors->GetLayer(m_LayerIndex);

  // Loop across the features in the layer (filtered by requested region in BeforeTGD already)
  otb::ogr::Layer::const_iterator featIt = layer.begin(); 
  for(; featIt!=layer.end(); ++featIt)
    {
    // Compute the intersection of thread region and polygon bounding region, called "considered region"
    // This need not be done in ThreadedGenerateData and could be pre-processed and cached before filter execution if needed
    RegionType consideredRegion = FeatureBoundingRegion(mask, featIt);
    bool regionNotEmpty = consideredRegion.Crop(requestedRegion);
    if (regionNotEmpty)
      {
         typedef itk::ImageRegionConstIteratorWithIndex<TMaskImage> IteratorType;
         IteratorType it(mask,consideredRegion);
         
         m_TemporaryStats->Add<IteratorType>(featIt, it, mask);

      }
    }
}

template<class TMaskImage>
void
PersistentOGRDataToResampledOGRData<TMaskImage>
::ApplyPolygonsSpatialFilter()
{
  TMaskImage* outputImage = this->GetOutput();
  otb::ogr::DataSource* vectors = const_cast<otb::ogr::DataSource*>(this->GetOGRData());
  const RegionType& requestedRegion = outputImage->GetRequestedRegion();
  typename TMaskImage::IndexType startIndex = requestedRegion.GetIndex();
  typename TMaskImage::IndexType endIndex = requestedRegion.GetUpperIndex();

  itk::Point<double, 2> startPoint;
  itk::Point<double, 2> endPoint;

  outputImage->TransformIndexToPhysicalPoint(startIndex, startPoint);
  outputImage->TransformIndexToPhysicalPoint(endIndex, endPoint);

  vectors->GetLayer(m_LayerIndex).SetSpatialFilterRect(
    std::min(startPoint[0],endPoint[0]),
    std::min(startPoint[1],endPoint[1]),
    std::max(startPoint[0],endPoint[0]),
    std::max(startPoint[1],endPoint[1]));
}

template<class TMaskImage>
typename PersistentOGRDataToResampledOGRData<TMaskImage>::RegionType
PersistentOGRDataToResampledOGRData<TMaskImage>
::FeatureBoundingRegion(const TMaskImage* image, otb::ogr::Layer::const_iterator& featIt) const
{
  // otb::ogr wrapper is incomplete and leaky abstraction is inevitable here
  OGREnvelope envelope;
  featIt->GetGeometry()->getEnvelope(&envelope);
  itk::Point<double, 2> lowerPoint, upperPoint;
  lowerPoint[0] = envelope.MinX;
  lowerPoint[1] = envelope.MinY;
  upperPoint[0] = envelope.MaxX;
  upperPoint[1] = envelope.MaxY;

  typename TMaskImage::IndexType lowerIndex;
  typename TMaskImage::IndexType upperIndex;

  image->TransformPhysicalPointToIndex(lowerPoint, lowerIndex);
  image->TransformPhysicalPointToIndex(upperPoint, upperIndex);

  // swap coordinate to keep lowerIndex as start index
  if (lowerIndex[0] > upperIndex[0])
    {
    int tmp = lowerIndex[0];
    lowerIndex[0] = upperIndex[0];
    upperIndex[0] = tmp;
    }
  if (lowerIndex[1] > upperIndex[1])
    {
    int tmp = lowerIndex[1];
    lowerIndex[1] = upperIndex[1];
    upperIndex[1] = tmp;
    }

  RegionType region;
  region.SetIndex(lowerIndex);
  region.SetUpperIndex(upperIndex);

  return region;
}

// -------------- otb::OGRDataToResampledOGRData --------------------------

template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetInput(const TMaskImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TMaskImage>
const TMaskImage*
OGRDataToResampledOGRData<TMaskImage>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template<class TMaskImage>
const otb::ogr::DataSource*
OGRDataToResampledOGRData<TMaskImage>
::GetOGRData()
{
  return this->GetFilter()->GetOGRData();
}

template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->GetFilter()->SetMask(mask);
}

template<class TMaskImage>
const TMaskImage*
OGRDataToResampledOGRData<TMaskImage>
::GetMask()
{
  return this->GetFilter()->GetMask();
}

template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetFieldName(std::string &key)
{
  this->GetFilter()->SetFieldName(key);
}

template<class TMaskImage>
std::string
OGRDataToResampledOGRData<TMaskImage>
::GetFieldName()
{
  return this->GetFilter()->GetFieldName();
}

template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetLayerIndex(int index)
{
  this->GetFilter()->SetLayerIndex(index);
}

template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetOutputSamplingVectorsPath(std::string path)
{
  this->GetFilter()->SetOutputSamplingVectorsPath(path);
}

template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetInputSamplingVectorsPath(std::string path)
{
  this->GetFilter()->SetInputSamplingVectorsPath(path);
}

template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetOutputVectorDataPath(std::string path)
{
  this->GetFilter()->SetOutputVectorDataPath(path);
}


template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetMaxSamplingTabSize(unsigned int max)
{
  this->GetFilter()->SetMaxSamplingTabSize(max);
}


template<class TMaskImage>
void
OGRDataToResampledOGRData<TMaskImage>
::SetRatesbyClass(const SamplingRateCalculator::mapRateType& map)
{
     this->GetFilter()->SetRatesbyClass(map);
     
}

template<class TMaskImage>
int
OGRDataToResampledOGRData<TMaskImage>
::GetLayerIndex()
{
  return this->GetFilter()->GetLayerIndex();
}


template<class TMaskImage>
unsigned int
OGRDataToResampledOGRData<TMaskImage>
::GetMaxSamplingTabSize()
{
  return this->GetFilter()->GetMaxSamplingTabSize();
}

template<class TMaskImage>
const std::string*
OGRDataToResampledOGRData<TMaskImage>
::GetOutputVectorDataPath() const
{
  return this->GetFilter()->GetOutputVectorDataPath();
}

template<class TMaskImage>
std::string*
OGRDataToResampledOGRData<TMaskImage>
::GetOutputVectorDataPath()
{
  return this->GetFilter()->GetOutputVectorDataPath();
}

} // end of namespace otb

#endif
