/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbImageFileReader.h"

namespace otb {

ImageFileReaderException::ImageFileReaderException(const char *file, unsigned int line,
                                                   const std::string& desc,
                                                   const std::string& filename) :
    ExceptionObject(file, line, desc),
    m_Filename(filename)
{
}

ImageFileReaderException::~ImageFileReaderException()
{
}

// Explicit instanciation of common types
template class ImageFileReader<Image<unsigned int, 2>>;
template class ImageFileReader<Image<int, 2>>;
template class ImageFileReader<Image<unsigned char, 2>>;
template class ImageFileReader<Image<char, 2>>;
template class ImageFileReader<Image<unsigned short, 2>>;
template class ImageFileReader<Image<short, 2>>;
template class ImageFileReader<Image<float, 2>>;
template class ImageFileReader<Image<double, 2>>;

}