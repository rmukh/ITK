/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkBoundingBox.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
#include "itkBoundingBox.h"
#include "itkNumericTraits.h"

ITK_NAMESPACE_BEGIN

/**
 * Print out the bounding box.
 */
template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
void
BoundingBox<TPointIdentifier , VPointDimension, TCoordRep, TPointsContainer >
::PrintSelf(std::ostream& os, Indent indent)
{
  Object::PrintSelf(os, indent);

  if ( m_Bounds )
    {
    os << indent << "Bounding Box: ( " ;
    for (int i=0; i<PointDimension; i++)
      {
      os << m_Bounds[2*i] << "," << m_Bounds[2*i+1] << " ";
      }
    os << " )" << std::endl;
    }
  else
    {
    os << indent << "Bounding Box not defined" << std::endl;
    }
}

/**
 * Access routine to set the points container.
 */
template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
void
BoundingBox<TPointIdentifier , VPointDimension, TCoordRep, TPointsContainer >
::SetPoints(PointsContainer* points)
{
  itkDebugMacro(<< this->GetClassName() << " (" << this
                << "): setting Points container to " << points);
  if(m_PointsContainer != points)
    {
    m_PointsContainer = points;
    this->Modified();
    }
}

/**
 * Access routine to get the points container.
 */
template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
BoundingBox<TPointIdentifier , VPointDimension, TCoordRep, TPointsContainer >::PointsContainerPointer
BoundingBox<TPointIdentifier , VPointDimension, TCoordRep, TPointsContainer >
::GetPoints(void)
{
  itkDebugMacro(<< this->GetClassName() << " (" << this
                << "): returning Points container of " << m_PointsContainer );

  return m_PointsContainer;
}

/******************************************************************************
 * PROTECTED METHOD DEFINITIONS
 *****************************************************************************/
template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
BoundingBox<TPointIdentifier , VPointDimension, TCoordRep, TPointsContainer >
::BoundingBox():
  m_PointsContainer(NULL),
  m_Bounds(NULL)
{
}

template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
BoundingBox<TPointIdentifier , VPointDimension, TCoordRep, TPointsContainer >
::~BoundingBox()
{
  if ( m_Bounds )
    {   
    delete [] m_Bounds;
    }
}

template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
bool  
BoundingBox<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>
::ComputeBoundingBox(void)
{
  if ( !m_PointsContainer )
    {
    return false;
    }

  if ( !m_Bounds || this->GetMTime() > m_BoundsMTime )
    {
    if ( !m_Bounds )
      {
      m_Bounds = new CoordRep[2*PointDimension];
      }
    
    //iterate over points determining min/max
    for (int i=0; i < PointDimension; i++)
      {
      m_Bounds[2*i] = NumericTraits<CoordRep>::max();
      m_Bounds[2*i+1] = NumericTraits<CoordRep>::min();
      }

    ConstIterator ii;
    m_PointsContainer->Begin();    

    m_BoundsMTime.Modified();
    }

  return true;
}

template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
BoundingBox<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>::CoordRep* 
BoundingBox<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>
::GetBoundingBox(CoordRep bounds[PointDimension*2])
{
  if ( this->ComputeBoundingBox() )
    {
    for (int i=0; i<PointDimension; i++)
      {
      bounds[2*i] = m_Bounds[2*i];
      bounds[2*i+1] = m_Bounds[2*i+1];
      }
    return bounds;
    }
  else
    {
    return NULL;
    }
}

template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
BoundingBox<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>::CoordRep*  
BoundingBox<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>
::GetCenter(CoordRep center[PointDimension*2])
{
  if ( this->ComputeBoundingBox() )
    {
    for (int i=0; i<PointDimension; i++)
      {
      center[i] = (m_Bounds[2*i] + m_Bounds[2*i+1]) / 2.0;
      }
    return center;
    }
  else
    {
    return NULL;
    }
}

template <typename TPointIdentifier, int VPointDimension,
          typename TCoordRep, typename TPointsContainer>
NumericTraits<BoundingBox<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>::CoordRep>::AccumulateType 
BoundingBox<TPointIdentifier,VPointDimension,TCoordRep,TPointsContainer>
::GetDiagonalLength2(void)
{
  NumericTraits<CoordRep>::AccumulateType dist2 = NumericTraits<CoordRep>::zero;

  if ( this->ComputeBoundingBox() )
    {
    for (int i=0; i<PointDimension; i++)
      {
      dist2 += (m_Bounds[2*i]-m_Bounds[2*i+1]) * (m_Bounds[2*i]-m_Bounds[2*i+1]);
      }
    }

  return dist2;
}

ITK_NAMESPACE_END
