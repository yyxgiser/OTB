/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mvdImageViewRenderer.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtOpenGL>

//
// System includes (sorted by alphabetic order)
// necessary for the opengl variables and methods

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbStandardShader.h"
#include "otbGlImageActor.h"
#include "otbGlVersionChecker.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractLayerModel.h"
#include "Core/mvdAlgorithm.h"
#include "Core/mvdTypes.h"
#include "Core/mvdVectorImageModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::ImageViewRenderer

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
ImageViewRenderer
::ImageViewRenderer( QObject* parent ) :
  AbstractImageViewRenderer( parent ),
  m_GlView( otb::GlView::New() ),
  m_EffectsEnabled( true )
#ifdef _WIN32
#else // _WIN32
  ,  m_ReferencePair( NULL, otb::GlActor::Pointer() )
#endif // _WIN32
  // m_ModelActorPairs(),
{
  // assert( !m_GlView.IsNull() );

  // assert( m_GlView->GetSettings()!=NULL );
  // m_GlView->GetSettings()->SetUseProjection( true );

#ifdef _WIN32
  m_ReferencePair.first = NULL;
  // m_ReferencePair.second is initialized by otb::GlActor::Pointer default constructor.
#else // _WIN32
#endif // _WIN32
}

/*****************************************************************************/
ImageViewRenderer
::~ImageViewRenderer()
{
}

/*****************************************************************************/
bool
ImageViewRenderer
::CheckGLCapabilities( int * glsl140 ) const
{
#if USE_REMOTE_DESKTOP_DISABLED_RENDERING
  return true;

#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING

  //
  // Trace required OpenGL and GLSL versions.
  qWarning() <<
    ToStdString(
      tr( "Required OpenGL version '%1' with GLSL version '%2'." )
      .arg( otb::GlVersionChecker::REQUIRED_GL_VERSION )
      .arg( otb::GlVersionChecker::REQUIRED_GLSL_VERSION )
    ).c_str();

  //
  // Get and check OpenGL and GLSL versions.

  const char * glVersion = NULL;
  const char * glslVersion = NULL;

  bool isOk = false;

  try
    {
    isOk = otb::GlVersionChecker::CheckGLCapabilities( glVersion, glslVersion );

    if( glsl140!=NULL )
      *glsl140 = otb::GlVersionChecker::VerCmp( glslVersion, "1.40" );

    //
    // Trace runtime OpenGL and GLSL versions.
    qWarning() <<
    ToStdString(
      tr( "Runtime OpenGL version '%1' with GLSL version '%2'." )
      .arg( glVersion )
      .arg( glslVersion )
    ).c_str();
    }
  catch( std::exception& exc )
    {
    QMessageBox::critical(
      qobject_cast< QWidget* >( parent() ),
      tr( "Critical error!"),
      ToQString( exc.what() )
    );
    }

  //
  // Return if check has succeeded.
  if( isOk )
    return true;

  //
  // Construct message.
  QString message(
    // tr( "Current OpenGL version is '%1' supporting GLSL version '%2'.\nRequired OpenGL version is '%3' with Shading-Laguage version '%4'.\nPlease upgrade your graphics driver and hardware for the application to run properly on this platform.\nUsing the application on this platform may lead to unknown behaviour. Would you still like to continue using the application?" )
    tr( "Current OpenGL version is '%1' supporting OpenGL Shading-Language (GLSL) version '%2'.\nRequired OpenGL version is at least '%3' with GLSL version at least '%4'.\nPlease upgrade your graphics driver and/or hardware for the application to run properly on this platform.\nIf you are running the application under some remote-desktop service, runtime OpenGL and GLSL versions may differ from those running directly on remote platform.\nPlease contact your system administrator.\nApplication will now exit!" )
    .arg( glVersion )
    .arg( glslVersion )
    .arg( otb::GlVersionChecker::REQUIRED_GL_VERSION )
    .arg( otb::GlVersionChecker::REQUIRED_GLSL_VERSION )
  );

  //
  // Warn user is check has failed.
  qWarning() << ToStdString( message ).c_str();

  QMessageBox::critical(
    qobject_cast< QWidget* >( parent() ),
    tr( "Critical error!" ),
    message
  );

  //
  // KO.
  return false;

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING
}

/*****************************************************************************/
AbstractImageViewRenderer::RenderingContext*
ImageViewRenderer
::NewRenderingContext() const
{
  RenderingContext* context = new ImageViewRenderer::RenderingContext();

#if USE_VIEW_SETTINGS_SIDE_EFFECT
#else
  assert( !m_GlView.IsNull() );

  //
  // Share otb::GlViewRendering settings with manipulator using
  // RenderingContext. Manipulator can then setup otb::ViewSettings
  // directly by side-effect.
  context->m_ViewSettings = m_GlView->GetSettings();
#endif

  return context;
}

/*****************************************************************************/
const AbstractLayerModel *
ImageViewRenderer
::GetReferenceModel() const
{
  return m_ReferencePair.first;
}

/*****************************************************************************/
AbstractLayerModel*
ImageViewRenderer
::GetReferenceModel()
{
  return m_ReferencePair.first;
}

/*****************************************************************************/
void
ImageViewRenderer
::GetLayerExtent( const StackedLayerModel::KeyType & key,
                  PointType & origin,
                  PointType & extent  ) const
{
  assert( !m_GlView.IsNull() );

  otb::GlActor::Pointer actor( m_GlView->GetActor( key ) );

  if( actor.IsNull() )
    {
    origin[ 0 ] = origin[ 1 ] = 0;

    extent[ 0 ] = extent[ 1 ] = 0;

    return;
    }

  actor->GetExtent( origin[ 0 ], origin[ 1 ], extent[ 0 ], extent[ 1 ] );
}

/*****************************************************************************/
void
ImageViewRenderer
::GetReferenceExtent( PointType& origin, PointType& extent  ) const
{
  assert( GetLayerStack()!=NULL );

  GetLayerExtent(
    GetLayerStack()->GetKey( GetLayerStack()->GetReferenceIndex() ),
    origin,
    extent
  );
}

/*****************************************************************************/
void
ImageViewRenderer
::GetViewExtent( PointType & origin, PointType & extent ) const
{
  assert( !m_GlView.IsNull() );

  m_GlView->GetExtent( origin, extent );
}

/*****************************************************************************/
void
ImageViewRenderer
::InitializeGL()
{
  assert( !m_GlView.IsNull() );

  //
  // WARNING: This call to otb::GlView::Initialize() allocates a new
  // otb::ViewSettings() instance. So, the shared pointer between
  // renderer and manipulator may be unshated depending on order of
  // calls.
  //
  m_GlView->Initialize( 0, 0 );
}

/*****************************************************************************/
void
ImageViewRenderer
::ResizeGL( int width, int height )
{
  // qDebug() << this << "::ResizeGL(" << width << "," << height << ")";

  /*
  // Should be done here and not in OTB-Ice render routine.
  glViewport(
    0,
    0,
    static_cast< GLint >( width ),
    static_cast< GLint >( height )
  );
  */

  assert( !m_GlView.IsNull() );

  // qDebug() << m_GlView.GetPointer();

  m_GlView->Resize( width, height );

  /*
  glViewport(
    0, 0,
    static_cast< GLint >( width ), static_cast< GLint >( height )
  );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho(
    0, static_cast< GLint >( width ),
    0, static_cast< GLint >( height ),
    0, 1
  );
  */
}

/*****************************************************************************/
void
ImageViewRenderer
::PaintGL( const AbstractImageViewRenderer::RenderingContext* c )
{
  assert( !m_GlView.IsNull() );

  // qDebug() << this << "::PaintGL(" << c << ")";
  // qDebug() << "{";

  // qDebug() << m_GlView.GetPointer();

  /*
  //
  // Get appropriate rendering-context.
  assert( c==dynamic_cast< RenderingContext* >( c ) );
  RenderingContext* context = dynamic_cast< RenderingContext* >( c );
  assert( context!=NULL );

  //
  // Set view settings.
  m_GlView->SetSettings( context->m_ViewSettings );
  */

#if USE_REMOTE_DESKTOP_DISABLED_RENDERING && 0

#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING
  //
  // Apply VectorImageSettings to otb::GlImageActor.
  UpdateActors( c );

  //
  // Bypass rendering if needed.
  if( IsBypassRenderingEnabled() )
    {
    // qDebug() << "}\n";

    return;
    }

  // qDebug() << m_GlView.GetPointer() << "::BeforeRendering()";
  // qDebug() << "{";

  //
  // Pre-render scene.
  m_GlView->BeforeRendering();
  {
  //
  // Render scene.
  switch( c->m_RenderMode )
    {
    case RenderingContext::RENDER_MODE_LIGHT:
      // qDebug() << "otb::GlView::LightRender()";
      m_GlView->LightRender();
      break;

    case RenderingContext::RENDER_MODE_FULL:
      // qDebug() << "otb::GlView::HeavyRender()";
      m_GlView->HeavyRender();
      break;

    default:
      assert( false && "Unhandled RenderingContext::RenderModel value!" );
      break;
    }
  }
  //
  // Post-render scene.
  m_GlView->AfterRendering();

  // qDebug() << "}";
  // qDebug() << m_GlView.GetPointer() << "::AfterRendering()";

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING

  // qDebug() << "}\n";
}

/*****************************************************************************/
void
ImageViewRenderer
::Pick( const PointType & ptView,
	PixelInfo::Vector & pixels ) const
{
  // qDebug()
  //   << this << "::Pick("
  //   << ptView[ 0 ] << "," << ptView[ 1 ]
  //   << ")";

  assert( !m_GlView.IsNull() );

  //
  // Get actor keys.
  otb::GlView::StringVectorType keys( m_GlView->GetRenderingOrder() );

  // Prepare picked pixels container.
  pixels.resize( keys.size() );

  // Pick each layer.
  size_t i = 0;

  for( otb::GlView::StringVectorType::const_iterator it( keys.begin() );
       it != keys.end();
       ++ it, ++ i )
    {
    // Register layer key.
    pixels[ i ].m_Key = *it;

    // Get actor.
    otb::GlActor::Pointer actor( m_GlView->GetActor( *it ) );
    assert( !actor.IsNull() );

    // Get geo-interface.
    const otb::GeoInterface * geoInterface =
      dynamic_cast< const otb::GeoInterface * >( actor.GetPointer() );

    // If geo-interface...
    if( geoInterface!=NULL )
      {
      // ...compute physical point.
      pixels[ i ].m_HasPoint =
	geoInterface->TransformFromViewport( pixels[ i ].m_Point, ptView, true );
      }
    else
      {
      pixels[ i ].m_HasPoint = false;
      pixels[ i ].m_Point = PointType();
      }

    // If image-actor...
    otb::GlImageActor::Pointer imageActor( otb::DynamicCast< otb::GlImageActor >( actor ) );

    if( !imageActor.IsNull() )
      {
      // ...get pixel and it's index...
      pixels[ i ].m_HasIndex =
      pixels[ i ].m_HasPixel =
	imageActor->GetPixel(
	  pixels[ i ].m_Point,
	  pixels[ i ].m_Pixel,
	  pixels[ i ].m_Index
	);

      // ...and get resolutions.
      pixels[ i ].m_HasResolution = true;
      pixels[ i ].m_Resolution = imageActor->GetCurrentResolution();
      }
    }
}

/*****************************************************************************/
void
ImageViewRenderer
::GetResolutions( PixelInfo::Vector & pixels ) const
{
  // qDebug() << this << "::GetResolutions()";

  assert( !m_GlView.IsNull() );

  //
  // Get actor keys.
  otb::GlView::StringVectorType keys( m_GlView->GetRenderingOrder() );

  if( keys.empty() )
    return;

  // Resize pixels container, if needed.
  if( keys.size()!=pixels.size() )
    pixels.resize( keys.size() );

  // Pick each layer.
  size_t i = 0;

  for( otb::GlView::StringVectorType::const_iterator it( keys.begin() );
       it != keys.end();
       ++ it, ++ i )
    {
    // Check layer key.
    pixels[ i ].m_Key = *it;

    // Get actor.
    otb::GlActor::Pointer actor( m_GlView->GetActor( *it ) );
    assert( !actor.IsNull() );

    // If image-actor...
    otb::GlImageActor::Pointer imageActor( otb::DynamicCast< otb::GlImageActor >( actor ) );

    if( !imageActor.IsNull() )
      {
      pixels[ i ].m_HasResolution = true;
      pixels[ i ].m_Resolution = imageActor->GetCurrentResolution();
      }
    else
      {
      pixels[ i ].m_HasResolution = false;
      }
    }
}

/*****************************************************************************/
bool
ImageViewRenderer
::TransformToView( PointType & point,
		   const StackedLayerModel::KeyType & key,
		   const IndexType & index,
		   bool isPhysical ) const
{
  assert( !m_GlView.IsNull() );
  assert( !key.empty() );

  otb::GlActor::Pointer actor( m_GlView->GetActor( key ) );
  assert( !actor.IsNull() );

  const otb::GeoInterface * geo =
    dynamic_cast< const otb::GeoInterface * >( actor.GetPointer() );

  if( geo==NULL )
    return false;

  point[ 0 ] = static_cast< double >( index[ 0 ] );
  point[ 1 ] = static_cast< double >( index[ 1 ] );

  return geo->TransformToViewport( point, point, isPhysical );
}

/*****************************************************************************/
void
ImageViewRenderer
::UpdateActors( const AbstractImageViewRenderer::RenderingContext * )
{
  // qDebug() << this << "::virtual_UpdateActors()";

  assert( !m_GlView.IsNull() );


  StackedLayerModel * stackedLayerModel = GetLayerStack();
  assert( stackedLayerModel!=NULL );

  /*
  otb::GlImageActor::Pointer refImageActor(
    GetReferenceActor< otb::GlImageActor >()
  );
  */

  for( StackedLayerModel::ConstIterator it( stackedLayerModel->Begin() );
       it!=stackedLayerModel->End();
       ++it )
    {
    assert( !it->first.empty() );
    assert( it->second!=NULL );

    if( m_GlView->ContainsActor( it->first ) )
      {
      if( it->second->inherits( VectorImageModel::staticMetaObject.className() ) )
        {
        //
        // Get vector image-model.
        VectorImageModel * vectorImageModel =
          dynamic_cast< VectorImageModel * >( it->second );

        assert( vectorImageModel!=NULL );

        //
        // Get vector image-model data.
        const VectorImageSettings & settings =
          vectorImageModel->GetSettings();

        const ImageProperties * properties =
          vectorImageModel->GetProperties();

        //
        // Get GL image-actor.
        assert(
          m_GlView->GetActor( it->first )==
          otb::DynamicCast< otb::GlImageActor >(
            m_GlView->GetActor( it->first )
          )
        );

        otb::GlImageActor::Pointer imageActor(
          otb::DynamicCast< otb::GlImageActor >(
            m_GlView->GetActor( it->first )
          )
        );
        assert( !imageActor.IsNull() );

        imageActor->SetVisible( vectorImageModel->IsVisible() );

        //
        // Apply color-setup.
        VectorImageSettings::ChannelVector channels;

        settings.GetSmartChannels( channels );

        imageActor->SetRedIdx( channels[ RGBW_CHANNEL_RED ] + 1 );
        imageActor->SetGreenIdx( channels[ RGBW_CHANNEL_GREEN ] + 1 );
        imageActor->SetBlueIdx( channels[ RGBW_CHANNEL_BLUE ] + 1 );

        //
        // Apply color-dynamics.
        //
        // Must use local variable to cast from T* to T::Pointer because
        // of ITK set/get macros...
        otb::FragmentShader::Pointer fragmentShader( imageActor->GetShader() );
        assert(
          fragmentShader==otb::DynamicCast< otb::StandardShader >( fragmentShader )
        );

        otb::StandardShader::Pointer shader(
          otb::DynamicCast< otb::StandardShader >( fragmentShader )
        );

        assert( !shader.IsNull() );

        if( settings.IsGrayscaleActivated() )
          {
          shader->SetMinRed( settings.GetLowIntensity( RGBW_CHANNEL_WHITE ) );
          shader->SetMaxRed( settings.GetHighIntensity(RGBW_CHANNEL_WHITE  ) );

          shader->SetMinGreen( settings.GetLowIntensity( RGBW_CHANNEL_WHITE ) );
          shader->SetMaxGreen( settings.GetHighIntensity(RGBW_CHANNEL_WHITE ) );

          shader->SetMinBlue( settings.GetLowIntensity( RGBW_CHANNEL_WHITE ) );
          shader->SetMaxBlue( settings.GetHighIntensity(RGBW_CHANNEL_WHITE ) );
          }
        else
          {
          shader->SetMinRed( settings.GetLowIntensity( RGBW_CHANNEL_RED ) );
          shader->SetMaxRed( settings.GetHighIntensity(RGBW_CHANNEL_RED  ) );

          shader->SetMinGreen( settings.GetLowIntensity( RGBW_CHANNEL_GREEN ) );
          shader->SetMaxGreen( settings.GetHighIntensity(RGBW_CHANNEL_GREEN ) );

          shader->SetMinBlue( settings.GetLowIntensity( RGBW_CHANNEL_BLUE ) );
          shader->SetMaxBlue( settings.GetHighIntensity(RGBW_CHANNEL_BLUE ) );
          }

        shader->SetGamma( settings.GetGamma() );

        if( properties==NULL )
          shader->SetUseNoData( false );

        else
          {
          shader->SetUseNoData( properties->IsNoDataEnabled() );
          shader->SetNoData( properties->GetNoData() );
          }

	//
	// Apply shader properties.
	//
	// qDebug()
	//   << "alpha:" << settings.GetAlpha()
	//   << "'" << it->first.c_str() << "'";
	shader->SetAlpha( settings.GetAlpha() );

	if( m_EffectsEnabled )
	  switch( settings.GetEffect() )
	    {
	    case EFFECT_CHESSBOARD:
	      shader->SetShaderType( otb::SHADER_ALPHA_GRID );
	      shader->SetChessboardSize( settings.GetSize() );
	      break;

	    case EFFECT_GRADIENT:
	      shader->SetShaderType( otb::SHADER_GRADIENT );
	      shader->SetRadius( settings.GetSize() );
	      break;

	    case EFFECT_LOCAL_CONTRAST:
	      shader->SetShaderType( otb::SHADER_LOCAL_CONTRAST );
	      shader->SetRadius( settings.GetSize() );
	      shader->SetLocalContrastRange(
#if 0
		settings.GetValue() *
		std::max(
		  std::max(
		    shader->GetMaxRed() - shader->GetMinRed(),
		    shader->GetMaxGreen() - shader->GetMinGreen()
		  ),
		  shader->GetMaxBlue() - shader->GetMinBlue()
		)
#else
		settings.GetValue()
#endif
	      );
	      break;

	    case EFFECT_LOCAL_TRANSLUCENCY:
	      shader->SetShaderType( otb::SHADER_LOCAL_ALPHA );
	      shader->SetRadius( settings.GetSize() );
	      break;

	    case EFFECT_NONE:
	    case EFFECT_NORMAL:
	      shader->SetShaderType( otb::SHADER_STANDARD );
	      break;

	    case EFFECT_SPECTRAL_ANGLE:
	      shader->SetShaderType( otb::SHADER_SPECTRAL_ANGLE );
	      shader->SetRadius( settings.GetSize() );
	      shader->SetSpectralAngleRange( settings.GetValue() );
	      break;

	    case EFFECT_SWIPE_H:
	      shader->SetShaderType( otb::SHADER_ALPHA_SLIDER );
	      shader->SetVerticalSlider( false );
	      break;

	    case EFFECT_SWIPE_V:
	      shader->SetShaderType( otb::SHADER_ALPHA_SLIDER );
	      shader->SetVerticalSlider( true );
	      break;

	    default:
	      assert( false && "Unhandled mvd::Effect value!" );
	      break;
	    }
        }
      //
      else
        {
        qDebug()
          << __FILE__ << ":" << __LINE__
          << ": Unhandled AbstractLayerModel derived class.";
        }
      }
    }

  m_GlView->SetRenderingOrder( stackedLayerModel->GetKeys(), false );
}

/*******************************************************************************/
void
ImageViewRenderer
::virtual_UpdateScene()
{
  // qDebug() << this << "::virtual_UpdateScene()";

  assert( !m_GlView.IsNull() );

  StackedLayerModel * stackedLayerModel = GetLayerStack();


  if( stackedLayerModel==NULL || stackedLayerModel->IsEmpty() )
    m_GlView->ClearActors();

  else
    {
      {
      otb::GlView::StringVectorType keys( m_GlView->GetActorsKeys() );

      for( otb::GlView::StringVectorType::const_iterator it( keys.begin() );
           it!=keys.end();
           ++it )
        if( !stackedLayerModel->Contains( *it ) )
          {
          // qDebug() << QString( "Removing image-actor '%1'..." ).arg( it->c_str() );

          m_GlView->RemoveActor( *it );
          }
      }


#if USE_REMOTE_DESKTOP_DISABLED_RENDERING
#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING

    for( StackedLayerModel::ConstIterator it( stackedLayerModel->Begin() );
         it!=stackedLayerModel->End();
         ++it )
      if( !m_GlView->ContainsActor( it->first ) )
        {
        assert( it->second!=NULL );

        if( it->second->inherits( VectorImageModel::staticMetaObject.className()))
          {
          otb::GlImageActor::Pointer glImageActor( otb::GlImageActor::New() );

          // Should all AbstractLayerModel have a ::GetFilename()
          // method?
          // -> Not sure: AbstractImageModel coud be derived as a
          // in-memory image-model.
          VectorImageModel * vectorImageModel =
            dynamic_cast< VectorImageModel * >( it->second );

	  // Coverity-19839
	  // {
	  assert( vectorImageModel!=NULL );
	  // }

	  // qDebug()
	  //   << QString( "Adding image-actor from file '%1'..." )
	  //   .arg( vectorImageModel->GetFilename() );

          glImageActor->Initialize(
            ToStdString(
              vectorImageModel->GetFilename()
            )
          );

          m_GlView->AddActor( glImageActor, it->first );

          // glImageActor->SetVisible( vectorImageModel->IsVisible() );

          // qDebug() <<
	  //   QString( "Added image-actor '%1' from file '%2'" )
	  //   .arg( FromStdString( it->first ) )
	  //   .arg( vectorImageModel->GetFilename() );
          }
        else
          {
          assert( false && "Unhandled AbstractLayerModel derived type." );
          }
        }

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING
    }

  RefreshScene();
}

/*******************************************************************************/
void
ImageViewRenderer
::virtual_RefreshScene()
{
  // qDebug() << this << "::virtual_RefreshScene()";

  //
  // Get layer-stack.
  StackedLayerModel * stackedLayerModel = GetLayerStack();

  //
  // Check if empty.
  if( stackedLayerModel==NULL ||
      stackedLayerModel->IsEmpty() )
    {
    m_ReferencePair.first = NULL;
    m_ReferencePair.second = otb::GlActor::Pointer();

    // virtual_ClearProjection();

    // assert( !m_GlView.IsNull() );
    // assert( m_GlView->GetSettings() );

    // m_GlView->GetSettings()->SetUseProjection( false );

    // emit ClearProjectionRequired();

    return;
    }

  //
  // Check if non-projected mode.
  if( !stackedLayerModel->HasReference() )
    {
    m_ReferencePair.first = NULL;
    m_ReferencePair.second = otb::GlActor::Pointer();

    virtual_ClearProjection();

    assert( !m_GlView.IsNull() );
    assert( m_GlView->GetSettings() );

    m_GlView->GetSettings()->SetUseProjection( false );

    emit ClearProjectionRequired();

    return;
    }

  //
  // Otherwise, it's projected mode.
  assert( !m_GlView.IsNull() );
  assert( m_GlView->GetSettings()!=NULL );

  m_GlView->GetSettings()->SetUseProjection( true );


  //
  // Store reference-pair.
  ModelActorPair referencePair( m_ReferencePair );

  //
  // Remember first layer-model as projection reference.
  m_ReferencePair.first = stackedLayerModel->GetReference();

  StackedLayerModel::KeyType referenceKey(
    stackedLayerModel->GetKey(
      stackedLayerModel->GetReferenceIndex()
    )
  );

#if USE_REMOTE_DESKTOP_DISABLED_RENDERING
  m_ReferencePair.second = otb::GlActor::Pointer();

#else // USE_REMOTE_DESKTOP_DISABLED_RENDERING

  //
  // Remember reference actor.
  {
  otb::GlActor::Pointer glActor( m_GlView->GetActor( referenceKey ) );
  // assert( !glActor.IsNull() );

  assert( glActor==otb::DynamicCast< otb::GlImageActor >( glActor ) );
  m_ReferencePair.second = otb::DynamicCast< otb::GlImageActor >( glActor );
  // assert( !m_ReferencePair.second.IsNull() );
  }

  if( m_ReferencePair.first!=NULL )
    {
    if( referencePair.first==NULL )
      {
      virtual_SetProjection();

      emit SetProjectionRequired();
      }
    else if( m_ReferencePair.first!=referencePair.first )
      {
      virtual_UpdateProjection();

      emit UpdateProjectionRequired();
      }
    }

#endif // USE_REMOTE_DESKTOP_DISABLED_RENDERING
}

/*****************************************************************************/
bool
ImageViewRenderer
::Reproject( PointType & center,
             SpacingType & spacing,
             const PointType & vcenter,
             const SpacingType & vspacing ) const
{
  assert( !m_GlView.IsNull() );

  const StackedLayerModel * stackedLayerModel = GetLayerStack();

  if( stackedLayerModel==NULL )
    return false;

  return
    m_GlView->ReprojectFromView(
      center,
      spacing,
      stackedLayerModel->GetKey( stackedLayerModel->GetReferenceIndex() ),
      vcenter,
      vspacing
    );
}

/*****************************************************************************/
bool
ImageViewRenderer
::GetLayerDynamics( const StackedLayerModel::KeyType & key,
		    ParametersType & params,
		    bool isGlobal ) const
{
  assert( !m_GlView.IsNull() );

  // Get actor.
  otb::GlImageActor::Pointer imageActor(
    otb::DynamicCast< otb::GlImageActor >(
      m_GlView->GetActor( key )
    )
  );

  if( imageActor.IsNull() )
    return false;

  // Get dynamics.
  imageActor->AutoColorAdjustment(
    params[ 0 ], params[ 1 ],
    params[ 2 ], params[ 3 ],
    params[ 4 ], params[ 5 ],
    isGlobal,
    VectorImageModel::DEFAULT_LOD_SIZE,
    0.02,
    0.02
  );

  return true;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void
ImageViewRenderer
::UpdatePixelInfo( const QPoint & screen,
		   const PointType & /* view */,
		   const PixelInfo::Vector & pixels )
{
  // qDebug()
  //   << this << "::UpdatePixelInfo("
  //   << screen << ", [" << view[ 0 ] << ";" << view[ 1 ]
  //   << "] )";

  assert( !m_GlView.IsNull() );

  for( PixelInfo::Vector::const_iterator it( pixels.begin() );
       it != pixels.end();
       ++ it )
    {
    assert( !it->m_Key.empty() );

    //
    // Check GL-view.
    assert( m_GlView->ContainsActor( it->m_Key ) );

    //
    // Get GL image-actor.
    otb::GlImageActor::Pointer glImageActor(
      otb::DynamicCast< otb::GlImageActor >(
	m_GlView->GetActor( it->m_Key )
      )
    );

    if( !glImageActor.IsNull() )
      {
      //
      // Get shader.
      otb::FragmentShader::Pointer fshader( glImageActor->GetShader() );
      assert( !fshader.IsNull() );

      otb::StandardShader::Pointer shader(
        otb::DynamicCast< otb::StandardShader >(
          fshader
        )
      );

      assert( !shader.IsNull() );

      //
      // Update cursor position of shader.
      PointType p_screen;

      assert( m_GlView->GetSettings()!=NULL );

      p_screen[ 0 ] = screen.x();
      p_screen[ 1 ] =
        m_GlView->GetSettings()->GetViewportSize()[ 1 ] - screen.y();

      shader->SetCenter( p_screen );

      if( shader->GetShaderType()==otb::SHADER_ALPHA_SLIDER )
        shader->SetSliderPosition(
          p_screen[
            shader->GetVerticalSlider()
            ? 1
            : 0
          ]
        );

      //
      // Update pixel-info of shader.
      if( it->m_HasPixel )
	{
	assert( it->m_Pixel.Size()>0 );

	shader->SetCurrentRed( it->m_Pixel[ 0 ] );
	shader->SetCurrentGreen( it->m_Pixel[ 1 ] );
	shader->SetCurrentBlue( it->m_Pixel[ 2 ] );
	}
      }
    }
}

/******************************************************************************/
bool
ImageViewRenderer
::virtual_ZoomToRegion( const PointType & origin,
			const PointType & extent,
			PointType & center,
			SpacingType & spacing ) const
{
  assert( !m_GlView.IsNull() );

  // Use spacing of viewport as native spacing of reference layer
  // because it has already been set for projection.
  //
  // Same as of Ice-viewer.

  return
    m_GlView->ZoomToRegion(
      origin,
      extent,
      m_GlView->GetSettings()->GetSpacing(),
      center,
      spacing
    );
}

/******************************************************************************/
bool
ImageViewRenderer
::virtual_ZoomToExtent( PointType & center, SpacingType & spacing ) const
{
  assert( !m_GlView.IsNull() );

  // Use spacing of viewport as native spacing of reference layer
  // because it has already been set for projection.
  //
  // Same as of Ice-viewer.

  return
    m_GlView->ZoomToExtent(
      m_GlView->GetSettings()->GetSpacing(),
      center,
      spacing
    );
}

/******************************************************************************/
bool
ImageViewRenderer
::virtual_ZoomToLayer( const StackedLayerModel::KeyType & key,
		       PointType & center,
		       SpacingType & spacing ) const
{
  assert( !m_GlView.IsNull() );

  // Use spacing of viewport as native spacing of reference layer
  // because it has already been set for projection.
  //
  // Same as of Ice-viewer.

  return
    m_GlView->ZoomToLayer(
      key,
      m_GlView->GetSettings()->GetSpacing(),
      center,
      spacing
    );
}

/******************************************************************************/
bool
ImageViewRenderer
::virtual_ZoomToFull( const StackedLayerModel::KeyType & key,
		      PointType & center,
		      SpacingType & spacing ) const
{
  assert( !m_GlView.IsNull() );

  return m_GlView->ZoomToFull( key, center, spacing );
}

/******************************************************************************/
} // end namespace 'mvd'